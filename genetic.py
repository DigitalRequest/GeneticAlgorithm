import random
import os

class GeneticAlgo:
    def __init__(self, n_pop, cross_rate, mut_rate):
        self.n_pop = n_pop
        self.c_rate = cross_rate
        self.m_rate = mut_rate

    def init_pop(self, players):
        self.population = []
        for player in players:
            self.population.append(player)

    def calc_scores(self):
        self.scores = []
        for board in self.population:
            self.scores.append(board.score)

    def selection(self):
        i_selected = random.randint(0, self.n_pop - 1)
        for i in range(len(self.scores)):
            if self.scores[i_selected] < self.scores[i]:
                i_selected = i

        return self.population[i_selected].genes

    def crossover(self, par1, par2):
        child1, child2 = par1.copy(), par2.copy()
        if random.random() < self.c_rate:
            gens_point = random.randint(1, len(par1) - 1)
            child1 = par1[:gens_point] + par2[:gens_point]
            child2 = par2[:gens_point] + par1[:gens_point]

        return [child1, child2]

    def mutation(self, child):
        chance = random.random()
        if chance < (self.m_rate / 10):
            for x in range(len(child)):
                child[x] = random.random()
        elif random.random() < self.m_rate:
            child.reverse()
        
        return child
    
    def get_genes(self):
        genes = []
        for board in self.population:
            genes.append(board.genes)
        
        return genes

    def evolve(self):
        self.calc_scores()
        parent1 = self.selection()
        parent2 = self.selection()

        childrens = self.crossover(parent1, parent2)

        for board in self.population:
            child = random.choice(childrens)
            child = self.mutation(child)
            board.genes = child
            board.randomize()

        os.system('cls' if os.name == 'nt' else 'clear')
        print("Genes: ", parent1, "Score: ", sum(self.scores) / self.n_pop, end="\r", flush=True)

        return self.population
