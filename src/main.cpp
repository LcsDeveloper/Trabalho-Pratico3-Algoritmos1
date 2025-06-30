#include <iostream>
#include <chrono>
#include "../include/graph.hpp"
#include "../include/board_generator.hpp"

//Funções para obtenção de dados avaliativos (Evolução do tempo de execução e qualidade da solução)
void AvaliarTempo(Graph& graph){
    auto start{chrono::steady_clock::now()};
    graph.tsp();
    auto finish{chrono::steady_clock::now()};
    chrono::duration<double> elapsed_seconds{finish - start};
    cout << elapsed_seconds.count() << ";";

    start = {chrono::steady_clock::now()};
    graph.approxTSP_NNH();
    finish = {chrono::steady_clock::now()};
    elapsed_seconds = {finish - start};
    cout << elapsed_seconds.count() << ";\n";
}

void AvaliarQualidadeDaResposta(Graph& graph){
    cout << graph.tsp() << ";" << graph.approxTSP_NNH() << endl;
}

void EvolucaoDoTempo(int k_max){
    for(int i = 1; i <= k_max; i++){
        cout << i << ";";
        vvc board = generate_board(i);
        Graph graph(board, 100, 100);
        AvaliarTempo(graph);
    }
}

void AnaliseDaQualidade(int k_max){
    for(int i = 1; i <= k_max; i++){
        cout << i << ";";
        vvc board = generate_board(i);
        Graph graph(board, 100, 100);
        AvaliarQualidadeDaResposta(graph);
    }
}

int main(){
    int n, m, k;
    cin >> n >> m >> k; //Recebe os valores de entrada

    vvc board(n, vector<char>(m));

    //Recebe o tabuleiro
    for(int i = 0; i < n; i++){
        for(int j = 0; j < m; j++){
            cin >> board[i][j];
        }
    }
    
    Graph graph(board, n, m); //Cria o grafo
    cout << graph.tsp() << endl; //Resposta exata

    return 0;
}