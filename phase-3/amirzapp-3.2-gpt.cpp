#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <set>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <algorithm>

using namespace std;

void shuffleWord(string& word) {
  random_shuffle(word.begin(), word.end());
}

void displayShuffledWord(const string& word) {
  cout << "The letters are: ";
  for (char c : word) {
    cout << c << ' ';
  }
  cout << endl;
}

bool isValidGuess(const string& guess, const string& target, const set<string>& otherWords) {
  if (guess == target) {
    cout << "Congratulations! You found the target word!" << endl;
    return true;
  }
  if (otherWords.count(guess) > 0) {
    cout << "Nice, but it's not what I have in mind!" << endl;
  } else {
    set<char> targetChars(target.begin(), target.end());
    for (char c : guess) {
      if (targetChars.find(c) == targetChars.end()) {
        cout << "You used some extra characters!" << endl;
        return false;
      }
    }
    cout << "Is it even a word?!" << endl;
  }
  return false;
}

void playLevel(const string& target, const set<string>& otherWords) {
  string shuffled = target;
  shuffleWord(shuffled);
  displayShuffledWord(shuffled);
  cout << "Please guess the target word. Press Ctrl-D to exit." << endl;
  
  string input;
  while (cin >> input) {
    transform(input.begin(), input.end(), input.begin(), ::tolower);
    if (isValidGuess(input, target, otherWords)) {
      return;
    }
  }
  cout << "Giving up so soon? Why?!" << endl;
  exit(0);
}

void loadLevels(const string& filename) {
  ifstream levelFile(filename);
  if (!levelFile) {
    cerr << "Error opening levels file!" << endl;
    exit(1);
  }
  
  string line;
  int levelNo = 0;
  while (getline(levelFile, line)) {
    cout << endl << "Level " << ++levelNo << endl;
    
    istringstream iss(line);
    string target;
    iss >> target;
    
    set<string> otherWords;
    string word;
    while (iss >> word) {
      otherWords.insert(word);
    }
    
    playLevel(target, otherWords);
  }
  cout << endl << "Congratulations! You solved all levels!" << endl;
}

int main() {
  srand(static_cast<unsigned int>(time(0)));
  loadLevels("levels.txt");
  return 0;
}
