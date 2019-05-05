import pygame
import collections
from GameSettings import *


class ScreenHandle(pygame.Surface):

    def __init__(self, *args, **kwargs):
        self.engine = None
        if len(args) > 1:
            self.successor = args[-1]
            self.next_coord = args[-2]
            args = args[:-2]
        else:
            self.successor = None
            self.next_coord = (0, 0)
        super().__init__(*args, **kwargs)
        self.fill(COLORS["wooden"])

    def draw(self, canvas):
        if self.successor is not None:
            canvas.blit(self.successor, self.next_coord)
            self.successor.draw(canvas)

    def connect_engine(self, engine):
        engine.subscribe(self)
        self.engine = engine
        if self.successor is not None:
            self.successor.connect_engine(engine)

    def update(self, value):
        pass

    @property
    def min_x(self):
        return self.get_size()[0]

    @property
    def min_y(self):
        return self.get_size()[1]


class GameSurface(ScreenHandle):

    def __init__(self, *args, **kwargs):
        super().__init__(*args, **kwargs)
        self.map_beg_pos = [0, 0]
        self.map_shift = [0, 0]
        self.display_items = [0, 0]

    @property
    def sprite_size(self):
        return self.engine.sprite_size

    def canvas_pos(self, pos, sprite_size=None):
        # Преобразуем положение на карте в координаты на слое
        sprite_size = self.sprite_size if sprite_size is None else sprite_size
        pos_x = pos[0] * sprite_size - self.map_shift[0]
        pos_y = pos[1] * sprite_size - self.map_shift[1]
        return pos_x, pos_y

    def scroll(self):
        if self.engine.map:
            self.map_beg_pos = [0, 0]
            self.map_shift = [0, 0]
            self.display_items = [0, 0]

            hero_pos = self.engine.hero.position if self.engine.hero else (0, 0)
            map_size = self.engine.map.size

            for i in range(2):
                self.map_shift[i] = hero_pos[i] * self.sprite_size + \
                                    self.sprite_size // 2 - self.get_size()[i] // 2

                if self.map_shift[i] < 0:
                    self.map_shift[i] = 0
                max_shift = map_size[i] * self.sprite_size - self.get_size()[i]
                if self.map_shift[i] > max_shift:
                    self.map_shift[i] = max_shift
                    if self.map_shift[i] < 0:
                        self.map_shift[i] = self.map_shift[i] // 2

    def draw_hero(self):
        if self.engine.hero:
            self.draw_object(self.engine.hero.sprite,
                             self.engine.hero.position)

    def draw_map(self):

        if self.engine.map:
            for i in range(self.engine.map.size[0]):
                for j in range(self.engine.map.size[1]):
                    self.draw_object(self.engine.map[i, j].sprite, (i, j))
        else:
            self.fill(COLORS["white"])

    def draw_object(self, sprite, coord):
        self.blit(sprite, self.canvas_pos(coord))

    def draw(self, canvas):
        self.scroll()

        # Рисуем карту
        self.draw_map()

        # Вспомогательные объекты
        for obj in self.engine.objects:
            self.draw_object(obj.sprite, obj.position)

        # Герой
        self.draw_hero()

        super().draw(canvas)


class ProgressBar(ScreenHandle):

    def __init__(self, *args, **kwargs):
        super().__init__(*args, **kwargs)
        self.fill(COLORS["wooden"])

    def draw(self, canvas):
        self.fill(COLORS["wooden"])
        pygame.draw.rect(self, COLORS["black"], (50, 30, 200, 30), 2)
        pygame.draw.rect(self, COLORS["black"], (50, 70, 200, 30), 2)

        if self.engine.hero:
            pygame.draw.rect(
                self, COLORS["red"],
                (50, 30, 200 * self.engine.hero.hp / self.engine.hero.max_hp, 30))
            pygame.draw.rect(
                self, COLORS["green"],
                (50, 70, 200 * self.engine.hero.exp / self.engine.hero.max_exp, 30))

            font = pygame.font.SysFont("comicsansms", 20)
            self.blit(font.render(f'Hero at {self.engine.hero.position}', True, COLORS["black"]),
                      (250, 0))

            self.blit(font.render(f'{self.engine.level} floor', True, COLORS["black"]), (10, 0))

            self.blit(font.render(f'HP', True, COLORS["black"]), (10, 30))
            self.blit(font.render(f'Exp', True, COLORS["black"]), (10, 70))

            self.blit(font.render(
                f'{self.engine.hero.hp}/{self.engine.hero.max_hp}', True, COLORS["black"]), (60, 30))
            self.blit(font.render(
                f'{self.engine.hero.exp}/{(100*(2**(self.engine.hero.level-1)))}',
                True, COLORS["black"]), (60, 70))

            self.blit(font.render(
                f'Level', True, COLORS["black"]), (300, 30))
            self.blit(font.render(
                f'Gold', True, COLORS["black"]), (300, 70))

            self.blit(font.render(
                f'{self.engine.hero.level}', True, COLORS["black"]), (360, 30))
            self.blit(font.render(
                f'{self.engine.hero.gold}', True, COLORS["black"]), (360, 70))

            self.blit(font.render(
                f'Str', True, COLORS["black"]), (420, 30))
            self.blit(font.render(
                f'Luck', True, COLORS["black"]),(420, 70))

            self.blit(font.render(
                f'{self.engine.hero.stats["strength"]}', True, COLORS["black"]), (480, 30))
            self.blit(font.render(
                f'{self.engine.hero.stats["luck"]}', True, COLORS["black"]), (480, 70))

            self.blit(font.render(
                f'SCORE', True, COLORS["black"]), (550, 30))
            self.blit(font.render(
                f'{self.engine.score:.4f}', True, COLORS["black"]), (550, 70))

        super().draw(canvas)


