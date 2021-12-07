#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <regex>
#include <map>
#include <tuple>
#include <cmath>

using namespace std;

int findOverlaps(string filename, bool useDiagonals=false) {
    map<pair<int, int>, int> points;
    ifstream sstream(filename);
    regex re("(\\d+),(\\d+) -> (\\d+),(\\d+)");
    for (string ss; getline(sstream, ss); ) {
        smatch match;
        regex_match(ss, match, re);
        int x1 = stoi(match[1].str());
        int y1 = stoi(match[2].str());
        int x2 = stoi(match[3].str());
        int y2 = stoi(match[4].str());

        if (x1 == x2) { // horizontal line
            for (int i = min(y1, y2); i <= max(y1, y2); ++i) {
                pair<int, int> p = {x1, i};
                try {
                    points[p]++;
                } catch (const exception& e) {
                    points[p] = 1;
                }
            }
        } else if (y1 == y2) { // vertical line
            for (int i = min(x1, x2); i <= max(x1, x2); ++i) {
                pair<int, int> p = {i, y1};
                try {
                    points[p]++;
                } catch (const exception& e) {
                    points[p] = 1;
                }
            }
        } else if (useDiagonals && (abs(x1-x2) == abs(y1-y2))) { // diagonals
            for (int i = 0; i <= abs(x1 - x2); ++i) {
                pair<int, int> p = {x1 + copysign(1, x2-x1)*i, y1 + copysign(1, y2-y1)*i};
                try {
                    points[p]++;
                } catch (const exception& e) {
                    points[p] = 1;
                }
            } 
        }
    }

    // now iterate over the points in the dictionary to find which 
    // ones are >= 2
    int nPoints = 0;
    for (const auto& [key, value] : points) {
        if (value > 1) nPoints++;
    }

    return nPoints;
}

int main() {

    auto nOverlaps = findOverlaps("day5.txt");

    cout << "At least 2 overlaps at " << nOverlaps << " points." << endl;

    nOverlaps = findOverlaps("day5.txt", true);

    cout << "With diagonals, there are at least 2 overlaps at " << nOverlaps << " points." << endl;

}