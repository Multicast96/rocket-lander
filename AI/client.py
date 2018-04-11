import threading

import struct
import zmq
import time
import pygame
import sys
from enum import Enum


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

    def init_sim(self, rocket_count):
        self.scene_socket.send_string(Commands.SCENE_INIT.value)
        if self.scene_socket.recv().decode("ASCII") == Commands.OK.value:
            self.scene_socket.send_string(str(rocket_count))

    def rocket_controller(self, id):
        socket = self.context.socket(zmq.REQ)
        socket.connect("tcp://localhost:"+str(50000+id))
        i = 0
        bonus = 0
        while not self.terminate:
            socket.send_string(Commands.CONTROL.value + chr(bonus))
            message = socket.recv()
            position = struct.unpack('f', message[:4])[0]
            velocity = struct.unpack('f', message[4:])[0]
            bonus = position > 400
            i += 1
        socket.send_string(Commands.KILL.value + "a")

    def main_loop(self):
        population_count = 20
        self.init_sim(population_count)

        threads = [threading.Thread(target=self.rocket_controller, args=(i,)) for i in range(population_count)]

        for t in threads:
            t.start()

        results = self.scene_socket.recv()
        self.terminate = True
        print(results.decode("ASCII"))
        for t in threads:
            t.join()
        self.scene_socket.send_string(Commands.KILL.value)


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
