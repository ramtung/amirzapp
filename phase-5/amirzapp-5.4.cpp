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
    for_each(lvl.target_letters.begin(), lvl.target_letters.end(), 
        [](char c) {
            cout << c << ' ';
        });
}

int level_score(level lvl) {
    return reduce(lvl.target_words.begin(), lvl.target_words.end(), 0,
        [](int sum, target_word tword) {
            return sum + (tword.found ? tword.word.length() : 0);
        }) +
        reduce(lvl.bonus_words_found.begin(), lvl.bonus_words_found.end(), 0,
            [](int sum, string bonus_word) {
                return sum + bonus_word.length();
            });
}

bool play_level(level lvl, int& score) {
    string input;
    print_level(lvl);
    cout << "> ";
    while (cin >> input) {
        transform(input.begin(), input.end(), input.begin(), ::tolower);

        if (input == "scramble!") {
            random_shuffle(lvl.target_letters.begin(), lvl.target_letters.end());
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
        level_stream >> lvl.target_letters;
        random_shuffle(lvl.target_letters.begin(), lvl.target_letters.end());
        while (level_stream >> word && (word != "-")) {
            lvl.target_words.push_back({word, false});
        }
        while (level_stream >> word)
            lvl.bonus_words_remaining.insert(word);
        lvl.target_words.sort(
            [](target_word a, target_word b) {
                return make_tuple(a.word.length(), a.word) < make_tuple(b.word.length(), b.word);
            });
        levels.push_back(lvl);
    }
    return levels;
}

int main() {
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