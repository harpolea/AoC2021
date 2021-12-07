#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>

using namespace std;

class Board {
    private:
        static const int boardSize = 5;

        vector<int> board;
        vector<bool> marked;

    public:

        Board(ifstream &numStream) {
            string ss;
            for (int i = 0; i < boardSize; ++i) {
                getline(numStream, ss);
                // cout << ss << endl;

                istringstream isstream(ss);
                string s;
                for (string s; isstream >> s; ) {
                    board.emplace_back(stoi(s));
                }
            }
            marked.assign(boardSize*boardSize, false);

            // for (int i = 0; i < boardSize*boardSize; ++i) {
            //     cout << board[i] << ' ';
            //     if ((i+1) % 5 == 0) cout << endl;
            // }
        }

        bool markBoard(int num) {
            for (int i = 0; i < boardSize*boardSize; ++i) {
                // cout << marked[i] << " ";
                if (num == board[i]) {
                    marked[i] = true;
                    // cout << endl;
                    return true;
                }
            }
                // cout << endl;
            return false;
        }

        bool checkBingo() {
            for (int i = 0; i < boardSize; ++i) {
                int colMarkSum = 0;
                int rowMarkSum = 0;
                for (int j = 0; j < boardSize; ++j) {
                    // cout << marked[j * boardSize + i] << ' ';
                    if (marked[j * boardSize + i]) colMarkSum++;
                    if (marked[i * boardSize + j]) rowMarkSum++;
                }
                // cout << endl;

                // cout << colMarkSum << " " << rowMarkSum << endl;

                if (rowMarkSum == boardSize || colMarkSum == boardSize) return true;
            }
            return false;
        }

        int score(const int& num) {
            int unmarkedSum = 0;
            for (int i = 0; i < boardSize*boardSize; ++i) {
                if (!marked[i]) {
                    unmarkedSum += board[i];
                }
            }

            return unmarkedSum * num;
        }
};

int playBingo(string filename) {
    // get numbers
    ifstream sstream(filename);
    string ss;
    getline(sstream, ss);

    vector<int> bingoNumbers;
    stringstream numStream(ss);
    string s;
    while (getline(numStream, s, ',')) {
        bingoNumbers.emplace_back(stoi(s));
    }

    getline(sstream, ss);

    // make boards
    vector<Board> boards;

    while(sstream.peek() != EOF) {
        boards.emplace_back(Board(sstream));
        getline(sstream, ss);
    }

    cout << "We have " << boards.size() << " boards" << endl;

    // let's play!
    for (auto n: bingoNumbers) {
        cout << "Number called = " << n << endl;
        for (auto &b: boards) {
            auto marked = b.markBoard(n);
            if (marked) {
                // cout << "Marked" << endl;
                auto bingo = b.checkBingo();

                if (bingo) {
                    cout << "BINGO!" << endl;
                    return b.score(n);
                }
            }
        }
    }

    return 0;
}

int loseBingo(string filename) {
    // get numbers
    ifstream sstream(filename);
    string ss;
    getline(sstream, ss);

    vector<int> bingoNumbers;
    stringstream numStream(ss);
    string s;
    while (getline(numStream, s, ',')) {
        bingoNumbers.emplace_back(stoi(s));
    }

    getline(sstream, ss);

    // make boards
    vector<Board> boards;

    while(sstream.peek() != EOF) {
        boards.emplace_back(Board(sstream));
        getline(sstream, ss);
    }

    cout << "We have " << boards.size() << " boards" << endl;

    vector<bool> hasWon(boards.size(), false);
    int nWon = 0; // number of boards that have won

    // let's play!
    for (auto n: bingoNumbers) {
        cout << "Number called = " << n << endl;
        for (int i = 0; i < boards.size(); ++i) {
            if (hasWon[i]) continue;
            auto marked = boards[i].markBoard(n);
            if (marked) {
                // cout << "Marked" << endl;
                auto bingo = boards[i].checkBingo();

                if (bingo) {
                    cout << "BINGO!" << endl;
                    hasWon[i] = true;
                    if (++nWon == boards.size()) {
                        // found the last board to win
                        return boards[i].score(n);
                    }
                }
            }
        }
    }

    return 0;
}

int main() {
    auto score = playBingo("day4.txt");

    cout << "The final score is " << score << endl;

    score = loseBingo("day4.txt");

    cout << "The losing score is " << score << endl;
}