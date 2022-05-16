import pygame.key
from entity import Entity
from data_types import BonusState


class BonusDrawer(Entity):
    def __init__(self, display, txtpath1, txtpath2, txtpath3, logic_unit):
        self._display = display
        self._map_w, self._map_h = display.get_size()
        self._logicUnit = logic_unit
        self._texture1 = pygame.image.load(txtpath1).convert()
        self._texture2 = pygame.image.load(txtpath2).convert()
        self._texture3 = pygame.image.load(txtpath3).convert()
        self.bonuses = []

    def on_event(self, event):
        pass

    def on_loop(self):
        self.bonuses = self._logicUnit.get_bonuses()

    def on_render(self, bg):
        for bonus in self.bonuses:
            if bonus.type == 0:
                self._display.blit(self._texture1, [bonus.x, bonus.y])
            elif bonus.type == 1:
                self._display.blit(self._texture2, [bonus.x, bonus.y])
            elif bonus.type == 2:
                self._display.blit(self._texture3, [bonus.x, bonus.y])