class InfoWindow(ScreenHandle):

    def __init__(self, *args, **kwargs):
        super().__init__(*args, **kwargs)
        self.len = 30
        clear_for_d = []
        self.data = collections.deque(clear_for_d, maxlen=self.len)

    def clear(self):
        self.data = []

    def update(self, value):
        if "info" in value:
            self.data.append(f"> {str(value['info'])}")
        if "start" in value:
            self.clear()

    def draw(self, canvas):
        self.fill(COLORS["wooden"])

        font = pygame.font.SysFont("comicsansms", 16)
        for i, text in enumerate(self.data):
            self.blit(font.render(text, True, COLORS["black"]),
                      (5, 20 + 18 * i))

        super().draw(canvas)


class MiniMap(ScreenHandle):

    def __init__(self, *args, **kwargs):
        super().__init__(*args, **kwargs)
        self.size = 4
        self.alpha = 198
        self.colors = {"hero": (255, 0, 0),
                       "wall": (0, 0, 0, self.alpha),
                       "ground": (168, 168, 168, self.alpha),
                       "object": (255, 255, 0)
                       }

    def canvas_pos(self, pos):
        return pos[0] * self.size, \
               pos[1] * self.size

    def draw_object(self, pos, color):
        x, y = self.canvas_pos(pos)
        pygame.draw.rect(self, color, (x, y, self.size, self.size))

    def draw(self, gd):
        self.fill((0, 0, 0, 0))
        if self.engine.show_minimap:
            if self.engine.map:
                # Рисуем карту
                for i in range(self.engine.map.size[0]):  # X
                    for j in range(self.engine.map.size[1]):  # Y
                        color = self.colors["ground"] \
                            if self.engine.map[i, j].passable \
                            else self.colors["wall"]
                        self.draw_object((i, j), color)

            if self.engine.hero:
                # Герой
                self.draw_object(self.engine.hero.position, self.colors["hero"])

            for obj in self.engine.objects:
                # Вспомогательные объекты
                self.draw_object(obj.position, self.colors["object"])

        super().draw(gd)


class HelpWindow(ScreenHandle):

    def __init__(self, *args, **kwargs):
        super().__init__(*args, **kwargs)
        self.len = 30
        clear = []
        self.data = collections.deque(clear, maxlen=self.len)
        self.data.append([" →", "Move Right"])
        self.data.append([" ←", "Move Left"])
        self.data.append([" ↑ ", "Move Top"])
        self.data.append([" ↓ ", "Move Bottom"])
        self.data.append([" H ", "Show Help"])
        self.data.append(["Num+", "Zoom +"])
        self.data.append(["Num-", "Zoom -"])
        self.data.append([" R ", "Restart Game"])
        self.data.append([" M ", "Show Minimap"])

    def draw(self, canvas):
        alpha = 0
        if self.engine.show_help:
            alpha = 128
        self.fill((0, 0, 0, alpha))
        font1 = pygame.font.SysFont("courier", 24)
        font2 = pygame.font.SysFont("serif", 24)
        if self.engine.show_help:
            pygame.draw.lines(self, (255, 0, 0, 255), True, [
                              (0, 0), (700, 0), (700, 500), (0, 500)], 5)
            for i, text in enumerate(self.data):
                self.blit(font1.render(text[0], True, ((128, 128, 255))),
                          (50, 50 + 30 * i))
                self.blit(font2.render(text[1], True, ((128, 128, 255))),
                          (150, 50 + 30 * i))

        super().draw(canvas)
