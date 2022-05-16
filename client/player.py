import pygame.key
from entity import Entity
from data_types import *


class Player(Entity):
    def __init__(self, id, x, y, size, color, display, txtpath, txtpath_b1, txtpath_b2, txtpath_b3, logic_unit):
        self.id = id
        self.alive = True
        self._x = x
        self._y = y
        self._w = self._h = self._size = size
        self._color = color
        self._display = display
        self._draw = True
        self._map_w, self._map_h = display.get_size()
        self._logicUnit = logic_unit
        self._texture = pygame.image.load(txtpath).convert()
        self._texture_b1 = pygame.image.load(txtpath_b1).convert()
        self._texture_b2 = pygame.image.load(txtpath_b2).convert()
        self._texture_b3 = pygame.image.load(txtpath_b3).convert()
        self.bonus = 0

    def on_event(self, event):
        pass

    def on_loop(self):
        update = self._logicUnit.get_update(self.id)

        self.bonus = update.bonus
        self._x += update.dx
        self._y += update.dy

        self._draw = update.draw
        self._size = update.size

        if not update.alive:
            self.alive = False

    def on_render(self, bg):
        if self._draw:
            pygame.draw.rect(bg, self._color,
                             pygame.Rect(self._x - self._size, self._y - self._size, self._size * 2, self._size * 2))

        if self.bonus == 1:
            txt = self._texture_b1
        elif self.bonus == 2:
            txt = self._texture_b2
        elif self.bonus == 3:
            txt = self._texture_b3
        else:
            txt = self._texture
        self._display.blit(txt, [self._x - self._texture.get_size()[0] / 2,
                                 self._y - self._texture.get_size()[1] / 2])

    @property
    def color(self):
        return self._color
