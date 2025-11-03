#include "StudentDB3.h"

#include <iostream>
#include <unordered_map>

void StudentDB3::load_students(const std::vector<Student>& students_) {
    students = students_;
};

std::string StudentDB3::op1_max_student_group() {
    std::unordered_map<std::string, int> groups;
    for (const Student& student : students) {
        groups[student.m_group]++;
    }
    int max_student_count = 0;
    std::string max_student_group;
    for (const auto& [group, count] : groups) {
        if (count > max_student_count) {
            max_student_count = count;
            max_student_group = group;
        }
    }
    return max_student_group;
};

void StudentDB3::op2_change_group_by_email(const std::string& email, const std::string& new_group) {
    for (Student student : students) {
        if (student.m_email == email) {
            student.m_group = new_group;
            break;
        }
    }
};


std::string StudentDB3::op3_max_avg_rating_group() {
    std::unordered_map<std::string, std::pair<double, int>> groups;
    for (const Student& student : students) {
        groups[student.m_group].first += student.m_rating;
        groups[student.m_group].second++;
    }
    double max_student_avg = 0;
    std::string max_student_group;
    for (const auto& [group, info] : groups) {
        double rating = info.first;
        int count = info.second;
        if (count != 0 && rating / count > max_student_avg) {
            max_student_avg = rating / count;
            max_student_group = group;
        }
    }
    return max_student_group;
};
