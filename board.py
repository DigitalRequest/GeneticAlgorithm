import pygame
import random


class Board:
    def __init__(self, genes=None):

        self.randomize()

        if genes:
            self.genes = genes
        else:
            self.genes = [random.random(), random.random()] +  [stone for stone in self.stones]

    def move_tile(self):
        chanceX = random.random()
        chanceY = random.random()
        target_x, target_y = 0, 0

        if chanceX < self.genes[0]:
            target_x = random.randint(0, 7)
        if chanceY < self.genes[1]:
            target_y = random.randint(0, 7)

        return target_x * target_y

    def randomize(self):
        self.score = 0
        self.margin = 150
        self.tile_size = 50
        self.board_size = 8
        self.board = []
        self.stones = []
        self.stones_rect = []

        for y in range(self.board_size):
            for x in range(self.board_size):
                choice = [0, 1]
                stone = random.choice(choice)
                tile = pygame.Rect(self.margin + ((self.tile_size + 10) * x),
                                   self.margin - 100 +
                                   ((self.tile_size + 10) * y),
                                   self.tile_size, self.tile_size)
                stone_tile = pygame.Rect(self.margin + ((self.tile_size + 10) *
                                                        x + (self.tile_size / 4)),
                                         self.margin - 100 +
                                         ((self.tile_size + 10) *
                                          y + (self.tile_size / 4)),
                                         self.tile_size / 2, self.tile_size / 2)
                self.stones.append(stone)
                self.board.append(tile)
                if stone:
                    self.stones_rect.append(stone_tile)
                else:
                    self.stones_rect.append(0)
