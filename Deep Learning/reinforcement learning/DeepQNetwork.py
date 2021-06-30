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

class DeepQNetwork(nn.Module):
	def __init__(self, lr, input_channels, fc_dims, n_actions, n_games):
		super(DeepQNetwork, self).__init__()
		self.input_channels = input_channels
		self.fc_dims = fc_dims
		self.n_actions = n_actions

		self.fc1 = nn.Linear(input_channels, fc_dims[0])
		self.fc2 = nn.Linear(fc_dims[0], fc_dims[1])
		self.fc3 = nn.Linear(fc_dims[1], fc_dims[2])
		self.fc4 = nn.Linear(fc_dims[2], fc_dims[3])
		self.fc5 = nn.Linear(fc_dims[3], n_actions)
		self.activate = nn.ReLU()
		self.lr_adjuster = SnapshotLR(initial_lr = lr, iteration_type = 'mini_batch', max_lr = lr, total_iters = n_games * 100, n_cycles = n_games)
		self.optimizer = optim.Adam(self.parameters(), lr = lr, weight_decay = 1e-5, amsgrad = True)
		self.lr_scheduler = optim.lr_scheduler.StepLR(self.optimizer, step_size = n_games // 3, gamma = 0.1)
		self.mseloss = nn.MSELoss()
		self.device = torch.device('cuda' if torch.cuda.is_available() else 'cpu')
		self.to(self.device)

	def forward(self, state):
		x = self.activate(self.fc1(state))
		x = self.activate(self.fc2(x))
		x = self.activate(self.fc3(x))
		x = self.activate(self.fc4(x))
		actions = self.fc5(x)
		return actions

class Agent():
	def __init__(self, gamma, epsilon, lr, input_channels, batch_size, n_actions, n_games,
				max_mem_size = 10000000, eps_end = 0.01, eps_dec = 5e-5):
		self.gamma = gamma
		self.epsilon = epsilon
		self.eps_end = eps_end
		self.eps_dec = eps_dec
		self.lr = lr
		self.action_space = [i for i in range(n_actions)]
		self.mem_size = max_mem_size
		self.batch_size = batch_size
		self.mem_counter = 0
		self.Q_eval = DeepQNetwork(lr = lr, n_actions = n_actions, 
								input_channels = input_channels, fc_dims = [128, 256, 512, 1024], n_games = n_games)
		self.state_mem = np.zeros((self.mem_size, input_channels), dtype = np.float32)
		self.new_state_mem = np.zeros((self.mem_size, input_channels), dtype = np.float32)
		self.action_mem = np.zeros(self.mem_size, dtype = np.int32)
		self.reward_mem = np.zeros(self.mem_size, dtype = np.float32)
		self.terminal_mem = np.zeros(self.mem_size, dtype = np.bool)
		weights_init_uniform(self.Q_eval)

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
			state = torch.tensor([observation]).to(self.Q_eval.device)
			actions = self.Q_eval.forward(state)
			action = torch.argmax(actions).item()
		else:
			action = np.random.choice(self.action_space)

		return action

	def learn(self, epoch, count):
		if self.mem_counter < self.batch_size:
			return
		self.Q_eval.optimizer.zero_grad()
		max_mem = min(self.mem_counter, self.mem_size)
		batch = np.random.choice(max_mem, self.batch_size, replace = False)
		batch_index = np.arange(self.batch_size, dtype = np.int32)
		state_batch = torch.tensor(self.state_mem[batch]).to(self.Q_eval.device)
		new_state_batch = torch.tensor(self.new_state_mem[batch]).to(self.Q_eval.device)
		reward_batch = torch.tensor(self.reward_mem[batch]).to(self.Q_eval.device)
		terminal_batch = torch.tensor(self.terminal_mem[batch]).to(self.Q_eval.device)
		action_batch = self.action_mem[batch]
		q_eval = self.Q_eval.forward(state_batch)[batch_index, action_batch]
		q_next = self.Q_eval.forward(new_state_batch)
		q_next[terminal_batch] = 0.0
		q_target = reward_batch + self.gamma * torch.max(q_next) - q_eval
		loss = self.Q_eval.mseloss(q_target, q_eval).to(self.Q_eval.device)
		loss.backward()
		self.Q_eval.optimizer.step()
		cur_iter = int((epoch - 1) * 100) + count
		if self.Q_eval.lr_adjuster.iteration_type == 'mini_batch':
			self.Q_eval.lr_adjuster.adjust(self.Q_eval.optimizer, cur_iter)
		self.epsilon = self.epsilon - self.eps_dec if self.epsilon > self.eps_end else self.eps_end

