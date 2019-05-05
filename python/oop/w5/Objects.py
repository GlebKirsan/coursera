from abc import ABC, abstractmethod
from random import randint


class AbstractHeroFactory(ABC):
    _sprite = None

    @abstractmethod
    def create(self):
        pass


class AbstractObject(ABC):

    @abstractmethod
    def __init__(self):
        self._sprite = []

    @abstractmethod
    def draw(self, display):
        pass

    @property
    def sprite(self):
        return self._sprite[0]

    @sprite.setter
    def sprite(self, value):
        self._sprite = value if isinstance(value, list) else [value]


class Interactive(ABC):

    @abstractmethod
    def interact(self, engine, hero):
        pass


class Creature(AbstractObject):

    def __init__(self, icon, stats, position):
        self._sprite = icon
        self.stats = stats
        self.position = position
        self.hp = 0
        self.max_hp = 0
        self.calc_max_HP()
        self.hp = self.max_hp

    def calc_max_HP(self):
        self.max_hp = 5 + self.stats["endurance"] * 2
        if self.max_hp < self.hp:
            self.hp = self.max_hp

    def draw(self, display):
        display.draw_object(self.sprite, self.position)


class Ally(AbstractObject, Interactive):

    def __init__(self, icon, action, position):
        self.sprite = icon
        self.action = action
        self.position = position

    def interact(self, engine, hero):
        self.action(engine, hero)

    def draw(self, display):
        pass


class Enemy(Creature):

    def __init__(self, icon, stats, exp, position):
        super().__init__(icon, stats, position)
        self.exp = exp

    def interact(self, engine, hero):
        delta = (self.stats["strength"] + self.stats["endurance"]) / \
                (hero.stats["strength"] + hero.stats["endurance"])
        exp = self.exp
        if delta > 1.25:
            for i in range(hero.stats["luck"]):
                if hero.stats["luck"] >= randint(1, 150):
                    exp += int(self.exp // 2)
                    break

        exp = int(exp)
        hero.exp += exp
        engine.notify({"info": f"{exp} exp up"})


class Hero(Creature):

    def __init__(self, stats, icon):
        pos = [1, 1]
        super().__init__(icon, stats, pos)
        self.level = 1
        self.exp = 0
        self.gold = 0

    @property
    def max_exp(self):
        return 100 * (2 ** (self.level - 1))

    def level_up(self):
        while self.exp >= self.max_exp:
            yield "level up!"
            self.level += 1
            self.stats["strength"] += 2
            self.stats["endurance"] += 2
            self.stats["intelligence"] += 2
            self.calc_max_HP()
            self.hp = self.max_hp


class Effect(Hero):

    def __init__(self, base):
        self.base = base
        self.stats = self.base.stats.copy()
        self.apply_effect()

    @property
    def position(self):
        return self.base.position

    @position.setter
    def position(self, value):
        self.base.position = value

    @property
    def level(self):
        return self.base.level

    @level.setter
    def level(self, value):
        self.base.level = value

    @property
    def gold(self):
        return self.base.gold

    @gold.setter
    def gold(self, value):
        self.base.gold = value

    @property
    def hp(self):
        return self.base.hp

    @hp.setter
    def hp(self, value):
        self.base.hp = value

    @property
    def max_hp(self):
        return self.base.max_hp

    @max_hp.setter
    def max_hp(self, value):
        self.base.max_hp = value

    @property
    def exp(self):
        return self.base.exp

    @exp.setter
    def exp(self, value):
        self.base.exp = value

    @property
    def sprite(self):
        return self.base.sprite

    @abstractmethod
    def apply_effect(self):
        pass


class Berserk(Effect):
    def apply_effect(self):
        self.stats["strength"] += 7
        self.stats["endurance"] += 7
        self.stats["intelligence"] -= 3
        self.stats["luck"] += 7
        super().apply_effect()


class Blessing(Effect):
    def apply_effect(self):
        self.stats["strength"] += 1
        self.stats["endurance"] += 2
        self.stats["intelligence"] += 2
        self.stats["luck"] += 1
        super().apply_effect()


class Weakness(Effect):
    def apply_effect(self):
        self.stats["strength"] -= 4
        self.stats["endurance"] -= 4
        super().apply_effect()


class Sunset(Effect):
    def __init__(self, base):
        super().__init__(base)

    @property
    def position(self):
        return self.base.position

    @position.setter
    def position(self, value):
        self.gold += 1
        self.base.position = value

    def apply_effect(self):
        super().apply_effect()