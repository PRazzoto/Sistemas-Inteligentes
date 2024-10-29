#include "item.cpp"
#include "solution.cpp"
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <vector>
#include <iostream>
#include <algorithm>

using namespace std;

// Parametros
#define POPULATION_SIZE 100
#define QUANTITY_ITENS 20
#define BACKPACK_SIZE 101
#define MAX_GENERATIONS 2500 // Normalmente é 1000
#define MUTATION_RATE 2      // 2% de chance de mutacao
#define ITERATIONS 1000

// Vetores de itens da mochila e da populacao
vector<Item> itens;
vector<Solution> population;

int fitness(Solution &solucao)
{
  int totalWeight = 0;
  int totalValue = 0;
  for (int i = 0; i < solucao.solution.size(); i++)
  {
    if (solucao.solution[i] == 1)
    {
      totalWeight += itens[i].getWeight();
      totalValue += itens[i].getValue();
    }
  }
  int fitness_value = totalValue - totalWeight;
  if (totalWeight <= BACKPACK_SIZE && fitness_value >= 0)
  {
    return fitness_value;
  }
  else
  {
    return 1; // Ultrapassou o peso maximo, portanto o fitness e 1
  }
}

// Função para verificar se uma solução está dentro da capacidade da mochila
bool isWithinCapacity(const Solution &solucao)
{
  int totalWeight = 0;
  for (int i = 0; i < solucao.solution.size(); i++)
  {
    if (solucao.solution[i] == 1)
    {
      totalWeight += itens[i].getWeight();
    }
  }
  return totalWeight <= BACKPACK_SIZE;
}

// Encontra a melhor solução dentro da capacidade da mochila
Solution findBestValidSolution()
{
  // Ordena a população com base no fitness em ordem decrescente
  sort(population.begin(), population.end(), [](const Solution &a, const Solution &b)
       { return a.fitness > b.fitness; });

  // Procura a melhor solução dentro da capacidade
  for (const Solution &sol : population)
  {
    if (isWithinCapacity(sol))
    {
      return sol; // Retorna a primeira solução válida encontrada
    }
  }

  // Se nenhuma solução válida for encontrada, retorna uma solução padrão
  Solution fallback_solution;
  fallback_solution.fitness = 1;
  fallback_solution.solution = vector<int>(QUANTITY_ITENS, 0); // Genes todos 0
  return fallback_solution;
}

Solution createSolution()
{
  Solution solucao;
  for (int i = 0; i < QUANTITY_ITENS; i++)
  {
    // solucao.solution.push_back(rand() % 2); // Adiciona 0 ou 1 aleatoriamente
    solucao.solution.push_back(1);
  }
  solucao.fitness = fitness(solucao);
  return solucao;
}

void createPopulation()
{
  population.clear();
  for (int i = 0; i < POPULATION_SIZE; i++)
  {
    population.push_back(createSolution());
  }

  if (population.empty())
  {
    cerr << "Erro: Populacao inicial esta vazia." << endl;
    exit(1);
  }
}

