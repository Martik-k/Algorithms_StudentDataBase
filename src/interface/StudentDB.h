#pragma once

#include "common/Student.h"
#include <string>
#include <vector>

class StudentDB {
public:
    virtual ~StudentDB() = default;

    virtual void load_students(const std::vector<Student>& students) = 0;

    virtual std::string op1_max_student_group() = 0;
    virtual void op2_change_group_by_email(const std::string& email, const std::string& new_group) = 0;
    virtual std::string op3_max_avg_rating_group() = 0;
};