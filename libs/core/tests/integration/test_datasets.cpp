/*
 * This file is part of Cynthia.
 *
 * Cynthia is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Cynthia is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Cynthia.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "integration_tests_utils.hpp"
#include <catch.hpp>
#include <iostream>

namespace cynthia::core::Test {

TEST_CASE("Test GFand patterns", "[integration][gfand]") {
  auto problem = GENERATE(GeneratorWrapper<problem_t>(
      std::make_unique<GFAndDatasetProblemGenerator>()));
  check_realizability(problem);
}

TEST_CASE("Test Uright patterns", "[integration][uright]") {
  auto problem = GENERATE(GeneratorWrapper<problem_t>(
      std::make_unique<URightDatasetProblemGenerator>()));
  check_realizability(problem);
}

TEST_CASE("Test Single-counter", "[integration][single_counter]") {
  // we take only the first three instances, the next ones are too complex for
  // Cynthia
  auto problem = GENERATE(
      take(3, GeneratorWrapper<problem_t>(
                  std::make_unique<SingleCounterDatasetProblemGenerator>())));
  check_realizability(problem);
}
TEST_CASE("Test Double-counter", "[integration][double_counter]") {
  // we take only the first two instances, the next ones are too complex for
  // Cynthia
  auto problem = GENERATE(
      take(2, GeneratorWrapper<problem_t>(
                  std::make_unique<DoubleCounterDatasetProblemGenerator>())));
  check_realizability(problem);
}
TEST_CASE("Test Nim-1", "[integration][nim_1]") {
  auto problem = GENERATE(GeneratorWrapper<problem_t>(
      std::make_unique<Nim1DatasetProblemGenerator>()));
  check_realizability(problem);
}
TEST_CASE("Test Nim-2", "[integration][nim_2]") {
  auto problem =
      GENERATE(take(4, GeneratorWrapper<problem_t>(
                           std::make_unique<Nim2DatasetProblemGenerator>())));
  check_realizability(problem);
}
TEST_CASE("Test Nim-3", "[integration][nim_3]") {
  // we take only the first instance, the next ones are too complex for Cynthia
  auto problem =
      GENERATE(take(1, GeneratorWrapper<problem_t>(
                           std::make_unique<Nim3DatasetProblemGenerator>())));
  check_realizability(problem);
}

TEST_CASE("Test Lydia Random case 03 50", "[integration][lydia_random_03_50]") {
  auto problem = GENERATE(filter(
      tractable_lydia_random_03_50,
      GeneratorWrapper<problem_t>(
          std::make_unique<LydiaRandom_03_50_DatasetProblemGenerator>())));
  check_realizability(problem);
}
} // namespace cynthia::core::Test