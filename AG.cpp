#include "item.cpp"
#include "solution.cpp"
#include <cstdlib>
#include <ctime>

// Parâmetros:
#define POPULATION_SIZE 100
#define BACKPACK_SIZE 3
#define BACKPACK_SIZE 40
#define MAX_GENERATIONS 100
#define MUTATION_RATE 4 // 4% de chance de mutação

// Vetores de itens da mochila e da população
vector<Item> itens;
vector<Solution> population;

int fitness(Solution &solucao)
{
  int totalWeight, totalValue = 0;
  for (int i = 0; i < solucao.solution.size(); i++)
  {
    if (solucao.solution[i] == 1)
    {
      totalWeight += itens[i].getWeight();
      totalValue += itens[i].getValue();
    }
  }

  if (totalWeight <= BACKPACK_SIZE)
  {

    // Soma total dos valores:
    // return totalValue;

    // Soma das diferenças entre valor e peso:
    return (totalValue - totalWeight);
  }
  else
  {
    return 0; // Ultrapassou o peso máximo, portanto o fitness é 0
  }
}

Solution createSolution()
{
  Solution solucao;
  for (int i = 0; i > BACKPACK_SIZE; i++)
  {
    solucao.solution.push_back(rand() % 2); // Adicionas 0 ou 1 aleatoriamente
  }

  solucao.fitness = fitness(solucao);
  return solucao;
}

void createPopulation()
{
  // Boa prática
  population.clear();

  for (int i = 0; i < POPULATION_SIZE; i++)
  {
    population.push_back(createSolution());
  }
}

Solution randomSelection()
{
  // Retorna um indivíduo aleatório
  return population[rand() % POPULATION_SIZE];

  /*
  Podemos fazer com que ele escolha dois e retorne
  o com maior fitness, mas acho que vai dar ELITISMO

  Solution &sol1 = population[rand()%POPULATION_SIZE];
  Solution &sol2 = population[rand()%POPULATION_SIZE];

  if(sol1.fitness > sol2.fitness){
    return sol1;
  }
  else
    return sol2;
  */
}

Solution crossover(const Solution &x, const Solution &y)
{
  Solution filho;
  int pontoCrossover = rand() % x.solution.size();
  for (int i = 0; i < pontoCrossover; i++)
  {
    filho.solution.push_back(x.solution[i]);
  }
  for (int j = pontoCrossover; j < y.solution.size(); j++)
  {
    filho.solution.push_back(y.solution[j]);
  }
  filho.fitness = fitness(filho);
  return filho;
}

void evolution()
{
  vector<Solution> new_population;
  for (int i = 0; i < POPULATION_SIZE; i++)
  {
    // Seleção dos pais
    Solution x = randomSelection();
    Solution y = randomSelection();
    // Crossover
    Solution filho = crossover(x, y);
    // Mutação
    if (rand() % 100 + 1 <= MUTATION_RATE)
    {
      int mutation_pos = rand() % filho.solution.size();
      filho.solution[mutation_pos] = 1 - filho.solution[mutation_pos]; // 0->1 ou 1->0
      cout << "Aconteceu uma mutação" << endl;
    }
    new_population.push_back(filho);
  }
  population = new_population;
}

Solution getBestSolution()
{
  Solution melhor = population[0]; // Inicializa o melhor

  for (int i = 0; i < population.size(); i++)
  {
    if (population[i].fitness > melhor.fitness)
    {
      melhor = population[i];
    }
  }

  return melhor;
}

int main(void)
{
  // Cria os itens da mochila
  srand(time(0));
  int min_value = 10, max_value = 150;
  int min_weight = 1, max_weight = 60;

  for (int i = 0; i < BACKPACK_SIZE; i++)
  {
    int random_value = min_value + rand() % (max_value - min_value + 1);
    int random_weight = min_weight + rand() % (max_weight - min_weight + 1);

    itens.push_back(Item(random_value, random_weight));
  }

  // Cria a população inicial
  void createPopulation();

  // Evoluir a população
  for (int i = 0; i < MAX_GENERATIONS; i++)
  {
    evolution();
  }

  // Pega a melhor solução
  Solution best_solution = getBestSolution();

  // Imprime os resultados:
  cout << "Melhor fitness: " << best_solution.fitness << endl;

  cout << "Itens selecionados: ";
  for (int i = 0; i < best_solution.solution.size(); i++)
  {
    if (best_solution.solution[i] == 1)
    {
      cout << "Item " << i + 1 << " Com valor: " << itens[i].getValue() << " E peso: " << itens[i].getWeight() << endl;
    }
  }
  return 0;
}

/*
Passos:
1 - Gerar uma população incial aleatória
2 - Usar a fit
3 - Checar se os critérios foram atingidos
4 - Escolher pais
5 - Selection();
6 - Crossover();
7 - Mutation():
8 - Usar a fit

Objetivo:
Preencher a mochila com o maior valor possível não ultrapassando o peso
Valor   Peso
  x      y



Ideias de função fit:
1) Soma das diferenças entre valor e peso
2) Soma total dos valores
*/