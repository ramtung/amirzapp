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
    string target_letters;
    list<string> target_words;
    list<bool> target_word_found;
    set<string> bonus_words_found;
    set<string> bonus_words_remaining;
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

void print_level(level lvl, int score) {
    auto target_found_it = lvl.target_word_found.begin();
    for (string word : lvl.target_words) {
        if (*target_found_it)
            cout << word << endl;
        else
            cout << string(word.length(), '_') << endl;
        target_found_it++;
    }
    cout << "Score: " << score << endl;

    cout << "The letters are: ";
    print_scrambled(lvl.target_letters);
}

bool play_level(level lvl, int& score) {
    string input;
    while (cin >> input) {
        input = tolower(input);
        if (find(lvl.target_words.begin(), lvl.target_words.end(), input) != lvl.target_words.end()) {
            // This is the place where I give up defining two parallel lists for the target words and the found status
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
    getline(level_file, level_str); // skip the header

    while (getline(level_file, level_str)) {
        level lvl;
        string word;
        istringstream level_stream(level_str);       
        level_stream >> lvl.score;
        level_stream >> lvl.target_letters;
        while (level_stream >> word && (word != "-")) {
            lvl.target_words.push_back(word);
            lvl.target_word_found.push_back(false);
        }
        while (level_stream >> word)
            lvl.bonus_words_remaining.insert(word);
        levels.push_back(lvl);
    }
    level_file.close();
    return levels;
}

int main() {
    srand(time(0));

    const list<level> levels = read_levels("levels-5.1.txt");

    int level_no = 0;
    int score = 0;
    for (level lvl : levels) {
        level_no++;
        cout << endl << "Level " << level_no << endl;
        cout << "Please guess the target word. Press Ctrl-D to exit." << endl;
        if (!play_level(lvl, score))
            return 0;
    }
    cout << endl << "Congratulations! You solved all levels!" << endl;
}