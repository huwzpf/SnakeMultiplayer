import struct
from enum import Enum, IntEnum


class KeyState(IntEnum):
    UP = 0,
    NO_CHANGE = 1,
    DOWN = 2


class COLOR(Enum):
    RED = (255, 0, 0)
    GREEN = (0, 255, 0)
    BLUE = (0, 0, 255)
    WHITE = (255, 255, 255)


class PlayerInitData:
    def __init__(self, *args):
        arg = args[0]
        self.ID = arg[0]
        self.color = [arg[1], arg[2], arg[3]]
        self.start_x = arg[4]
        self.start_y = arg[5]
        self.size = arg[6]


class InitData:
    def __init__(self, data):
        self.sess_id = struct.unpack("i", data[-4:])
        self.players_data = []
        self.player_count = 0
        """
        int ID
        int color[3]
        double start_x
        double start_y
        double size
        """
        it = struct.iter_unpack("<iiiiddd", data[:-4])
        while True:
            try:
                self.players_data.append(PlayerInitData(list(next(it))))
                self.player_count += 1
            except StopIteration:
                break


class PlayerState:
    def __init__(self, *args):
        arg = args[0]
        self.ID = arg[0]
        self.alive = arg[1]
        self.draw = arg[2]
        self.dx = arg[3]
        self.dy = arg[4]
        self.size = arg[5]
        self.bonus = arg[6]


class BonusState:
    def __init__(self, *args):
        arg = args[0]
        self.x = arg[0]
        self.y = arg[1]
        self.type = arg[2]
        self.active = arg[3]


class StateUpdate:
    def __init__(self, data, player_count):
        self.running = struct.unpack("<i", data[-4:])
        self.players_data = []
        self.bonuses = []
        """
        int id      4
        int alive   4
        int draw    4
        double dx   8
        double dy   8
        double size 8
        int bonus   4
        ----------------+
                    40 bytes -----------------------|                        
                                                    V
        """
        it_p = struct.iter_unpack("<iiidddi", data[:(40 * player_count)])
        """
        int x
        int y
        int type
        int active
        """
        it_b = struct.iter_unpack("<iiii", data[(40 * player_count):-4])
        while True:
            try:
                self.players_data.append(PlayerState(list(next(it_p))))
                self.bonuses.append(BonusState(list(next(it_b))))
            except StopIteration:
                break
