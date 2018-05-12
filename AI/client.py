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
    KILL = 'T'
    CONTROL = 'C'


class Manager:
    def __init__(self):
        self.context = zmq.Context()
        self.scene_socket = self.context.socket(zmq.REQ)
        self.scene_socket.connect("tcp://localhost:5555")
        self.terminate = False
        self.agent = Agent(3, 2) #inputs, outputs
        self.state = None

    def init_sim(self, rocket_count):
        self.scene_socket.send_string(Commands.SCENE_INIT.value)
        if self.scene_socket.recv().decode("ASCII") == Commands.OK.value:
            self.scene_socket.send_string(str(rocket_count))

    def rocket_controller(self, id):
        socket = self.context.socket(zmq.REQ)
        socket.connect("tcp://localhost:"+str(50000+id))
        next_state = np.array([[420.0, 0.0, 0.0]])
        while not self.terminate:
            self.state = next_state
            action = self.agent.act(self.state)
            socket.send_string(Commands.CONTROL.value + chr(action))

            message = socket.recv()
            height = struct.unpack('f', message[:4])[0]
            velocity = struct.unpack('f', message[4:8])[0]
            time_left = struct.unpack('f', message[8:])[0]
            next_state = np.array([[height, velocity, time_left]])

            self.agent.remember(self.state, action, reward(next_state), next_state, False)
        socket.send_string(Commands.KILL.value + "a")
        socket.close()

    def main_loop(self):
        population_count = 1
        simulation_count = 4
        for sim in range(simulation_count):
            self.init_sim(population_count)

            threads = [threading.Thread(target=self.rocket_controller, args=(i,)) for i in range(population_count)]

            for t in threads:
                t.start()

            results = self.scene_socket.recv()
            self.terminate = True
            print("cpp results:")
            print(results.decode("ASCII"))

            print("simulation: {}/{}, score: {}".format(sim, simulation_count, reward(self.state)))

            for t in threads:
                t.join()

            self.agent.replay(32)

        self.scene_socket.send_string(Commands.KILL.value)
        self.scene_socket.close()


m = Manager()
m.main_loop()

# canvas = pygame.display.set_mode((480, 320))
# a_pressed = False
# d_pressed = False
# while True:
#     # Events handler
#     for event in pygame.event.get():
#         if event.type == pygame.QUIT:
#             sys.exit(0)
#
#     keys = pygame.key.get_pressed()
#     socket.send_string(":(\n")
#     message = socket.recv()
#     print(message[0])

#	msg = 0;
#	if keys[pygame.K_LEFT]:
#		msg += 1
#	if keys[pygame.K_RIGHT]:
#		msg += 2
#	if keys[pygame.K_UP]:
#		msg += 4
#	
#	if msg is not 0:
#		socket.send_string(str(msg))
#		message = socket.recv()
#		print("Received reply %s "% message)
