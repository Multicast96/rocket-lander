import threading

import struct
import zmq
import time
# import pygame
import sys
import keyboard
import operator
from copy import deepcopy

from ai import *
from commands import Commands


class Manager:
    def __init__(self, pop_count):
        self.pop_count = pop_count
        self.context = zmq.Context()
        self.scene_socket = self.context.socket(zmq.PAIR)
        self.scene_socket.connect("tcp://localhost:5555")
        self.terminate = False
        self.agent = Agent(3, 2, pop_count)     # inputs, outputs, pop_count
        self.agent_x = Agent(2, 2, pop_count)

    def init_sim(self, rocket_count):
        self.scene_socket.send_string(Commands.SCENE_INIT.value)
        if self.scene_socket.recv().decode("ASCII") == Commands.OK.value:
            self.scene_socket.send_string(str(rocket_count))
        if self.scene_socket.recv().decode("ASCII") != Commands.OK.value:
            print("och")

    def decode_message(self, message):
        status = struct.unpack('c', message[:1])[0].decode("ASCII")
        height = struct.unpack('f', message[1:5])[0]
        velocity = struct.unpack('f', message[5:9])[0]
        time_left = struct.unpack('f', message[9:13])[0]
        x_distance = struct.unpack('f', message[13:])[0]
        return status, np.array([[height, velocity, time_left]]), np.array([[x_distance, time_left]])

    def rocket_controller(self, id):
        socket = self.context.socket(zmq.PAIR)
        socket.connect("tcp://localhost:" + str(60000 + id))

        message = socket.recv()
        status, next_state, next_state_x = self.decode_message(message)

        # pakietas = 0

        while (not self.terminate) and (status == Commands.FLYING.value):
            state = next_state
            state_x = next_state_x
            action = self.agent.act(state)
            action_x = self.agent_x.act(state_x)
            socket.send_string(Commands.CONTROL.value + chr(action + 2*action_x))

            message = socket.recv()
            status, next_state, next_state_x = self.decode_message(message)

            self.agent.remember(id, state, action, reward(next_state, status), next_state, False)
            self.agent_x.remember(id, state_x, action_x, reward_x(next_state_x), next_state_x, False)

            # pakietas += 1

        # zamiana oststniego done na True
        if self.terminate:
            self.agent_x.pop_memory[id].pop()
            self.agent_x.remember(id, state_x, action_x, reward_x(next_state_x), next_state_x, True)
        self.agent.pop_memory[id].pop()
        self.agent.remember(id, state, action, reward(next_state, status), next_state, True)

        self.agent.save_result(id, reward(state, status))
        self.agent_x.save_result(id, reward_x(state_x))
        # print(pakietas)
        socket.send_string(Commands.KILL.value + "K")
        socket.close()

    def main_loop(self):
        simulation_count = 1000
        # for sim in range(simulation_count):
        sim = 0
        while True:
            self.terminate = False
            self.init_sim(self.pop_count)

            threads = [threading.Thread(target=self.rocket_controller, args=(i,)) for i in range(self.pop_count)]

            print("simulation {}/{}".format(sim + 1, simulation_count))

            for t in threads:
                t.start()

            print("simulating...")

            self.scene_socket.send_string(Commands.WAITING_FOR_RESULTS.value)
            # results =
            self.scene_socket.recv()
            self.terminate = True

            print("finished.")

            for t in threads:
                t.join()

            if keyboard.is_pressed('q'):
                break

            avg = sum(self.agent.results)/self.pop_count

            print("Agents:\nbest rocket ({}):\t{}\nworst rocket ({}):\t{}\navg rocket:\t\t\t{}".format(
                *max(enumerate(self.agent.results), key=operator.itemgetter(1)),
                *min(enumerate(self.agent.results), key=operator.itemgetter(1)),
                avg))

            if avg >= self.agent.best_average:
                print("\t NEW BEST")
                self.agent.best_memory = deepcopy(self.agent.pop_memory)
                self.agent.best_average = avg

            avg = sum(self.agent_x.results) / self.pop_count

            print("Agent_xs:\nbest rocket ({}):\t{}\nworst rocket ({}):\t{}\navg rocket:\t\t\t{}".format(
                *max(enumerate(self.agent_x.results), key=operator.itemgetter(1)),
                *min(enumerate(self.agent_x.results), key=operator.itemgetter(1)),
                avg))

            if avg >= self.agent_x.best_average:
                print("\t NEW BEST")
                self.agent_x.best_memory = deepcopy(self.agent_x.pop_memory)
                self.agent_x.best_average = avg

            self.agent.choose_memories()
            self.agent_x.choose_memories()
            self.agent.replay(2000)
            print("learned.")
            self.agent_x.replay(400)
            print("learned x.")

            sim += 1

        self.scene_socket.send_string(Commands.KILL.value)
        self.scene_socket.close()


m = Manager(10)
m.main_loop()
