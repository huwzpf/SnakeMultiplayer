import pygame.key
import playerlogic


class Player:
    def __init__(self, x, y, size, velocity, bg_color, color, surface):
        self._x = x
        self._y = y
        self._w = self._h = self._size = size
        self._color = color
        self._surface = surface
        self.logicUnit = playerlogic.PlayerLogic(velocity)
        self.draw = True
        self.bg_color = bg_color
        self._texture = None
        self._hitbox = None

        self.init_texture()

    def on_event(self, event):
        if event.type == pygame.KEYDOWN:
            if event.key == pygame.K_LEFT:
                self.logicUnit.set_left_down()
            elif event.key == pygame.K_RIGHT:
                self.logicUnit.set_right_down()
        if event.type == pygame.KEYUP:
            if event.key == pygame.K_LEFT:
                self.logicUnit.set_left_up()
            elif event.key == pygame.K_RIGHT:
                self.logicUnit.set_right_up()

    def on_loop(self):

        self.draw, dx, dy = self.logicUnit.loop()
        self._x += dx
        self._y += dy

    def on_render(self, bg):
        if self.draw:
            pygame.draw.circle(bg, self._color, (self._x, self._y), self._size)
        self._surface.blit(bg, pygame.Rect(0, 0, 2 * 640, 2 * 400))
        self.draw_head()

    def get_angle(self):
        return self.logicUnit.get_angle()

    def init_texture(self):
        self._texture = pygame.image.load("player.jpg").convert()
        self._hitbox = pygame.Rect(self._x, self._y,self._w,self._h)

    def draw_head(self):
        self._hitbox = pygame.Rect(self._x, self._y, self._w, self._h)
        self._surface.blit(self._texture, self._hitbox)

