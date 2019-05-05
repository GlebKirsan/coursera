import yaml
import Objects
import ObjActions
import MapCreator as MC
from GameSettings import *
from random import randint


class LevelGenerator:
    obj_lib = None
    graph_lib = None
    map_obj = {WALL: MC.Tail(False),
               FLOOR: [MC.Tail(True),
                       MC.Tail(True),
                       MC.Tail(True)]}
    levels = []
    _map = None
    _obj = None
    _hero_pos = None

    @classmethod
    def total(cls):
        return len(cls.levels)

    @classmethod
    def load(cls, file):
        with open(file, "r") as file:
            cls.levels = yaml.load(file.read())['levels']

        cls.levels = [] if cls.levels is None else cls.levels

    @classmethod
    def set_libs(cls, obj_lib):
        # Библиотека с описанием объектов
        cls.obj_lib = obj_lib
        # Спрайты стен и пола, все тайлы на картах будут ссылаться на них
        cls.map_obj[WALL].sprite = cls.obj_lib.textures[WALL]["sprite"]

        for i, obj in enumerate(cls.map_obj[FLOOR]):
            obj.state = i
            obj.sprite = cls.obj_lib.textures[FLOOR]["sprite"]

    @classmethod
    def hero_coord(cls):
        # Координаты героя
        return cls._hero_pos

    @classmethod
    def map(cls):
        # Объект Карта
        return cls._map

    @classmethod
    def objects(cls):
        # Список объектов на карте
        return cls._obj

    @classmethod
    def create(cls, num):
        def get_count(obj):
            return randint(obj["min-count"], obj["max-count"])

        # Получаем генераторы:
        map_gen = cls.levels[num - 1]['map']  # карты
        obj_gen = cls.levels[num - 1]['obj']  # объектов

        # Создаем карту
        cls._map = MC.GameMap(map_gen.get(cls.map_obj))

        # Дополняем перечень объектов, загруженных из настроечного файла, данными общими для всех карт
        for group in (cls.obj_lib.objects, cls.obj_lib.ally):
            for name, info in group.items():
                obj_gen.enemy_list[name] = get_count(info)
                obj_gen.enemy_list[name] = get_count(info)

        # Дополняем перечень объектов, полным списоком врагов
        for name in cls.obj_lib.enemies:
            obj_gen.enemy_list.setdefault(name, 0)

        # Создаем словарь из координат и названия объекта
        objects = obj_gen.get(cls._map)

        cls._obj = []
        for coord, name in objects.items():
            if name == "hero":
                # Координаты героя
                cls._hero_pos = list(coord)
            elif name in cls.obj_lib.enemies:
                # Враждебные юниты
                obj_info = cls.obj_lib.enemies[name]
                cls._obj.append(Objects.Enemy(obj_info["sprite"],
                                              obj_info["stats"],
                                              obj_info["experience"],
                                              coord))
            elif name in cls.obj_lib.objects:
                # Неодушевленные объекты
                obj_info = cls.obj_lib.objects[name]
                cls._obj.append(Objects.Ally(obj_info["sprite"],
                                             obj_info["action"],
                                             coord))
            elif name in cls.obj_lib.ally:
                # Союзные или нейтральные объекты
                obj_info = cls.obj_lib.ally[name]
                cls._obj.append(Objects.Ally(obj_info["sprite"],
                                             obj_info["action"],
                                             coord))


class GraphicalLib:
    # Класс реализует работу с графикой, в частности создает спрайт объекта
    def __init__(self, render, sprite_size, dict_dirs):
        self.render = render
        self.sprite_size = sprite_size, sprite_size
        self.dict_dirs = dict_dirs

    def set_size(self, sprite_size):
        # Размер спрайта
        if isinstance(sprite_size, int):
            sprite_size = sprite_size, sprite_size
        if sprite_size == self.sprite_size:
            return False

        self.sprite_size = sprite_size
        return True

    def create(self, _type, files):
        sprites = []
        _dir = self.dict_dirs[_type]
        for file in files:
            file_name = os.path.join(_dir, file)

            icon = self.render.image.load(file_name).convert_alpha()
            icon = self.render.transform.scale(icon, self.sprite_size)

            sprite = self.render.Surface(
                self.sprite_size,
                self.render.SRCALPHA, 32).convert_alpha()
            sprite.blit(icon, (0, 0))

            sprites.append(sprite)
        return sprites


