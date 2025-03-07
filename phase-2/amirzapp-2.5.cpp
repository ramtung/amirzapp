#include <iostream>
#include <string>
#include <list>
#include <algorithm>
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

int main() {
  srand(time(0));

  const string target = "prince";
  const list<string> otherWords = {"i", "ice", "in", "nice", "pi", "epic", "pie", "pin", "nip", "pine", "pen", "per", "price", "pier", "ripe", "pincer", "ripen", "rice", "eric", "nicer", "rein"};

  cout << "The letters are: ";
  printScrambled(target);

  cout << "Please enter the word. "
       << "Press Ctrl-D to exit." << endl;

  string input;
  while (cin >> input) {
    input = toLower(input);
    if (input == target) {
      cout << "Congratulations! "
           << "You found the target word!" << endl;
       return 0;
    } else if (find(otherWords.begin(), otherWords.end(), 
                    input) != otherWords.end()) {
      cout << "Nice, "
           << "but it's not what I have in mind!" << endl;
    } else if (containsExtraCharacters(input, target)) {
      cout << "You used some extra characters!" << endl;
    } else {
      cout << "Sorry, please try again!" << endl;
    }
  }
  cout << "Giving up so soon? Why?!" << endl;
}