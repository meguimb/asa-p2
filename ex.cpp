// g++ -std=c++11 -O3 -Wall file.cpp -lm
#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>

#define WHITE 0
#define RED 1
#define BLACK 2

using namespace std;

struct pessoa{
    int pessoa_id;
    struct pessoa *prox;
    int cor;
    int descoberto;
    int fechado;
    int pai;
    int startingVertice;
    struct pessoa *last;
};
typedef struct pessoa Pessoa;


Pessoa *obterAdjacencias(Pessoa **adj_list, int pessoa_id);
int adicionarListaAdj(Pessoa **adj_list, int pessoa, int pessoa_adjacente);
Pessoa *criar_pessoa(int pessoa_id);
int dfs(int nOfVertices, Pessoa **adjList);
int dfs_visit(Pessoa *p, Pessoa **adjList, vector<bool> &marked, vector<bool> &onStack);
Pessoa *ancestraisEmComum(Pessoa **adjListTransposed, int v1, int v2);
int limparListasPessoas(Pessoa *lst);
int imprimirListaPessoas(Pessoa *lst);
bool ehAscendenteDeA(Pessoa **adjList, int ascendente, int pessoa_a);
Pessoa *obterAncestraisComunsMaisProxOrdemAlfabetica(Pessoa **adjList, Pessoa **adjListTransposed, int v1, int v2);
int limparListaAdjacencias(Pessoa **adjs_lst, int tamanho);
Pessoa *adicionarPessoaOrdemCrescente(Pessoa *lst, Pessoa *p);
Pessoa *newAlgo(Pessoa **adjList, Pessoa **adjListTransposed, int v1, int v2);
int DFS_Visit(int numVertices, Pessoa **adjListTransposed, int vertice, int startingIndex);
int DFS(int numVertices, Pessoa **adjListTransposed, int v1);
Pessoa *algo(int numOfVertices, Pessoa **adjListTransposed, int v1, int v2);


Pessoa *criar_pessoa(int pessoa_id){
    Pessoa *p = new Pessoa;
    p->pessoa_id = pessoa_id;
    p->prox = NULL;
    return p;
}

int tempo;
Pessoa *pais = criar_pessoa(0);
int main(){
    int v1, v2, nOfVertices, nOfEdges, temp1, temp2;
    // receber input de v1 e v2
    cin >> v1 >> v2;

    // receber numero de vertices e numero de arcos
    cin >> nOfVertices >> nOfEdges;

    // criar matriz de adjacencia e matriz de adjacencia transposta
    Pessoa **adjList = new Pessoa*[nOfVertices]; // cada adjList[i] tem as adjacencias da Pessoa i (onde aponta)
    Pessoa **adjListTransposed = new Pessoa*[nOfVertices]; // cada adjListTransposed[i] tem as Pessoas a que é adjacente (onde é apontado)
    
    // int color [nOfVertices]; // 0 - unvisited, 1 - being visited, 2 - has been visited
    for (int i = 1; i <= nOfVertices; i++){
        // color[i] = 0;
        adjList[i-1] = criar_pessoa(i);
        adjListTransposed[i-1] = criar_pessoa(i);
        adjListTransposed[i-1]->last = adjListTransposed[i-1];
        adjList[i-1]->last = adjList[i-1];
    }
    
    // receber arcos
    for (int i = 0; i < nOfEdges; i++){
        cin >> temp1 >> temp2;
        // verificar condições 
        if (temp1 == temp2 || temp1 < 1 || temp2 < 1 || temp1 > nOfVertices || temp2 > nOfVertices){
            printf("0\n");
            limparListaAdjacencias(adjList, nOfVertices);
            limparListaAdjacencias(adjListTransposed, nOfVertices);
            return 0;
        }

        // adicionar arco às listas de adjacencia
        adicionarListaAdj(adjList, temp1, temp2);
        adicionarListaAdj(adjListTransposed, temp2, temp1);
    }
    
    // verificar se qualquer filho tem no máximo 2 pais
    for (int i = 1; i <= nOfVertices; i++){ 
        Pessoa *p = obterAdjacencias(adjListTransposed, i);
        int counter = 0;
        while (p->prox != NULL){
            counter++;
            p = p->prox;
        }
        if (counter > 2){
            printf("0\n");
            limparListaAdjacencias(adjList, nOfVertices);
            limparListaAdjacencias(adjListTransposed, nOfVertices);
            return 0;
        }
    }

    // verificar se há ciclo de parentes
    if (dfs(nOfVertices, adjList) == -1) {
        cout << "0\n";
        return 0;
    }
    // cout << "no cicle\n";
    
    // pesquisar ancestrais comuns mais próximos

    // output 
    algo(nOfVertices, adjListTransposed, v1, v2);
    // pais -> 0 1 2 4
    if (pais == NULL){
        printf("-\n");
    }
    else{
        imprimirListaPessoas(pais);
        limparListasPessoas(pais);
    }
    limparListaAdjacencias(adjList, nOfVertices);
    limparListaAdjacencias(adjListTransposed, nOfVertices);
    // limpar listas de adjacencias
    return 0;
}

