import pygame
import random
import time
from board import Board
from genetic import GeneticAlgo

pygame.init()
screen_width = 800
screen_height = 600
screen = pygame.display.set_mode((screen_width, screen_height))
clock = pygame.time.Clock()

n_pop = 1000
genetic_algorithm = GeneticAlgo(n_pop, 0.005, 0.001)
boards = []
for _ in range(n_pop):
    board = Board()
    boards.append(board)

genetic_algorithm.init_pop(boards)
max_generation = 20
generation = 0

pygame.display.set_caption("Chess")

running = True
start_time = time.time()
while running:
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            running = False

    screen.fill((0, 0, 0))

    for board in boards[0].board:
        pygame.draw.rect(screen, (255, 255, 255), board)
    for stone in boards[0].stones_rect:
        if stone:
            pygame.draw.rect(screen, (0, 0, 0), stone)

    for board in boards:
        for stone in board.stones:
            if stone:
                tile = board.move_tile()
                if board.stones[tile] == 1:
                    board.score += 1
                else:
                    board.score = 0


    elapsed_time = time.time() - start_time
    minutes = int(elapsed_time // 60)
    seconds = int(elapsed_time % 60)
    milliseconds = int((elapsed_time % 1) * 1000)
    time.sleep(0.001)

    if seconds == 1:
        boards = genetic_algorithm.evolve()
        start_time = time.time()
        generation += 1

    if generation > max_generation:
        f = open('gen.txt', 'w')
        f.write(', '.join(str(elm) for elm in genetic_algorithm.get_genes()))
        f.close()
        running = False

    

    pygame.display.flip()

pygame.quit()