pair<Solution, Solution> rouletteSelection(vector<Solution> population)
{
  if (population.empty())
  {
    cerr << "Erro: Tentativa de selecao com populacao vazia." << endl;
    exit(1);
  }

  int totalFitness = 0;
  for (const Solution &ind : population)
  {
    totalFitness += ind.fitness;
  }

  pair<Solution, Solution> parents;
  for (int parentIdx = 0; parentIdx < 2; parentIdx++)
  {
    int randomValue = rand() % totalFitness;
    int partialSum = 0;
    bool parentSelected = false;

    for (size_t i = 0; i < population.size(); i++)
    {
      partialSum += population[i].fitness;
      if (partialSum >= randomValue)
      {
        if (parentIdx == 0)
          parents.first = population[i];
        else
          parents.second = population[i];

        totalFitness -= population[i].fitness;
        population.erase(population.begin() + i);
        parentSelected = true;
        break;
      }
    }

    if (!parentSelected)
    {
      cerr << "Erro: Nao foi possivel selecionar um pai na roleta." << endl;
      exit(1);
    }
  }

  return parents;
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

void evolution(Solution &best_solution, int &generation_found, Solution &last_solution_in_last_generation)
{
  vector<Solution> new_population;
  int best_fitness = best_solution.fitness;

  for (int i = 0; i < POPULATION_SIZE; i++)
  {
    pair<Solution, Solution> parents = rouletteSelection(population);
    Solution filho = crossover(parents.first, parents.second);

    if (rand() % 100 < MUTATION_RATE)
    {
      int mutation_pos = rand() % filho.solution.size();
      filho.solution[mutation_pos] = 1 - filho.solution[mutation_pos];
    }

    filho.fitness = fitness(filho);
    new_population.push_back(filho);

    // Atualiza a melhor solucao global apenas se estiver dentro da capacidade
    if (isWithinCapacity(filho) && filho.fitness > best_fitness)
    {
      best_solution = filho;
      best_fitness = filho.fitness;
      generation_found = i;
    }

    // Armazena a ultima solucao gerada na ultima geracao
    if (i == POPULATION_SIZE - 1)
    {
      last_solution_in_last_generation = filho;
    }
  }
  population = new_population;

  if (population.empty())
  {
    cerr << "Erro: Populacao apos evolucao esta vazia." << endl;
    exit(1);
  }
}

void loadItemsFromFile(const string &filename)
{
  ifstream file(filename);
  if (!file.is_open())
  {
    cerr << "Erro ao abrir o arquivo " << filename << endl;
    exit(1);
  }

  int value, weight;
  itens.clear();
  while (file >> value >> weight)
  {
    itens.push_back(Item(value, weight));
  }
  file.close();

  if (itens.empty())
  {
    cerr << "Erro: Nenhum item foi carregado a partir do arquivo." << endl;
    exit(1);
  }
}

int main(void)
{
  srand(time(NULL));
  loadItemsFromFile("itens.txt");

  // Abre o arquivo de saída para salvar os resultados
  ofstream outputFile("resultados1.txt");
  if (!outputFile.is_open())
  {
    cerr << "Erro ao abrir o arquivo de saida resultados.txt." << endl;
    exit(1);
  }

  for (int exec = 1; exec <= ITERATIONS; exec++)
  {
    cout << "\nExecucao " << exec << ":\n";

    createPopulation(); // Reinicia a populacao inicial aleatoriamente em cada iteracao
    Solution best_solution = findBestValidSolution();
    Solution last_solution_in_last_generation;
    int generation_found = 0;

    for (int i = 0; i < MAX_GENERATIONS; i++)
    {
      evolution(best_solution, generation_found, last_solution_in_last_generation);
    }

    int totalValue = 0, totalWeight = 0;
    for (int i = 0; i < best_solution.solution.size(); i++)
    {
      if (best_solution.solution[i] == 1)
      {
        totalValue += itens[i].getValue();
        totalWeight += itens[i].getWeight();
      }
    }

    int lastGenValue = 0, lastGenWeight = 0;
    for (int i = 0; i < last_solution_in_last_generation.solution.size(); i++)
    {
      if (last_solution_in_last_generation.solution[i] == 1)
      {
        lastGenValue += itens[i].getValue();
        lastGenWeight += itens[i].getWeight();
      }
    }

    // Caso especial: se a solução retornada for a padrão
    if (best_solution.fitness == 1 && totalValue == 0 && totalWeight == 0)
    {
      cout << "1 | 0 | 0 | 0 | " << lastGenValue << ", " << lastGenWeight << endl;
      outputFile << "1 | 0 | 0 | 0 | " << lastGenValue << ", " << lastGenWeight << endl;
    }
    else
    {
      // Imprime no terminal
      cout << best_solution.fitness << " | " << totalValue << " | " << totalWeight << " | "
           << generation_found << " | " << lastGenValue << ", " << lastGenWeight << endl;

      // Salva no arquivo
      outputFile << best_solution.fitness << " | " << totalValue << " | " << totalWeight << " | "
                 << generation_found << " | " << lastGenValue << ", " << lastGenWeight << endl;
    }
  }

  // Fecha o arquivo de saida
  outputFile.close();
  cout << "\nResultados salvos no arquivo resultados.txt\n";

  return 0;
}

/*
Passos:
1 - Gerar uma populacao incial aleatoria
2 - Usar a fit
3 - Checar se os criterios foram atingidos
4 - Escolher pais
5 - Selection();
6 - Crossover();
7 - Mutation():
8 - Usar a fit

Objetivo:
Preencher a mochila com o maior valor possivel nao ultrapassando o peso
Valor   Peso
  x      y



Ideias de funcao fit:
1) Soma das diferencas entre valor e peso
2) Soma total dos valores


Melhor Fitness|Soma dos Valores(melhor)|Soma dos Pesos(melhor)|Iteracao que a solucao foi achada| Valor e Peso da ultima iteracao
*/