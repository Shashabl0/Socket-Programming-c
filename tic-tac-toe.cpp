#include<iostream>

using namespace std;

char board[9] = {' ',' ',' ',' ',' ',' ',' ',' ',' '};

void insertLetter(char letter,int pos){
    board[pos] = letter;
    cout<<"updating position "<<board[pos]<<endl;
}

bool isfreespace(int pos){
    return board[pos] == ' '?1:0;
    
}

void print(char board[]){
    cout<<"   |   |   "<<endl;
    cout<<" "<<board[0]<<" | "<<board[1]<<" | "<<board[2]<<""<<endl;
    cout<<"   |   |  "<<endl;
    cout<<"----------"<<endl;
    cout<<"   |   |   "<<endl;
    cout<<" "<<board[3]<<" | "<<board[4]<<" | "<<board[5]<<""<<endl;
    cout<<"   |   |   "<<endl;
    cout<<"----------"<<endl;
    cout<<"   |   |   "<<endl;
    cout<<" "<<board[6]<<" | "<<board[7]<<" | "<<board[8]<<""<<endl;
    cout<<"   |   |   "<<endl;
}
bool isWinner(int le){
    bool w =    ((board[0] == le && board[1] == le && board[2] == le) ||
                (board[3] == le && board[4] == le && board[5] == le) ||
                (board[6] == le && board[7] == le && board[8] == le) ||
                (board[0] == le && board[3] == le && board[6] == le) ||
                (board[1] == le && board[4] == le && board[7] == le) ||
                (board[2] == le && board[5] == le && board[8] == le) ||
                (board[0] == le && board[4] == le && board[8] == le) ||
                (board[2] == le && board[4] == le && board[6] == le));
    return w;
}
bool isfull(char board[]){
    bool empty =  ((board[0] == ' ' || board[1] == ' ' || board[2] == ' ' ) ||
                (board[3] == ' ' || board[4] == ' ' || board[5] == ' ' ) ||
                (board[6] == ' ' || board[7] == ' ' || board[8] == ' ' ));
    return !empty;
}

void playermove(char letter){
    bool run = true;
    while(run){
        int m;
        cout<<letter<<"'s move"<<endl;
        cout<<"Enter the position to place :(0-8) ";
        cin>>m;
        if(m > 0 && m <= 9){
            if(isfreespace(m-1)){
                run = false;
                insertLetter(letter,m-1);
            }
            else    cout<<" This place is already Occupied!"<<endl;
        }
        else    cout<<"Enter a valid position!"<<endl;
    }
}
int main(){

    cout<<"Commandline Tic-tac-toe !"<<endl;

    print(board);
    while(!isfull(board)){
        if(!isWinner('X')){
            playermove('O');
            print(board);
        }
        else{
            cout<<"X is winner !!"<<endl;
            break;
        }
        if(!isWinner('O')){
            playermove('X');
            print(board);
        }
        else{
            cout<<"O is winner !!"<<endl;
            break;
        }
    }
    if(isfull(board))
        cout<<"Tie Game !! "<<endl;
    return 0;
}