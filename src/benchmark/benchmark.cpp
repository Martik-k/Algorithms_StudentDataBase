#include "benchmark.h"
#include "sorting/sorting.h"
#include "implementations/StudentDB1.h"
#include "implementations/StudentDB2.h"
#include "implementations/StudentDB3.h"
#include "common/CsvParser.h"
#include <iostream>
#include <vector>
#include <chrono>
#include <random>
#include <algorithm>
#include <fstream>
#include <string>
#include <thread>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <cstdio>
#include <sys/resource.h>
#include <cstring>
#include <iomanip>

std::vector<Student> Benchmark::pick_random(const std::vector<Student>& students, size_t n) {
    n = std::min(n, students.size());
    std::vector<Student> copy = students;
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(copy.begin(), copy.end(), g);

    std::vector<Student> result;
    result.insert(result.end(), copy.begin(), copy.begin() + n);
    return result;
}

std::vector<std::vector<std::string>> Benchmark::get_groups_emails(const std::vector<Student>& students) {
    std::unordered_set<std::string> groups;
    std::unordered_set<std::string> emails;
    for (const Student& s : students) {
        groups.insert(s.m_group);
        emails.insert(s.m_email);
    }
    std::vector<std::string> vec_gr(groups.begin(), groups.end());
    std::vector<std::string> vec_em(emails.begin(), emails.end());
    return {vec_gr, vec_em};
}

static long get_rss_kb() {
    FILE* f = fopen("/proc/self/status", "r");
    if (f) {
        char line[256];
        while (fgets(line, sizeof(line), f)) {
            if (strncmp(line, "VmRSS:", 6) == 0) {
                long kb = 0;
                if (sscanf(line + 6, "%ld", &kb) == 1) {
                    fclose(f);
                    return kb;
                }
            }
        }
        fclose(f);
    }
    struct rusage usage;
    if (getrusage(RUSAGE_SELF, &usage) == 0) {
        return usage.ru_maxrss;
    }
    return -1;
}

std::pair<int,long> Benchmark::run_experiments_for_db_size(StudentDB& db, const std::vector<Student>& students,
                                  size_t sample_size,
                                  const double time_limit_sec, std::vector<std::string>& groups,
                                  std::vector<std::string>& emails) {
    auto subset = pick_random(students, sample_size);
    int op_count = 0;

    auto start = std::chrono::high_resolution_clock::now();
    std::mt19937 rng(std::random_device{}());
    std::uniform_int_distribution<int> distribution(0, 131);

    long peak_rss_kb = get_rss_kb();
    auto last_sample = start;
    const std::chrono::milliseconds sample_interval(100); // sample every 100 ms

    while (true) {
        int r = distribution(rng);
        if (r < 2) {
            db.op1_max_student_group();
        }
        else if (r < 102) {
            const std::string& em = emails[op_count % emails.size()];
            const std::string& gr = groups[op_count % groups.size()];
            db.op2_change_group_by_email(em, gr);
        }
        else {
            db.op3_max_avg_rating_group();
        }
        ++op_count;

        auto now = std::chrono::high_resolution_clock::now();
        if (now - last_sample >= sample_interval) {
            long rss = get_rss_kb();
            if (rss > peak_rss_kb) peak_rss_kb = rss;
            last_sample = now;
        }
        if (std::chrono::duration<double>(now - start).count() >= time_limit_sec) break;
    }
    {
        long rss = get_rss_kb();
        if (rss > peak_rss_kb) peak_rss_kb = rss;
    }
    return {op_count, peak_rss_kb};
}

std::vector<std::pair<int,long>> Benchmark::run_experiments_for_db(StudentDB& db, std::vector<Student>& students,
                           const std::vector<size_t>& sample_sizes,
                           const double time_limit_sec, std::vector<std::string>& groups,
                           std::vector<std::string>& emails) {
    std::vector<std::pair<int,long>> result;
    for (size_t sample_size : sample_sizes) {
        auto res = run_experiments_for_db_size(db, students, sample_size, time_limit_sec, groups, emails);
        result.push_back(res);
    }
    return result;
}

