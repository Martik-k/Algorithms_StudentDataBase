#ifndef HM1_BENCHMARK_H
#define HM1_BENCHMARK_H

#include <vector>
#include "common/Student.h"
#include "interface/StudentDB.h"
#include "sorting/sorting.h"
#include <unordered_set>

class Benchmark {
private:
    static std::vector<Student> pick_random(const std::vector<Student>& students, size_t n);
    static std::vector<std::vector<std::string>> get_groups_emails(const std::vector<Student>& students);
    static std::pair<int,long> run_experiments_for_db_size(StudentDB& db, const std::vector<Student>& students,
                                  size_t sample_size,
                                  const double time_limit_sec, std::vector<std::string>& groups,
                                  std::vector<std::string>& emails);
    static std::vector<std::pair<int,long>> run_experiments_for_db(StudentDB& db, std::vector<Student>& students,
                           const std::vector<size_t>& sample_sizes,
                           const double time_limit_sec, std::vector<std::string>& groups,
                           std::vector<std::string>& emails);
public:
    static std::vector<std::vector<int>> run_all_experiments(std::vector<Student>& students, std::vector<size_t> sample_sizes,
                                    const double time_limit_sec);
    static std::vector<double> run_sorting_experiments(std::vector<Student>& students);
};

#endif //HM1_BENCHMARK_H