#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <set>
#include <algorithm>
#include <vector>
#include <map>

using namespace std;

int findDigits(string filename) {
    int nDigits = 0;

    ifstream sstream(filename);
    for (string ss; getline(sstream, ss); ) {
        // get everything after |
        auto outputs = ss.substr(ss.find('|')+1);

        istringstream iss(outputs);
        for (string s; iss >> s; ) {
            if (s.size() == 2 || s.size() == 4 || s.size() == 3 || s.size() == 7) {
                nDigits++;
            }
        }
    }

    return nDigits;
}

long long decode(string filename) {
    long long outputSum = 0;

    ifstream sstream(filename);
    for (string ss; getline(sstream, ss); ) {
        auto inputs = ss.substr(0, ss.find('|'));
        auto outputs = ss.substr(ss.find('|')+1);

        auto words = inputs + " " + outputs;

        // make a set of words
        set<set<char>> digits;
        istringstream iss(words);
        for (string s; iss >> s; ) {
            digits.insert(set<char>(s.begin(), s.end()));
        }

        // now try to deduce things
        vector<set<char>> decodedDigits(10);
        // find 1, 4, 7 and 8
        for (const auto& d : digits) {
            if (d.size() == 2) {
                decodedDigits[1] = d;
            } else if (d.size() == 4) {
                decodedDigits[4] = d;
            } else if (d.size() == 3) {
                decodedDigits[7] = d;
            } else if (d.size() == 7) {
                decodedDigits[8] = d;
            }
        }

        map<char, char> letterMap;
        // find a
        set<char> diff;
        set_difference(decodedDigits[7].begin(), decodedDigits[7].end(), decodedDigits[1].begin(), decodedDigits[1].end(), inserter(diff, diff.begin()));
        letterMap['a'] = *(diff.begin());

        // 3 is the only digit with 5 segments and includes both segments of 1
        for (const auto& d : digits) {
            if (d.size() == 5) {
                diff.clear();
                set_intersection(decodedDigits[1].begin(), decodedDigits[1].end(), d.begin(), d.end(), inserter(diff, diff.begin()));
                if (diff.size() == 2) {
                    decodedDigits[3] = d;
                    break;
                }
            }
        }

        // from comparing with 4 we can deduce d
        diff.clear();
        set_intersection(decodedDigits[3].begin(), decodedDigits[3].end(), decodedDigits[4].begin(), decodedDigits[4].end(), inserter(diff, diff.begin()));
        // subtract 1
        set<char> diff2;
        set_difference(diff.begin(), diff.end(), decodedDigits[1].begin(), decodedDigits[1].end(), inserter(diff2, diff2.begin()));
        letterMap['d'] = *(diff2.begin());

        // subtract 3 from 8 then do intersection with 4 to deduce b
        diff.clear();
        diff2.clear();
        set_difference(decodedDigits[8].begin(), decodedDigits[8].end(), decodedDigits[3].begin(), decodedDigits[3].end(), inserter(diff, diff.begin()));
        set_intersection(diff.begin(), diff.end(), decodedDigits[4].begin(), decodedDigits[4].end(), inserter(diff2, diff2.begin()));
        letterMap['b'] = *(diff2.begin());

        // subtract this from 8-3 to get e
        set<char> diff3;
        set_difference(diff.begin(), diff.end(), diff2.begin(), diff2.end(), inserter(diff3, diff3.begin()));
        letterMap['e'] = *(diff3.begin());

        // find g from 3-1 - a - d
        diff.clear();
        // diff2.clear();
        // diff3.clear();
        set_difference(decodedDigits[3].begin(), decodedDigits[3].end(), decodedDigits[1].begin(), decodedDigits[1].end(), inserter(diff, diff.begin()));
        diff.erase(letterMap['a']);
        diff.erase(letterMap['d']);
        letterMap['g'] = *(diff.begin());

        // we can find c and f by counting how many instances they have in the inputs. c appears in 8/10 digits, f appears in 9/10;
        int icount = 0;
        int jcount = 0;

        char i = *(decodedDigits[1].begin());
        char j = *(++decodedDigits[1].begin());
        for (const auto& d : digits) {
            if (d.count(i) == 1) icount++;
            if (d.count(j) == 1) jcount++;
        }

        if (icount == 8) {
            letterMap['c'] = i;
            letterMap['f'] = j;
        } else {
            letterMap['c'] = j;
            letterMap['f'] = i;
        }

        // now print the letter map
        for (const auto& [key, l] : letterMap) {
            cout << l;
        }
        cout << endl;

        // decode the other numbers
        decodedDigits[0] = set<char>({letterMap['a'], letterMap['b'], letterMap['c'], letterMap['e'], letterMap['f'], letterMap['g']});
        decodedDigits[2] = set<char>({letterMap['a'], letterMap['c'], letterMap['d'], letterMap['e'], letterMap['g']});
        decodedDigits[5] = set<char>({letterMap['a'], letterMap['b'], letterMap['d'], letterMap['f'], letterMap['g']});
        decodedDigits[6] = set<char>({letterMap['a'], letterMap['b'], letterMap['d'], letterMap['e'], letterMap['f'], letterMap['g']});
        decodedDigits[9] = set<char>({letterMap['a'], letterMap['b'], letterMap['c'], letterMap['d'], letterMap['f'], letterMap['g']});

        // decode the outputs
        istringstream is(outputs);
        string num;
        for (string s; is >> s; ) {
            for (int i = 0; i < decodedDigits.size(); ++i) {
                if (decodedDigits[i].size() == s.size()) {
                    diff.clear();
                    set_union(decodedDigits[i].begin(), decodedDigits[i].end(), s.begin(), s.end(), inserter(diff, diff.begin()));
                    if (diff.size() == decodedDigits[i].size()) {
                        num += to_string(i);
                        break;
                    }
                }
            }
        }
        cout << "num = " << num << endl;
        outputSum += stoi(num);
    }

    return outputSum;

}

int main() {
    auto nDigits = findDigits("day8.txt");
    cout << "1, 4, 7 and 8 appear " << nDigits << " times in the outputs." << endl;

    auto score = decode("day8.txt");

    cout << "The total score is " << score << endl;
}