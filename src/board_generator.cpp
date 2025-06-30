#include "../include/board_generator.hpp"

vector<vector<char>> generate_board(int k) {
    const int SIZE = 100;
    vector<vector<char>> board(SIZE, vector<char>(SIZE, '.'));
    
    srand((unsigned int) time(nullptr));
    
    //Coloca obstáculos
    for(int i = 0; i < SIZE; ++i){
        for(int j = 0; j < SIZE; ++j){
            if(rand() % 100 < 20){ //20% de chance de ser obstáculo
                board[i][j] = '-';
            }
        }
    }
    
    //Posiciona a rainha em um lugar aleatório válido
    int qi, qj;
    do{
        qi = rand() % SIZE;
        qj = rand() % SIZE;
    }while(board[qi][qj] != '.');
    board[qi][qj] = 'R';
    
    //Posiciona k peões em lugares aleatórios válidos
    int pawns_placed = 0;
    while(pawns_placed < k){
        int pi = rand() % SIZE;
        int pj = rand() % SIZE;
        
        if (board[pi][pj] == '.') {
            board[pi][pj] = 'P';
            pawns_placed++;
        }
    }
    
    return board;
}