class ActionLib:
    # Библиотека действий, для привязки с произвольными объектами карты
    def __init__(self):
        self.actions = {
            'reload_game': ObjActions.NextLevelAction,
            'add_gold': ObjActions.AddGoldAction,
            'apply_blessing': ObjActions.BlessingAction,
            'remove_effect': ObjActions.RemEffectAction,
            'sunset_effect': ObjActions.SunsetAction,
            'restore_hp': ObjActions.RestoreHPAction}

    def get_action(self, name):
        return self.actions[name].cast if name in self.actions else name

    def set_action(self, name, value):
        self.actions[name] = value


class ObjectsLib:
    # Основное хранилище описаний объектов
    class ClassPropertyGetter:
        def __init__(self, name):
            self.name = name

        def __get__(self, instance, owner):
            return owner._objects.get(self.name, dict())

    _objects = dict()
    _generators = {"sprite": None, "action": None}
    ally = ClassPropertyGetter("ally")
    enemies = ClassPropertyGetter("enemies")
    objects = ClassPropertyGetter("objects")
    textures = ClassPropertyGetter("textures")

    @classmethod
    def set_generators(cls, sprite_generator=None, action_generator=None):
        cls._generators["sprite"] = sprite_generator
        cls._generators["action"] = action_generator

    @classmethod
    def append(cls, stream):
        objects = yaml.load(stream)
        sprite = cls._generators.get("sprite", None)
        sprite_creation = sprite.create \
            if sprite is not None else lambda x, y: None

        action = cls._generators.get("action", None)
        action_mapping = action.get_action \
            if action is not None else lambda x: None

        for group in objects:
            for name, val in map(lambda x: (x, objects[group][x]), objects[group]):
                if "sprite" in val:
                    # Создаем спрайт
                    val["file"] = val["sprite"]
                    val["sprite"] = sprite_creation(group, val["file"])

                if "action" in val:
                    # Прописываем действие для объектов
                    val["action"] = action_mapping(val["action"])

                if "enemies" == group:
                    # Пересоберем статы врагов в 1 раздел
                    val["stats"] = {
                        stat_name: val.pop(stat_name, 0) for stat_name in LIST_STATS
                    }

        # Объединяем словари
        cls._objects = {**cls._objects, **objects}

    @classmethod
    def sprite_resize(cls, sprite_size):
        sprite = cls._generators.get("sprite", None)
        if sprite is not None and sprite.set_size(sprite_size):
            for group in cls._objects:
                for val in cls._objects[group].values():
                    if "sprite" in val and "file" in val:
                        val["sprite"].clear()
                        val["sprite"].extend(sprite.create(group, val["file"]))

    @classmethod
    def clear(cls):
        cls._objects = dict()

    @classmethod
    def load(cls, stream):
        cls.clear()
        cls.append(stream)

    @classmethod
    def update(cls, message):
        if "sprite_size" in message:
            cls.sprite_resize(message["sprite_size"][1])


class MapFactory(yaml.YAMLObject):
    @classmethod
    def from_yaml(cls, loader, node):
        map_gen = cls.create_map()
        obj_gen = cls.create_objects()
        obj_gen.enemy_list = loader.construct_mapping(node)
        return {'map': map_gen, 'obj': obj_gen}

    @classmethod
    def create_map(cls):
        return MC.MapSurface(cls.__name__)

    @classmethod
    def create_objects(cls):
        return MC.ObjSpawn(cls.__name__)


class RandomMap(MapFactory):
    yaml_tag = "!random_map"

    class RandomSpawn(MC.RandomSpawn):
        def get(self, _map):
            result = super().get(_map)

            # В отличии от базового класса, количество врагов тоже генерируется динамически
            for name, count in filter(lambda x: x[1] == 0, self.enemy_list.items()):
                for _ in range(5):
                    # Доступные координаты
                    coord = MC.GeneratorHelper.random_coord(_map, result)
                    # Сохраняем позицию
                    result[coord] = name
            return result

    @classmethod
    def create_map(cls):
        return MC.RandomMapSurface(cls.__name__)

    @classmethod
    def create_objects(cls):
        return RandomMap.RandomSpawn(cls.__name__)


class EmptyMap(MapFactory):
    yaml_tag = "!empty_map"

    @classmethod
    def create_map(cls):
        return MC.EmptyMapSurface(cls.__name__)

    @classmethod
    def create_objects(cls):
        return MC.RandomSpawn(cls.__name__)


class SpecialMap(MapFactory):
    yaml_tag = "!special_map"

    @classmethod
    def create_map(cls):
        return MC.SpecialMapGenerator(cls.__name__)

    @classmethod
    def create_objects(cls):
        return MC.RandomSpawn(cls.__name__)


class EndMap(MapFactory):
    yaml_tag = "!end_map"

    @classmethod
    def create_map(cls):
        return MC.EndMapSurface(cls.__name__)

    @classmethod
    def create_objects(cls):
        return MC.EmptyMapSpawn(cls.__name__)