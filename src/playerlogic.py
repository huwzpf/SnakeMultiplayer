import math


class PlayerLogic:
    def __init__(self, velocity):
        self._vel = velocity
        self._angle = 0
        self._da = 10
        self._left_pressed = False
        self._right_pressed = False
        self.cnt = 0
        self.cnt_2 = 0

    def get_angle(self):
        return self._angle

    def set_left_down(self):
        self._left_pressed = True

    def set_left_up(self):
        self._left_pressed = False

    def set_right_down(self):
        self._right_pressed = True

    def set_right_up(self):
        self._right_pressed = False

    def loop(self):
        draw = True
        if self.cnt == 0:
            if self._left_pressed:
                self._angle -= self._da
                if self._angle < 0:
                    self._angle += 360

            if self._right_pressed:
                self._angle += self._da
                if self._angle > 360:
                    self._angle -= 360

        self.cnt += 1
        self.cnt_2 += 1
        if self.cnt == 11:
            self.cnt = 0
        if self.cnt_2 == 20000:
            self.cnt_2 = 0
        if self.cnt_2 > 17000:
            draw = False
        dx = self._vel * math.cos(math.radians(self._angle))
        dy = self._vel * math.sin(math.radians(self._angle))
        return draw, dx, dy