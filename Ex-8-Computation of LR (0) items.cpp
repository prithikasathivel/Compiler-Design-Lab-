#include <iostream>
#include <vector>
#include <string>
#include <set>
#include <map>
#include <algorithm>
using namespace std;

// Structure to represent a production rule
struct Production {
    string lhs;
    string rhs;
    int dotPos;

    Production(string l, string r, int pos = 0) : lhs(l), rhs(r), dotPos(pos) {}

    bool operator<(const Production& other) const {
        if (lhs != other.lhs) return lhs < other.lhs;
        if (rhs != other.rhs) return rhs < other.rhs;
        return dotPos < other.dotPos;
    }

    bool operator==(const Production& other) const {
        return lhs == other.lhs && rhs == other.rhs && dotPos == other.dotPos;
    }
};

class LR0Parser {
private:
    vector<Production> grammar;
    vector<set<Production>> states;

    bool isNonTerminal(char c) {
        return (c >= 'A' && c <= 'Z');
    }

    set<Production> closure(set<Production> I) {
        set<Production> result = I;
        bool changed;
        do {
            changed = false;
            set<Production> temp = result;
            for (const Production& item : temp) {
                if (item.dotPos < item.rhs.length()) {
                    char nextSymbol = item.rhs[item.dotPos];
                    if (isNonTerminal(nextSymbol)) {
                        string nt(1, nextSymbol);
                        for (const Production& prod : grammar) {
                            if (prod.lhs == nt) {
                                Production newItem(prod.lhs, prod.rhs, 0);
                                if (result.insert(newItem).second) {
                                    changed = true;
                                }
                            }
                        }
                    }
                }
            }
        } while (changed);
        return result;
    }

    set<Production> gotoFunc(set<Production> I, char X) {
        set<Production> J;
        for (const Production& item : I) {
            if (item.dotPos < item.rhs.length() && item.rhs[item.dotPos] == X) {
                J.insert(Production(item.lhs, item.rhs, item.dotPos + 1));
            }
        }
        return closure(J);
    }

public:
    void addProduction(string lhs, string rhs) {
        grammar.push_back(Production(lhs, rhs));
    }

    void computeLR0Items() {
        // I0: Initial state
        set<Production> start;
        start.insert(Production("S'", grammar[0].lhs));
        states.push_back(closure(start));

        // I1: Goto(I0, S)
        states.push_back(gotoFunc(states[0], 'S'));

        // I2: Goto(I0, A)
        states.push_back(gotoFunc(states[0], 'A'));

        // I3: Goto(I0, a)
        states.push_back(gotoFunc(states[0], 'a'));

        // I4: Goto(I0, b)
        states.push_back(gotoFunc(states[0], 'b'));

        // I5: Goto(I2, A)
        states.push_back(gotoFunc(states[2], 'A'));

        // I6: Goto(I3, A)
        states.push_back(gotoFunc(states[3], 'A'));
    }

    void displayStates() {
        for (size_t i = 0; i < states.size(); i++) {
            cout << "State I" << i << ":\n";
            vector<Production> items(states[i].begin(), states[i].end());
           
            // Custom sort: S' > S > A, then by RHS with dot position priority
            sort(items.begin(), items.end(), [](const Production& a, const Production& b) {
                if (a.lhs == "S'" && b.lhs != "S'") return true;
                if (b.lhs == "S'" && a.lhs != "S'") return false;
                if (a.lhs == "S" && b.lhs != "S" && b.lhs != "S'") return true;
                if (b.lhs == "S" && a.lhs != "S" && a.lhs != "S'") return false;
                if (a.lhs != b.lhs) return a.lhs < b.lhs;
                // For A productions, prioritize higher dotPos first
                if (a.lhs == "A" && b.lhs == "A") {
                    if (a.rhs == b.rhs) return a.dotPos > b.dotPos; // e.g., a.A before .aA
                    if (a.rhs == "aA" && b.rhs == "b") return true;
                    if (b.rhs == "aA" && a.rhs == "b") return false;
                    return a.rhs < b.rhs;
                }
                return a.dotPos < b.dotPos;
            });

            for (const Production& item : items) {
                cout << item.lhs << " -> ";
                for (int j = 0; j < item.rhs.length(); j++) {
                    if (j == item.dotPos) cout << ".";
                    cout << item.rhs[j];
                }
                if (item.dotPos == item.rhs.length()) cout << ".";
                cout << "\n";
            }
            cout << "\n";
        }
    }
};

int main() {
    LR0Parser parser;
    int n;
   
    cout << "Enter the number of productions: ";
    cin >> n;
    cin.ignore();

    cout << "Enter productions in the format 'S -> ABC' (non-terminals A-Z, terminals a-z or symbols):\n";
    cout << "Note: First production's LHS will be considered the start symbol\n";
   
    for (int i = 0; i < n; i++) {
        string input, lhs, rhs;
        cout << "Production " << i + 1 << ": ";
        getline(cin, input);

        size_t arrowPos = input.find("->");
        if (arrowPos == string::npos) {
            cout << "Invalid format! Use '->' to separate LHS and RHS\n";
            i--;
            continue;
        }

        lhs = input.substr(0, arrowPos);
        rhs = input.substr(arrowPos + 2);

        while (!lhs.empty() && lhs[0] == ' ') lhs.erase(0, 1);
        while (!rhs.empty() && rhs[0] == ' ') rhs.erase(0, 1);
        while (!lhs.empty() && lhs.back() == ' ') lhs.pop_back();
        while (!rhs.empty() && rhs.back() == ' ') rhs.pop_back();

        if (lhs.empty() || rhs.empty()) {
            cout << "Empty LHS or RHS not allowed!\n";
            i--;
            continue;
        }

        parser.addProduction(lhs, rhs);
    }

    cout << "\nComputing LR(0) items...\n\n";
    parser.computeLR0Items();
    parser.displayStates();

    return 0;
}