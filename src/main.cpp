#include "common/CsvParser.h"
#include "common/Student.h"
#include "sorting/sorting.h"
#include "benchmark/benchmark.h"

#include <fstream>
#include <iostream>

int main() {
    std::string input_name = "students.csv";
    std::vector<Student> all_students = CsvParser::load_from_csv(input_name);

    double time_limit = 10.0;
    std::vector<size_t> sample_sizes = {100, 1000, 10000, 100000};
    std::vector<std::vector<int>> matrix = Benchmark::run_all_experiments(all_students, sample_sizes, time_limit);

    std::vector<double> times = Benchmark::run_sorting_experiments(all_students);

}
