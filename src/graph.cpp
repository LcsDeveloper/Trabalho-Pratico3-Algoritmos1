#include "../include/graph.hpp"

Graph::Graph(vvc& board, int n, int m){
    _board = board; _n = n; _m = m;

    //Encontra a posição da rainha e dos peões
    Position queen;
    vector<Position> pawns;
    
    for(int8_t i = 0; i < _n; i++){
        for(int8_t j = 0; j < _m; j++){
            if(_board[i][j] == 'R') {
                queen = {i, j};
            } else if(_board[i][j] == 'P'){
                pawns.push_back({i, j});
            }
        }
    }

    //Cria lista de todos os pontos importantes (rainha + peões)
    vector<Position> points;
    points.push_back(queen);
    for(auto it : pawns) points.push_back(it);

    int total_points = (int) points.size();

    //Inicializa matriz de custos
    _cost.resize(total_points, vector<int>(total_points, 0));

    //Preenche a matriz de custos
    for(int i = 0; i < total_points; i++){
        for(int j = i + 1; j < total_points; j++){            
            //Calcula o custo (mudanças de direção)
            int cost = _dijkstra(points[i], points[j]);
            
            //Armazena o custo simetricamente
            _cost[i][j] = cost;
            _cost[j][i] = cost;
        }
    }
}

typedef struct Node{
    int16_t cost;
    Position posi;
    int8_t dir_i, dir_j; //Direção previa
}NodeData;

auto node_compare = [](const NodeData& a, const NodeData& b){
    return a.cost < b.cost;
};

int Graph::_dijkstra(Position v1, Position v2){
    //Direções possíveis da rainha (8 movimentos)
    const int8_t directions[8][2] = {{-1, -1},{-1, 0},{-1, 1},{0, -1},{0, 1},{1, -1},{1, 0},{1, 1}};
    
    vector<vector<int16_t>> dist(_n, vector<int16_t>(_m, __INT8_MAX__));
    multiset<NodeData, decltype(node_compare)> q(node_compare);
    q.insert(NodeData{0, v1, 0, 0}); //Custo: 0; Posição: v1; Direção (0,0);

    NodeData current;
    while(!q.empty()){
        current = *q.begin();
        q.erase(q.begin());

        //Verifica se chegou ao destino
        if(current.posi.i == v2.i && current.posi.j == v2.j) return current.cost;

        if(current.cost > dist[current.posi.i][current.posi.j]) continue;

        //Explora todas as direções possíveis
        for(auto dir : directions){
            int8_t ni = (int8_t)(current.posi.i + dir[0]);
            int8_t nj = (int8_t)(current.posi.j + dir[1]);

            //Verifica se a nova posição é válida
            if(ni >= 0 && ni < _n && nj >= 0 && nj < _m && _board[ni][nj] != NVALID){
                const bool same_dir = (dir[0] == current.dir_i && dir[1] == current.dir_j);
                const int16_t new_cost = (int16_t)(current.cost + (same_dir ? 0 : 1));
                
                //Atualização da distância
                if(new_cost <= dist[ni][nj]){
                    dist[ni][nj] = new_cost;
                    q.insert(NodeData{new_cost, {ni, nj}, dir[0], dir[1]});
                }
            }
        }
    }

    return -INT_MAX;
}


int Graph::_totalCost(int mask, int curr, int num_pawns, vvs& memo){
    //Todos os peões capturados
    if(mask == (1 << num_pawns) - 1) return 0;

    if(memo[curr][mask] != -1) return memo[curr][mask];

    //Tenta capturar todos os peões
    int ans = INT_MAX;
    for(int i = 0; i < num_pawns; i++){
        if(!(mask & (1 << i))){
            int next_pos = i + 1;
            if(_cost[curr][next_pos] != INT_MAX){
                int new_mask = mask | (1 << i);
                int candidate = _cost[curr][next_pos] + _totalCost(new_mask, next_pos, num_pawns, memo);
                if(candidate < ans){
                    ans = candidate;
                }
            }
        }
    }
    
    memo[curr][mask] = (short) ans;
    return ans;
}

int Graph::tsp(){
    int num_pawns = (int) _cost.size() - 1;
    
    vvs memo(_cost.size(), vector<short>(1 << num_pawns, -1));
    
    int result = _totalCost(0, 0, num_pawns, memo);
    return (result == INT_MAX || result < 0) ? -1 : result;
}

int Graph::approxTSP_NNH(){
    short n = (short) _cost.size();
    vector<bool> visited(n, false);

    short totalCost = 0;
    short current = 0; //Começa na rainha
    visited[current] = true;
    short visitedCount = 1;

    while(visitedCount < n){
        short minCost = SHRT_MAX;
        short nearest = current;

        //Encontra o vizinho mais próximo não visitado
        for(short next = 0; next < n; ++next){
            if(_cost[current][next] < 0) continue;
            if(!visited[next] && _cost[current][next] < minCost){
                minCost = (short) _cost[current][next];
                nearest = next;
            }
        }

        //Se não encontrar caminho válido
        if (minCost == SHRT_MAX) return -1;

        totalCost += minCost;
        current = nearest;
        visited[current] = true;
        visitedCount++;
    }

    return totalCost;
}
