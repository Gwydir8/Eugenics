#include "genetic.h"

#include <string>
#include <iostream>
#include <functional>
#include <utility>    // std::map
#include <algorithm>  // std::max
#include <random>     // std::mt19937

#include <cstdio>
#include <cstdlib>
#include <ctime>  // std::time

#include "utility.h"

using std::string;
using std::vector;
using std::srand;
using std::rand;
using std::time;

// don't we want this in genetic.h (outside class)? if it's here it's global

Genetic::Genetic(int n, BooleanTable outputs) : population(), input_no(n), expected_outputs(outputs), rand_engine(std::random_device{}()){ population = spawnPopulation(1000);}

Genetic::Genetic(int n, BooleanTable outputs, std::map<int, Circuit> pop)
    : population(pop), input_no(n), expected_outputs(outputs), rand_engine(std::random_device{}()) { };
Genetic::~Genetic() {}

int Genetic::fitness(GeneticCircuit c) {
  int score = 0;
  score += c.getNotCount() * 10000;
  score += c.getAndCount() * 10;
  score += c.getOrCount() * 1;
  return score;
}

void Genetic::split(Circuit c1, Circuit c2) {
  /* dist(input_no, outputs.front().size()); */
  for (int i = 0; i < 50; ++i) {
    std::cout << dist(rand_engine) << '\n';

    std::string errmsg = "Genetic::split: random number: " +
                         std::to_string(dist(rand_engine));
    errlog(errmsg);
  }
}

void Genetic::splice() {}

void Genetic::splitAndSplice() {}

size_t hash_circ(GeneticCircuit c) {
  string s = "";
  std::hash<std::string> hash_fn;
  BooleanTable outputs = c.evaluateAllInputs();
  for (vector<bool> row : outputs) {
    for (bool bit : row) {
      s += std::to_string((int)bit);
    }
  }

  std::cout << "Hashing..." << std::endl;
  size_t hash = hash_fn(s);
  std::cout << "Done Hashing..: " << hash << std::endl;
  std::cout << " NOT: " << c.getNotCount() << " AND: " << c.getAndCount()
            << " OR: " << c.getOrCount();
  return hash;
}

std::map<int, Circuit> Genetic::spawnPopulation(int populationSize) {
  srand(time(NULL));

  std::map<int, Circuit> spawned_pop;
  for (int i = 0; i < populationSize; ++i) {
    GeneticCircuit c = GeneticCircuit(input_no, expected_outputs[0].size());
    int circuit_fitness = fitness(c);
    c.setFitness(circuit_fitness);

    std::string errmsg =
        "Genetic::spawnPopulation inserting Circuit fitness: " +
        std::to_string(circuit_fitness);
    errlog(errmsg);

    std::pair<int, Circuit> zergling(hash_circ(c), c);
    spawned_pop.insert(zergling);
  }
  return spawned_pop;
}
