/***
 * Class to hold game states, values of nodes, min/max bounds|window
 *
 */
#ifndef STATE_H
#define STATE_H
#include <iomanip>
#include <iostream>
#include <vector>
#include <regex>

using namespace std;
typedef pair<char, int> action;

class state {
private:
    int d;                                      // board dimension
    char **board;                               // X, O, - (ASCII: 88, 79, 45)
    action takenToGetHere;                      // No need to keep track of parent just the move so the parent knows
    state* parent;
    vector<state*>* successors;                 // Ordered list of successors TODO Not sure if we need this I think we should have to generate this every time we come to a state

public:

    state(int dimension = 8);                   // constructor
    state(char** fromBoard, int dimension = 8);
    state(const state &original);               // copy constructor
    ~state();                                   // destructor
    char** getBoard() const;                    // accessor for board
    int getDimension() const;                   // accessor for d
    bool makeMove(char r, int c, char typeChar);// success|failure
    action getActionTakenToGetHere();
    vector<state*> getOrderedSuccessors(char typeChar, vector<regex>* orderOfSuccession);

    friend std::ostream& operator<<(std::ostream&, const state&);
    string getStringFromRow(char *row);
};

// default constructor
state::state(int dimension) {
    this->d = dimension;
    this->board = new char *[this->d]; // structure
    for (int i = 0; i < this->d; i++) {
        this->board[i] = new char[this->d];
        for (int j = 0; j < this->d; j++) {
            this->board[i][j] = '-'; // empty
        }
    }
}

// convenience constructor to set up a board from a 2-dimensional array
state::state(char** fromBoard, int dimension) : state(dimension) {
    for (int i = 0; i < this->d; i++) {
        for (int j = 0; j < this->d; j++) {
            this->board[i][j] = fromBoard[i][j];
        }
    }
}

// copy constructor
state::state(const state &original) {
    this->d = original.getDimension();
    this->board = new char*[this->d];
    for (int i = 0; i < this->d; i++) {
        this->board[i] = new char[this->d];
        for (int j = 0; j < this->d; j++) {
            this->board[i][j] = original.getBoard()[i][j]; // [] dereferences
        }
    }
}

// destructor
state::~state() {
    for (int i = 0; i < this->d; i++) {
        delete this->board[i];
    } // delete the rows in freestore
}

char** state::getBoard() const {
    return this->board;
}

int state::getDimension() const {
    return this->d;
}

// ternary move returns true if move was made, false otherwise... except in the off chance your typeChar is 0
bool state::makeMove(char r, int c, char typeChar) {
    this->takenToGetHere = action(char(r+'A'), c);
    return this->board[r-'A'][c-1] == '-' ? (this->board[r-'A'][c-1] = typeChar) : false;
}

string state::getStringFromRow(char* row){
    return string(row);
}

// TODO XXX!!!!!
vector<state*> state::getOrderedSuccessors(char typeChar, vector<regex>* orderOfSuccession) {
    char altChar = typeChar == 'X' ? 'O' : 'X';
    if (this->successors == nullptr) {
        this->successors = new vector<state *>();
        char **transpose_board = new char*[d];
        for (int i = 0; i < d; i++)
            transpose_board[i] = new char[d];
        for (int i = 0; i < d; i++) {
            for (int j = 0; j < d; j++) {
                transpose_board[j][i] = this->board[i][j];
            }
        }
        smatch matcher;
        if (typeChar == 'X') {
            // priority Block O
            // priority longer chains
            // when we make a state here set parent to this state
            // then remember to successor->makeMove()

        } else {
            swap(orderOfSuccession[0], orderOfSuccession[1]);
            swap(orderOfSuccession[2], orderOfSuccession[3]);
            swap(orderOfSuccession[4], orderOfSuccession[5]);
            // opposite of above
        }
        for (regex expression : *orderOfSuccession) {
            for (int i = 0; i < d; i++) {
                string transposeRow = getStringFromRow(transpose_board[i]);
                string row = getStringFromRow(this->board[i]);
                string rowTemp(row);
                while (regex_search(rowTemp, matcher, expression)) {
                    int pos;
                    if (matcher[1].length() == 0) {
                        pos = (int)matcher.prefix().length() + (int)matcher[0].length() - 1;
                    } else {
                        pos = (int)matcher.prefix().length();
                    }
                    state *successor_state = new state(this->board, this->d);
                    successor_state->makeMove(i + 'A', pos, typeChar);
                    this->successors->push_back(successor_state);
                    rowTemp = matcher.suffix().str();
                }
                rowTemp = transposeRow;
                while (regex_search(rowTemp, matcher, expression)) {
                    int pos;
                    if (matcher[1].length() == 0) {
                        pos = (int)matcher.prefix().length() + (int)matcher[0].length() - 1;
                    } else {
                        pos = (int)matcher.prefix().length();
                    }
                    // this means a column i has a killer move and row pos
                    state *successor_state = new state(this->board, this->d);
                    successor_state->makeMove((char) (pos + 'A'), i, typeChar);
                    this->successors->push_back(successor_state);
                    rowTemp = matcher.suffix().str();
                }
            }
        }
    }
    if (this->successors->size() == 0) { // if there was nothing else on the board then push back a random
        state *random_successor = new state(this->board, this->d);
        while (!random_successor->makeMove((char) (rand() % this->d), rand() % this->d, typeChar));
        this->successors->push_back(random_successor);
    }
    return *this->successors;
}

action state::getActionTakenToGetHere() {
    return this->takenToGetHere;
}


// friends
ostream& operator<<(ostream& stream, const state& s) {
    // pretty print?
    int dim = s.getDimension();
    int A = 'A';
    stream << setw(3) << " ";
    for (int i = 0; i < dim; i++)
        stream << setw(2) << i+1;
    stream << endl;
    for (int i = 0; i < dim; i++) {
        stream << setw(2) << char('A'+i) << " ";
        for (int j = 0; j < dim; j++) {
            stream << setw(2) << s.getBoard()[i][j];
        }
        stream << endl;
    }
    return stream;
}


#endif // STATE_H