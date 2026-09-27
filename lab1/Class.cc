#include "Class.h"
#include <string>
#include <vector>
#include "Student.h"
#include <sstream>
#include <fstream>

std::string Class::toString() const {
    std::ostringstream oss;
    oss << "Class Information:"
        << "\n\tname: " << name
        << "\n\tpoint: " << point
        << std::endl;
    return oss.str();
}

void Class::addStudent(const Student& st) {
    StudentWrapper sw(st.id, st);
    students.push_back(sw);
}

StudentWrapper& Class::getStudentWrapper(const std::string& studentId) {
    for (std::vector<StudentWrapper>::iterator it = students.begin();
            it != students.end();
            ++ it) {
        if (it->id == studentId)
            return *it;
    }
    throw "No Match Student!";
}

double Class::getHighestScore() {
    // TODO implement getHighestScore
    double highestScore = 0.0;
    for (std::vector<StudentWrapper>::iterator it = students.begin();
            it != students.end();
            ++ it) {
            double score = it->getScore();
        if (score > highestScore)
            highestScore = score;
    }
    if (highestScore >= 0.0) {
        return highestScore;
    }
    throw "No Valid Score!";
}

double Class::getLowestScore() {
    // TODO implement getLowestScore
    double lowestScore = 101.0;
    for (std::vector<StudentWrapper>::iterator it = students.begin();
            it != students.end();
            ++ it) {
            double score = it->getScore();
        if (score < lowestScore && score >= 0.0)
            lowestScore = it->getScore();
    }
    if (lowestScore < 101.0) {
        return lowestScore;
    }
    throw "No Valid Score!";
}

double Class::getAvgScore() {
    // TODO implement getAvgScore
    double sum = 0.0;
    int cnt = 0;
    for (std::vector<StudentWrapper>::iterator it = students.begin();
            it != students.end();
            ++ it) {
            double score = it->getScore();
        if (score >= 0.0) {
            sum += score;
            cnt++;
        }
    }
    if (cnt > 0) {
        return sum / cnt;
    }
    throw "No Valid Score!";
}

void Class::saveScore(const std::string& filename) {
    // TODO implement saveScore
    std::ofstream ofs(filename, std::ios::app);
    if (!ofs) {
        throw "Unable to open file for writing!";
    }

    ofs << name << std::endl;
    for (std::vector<StudentWrapper>::iterator it = students.begin();
            it != students.end();
            ++ it) {
        if (it->getScore() >= 0.0) {
            ofs << it->id << ',' << it->getScore();
        }
    }
}
