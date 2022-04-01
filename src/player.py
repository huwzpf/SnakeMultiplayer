import pygame.key
import playerlogic
from Entity import Entity


class Player(Entity):
    def __init__(self, x, y, size, velocity, color, display, txtpath):
        self.alive = True
        self._x = x
        self._y = y
        self._w = self._h = self._size = size
        self._color = color
        self._display = display
        self._draw = True
        self._map_w, self._map_h = display.get_size()
        self._logicUnit = playerlogic.PlayerLogic(velocity, self._map_w, self._map_h, self._x, self._y, size)
        self.texture = pygame.image.load(txtpath).convert()

    def get_angle(self):
        return self._logicUnit.get_angle()

    def on_event(self, event):
        pass

    def on_loop(self):
        collision, self._draw, dx, dy = self._logicUnit.loop()
        self._x += dx
        self._y += dy

        if collision:
            self.alive = False

    def on_render(self, bg):
        if self._draw:
            pygame.draw.rect(bg, self._color,
                             pygame.Rect(self._x - self._size, self._y- self._size, self._size * 2, self._size * 2))
        # Draw head texture
        self._display.blit(self.texture, [self._x, self._y])
