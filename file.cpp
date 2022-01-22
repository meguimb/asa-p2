// g++ -std=c++11 -O3 -Wall file.cpp -lm
#include <iostream>
#include <vector>
#include <bits/stdc++.h>
#include <algorithm>

using namespace std;

struct pessoa{
    int pessoa_id;
    Pessoa *prox;
};
typedef struct pessoa Pessoa;


Pessoa *obterAdjacencias(Pessoa **adj_list, int pessoa_id);
int adicionarListaAdj(Pessoa **adj_list, int pessoa, int pessoa_adjacente);
Pessoa *criar_pessoa(int pessoa_id);

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
    for (int i = 1; i <= nOfVertices; i++){
        // color[i] = 0;
        adjList[i-1] = criar_pessoa(i);
        adjListTransposed[i-1] = criar_pessoa(i);
    }
    // receber arcos
    for (int i = 0; i < nOfEdges; i++){
        cin >> temp1 >> temp2;
        // verificar condições 
        if (temp1 == temp2 || temp1 < 1 || temp2 < 1 || temp1 > nOfVertices || temp2 > nOfVertices){
            return -1;
        }
        // adicionar arco às listas de adjacencia
        if (adicionarListaAdj(adjList, temp1-1, temp2)==-1){
            return -1;
        }
        if (adicionarListaAdj(adjListTransposed, temp2-1, temp1)==-1){
            return -1;
        }
    }

    // verificar se há ciclo de parentes
    for (int i = 1; i <= nOfVertices; i++){
        
    }
    // pesquisar ancestrais comuns mais próximos

    // output 

    return 0;
}

int dfs(int initialVertice){
    return 0;
}

Pessoa *obterAdjacencias(Pessoa **adj_list, int pessoa_id){
    return adj_list[pessoa_id-1];
}

int adicionarListaAdj(Pessoa **adj_list, int pessoa, int pessoa_adjacente){
    Pessoa *adjs = obterAdjacencias(adj_list, pessoa);
    while (adjs->prox != NULL){
        // arco / relação inválida
        if (adjs->pessoa_id == pessoa_adjacente){
            return -1;
        }
        adjs = adjs->prox;
    }
    adjs->prox = criar_pessoa(pessoa_adjacente);
    return 0;
}

Pessoa *criar_pessoa(int pessoa_id){
    Pessoa *p = new Pessoa;
    p->pessoa_id = pessoa_id;
    p->prox = NULL;
    return p;
}