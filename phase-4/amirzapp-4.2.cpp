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
    string target;
    set<string> otherWords;
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

bool playLevel(Level lvl) {
    cout << "The letters are: ";
    printScrambled(lvl.target);

    cout << "Please guess the target word. Press Ctrl-D to exit." << endl;

    string input;
    while (cin >> input) {
        input = toLower(input);
        if (input == lvl.target) {
            cout << "Congratulations! You found the target word!" << endl;
            return true;
        } else if (lvl.otherWords.count(input) != 0) {
            cout << "Nice, but it's not what I have in mind!" << endl;
        } else if (containsExtraCharacters(input, lvl.target)) {
            cout << "You used some extra characters!" << endl;
        } else {
            cout << "Is it even a word?!" << endl;
        }
    }
    cout << "Giving up so soon? Why?!" << endl;
    return false;
}

list<Level> readLevels(string filename) {
    list<Level> levels;
    ifstream levelFile(filename);
    string levelStr;
    while (getline(levelFile, levelStr)) {
        Level lvl;
        istringstream levelStream(levelStr);       
        levelStream >> lvl.target;
        string word;
        while (levelStream >> word)
            lvl.otherWords.insert(word);
        levels.push_back(lvl);
    }
    return levels;
}

int main() {
    srand(time(0));

    const list<Level> levels = readLevels("levels.txt");

    int levelNo = 0;
    for (Level lvl : levels) {
        levelNo++;
        cout << endl << "Level " << levelNo << endl;
        if (!playLevel(lvl))
            return 0;
    }
    cout << endl << "Congratulations! You solved all levels!" << endl;
}