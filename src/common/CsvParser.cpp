#include "CsvParser.h"

#include <fstream>
#include <stdexcept>
#include <sstream>
#include <iostream>
#include <algorithm>

std::vector<Student> CsvParser::load_from_csv(const std::string &filename_) {
    std::vector<Student> students;
    std::string filename = std::string("../data/") + filename_;
    std::ifstream file(filename);

    if (!file) {
        throw std::runtime_error("Cannot open file " + filename);
    }

    std::string line;
    std::getline(file, line);

    while (std::getline(file, line)) {
        if (line.empty()) continue;

        std::stringstream ss(line);
        Student student;
        std::string save;

        std::getline(ss, student.m_name, ',');
        std::getline(ss, student.m_surname, ',');
        std::getline(ss, student.m_email, ',');

        std::getline(ss, save, ',');
        student.m_birth_year = std::stoi(save);

        std::getline(ss, save, ',');
        student.m_birth_month = std::stoi(save);

        std::getline(ss, save, ',');
        student.m_birth_day = std::stoi(save);

        std::getline(ss, student.m_group, ',');

        std::getline(ss, save, ',');
        student.m_rating = std::stof(save);

        std::getline(ss, student.m_phone_number);

        students.push_back(student);
    }

    file.close();
    return students;
}

void CsvParser::save_to_csv(const std::string& filename_, const std::vector<Student>& students) {
    std::string filename = std::string("../results/") + filename_;
    std::ofstream file(filename);

    if (!file.is_open()) {
        throw std::runtime_error("Cannot open file " + filename);
        return;
    }

    file << "name,surname,email,birth_year,birth_month,birth_day,group,rating,phone_number\n";

    for (const Student& student : students) {
        file << student.m_name << ",";
        file << student.m_surname << ",";
        file << student.m_email << ",";
        file << student.m_birth_year << ",";
        file << student.m_birth_month << ",";
        file << student.m_birth_day << ",";
        file << student.m_group << ",";
        file << student.m_rating << ",";
        file << student.m_phone_number << "\n";
    }

    file.close();
}
