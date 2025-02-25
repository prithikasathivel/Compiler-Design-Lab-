// Online C++ compiler to run C++ program online
#include <iostream>
#include <bits/stdc++.h>
using namespace std;

map<char, set<char>> first, follow;
map<char, vector<string>> productions;
set<char> nonTerminals, terminals;
char startSymbol;

void computeFirst(char symbol) {
    if (!first[symbol].empty()) return;
    for (auto &prod : productions[symbol]) {
        for (int i = 0; i < prod.size(); i++) {
            char ch = prod[i];
            if (terminals.count(ch)) {
                first[symbol].insert(ch);
                break;
            } else {
                computeFirst(ch);
                first[symbol].insert(first[ch].begin(), first[ch].end());
                if (first[ch].count('#') == 0) break;
            }
        }
    }
}

void computeFollow(char symbol) {
    if (!follow[symbol].empty()) return;
    if (symbol == startSymbol) follow[symbol].insert('$');
    for (auto &prod : productions) {
        for (auto &rule : prod.second) {
            for (int i = 0; i < rule.size(); i++) {
                if (rule[i] == symbol) {
                    bool nullable = true;
                    for (int j = i + 1; j < rule.size(); j++) {
                        char nextSymbol = rule[j];
                        follow[symbol].insert(first[nextSymbol].begin(), first[nextSymbol].end());
                        if (first[nextSymbol].count('#') == 0) {
                            nullable = false;
                            break;
                        }
                    }
                    if (nullable && prod.first != symbol) {
                        computeFollow(prod.first);
                        follow[symbol].insert(follow[prod.first].begin(), follow[prod.first].end());
                    }
                }
            }
        }
    }
    follow[symbol].erase('#');
}

void createParsingTable(map<pair<char, char>, string> &parsingTable) {
    for (auto &prod : productions) {
        char nonTerm = prod.first;
        for (auto &rule : prod.second) {
            for (char ch : first[rule[0]]) {
                if (ch != '#') parsingTable[{nonTerm, ch}] = rule;
            }
            if (first[rule[0]].count('#')) {
                for (char ch : follow[nonTerm]) {
                    parsingTable[{nonTerm, ch}] = rule;
                }
            }
        }
    }
}

int main() {
    int numProductions;
    cout << "Enter number of productions: ";
    cin >> numProductions;
    cout << "Enter productions (e.g., S abS | aSb):\n";
    cin.ignore();
    
    for (int i = 0; i < numProductions; i++) {
        string input;
        getline(cin, input);
        char nonTerm = input[0];
        string rightHand = input.substr(2);
        stringstream ss(rightHand);
        string rule;
        while (getline(ss, rule, '|')) {
            productions[nonTerm].push_back(rule);
        }
        nonTerminals.insert(nonTerm);
    }
    
    cout << "Enter start symbol: ";
    cin >> startSymbol;
    
    for (auto &prod : productions) {
        for (auto &rule : prod.second) {
            for (char ch : rule) {
                if (!isupper(ch)) terminals.insert(ch);
            }
        }
    }
    terminals.insert('$');
    
    for (char nt : nonTerminals) computeFirst(nt);
    for (char nt : nonTerminals) computeFollow(nt);
    
    cout << "FIRST sets:\n";
    for (auto &p : first) {
        cout << "FIRST(" << p.first << ") = { ";
        for (char c : p.second) cout << c << " ";
        cout << "}\n";
    }
    
    cout << "FOLLOW sets:\n";
    for (auto &p : follow) {
        cout << "FOLLOW(" << p.first << ") = { ";
        for (char c : p.second) cout << c << " ";
        cout << "}\n";
    }
    
    map<pair<char, char>, string> parsingTable;
    createParsingTable(parsingTable);
    
    cout << "\nPredictive Parsing Table:\n";
    cout << "----------------------------------\n";
    cout << "Non-Terminal | Terminal | Production\n";
    cout << "----------------------------------\n";
    for (auto &entry : parsingTable) {
        cout << "    " << entry.first.first << "          " << entry.first.second << "        " << entry.second << "\n";
    }
    cout << "----------------------------------\n";
    return 0;
}
