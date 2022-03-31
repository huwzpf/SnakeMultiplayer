import pygame.key
import playerlogic


class Player:
    def __init__(self, x, y, size, velocity, color, display):
        self.alive = True
        self._x = x
        self._y = y
        self._w = self._h = self._size = size
        self._color = color
        self._display = display
        self._draw = True
        self._texture = None
        self._hitbox = None
        self._map_w, self._map_h = display.get_size()
        self._logicUnit = playerlogic.PlayerLogic(velocity, self._map_w, self._map_h, self._x, self._y, size)
        self._texture = pygame.image.load("player.jpg").convert()

    def get_angle(self):
        return self._logicUnit.get_angle()

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
        # Draw background and tails
        self._display.blit(bg, pygame.Rect(0, 0, self._map_w, self._map_h))
        # Draw head texture on top of it
        self.draw_head()

    def draw_head(self):
        # Update hit box coordinates
        self._hitbox = pygame.Rect(self._x, self._y, self._w, self._h)
        # Draw Head texture on display
        self._display.blit(self._texture, self._hitbox)
