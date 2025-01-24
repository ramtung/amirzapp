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

struct target_word {
    string word;
    bool found;
};

struct level {
    string target_letters;
    list<target_word> target_words;
    set<string> bonus_words_found;
    set<string> bonus_words_remaining;
};

string scramble(string s) {
    for (int i = 0; i < s.length(); i++) {
        int j = rand() % s.length();
        swap(s[i], s[j]);
    }
    return s;
}

void print_letters(string target_letters) {
    for (int i = 0; i < target_letters.length(); i++) {
        cout << target_letters[i] << ' ';
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

void print_level(level lvl) {
    for (target_word tword : lvl.target_words) {
        if (tword.found)
            cout << tword.word << endl;
        else
            cout << string(tword.word.length(), '-') << endl;
    }
    cout << "The letters are: ";
    print_letters(lvl.target_letters);
}

int level_score(level lvl) {
    int sum = 0;
    for (target_word tword : lvl.target_words) {
        if (tword.found)
            sum += tword.word.length();
    }
    for (string bonus_word : lvl.bonus_words_found) {
        sum += bonus_word.length();
    }
    return sum;
}

bool play_level(level lvl, int& score) {
    string input;
    print_level(lvl);
    cout << "> ";
    while (cin >> input) {
        input = tolower(input);
        if (input == "scramble!") {
            lvl.target_letters = scramble(lvl.target_letters);
            print_level(lvl);
            cout << "> ";
            continue;
        }
        auto it = find_if(lvl.target_words.begin(), lvl.target_words.end(), 
            [input](target_word tword) {
                return tword.word == input;
            });
        if (it != lvl.target_words.end()) {
            it->found = true;
            cout << "Great! You found a target word!" << endl;
            print_level(lvl);
            if (all_of(lvl.target_words.begin(), lvl.target_words.end(), 
                [](target_word tword) {
                    return tword.found;
                })) {
                cout << "Congratulations! You found all target words!" << endl;
                score += level_score(lvl);
                cout << "Your score is now " << score << endl;
                return true;
            }
        } else if (lvl.bonus_words_found.count(input) != 0) {
            cout << "You already found this bonus word!" << endl;
        } else if (lvl.bonus_words_remaining.count(input) != 0) {
            cout << "Nice! You found a bonus word!" << endl;
            lvl.bonus_words_remaining.erase(input);
            lvl.bonus_words_found.insert(input);
            cout << "Bonus words found: " << lvl.bonus_words_found.size() << endl;
        } else if (contains_extra_characters(input, lvl.target_letters)) {
            cout << "You used some extra characters!" << endl;
        } else {
            cout << "Hmmm, sorry!" << endl;
        }
        print_level(lvl);
        cout << "> ";
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
        level_stream >> word;
        lvl.target_letters = scramble(word);
        while (level_stream >> word && (word != "-")) {
            lvl.target_words.push_back({word, false});
        }
        while (level_stream >> word)
            lvl.bonus_words_remaining.insert(word);
        lvl.target_words.sort(
            [](target_word a, target_word b) {
                return (a.word.length() < b.word.length()) || 
                    ((a.word.length() == b.word.length()) && (a.word < b.word));
            });
        levels.push_back(lvl);
    }
    level_file.close();
    return levels;
}

int main() {
    srand(time(0));

    const list<level> levels = read_levels("levels-5.txt");

    cout << "Find all the target words in the levels." << endl 
         << "Gain extra points by finding bonus words." << endl
         << "Enter scramble! to re-scramble the words." << endl
         << "Press Ctrl-D to exit." << endl;
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