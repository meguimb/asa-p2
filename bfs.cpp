#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>

#define WHITE 0
#define BLUE  1
#define RED   2
#define BLACK 3

using namespace std;

struct pessoa{
    int pessoa_id;
    struct pessoa *prox;
    struct pessoa *last;
};
typedef struct pessoa Pessoa;

Pessoa *obterAdjacencias(Pessoa **adj_list, int pessoa_id);
int adicionarListaAdj(Pessoa **adj_list, int pessoa, int pessoa_adjacente, bool verificar_num_max_pais);
Pessoa *criar_pessoa(int pessoa_id);
int dfs(int nOfVertices, Pessoa **adjList);
int dfs_visit(Pessoa *p, Pessoa **adjList, vector<bool> &marked, vector<bool> &onStack);
void colour(Pessoa **adjListTransposed, int colour1, int colour2, int id, vector<int> &colours);
void count(Pessoa **adjListTransposed, int id, vector<int> &colours, vector<int> &counts, int nOfVertices);

Pessoa *criar_pessoa(int pessoa_id){
    Pessoa *p = new Pessoa;
    p->pessoa_id = pessoa_id;
    p->prox = NULL;
    p->last = NULL;
    return p;
}

int main(){
    int v1, v2, nOfVertices, nOfEdges, temp1, temp2, temp;
    // receber input de v1 e v2
    cin >> v1 >> v2;

    // receber numero de vertices e numero de arcos
    cin >> nOfVertices >> nOfEdges;

    // criar matriz de adjacencia e matriz de adjacencia transposta
    Pessoa **adjListTransposed = new Pessoa*[nOfVertices]; // cada adjListTransposed[i] tem as Pessoas a que é adjacente (onde é apontado)
    // int color [nOfVertices]; // 0 - unvisited, 1 - being visited, 2 - has been visited
    for (int i = 1; i <= nOfVertices; i++){
        // color[i] = 0;
        adjListTransposed[i-1] = criar_pessoa(i);
    }
    // receber arcos
    for (int i = 0; i < nOfEdges; i++){
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
        if (adicionarListaAdj(adjListTransposed, temp2, temp1, true)==-1)
            return -1;
    }
    
    if(cin >> temp) {
        cout << "0\n";
        return 0;
    }
    
    // verificar se qualquer filho tem mais de 2 pais
    // TODO - mudar para utilizar uma array e ir aumentando quando se adiciona arestas
    for (int i = 1; i <= nOfVertices; i++){ 
        Pessoa *p = obterAdjacencias(adjListTransposed, i);
        int counter = 0;
        while (p->prox != NULL){
            counter++;
            p = p->prox;
        }
        if (counter > 2){
            cout << "0\n";
            return 0;
        }
    }
    // verificar se há ciclo de parentes
    if (dfs(nOfVertices, adjListTransposed) == -1) {
        cout << "0\n";
        return 0;
    }

    // pesquisar ancestrais comuns mais próximos
    vector<int> colours(nOfVertices, WHITE);
    vector<int> counts(nOfVertices, 0);

    colour(adjListTransposed, WHITE, BLUE, v1, colours);
    colour(adjListTransposed, BLUE, RED, v2, colours);

    count(adjListTransposed, v1, colours, counts, nOfVertices);

    /*for(int i = 0; i < nOfVertices; i++) {
        cout << "pessoa " << i+1 << ": ";
        if(colours[i] == WHITE)
            cout << "WHITE\n";
        if(colours[i] == BLUE)
            cout << "BLUE\n";
        if(colours[i] == RED)
            cout << "RED\n";
    }

    for(int i = 0; i < nOfVertices; i++)
        cout << "pessoa " << i+1 << ": " << counts[i] << "\n";*/

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

int dfs(int nOfVertices, Pessoa **adjList){
    vector<bool> marked(nOfVertices, false);
    vector<bool> onStack(nOfVertices, false);

    for (int i = 0; i < nOfVertices; i++){
        Pessoa *p = adjList[i];
        if ((dfs_visit(p, adjList, marked, onStack)) == -1) {
            return -1;
        }
    }
    return 0;
}

int dfs_visit(Pessoa *p, Pessoa **adjList, vector<bool> &marked, vector<bool> &onStack){
    int id = p->pessoa_id;

    marked[id-1] = true;
    onStack[id-1] = true;

    while(p->prox != NULL){
        p = p->prox;
        int idAdj = p->pessoa_id;

        if(!(marked[idAdj-1])) {
            if(dfs_visit(obterAdjacencias(adjList, idAdj), adjList, marked, onStack) == -1)
                return -1;
        }
        else if(onStack[idAdj-1])
            return -1;
    }

    onStack[id-1] = false;

    return 0;
}

void colour(Pessoa **adjListTransposed, int colour1, int colour2, int id, vector<int> &colours) {
    queue<int> Q;
    Q.push(id);

    while(!(Q.empty())) {
        int current_id = Q.front();
        Q.pop();

        Pessoa *current = obterAdjacencias(adjListTransposed, current_id);

        while(current != NULL) {
            int prox_id = current->pessoa_id;

            if(colours[prox_id-1] == colour1) {
                colours[prox_id-1] = colour2;
                Q.push(prox_id);
            }
            else if(colour1 != WHITE && colours[prox_id-1] == WHITE) {
                colours[prox_id-1] = BLACK;
                Q.push(prox_id);
            }
            current = current->prox;                
        }
    }
}

void count(Pessoa **adjListTransposed, int id, vector<int> &colours, vector<int> &counts, int nOfVertices) {
    for(int i = 1; i <= nOfVertices; i++) {
        Pessoa *p = obterAdjacencias(adjListTransposed, i);
        int id = p->pessoa_id;
        //cout << "id: " << id << "\n";

        if(colours[id-1] == RED) {
            //cout << "is red\n";
            while(p->prox != NULL) {
                p = p->prox;
                int id_adj = p->pessoa_id;
                //cout << "id adj: " << id_adj << "\n";

                counts[id_adj-1]++;
            }
        }
    }
}

Pessoa *obterAdjacencias(Pessoa **adj_list, int pessoa_id){
    return adj_list[pessoa_id-1];
}

// TODO - mudar para utilizar a propriedade last
int adicionarListaAdj(Pessoa **adj_list, int pessoa, int pessoa_adjacente, bool verificar_num_max_pais){
    int tamanho = 0;
    Pessoa *adjs = obterAdjacencias(adj_list, pessoa);
    while (adjs->prox != NULL){
        // arco / relação inválida
        if (adjs->pessoa_id == pessoa_adjacente){
            return -1;
        }
        adjs = adjs->prox;
        tamanho++;
    }
    if (verificar_num_max_pais && tamanho >= 2){
        return -1;
    }
    adjs->prox = criar_pessoa(pessoa_adjacente);
    return 0;
}

int adicionarListaAdj(Pessoa **adj_list, int pessoa, int pessoa_adjacente){ 
    Pessoa *adjs = obterAdjacencias(adj_list, pessoa);
    if (adjs->last == NULL){
        adjs->last = criar_pessoa(pessoa_adjacente);
        adjs->prox = adjs->last;
    }
    else{
        adjs->last->prox = criar_pessoa(pessoa_adjacente);
        adjs->last = adjs->last->prox;
    }
    return 0;
}