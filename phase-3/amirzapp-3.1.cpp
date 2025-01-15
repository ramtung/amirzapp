// use linked lists to store other words

#include <iostream>
#include <string>
#include <list>
#include <cstdlib>
#include <ctime>
using namespace std;

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

bool find_in_others(list<string> others, string s) {
    list<string>::iterator it = others.begin();
    while (it != others.end()) {
        if (*it == s) {
            return true;
        }
        ++it;
    }
    return false;
}

string tolower(string s) {
    for (int i = 0; i < s.length(); i++) {
        s[i] = tolower(s[i]);
    }
    return s;
}

int main() {
    srand(time(0));

    const string target = "prince";
    const list<string> other_words = {"i", "ice", "in", "nice", "pi", "epic", "pie", "pin", "nip", "pine", "pen", "per", "price", "pier", "ripe", "pincer", "ripen", "rice", "eric", "nicer", "rein"};

    cout << "The letters are: ";
    print_scrambled(target);

    cout << "Please guess the target word. Press Ctrl-D to exit." << endl;

    string input;
    while (cin >> input) {
        input = tolower(input);
        if (input == target) {
            cout << "Congratulations! You found the target word!" << endl;
            return 0;
        } else if (find_in_others(other_words, input)) {
            cout << "Nice, but it's not what I have in mind!" << endl;
        } else {
            cout << "Is it even a word?!" << endl;
        }
    }
    cout << "Giving up so soon? Why?!" << endl;
}