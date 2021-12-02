#include <iostream>
#include <string>
#include <sstream>
#include <fstream>

using namespace std;

int moveSubmarine(const string filename) {

    int horizontalPos = 0;
    int depth = 0;

    ifstream sstream(filename);
    for (string ss; getline(sstream, ss); ) {
        string dir = ss.substr(0, ss.find(' '));
        int dx = stoi(ss.substr(ss.find(' ')+1));

        if (dir == "forward") {
            horizontalPos += dx;
        } else if (dir == "down") {
            depth += dx;
        } else if (dir == "up") {
            depth -= dx;
        }
    }

    return horizontalPos * depth;
}

int altMoveSubmarine(const string filename) {

    int horizontalPos = 0;
    int depth = 0;
    int aim = 0;

    ifstream sstream(filename);
    for (string ss; getline(sstream, ss); ) {
        string dir = ss.substr(0, ss.find(' '));
        int dx = stoi(ss.substr(ss.find(' ')+1));

        if (dir == "forward") {
            horizontalPos += dx;
            depth += aim * dx;
        } else if (dir == "down") {
            aim += dx;
        } else if (dir == "up") {
            aim -= dx;
        }
    }

    return horizontalPos * depth;
}

int main() {
    auto multiple = moveSubmarine("day2.txt");

    cout << "Final horizontal x depth = " << multiple << endl;

    multiple = altMoveSubmarine("day2.txt");

    cout << "Final horizontal x depth = " << multiple << endl;
}