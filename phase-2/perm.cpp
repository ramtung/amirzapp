#include <iostream>
#include <fstream>
#include <string>
#include <set>
using namespace std;

set<string> words;

set<string> all_subsets(string s) {
    set<string> subsets;
    for (int i = 0; i < (1 << s.length()); i++) {
        string subset;
        for (int j = 0; j < s.length(); j++) {
            if (i & (1 << j)) {
                subset += s[j];
            }
        }
        subsets.insert(subset);
    }
    return subsets;
}

void print_all_permutations(string s, int i = 0) {
    if (i == s.length()) {
        if (words.count(s) != 0)
            cout << s << endl;
        return;
    }
    for (int j = i; j < s.length(); j++) {
        swap(s[i], s[j]);
        print_all_permutations(s, i + 1);
        swap(s[i], s[j]);
    }
}

int main() {
    ifstream file("words_alpha.txt");
    string word;
    while (file >> word) {
        words.insert(word);
    }

    set<string> subsets = all_subsets("prince");
    for (string subset : subsets) {
        print_all_permutations(subset);
        // cout << subset << endl;
    }

    print_all_permutations("prince");
}