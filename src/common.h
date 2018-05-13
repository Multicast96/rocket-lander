#pragma once
#pragma warning( disable : 4091 ) 
#pragma warning( disable : 4244 ) 
#pragma warning( disable : 4305 ) 
//ostrze¿enia o konwersji float <-> double

#include<vector>
#include<list>
#include<iostream>
#include<thread>
#include<string>
#include<future>
#include<chrono>
#include<SFML\Graphics.hpp>

#define DEBUGINHO false

enum Fonts {
	MAIN,
	SPACE,
};

enum Commands {
	SCENE_INIT = 'I',
	OK = 'K',
	ERROR_COM = 'E',
	SIMULATION_DONE = 'D',
	WAITING_FOR_RESULTS = 'W',
	KILL = 'T',
	CONTROL = 'C',
	LANDING = 'L',
	BOOM = 'B',
	FLIGHT = 'F' //flight in progres
};