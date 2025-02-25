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

void printLevel(const Level& lvl) {
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

enum GuessResult {NOT_FOUND, FOUND_NEW, FOUND_ALREADY};

GuessResult checkForTargetWord(const Level& level, string input) {
  for (auto gameWord : level.targetWords) {
    if (gameWord.word == input)
      return gameWord.found ? FOUND_ALREADY : FOUND_NEW;
  }
  return NOT_FOUND;
}

void markTargetWordAsFound(Level& level, string input) {
  for (auto gameWord : level.targetWords) {
    if (gameWord.word == input) {
      gameWord.found = true;
      return;
    }
  }
}

bool playLevel(Level& level) {
  printLevel(level);
  string input;
  while (cin >> input) {
    input = toLower(input);
    GuessResult targetGuessResult = checkForTargetWord(level, input);
    if (targetGuessResult == FOUND_NEW) {
      markTargetWordAsFound(level, input);
      cout << "Target word found!" << endl;
      printLevel(level);
      if (foundAllTargetWords(level))
        return true;
    } else if (targetGuessResult == FOUND_ALREADY) {
      cout << "Target word found already!" << endl;
    } else if (foundNewBonusWord(level, input)) {
      cout << "Bonus word found!" << endl;
    } else if (containsExtraCharacters(input, level.targetLetters)) {
      cout << "You used extra characters!" << endl;
    } else {
      cout << "Sorry! Try again!" << endl;
    }
  }
  return false;
}

void playGame(vector<Level>& levels) {
  int levelNo = 0;
  for (Level& level : levels) {
    levelNo++;
    cout << "Level " << levelNo << endl;
    if (!playLevel(level)) {
      cout << "Giving up so sonn! Why?!" << endl;
      return;
    }
  }
  cout << "Congratulations! You passed all levels!" << endl;
}

int main() {
  srand(time(0));
  vector<Level> levels = readLevels("levels-5.txt");

  cout << "Find all the target words in the levels." << endl 
       << "Gain extra points by finding bonus words." << endl
       << "Press Ctrl-D to exit." << endl;

  playGame(levels);
}