import pygame
from player import Player
from active_player import ActivePlayer
from bonus import Bonus

from enum import Enum


class COLOR(Enum):
    RED = (255, 0, 0)
    GREEN = (0, 255, 0)
    BLUE = (0, 0, 255)
    WHITE = (255, 255, 255)


class App:
    def __init__(self):
        self._running = True
        self._display = None
        self.bg = None
        self.size = self._width, self._height = 640, 400
        self._entities = []
        self._font = None
        self._text_rect = pygame.Rect(0, 0, 80, 80)

    def on_init(self):
        pygame.init()
        self._display = self.create_window()
        self.bg = pygame.surface.Surface(self.size)
        self.bg.fill(COLOR.WHITE.value)

        self._running = True

        self._entities.append(Player(100, 200,
                                     2, 2, COLOR.GREEN.value, self._display, "player1.jpg"))
        self._entities.append(Player(100, 300,
                                     2, 2, COLOR.GREEN.value, self._display, "player1.jpg"))

        self._entities.append(ActivePlayer(self._width / 2, self._height / 2,
                                           2, 2, COLOR.RED.value, self._display, "player.jpg"))

        self._entities.append(Bonus(200, 50, self._display, "player.jpg"))

        self._display.fill(pygame.Color(COLOR.WHITE.value))
        self._font = pygame.font.Font("Minecraft_Russian_By_Nexon.ttf", 24)

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
        # Draw background and tails
        self._display.blit(self.bg, pygame.Rect(0, 0, self._width, self._height))
        for p in self._entities:
            if isinstance(p, Player) and not p.alive:
                pass
            else:
                p.on_render(self.bg)
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
        display = pygame.display.set_mode(self.size)
        pygame.display.set_caption("Projekt 1 PR")
        return display

    def show_stats(self):  # Debugger
        displayed_text = str(self._entities[0].get_angle())
        text = self._font.render(displayed_text, True, COLOR.GREEN.value, COLOR.WHITE.value)
        pygame.draw.rect(self._display,COLOR.WHITE.value,self._text_rect)
        self._display.blit(text, self._text_rect)


if __name__ == "__main__":
    theApp = App()
    theApp.on_execute()
