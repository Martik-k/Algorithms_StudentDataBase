//
// Created by marta on 11/2/25.
//

#ifndef HM1_STUDENTDB2_H
#define HM1_STUDENTDB2_H

#include <map>
#include "interface/StudentDB.h"

class StudentDB2 : public StudentDB{
private:
    struct GroupData {
        int count = 0;
        double total_rating = 0.0;
    };

    std::map<std::string, Student> students_by_email;
    std::map<std::string, GroupData> groups;

public:
    void load_students(const std::vector<Student>& students) override;

    std::string op1_max_student_group() override;
    void op2_change_group_by_email(const std::string& email, const std::string& new_group) override;
    std::string op3_max_avg_rating_group() override;
};

#endif //HM1_STUDENTDB2_H