import pygame
from Entity import Entity


class Bonus(Entity):
    def __init__(self, x, y, display, txtpath):
        self._x = x
        self._y = y
        self._display = display
        self.texture = pygame.image.load(txtpath).convert()

    def on_event(self, event):
        pass

    def on_loop(self):
        pass

    def on_render(self, bg):
        self._display.blit(self.texture, [self._x, self._y])
