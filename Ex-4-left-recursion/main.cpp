#include <iostream>
#include <vector>
#include <string>
using namespace std;

// Function to check and eliminate left recursion
void eliminateLeftRecursion(string nonTerminal, vector<string> productions) {
    vector<string> alpha, beta;
    string newNonTerminal = nonTerminal + "'";
    bool hasLeftRecursion = false;

    // Splitting productions into alpha (left-recursive) and beta (non-left-recursive)
    for (string prod : productions) {
        if (prod[0] == nonTerminal[0]) { // Left recursion case A -> Aα
            hasLeftRecursion = true;
            alpha.push_back(prod.substr(1)); // Store α
        } else {
            beta.push_back(prod); // Store β
        }
    }

    if (!hasLeftRecursion) {
        // If no left recursion, print original production
        cout << nonTerminal << " -> ";
        for (size_t i = 0; i < productions.size(); i++) {
            if (i > 0) cout << " | ";
            cout << productions[i];
        }
        cout << endl;
        return;
    }

    // Printing transformed grammar
    cout << nonTerminal << " -> ";
    for (size_t i = 0; i < beta.size(); i++) {
        if (i > 0) cout << " | ";
        cout << beta[i] << newNonTerminal;
    }
    cout << endl;

    cout << newNonTerminal << " -> ";
    for (size_t i = 0; i < alpha.size(); i++) {
        if (i > 0) cout << " | ";
        cout << alpha[i] << newNonTerminal;
    }
    cout << " | ε" << endl;
}

int main() {
    vector<pair<string, vector<string>>> grammar = {
        {"A", {"ABd", "Aa", "a"}},
        {"B", {"Be", "b"}}
    };

    for (const auto &rule : grammar) {
        eliminateLeftRecursion(rule.first, rule.second);
    }
    return 0;
}

