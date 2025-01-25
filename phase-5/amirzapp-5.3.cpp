#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <set>
#include <list>
#include <algorithm>
#include <cstdlib>
#include <ctime>
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

string scramble(string s) {
    for (int i = 0; i < s.length(); i++) {
        int j = rand() % s.length();
        swap(s[i], s[j]);
    }
    return s;
}

void printLetters(string targetLetters) {
    for (int i = 0; i < targetLetters.length(); i++) {
        cout << targetLetters[i] << ' ';
    }
    cout << endl;
}

string toLower(string s) {
    for (int i = 0; i < s.length(); i++) {
        s[i] = tolower(s[i]);
    }
    return s;
}

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
    printLetters(lvl.targetLetters);
}

int levelScore(Level lvl) {
    int sum = 0;
    for (TargetWord tword : lvl.targetWords) {
        if (tword.found)
            sum += tword.word.length();
    }
    for (string bonusWord : lvl.bonusWordsFound) {
        sum += bonusWord.length();
    }
    return sum;
}

bool playLevel(Level lvl, int& score) {
    string input;
    printLevel(lvl);
    cout << "> ";
    while (cin >> input) {
        input = toLower(input);
        if (input == "scramble!") {
            lvl.targetLetters = scramble(lvl.targetLetters);
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
        levelStream >> word;
        lvl.targetLetters = scramble(word);
        while (levelStream >> word && (word != "-")) {
            lvl.targetWords.push_back({word, false});
        }
        while (levelStream >> word)
            lvl.bonusWordsRemaining.insert(word);
        lvl.targetWords.sort(
            [](TargetWord a, TargetWord b) {
                return (a.word.length() < b.word.length()) || 
                    ((a.word.length() == b.word.length()) && (a.word < b.word));
            });
        levels.push_back(lvl);
    }
    return levels;
}

int main() {
    srand(time(0));

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