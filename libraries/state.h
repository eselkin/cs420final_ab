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
#include <cmath>

using namespace std;
typedef pair<char, int> action;


class state {
public:
    class greater_comp {
    public:
        bool operator() (state* a, state* b) { return a->getValue() >= b->getValue();}
    };

    class less_comp {
    public:
        bool operator() (state* a, state* b) { return a->getValue() <= b->getValue();}
    };
private:
    int d;                                      // board dimension
    char **board;                               // X, O, - (ASCII: 88, 79, 45)
    double value;                               // why calculate these things twice?
    action takenToGetHere;                      // No need to keep track of parent just the move so the parent knows
    state* parent;
    priority_queue<state*, vector<state*>, state::greater_comp >* successorsMax;
    priority_queue<state*, vector<state*>, state::less_comp >* successorsMin;
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
    priority_queue<state*, vector<state*>, greater_comp>& getOrderedSuccessorsMax(vector<regex> orderOfSuccession);
    priority_queue<state*, vector<state*>, less_comp>& getOrderedSuccessorsMin(vector<regex> orderOfSuccession);

    friend std::ostream& operator<<(std::ostream&, const state&);
    string getStringFromRow(char *row);

    template <typename T>
    void operateOrderOfSuccession(vector<regex> orderOfSuccession, priority_queue<state *, vector<state *>, T>*& pq, bool isMax, char typeChar);
    inline bool isEqual(const double &a, const double &b){
        return fabs(a-b) < 0.001;
    }

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
    if (this->board[r-'A'][c-1] == '-') {
        this->takenToGetHere = action(r, c);
        this->board[r-'A'][c-1] = typeChar;
        return true;
    }
    return false;
    //return this->board[r-'A'][c-1] == '-' ? (this->board[r-'A'][c-1] = typeChar) : false;
}

string state::getStringFromRow(char* row){
    return string(row, this->d);
}


priority_queue<state *, vector<state *>, state::less_comp > &
state::getOrderedSuccessorsMin(vector<regex> orderOfSuccession) {
    this->successorsMin = new priority_queue<state*, vector<state*>, state::less_comp >();
    swap(orderOfSuccession[0], orderOfSuccession[2]);
    swap(orderOfSuccession[1], orderOfSuccession[3]);
    swap(orderOfSuccession[4], orderOfSuccession[5]);
    swap(orderOfSuccession[6], orderOfSuccession[7]);
    swap(orderOfSuccession[8], orderOfSuccession[9]);
    this->operateOrderOfSuccession(orderOfSuccession, this->successorsMin, false, 'O');
    return *(this->successorsMin);
}

char** transpose(char **board, int d) {
    char **transpose_board = new char *[d];
    for (int i = 0; i < d; i++) {
        transpose_board[i] = new char[d];
        for (int j = 0; j < d; j++) {
            transpose_board[i][j] = board[j][i];
        }
    }
    return transpose_board;
};

