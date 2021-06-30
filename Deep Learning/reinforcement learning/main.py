import gym
from DeepQNetwork import Agent
# from utils import plot_learning_curve
import numpy as np

if __name__ == '__main__':
	env = gym.make('LunarLander-v2')
	n_games = 2000
	agent = Agent(gamma = 0.99, epsilon = 0.5, batch_size = 8192, n_actions = 4,
				  eps_end = 0.01, input_channels = 8, lr = 0.0001, n_games = n_games)
	scores, eps_histroy = [], []
	for i in range(1, n_games + 1):
		score = 0
		done = False
		observation = env.reset()
		count = 0
		while not done:
			count += 1
			action = agent.choose_action(observation)
			observation_, reward, done, info = env.step(action)
			score += reward
			agent.store_transition(observation, action, reward, observation_, done)
			agent.learn(i, count)
			observation = observation_
			if i % 50 == 0:
				still_open = env.render()
				if still_open == False: break
		agent.Q_eval.lr_scheduler.step()
		if i % (n_games // 3) == 0:
			agent.Q_eval.lr_adjuster.max_lr *= 0.1
		scores.append(score)
		eps_histroy.append(agent.epsilon)

		avg_score = np.mean(scores[-100:])
		print('epoch ', i, 'score %.2f' % score,
			  'average score %.2f' % avg_score,
			   'epsilon %.2f' % agent.epsilon)
		
	# x = [i + 1 for i in range(n_games)]
	# filename = 'lunar_lamder.png'
	# plot_learning_curve(x, scores, eps_histroy, filename)