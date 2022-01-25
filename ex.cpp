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
    struct pessoa *last;
};
typedef struct pessoa Pessoa;

struct cabeca_pessoa{
    int pessoa_id;
    int cor;
    int startingVertice;
};
typedef struct cabeca_pessoa CabecaPessoa;

Pessoa *obterAdjacencias(Pessoa **adj_list, int pessoa_id);
int adicionarListaAdj(Pessoa **adj_list, int pessoa, int pessoa_adjacente);
Pessoa *criar_pessoa(int pessoa_id);
int dfs(int nOfVertices, Pessoa **adjList);
int dfs_visit(Pessoa *p, Pessoa **adjList, vector<bool> &marked, vector<bool> &onStack);
int limparListasPessoas(Pessoa *lst);
int imprimirListaPessoas(Pessoa *lst);
bool ehAscendenteDeA(Pessoa **adjList, int ascendente, int pessoa_a);
int limparListaAdjacencias(Pessoa **adjs_lst, int tamanho);
Pessoa *adicionarPessoaOrdemCrescente(Pessoa *lst, Pessoa *p);
Pessoa *newAlgo(Pessoa **adjList, Pessoa **adjListTransposed, int v1, int v2);
int DFS_Visit(int numVertices, Pessoa **adjListTransposed, int vertice, int startingIndex, CabecaPessoa **cabecas);
int DFS(int otherVertice, Pessoa **adjListTransposed, int v1, CabecaPessoa **cabecas);
Pessoa *algo(int numOfVertices, Pessoa **adjListTransposed, int v1, int v2, CabecaPessoa **cabecas);
bool ehAscendenteDeA(Pessoa **adjListTransposed, int ascendente, int pessoa_a);


Pessoa *criar_pessoa(int pessoa_id){
    Pessoa *p = new Pessoa;
    p->pessoa_id = pessoa_id;
    p->prox = NULL;
    return p;
}

CabecaPessoa *criar_cabeca_pessoa(int pessoa_id){
    CabecaPessoa *p = new CabecaPessoa;
    p->pessoa_id = pessoa_id;
    return p;
}

int tempo;
Pessoa *pais = NULL;


int main(){
    int v1, v2, nOfVertices, nOfEdges, temp1, temp2;
    // receber input de v1 e v2
    cin >> v1 >> v2;

    // receber numero de vertices e numero de arcos
    cin >> nOfVertices >> nOfEdges;

    // criar matriz de adjacencia e matriz de adjacencia transposta
    
    Pessoa **adjListTransposed = new Pessoa *[nOfVertices]; // cada adjListTransposed[i] tem as Pessoas a que é adjacente (onde é apontado)
    CabecaPessoa **cabecas = new CabecaPessoa *[nOfVertices];
    // int color [nOfVertices]; // 0 - unvisited, 1 - being visited, 2 - has been visited
    for (int i = 1; i <= nOfVertices; i++){
        // color[i] = 0;
        adjListTransposed[i-1] = criar_pessoa(i);
        adjListTransposed[i-1]->last = adjListTransposed[i-1];
        cabecas[i-1] = criar_cabeca_pessoa(i);
    }
    
    // receber arcos
    for (int i = 0; i < nOfEdges; i++){
        cin >> temp1 >> temp2;
        // verificar condições 
        if (temp1 == temp2 || temp1 < 1 || temp2 < 1 || temp1 > nOfVertices || temp2 > nOfVertices){
            printf("0\n");
            limparListaAdjacencias(adjListTransposed, nOfVertices);
            delete cabecas;
            return 0;
        }

        // adicionar arco às listas de adjacencia
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
            limparListaAdjacencias(adjListTransposed, nOfVertices);
            delete cabecas;
            return 0;
        }
    }
    // verificar se há ciclo de parentes
    if (dfs(nOfVertices, adjListTransposed) == -1) {
        cout << "0\n";
        limparListaAdjacencias(adjListTransposed, nOfVertices);
        delete cabecas;
        return 0;
    }
    // cout << "no cicle\n";
    // pesquisar ancestrais comuns mais próximos

    // output 
    Pessoa *rtnList = algo(nOfVertices, adjListTransposed, v1, v2, cabecas);
    // pais -> 0 1 2 4
    if (rtnList == NULL){
        printf("-\n");
    }
    else{
        imprimirListaPessoas(rtnList);
        limparListasPessoas(rtnList);
    }
    limparListaAdjacencias(adjListTransposed, nOfVertices);
    delete cabecas;
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

        if(!(marked[idAdj-1])){
            if (dfs_visit(obterAdjacencias(adjList, idAdj), adjList, marked, onStack) == -1){
                return -1;
            }
        }
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

