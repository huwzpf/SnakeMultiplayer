import pygame.key
import playerlogic


class Player:
    def __init__(self, x, y, size, velocity, color, display):
        self._x = x
        self._y = y
        self._w = self._h = self._size = size
        self._color = color
        self._display = display
        self._draw = True
        self._texture = None
        self._hitbox = None

        self._logicUnit = playerlogic.PlayerLogic(velocity)
        self._texture = pygame.image.load("player.jpg").convert()

    def get_angle(self):
        return self._logicUnit.get_angle()

    def on_event(self, event):
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

    def on_loop(self):
        self._draw, dx, dy = self._logicUnit.loop()
        self._x += dx
        self._y += dy

    def on_render(self, bg):
        if self._draw:
            pygame.draw.circle(bg, self._color, (self._x, self._y), self._size)
        w, h = bg.get_size()
        self._display.blit(bg, pygame.Rect(0, 0, w, h))
        self.draw_head()

    def draw_head(self):
        self._hitbox = pygame.Rect(self._x, self._y, self._w, self._h)
        self._display.blit(self._texture, self._hitbox)

