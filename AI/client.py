import threading

import struct
import zmq
import time
#import pygame
import sys
from enum import Enum

from ai import *


class Commands(Enum):
    SCENE_INIT = 'I'
    OK = 'K'
    ERROR = 'E'
    SIMULATION_DONE = 'D'
    WAITING_FOR_RESULTS = 'W'
    KILL = 'T'
    CONTROL = 'C'


class Manager:
    def __init__(self, pop_count):
        self.pop_count = pop_count
        self.context = zmq.Context()
        self.scene_socket = self.context.socket(zmq.PAIR)
        self.scene_socket.connect("tcp://192.168.0.11:5555")
        self.terminate = False
        self.agent = Agent(3, 2, pop_count) #inputs, outputs

    def init_sim(self, rocket_count):
        self.scene_socket.send_string(Commands.SCENE_INIT.value)
        if self.scene_socket.recv().decode("ASCII") == Commands.OK.value:
            self.scene_socket.send_string(str(rocket_count))
        if self.scene_socket.recv().decode("ASCII") != Commands.OK.value:
            print("och")

    def rocket_controller(self, id, sim):
        socket = self.context.socket(zmq.PAIR)
        socket.connect("tcp://192.168.0.11:" + str(50000 + id))
        next_state = np.array([[420.0, 0.0, 10.0]])
        while not self.terminate:
            state = next_state
            action = self.agent.act(state)
            socket.send_string(Commands.CONTROL.value + chr(action))

            message = socket.recv()
            height = struct.unpack('f', message[:4])[0]
            velocity = struct.unpack('f', message[4:8])[0]
            time_left = struct.unpack('f', message[8:])[0]
            next_state = np.array([[height, velocity, time_left]])

            if next_state[0][2] == -1.0:
                self.agent.remember(id, state, action, reward(next_state), next_state, True)
                break

            self.agent.remember(id, state, action, reward(next_state), next_state, False)

        self.agent.save_result(id, reward(state))
        socket.send_string(Commands.KILL.value + "K")
        socket.close()

    def main_loop(self):
        simulation_count = 1000
        for sim in range(simulation_count):
            self.terminate = False
            self.init_sim(self.pop_count)

            threads = [threading.Thread(target=self.rocket_controller, args=(i, sim)) for i in range(self.pop_count)]

            print("simulation {}/{}...".format(sim + 1, simulation_count))

            for t in threads:
                t.start()

            self.scene_socket.send_string(Commands.WAITING_FOR_RESULTS.value)
            results = self.scene_socket.recv()
            self.terminate = True

            for t in threads:
                t.join()

            print("best rocket: {}\n".format(max(self.agent.results)))
            self.agent.choose_memories()
            self.agent.replay(300)

        self.scene_socket.send_string(Commands.KILL.value)
        self.scene_socket.close()


m = Manager(10)
m.main_loop()
