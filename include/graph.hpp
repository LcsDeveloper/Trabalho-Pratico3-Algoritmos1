#pragma once

#include <iostream>
#include <vector>
#include <climits>
#include <set>

#define VALID '.'
#define PAWNS 'P'
#define NVALID '-'
#define QUEEN 'R'
#define vvc vector<vector<char>>
#define vvi vector<vector<int>>
#define vvs vector<vector<short>>

using namespace std;

typedef struct{
    int8_t i, j;
}Position;

class Graph{
    public:
        Graph(vvc& board, int n, int m); //Inicia os campos privados criando o grafo completo e ponderado pelo custo de movimento
        int tsp(); //Resolve o caxeiro viajante utilizando uma dp com memoization. Tempo: O(v*v*2^v)
        int approxTSP_NNH(); //Aproxima o caxeiro viajante utilizando NNH

    private:
        int _dijkstra(Position v1, Position v2); //Encontra o caminho que minimiza as mudanças de movimento
        int _totalCost(int mask, int curr, int num_pawns, vvs& memo);
        int _n, _m;
        vvc _board;
        vvi _cost;
};
