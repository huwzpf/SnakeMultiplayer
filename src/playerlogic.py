import math


class PlayerLogic:
    def __init__(self, velocity, map_w, map_h, x, y, size):
        self._vel = velocity
        self._angle = 0
        self._da = 7
        self._left_pressed = False
        self._right_pressed = False
        self.cnt = 0
        self.current_x = x
        self.current_y = y
        self.map_w = map_w
        self.map_h = map_h
        self.size = size
        self._collision_arr = [[False for j in range(map_h + 1)] for i in range(map_w + 1)]

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
        self.cnt += 1
        draw = True
        if self.cnt % 2 == 0:
            if self._left_pressed:
                self._angle -= self._da
                if self._angle < 0:
                    self._angle += 360

            if self._right_pressed:
                self._angle += self._da
                if self._angle > 360:
                    self._angle -= 360


        if self.cnt == 200:
            self.cnt = 0
        if self.cnt > 160:
            draw = False
        dx = self._vel * math.cos(math.radians(self._angle))
        dy = self._vel * math.sin(math.radians(self._angle))

        collision = False

        if not (self.map_w > self.current_x + dx > 0):
            dx = 0
        if not (self.map_h > self.current_y + dy > 0):
            dy = 0



        def upper_right(x, y, x_prev, y_prev):
            return x <= (x_prev + self.size + 1) and y >= (y_prev - self.size - 1)

        def upper_left(x, y, x_prev, y_prev):
            return x >= (x_prev - self.size - 1) and y >= (y_prev - self.size - 1)

        def lower_left(x, y, x_prev, y_prev):
            return x >= (x_prev - self.size - 1) and y <= (y_prev + self.size + 1)

        def lower_right(x, y, x_prev, y_prev):
            return x <= (x_prev + self.size) and y <= (y_prev + self.size)

        new_x = self.current_x + dx
        new_y = self.current_y + dy

        predicate = None
        if self._angle <= 90:
            predicate = lower_right
        elif self._angle <= 180:
            predicate = lower_left
        elif self._angle <= 270:
            predicate = upper_left
        else:
            predicate = upper_right

        for i in range(int(new_x - self.size), int(new_x + self.size)):
            for j in range(int(new_y - self.size), int(new_y + self.size)):
                if not predicate(i, j, self.current_x, self.current_y):
                    if self._collision_arr[i][j]:
                        collision = True
                        break
            if collision:
                break

        if draw:
            for i in range(int(new_x - self.size), int(new_x + self.size)):
                for j in range(int(new_y - self.size), int(new_y + self.size)):
                    self._collision_arr[i][j] = True

        self.current_x = new_x
        self.current_y = new_y

        return collision, draw, dx, dy
