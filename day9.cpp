#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <deque>
#include <vector>
#include <tuple>
#include <algorithm>

using namespace std;

int findLowPoints(string filename) {
    int risk = 0;

    // we only need to store 3 rows at a time, so let's just do a queue.
    deque<vector<int>> lineBuff;

    ifstream sstream(filename);
    // we're just going to special case the first line
    {
        string ss;
        getline(sstream, ss);
        vector<int> newline;
        for (const auto& c : ss) {
            newline.emplace_back(c - '0');
        }
        lineBuff.emplace_back(newline);
    }

    const int lineLength = lineBuff[0].size();

    int iline = 0;

    for (string ss; getline(sstream, ss); ) {
        if (lineBuff.size() == 3) lineBuff.pop_front();

        vector<int> newline;
        for (const auto& c : ss) {
            newline.emplace_back(c - '0');
        }
        lineBuff.emplace_back(newline);

        // iterate over the middle line
        // compare entries with adjacent points
        for (int i = 0; i < lineLength; ++i) {
            if ((iline == 0 || lineBuff[iline][i] < lineBuff[iline-1][i]) && // above
                (i == 0 || lineBuff[iline][i] < lineBuff[iline][i-1]) && // left
                (i == lineLength-1 || lineBuff[iline][i] < lineBuff[iline][i+1]) && // right
                (lineBuff[iline][i] < lineBuff[iline+1][i]) // below
                ) {
                    risk += lineBuff[iline][i] + 1;
                }
        }
        
        if (iline == 0) iline++;
    }

    // need to finish up by doing the last line
    {
        lineBuff.pop_front();
        for (int i = 0; i < lineLength; ++i) {
            if ((lineBuff[iline][i] < lineBuff[iline-1][i]) && // above
                (i == 0 || lineBuff[iline][i] < lineBuff[iline][i-1]) && // left
                (i == lineLength-1 || lineBuff[iline][i] < lineBuff[iline][i+1]) // right
                ) {
                    risk += lineBuff[iline][i] + 1;
                }
        }
    }
    return risk;
}

