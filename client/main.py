import sys
import pygame
from player import Player
from active_player import ActivePlayer
from typing import Final
from server import Server
from bonus_drawer import BonusDrawer
from data_types import *

# Constants
FONT_SIZE: Final = 24
WIDTH: Final = 1280
HEIGHT: Final = 720


class App:
    def __init__(self, ip="127.0.0.1"):
        self._running = True
        self._display = None
        self._bg = None
        self._size = self._width, self._height = WIDTH, HEIGHT
        self._entities = []
        self._font = None
        self._leaderboard = None
        self._server = Server(ip)
        self.on_execute()

    def on_init(self):
        pygame.init()
        # initialize variables
        self._display = self.create_window()
        self._bg = pygame.surface.Surface(self._size)
        self._bg.fill(COLOR.WHITE.value)
        self._display.fill(pygame.Color(COLOR.WHITE.value))
        self._font = pygame.font.Font("client/resources/font.ttf", FONT_SIZE)

        # establish connection and display lobby
        self._server.connection_lobby(self.show_lobby_screen, self.refresh_window)
        self._server.start_game()
        init_data = self._server.get_init_data()

        # add entities
        for i in range(init_data.player_count):
            if i == int(init_data.sess_id[0]):
                self._entities.append(ActivePlayer(init_data.players_data[i].ID, init_data.players_data[i].start_x,
                                                   init_data.players_data[i].start_y, init_data.players_data[i].size,
                                                   tuple(init_data.players_data[i].color),
                                                   self._display, "client/resources/active_player.jpg",
                                                   "client/resources/active_player_b1.jpg",
                                                   "client/resources/active_player_b2.jpg",
                                                   "client/resources/active_player_b3.jpg", self._server))
            else:
                self._entities.append(Player(init_data.players_data[i].ID, init_data.players_data[i].start_x,
                                             init_data.players_data[i].start_y, init_data.players_data[i].size,
                                             tuple(init_data.players_data[i].color),
                                             self._display, "client/resources/player.jpg",
                                             "client/resources/player_b1.jpg", "client/resources/player_b2.jpg",
                                             "client/resources/player_b3.jpg", self._server))
        self._entities.append(BonusDrawer(self._display, "client/resources/bonus1.jpg", "client/resources/bonus2.jpg",
                                          "client/resources/bonus3.jpg", self._server))

        self._running = True

    def on_event(self, event):
        for p in self._entities:
            if isinstance(p, Player) and not p.alive:
                pass
            else:
                p.on_event(event)
        if event.type == pygame.QUIT:
            self._running = False

    def on_loop(self):
        if self._server.loop() == 0:
            self._leaderboard = self._server.recive_leaderboard()
            self._running = False
        for p in self._entities:
            if isinstance(p, Player) and not p.alive:
                pass
            else:
                p.on_loop()

    def on_render(self):
        self._display.blit(self._bg, pygame.Rect(0, 0, self._width, self._height))
        for p in self._entities:
            p.on_render(self._bg)
        pygame.display.flip()

    def on_execute(self):
        clock = pygame.time.Clock()
        self.on_init()
        while self._running:
            for event in pygame.event.get():
                self.on_event(event)
            self.on_loop()
            self.on_render()
            clock.tick(30)

        # GAME OVER, clear screen and show stats
        self._bg.fill(COLOR.WHITE.value)
        self._display.blit(self._bg, pygame.Rect(0, 0, self._width, self._height))
        self.show_stats()
        pygame.display.flip()
        while True:
            event = pygame.event.wait()
            if event.type == pygame.QUIT or event.type == pygame.KEYDOWN:
                pygame.quit()
                break

    def refresh_window(self):
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                pygame.quit()
                self._running = False

    def create_window(self):
        display = pygame.display.set_mode(self._size)
        pygame.display.set_caption("Projekt 1 PR 184297 184711")
        return display

    def show_lobby_screen(self, current, target):
        text_rect = pygame.Rect(WIDTH / 4, 50, WIDTH / 2, 50)
        displayed_text = "Waiting for players"
        text = self._font.render(displayed_text, True, COLOR.RED.value, COLOR.WHITE.value)
        self._display.blit(text, text_rect)
        displayed_text = f"{current} / {target} players"
        text = self._font.render(displayed_text, True, COLOR.RED.value, COLOR.WHITE.value)
        self._display.blit(text, text_rect.move(text_rect.x, text_rect.y + 100))
        pygame.display.flip()

    def show_stats(self):
        text_rect = pygame.Rect(WIDTH * 0.4, 50,  WIDTH * 0.6, 50)
        displayed_text = str("Leaderboards")
        text = self._font.render(displayed_text, True, COLOR.RED.value, COLOR.WHITE.value)
        self._display.blit(text, text_rect)
        for i in range(len(self._leaderboard)):
            if self._leaderboard[i] == self._server.get_init_data().sess_id[0]:
                displayed_text = str(f"{i + 1} : You")
            else:
                displayed_text = str(f"{i + 1} : Player {self._leaderboard[i] + 1}")
            # select i-th player's color
            color = next((x for x in self._entities if isinstance(x, Player) and x.id == self._leaderboard[i])).color
            text = self._font.render(displayed_text, True, color, COLOR.WHITE.value)
            self._display.blit(text, text_rect.move(0, text_rect.y + ((i + 1) * 50)))


if __name__ == "__main__":
    if len(sys.argv) > 1:
        App(sys.argv[1])
    else:
        App()
