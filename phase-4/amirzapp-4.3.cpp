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

struct level {
    string target;
    set<string> other_words;
    int score;
};

string scramble(string s) {
    for (int i = 0; i < s.length(); i++) {
        int j = rand() % s.length();
        swap(s[i], s[j]);
    }
    return s;
}

void print_scrambled(string s) {
    string scrambled = scramble(s);
    for (int i = 0; i < scrambled.length(); i++) {
        cout << scrambled[i] << ' ';
    }
    cout << endl;
}

string tolower(string s) {
    for (int i = 0; i < s.length(); i++) {
        s[i] = tolower(s[i]);
    }
    return s;
}

bool contains_extra_characters(string s, string target) {
    for (int i = 0; i < s.length(); i++) {
        size_t pos = target.find(s[i]);
        if (pos == string::npos) {
            return true;
        }
        target.erase(pos, 1);
    }
    return false;
}

bool play_level(level lvl, int& score) {
    cout << "The letters are: ";
    print_scrambled(lvl.target);

    cout << "Please guess the target word. Press Ctrl-D to exit." << endl;
    cout << "Score: " << score << endl;

    string input;
    while (cin >> input) {
        input = tolower(input);
        if (input == lvl.target) {
            score += lvl.score;
            cout << "Congratulations! You found the target word!" << endl;
            return true;
        } else if (lvl.other_words.count(input) != 0) {
            cout << "Nice, but it's not what I have in mind!" << endl;
            score += input.length();
        } else if (contains_extra_characters(input, lvl.target)) {
            cout << "You used some extra characters!" << endl;
        } else {
            cout << "Is it even a word?!" << endl;
        }
        cout << "Score: " << score << endl;
    }
    cout << "Giving up so soon? Why?!" << endl;
    return false;
}

list<level> read_levels(string filename) {
    list<level> levels;
    ifstream level_file(filename);
    string level_str;
    while (getline(level_file, level_str)) {
        level lvl;
        istringstream level_stream(level_str);       
        level_stream >> lvl.score;
        level_stream >> lvl.target;
        string word;
        while (level_stream >> word)
            lvl.other_words.insert(word);
        levels.push_back(lvl);
    }
    level_file.close();
    return levels;
}

int main() {
    srand(time(0));

    const list<level> levels = read_levels("levels-score.txt");

    int level_no = 0;
    int score = 0;
    for (level lvl : levels) {
        level_no++;
        cout << endl << "Level " << level_no << endl;
        if (!play_level(lvl, score))
            return 0;
    }
    cout << endl << "Congratulations! You solved all levels!" << endl;
}