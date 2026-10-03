#include <climits>
#include <iostream>
#include <string>

#include "board.h"

using namespace std;

int main() {
    while (true) {
        cout << "How many disks do you want? (1 ~ 5)" << endl;
        string input;
        getline(cin, input);
        if (input == "Q") {
            break;
        } else if ("1" <= input && input <= "5") {
            int num_disk = stoi(input);
            Board board(num_disk);
            board.draw();
            while (!board.win()) {
                cout << "Move a disk. Format: x y" << endl;
                int x, y;
                cin >> x >> y;
                if (cin.fail() || x < 0 || x > 3 || y < 0 || y > 3 || (x == 0 && y != 0) || (x != 0 && y == 0)) {
                    board.draw();
                    cin.clear();
                    cin.ignore(INT_MAX, '\n');
                    continue;
                }
                bool log = !(x == 0 && y == 0);
                if (!log) {
                    board.autoplay();
                } else {
                    board.move(x, y, log);
                }
            }
            cout << "Congratulations ! You win!" << endl;
            cin.ignore(INT_MAX, '\n');
            continue;
        } else {
            continue;
        }
    }
    return 0;
}
