import torch
import torch.nn as nn
import torch.optim as optim
import numpy as np
import torch.nn.functional as F
import math
from AdaBelief import AdaBelief

class LearningRate():
    def __init__(self, initial_lr, iteration_type):
        self.initial_lr = initial_lr
        self.iteration_type = iteration_type #epoch or mini_batch

    def get_learning_rate(self, optimizer):
        return optimizer.param_groups[0]['lr']

    def set_learning_rate(self, optimizer, new_lr):
        for param_group in optimizer.param_groups:
            param_group['lr'] = new_lr

    def adjust(self, optimizer, lr, iteration, params=None):
        self.set_learning_rate(optimizer, lr)
        return lr

class SnapshotLR(LearningRate):
    '''https://arxiv.org/abs/1704.00109'''
    def __init__(self, initial_lr, iteration_type,
                 max_lr, total_iters, n_cycles):
        '''
        n_iters = total number of mini-batch iterations during training
        n_cycles = total num snapshots during training
        max_lr = starting learning rate each cycle'''
        super().__init__(initial_lr, iteration_type)
        self.max_lr = max_lr
        self.total_iters = total_iters
        self.cycles = n_cycles
        self.iters_per_cycle = total_iters // n_cycles

    def cosine_annealing(self, t):
        '''t = current mini-batch iteration'''
        # return self.max_lr/2 * (math.cos(
        #  (math.pi * (t % (self.total_iters//self.cycles))) /
        #  (self.total_iters//self.cycles)) + 1)
        return 0.5 * self.max_lr * (math.cos((math.pi * (t % self.iters_per_cycle)) / self.iters_per_cycle) + 1)

    def adjust(self, optimizer, iteration, params=None):
        new_lr = self.cosine_annealing(iteration)
        self.set_learning_rate(optimizer, new_lr)
        return new_lr

def weights_init_uniform(m):
    classname = m.__class__.__name__
    # for every Linear layer in a model..
    if classname.find('Linear') != -1:
        # apply a uniform distribution to the weights and a bias=0
        nn.init.xavier_uniform_(m)
        # m.weight.data.uniform_(0.0, 1.0)
        # m.bias.data.fill_(0)

def get_n_params(module):
    return sum(p.numel() for p in module.parameters() if p.requires_grad)

class Attention(nn.Module):
    def __init__(self, embed_dim, n_heads=12, qkv_bias=True, attn_p=0., proj_p=0.):
        super().__init__()
        self.n_heads = n_heads
        self.embed_dim = embed_dim
        self.head_dim = embed_dim // n_heads
        self.scale = self.head_dim ** -0.5

        self.qkv = nn.Linear(embed_dim, embed_dim * 3, bias=qkv_bias)
        self.attn_drop = nn.Dropout(attn_p)
        self.proj = nn.Linear(embed_dim, embed_dim)
        self.proj_drop = nn.Dropout(proj_p)

    def forward(self, x):
        batch_size, n_tokens, embed_dim = x.shape

        if embed_dim != self.embed_dim:
            raise ValueError

        qkv = self.qkv(x)
        qkv = qkv.reshape(batch_size, n_tokens, 3, self.n_heads, self.head_dim)
        qkv = qkv.permute(2, 0, 3, 1, 4) # (3, batch_size, self.n_heads, n_tokens, self.head_dim)

        q, k, v = qkv[0], qkv[1], qkv[2]
        k_t = k.transpose(-2, -1)
        dp = (q @ k_t) * self.scale
        attn = dp.softmax(dim=-1)
        attn = self.attn_drop(attn)

        weighted_avg = attn @ v
        weighted_avg = weighted_avg.transpose(1, 2)
        weighted_avg = weighted_avg.flatten(2)
        x = self.proj(weighted_avg)
        x = self.proj_drop(x)

        return x


class MLP(nn.Module):
    def __init__(self, in_features, hidden_dim, out_features, p = 0.):
        super().__init__()
        self.fc1 = nn.Linear(in_features, hidden_dim)
        self.act = nn.GELU()
        self.fc2 = nn.Linear(hidden_dim, out_features)
        self.drop = nn.Dropout(p)

    def forward(self, x):
        x = self.fc1(x)
        x = self.act(x) 
        x = self.drop(x) 
        x = self.fc2(x) 
        x = self.drop(x) 

        return x


class Block(nn.Module):
    def __init__(self, embed_dim, n_heads, qkv_bias = True, mlp_ratio = 4.0, attn_p = 0., proj_p = 0.):
        super().__init__()
        self.norm1 = nn.LayerNorm(embed_dim, eps = 1e-6)
        self.attn = Attention(embed_dim, n_heads = n_heads, qkv_bias = qkv_bias, attn_p = attn_p, proj_p = proj_p)
        self.norm2 = nn.LayerNorm(embed_dim, eps = 1e-6)
        hidden_dim = int(embed_dim * mlp_ratio)
        self.mlp = MLP(in_features = embed_dim, hidden_dim = hidden_dim, out_features = embed_dim)

    def forward(self, x):
        x = x + self.attn(self.norm1(x))
        x = x + self.mlp(self.norm2(x))

        return x

