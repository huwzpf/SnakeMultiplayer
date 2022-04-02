import pygame
from player import Player
from active_player import ActivePlayer
from bonus import Bonus
from typing import Final
from enum import Enum
from server import Server


class COLOR(Enum):
    RED = (255, 0, 0)
    GREEN = (0, 255, 0)
    BLUE = (0, 0, 255)
    WHITE = (255, 255, 255)


# Constants
VELOCITY: Final = 2
SIZE: Final = 2
FONT_SIZE: Final = 24
WIDTH: Final = 640
HEIGHT: Final = 400


class App:
    def __init__(self):
        self._running = True
        self._display = None
        self._bg = None
        self._size = self._width, self._height = WIDTH, HEIGHT
        self._entities = []
        self._font = None
        self._text_rect = pygame.Rect(0, 0, 60, 60)
        self._server = Server()

    def on_init(self):
        pygame.init()
        self._display = self.create_window()
        self._bg = pygame.surface.Surface(self._size)
        self._bg.fill(COLOR.WHITE.value)

        self._running = True

        self._entities.append(Player(100, 200,
                                     SIZE, VELOCITY, COLOR.GREEN.value, self._display, "resources/player1.jpg"))
        self._entities.append(Player(100, 300,
                                     SIZE, VELOCITY, COLOR.GREEN.value, self._display, "resources/player1.jpg"))

        self._entities.append(ActivePlayer(WIDTH / 2, HEIGHT / 2,
                                           SIZE, VELOCITY, COLOR.RED.value, self._display, "resources/player.jpg"))

        self._entities.append(Bonus(200, 50, self._display, "resources/player.jpg"))

        self._display.fill(pygame.Color(COLOR.WHITE.value))
        self._font = pygame.font.Font("resources/font.ttf", FONT_SIZE)

    def on_event(self, event):
        for p in self._entities:
            if isinstance(p, Player) and not p.alive:
                pass
            else:
                p.on_event(event)
        if event.type == pygame.QUIT:
            self._running = False

    def on_loop(self):
        for p in self._entities:
            if isinstance(p, Player) and not p.alive:
                pass
            else:
                p.on_loop()

    def on_render(self):
        self._display.blit(self._bg, pygame.Rect(0, 0, self._width, self._height))
        for p in self._entities:
            if isinstance(p, Player) and not p.alive:
                pass
            else:
                p.on_render(self._bg)
        # self.show_stats()
        pygame.display.flip()

    def on_execute(self):
        clock = pygame.time.Clock()
        self.on_init()
        cnt = 1
        while self._running:
            for event in pygame.event.get():
                self.on_event(event)
            self.on_loop()
            self.on_render()
            clock.tick(60)
            cnt += 1
            if cnt == 120:
                self._entities.pop()
        pygame.quit()

    def create_window(self):
        display = pygame.display.set_mode(self._size)
        pygame.display.set_caption("Projekt 1 PR")
        return display

    def show_stats(self):
        displayed_text = str(self._entities[0].get_angle())
        text = self._font.render(displayed_text, True, COLOR.GREEN.value, COLOR.WHITE.value)
        pygame.draw.rect(self._display, COLOR.WHITE.value, self._text_rect)
        self._display.blit(text, self._text_rect)

    def communicate(self):
        self._server.connection()


if __name__ == "__main__":
    theApp = App()
    theApp.communicate()
    theApp.on_execute()
