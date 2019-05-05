from abc import ABC, abstractmethod
from random import randint
import Objects


class AbstractAction(ABC):
    @classmethod
    @abstractmethod
    def cast(cls, engine, hero):
        pass


class EmptyAction(AbstractAction):
    # Пустой метод
    @classmethod
    def cast(cls, engine, hero):
        pass


class NextLevelAction(AbstractAction):
    # Переход на следующий уровень
    @classmethod
    def cast(cls, engine, hero):
        engine.level_next()


class RestoreHPAction(AbstractAction):
    # Полное восстановление здоровья
    @classmethod
    def cast(cls, engine, hero):
        engine.score += 0.1
        hero.hp = hero.max_hp
        engine.notify({"info": "HP restored"})


class BlessingAction(AbstractAction):
    # эффект: Благословение
    @classmethod
    def cast(cls, engine, hero):
        val = int(20 * 1.5**engine.level) - 2 * hero.stats["intelligence"]
        if hero.gold >= val:
            engine.score += 0.2
            hero.gold -= val
            if randint(0, 1) == 0:
                engine.hero = Objects.Blessing(hero)
                engine.notify({"info": "Blessing applied"})
            else:
                engine.hero = Objects.Berserk(hero)
                engine.notify({"info": "Berserk applied"})
        else:
            engine.score -= 0.1


class RemEffectAction(AbstractAction):
    @classmethod
    def cast(cls, engine, hero):
        cost = int(10 * 1.5**engine.level) - 2 * hero.stats["intelligence"]
        if "base" in dir(hero) and hero.gold >= cost:
            hero.gold -= cost
            engine.hero = hero.base
            engine.hero.calc_max_HP()
            engine.notify({"info": "Effect removed"})


class AddGoldAction(AbstractAction):
    @classmethod
    def cast(cls, engine, hero):
        if randint(1, 10) == 1:
            engine.score -= 0.05
            engine.hero = Objects.Weakness(hero)
            engine.notify({"info": "You were cursed"})
        else:
            engine.score += 0.1
            gold = int(randint(10, 1000) * (1.1**(engine.hero.level - 1)))
            hero.gold += gold
            engine.notify({"info": f"{gold} gold added"})


class SunsetAction(AbstractAction):
    @classmethod
    def cast(cls, engine, hero):
        engine.hero = Objects.Sunset(hero)
        engine.notify({"info": "1 gold per move"})
