#include <vector>
#include <iostream>
#include <fstream>
#include <iomanip>
#include "Class.h"
#include "Student.h"

using namespace std;

class AppX {
private:
    vector<Student *> studentVec;
    vector<Class *> classVec;

    void loadFiles();
    void inputScore();
    void printScoreStats();
    void printGrade();
    void saveScore();

public:
    AppX();
    ~AppX();
    int run();
};

AppX::~AppX()
{
    // You can use the traditional loop, which is more clear.
    for (vector<Class *>::iterator it = classVec.begin();
         it != classVec.end();
         ++it) {
        if (*it) delete (*it);
    }
    // You can use modern and simpler loops only if you know what it is doing.
    for (const auto &s: studentVec) {
        if (s) delete (s);
    }
}

AppX::AppX()
{
    loadFiles();
}

void AppX::loadFiles()
{
    string line;
    size_t pos1, pos2;
    vector<string> bufv;
    Student *st = nullptr;
    string clsname;
    int point;
    Class *cl = nullptr;

    // Open a file as a input stream.
    ifstream stfile("Lab Init/lab1/Students.txt");

    while (getline(stfile, line)) {
        if (line.empty()) // It's an empty line.
            continue;
        if (line[0] == '#') // It's a comment line.
            continue;

        // The bufv vector stores each column in the line.
        bufv.clear();
        // Split the line into columns.
        //   pos1: beginning of the column.
        //   pos2: end of the column.
        pos1 = 0;
        while (true) {
            pos2 = line.find(';', pos1 + 1);
            if (pos2 == string::npos) { // No more columns.
                // Save the last column (pos1 ~ eol).
                bufv.push_back(line.substr(pos1, string::npos));
                break;
            } else {
                // Save the column (pos1 ~ pos2).
                bufv.push_back(line.substr(pos1, pos2 - pos1));
            }
            pos1 = pos2 + 1;
        }

        // TODO: uncomment next lines after implementing class Undergraduate
        // and Graduate.
        if (bufv[3] == "U")
            st = new Undergraduate(bufv[0], bufv[1], bufv[2]);
        else
            st = new Graduate(bufv[0], bufv[1], bufv[2]);

        studentVec.push_back(st);
    }
    stfile.close();

    // TODO: load data from ./Classes.txt and push objects to the vector.
    // Hint: how is student information read?
    ifstream clsfile("Lab Init/lab1/Classes.txt");
    
    while(getline(clsfile, line)) {
        bufv.clear();
        if (line.empty() || line.find("#") == 0)
            continue;
        if (line.find("Class") == 0) {
            clsname  = line.substr(line.find(":") + 1);
            continue;
        } else if (line.find("Point") == 0) {
            point = stoi(line.substr(line.find(":") + 1));
            cl = new Class(clsname, point);
            classVec.push_back(cl);
            continue;
        } else {
            for (vector<Student*>::iterator it = studentVec.begin();
                it != studentVec.end();
                ++ it) {
                if ((*it)->id == line) {
                    cl->addStudent(**it);
                    (*it)->addClasses(cl);
                    break;
                }
            }
        }
    }
    clsfile.close();
}

void AppX::inputScore()
{
    // TODO: implement inputScore.
    // Hint: Take a look at printScoreStats().
    string sbuf;
    Class *cl;

    while (true) {
        cout << "Please input the class name (or input q to quit): ";
        cin >> sbuf;
        if (sbuf == "q") 
            break;

        cl = nullptr;
        for (vector<Class*>::iterator it = classVec.begin();
                it != classVec.end();
                ++ it) {
            if ((*it)->name == sbuf) {
                cl = *it;
                break;
            }
        }
        if (cl == nullptr) {
            cerr << "No Match Class!" << endl;
            continue;
        } else {
            while (true) {
                string stu;
                string stuid;
                double score;
                cout << "Please input the student id and score (or input q to quit): ";
                cin >> stu;
                if (stu == "q")
                    break;
                bool found = false;
                for (vector<Student*>::iterator it = studentVec.begin();
                        it != studentVec.end();
                        ++ it) {
                    if ((*it)->id == stu.substr(0, stu.find(","))) {
                        stuid = (*it)->id;
                        found = true;
                        break;
                    }
                }
                if (!found) {
                    cerr << "No Match Student!" << endl;
                    continue;
                }

                stu = stu.substr(stu.find(",") + 1);
                try {
                    StudentWrapper& sw = cl->getStudentWrapper(stuid);
                    score = stod(stu);
                    sw.setScore(score);
                    continue;
                } catch (invalid_argument& e) {
                    cerr << "Wrong Score!" << endl;
                    continue;
                } catch (string e) {
                    cerr << e << endl;
                } catch (char const* e) {
                    cerr << e << endl;
                } catch (...) {
                    cerr << "Unknown exception caught" << endl;
                }
            }
        }
    }
}

