class GameEngine:
    objects = []
    map = None
    hero = None
    level = -1
    working = True
    subscribers = set()
    score = 0.
    _game_process = True
    show_help = False
    show_minimap = False
    _sprite_size = 1
    level_generator = None
    hero_generator = None

    def subscribe(self, obj):
        if obj not in self.subscribers:
            self.subscribers.add(obj)

    def unsubscribe(self, obj):
        if obj in self.subscribers:
            self.subscribers.remove(obj)

    def notify(self, message):
        for i in self.subscribers:
            i.update(message)

    # LEVEL
    def start(self):
        # Запуск новой игры
        self.level = 1
        self.score = 0
        self.hero_create()
        self.level_reset()
        self.notify({"start": "NewGame"})

    def level_reset(self):
        # Сбрасываем уровень в начальное состояние
        # Параметры героя не сбрасываются, уровни случайны - оставим как геймлейную "фишку"
        # Уведомления о начале новой игры
        self.level_load()

    def level_next(self):
        self.level += 0 if self.level_generator.total() == self.level else 1
        self.level_load()

    def level_load(self, number=None):
        self.level = self.level if number is None else number
        self.clear_object()
        self.level_generator.create(self.level)
        self.load_map(self.level_generator.map())
        self.add_objects(self.level_generator.objects())

        self.hero.position[0] = self.level_generator.hero_coord()[0]
        self.hero.position[1] = self.level_generator.hero_coord()[1]

    # HERO
    def hero_create(self):
        self.hero = self.hero_generator.create()

    def interact(self):

        for obj in self.objects:
            if list(obj.position) == self.hero.position:
                self.delete_object(obj)
                obj.interact(self, self.hero)

        for msg in self.hero.level_up():
            # Уведомления: Достигнут новый уровень
            self.notify({"info": msg})

    # MOVEMENT
    def move(self, diff):
        if self.map[self.hero.position[0] + diff[0], self.hero.position[1] + diff[1]].passable:
            self.score -= 0.02
            for num, step in enumerate(diff):
                self.hero.position[num] += step
            self.hero.position = self.hero.position
            self.interact()

    def move_up(self):
        self.move((0, -1))

    def move_down(self):
        self.move((0, 1))

    def move_left(self):
        self.move((-1, 0))

    def move_right(self):
        self.move((1, 0))

    # MAP
    def load_map(self, game_map):
        self.map = game_map

    # OBJECTS
    def add_object(self, obj):
        self.objects.append(obj)

    def add_objects(self, objects):
        self.objects.extend(objects)

    def delete_object(self, obj):
        self.objects.remove(obj)

    def clear_object(self):
        self.objects = []

    # GRAPHIC
    def zoom_in(self):
        self.zoom(+1)

    def zoom_out(self):
        self.zoom(-1)

    def zoom(self, val):
        prev_size = self.sprite_size
        self.sprite_size += 0 if self.sprite_size < 1 and val < 0 else val
        # Уведомления об изменении размера спрайта
        self.notify({"sprite_size": (prev_size, self.sprite_size)})

    @property
    def sprite_size(self):
        return self._sprite_size

    @sprite_size.setter
    def sprite_size(self, value):
        prev_size = self._sprite_size
        self._sprite_size = value
        # Уведомления об изменении размера спрайта
        self.notify({"sprite_size": (prev_size, self.sprite_size)})

    @property
    def game_process(self):
        return not self.show_help and self._game_process

    @game_process.setter
    def game_process(self, value):
        self._game_process = value
