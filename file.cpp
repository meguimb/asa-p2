// g++ -std=c++11 -O3 -Wall file.cpp -lm
#include <iostream>
#include <vector>
#include <bits/stdc++.h>
#include <algorithm>

#define WHITE 0
#define GREY  1
#define BLACK 2

using namespace std;

struct pessoa{
    int pessoa_id;
    struct pessoa *prox;
    int mark;
    bool onStack;
};
typedef struct pessoa Pessoa;


Pessoa *obterAdjacencias(Pessoa **adj_list, int pessoa_id);
int adicionarListaAdj(Pessoa **adj_list, int pessoa, int pessoa_adjacente, bool verificar_num_max_pais);
Pessoa *criar_pessoa(int pessoa_id);
int dfs(int nOfVertices, Pessoa **adjList);
int dfs_visit(Pessoa *p);

Pessoa *criar_pessoa(int pessoa_id){
    Pessoa *p = new Pessoa;
    p->pessoa_id = pessoa_id;
    p->prox = NULL;
    p->mark = WHITE;
    p->onStack = false;
    return p;
}

int main(){
    int v1, v2, nOfVertices, nOfEdges, temp1, temp2;
    // receber input de v1 e v2
    cin >> v1 >> v2;
    printf("v1 is %d and v2 is %d\n", v1, v2);

    // receber numero de vertices e numero de arcos
    cin >> nOfVertices >> nOfEdges;

    // criar matriz de adjacencia e matriz de adjacencia transposta
    Pessoa **adjList = new Pessoa*[nOfVertices]; // cada adjList[i] tem as adjacencias da Pessoa i (onde aponta)
    Pessoa **adjListTransposed = new Pessoa*[nOfVertices]; // cada adjListTransposed[i] tem as Pessoas a que é adjacente (onde é apontado)
    // int color [nOfVertices]; // 0 - unvisited, 1 - being visited, 2 - has been visited
    printf("inicializando as listas de adjacencia\n");
    for (int i = 1; i <= nOfVertices; i++){
        // color[i] = 0;
        adjList[i-1] = criar_pessoa(i);
        adjListTransposed[i-1] = criar_pessoa(i);
    }
    printf("recebendo os arcos\n");
    // receber arcos
    for (int i = 0; i < nOfEdges; i++){
        printf("%d\n", i);
        cin >> temp1 >> temp2;
        // verificar condições 
        if (temp1 == temp2 || temp1 < 1 || temp2 < 1 || temp1 > nOfVertices || temp2 > nOfVertices){
            return -1;
        }
        // adicionar arco às listas de adjacencia
        if (adicionarListaAdj(adjList, temp1, temp2, false)==-1){
            return -1;
        }
        if (adicionarListaAdj(adjListTransposed, temp2, temp1, true)==-1){
            printf("more than 2 parents lol\n");
            return -1;
        }
    }
    printf("verificando se algum filho tem mais do que 2 pais\n");
    // verificar se qualquer filho tem no máximo 2 pais
    for (int i = 1; i <= nOfVertices; i++){ 
        Pessoa *p = obterAdjacencias(adjListTransposed, i);
        int counter = 0;
        while (p->prox != NULL){
            counter++;
            p = p->prox;
        }
        if (counter > 2){
            return -1;
        }
    }

    // verificar se há ciclo de parentes
    if (dfs(nOfVertices, adjList) == -1) {
        cout << '0\n';
        return -1;
    }
    // pesquisar ancestrais comuns mais próximos

    // output 

    return 0;
}

int dfs(int nOfVertices, Pessoa **adjList){
    for (int i = 0; i < nOfVertices; i++){
        Pessoa *p = adjList[nOfVertices];
        if (p->mark == WHITE)
            if (dfs_visit(p) == -1)
                return -1;
    }
    return 0;
}

int dfs_visit(Pessoa *p){
    Pessoa *adj = p;

    p->mark = GREY;
    p->onStack = true;

    while(adj->prox != NULL){
        adj = adj->prox;

        if(adj->mark == WHITE)
            dfs_visit(adj);
        else if(adj->mark == BLACK && adj->onStack == true)
            return -1;
    }

    p->mark = BLACK;
    p->onStack = false;

    return 0;
}

Pessoa *obterAdjacencias(Pessoa **adj_list, int pessoa_id){
    return adj_list[pessoa_id-1];
}

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
