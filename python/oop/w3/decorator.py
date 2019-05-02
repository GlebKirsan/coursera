# Представьте себя ненадолго разработчиком компьютерной игры в стиле фэнтези. Вы будете прописывать систему эффектов, которые могут быть наложены на героя вашей игры.
# У вас есть герой, который обладает некоторым набором характеристик. Враги и союзники могут накладывать на героя положительные и отрицательные эффекты. Эти эффекты 
# каким-то образом изменяют характеристики героя. На героя можно накладывать бесконечно много эффектов, действие одинаковых эффектов суммируется. Игрок должен знать, 
# какие положительные и какие отрицательные эффекты на него были наложены и в каком порядке.

# Класс герой описан следующим образом (характеристики могут быть другими):

class Hero:
    def __init__(self):
        self.positive_effects = []
        self.negative_effects = []
        
        self.stats = {
            "HP": 128,
            "MP": 42,
            "SP": 100,
            
            "Strength": 15,
            "Perception": 4,
            "Endurance": 8,
            "Charisma": 2,
            "Intelligence": 3,
            "Agility": 8,
            "Luck": 1
        } 
        
    def get_positive_effects(self):
        return self.positive_effects.copy()
    
    def get_negative_effects(self):
        return self.negative_effects.copy()
    
    def get_stats(self):
        return self.stats.copy()


# Описывать класс героя в коде НЕ НУЖНО.
# Вам нужно написать систему декораторов, представленную на UML-диаграмме:
# Названия наложенных положительных и отрицательных эффектов добавляются каждое в свой счетчик. Названия эффектов совпадают с названиями классов.
# Описания эффектов:

    # Берсерк — Увеличивает параметры Сила, Выносливость, Ловкость, Удача на 7; уменьшает параметры Восприятие, Харизма, Интеллект на 3. Количество единиц 
    # здоровья увеличивается на 50.
    # Благословение — Увеличивает все основные характеристики на 2.
    # Слабость — Уменьшает параметры Сила, Выносливость, Ловкость на 4.
    # Сглаз — Уменьшает параметр Удача на 10.
    # Проклятье — Уменьшает все основные характеристики на 2.

# К основным характеристикам относятся Сила (Strength), Восприятие (Perception), Выносливость (Endurance), Харизма (Charisma), Интеллект (Intelligence), 
# Ловкость (Agility), Удача (Luck).

# При выполнении задания учитывайте, что:

# Изначальные характеристики базового объекта не должны меняться.
# Изменения характеристик и накладываемых эффектов (баффов/дебаффов) должно происходить динамически, то есть при запросе get_stats, get_positive_effects, get_negative_effects
# Абстрактные классы AbstractPositive, AbstractNegative и соответственно их потомки могут принимать любой параметр base при инициализации объекта (_ _ init _ _ (self, base))
# Проверяйте, что эффекты корректно снимаются, в том числе и из середины стека

from abc import ABC, abstractmethod

class AbstractEffect(Hero, ABC):
    def __init__(self, base):
        self.base = base
    
    @abstractmethod
    def get_stats(self): # Возвращает итоговые хараетеристики
                         # после применения эффекта 
        pass
    
    @abstractmethod
    def get_positive_effects(self):
        pass
    
    @abstractmethod
    def get_negative_effects(self):
        pass


class AbstractPositive(AbstractEffect, ABC):

    def get_negative_effects(self):
        return self.base.get_negative_effects()
        

class Berserk(AbstractPositive, ABC):
    
    def get_positive_effects(self):
        return self.base.get_positive_effects() + ['Berserk']

    def get_stats(self):
        stats = self.base.get_stats()
        buffed_params = ['Strength', 'Endurance', 'Agility', 'Luck']
        debuffed_params = ['Perception', 'Charisma', 'Intelligence']
        for param in buffed_params:
            stats[param] += 7
        for param in debuffed_params:
            stats[param] -= 3
        stats['HP'] += 50
        return stats


class Blessing(AbstractPositive):
    
    def get_positive_effects(self):
        return self.base.get_positive_effects() + ['Blessing']

    def get_stats(self):
        stats = self.base.get_stats()
        for param in stats.keys():
            if param not in ['HP', 'MP', 'SP']:
                stats[param] += 2
        return stats


class AbstractNegative(AbstractEffect):
    
    def get_positive_effects(self):
        return self.base.get_positive_effects()


class Weakness(AbstractNegative):

    def get_negative_effects(self):
        return self.base.get_negative_effects() + ['Weakness']

    def get_stats(self):
        stats = self.base.get_stats()
        debuffed_params = ['Strength', 'Endurance', 'Agility']
        for param in debuffed_params:
            stats[param] -= 4
        return stats


class Curse(AbstractNegative):

    def get_negative_effects(self):
        return self.base.get_negative_effects() + ['Curse']

    def get_stats(self):
        stats = self.base.get_stats()
        for param in stats.keys():
            if param not in ['HP', 'MP', 'SP']:
                stats[param] -= 2
        return stats


class EvilEye(AbstractNegative):

    def get_negative_effects(self):
        return self.base.get_negative_effects() + ['EvilEye']

    def get_stats(self):
        stats = self.base.get_stats()
        stats['Luck'] -= 10
        return stats


hero = Hero()
print(hero.get_stats() == {'HP': 128, 'MP': 42, 'SP': 100, 'Strength': 15, 'Perception': 4, 'Endurance': 8, 'Charisma': 2, 'Intelligence': 3, 'Agility': 8, 'Luck': 1})
print(hero.stats == {'HP': 128, 'MP': 42, 'SP': 100, 'Strength': 15, 'Perception': 4, 'Endurance': 8, 'Charisma': 2, 'Intelligence': 3, 'Agility': 8, 'Luck': 1})
print(hero.get_negative_effects() == [ ])
print(hero.get_positive_effects() == [ ])
brs1 = Berserk(hero)
print(brs1.get_stats() == {'HP': 178, 'MP': 42, 'SP': 100, 'Strength': 22, 'Perception': 1, 'Endurance': 15, 'Charisma': -1, 'Intelligence': 0, 'Agility': 15, 'Luck': 8})
print(brs1.get_negative_effects() == [ ])
print(brs1.get_positive_effects() == ['Berserk'])
brs2 = Berserk(brs1)
cur1 = Curse(brs2)
print(cur1.get_stats() == {'HP': 228, 'MP': 42, 'SP': 100, 'Strength': 27, 'Perception': -4, 'Endurance': 20, 'Charisma': -6, 'Intelligence': -5, 'Agility': 20, 'Luck': 13})
print(cur1.get_positive_effects() == ['Berserk', 'Berserk'])
print(cur1.get_negative_effects() == ['Curse'])
cur1.base = brs1
print(cur1.get_stats() == {'HP': 178, 'MP': 42, 'SP': 100, 'Strength': 20, 'Perception': -1, 'Endurance': 13, 'Charisma': -3, 'Intelligence': -2, 'Agility': 13, 'Luck': 6})
print(cur1.get_positive_effects() == ['Berserk'])
print(cur1.get_negative_effects() == ['Curse'])
