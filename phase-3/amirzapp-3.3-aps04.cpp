#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <list>
#include <cstdlib>
#include <ctime>
using namespace std;

struct GameWord {
  string word;
  bool found;
};

struct Level {
  string targetLetters;
  list<GameWord> targetWords;
  list<GameWord> bonusWords;
};

string scramble(string s) {
  for (int i = 0; i < s.length(); i++) {
    int j = rand() % s.length();
    swap(s[i], s[j]);
  }
  return s;
}

void printLetters(string targetLetters) {
  for (int i = 0; i < targetLetters.length(); i++) {
    cout << targetLetters[i] << ' ';
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

void printLevel(Level lvl) {
  for (GameWord tword : lvl.targetWords) {
    if (tword.found)
      cout << tword.word << endl;
    else
      cout << string(tword.word.length(), '-') << endl;
  }
  cout << "The letters are: ";
  printLetters(lvl.targetLetters);
}

void sortTargetWords(Level& lvl) {
  if (lvl.targetWords.empty()) return;

  for (auto it1 = lvl.targetWords.begin(); it1 != lvl.targetWords.end(); ++it1) {
    for (auto it2 = lvl.targetWords.begin(); next(it2) != lvl.targetWords.end(); ++it2) {
      if ((it2->word.length() > next(it2)->word.length()) || 
          (it2->word.length() == next(it2)->word.length() && it2->word > next(it2)->word)) {
        swap(*it2, *next(it2));
      }
    }
  }
}

vector<Level> readLevels(string filename) {
  vector<Level> levels;
  ifstream levelFile(filename);
  string levelStr;
  getline(levelFile, levelStr); // skip the header

  while (getline(levelFile, levelStr)) {
    Level lvl;
    string word;
    istringstream levelStream(levelStr);     
    levelStream >> lvl.targetLetters;
    lvl.targetLetters = scramble(lvl.targetLetters);
    while (levelStream >> word && (word != "-")) {
      lvl.targetWords.push_back({word, false});
    }
    while (levelStream >> word)
      lvl.bonusWords.push_back({word, false});
    sortTargetWords(lvl);
    levels.push_back(lvl);
  }
  return levels;
}

int main() {
  srand(time(0));
  const vector<Level> levels = readLevels("levels-5.txt");

  cout << "Find all the target words in the levels." << endl 
       << "Gain extra points by finding bonus words." << endl
       << "Press Ctrl-D to exit." << endl;
}