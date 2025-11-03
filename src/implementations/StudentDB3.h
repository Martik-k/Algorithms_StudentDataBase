//
// Created by marta on 11/2/25.
//

#ifndef HM1_STUDENTDB3_H
#define HM1_STUDENTDB3_H

#include <unordered_map>
#include "interface/StudentDB.h"

class StudentDB3 : public StudentDB{
private:
    std::vector<Student> students;

public:
    void load_students(const std::vector<Student>& students_) override;

    std::string op1_max_student_group() override;
    void op2_change_group_by_email(const std::string& email, const std::string& new_group) override;
    std::string op3_max_avg_rating_group() override;
};


#endif //HM1_STUDENTDB3_H