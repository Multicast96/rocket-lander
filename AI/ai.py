import tensorflow
import random
import numpy as np
from collections import deque
from keras.models import Sequential
from keras.layers import Dense
from keras.optimizers import Adam
from keras import backend as K

import math


class Agent:
    def __init__(self, state_size, action_size, pop_count):
        self.state_size = state_size
        self.action_size = action_size
        self.memory = deque(maxlen=2000)
        self.gamma = 0.95    # discount rate
        self.epsilon = 1.0  # exploration rate
        self.epsilon_min = 0.01
        self.epsilon_decay = 0.95
        self.learning_rate = 0.001
        self.model = self._build_model()
        self.target_model = self._build_model()
        self.update_target_model()

        self.pop_memory = [deque(maxlen=600) for _ in range(pop_count)]
        self.results = [None] * pop_count

    def _build_model(self):
        # Neural Net for Deep-Q learning Model
        model = Sequential()
        model.add(Dense(24, input_dim=self.state_size, activation='relu'))
        model.add(Dense(24, activation='relu'))
        model.add(Dense(self.action_size, activation='linear'))
        model.compile(loss='mse',
                      optimizer=Adam(lr=self.learning_rate))
        return model

    def update_target_model(self):
        # copy weights from model to target_model
        self.target_model.set_weights(self.model.get_weights())

    def remember(self, id, state, action, reward, next_state, done):
        self.pop_memory[id].append((state, action, reward, next_state, done))

    def act(self, state):
        if np.random.rand() <= self.epsilon:
            return random.randrange(self.action_size)
        act_values = self.model.predict(state)
        return np.argmax(act_values[0])  # returns action

    def replay(self, batch_size):
        minibatch = random.sample(self.memory, batch_size)
        for state, action, reward, next_state, done in minibatch:
            target = reward
            if not done:
                target = reward + self.gamma * \
                         np.amax(self.model.predict(next_state)[0])
            target_f = self.model.predict(state)
            target_f[0][action] = target
            self.model.fit(state, target_f, epochs=1, verbose=0)
        if self.epsilon > self.epsilon_min:
            self.epsilon *= self.epsilon_decay

    def save_result(self, id, result):
        self.results[id] = result

# wybór najlepszych doświadczeń z każdej iteracji
    def choose_memories(self):
        res_sum = sum(self.results)
        for i, r in enumerate(self.results):
            batch_size = min(int(2000*r/res_sum), 600)
            self.memory.extend(random.sample(self.pop_memory[i], batch_size))


# magic number
f1 = 0.118950730798063


def logsig(n):
    try:
        return 1/(1+math.exp(-n))
    except OverflowError:
        return 1


def reward(state):
    return (0.5 + state[0][2] * 0.05) * (1 - logsig(1 / (1 - state[0][0] / 2500) + 1 / (1 - abs(state[0][1] / 500)))) / f1