void AppX::printScoreStats()
{
    string sbuf;
    Class *cl;
    double highest, lowest, avg;

    while (true) {
        cout << "Please input the class name (or input q to quit): ";
        cin >> sbuf;
        if (sbuf == "q")
            break;

        cl = nullptr;
        for (vector<Class*>::iterator it = classVec.begin();
                it != classVec.end();
                ++ it) {
            if ((*it)->name == sbuf) {
                cl = *it;
                break;
            }
        }
        if (cl == nullptr) {
            cerr << "No Match Class!" << endl;
            continue;
        }

        try {
            highest = cl->getHighestScore();
            lowest = cl->getLowestScore();
            avg = cl->getAvgScore();

            cout << cl->toString() << endl;
            cout << setiosflags(ios::fixed) << setprecision(2)<< "Highest,Lowest,Avg = " << highest << "," << lowest << "," << avg << endl;
        } catch (string e) {
            cerr << e << endl;
        } catch (char const* e){
            cerr << e << endl;
        } catch (...) {
            cerr << "Unknown exception caught" << endl;
        }
    }
}

void AppX::printGrade()
{
    // TODO: implement printGrade.
    // Hint: Take a look at printScoreStats().
    string sbuf;
    Student *st;
    double gpa, avg;

    while (true) {
        cout << "Please input the student id (or input q to quit): ";
        cin >> sbuf;
        if (sbuf == "q") 
            break;
        
        st = nullptr;
        for (vector<Student*>::iterator it = studentVec.begin();
                it != studentVec.end();
                ++ it) {
            if ((*it)->id == sbuf) {
                st = *it;
                break;
            }
        }
        if (st == nullptr) {
            cerr << "No Match Student!" << endl;
            continue;
        }

        try {
            gpa = st->getGpa();
            avg = st->getAvgScore();

            cout << st->toString() << endl;
            cout << setiosflags(ios::fixed) << setprecision(2) << "GPA,AVG = " << gpa << "," << avg << endl;
            continue;
        } catch (string e) {
            cerr << e << endl;
        } catch (char const* e) {
            cerr << e << endl;
        } catch (...) {
            cerr << "Unknown exception caught" << endl;
        }
    }
}

void AppX::saveScore() {
    // TODO: implement saveScore.
    ofstream ofs("Lab Init/lab1/score.txt", ios::trunc);
    ofs.close();

    for (vector<Class*>::iterator it = classVec.begin();
            it != classVec.end();
            ++ it) {
        (*it)->saveScore("Lab Init/lab1/score.txt");
        }
}

int AppX::run()
{
    char cmd;
    while (true) {
        cout << "Command menu:\n"
            << "\ti: Input score\n"
            << "\ta: Compute score statistics of a class\n"
            << "\tg: Compute grade of a student\n"
            << "\tq: Quit\n"
            << "Please input the command: ";
        cin >> cmd;
        if (cmd == 'i') {
            inputScore();
        } else if (cmd == 'a') {
            printScoreStats();
        } else if (cmd == 'g') {
            printGrade();
        } else if (cmd == 'q') {
            saveScore();
            break;
        } else {
            cout << "Invalid command!\n" << endl;
        }
    }
    return 0;
}

int main()
{
    AppX app;
    return app.run();
}