int dfs(int nOfVertices, Pessoa **adjList){
    vector<bool> marked(nOfVertices, false);
    vector<bool> onStack(nOfVertices, false);

    for (int i = 0; i < nOfVertices; i++){
        Pessoa *p = adjList[i];
        if (dfs_visit(p, adjList, marked, onStack) == -1)
            return -1;
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

        if(!(marked[idAdj-1]))
            dfs_visit(obterAdjacencias(adjList, idAdj), adjList, marked, onStack);

        else if(onStack[idAdj-1])
            return -1;
    }

    onStack[id-1] = false;

    return 0;
}

Pessoa *obterAdjacencias(Pessoa **adj_list, int pessoa_id){
    return adj_list[pessoa_id-1];
}

int adicionarListaAdj(Pessoa **adj_list, int pessoa, int pessoa_adjacente){ 
    Pessoa *adjs = obterAdjacencias(adj_list, pessoa);
    adjs->last->prox = criar_pessoa(pessoa_adjacente);
    adjs->last = adjs->last->prox;
    /*
    while (adjs->prox != NULL){
        // arco / relação inválida
        if (adjs->pessoa_id == pessoa_adjacente){
            // arco repetido
            return -1;
        }
        adjs = adjs->prox;
        tamanho++;
    }
    if (verificar_num_max_pais && tamanho >= 2){
        return -1;
    }
    adjs->prox = criar_pessoa(pessoa_adjacente);
    */
    return 0;
}


int limparListasPessoas(Pessoa *lst){
    Pessoa *temp;
    while (lst->prox != NULL){
        temp = lst;
        lst = lst->prox;
        delete temp;
    }
    delete lst;
    return 0;
}

int limparListaAdjacencias(Pessoa **adjs_lst, int tamanho){
    for (int i = 0; i < tamanho; i++){
        limparListasPessoas(adjs_lst[i]);
    }
    delete adjs_lst;
    return 0;
}

int imprimirListaPessoas(Pessoa *lst){
    while (lst != NULL){
        printf("%d ", lst->pessoa_id);
        lst = lst->prox;
    }
    printf("\n");
    return 0;
}

Pessoa *algo(int numOfVertices, Pessoa **adjListTransposed, int v1, int v2){
    // Suppose we want to find the LCA(u, v) in graph G. 
    // Initially, all the vertices are colored white.
    // First, we do a Depth-First Search (DFS) on one of the target nodes. 
    // Let it be node u. Also, we’ll keep track of the parent’s array (current path from a starting vertex). 
    // During the DFS, we color all the ancestors of u in red each time we reach it.
    // Second, we should start the DFS on the other node v. When we reach it, we recolor all red ancestors of v in black.
    // Finally, we built a subgraph, induced by the black nodes. The nodes in a new graph with zero out-degrees are the answers.
    for (int i = 0; i < numOfVertices; i++){
        adjListTransposed[i]->cor = WHITE;
        adjListTransposed[i]->descoberto = 0;
        adjListTransposed[i]->fechado = 0;
        adjListTransposed[i]->pai = 0;
    }
    DFS(numOfVertices, adjListTransposed, v1);
    DFS(numOfVertices, adjListTransposed, v2);
    return NULL;
}   


int DFS(int numVertices, Pessoa **adjListTransposed, int v1){
    tempo = 0;
    // fazer dfs nas pessoas pedidas v1 e v2
    // printf("before DFS_VISIT\n");
    DFS_Visit(numVertices, adjListTransposed, v1, v1);
    return 0;
}

int DFS_Visit(int numVertices, Pessoa **adjListTransposed, int vertice, int startingVertice){
    // printf("inside DFS_Visit, vertice %d\n", vertice);
    tempo = tempo + 1;
    Pessoa *vertice_pessoa = obterAdjacencias(adjListTransposed, vertice);
    vertice_pessoa->descoberto = tempo;
    vertice_pessoa->cor = RED;
    vertice_pessoa->startingVertice = startingVertice;
    Pessoa *adjs_vertice = vertice_pessoa->prox;
    while (adjs_vertice != NULL){
        Pessoa *p = obterAdjacencias(adjListTransposed, adjs_vertice->pessoa_id);
        if (p->cor == RED && p->startingVertice != startingVertice){
            // printf("BOTH DFS GOT HERE\nid of p is %d and startingVertice is %d\n\n", p->pessoa_id, p->startingVertice);
            pais = adicionarPessoaOrdemCrescente(pais, criar_pessoa(adjs_vertice->pessoa_id));
            p->cor = BLACK;
        }
        if (p->cor == WHITE){
            p->pai = vertice_pessoa->pessoa_id;
            DFS_Visit(numVertices, adjListTransposed, adjs_vertice->pessoa_id, startingVertice);
        }
        adjs_vertice = adjs_vertice->prox;
    }
    tempo = tempo + 1;
    vertice_pessoa->fechado = tempo;
    /* vertice_pessoa->cor = BLACK; */
    return 0;
}

Pessoa *adicionarPessoaOrdemCrescente(Pessoa *lst, Pessoa *p){
    Pessoa *lst_temp = lst;
    // se lista estiver vazia
    if (lst == NULL){
        return p;
    }
    // se for menor do que o primeiro logo
    if (lst_temp->pessoa_id > p->pessoa_id){
        p->prox = lst_temp;
        return p;
    }
    // senao, percorrer a lista
    while(lst_temp->prox != NULL && lst_temp->pessoa_id <= p->pessoa_id){

        lst_temp = lst_temp->prox;
    }
    if (lst_temp->pessoa_id == p->pessoa_id){
        return lst;
    }
    else{
        lst_temp->prox = p;
    }
    return lst;
}