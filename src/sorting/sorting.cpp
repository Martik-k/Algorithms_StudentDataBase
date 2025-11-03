#include "sorting.h"
#include <algorithm>

using buckets = std::vector<std::vector<Student>>;

void Sorting::standard_sort(std::vector<Student>& students) {
    std::sort(students.begin(), students.end(),
        [](const Student& a, const Student& b){return a.m_rating < b.m_rating;});
};

void Sorting::merge(std::vector<Student>& students, int left, int right, int mid, std::vector<Student>& buffer) {
    int i = left;
    int j = mid + 1;
    int k = left;

    while (i <= mid && j <= right) {
        if (students[i].m_rating <= students[j].m_rating) {
            buffer[k++] = students[i++];
        } else {
            buffer[k++] = students[j++];
        }
    }
    while (i <= mid) {
        buffer[k++] = students[i++];
    }
    while (j <= right) {
        buffer[k++] = students[j++];
    }
    for (int t = left; t <= right; ++t) {
        students[t] = buffer[t];
    }
}

void Sorting::partition(std::vector<Student>& students, int left, int right, std::vector<Student>& buffer) {
    if  (right <= left) return;
    int mid = (left + right) / 2;
    partition(students, left, mid, buffer);
    partition(students, mid + 1, right, buffer);
    merge(students, left, right, mid, buffer);
}

void Sorting::my_sort(std::vector<Student>& students) {
    std::vector<Student> buffer = students;
    partition(students, 0, students.size() - 1, buffer);
}
