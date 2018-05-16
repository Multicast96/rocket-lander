import tensorflow
import random
import numpy as np
from collections import deque
from keras.models import Sequential
from keras.layers import Dense
from keras.optimizers import Adam
from keras import backend as K

import math
from scipy.special import expit

from commands import Commands


class Agent:
    def __init__(self, state_size, action_size, pop_count, presentation, name):
        self.pop_count = pop_count
        self.state_size = state_size
        self.action_size = action_size
        self.memory = deque(maxlen=24000)
        self.gamma = 0.95    # discount rate
        self.epsilon = 1.0  # exploration rate
        self.epsilon_min = 0.01
        self.epsilon_decay = 0.95
        self.learning_rate = 0.001
        self.model = self._build_model()
        self.target_model = self._build_model()
        self.update_target_model()

        self.pop_memory = [deque(maxlen=600)] * pop_count
        self.results = [None] * pop_count
        self.best_average = 0
        self.best_memory = [deque(maxlen=600)] * pop_count
        self.presentation = presentation

        if presentation:
            self.load(name)
            self.epsilon_decay = 0.0

    def _build_model(self):
        model = Sequential()
        model.add(Dense(24, input_dim=self.state_size, activation='relu'))
        model.add(Dense(24, activation='relu'))
        model.add(Dense(self.action_size, activation='linear'))
        model.compile(loss='mse', optimizer=Adam(lr=self.learning_rate))
        return model

    def update_target_model(self):
        # copy weights from model to target_model
        self.target_model.set_weights(self.model.get_weights())

    def load(self, name):
        self.model.load_weights(name)

    def save(self, name):
        self.model.save_weights(name)

    def remember(self, id, state, action, reward, next_state, done):
        self.pop_memory[id].append((state, action, reward, next_state, done))

    def act(self, state):
        if np.random.rand() <= self.epsilon:
            return random.randrange(self.action_size)
        act_values = self.model.predict(state)
        return np.argmax(act_values[0])  # returns action

    def replay(self, batch_size):

        batch_size = min(batch_size, len(self.memory))

        minibatch = random.sample(self.memory, batch_size)
        for state, action, reward, next_state, done in minibatch:
            target = reward
            if not done:
                target = reward + self.gamma * np.amax(self.model.predict(next_state)[0])
            target_f = self.model.predict(state)
            target_f[0][action] = target
            self.model.fit(state, target_f, epochs=1, verbose=0)
        if self.epsilon > self.epsilon_min:
            self.epsilon *= self.epsilon_decay

    def save_result(self, id, result):
        self.results[id] = result

    # wybór najlepszych doświadczeń z każdej iteracji
    def choose_memories(self):
        # res_sum = sum(self.results)
        # for i, r in enumerate(self.results):
        #     if r > 0.0:
        #         batch_size = min(int(2000*r/res_sum + 1), len(self.pop_memory[i]))
        #     else:
        #         batch_size = 200
        #     self.memory.extend(random.sample(self.pop_memory[i], batch_size))
        for pm in self.pop_memory:
            self.memory.extend(pm)
        for bm in self.best_memory:
            self.memory.extend(bm)

# magic number
# f1 = 0.047230125412280


h_max = 750
v_max = 300
t_max = 8

v_boom = 0.731058578630005

x_magic = -409601


# def logsig(n):
#     try:
#         return 1/(1+math.exp(-n))
#     except OverflowError:
#         return 1


def h_reward(h):
    return 1 - expit((h - h_max / 2)/(h_max / 6))


def v_reward(v):
    return 1 - expit((abs(v) - v_max / 2)/(v_max / 6))


def t_reward(t):
    return expit((t - t_max/2)/(t_max/6))


def reward(state, status):
    # height = min(state[0][0], 2999.99)
    # velocity = min(abs(state[0][1]), 499.99)
    # time_left = state[0][2]
    h_rew = h_reward(state[0][0])
    v_rew = v_reward(state[0][1])
    t_rew = t_reward(state[0][2])

    if status == Commands.FLYING.value:
        return h_rew*(1 + v_rew)
    elif status == Commands.BOOM.value:
        return 0.5 + v_rew
    else:
        return 2 + (v_rew - v_boom)/(1 - v_boom) + 0.5 * t_rew

    # if time_left != -1.0 and height == 0.0:
    #     return (0.5 + time_left * 0.05) + 1.0
    # else:
    #     return (1 - logsig(1 / (1 - height / 3000) + 2 / (1 - velocity / 500))) / f1


def reward_x(state):
    return state[0][0]**2 / x_magic + 1 + 0.5*t_reward(state[0][1])
