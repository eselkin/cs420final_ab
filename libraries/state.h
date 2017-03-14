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
int chooseClosestToChar(string matched, vector<size_t> dash_positions);

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
    int charsRemaining;                         // Spaces left?
    priority_queue<state*, vector<state*>, state::greater_comp >* successorsMax;
    priority_queue<state*, vector<state*>, state::less_comp >* successorsMin;
    // Ordered list of successors TODO Not sure if we need this I think we should have to generate this every time we come to a state

public:

    state(int dimension = 8);                   // constructor
    state(char** fromBoard, int charsRemaining, int dimension = 8);
    state(const state &original);               // copy constructor
    ~state();                                   // destructor
    char** getBoard() const;                    // accessor for board
    int getDimension() const;                   // accessor for d
    double getValue() const;
    void setValue(double value);
    bool makeMove(char r, int c, char typeChar);// success|failure
    action getActionTakenToGetHere();
    bool isOver();
    priority_queue<state*, vector<state*>, greater_comp>& getOrderedSuccessorsMax(vector<regex> orderOfSuccession);
    priority_queue<state*, vector<state*>, less_comp>& getOrderedSuccessorsMin(vector<regex> orderOfSuccession);

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
    this->charsRemaining = d*d;
}

// convenience constructor to set up a board from a 2-dimensional array
state::state(char** fromBoard, int charsRemaining, int dimension) : state(dimension) {
    for (int i = 0; i < this->d; i++) {
        for (int j = 0; j < this->d; j++) {
            this->board[i][j] = fromBoard[i][j];
        }
    }
    this->charsRemaining = charsRemaining;
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
        this->charsRemaining--;
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
                state *successor_state = new state(this->board, this->charsRemaining, this->d);

                if (matcher[0].str().length() == 5) { // --X-- -XXX-
                    size_t dash2 = matcher[0].str().find("-", dashes[0]);
                    size_t dash3 = matcher[0].str().find("-", dash2);
                    size_t dash4 = matcher[0].str().find("-", dash3);
                    if (dash2 != string::npos) {
                        dashes.push_back(dash2);
                        if (dash3 != string::npos) {
                            dashes.push_back(dash3);
                            if (dash4 != string::npos) {
                                dashes.push_back(dash4);
                                successor_state->setValue(isMax ? .1 : -.1);
                            } else {
                                successor_state->setValue(isMax?1.5:-1.5);
                            }
                        } else {
                            successor_state->setValue(isMax?1.75:-1.75);
                        }
                    } else {
                        // shouldn't get here!
                        successor_state->setValue(isMax?2:-2.);
                    }
                    pos += dashes[chooseClosestToChar(matcher[0], dashes)];
                } else {
                    pos += dashes[0];
                    if (matcher[0].str().length() == 4) {
                        if (matcher[0].str().find("-", pos) != string::npos) {
                            successor_state->setValue(isMax ? 1.5 : -1.5    ); // this creates a bias toward avoiding loss
                        } else {
                            successor_state->setValue(isMax ? 1.75 : -2.5);
                        }
                    } else if (matcher[0].str().length() == 3) {
                        successor_state->setValue(isMax ? 1. : -2);
                    } else if (matcher[0].str().length() == 2) {
                        if (successor_state->charsRemaining >= ( d * d )- 2 ){
                            successor_state->setValue(isMax? 1.25 : -1.5);
                        } else {
                            successor_state->setValue(isMax ? .75 : -1.25);
                        }
                    }
                }
                successor_state->makeMove((char) (i + 'A'), pos+1, typeChar);
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
                state *successor_state = new state(this->board, this->charsRemaining, this->d);

                if (matcher2[0].str().length() == 5) {
                    // we have one of the larger picture situations
                    // pos may change
                    size_t dash2 = matcher2[0].str().find("-", dashes[0]);
                    size_t dash3 = matcher2[0].str().find("-", dash2);
                    size_t dash4 = matcher2[0].str().find("-", dash3);
                    if (dash2 != string::npos) {
                        dashes.push_back(dash2);
                        if (dash3 != string::npos) {
                            dashes.push_back(dash3);
                            if (dash4 != string::npos) {
                                dashes.push_back(dash4);
                                successor_state->setValue(isMax ? .1 : -.1);
                            } else {
                                successor_state->setValue(isMax?1.5:-1.5);
                            }
                        } else {
                            successor_state->setValue(isMax?1.75:-1.75);
                        }
                    } else {
                        // shouldn't get here!
                        successor_state->setValue(isMax?2:-2.);
                    }
                    pos += dashes[chooseClosestToChar(matcher2[0], dashes)];
                } else {
                    pos = pos + dashes[0];
                    if (matcher2[0].str().length() == 4) {
                        if (matcher2[0].str().find("-", size_t(pos)) != string::npos) {
                            successor_state->setValue(isMax ? 1.5 : -1.5); // avoid losses
                        } else {
                            successor_state->setValue(isMax ? 1.75 : -2.5); //
                        }
                    } else if (matcher2[0].str().length() == 3) {
                        successor_state->setValue(isMax ? 1. : -2);
                    } else if (matcher2[0].str().length() == 2) {
                        if (successor_state->charsRemaining == ( d * d )- 1 ){
                            successor_state->setValue(isMax? 1.2 : -1.5);
                        } else {
                            successor_state->setValue(isMax ? .75 : -1.25);
                        }
                    }
                }
                successor_state->makeMove((char) (pos + 'A'), i+1, typeChar);
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
    if (this->charsRemaining == 0) {
        return;
    }
    else if (pq->empty() && this->charsRemaining <= (this->d*this->d)/2) {
        vector<action> actionsLeft;
        // find all actions remaining
        for (int i = 0; i < d; i++) {
            for (int j = 0; j < d; j++){
                if (this->board[i][j] == '-') {
                    actionsLeft.push_back(action(i+'A',j+1));
                }
            }
        }
        state *random_successor = new state(this->board, this->charsRemaining, this->d);
        action randomAction = actionsLeft[rand()%actionsLeft.size()];
        random_successor->makeMove(randomAction.first, randomAction.second, typeChar);
        random_successor->setValue(isMax? 0.25 : -0.25);
        pq->push(random_successor);
    } else if (pq->empty()) { // if there was nothing else on the board then push back a random
        state *random_successor = new state(this->board, this->charsRemaining, this->d);
        while (!random_successor->makeMove((char) ('A'+((rand() % (this->d-6))+4)), (rand() % (this->d-6))+4, typeChar));
        random_successor->setValue(isMax? 0.25 : -0.25);
        pq->push(random_successor);
    }
    return;
};


