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

struct Level {
    string targetLetters;
    list<string> targetWords;
    list<bool> targetWordFound;
    set<string> bonusWordsFound;
    set<string> bonusWordsRemaining;
    int score;
};

string scramble(string s) {
    for (int i = 0; i < s.length(); i++) {
        int j = rand() % s.length();
        swap(s[i], s[j]);
    }
    return s;
}

void printScrambled(string s) {
    string scrambled = scramble(s);
    for (int i = 0; i < scrambled.length(); i++) {
        cout << scrambled[i] << ' ';
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

void printLevel(Level lvl, int score) {
    auto targetFoundIt = lvl.targetWordFound.begin();
    for (string word : lvl.targetWords) {
        if (*targetFoundIt)
            cout << word << endl;
        else
            cout << string(word.length(), '_') << endl;
        targetFoundIt++;
    }
    cout << "Score: " << score << endl;

    cout << "The letters are: ";
    printScrambled(lvl.targetLetters);
}

bool playLevel(Level lvl, int& score) {
    string input;
    while (cin >> input) {
        input = toLower(input);
        if (find(lvl.targetWords.begin(), lvl.targetWords.end(), input) != lvl.targetWords.end()) {
            // This is the place where I give up defining two parallel lists for the target words and the found status
            score += lvl.score;
            cout << "Congratulations! You found the target word!" << endl;
            return true;
        } else if (lvl.otherWords.count(input) != 0) {
            cout << "Nice, but it's not what I have in mind!" << endl;
            score += input.length();
        } else if (containsExtraCharacters(input, lvl.target)) {
            cout << "You used some extra characters!" << endl;
        } else {
            cout << "Is it even a word?!" << endl;
        }
        cout << "Score: " << score << endl;
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
        levelStream >> lvl.score;
        levelStream >> lvl.targetLetters;
        while (levelStream >> word && (word != "-")) {
            lvl.targetWords.push_back(word);
            lvl.targetWordFound.push_back(false);
        }
        while (levelStream >> word)
            lvl.bonusWordsRemaining.insert(word);
        levels.push_back(lvl);
    }
    return levels;
}

int main() {
    srand(time(0));

    const list<Level> levels = readLevels("levels-5.1.txt");

    int levelNo = 0;
    int score = 0;
    for (Level lvl : levels) {
        levelNo++;
        cout << endl << "Level " << levelNo << endl;
        cout << "Please guess the target word. Press Ctrl-D to exit." << endl;
        if (!playLevel(lvl, score))
            return 0;
    }
    cout << endl << "Congratulations! You solved all levels!" << endl;
}