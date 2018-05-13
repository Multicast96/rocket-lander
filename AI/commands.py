from enum import Enum

class Commands(Enum):
    SCENE_INIT = 'I'
    OK = 'K'
    ERROR = 'E'
    SIMULATION_DONE = 'D'
    WAITING_FOR_RESULTS = 'W'
    KILL = 'T'
    CONTROL = 'C'

    LANDING = 'L'
    BOOM = 'B'
    FLYING = 'F'
