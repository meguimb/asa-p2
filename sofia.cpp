#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>
#include <array>

#define WHITE 0
#define BLUE  1
#define RED   2
#define BLACK 3

using namespace std;
using adjMatrix = vector<array<int, 3>>;

int adicionarMatrizAdj(adjMatrix &adj, int pessoa, int pai);
int dfs(int nOfVertices, adjMatrix const &adj);
int dfs_visit(int pessoa, adjMatrix const &adj, vector<bool> &marked, vector<bool> &onStack);
void colour(adjMatrix const &adj, int colour1, int colour2, int pessoa, vector<int> &colours);
void count(adjMatrix const &adj, int pessoa, vector<int> &colours, vector<int> &counts, int nOfVertices);

int main(){
    int v1, v2, nOfVertices, nOfEdges, temp1, temp2, temp;
    // receber input de v1 e v2
    cin >> v1 >> v2;

    // receber numero de vertices e numero de arcos
    cin >> nOfVertices >> nOfEdges;

    // criar matriz
    vector<array<int, 3>> adjTransposed(nOfVertices, {0,0,0});

    // receber arcos
    for (int i = 0; i < nOfEdges; i++) {
        if(!(cin >> temp1 >> temp2)) {
            cout << "0\n";
            return 0;
        }
        // verificar condições 
        if (temp1 == temp2 || temp1 < 1 || temp2 < 1 || temp1 > nOfVertices || temp2 > nOfVertices) {
            cout << "0\n";
            return 0;
        }
        // adicionar arco às listas de adjacencia
        if (adicionarMatrizAdj(adjTransposed, temp2, temp1) == -1) {
            cout << "0\n";
            return 0;
        }
    }
    
    if(cin >> temp) {
        cout << "0\n";
        return 0;
    }
    // verificar se há ciclo de parentes
    if (dfs(nOfVertices, adjTransposed) == -1) {
        cout << "0\n";
        return 0;
    }

    // pesquisar ancestrais comuns mais próximos
    vector<int> colours(nOfVertices, WHITE);
    vector<int> counts(nOfVertices, 0);

    colour(adjTransposed, WHITE, BLUE, v1, colours);
    colour(adjTransposed, BLUE, RED, v2, colours);

    count(adjTransposed, v1, colours, counts, nOfVertices);

    // output
    bool found = false;
    for(int i = 0; i < nOfVertices; i++) {
        if(colours[i] == RED && counts[i] == 0) { 
            cout << i+1 << " ";
            found = true;
        }
    }
    if(!(found))
        cout << "-";
    cout << "\n";
    
    return 0;
}

int dfs(int nOfVertices, adjMatrix const &adj) {
    vector<bool> marked(nOfVertices, false);
    vector<bool> onStack(nOfVertices, false);

    for (int i = 1; i <= nOfVertices; i++){
        if ((dfs_visit(i, adj, marked, onStack)) == -1) {
            return -1;
        }
    }
    return 0;
}

int dfs_visit(int pessoa, adjMatrix const &adj, vector<bool> &marked, vector<bool> &onStack) {
    marked[pessoa-1] = true;
    onStack[pessoa-1] = true;

    for(int i = 0; i < adj[pessoa-1][2]; i++) {
        int pai = adj[pessoa-1][i];

        if(!(marked[pai-1])) {
            if(dfs_visit(pai, adj, marked, onStack) == -1)
                return -1;
        }
        else if(onStack[pai-1])
            return -1;
    }

    onStack[pessoa-1] = false;

    return 0;
}

void colour(adjMatrix const &adj, int colour1, int colour2, int pessoa, vector<int> &colours) {
    queue<int> Q;
    Q.push(pessoa);

    while(!(Q.empty())) {
        int current_id = Q.front();
        Q.pop();
        
        if(colours[current_id-1] == colour1) {
            colours[current_id-1] = colour2;
        }
        else if(colour1 != WHITE && colours[current_id-1] == WHITE) {
            colours[current_id-1] = BLACK;
        }

        for(int i = 0; i < adj[current_id-1][2]; i++) {
            int pai = adj[current_id-1][i];

            if(colours[pai-1] == colour1) {
                colours[pai-1] = colour2;
                Q.push(pai);
            }
            else if(colour1 != WHITE && colours[pai-1] == WHITE) {
                colours[pai-1] = BLACK;
                Q.push(pai);
            }
        }
    }
}

void count(adjMatrix const &adj, int pessoa, vector<int> &colours, vector<int> &counts, int nOfVertices) {

    for(int i = 0; i < nOfVertices; i++) {
        if(colours[i] == RED) {
            for(int j = 0; j < adj[i][2]; j++) {
                counts[adj[i][j]-1]++;
            }
        }
    }
}

int adicionarMatrizAdj(adjMatrix &adj, int pessoa, int pai) {
    int num_pais = adj[pessoa-1][2];

    if(num_pais < 2) {
        adj.at(pessoa-1)[num_pais] = pai;
        adj.at(pessoa-1)[2] = num_pais + 1;
    }
    else
        return -1;

    return 0;     
}
