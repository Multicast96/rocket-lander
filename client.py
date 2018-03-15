#
#   Hello World client in Python
#   Connects REQ socket to tcp://localhost:5555
#   Sends "Hello" to server, expects "World" back
#

import zmq
import time
import pygame
import sys

context = zmq.Context()

#  Socket to talk to server
print("Connecting to hello world server…")
socket = context.socket(zmq.REQ)
socket.connect("tcp://localhost:5555")

canvas = pygame.display.set_mode((480, 320))
a_pressed = False
d_pressed = False
while True:
	# Events handler
	for event in pygame.event.get():
		if event.type == pygame.QUIT:
			sys.exit(0)
	
	keys = pygame.key.get_pressed()
	
	msg = 0;
	if keys[pygame.K_LEFT]:
		msg += 1
	if keys[pygame.K_RIGHT]:
		msg += 2
	if keys[pygame.K_UP]:
		msg += 4
	
	if msg is not 0:
		socket.send_string(str(msg))
		message = socket.recv()
		print("Received reply %s "% message)
			
			
			
