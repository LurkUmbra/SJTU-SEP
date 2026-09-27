#include "Student.h"
#include <string>
#include <sstream>
#include "Class.h"

std::string Student::toString() const
{
    // TODO: uncomment the following code after implementing class Student.
    std::ostringstream oss;
    oss << "Student Information:"
        << "\n\tid: " << id
        << "\n\tenrollment year: " << year
        << "\n\tdegree: " << (degree == graduate ? "graduate" : "undergraduate")
        << std::endl;
    return oss.str();
}

// TODO: implement functions which are declared in Student.h.
void Student::addClasses(Class *cl) {
    classes.push_back(cl);
}

double Graduate::getGpa() const {
    double totalPoints = 0.0;
    double weightedGradePoints = 0.0;

    for (Class* cl : classes) {
        try {
            const StudentWrapper& sw = cl->getStudentWrapper(id);
            double score = sw.getScore();
            if (score < 0.0) continue;
            totalPoints += cl->point;

            double gradePoint = 0.0;
            if (score >= 90.0) gradePoint = 4.0;
            else if (score >= 80.0) gradePoint = 3.5;
            else if (score >= 70.0) gradePoint = 3.0;
            else if (score >= 60.0) gradePoint = 2.5;
            else gradePoint = 2.0;
            
            weightedGradePoints += gradePoint * cl->point;
        } catch(...) {
            continue;
        }   
    }

    if (totalPoints == 0.0) return 0.0;
    return weightedGradePoints / totalPoints;
}

double Undergraduate::getGpa() const {
    double Gpa = 0.0;
    Gpa = getAvgScore() / 20.0;
    return Gpa;
}

double Student::getAvgScore() const {
    double weightedSum = 0.0;
    double totalPoints = 0.0;
    
    for (Class* cl : classes) {
        try {
            const StudentWrapper& sw = cl->getStudentWrapper(id);
            double score = sw.getScore();
            if (score < 0.0) continue;
            weightedSum += score * cl->point;
            totalPoints += cl->point;
        } catch (...) {
            continue;
        }
    }

    if (totalPoints == 0.0) return 0.0;
    return weightedSum / totalPoints;
}