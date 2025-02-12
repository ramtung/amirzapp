#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <set>
#include <list>
#include <vector>
#include <cstdlib>
#include <ctime>
using namespace std;

int main() {
  srand(time(0));

  ifstream levelFile("levels.txt");
  string line;
  int levelNo = 0;
  while (getline(levelFile, line)) {
    cout << endl << "Level " << ++levelNo << endl;
    string target;
    set<string> otherWords;
    int pos = 0;
    while (pos < line.size() && line[pos] != ' ')
      ++pos;
    target = line.substr(0, pos);
    int start = pos;
    while (start < line.size() && line[start] == ' ')
      ++start;
    while (start < line.size()) {
      pos = start;
      while (pos < line.size() && line[pos] != ' ')
        ++pos;
      otherWords.insert(line.substr(start, pos-start));
      start = pos;
      while (start < line.size() && line[start] == ' ')
        ++start;
    }
    vector<bool> masraf(target.size());
    string s = target;
    cout << "The letters are: ";
    for (int i = 0; i < s.length(); i++) {
      int j = rand() % s.length();
      swap(s[i], s[j]);
    }
    for (int i = 0; i < s.length(); i++)
      cout << s[i] << ' ';
    cout << endl << "Please guess the target word. Press Ctrl-D to exit." << endl;

    bool flag = false;
    string input;
    while (cin >> input) {
      for (int i = 0; i < input.length(); i++)
        input[i] = tolower(input[i]);
      if (input == target) {
        cout << "Congratulations! You found the target word!" << endl;
        flag = true;
        break;
      } else if (otherWords.count(input) != 0) {
        cout << "Nice, but it's not what I have in mind!" << endl;
      } else {
        for (int i = 0; i < masraf.size(); i++)
          masraf[i] = false;
        bool flag3 = false;
        for (int i = 0; i < input.length(); i++) {
          bool flag2 = false;
          for (int j = 0; j < target.length(); j++) {
            if (masraf[j])
            continue;
            if (target[j] == input[i])
              masraf[j] = true;
              flag2 = true;
          }
          if (!flag2) {
            cout << "You used some extra characters!" << endl;
            flag3 = true;
            break;
          }
        }
        if (!flag3)
          cout << "Is it even a word?!" << endl;
      }
    }
    if (!flag) {
      cout << "Giving up so soon? Why?!" << endl;
      return 0;
    }
  }
  cout << endl << "Congratulations! You solved all levels!" << endl;
}
