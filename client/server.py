import socket
from data_types import *


class Server:
    def __init__(self, ip):
        self.init_data = None

        self._port = 2137
        self._host = ip

        self._address = (self._host, self._port)
        self._buffer_size = 1024

        self._format = "utf-8"
        self._confirm_msg = "connected\0".encode(self._format)

        self._update = None
        self._prev_left_pressed = False
        self._prev_right_pressed = False
        self._left_pressed = False
        self._right_pressed = False

        self._client = self._client = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self._client.connect(self._address)

    # setters for player input
    def set_left_down(self):
        self._left_pressed = True

    def set_left_up(self):
        self._left_pressed = False

    def set_right_down(self):
        self._right_pressed = True

    def set_right_up(self):
        self._right_pressed = False

    # getters for runtime data
    def get_init_data(self):
        return self.init_data

    def get_update(self, id):
        for x in self._update.players_data:
            if x.ID == id:
                return x

    def get_bonuses(self):
        ret = []
        for x in self._update.bonuses:
            if x.active:
                ret.append(x)
        return ret

    def connection_lobby(self, show_lobby, refresh):
        data = self._client.recv(9).decode(self._format)
        current = 0
        target = 1
        while current < target:
            self._client.send(self._confirm_msg)
            data = self._client.recv(8)
            current, target = struct.unpack("<ii", data)
            show_lobby(current, target)
            refresh()
        self._client.settimeout(None)

    def start_game(self):
        data = self._client.recv(self._buffer_size)
        self.init_data = InitData(data)
        self._client.send(self._confirm_msg)

    def loop(self):

        move_l = KeyState.NO_CHANGE
        move_r = KeyState.NO_CHANGE
        if self._prev_left_pressed != self._left_pressed:
            if self._prev_left_pressed:
                move_l = KeyState.UP
            else:
                move_l = KeyState.DOWN
        if self._prev_right_pressed != self._right_pressed:
            if self._prev_right_pressed:
                move_r = KeyState.UP
            else:
                move_r = KeyState.DOWN
        """
        int left_pressed
        int right_pressed
        """
        move = struct.pack("<ii", move_l.value, move_r.value)
        self._client.send(move)
        data = self._client.recv(self._buffer_size)

        self._update = StateUpdate(data, self.init_data.player_count)
        self._prev_left_pressed = self._left_pressed
        self._prev_right_pressed = self._right_pressed
        return self._update.running

    def recive_leaderboard(self):
        """
        int leaderboard[players_count]
        """
        data_format = "<"
        for i in range(self.init_data.player_count):
            data_format += "i"
        data = self._client.recv(self.init_data.player_count*4)
        leaderboard = struct.unpack(data_format, data)
        return leaderboard



