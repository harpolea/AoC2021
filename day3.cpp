#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <math.h>

using namespace std;

int getPowerConsumption(const string filename, const int binaryLength) {
    vector<int> binarySum(binaryLength, 0);
    int nNumbers = 0;

    ifstream sstream(filename);
    for (string ss; getline(sstream, ss); ) {
        nNumbers++;
        for (int i = 0; i < binaryLength; ++i) {
            binarySum[i] += ss[i] - '0';
        }
    }

    for (int i = 0; i < binaryLength; ++i) {
        binarySum[i] = binarySum[i] > nNumbers / 2 ? 1 : 0;
    }

    int gammaRate = 0;
    for (int i = 0; i < binaryLength; ++i) {
        gammaRate += binarySum[binaryLength-i-1] * pow(2, i);
    }
    int epsilonRate = pow(2, binaryLength) - 1 - gammaRate;

    cout << "gamma = " << gammaRate << ", epsilon = " << epsilonRate << endl;

    return gammaRate * epsilonRate;
}

int getLifeSupport(const string filename, const int binaryLength) {
    vector<vector<bool>> inputs;

    int nNumbers = 0;

    ifstream sstream(filename);
    for (string ss; getline(sstream, ss); ) {
        nNumbers++;
        vector<bool> line(binaryLength);
        for (int i = 0; i < binaryLength; ++i) {
            line[i] = bool((int)ss[i] - '0');
        }
        inputs.emplace_back(line);
    }

    vector<bool> valid(nNumbers, true);

    auto nValid = [](vector<bool> bVec) {
        int n = 0;
        for (const bool &b : bVec) {
            n += b;
        }
        return n;
    };

    for (int i = 0; i < binaryLength; ++i) {
        if (nValid(valid) == 1) break;

        int bitSum = 0;
        for (int j = 0; j < nNumbers; j++) {
            if (valid[j]) {
                bitSum += inputs[j][i];
            }
        }

        bool bit = bitSum >= float(nValid(valid)) / 2.f ? true : false;
        
        for (int j = 0; j < nNumbers; j++) {
            if (valid[j] && inputs[j][i] != bit) {
                valid[j] = false;
            }
        }
    }

    int validIndex;
    for (int i = 0; i < nNumbers; ++i) {
        if (valid[i]) {
            validIndex = i;
            break;
        }
    }

    int oxGenRating = 0;
    for (int i = 0; i < binaryLength; ++i) {
        cout << inputs[validIndex][i] << ' ';
        oxGenRating += inputs[validIndex][binaryLength-i-1] * pow(2, i);
    }
    cout << endl;

    // reset valid list 
    for (int i = 0; i < nNumbers; ++i) {
        valid[i] = true;
    }
    for (int i = 0; i < binaryLength; ++i) {
        if (nValid(valid) == 1) break;

        int bitSum = 0;
        for (int j = 0; j < nNumbers; j++) {
            if (valid[j]) {
                bitSum += inputs[j][i];
            }
        }

        bool bit = bitSum >= float(nValid(valid)) / 2.f ? false : true;
        
        for (int j = 0; j < nNumbers; j++) {
            if (valid[j] && inputs[j][i] != bit) {
                valid[j] = false;
            }
        }
    }

    for (int i = 0; i < nNumbers; ++i) {
        if (valid[i]) {
            validIndex = i;
            break;
        }
    }

    int co2ScrubberRating = 0;
    for (int i = 0; i < binaryLength; ++i) {
        cout << inputs[validIndex][i] << ' ';
        co2ScrubberRating += inputs[validIndex][binaryLength-i-1] * pow(2, i);
    }
    cout << endl;

    return oxGenRating * co2ScrubberRating;

}

int main() {

    auto powerConsumption = getPowerConsumption("day3.txt", 12);

    cout << "Power Consumption = " << powerConsumption << endl;

    auto lifeSupport = getLifeSupport("day3.txt", 12);

    cout << "Life Support = " << lifeSupport << endl;

}