int findBasins(string filename) {
    int risk = 0;

    // For this we do actually need the whole map as we're going to find the low points then go back to find basins
    vector<vector<int>> heightmap;
    vector<pair<int,int>> lowPoints;

    ifstream sstream(filename);
    // we're just going to special case the first line
    {
        string ss;
        getline(sstream, ss);
        vector<int> newline;
        for (const auto& c : ss) {
            newline.emplace_back(c - '0');
        }
        heightmap.emplace_back(newline);
    }

    const int lineLength = heightmap[0].size();

    int iline = 0;

    for (string ss; getline(sstream, ss); ) {

        vector<int> newline;
        for (const auto& c : ss) {
            newline.emplace_back(c - '0');
        }
        heightmap.emplace_back(newline);

        // iterate over the middle line
        // compare entries with adjacent points
        for (int i = 0; i < lineLength; ++i) {
            if ((iline == 0 || heightmap[iline][i] < heightmap[iline-1][i]) && // above
                (i == 0 || heightmap[iline][i] < heightmap[iline][i-1]) && // left
                (i == lineLength-1 || heightmap[iline][i] < heightmap[iline][i+1]) && // right
                (heightmap[iline][i] < heightmap[iline+1][i]) // below
                ) {
                    lowPoints.emplace_back(pair<int,int>({iline, i}));
                }
        }
        iline++;
    }

    const auto nLines = heightmap.size();

    // need to finish up by doing the last line
    {
        for (int i = 0; i < lineLength; ++i) {
            if ((heightmap[iline][i] < heightmap[iline-1][i]) && // above
                (i == 0 || heightmap[iline][i] < heightmap[iline][i-1]) && // left
                (i == lineLength-1 || heightmap[iline][i] < heightmap[iline][i+1]) // right
                ) {
                    lowPoints.emplace_back(pair<int,int>({iline, i}));
                }
        }
    }

    // now we've constructed a list of low points we need to iterate over them to find the basins
    vector<int> basinSizes;

    // iterate over lowpoints
    for (const auto& [i,j] : lowPoints) {
        vector<pair<int,int>> basin;
        basin.emplace_back(pair<int,int>({i,j}));

        // cout << "i,j = "<< i << ',' << j << endl;

        auto inBasin = [&basin](pair<int,int> p) {
            for (const auto& [m,n] : basin) {
                if (m == p.first && n == p.second) return true;
            }
            return false;
        };

        int oldSize = 0;
        int newSize = basin.size();
        while (newSize > oldSize) {
            // cout << "newSize = " << newSize << endl;
            // iterate over new points. A point is added to the basin if it's less than all the surrounding points that are not in the basin
            for (int k = oldSize; k < newSize; ++k) {
                int m = basin[k].first;
                int n = basin[k].second;

                // cout << "m,n = " << m << "," << n << endl;
                
                // test above point
                if (m > 0 && !inBasin(pair<int,int>({m-1,n})) && heightmap[m-1][n] != 9) {
                    if ((m-1 == 0 || inBasin(pair<int,int>({m-2,n})) || heightmap[m-1][n] < heightmap[m-2][n]) && // above
                    (n == 0 || inBasin(pair<int,int>({m-1,n-1})) || heightmap[m-1][n] < heightmap[m-1][n-1]) && // left
                    (n == lineLength-1 || inBasin(pair<int,int>({m-1,n+1})) || heightmap[m-1][n] < heightmap[m-1][n+1]) // right
                    ) {
                        basin.emplace_back(pair<int,int>({m-1,n}));
                    }
                }

                // test left point 
                if (n > 0 && !inBasin(pair<int,int>({m,n-1})) && heightmap[m][n-1] != 9) {
                    if ((m == 0 || inBasin(pair<int,int>({m-1,n-1})) || heightmap[m][n-1] < heightmap[m-1][n-1]) && // above
                    (n-1 == 0 || inBasin(pair<int,int>({m,n-2})) || heightmap[m][n-1] < heightmap[m][n-2]) && // left
                    (m == nLines-1 || inBasin(pair<int,int>({m+1,n-1})) || heightmap[m][n-1] < heightmap[m+1][n-1]) // below
                    ) {
                        basin.emplace_back(pair<int,int>({m,n-1}));
                    }
                }

                // test right point
                if (n < lineLength-1 && !inBasin(pair<int,int>({m,n+1})) && heightmap[m][n+1] != 9) {
                    if ((m == 0 || inBasin(pair<int,int>({m-1,n+1})) || heightmap[m][n+1] < heightmap[m-1][n+1]) && // above
                    (n+1 == lineLength-1 || inBasin(pair<int,int>({m,n+2})) || heightmap[m][n+1] < heightmap[m][n+2]) && // right
                    (m == nLines-1 || inBasin(pair<int,int>({m+1,n+1})) || heightmap[m][n+1] < heightmap[m+1][n+1]) // below
                    ) {
                        basin.emplace_back(pair<int,int>({m,n+1}));
                    }
                }

                // test below point 
                if (m < nLines-1 && !inBasin(pair<int,int>({m+1,n})) && heightmap[m+1][n] != 9) {
                    if ((n == 0 || inBasin(pair<int,int>({m+1,n-1})) || heightmap[m+1][n] < heightmap[m+1][n-1]) && // left
                    (n == lineLength-1 || inBasin(pair<int,int>({m+1,n+1})) || heightmap[m+1][n] < heightmap[m+1][n+1]) && // right
                    (m+1 == nLines-1 || inBasin(pair<int,int>({m+2,n})) || heightmap[m+1][n] < heightmap[m+2][n]) // below
                    ) {
                        basin.emplace_back(pair<int,int>({m+1,n}));
                    }
                }
            }
            oldSize = newSize;
            newSize = basin.size();
        }
        basinSizes.emplace_back(basin.size());
    }

    sort(basinSizes.begin(), basinSizes.end(), greater<int>());

    cout << "basin sizes = ";
    for (const auto& b : basinSizes) {
        cout << b << " ";
    }
    cout << endl;


    return basinSizes[0] * basinSizes[1] * basinSizes[2];
}

int main() {

    auto risk = findLowPoints("day9.txt");

    cout << "The total risk is " << risk << endl;

    auto basins = findBasins("day9.txt");

    cout << "Multiplying largest basins together gives " << basins << endl;

}