int chooseClosestToChar(string matched, vector<size_t> dash_positions) {
    vector<size_t> distances;
    size_t last_dash_pos = 0;
    size_t largest_distance = 0;
    int index_largest_distance = 0;
    for (int i = 0; i < dash_positions.size(); i++) {
        if (dash_positions[i]-last_dash_pos > largest_distance) {
            index_largest_distance = i;
        }
    }
    // largest_distance and largest_distance-1
    if (index_largest_distance > 1) {
        if (rand() % 1 == 0) {
            return index_largest_distance - 1;
        } else {
            return index_largest_distance;
        }
    } else {
        if (dash_positions.size() == 2) {
            return 1;
        }
        return index_largest_distance;
    }
}

// TODO XXX!!!!!
priority_queue<state*, vector<state*>, state::greater_comp>& state::getOrderedSuccessorsMax(vector<regex> orderOfSuccession) {
    this->successorsMax = new priority_queue<state*, vector<state*>, state::greater_comp>();
    this->operateOrderOfSuccession(orderOfSuccession, this->successorsMax, true, 'X');

    return *(this->successorsMax);
}


priority_queue<state *, vector<state *>, state::less_comp > &
state::getOrderedSuccessorsMin(vector<regex> minOrderOfSuccession) { // we're passed in the minOrderOfSuccession
    this->successorsMin = new priority_queue<state*, vector<state*>, state::less_comp >();
    this->operateOrderOfSuccession(minOrderOfSuccession, this->successorsMin, false, 'O');
    return *(this->successorsMin);
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

bool state::isOver() {
    return false;
}


#endif // STATE_H