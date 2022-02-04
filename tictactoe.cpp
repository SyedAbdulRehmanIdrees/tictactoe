#include <iostream>
#include <fstream>
using namespace std;

const int gridSize = 3;
char board[gridSize][gridSize];
char marks[2] = {'O', 'X'};
string saveFile = "game_save.conf";

void printBoard();
bool putMark(char, string);
bool isWinner(char);
int getWinner();
bool checkRows(char);
bool checkOneRow(char, int);
bool checkColumns(char);
bool checkOneColumn(char, int);
bool checkDiagonals(char);
bool checkLeftDiagonal(char);
bool checkRightDiagonal(char);
void save();
int load();
bool doesExists(string);

int main(){
    string location;
    int nMarks = sizeof(marks)/sizeof(marks[0]);
    int x = 0, ret;
    bool isTie = true;

    for(int x = 0; x < gridSize; x++){
        for(int y = 0; y < gridSize; y++){
            board[x][y] = ' ';
        }
    }

    if(doesExists(saveFile)){
        cout << "Found game config" << endl;
        cout << "Do you want to load config? [y/n]: " << endl;
        cin >> location;

        if(location == "y" || location == "Y"){
            x = load();
        }
    }

    while(x < gridSize * gridSize){
        printBoard();

        cout << "Enter location of " << marks[x % nMarks] << " mark (i.e. A1): ";
        cin >> location;
        
        if(!putMark(marks[x % nMarks], location)){
            cout << "Try again" << endl;
            continue;
        }
        
        ret = getWinner();

        if(ret != -1){
            cout << "Player with " << marks[ret] << " mark won" << endl;
            isTie = false;
            break;
        }

        save();
        x++;
    }

    remove(saveFile.c_str());

    if(isTie){
        cout << "No one won. It's a tie." << endl;
    }
}

void printBoard(){
    cout << "   ";
    for(int i = 0; i < gridSize; i++){
        cout << "  " << char('A' + i) << " ";
    }
    cout << endl;

    cout << "   ┌";
    for(int i = 0; i < gridSize - 1; i++){
        cout << "───┬";
    }

    cout << "───┐" << endl;
    for(int i = 0; i < gridSize; i++){
        cout << i + 1 << ": │";
        for(int j = 0; j < gridSize; j++){
            cout << " " << board[i][j] << " │";
        }
        cout << endl;


        if(i != gridSize - 1){
            cout << "   ├";
            for(int j = 0; j < gridSize - 1; j++){
                cout << "───┼";
            }
            cout << "───┤" << endl;
        }
    }

    cout << "   └";
    for(int i = 0; i < gridSize - 1; i++){
        cout << "───┴";
    }

    cout << "───┘" << endl;
}

bool putMark(char c, string coord){
    int y = coord[0] - 'A';
    int x = coord[1] - '1';

    if(x < 0 || x >= gridSize || y < 0 || y >= gridSize || board[x][y] != ' '){
        return false;
    }

    board[x][y] = c;
    return true;
}

bool isWinner(char c){
    return checkRows(c) || checkColumns(c) || checkDiagonals(c);
}

bool checkRows(char c){
    for(int i = 0; i < gridSize; i++){
        if(checkOneRow(c, i)){
            return true;
        }
    }

    return false;
}

bool checkColumns(char c){
    for(int i = 0; i < gridSize; i++){
        if(checkOneColumn(c, i)){
            return true;
        }
    }

    return false;
}

bool checkDiagonals(char c){
    return checkLeftDiagonal(c) || checkRightDiagonal(c);
}

bool checkOneRow(char c, int ridx){
    for(int i = 0; i < gridSize; i++){
        if(board[i][ridx] != c){
            return false;
        }
    }

    return true;
}

bool checkOneColumn(char c, int cidx){
    for(int i = 0; i < gridSize; i++){
        if(board[cidx][i] != c){
            return false;
        }
    }

    return true;
}

bool checkLeftDiagonal(char c){
    for(int i = 0; i < gridSize; i++){
        if(board[i][i] != c){
            return false;
        }
    }

    return true;
}

bool checkRightDiagonal(char c){
    for(int i = 0; i < gridSize; i++){
        if(board[gridSize - i - 1][i] != c){
            return false;
        }
    }

    return true;
}

void save(){
    fstream file;

    file.open(saveFile, ios::out);

    for(int x = 0; x < gridSize; x++){
        for(int y = 0; y < gridSize; y++){
            file << board[x][y];
        }
        file << endl;
    }

    file.close();
}

int load(){
    fstream file;
    string line;
    char c;
    int count = 0;

    file.open(saveFile, ios::in);

    for(int x = 0; x < gridSize; x++){
        getline(file, line);

        for(int y = 0; y < gridSize; y++){
            c = line[y];
            board[x][y] = c;

            if(c != ' '){
                count++;
            }
        }
    }

    file.close();

    return count;
}

bool doesExists(string filename){
    fstream file;

    file.open(filename, ios::in);
    bool ret = file.good();
    file.close();

    return ret;
}

int getWinner(){
    for(int i = 0; i < sizeof(marks)/sizeof(marks[0]); i++){
        if(isWinner(marks[i])){
            return i;
        }
    }

    return -1;
}
