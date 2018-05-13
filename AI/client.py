import threading

import struct
import zmq
import time
# import pygame
import sys
import keyboard
import operator

from ai import *
from commands import Commands


class Manager:
    def __init__(self, pop_count):
        self.pop_count = pop_count
        self.context = zmq.Context()
        self.scene_socket = self.context.socket(zmq.PAIR)
        self.scene_socket.connect("tcp://192.168.0.11:5555")
        self.terminate = False
        self.agent = Agent(3, 2, pop_count) # inputs, outputs

    def init_sim(self, rocket_count):
        self.scene_socket.send_string(Commands.SCENE_INIT.value)
        if self.scene_socket.recv().decode("ASCII") == Commands.OK.value:
            self.scene_socket.send_string(str(rocket_count))
        if self.scene_socket.recv().decode("ASCII") != Commands.OK.value:
            print("och")

    def rocket_controller(self, id):
        socket = self.context.socket(zmq.PAIR)
        socket.connect("tcp://192.168.0.11:" + str(50000 + id))

        message = socket.recv()
        status = struct.unpack('c', message[:1])[0].decode("ASCII")
        height = struct.unpack('f', message[1:5])[0]
        velocity = struct.unpack('f', message[5:9])[0]
        time_left = struct.unpack('f', message[9:])[0]
        next_state = np.array([[height, velocity, time_left]])

        # pakietas = 0

        while (not self.terminate) and (status == Commands.FLYING.value):
            state = next_state
            action = self.agent.act(state)
            socket.send_string(Commands.CONTROL.value + chr(action))

            message = socket.recv()
            status = struct.unpack('c', message[:1])[0].decode("ASCII")
            height = struct.unpack('f', message[1:5])[0]
            velocity = struct.unpack('f', message[5:9])[0]
            time_left = struct.unpack('f', message[9:])[0]
            next_state = np.array([[height, velocity, time_left]])

            self.agent.remember(id, state, action, reward(next_state, status), next_state, False)

            # pakietas += 1

        self.agent.pop_memory[id].pop()
        self.agent.remember(id, state, action, reward(next_state, status), next_state, True)
        self.agent.save_result(id, reward(state, status))
        # print(pakietas)
        socket.send_string(Commands.KILL.value + "K")
        socket.close()

    def main_loop(self):
        simulation_count = 1000
        # for sim in range(simulation_count):
        sim = 0
        while(True):
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

            print("best rocket ({}):\t{}\nworst rocket ({}):\t{}\navg rocket:\t\t\t{}".format(
                *max(enumerate(self.agent.results), key=operator.itemgetter(1)),
                *min(enumerate(self.agent.results), key=operator.itemgetter(1)),
                sum(self.agent.results)/self.pop_count))

            self.agent.choose_memories()
            self.agent.replay(2000)

            print("learned.\n")

            sim += 1

        self.scene_socket.send_string(Commands.KILL.value)
        self.scene_socket.close()


m = Manager(10)
m.main_loop()