template <typename T>
void state::operateOrderOfSuccession(vector<regex> orderOfSuccession, priority_queue<state*, vector<state*>, T>*& pq, bool isMax, char typeChar) {
    char **transpose_board = transpose(this->board, this->d);
    state ***board_of_states = new state**[d]; // two dimensional array of state pointers! Crazy, I know
    for (int i = 0; i < d; i++) {
        board_of_states[i] = new state *[d]; // should be NULLs
        for (int j = 0; j < d; j++)
            board_of_states[i][j] = NULL;
    }
    smatch matcher;
    smatch matcher2;
    for (regex expression : orderOfSuccession) {
        for (int i = 0; i < d; i++) {
            string transposeRow = getStringFromRow(transpose_board[i]);
            string row = getStringFromRow(this->board[i]);
            string rowTemp(row);
            if (regex_search(rowTemp, matcher, expression)) {
                size_t pos;
                vector<size_t> dashes;
                dashes.push_back(matcher[0].str().find("-"));
                pos = size_t(matcher.prefix().length());
                state *successor_state = new state(this->board, this->d);

                if (matcher[0].str().length() == 5) {
                    size_t dash2 = matcher[0].str().find("-", dashes[0]);
                    size_t dash3 = matcher[0].str().find("-", dash2);
                    if (dash2 != string::npos) {
                        dashes.push_back(dash2);
                        if (dash3 != string::npos)
                            dashes.push_back(dash3);
                    }
                    pos += dashes[rand()%size_t(dashes.size())];
                } else {
                    pos += dashes[0];
                    if (matcher[0].str().length() == 4) {
                        if (matcher[0].str().find("-", size_t(pos)) != string::npos) {
                            successor_state->setValue(isMax ? .8 : -.8);
                        } else {
                            successor_state->setValue(isMax ? 1.1 : -1.1);
                        }
                    } else if (matcher[0].str().length() == 3) {
                        successor_state->setValue(isMax ? .75 : -.75);
                    } else if (matcher[0].str().length() == 2) {
                        successor_state->setValue(isMax ? .4 : -.4);
                    }
                }
                successor_state->makeMove((char) (i + 'A'), pos+1, typeChar);
                cout << "ROW MATCH..." << char(i+'A') << ":" << pos+1 << " [" << matcher[0].str() << "]" << endl;
                cout << *successor_state << endl;
                if (board_of_states[i][pos] != NULL) {
                    board_of_states[i][pos]->setValue(board_of_states[i][pos]->getValue() + successor_state->getValue());
                } else {
                    board_of_states[i][pos] = successor_state;
                }
            }
            string rowTempTranspose(transposeRow);
            if (regex_search(rowTempTranspose, matcher2, expression)) {
                size_t pos;
                vector<size_t> dashes;
                dashes.push_back(matcher2[0].str().find("-"));
                pos = size_t(matcher2.prefix().length());
                // this means a column i has a killer move and row pos
                state *successor_state = new state(this->board, this->d);

                if (matcher2[0].str().length() == 5) {
                    // we have one of the larger picture situations
                    // pos may change
                    size_t dash2 = matcher[0].str().find("-", dashes[0]);
                    size_t dash3 = matcher[0].str().find("-", dash2);
                    if (dash2 != string::npos) {
                        dashes.push_back(dash2);
                        if (dash3 != string::npos)
                            dashes.push_back(dash3);
                    }
                    pos += dashes[rand()%size_t(dashes.size())];
                } else {
                    pos = pos + dashes[0];
                    if (matcher2[0].str().length() == 4) {
                        if (matcher2[0].str().find("-", size_t(pos)) != string::npos) {
                            successor_state->setValue(isMax ? .8 : -.8);
                        } else {
                            successor_state->setValue(isMax ? 1.1 : -1.1);
                        }
                    } else if (matcher2[0].str().length() == 3) {
                        successor_state->setValue(isMax ? .75 : -.75);
                    } else if (matcher2[0].str().length() == 2) {
                        successor_state->setValue(isMax ? .4 : -.4);
                    }
                }
                successor_state->makeMove((char) (pos + 'A'), i+1, typeChar);
                cout << "COL MATCH..." << char(pos + 'A') << ":" << i+1 << "[" << matcher2[0].str() << "]" << endl;
                cout << *successor_state << endl;
                if (board_of_states[pos][i] != NULL) {
                    board_of_states[pos][i]->setValue(board_of_states[pos][i]->getValue() + successor_state->getValue());
                } else {
                    board_of_states[pos][i] = successor_state;
                }
            }
        }
    }
    for (int i = 0; i < d; i++) {
        for (int j = 0; j < d; j++) {
            if (board_of_states[i][j] != NULL){
                pq->push(board_of_states[i][j]);
            }
        }
    }
    if (pq->empty()) { // if there was nothing else on the board then push back a random
        state *random_successor = new state(this->board, this->d);
        while (!random_successor->makeMove((char) ('A'+((rand() % (this->d-4))+3)), (rand() % (this->d-6))+3+1, typeChar));
        random_successor->setValue(isMax? 0.25 : -0.25);
        pq->push(random_successor);
    }
    return;
};

// TODO XXX!!!!!
priority_queue<state*, vector<state*>, state::greater_comp>& state::getOrderedSuccessorsMax(vector<regex> orderOfSuccession) {
    this->successorsMax = new priority_queue<state*, vector<state*>, state::greater_comp>();
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