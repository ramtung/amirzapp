#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <set>
#include <list>
#include <algorithm>
#include <numeric>
#include <tuple>
using namespace std;

struct TargetWord {
    string word;
    bool found;
};

struct Level {
    string targetLetters;
    list<TargetWord> targetWords;
    set<string> bonusWordsFound;
    set<string> bonusWordsRemaining;
};

bool containsExtraCharacters(string s, string target) {
    for (int i = 0; i < s.length(); i++) {
        size_t pos = target.find(s[i]);
        if (pos == string::npos) {
            return true;
        }
        target.erase(pos, 1);
    }
    return false;
}

void printLevel(Level lvl) {
    for (TargetWord tword : lvl.targetWords) {
        if (tword.found)
            cout << tword.word << endl;
        else
            cout << string(tword.word.length(), '-') << endl;
    }
    cout << "The letters are: ";
    for_each(lvl.targetLetters.begin(), lvl.targetLetters.end(), 
        [](char c) {
            cout << c << ' ';
        });
    cout << endl;
}

int levelScore(Level lvl) {
    return accumulate(lvl.targetWords.begin(), lvl.targetWords.end(), 0,
        [](int sum, TargetWord tword) {
            return sum + (tword.found ? tword.word.length() : 0);
        }) +
        accumulate(lvl.bonusWordsFound.begin(), lvl.bonusWordsFound.end(), 0,
            [](int sum, string bonusWord) {
                return sum + bonusWord.length();
            });
}

bool playLevel(Level lvl, int& score) {
    string input;
    printLevel(lvl);
    cout << "> ";
    while (cin >> input) {
        transform(input.begin(), input.end(), input.begin(), ::tolower);

        if (input == "scramble!") {
            random_shuffle(lvl.targetLetters.begin(), lvl.targetLetters.end());
            printLevel(lvl);
            cout << "> ";
            continue;
        }
        auto it = find_if(lvl.targetWords.begin(), lvl.targetWords.end(), 
            [input](TargetWord tword) {
                return tword.word == input;
            });
        if (it != lvl.targetWords.end()) {
            it->found = true;
            cout << "Great! You found a target word!" << endl;
            printLevel(lvl);
            if (all_of(lvl.targetWords.begin(), lvl.targetWords.end(), 
                [](TargetWord tword) {
                    return tword.found;
                })) {
                cout << "Congratulations! You found all target words!" << endl;
                score += levelScore(lvl);
                cout << "Your score is now " << score << endl;
                return true;
            }
        } else if (lvl.bonusWordsFound.count(input) != 0) {
            cout << "You already found this bonus word!" << endl;
        } else if (lvl.bonusWordsRemaining.count(input) != 0) {
            cout << "Nice! You found a bonus word!" << endl;
            lvl.bonusWordsRemaining.erase(input);
            lvl.bonusWordsFound.insert(input);
            cout << "Bonus words found: " << lvl.bonusWordsFound.size() << endl;
        } else if (containsExtraCharacters(input, lvl.targetLetters)) {
            cout << "You used some extra characters!" << endl;
        } else {
            cout << "Hmmm, sorry!" << endl;
        }
        printLevel(lvl);
        cout << "> ";
    }
    cout << "Giving up so soon? Why?!" << endl;
    return false;
}

list<Level> readLevels(string filename) {
    list<Level> levels;
    ifstream levelFile(filename);
    string levelStr;
    getline(levelFile, levelStr); // skip the header

    while (getline(levelFile, levelStr)) {
        Level lvl;
        string word;
        istringstream levelStream(levelStr);       
        levelStream >> lvl.targetLetters;
        random_shuffle(lvl.targetLetters.begin(), lvl.targetLetters.end());
        while (levelStream >> word && (word != "-")) {
            lvl.targetWords.push_back({word, false});
        }
        while (levelStream >> word)
            lvl.bonusWordsRemaining.insert(word);
        lvl.targetWords.sort(
            [](TargetWord a, TargetWord b) {
                return make_tuple(a.word.length(), a.word) < make_tuple(b.word.length(), b.word);
            });
        levels.push_back(lvl);
    }
    return levels;
}

int main() {
    const list<Level> levels = readLevels("levels-5.txt");

    cout << "Find all the target words in the levels." << endl 
         << "Gain extra points by finding bonus words." << endl
         << "Enter scramble! to re-scramble the words." << endl
         << "Press Ctrl-D to exit." << endl;
    int levelNo = 0;
    int score = 0;
    for (Level lvl : levels) {
        levelNo++;
        cout << endl << "Level " << levelNo << endl;
        if (!playLevel(lvl, score))
            return 0;
    }
    cout << endl << "Congratulations! You solved all levels!" << endl;
}