std::vector<std::vector<int>> Benchmark::run_all_experiments(
    std::vector<Student>& students,
    std::vector<size_t> sample_sizes,
    const double time_limit_sec
) {
    std::vector<std::vector<std::string>> groups_emails = get_groups_emails(students);
    std::vector<std::string> groups = groups_emails[0];
    std::vector<std::string> emails = groups_emails[1];

    StudentDB1 db1;
    db1.load_students(students);
    StudentDB2 db2;
    db2.load_students(students);
    StudentDB3 db3;
    db3.load_students(students);

    auto res1 = run_experiments_for_db(db1, students, sample_sizes, time_limit_sec, groups, emails);
    auto res2 = run_experiments_for_db(db2, students, sample_sizes, time_limit_sec, groups, emails);
    auto res3 = run_experiments_for_db(db3, students, sample_sizes, time_limit_sec, groups, emails);

    std::vector<std::vector<int>> matrix(3, std::vector<int>(sample_sizes.size(), 0));
    std::vector<std::vector<long>> mem_matrix(3, std::vector<long>(sample_sizes.size(), 0));

    for (size_t i = 0; i < sample_sizes.size(); ++i) {
        matrix[0][i] = res1[i].first;
        matrix[1][i] = res2[i].first;
        matrix[2][i] = res3[i].first;

        mem_matrix[0][i] = res1[i].second;
        mem_matrix[1][i] = res2[i].second;
        mem_matrix[2][i] = res3[i].second;
    }

    std::ofstream csv("../src/benchmark/matrix.csv");
    csv << "SampleSize,DB1,DB2,DB3\n";
    for (size_t i = 0; i < sample_sizes.size(); ++i) {
        csv << sample_sizes[i] << ","
            << matrix[0][i] << ","
            << matrix[1][i] << ","
            << matrix[2][i] << "\n";
    }
    csv.close();

    std::ofstream memcsv("../src/benchmark/memory.csv");
    memcsv << "SampleSize,DB1_kB,DB2_kB,DB3_kB\n";
    for (size_t i = 0; i < sample_sizes.size(); ++i) {
        memcsv << sample_sizes[i] << ","
               << mem_matrix[0][i] << ","
               << mem_matrix[1][i] << ","
               << mem_matrix[2][i] << "\n";
    }
    memcsv.close();

    std::ofstream txt("../results/database_analytics.txt");
    if (!txt.is_open()) {
        std::cerr << "Cannot open file for writing analytics." << std::endl;
        return matrix;
    }

    txt << std::left << std::setw(12) << "SampleSize"
        << std::setw(12) << "DB1_ops"
        << std::setw(12) << "DB2_ops"
        << std::setw(12) << "DB3_ops"
        << std::setw(14) << "DB1_mem(kB)"
        << std::setw(14) << "DB2_mem(kB)"
        << std::setw(14) << "DB3_mem(kB)"
        << "\n";

    txt << std::string(88, '-') << "\n";

    for (size_t i = 0; i < sample_sizes.size(); ++i) {
        txt << std::left << std::setw(12) << sample_sizes[i]
            << std::setw(12) << matrix[0][i]
            << std::setw(12) << matrix[1][i]
            << std::setw(12) << matrix[2][i]
            << std::setw(14) << mem_matrix[0][i]
            << std::setw(14) << mem_matrix[1][i]
            << std::setw(14) << mem_matrix[2][i]
            << "\n";
    }
    txt.close();

    return matrix;
}


std::vector<double> Benchmark::run_sorting_experiments(std::vector<Student>& students) {
    std::vector<Student> copy1 = students;
    std::vector<Student> copy2 = students;

    std::vector<double> times;

    auto start1 = std::chrono::high_resolution_clock::now();
    Sorting::standard_sort(copy1);
    auto end1 = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration1 = end1 - start1;
    times.push_back(duration1.count());

    auto start2 = std::chrono::high_resolution_clock::now();
    Sorting::my_sort(copy2);
    auto end2 = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration2 = end2 - start2;
    times.push_back(duration2.count());

    CsvParser::save_to_csv("standard_sort.csv", copy1);
    CsvParser::save_to_csv("my_sort.csv", copy2);

    std::ofstream analytics("../results/sorting_analytics.txt");
    analytics << std::left << std::setw(20) << "Algorithm"
              << std::setw(15) << "Time (s)" << "\n";
    analytics << std::string(35, '-') << "\n";
    analytics << std::left << std::setw(20) << "standard_sort"
              << std::setw(15) << times[0] << "\n";
    analytics << std::left << std::setw(20) << "my_sort"
              << std::setw(15) << times[1] << "\n";
    analytics.close();

    return times;
}

