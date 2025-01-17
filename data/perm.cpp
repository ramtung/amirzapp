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

void print_all_permutations(ostream& out, string s, int i = 0) {
    if (i == s.length()) {
        if (words.count(s) != 0)
            out << s << endl;
        return;
    }
    for (int j = i; j < s.length(); j++) {
        swap(s[i], s[j]);
        print_all_permutations(out, s, i + 1);
        swap(s[i], s[j]);
    }
}

int main() {
    ifstream file("mit-100000.txt");
    string word;
    while (file >> word) {
        words.insert(word);
    }

    cout << "Enter a word: ";
    cin >> word;

    ofstream out(word + "-words.txt");
    set<string> subsets = all_subsets(word);
    for (string subset : subsets) {
        if (subset.length() < 3)
            continue;
        print_all_permutations(out, subset);
    }

}