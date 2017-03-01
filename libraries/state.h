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
#include <queue>

using namespace std;
typedef pair<char, int> action;

class state {
private:
    int d;                                      // board dimension
    char **board;                               // X, O, - (ASCII: 88, 79, 45)
    double value;                               // why calculate these things twice?
    action takenToGetHere;                      // No need to keep track of parent just the move so the parent knows
    state* parent;
    priority_queue<state*, vector<state*>, greater_equal<state*> >* successorsMax;
    priority_queue<state*, vector<state*>, less_equal<state*> >* successorsMin;
    // Ordered list of successors TODO Not sure if we need this I think we should have to generate this every time we come to a state

public:

    state(int dimension = 8);                   // constructor
    state(char** fromBoard, int dimension = 8);
    state(const state &original);               // copy constructor
    ~state();                                   // destructor
    char** getBoard() const;                    // accessor for board
    int getDimension() const;                   // accessor for d
    double getValue() const;
    void setValue(double value);
    bool makeMove(char r, int c, char typeChar);// success|failure
    action getActionTakenToGetHere();
    priority_queue<state*, vector<state*>, greater_equal<state*>>& getOrderedSuccessorsMax(vector<regex> orderOfSuccession);
    priority_queue<state*, vector<state*>, less_equal<state*>>& getOrderedSuccessorsMin(vector<regex> orderOfSuccession);

    friend std::ostream& operator<<(std::ostream&, const state&);
    string getStringFromRow(char *row);

    template <typename T>
    void operateOrderOfSuccession(vector<regex> orderOfSuccession, priority_queue<state *, vector<state *>, T>*& pq, bool isMax, char typeChar);
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
    this->takenToGetHere = action(r, c+1);
    return this->board[r-'A'][c-1] == '-' ? (this->board[r-'A'][c-1] = typeChar) : false;
}

string state::getStringFromRow(char* row){
    return string(row);
}


priority_queue<state *, vector<state *>, less_equal<state *>> &
state::getOrderedSuccessorsMin(vector<regex> orderOfSuccession) {
    this->successorsMin = new priority_queue<state*, vector<state*>, less_equal<state*> >();
    swap(orderOfSuccession[0], orderOfSuccession[1]);
    swap(orderOfSuccession[2], orderOfSuccession[3]);
    swap(orderOfSuccession[4], orderOfSuccession[5]);
    this->operateOrderOfSuccession(orderOfSuccession, this->successorsMin, true, 'O');
    return *(this->successorsMin);
}

char** transpose(char **board, int d) {
    char **transpose_board = new char *[d];
    for (int i = 0; i < d; i++)
        transpose_board[i] = new char[d];
    for (int i = 0; i < d; i++) {
        for (int j = 0; j < d; j++) {
            transpose_board[j][i] = board[i][j];
        }
    }
    return transpose_board;
};

template <typename T>
void state::operateOrderOfSuccession(vector<regex> orderOfSuccession, priority_queue<state*, vector<state*>, T>*& pq, bool isMax, char typeChar) {
    char **transpose_board = transpose(this->board, this->d);
    smatch matcher;
    for (regex expression : orderOfSuccession) {
        for (int i = 0; i < d; i++) {
            string transposeRow = getStringFromRow(transpose_board[i]);
            string row = getStringFromRow(this->board[i]);
            string rowTemp(row);
            while (regex_search(rowTemp, matcher, expression)) {
                int pos;
                if (matcher[1].length() == 0) {
                    pos = (int) matcher.prefix().length() + (int) matcher[0].length();
                } else {
                    pos = (int) matcher.prefix().length();
                }
                state *successor_state = new state(this->board, this->d);
                successor_state->makeMove((char)(i + 'A'), pos, typeChar);
                if (matcher[0].length() == 4) {
                    successor_state->setValue(isMax? 1:-1);
                } else if (matcher[0].length() == 3) {
                    successor_state->setValue(isMax? .75:-.75);
                } else if (matcher[0].length() == 2) {
                    successor_state->setValue(isMax? .5:-.5);
                } else {
                    rowTemp = matcher.suffix().str();
                    pq->push(successor_state);
                    continue;
                }
                pq->push(successor_state);
                return;
            }
            rowTemp = transposeRow;
            while (regex_search(rowTemp, matcher, expression)) {
                int pos;
                pos = matcher[1].length() == 0 ? (int) matcher.prefix().length() + (int) matcher[0].length() - 1 : (int) matcher.prefix().length();
                // this means a column i has a killer move and row pos
                state *successor_state = new state(this->board, this->d);
                successor_state->makeMove((char) (pos + 'A'), i, typeChar);
                if (matcher[0].length() == 4) {
                    successor_state->setValue(isMax? 1:-1);
                } else if (matcher[0].length() == 3) {
                    successor_state->setValue(isMax? .75:-.75);
                } else if (matcher[0].length() == 2) {
                    successor_state->setValue(isMax? .5:-.5);
                } else {
                    rowTemp = matcher.suffix().str();
                    pq->push(successor_state);
                    continue;
                }
                pq->push(successor_state);
                return;
            }
        }
    }
    if (pq->empty()) { // if there was nothing else on the board then push back a random
        state *random_successor = new state(this->board, this->d);
        while (!random_successor->makeMove((char) ('A'+(rand() % this->d)), rand() % this->d, typeChar));
        random_successor->setValue(isMax? 0.25 : -0.25);
        pq->push(random_successor);
    }
};

// TODO XXX!!!!!
priority_queue<state*, vector<state*>, greater_equal<state*> >& state::getOrderedSuccessorsMax(vector<regex> orderOfSuccession) {
    this->successorsMax = new priority_queue<state*, vector<state*>, greater_equal<state*> >();
    this->operateOrderOfSuccession(orderOfSuccession, this->successorsMax, true, 'X');

    return *(this->successorsMax);
}

action state::getActionTakenToGetHere() {
    return this->takenToGetHere;
}


// friends
ostream& operator<<(ostream& stream, const state& s) {
    // pretty print?
    int dim = s.getDimension();
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

double state::getValue() const {
    return this->value;
}

void state::setValue(double value) {
    this->value = value;
}



#endif // STATE_H