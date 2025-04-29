#include <iostream>
#include <vector>
#include <string>
#include <sstream>

using namespace std;

struct Quadruple {
    string op, arg1, arg2, result;
};

struct Triple {
    string op, arg1, arg2;
};

int main() {
    vector<Quadruple> quads;
    vector<Triple> triples;
    vector<int> indirectTriple;

    string line;
    cout << "Enter expression (use space between tokens, e.g. a = b * - c + b * - c):\n";
    getline(cin, line);
    istringstream iss(line);

    vector<string> tokens;
    string tok;
    while (iss >> tok) tokens.push_back(tok);

    if (tokens.size() < 5 || tokens[1] != "=") {
        cout << "Invalid format. Example: a = b * - c + b * - c\n";
        return 1;
    }

    string resultVar = tokens[0];
    int tempCount = 1;

    for (size_t i = 2; i < tokens.size(); ) {
        if (tokens[i] == "-") {
            string temp1 = "T" + to_string(tempCount++);
            quads.push_back({"uminus", tokens[i+1], "", temp1});
            triples.push_back({"uminus", tokens[i+1], ""});
            i += 2;

            string temp2 = "T" + to_string(tempCount++);
            quads.push_back({"*", tokens[i-3], temp1, temp2});
            triples.push_back({"*", tokens[i-3], "(" + to_string(triples.size()-2) + ")"});
        } else if (tokens[i] == "+") {
            string temp3 = "T" + to_string(tempCount++);
            string t1 = "T" + to_string(tempCount - 3); // previous result
            string t2 = "T" + to_string(tempCount - 1); // last multiplication
            quads.push_back({"+", t1, t2, temp3});
            triples.push_back({"+", "(" + to_string(triples.size()-3) + ")", "(" + to_string(triples.size()-1) + ")"});
            i++;
        } else {
            i++;
        }
    }

    string finalTemp = "T" + to_string(tempCount - 1);
    quads.push_back({"=", finalTemp, "", resultVar});
    triples.push_back({"=", resultVar, "(" + to_string(triples.size() - 1) + ")"});

    for (int i = 0; i < triples.size(); ++i)
        indirectTriple.push_back(i);

    // Print Quadruples
    cout << "\nQuadruple Representation:\n";
    cout << "Loc\tOp\tArg1\tArg2\tResult\n";
    for (int i = 0; i < quads.size(); ++i)
        cout << i+1 << "\t" << quads[i].op << "\t" << quads[i].arg1 << "\t" << quads[i].arg2 << "\t" << quads[i].result << endl;

    // Print Triples
    cout << "\nTriple Representation:\n";
    cout << "Loc\tOp\tArg1\tArg2\n";
    for (int i = 0; i < triples.size(); ++i)
        cout << i+1 << "\t" << triples[i].op << "\t" << triples[i].arg1 << "\t" << triples[i].arg2 << endl;

    // Indirect Triple
    cout << "\nIndirect Triple Representation:\n";
    cout << "Statement\tPoints to\n";
    for (int i = 0; i < indirectTriple.size(); ++i)
        cout << (35+i) << "\t\t(" << indirectTriple[i]+1 << ")\n";

    cout << "\nLocation\tOp\tArg1\tArg2\n";
    for (int i = 0; i < triples.size(); ++i)
        cout << i+1 << "\t\t" << triples[i].op << "\t" << triples[i].arg1 << "\t" << triples[i].arg2 << endl;

    return 0;
}
