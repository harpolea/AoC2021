#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <deque>

using namespace std;

long long evolveLanternfish(vector<int> initialAges, int nDays=80) {
    // we're going to create a map that lists the numbers of laternfish of each age
    deque<long long> laternfishAges(9, 0);

    // now update the map with the initial fish age list
    for (const auto& age : initialAges) {
        laternfishAges[age]++;
    }

    // now we evolve for nDays
    for (int i = 0; i < nDays; ++i) {
        // pop off the fish with age 0
        auto pregnantFish = laternfishAges.front();
        laternfishAges.pop_front();

        // add these back with timers of 6 and add on new fish with timers of 8
        laternfishAges[6] += pregnantFish;
        laternfishAges.emplace_back(pregnantFish);
    }

    // sum up queue to get total number of fish
    long long nFish = 0;
    for (const auto& fish : laternfishAges) {
        nFish += fish;
    }

    return nFish;
}

int main() {
    // vector<int> ages = {3,4,3,1,2};
    vector<int> ages = {1,1,3,1,3,2,1,3,1,1,3,1,1,2,1,3,1,1,3,5,1,1,1,3,1,2,1,1,1,1,4,4,1,2,1,2,1,1,1,5,3,2,1,5,2,5,3,3,2,2,5,4,1,1,4,4,1,1,1,1,1,1,5,1,2,4,3,2,2,2,2,1,4,1,1,5,1,3,4,4,1,1,3,3,5,5,3,1,3,3,3,1,4,2,2,1,3,4,1,4,3,3,2,3,1,1,1,5,3,1,4,2,2,3,1,3,1,2,3,3,1,4,2,2,4,1,3,1,1,1,1,1,2,1,3,3,1,2,1,1,3,4,1,1,1,1,5,1,1,5,1,1,1,4,1,5,3,1,1,3,2,1,1,3,1,1,1,5,4,3,3,5,1,3,4,3,3,1,4,4,1,2,1,1,2,1,1,1,2,1,1,1,1,1,5,1,1,2,1,5,2,1,1,2,3,2,3,1,3,1,1,1,5,1,1,2,1,1,1,1,3,4,5,3,1,4,1,1,4,1,4,1,1,1,4,5,1,1,1,4,1,3,2,2,1,1,2,3,1,4,3,5,1,5,1,1,4,5,5,1,1,3,3,1,1,1,1,5,5,3,3,2,4,1,1,1,1,1,5,1,1,2,5,5,4,2,4,4,1,1,3,3,1,5,1,1,1,1,1,1};

    auto nFish = evolveLanternfish(ages);

    cout << "After 80 days, there are " << nFish << " laternfish." << endl;

    nFish = evolveLanternfish(ages, 256);

    cout << "After 256 days, there are " << nFish << " laternfish." << endl;
}