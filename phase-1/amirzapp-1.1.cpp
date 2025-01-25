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

int main() {
    srand(time(0));

    const string target = "prince";
    cout << "The letters are: ";
    printScrambled(target);

    cout << "Please enter the word. Press Ctrl-D to exit." << endl;

    string input;
    while (cin >> input) {
        if (input == target) {
            cout << "Congratulations! You found the target word!" << endl;
            break;
        } else {
            cout << "Sorry, wrong word! Please try again!" << endl;
        }
    }
}