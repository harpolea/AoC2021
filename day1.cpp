#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <deque>
#include <numeric>

using namespace std;

int countIncreases(string filename) {

    int nIncreases = 0;
    int previousMeasurement = 10000000;

    ifstream sstream(filename);
    for (string ss; getline(sstream, ss); ) {
        int measurement = stoi(ss);
        if (measurement > previousMeasurement) {
            nIncreases++;
        }
        previousMeasurement = measurement;
    }

    return nIncreases;
}

int countWindows(string filename) {

    // make a queue for the sliding window
    deque<int> window(3, 10000);

    auto qSum = [](deque<int> q) { return accumulate(q.begin(), q.end(), 0); };

    int nIncreases = 0;
    int previousSum = 1000000;

    ifstream sstream(filename);
    for (string ss; getline(sstream, ss); ) {
        window.pop_front();
        window.emplace_back(stoi(ss));
        auto newSum = qSum(window);
        if (newSum > previousSum) {
            nIncreases++;
        }
        previousSum = newSum;
    }

    return nIncreases;
}

int main() {
    auto increases = countIncreases("day1.txt");

    cout << "There are " << increases << " measurements larger than the previous one." << endl;

    increases = countWindows("day1.txt");

    cout << "There are " << increases << " sums larger than the previous sum." << endl;
}