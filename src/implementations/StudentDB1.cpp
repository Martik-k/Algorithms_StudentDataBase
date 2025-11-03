#include "StudentDB1.h"

#include <iostream>

void StudentDB1::load_students(const std::vector<Student>& students) {
    for (const Student& student : students) {
        students_by_email[student.m_email] = student;
        groups[student.m_group].count++;
        groups[student.m_group].total_rating += student.m_rating;
    }
};

std::string StudentDB1::op1_max_student_group() {
    std::string max_student_group;
    auto max_student_group_data = GroupData{0, 0};
    for (const auto& [name, data]: groups) {
        if (data.count > max_student_group_data.count) {
            max_student_group = name;
            max_student_group_data = data;
        }
    }
    return max_student_group;
};

void StudentDB1::op2_change_group_by_email(const std::string& email, const std::string& new_group) {
    const std::string old_group = students_by_email.at(email).m_group;
    students_by_email[email].m_group = new_group;

    groups[old_group].count--;
    groups[old_group].total_rating -= students_by_email[email].m_rating;
    groups[new_group].count++;
    groups[new_group].total_rating += students_by_email[email].m_rating;
};


std::string StudentDB1::op3_max_avg_rating_group() {
    std::string max_avg_student_group;
    auto max_avg_student_group_data = GroupData{0, 0};
    for (const auto& [name, data]: groups) {
        if (max_avg_student_group_data.count == 0 ||
            data.total_rating / data.count > max_avg_student_group_data.count) {
            max_avg_student_group = name;
            max_avg_student_group_data = data;
        }
    }
    return max_avg_student_group;
};
