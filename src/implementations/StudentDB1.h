#ifndef HM1_STUDENTDB1_H
#define HM1_STUDENTDB1_H

#include <unordered_map>
#include "interface/StudentDB.h"

class StudentDB1 : public StudentDB{
private:
    struct GroupData {
        int count = 0;
        double total_rating = 0.0;
    };

    std::unordered_map<std::string, Student> students_by_email;
    std::unordered_map<std::string, GroupData> groups;

public:
    void load_students(const std::vector<Student>& students) override;

    std::string op1_max_student_group() override;
    void op2_change_group_by_email(const std::string& email, const std::string& new_group) override;
    std::string op3_max_avg_rating_group() override;
};

#endif //HM1_STUDENTDB1_H