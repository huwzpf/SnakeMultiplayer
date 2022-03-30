import pygame
from player import *


class App:
    def __init__(self):
        self._running = True
        self._display = None
        self.size = self._width, self._height = 640, 400
        self._players = []

    def on_init(self):
        pygame.init()
        self._display = self.create_window()
        self._running = True
        self._players.append(Player(self._width / 2, self._height / 2,
                                    20, 10, pygame.Color(255, 0, 0), self._display))

    def on_event(self, event):
        for p in self._players:
            p.on_event()
        if event.type == pygame.QUIT:
            self._running = False

    def on_loop(self):
        pass

    def on_render(self):
        self._display.fill(pygame.Color(255, 255, 255))
        for p in self._players:
            p.on_render()
        pygame.display.update()

    def on_execute(self):
        self.on_init()
        while self._running:
            for event in pygame.event.get():
                self.on_event(event)
            self.on_loop()
            self.on_render()
        pygame.quit()

    def create_window(self):
        display = pygame.display.set_mode(self.size)
        pygame.display.set_caption("Projekt 1 PR")
        return display


if __name__ == "__main__":
    theApp = App()
    theApp.on_execute()