class transformer_DQNetwork(nn.Module):
    def __init__(self, lr, input_channels, embed_dim, n_patches, pos_p, n_heads, qkv_bias, mlp_ratio, attn_p, proj_p, depth, n_games, n_actions):
        super(transformer_DQNetwork, self).__init__()
        self.input_channels = input_channels
        self.cls_token = nn.Parameter(torch.zeros(1, 1, embed_dim))
        self.pos_embed = nn.Parameter(torch.zeros(1, n_patches + 1, embed_dim))

        self.input_embed = nn.Linear(input_channels, embed_dim)
        self.pos_drop = nn.Dropout(p = pos_p)
        self.blocks = nn.ModuleList([
                        Block(embed_dim = embed_dim, n_heads = n_heads, qkv_bias = qkv_bias, mlp_ratio = mlp_ratio, attn_p = attn_p, proj_p = proj_p) for _ in range(depth)
                      ])
        self.norm = nn.LayerNorm(embed_dim, eps=1e-6)
        self.head = nn.Linear(embed_dim, n_actions)

        self.lr_adjuster = SnapshotLR(initial_lr = lr, iteration_type = 'mini_batch', max_lr = lr, total_iters = n_games * 100, n_cycles = n_games)
        self.optimizer = optim.Adam(self.parameters(), lr = lr, weight_decay = 1e-5, amsgrad = True)
        self.lr_scheduler = optim.lr_scheduler.StepLR(self.optimizer, step_size = n_games // 2, gamma = 0.1)
        self.mseloss = nn.MSELoss()
        self.device = torch.device('cuda' if torch.cuda.is_available() else 'cpu')
        self.to(self.device)

    def forward(self, state):
        batch_size = state.shape[0]
        x = self.input_embed(state).unsqueeze(dim = 1)

        cls_token = self.cls_token.expand(batch_size, -1, -1)
        x = torch.cat([cls_token, x], dim = 1)
        # x = x + self.pos_embed
        # x = self.pos_drop(x)

        for block in self.blocks:
            x = block(x)

        x = self.norm(x)
        cls_token_final = x[:, 0]
        x = self.head(cls_token_final)
        return x

class Agent():
    def __init__(self, gamma, epsilon, lr, input_channels, batch_size, n_games, n_actions,
                 eps_end, eps_dec = 5e-5, max_mem_size = 10000000):
        self.gamma = gamma
        self.epsilon = epsilon
        self.eps_end = eps_end
        self.eps_dec = eps_dec
        self.lr = lr
        self.action_space = [i for i in range(n_actions)]
        self.mem_size = max_mem_size
        self.batch_size = batch_size
        self.mem_counter = 0
        self.transformerQ_eval = transformer_DQNetwork(lr = lr, input_channels = input_channels, embed_dim = 512, n_patches = 1, 
                                                       pos_p = 0., n_heads = 4, qkv_bias = True, mlp_ratio = 4.0, attn_p = 0., proj_p = 0.,
                                                       depth = 7, n_games = n_games, n_actions = n_actions)

        self.state_mem = np.zeros((self.mem_size, input_channels), dtype = np.float32)
        self.new_state_mem = np.zeros((self.mem_size, input_channels), dtype = np.float32)
        self.action_mem = np.zeros(self.mem_size, dtype = np.int32)
        self.reward_mem = np.zeros(self.mem_size, dtype = np.float32)
        self.terminal_mem = np.zeros(self.mem_size, dtype = np.bool)
        weights_init_uniform(self.transformerQ_eval)
        self.parameters = get_n_params(self.transformerQ_eval)
        print('parameters: ', self.parameters)
        print('model size: ', self.parameters / 1024 / 1024 / 4, ' MB')

    def store_transition(self, state, action, reward, state_, done):
        index = self.mem_counter % self.mem_size
        self.state_mem[index] = state
        self.new_state_mem[index] = state_
        self.reward_mem[index] = reward
        self.action_mem[index] = action
        self.terminal_mem[index] = done
        self.mem_counter += 1

    def choose_action(self, observation):
        if np.random.random() > self.epsilon:
            state = torch.tensor([observation]).to(self.transformerQ_eval.device)
            actions = self.transformerQ_eval.forward(state)
            action = torch.argmax(actions).item()
        else:
            action = np.random.choice(self.action_space)

        return action

    def learn(self, epoch, count):
        if self.mem_counter < self.batch_size:
            return
        self.transformerQ_eval.optimizer.zero_grad()
        max_mem = min(self.mem_counter, self.mem_size)
        batch = np.random.choice(max_mem, self.batch_size, replace = False)
        batch_index = np.arange(self.batch_size, dtype = np.int32)

        device = self.transformerQ_eval.device
        state_batch = torch.tensor(self.state_mem[batch]).to(device)
        new_state_batch = torch.tensor(self.new_state_mem[batch]).to(device)
        reward_batch = torch.tensor(self.reward_mem[batch]).to(device)
        terminal_batch = torch.tensor(self.terminal_mem[batch]).to(device)

        action_batch = self.action_mem[batch]
        transformerQ_eval = self.transformerQ_eval.forward(state_batch)[batch_index, action_batch]
        transformerQ_next = self.transformerQ_eval.forward(new_state_batch)
        transformerQ_next[terminal_batch] = 0.0
        transformerQ_target = reward_batch + self.gamma * torch.max(transformerQ_next) - transformerQ_eval
        loss = self.transformerQ_eval.mseloss(transformerQ_target, transformerQ_eval).to(device)
        loss.backward()
        self.transformerQ_eval.optimizer.step()
        cur_iter = int((epoch - 1) * 100) + count
        if self.transformerQ_eval.lr_adjuster.iteration_type == 'mini_batch':
            self.transformerQ_eval.lr_adjuster.adjust(self.transformerQ_eval.optimizer, cur_iter)
        self.epsilon = self.epsilon - self.eps_dec if self.epsilon > self.eps_end else self.eps_end

