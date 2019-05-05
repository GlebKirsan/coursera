import os

BASE_SPRITE_SIZE = 60
SCREEN_DIM = (800, 600)
KEYBOARD_CONTROL = True
OBJECT_TEXTURE = os.path.join("texture", "objects")
ENEMY_TEXTURE = os.path.join("texture", "enemies")
ALLY_TEXTURE = os.path.join("texture", "ally")
OTHER_TEXTURE = os.path.join("texture", "")
WALL = "wall"
FLOOR = "floor"

LIST_STATS = ("strength", "endurance", "intelligence", "luck")
MAP_TEXTURES = """textures:
  wall:
    sprite: [wall.png]
  floor:
    sprite: [Ground_1.png,Ground_2.png,Ground_3.png]
  hero:
    sprite: [Hero.png]
"""

BASE_STATS = {
    "strength": 20,
    "endurance": 20,
    "intelligence": 5,
    "luck": 5
}

COLORS = {
    "black": (0, 0, 0, 255),
    "white": (255, 255, 255, 255),
    "red": (255, 0, 0, 255),
    "green": (0, 255, 0, 255),
    "blue": (0, 0, 255, 255),
    "wooden": (153, 92, 0, 255),
}