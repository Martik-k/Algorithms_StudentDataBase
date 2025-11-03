#pragma once

#ifndef HM1_CSVPARSER_H
#define HM1_CSVPARSER_H

#include <string>
#include <vector>
#include "Student.h"

class CsvParser {
public:
    static std::vector<Student> load_from_csv(const std::string& filename);
    static void save_to_csv(const std::string& filename, const std::vector<Student>& students);
};

#endif //HM1_CSVPARSER_H