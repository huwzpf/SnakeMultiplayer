from player import Player
import pygame


class ActivePlayer(Player):
    def on_event(self, event):
        # Handling player input
        if event.type == pygame.KEYDOWN:
            if event.key == pygame.K_LEFT:
                self._logicUnit.set_left_down()
            elif event.key == pygame.K_RIGHT:
                self._logicUnit.set_right_down()
        if event.type == pygame.KEYUP:
            if event.key == pygame.K_LEFT:
                self._logicUnit.set_left_up()
            elif event.key == pygame.K_RIGHT:
                self._logicUnit.set_right_up()