Pessoa *algo(int numOfVertices, Pessoa **adjListTransposed, int v1, int v2, CabecaPessoa **cabecas){
    // Suppose we want to find the LCA(u, v) in graph G. 
    // Initially, all the vertices are colored white.
    // First, we do a Depth-First Search (DFS) on one of the target nodes. 
    // Let it be node u. Also, we’ll keep track of the parent’s array (current path from a starting vertex). 
    // During the DFS, we color all the ancestors of u in red each time we reach it.
    // Second, we should start the DFS on the other node v. When we reach it, we recolor all red ancestors of v in black.
    // Finally, we built a subgraph, induced by the black nodes. The nodes in a new graph with zero out-degrees are the answers.
    int adicionar;
    for (int i = 0; i < numOfVertices; i++){
        cabecas[i]->cor = WHITE;
    }
    if (DFS_Visit(v2, adjListTransposed, v1, v1, cabecas) != -1){
        DFS_Visit(v1, adjListTransposed, v2, v2, cabecas);
    }
    // DFS(numOfVertices, adjListTransposed, v1, cabecas);
    // DFS(numOfVertices, adjListTransposed, v2, cabecas);
    if (pais == NULL){
        return NULL;
    }
    Pessoa *rtn = NULL;
    for (Pessoa *temp1 = pais; temp1 != NULL; temp1=temp1->prox){
        adicionar = 1;
        for (Pessoa *temp2 = pais; temp2 != NULL; temp2=temp2->prox){
            if (temp1->pessoa_id == 0 || temp2->pessoa_id == 0){
                adicionar = 0;
                break;
            }
            if (temp1->pessoa_id != temp2->pessoa_id && ehAscendenteDeA(adjListTransposed, temp1->pessoa_id, temp2->pessoa_id)){
                adicionar = 0;
                break;
            }
        }
        if (adicionar==1){
            rtn = adicionarPessoaOrdemCrescente(rtn, criar_pessoa(temp1->pessoa_id));
        }
    }
    if (pais!=NULL)
        limparListasPessoas(pais);
    return rtn;
}   


int DFS_Visit(int other_vertice, Pessoa **adjListTransposed, int vertice, int startingVertice, CabecaPessoa **cabecas){
    CabecaPessoa *vertice_pessoa = cabecas[vertice-1];
    vertice_pessoa->cor = RED;
    vertice_pessoa->startingVertice = startingVertice;
    Pessoa *adjs_vertice = obterAdjacencias(adjListTransposed, vertice)->prox;
    while (adjs_vertice != NULL){
        if (adjs_vertice->pessoa_id == other_vertice){
            pais = criar_pessoa(other_vertice);
            return -1;
        }
        // Pessoa *p = obterAdjacencias(adjListTransposed, adjs_vertice->pessoa_id);
        CabecaPessoa *cp = cabecas[adjs_vertice->pessoa_id-1];
        if (cp->cor == RED && cp->startingVertice != startingVertice){
            // printf("BOTH DFS GOT HERE\nid of p is %d and startingVertice is %d\n\n", p->pessoa_id, p->startingVertice);
            pais = adicionarPessoaOrdemCrescente(pais, criar_pessoa(adjs_vertice->pessoa_id));
            cp->cor = BLACK;
        }
        if (cp->cor == WHITE){
            if (DFS_Visit(other_vertice, adjListTransposed, adjs_vertice->pessoa_id, startingVertice, cabecas) == -1){
                return -1;
            }
        }
        adjs_vertice = adjs_vertice->prox;
    }
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

bool ehAscendenteDeA(Pessoa **adjListTransposed, int ascendente, int pessoa_a){
    queue<int> ascendentes;
    int pessoa_id_atual;
    Pessoa *pessoa_atual;
    ascendentes.push(pessoa_a);
    while (!ascendentes.empty()){
        pessoa_id_atual = ascendentes.front();
        ascendentes.pop();
        pessoa_atual = adjListTransposed[pessoa_id_atual-1];
        while (pessoa_atual->prox != NULL){
            if (pessoa_atual->prox->pessoa_id == ascendente){
                return true;
            }
            ascendentes.push(pessoa_atual->prox->pessoa_id);
            pessoa_atual = pessoa_atual->prox;
        }
    }
    return false;
}