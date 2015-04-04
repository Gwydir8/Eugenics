#include "../../local/include/gtest/gtest.h"

#include <vector>

#include "genetic.h"
#include "genetic_circuit.h"
#include "circuit.h"
#include "utility.h"

namespace {

class GeneticSetup : public ::testing::Test {
 protected:
  virtual void SetUp() {
    // FUN FACT: dolly was the first cloned sheep! In honor of her...we send our
    // test genetic class into space with her title
    expected_population_size = 1000;
    expected_i = 3;
    expected_o = {{true, true, true},
                  {true, true, false},
                  {true, false, true},
                  {true, false, false},
                  {false, true, true},
                  {false, true, false},
                  {false, false, true},
                  {false, false, false}};
    dolly = Genetic(expected_i, expected_o);
  }

  Genetic dolly;
  unsigned long expected_population_size;
  int expected_i;
  BooleanTable expected_o;
};

TEST_F(GeneticSetup, Fitness) {
  std::mt19937 rand(std::random_device{}());
  GeneticCircuit c(3, 3, &rand);
  // It's expect EXPECTED is less than ACTUAL
  EXPECT_LT(0, c.generateFitness());
}

TEST_F(GeneticSetup, ConstructorTest) {
  EXPECT_EQ(expected_i, dolly.getExpectedInputs());
  EXPECT_EQ(expected_o, dolly.getExpectedOutputs());
  EXPECT_EQ(expected_population_size, dolly.getPopulation().size());
}

TEST_F(GeneticSetup, Splice) {
  EXPECT_EQ(expected_population_size, dolly.getPopulation().size());
  // dolly.split(dolly.getPopulation()[3], dolly.getPopulation()[900]);
}

}  // end of namespace

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
