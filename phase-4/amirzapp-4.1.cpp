#include <iostream>
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

bool play_level(level lvl) {
    cout << "The letters are: ";
    print_scrambled(lvl.target);

    cout << "Please guess the target word. Press Ctrl-D to exit." << endl;

    string input;
    while (cin >> input) {
        input = tolower(input);
        if (input == lvl.target) {
            cout << "Congratulations! You found the target word!" << endl;
            return true;
        } else if (lvl.other_words.count(input) != 0) {
            cout << "Nice, but it's not what I have in mind!" << endl;
        } else if (contains_extra_characters(input, lvl.target)) {
            cout << "You used some extra characters!" << endl;
        } else {
            cout << "Is it even a word?!" << endl;
        }
    }
    cout << "Giving up so soon? Why?!" << endl;
    return false;
}

int main() {
    srand(time(0));

    const list<level> levels = {
        { "prince", {"ice", "nice", "epic", "pie", "pin", "nip", "pine", "pen", "per", "price", "pier", "ripe", "pincer", "ripen", "rice", "eric", "nicer", "rein"} },
        { "orange", {"age", "gen", "ago", "geo", "one", "neo", "gone", "organ", "gore", "ron", "nor", "reno", "are", "ear", "era", "arg", "rage", "gear", "ran", "near", "earn", "range", "anger"} },
        { "regard", {"age", "aged", "are", "arg", "dare", "dear", "der", "der", "drag", "ear", "edgar", "era", "gear", "grad", "grade", "rage", "rare", "read", "rear", "red", "reg"} },
        { "outline", {"lie", "nil", "line", "neil", "len", "ion", "leo", "oil", "lion", "lone", "leon", "one", "neo", "toe", "into", "lot", "ton", "not", "tone", "note", "lou", "out", "unto", "tie", "tin", "let", "lit", "tile", "lite", "intel", "ten", "net", "unit", "until", "nut", "tune"} },
        { "mermaid", {"aid", "aim", "aimed", "air", "are", "arm", "armed", "dam", "dare", "dear", "die", "dim", "dream", "ear", "emma", "era", "idea", "mad", "made", "mar", "mardi", "med", "media", "mid", "mime", "raid", "ram", "read", "red", "rid", "ride", "rim"} }
    };

    int level_no = 0;
    for (level lvl : levels) {
        level_no++;
        cout << endl << "Level " << level_no << endl;
        if (!play_level(lvl))
            return 0;
    }
    cout << endl << "Congratulations! You solved all levels!" << endl;
}