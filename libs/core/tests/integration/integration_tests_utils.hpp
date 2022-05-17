#pragma once
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

#include <catch.hpp>
#include <cynthia/core.hpp>
#include <cynthia/input_output_partition.hpp>
#include <cynthia/parser/driver.hpp>
#include <filesystem>
#include <set>

namespace cynthia::core::Test {

// index in the dataset, path to formula, path to partition file, is realizable
typedef std::tuple<size_t, std::filesystem::path, std::filesystem::path, bool>
    problem_t;

void check_realizability(const problem_t& problem) {
  auto logger = utils::Logger("test");

  // unpack input
  size_t index;
  std::filesystem::path formula_path;
  std::filesystem::path partition_path;
  bool expected_realizability;
  std::tie(index, formula_path, partition_path, expected_realizability) =
      problem;

  logger.info("Processing problem " + formula_path.string());

  // prepare input
  auto driver = cynthia::parser::ltlf::LTLfDriver();
  driver.parse(formula_path.c_str());
  auto parsed_formula = driver.get_result();
  auto context = driver.context;
  auto end = context->make_end();
  auto not_end = context->make_not(end);
  parsed_formula = context->make_and({parsed_formula, not_end});
  auto partition =
      cynthia::core::InputOutputPartition::read_from_file(partition_path);

  // compute realizability
  bool actual_realizability =
      cynthia::core::is_realizable<cynthia::core::ForwardSynthesis>(
          parsed_formula, partition);

  REQUIRE(actual_realizability == expected_realizability);
}

template <typename IsRealizableChecker>
class AbstractDatasetProblemGenerator
    : public Catch::Generators::IGenerator<problem_t> {
private:
  std::filesystem::path dataset_folder_;
  int current_index = 0;
  std::vector<problem_t> problems;

public:
  explicit AbstractDatasetProblemGenerator(
      const std::filesystem::path& dataset_folder)
      : dataset_folder_(dataset_folder), current_index{0} {
    assert(is_directory(dataset_folder));

    std::filesystem::directory_iterator directory_iterator{dataset_folder};
    std::vector<std::filesystem::path> files;
    for (auto& entry : directory_iterator)
      files.push_back(entry);
    std::sort(files.begin(), files.end());
    // files are twice the number of instances;
    // an instance has both the formula and the partition files.
    for (auto i = 0; i < files.size(); i += 2) {
      auto formula = files[i];
      auto partition = files[i + 1];
      problems.emplace_back(i / 2, formula, partition,
                            IsRealizableChecker()(i / 2, formula, partition));
    }
  }

  const problem_t& get() const override {
    return this->problems[this->current_index];
  };
  bool next() override {
    this->current_index += 1;
    return this->current_index < problems.size();
  }
};

static const std::filesystem::path
    DATASETS_FOLDER("libs/core/tests/integration/finite-synthesis-datasets");

// to be used as template argument for AbstractDatasetProblemGenerator
struct always_unrealizable {
  bool operator()(size_t index, const std::filesystem::path& formula,
                  const std::filesystem::path& partition) {
    return false;
  }
};

// to be used as template argument for AbstractDatasetProblemGenerator
struct always_realizable {
  bool operator()(size_t index, const std::filesystem::path& formula,
                  const std::filesystem::path& partition) {
    return true;
  }
};

//********************************
// GF-and dataset generator
//********************************

class GFAndDatasetProblemGenerator
    : public AbstractDatasetProblemGenerator<always_unrealizable> {
public:
  GFAndDatasetProblemGenerator()
      : AbstractDatasetProblemGenerator(DATASETS_FOLDER / "Patterns/GFand") {}
};

//********************************
// Uright dataset generator
//********************************

struct is_uright_problem_realizable {
  bool operator()(size_t index, const std::filesystem::path& formula,
                  const std::filesystem::path& partition) {
    // only first is unrealizable
    if (index == 0)
      return false;
    // the other instances are realizable
    return true;
  }
};

class URightDatasetProblemGenerator
    : public AbstractDatasetProblemGenerator<is_uright_problem_realizable> {
public:
  URightDatasetProblemGenerator()
      : AbstractDatasetProblemGenerator(DATASETS_FOLDER / "Patterns/Uright") {}
};

//********************************
// Single-counter dataset generator
//********************************

class SingleCounterDatasetProblemGenerator
    : public AbstractDatasetProblemGenerator<always_realizable> {
public:
  SingleCounterDatasetProblemGenerator()
      : AbstractDatasetProblemGenerator(
            DATASETS_FOLDER / "Two-player-Game/Single-Counter/System-first") {}
};

//********************************
// Double-counter dataset generator
//********************************

class DoubleCounterDatasetProblemGenerator
    : public AbstractDatasetProblemGenerator<always_realizable> {
public:
  DoubleCounterDatasetProblemGenerator()
      : AbstractDatasetProblemGenerator(
            DATASETS_FOLDER / "Two-player-Game/Double-Counter/System-first") {}
};

//********************************
// Nim dataset generator
//********************************

struct is_nim1_problem_realizable {
  bool operator()(size_t index, const std::filesystem::path& formula,
                  const std::filesystem::path& partition) {
    // only first is unrealizable
    if (index == 0)
      return false;
    // the other instances are realizable
    return true;
  }
};
struct is_nim2_problem_realizable {
  bool operator()(size_t index, const std::filesystem::path& formula,
                  const std::filesystem::path& partition) {
    // only first is realizable
    if (index == 0)
      return true;
    // the other instances are unrealizable
    return false;
  }
};

class Nim1DatasetProblemGenerator
    : public AbstractDatasetProblemGenerator<is_nim1_problem_realizable> {
public:
  Nim1DatasetProblemGenerator()
      : AbstractDatasetProblemGenerator(
            DATASETS_FOLDER / "Two-player-Game/Nim/nim_1/System-first") {}
};
class Nim2DatasetProblemGenerator
    : public AbstractDatasetProblemGenerator<is_nim2_problem_realizable> {
public:
  Nim2DatasetProblemGenerator()
      : AbstractDatasetProblemGenerator(
            DATASETS_FOLDER / "Two-player-Game/Nim/nim_2/System-first") {}
};
class Nim3DatasetProblemGenerator
    : public AbstractDatasetProblemGenerator<always_unrealizable> {
public:
  Nim3DatasetProblemGenerator()
      : AbstractDatasetProblemGenerator(
            DATASETS_FOLDER / "Two-player-Game/Nim/nim_3/System-first") {}
};
class Nim4DatasetProblemGenerator
    : public AbstractDatasetProblemGenerator<always_unrealizable> {
public:
  Nim4DatasetProblemGenerator()
      : AbstractDatasetProblemGenerator(
            DATASETS_FOLDER / "Two-player-Game/Nim/nim_4/System-first") {}
};
class Nim5DatasetProblemGenerator
    : public AbstractDatasetProblemGenerator<always_unrealizable> {
public:
  Nim5DatasetProblemGenerator()
      : AbstractDatasetProblemGenerator(
            DATASETS_FOLDER / "Two-player-Game/Nim/nim_5/System-first") {}
};

//********************************
// Lydia Random dataset generator, case 03-50
//********************************

static const std::map<size_t, bool> case_03_50_realizability = {
    {1, false},  {2, false},  {3, false},  {4, false},  {5, true},
    {6, false},  {7, true},   {8, false},  {9, false},  {10, false},
    {11, true},  {12, false}, {13, false}, {14, true},  {15, true},
    {16, true},  {17, false}, {18, false}, {19, false}, {20, true},
    {21, true},  {22, false}, {23, true},  {24, false}, {25, false},
    {26, false}, {27, true},  {28, false}, {29, true},  {30, true},
    {31, false}, {32, false}, {33, false}, {34, false}, {35, true},
    {36, true},  {37, false}, {38, false}, {39, true},  {40, false},
    {41, false}, {42, true},  {43, true},  {44, false}, {45, false},
    {46, true},  {47, false}, {48, true},  {49, true},  {50, true}};
static const std::set<size_t> case_03_50_tractable_instances = {
    1,  4,  5,  7,  8,  9,  11, 12, 14, 15, 16, 18, 19, 20, 21, 22, 23,
    24, 27, 28, 31, 33, 34, 36, 39, 42, 43, 44, 45, 46, 47, 48, 49, 50};

struct is_case_03_50_problem_realizable {
  bool operator()(size_t index, const std::filesystem::path& formula,
                  const std::filesystem::path& partition) {
    return case_03_50_realizability.at(index + 1);
  }
};
class LydiaRandom_03_50_DatasetProblemGenerator
    : public AbstractDatasetProblemGenerator<is_case_03_50_problem_realizable> {
public:
  LydiaRandom_03_50_DatasetProblemGenerator()
      : AbstractDatasetProblemGenerator(DATASETS_FOLDER /
                                        "Random/Lydia/case_03_50") {}
};

bool tractable_lydia_random_03_50(const problem_t& problem) {
  return case_03_50_tractable_instances.find(std::get<0>(problem) + 1) !=
         case_03_50_tractable_instances.end();
}

} // namespace cynthia::core::Test
