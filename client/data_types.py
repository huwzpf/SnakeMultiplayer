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
        self.ID = int.from_bytes(arg[0],"big")
        self.alive = int.from_bytes(arg[1],"big")
        self.draw = int.from_bytes(arg[2],"big")
        self.dx = arg[3]
        self.dy = arg[4]
        self.size = arg[5]
        self.bonus = int.from_bytes(arg[6],"big")


class BonusState:
    def __init__(self, *args):
        arg = args[0]
        self.x = arg[0]
        self.y = arg[1]
        self.type = int.from_bytes(arg[2],"big")
        self.active = int.from_bytes(arg[3],"big")


class StateUpdate:
    def __init__(self, data, player_count):
        self.running = int.from_bytes(struct.unpack("<c", data[-1:])[0],"big")
        self.players_data = []
        self.bonuses = []
        if self.running != 0:
            """
            char id      1
            char alive   1
            char draw    1
            float dx     4
            float dy     4
            float size   4
            char bonus   1
            ----------------+
                        16 bytes -----------------------|                        
                                                        V
            """
            it_p = struct.iter_unpack("<cccfffc", data[:(16 * player_count)])
            """
            unsigned short x
            unsigned short y
            char type
            char active
            """
            it_b = struct.iter_unpack("<HHcc", data[(16 * player_count):-1])
            while True:
                try:
                    self.players_data.append(PlayerState(list(next(it_p))))
                    self.bonuses.append(BonusState(list(next(it_b))))
                except StopIteration:
                    break
