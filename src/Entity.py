from abc import ABC, abstractmethod


class Entity(ABC):
    @abstractmethod
    def on_event(self, event):
        pass

    @abstractmethod
    def on_loop(self):
        pass

    @abstractmethod
    def on_render(self, bg):
        pass
