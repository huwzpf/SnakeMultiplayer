import pygame.key


class Player:
    def __init__(self, x, y, size, velocity, color, surface):
        self._x = x
        self._y = y
        self._w = self._h = self._size = size
        self._vel = velocity
        self._color = color
        self._surface = surface

    def on_event(self):
        keys = pygame.key.get_pressed()
        if keys[pygame.K_LEFT]:
            self._x -= self._vel

        if keys[pygame.K_RIGHT]:
            self._x += self._vel

        if keys[pygame.K_UP]:
            self._y -= self._vel

        if keys[pygame.K_DOWN]:
            self._y += self._vel

    def on_render(self):
        pygame.draw.circle(self._surface, self._color, (self._x, self._y), self._size)
