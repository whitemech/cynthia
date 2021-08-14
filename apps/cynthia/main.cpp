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

#include <CLI/CLI.hpp>

#include <cynthia/core.hpp>
#include <cynthia/logger.hpp>
#include <cynthia/parser/driver.hpp>

int main(int argc, char** argv) {
  cynthia::utils::Logger logger("main");
  cynthia::utils::Logger::level(cynthia::utils::LogLevel::info);

  CLI::App app{"A tool for SDD-based forward LTLf synthesis."};

  bool version = false;
  app.add_flag("-V,--version", version, "Print the version and exit.");

  // options & flags
  std::string filename;
  std::string formula;

  auto format = app.add_option_group("Input format");
  CLI::Option* formula_opt = app.add_option("-i,--inline", formula, "Formula.");
  CLI::Option* file_opt =
      app.add_option("-f,--file", filename, "File to formula.")
          ->check(CLI::ExistingFile);
  formula_opt->excludes(file_opt);
  file_opt->excludes(formula_opt);
  format->add_option(formula_opt);
  format->add_option(file_opt);
  format->require_option(1, 1);

  std::string part_file;
  CLI::Option* part_opt = app.add_option("--part", part_file, "Partition file.")
                              ->check(CLI::ExistingFile);

  CLI11_PARSE(app, argc, argv)

  if (version) {
    std::cout << CYNTHIA_VERSION << std::endl;
    return 0;
  }

  auto driver = cynthia::parser::ltlf::LTLfDriver();
  if (!file_opt->empty()) {
    std::ifstream t(filename.c_str());
    std::string str((std::istreambuf_iterator<char>(t)),
                    std::istreambuf_iterator<char>());
    logger.info("Parsing {}", filename);
    driver.parse(str.c_str());
  } else {
    std::stringstream formula_stream(formula);
    logger.info("Parsing {}", formula);
    driver.parse(formula_stream);
  }

  auto parsed_formula = driver.get_result();

  logger.info("Reading partition file {}", part_file);
  auto partition =
      cynthia::core::InputOutputPartition::read_from_file(part_file);

  auto t_start = std::chrono::high_resolution_clock::now();

  bool result = cynthia::core::is_realizable<cynthia::core::ForwardSynthesis>(
      parsed_formula, partition);
  if (result)
    logger.info("realizable.");
  else
    logger.info("unrealizable.");

  auto t_end = std::chrono::high_resolution_clock::now();
  double elapsed_time =
      std::chrono::duration<double, std::milli>(t_end - t_start).count();
  logger.info("Overall time elapsed: {}ms", elapsed_time);
  return 0;
}
