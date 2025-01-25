#include <iostream>
#include <string>
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

void printScrambled(string s) {
    string scrambled = scramble(s);
    for (int i = 0; i < scrambled.length(); i++) {
        cout << scrambled[i] << ' ';
    }
    cout << endl;
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

string toLower(string s) {
    for (int i = 0; i < s.length(); i++) {
        s[i] = tolower(s[i]);
    }
    return s;
}

int main() {
    srand(time(0));

    const string target = "prince";
    cout << "The letters are: ";
    printScrambled(target);

    cout << "Please enter the word. Press Ctrl-D to exit." << endl;

    string input;
    while (cin >> input) {
        input = toLower(input);
        if (input == target) {
            cout << "Congratulations! You found the target word!" << endl;
            break;
        } else if (containsExtraCharacters(input, target)) {
            cout << "You used some extra characters!" << endl;
        } else {
            cout << "Sorry, wrong word! Please try again!" << endl;
        }
    }
}