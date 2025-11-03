#ifndef HM1_SORTING_H
#define HM1_SORTING_H

#include "common/Student.h"
#include <vector>

class Sorting {
// private:
//     static void insert(std::vector<Student>& students, Student& student);
private:
    static void merge(std::vector<Student>& students, int left, int right, int mid, std::vector<Student>& buffer);
    static void partition(std::vector<Student>& students, int left, int right, std::vector<Student>& buffer);
public:
    // static void insert(std::vector<Student>& students, Student& student);
    static void standard_sort(std::vector<Student>& students);

    static void my_sort(std::vector<Student>& students);
};

#endif //HM1_SORTING_H