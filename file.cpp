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
Pessoa *ancestraisEmComum(Pessoa **adjListTransposed, int v1, int v2);
int limparListasPessoas(Pessoa *lst);
int imprimirListaPessoas(Pessoa *lst);

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

    printf("verificando se há ciclo de parentes com dfs\n");
    // verificar se há ciclo de parentes
    /*
    if (dfs(nOfVertices, adjList) == -1) {
        cout << "0\n";
        return -1;
    }
    */
    printf("procurando ancestrais comuns\n");
    // pesquisar ancestrais comuns mais próximos
    Pessoa *lst = ancestraisEmComum(adjListTransposed, v1, v2);
    printf("imprimindo lista de ancestrais em comum\n");
    imprimirListaPessoas(lst);
    limparListasPessoas(lst);
    // output 
    // limpar listas de adjacencias
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

// retorna ancestrais da Pessoa a
Pessoa *todosAncestraisDePessoaA(Pessoa **adjListTransposed, int pessoa_a_id){
    Pessoa *ancestrais = NULL;
    Pessoa *ancestrais_temp = NULL;
    Pessoa *pais = obterAdjacencias(adjListTransposed, pessoa_a_id);
    // passar o filho à frente
    pais = pais->prox;
    if (pais==NULL){
        return NULL;
    }
    ancestrais = criar_pessoa(pessoa_a_id);
    ancestrais_temp = ancestrais;
    while (pais != NULL){
        ancestrais_temp->prox = criar_pessoa(pais->pessoa_id);
        ancestrais_temp = ancestrais_temp->prox;
        ancestrais_temp->prox = todosAncestraisDePessoaA(adjListTransposed, pais->pessoa_id);
        pais = pais->prox;
    }
    ancestrais_temp = ancestrais;
    ancestrais = ancestrais->prox;
    delete ancestrais_temp;
    return ancestrais;
}

Pessoa *ancestraisEmComum(Pessoa **adjListTransposed, int v1, int v2){
    Pessoa * ancestrais_v1 = todosAncestraisDePessoaA(adjListTransposed, v1);
    Pessoa * ancestrais_v2 = todosAncestraisDePessoaA(adjListTransposed, v2);
    Pessoa *ancs_v1_temp, *ancs_v2_temp;
    if (ancestrais_v1 == NULL || ancestrais_v2 == NULL){
        return NULL;
    }
    ancs_v1_temp = ancestrais_v1;
    ancs_v2_temp = ancestrais_v2;
    Pessoa *ancestrais_comum = criar_pessoa(0);
    Pessoa *rtn_ancestrais = ancestrais_comum;
    while (ancs_v1_temp != NULL){
        while (ancs_v2_temp != NULL){
            if (ancs_v1_temp->pessoa_id == ancs_v2_temp->pessoa_id){
                ancestrais_comum->prox = criar_pessoa(ancs_v1_temp->pessoa_id);
                ancestrais_comum = ancestrais_comum->prox;
            }
            ancs_v2_temp = ancs_v2_temp->prox;
        }
        ancs_v1_temp = ancs_v1_temp->prox;
        ancs_v2_temp = ancestrais_v2;
    }

    printf("encontramos lista de ancestrais em comum\n");
    imprimirListaPessoas(rtn_ancestrais);

    // limpar listas de ancestrais
    limparListasPessoas(ancestrais_v1);
    limparListasPessoas(ancestrais_v2);

    // passando o zero inicial à frente (cabeça da lista)
    ancestrais_comum = rtn_ancestrais;
    rtn_ancestrais = rtn_ancestrais->prox;
    delete ancestrais_comum;
    return rtn_ancestrais;
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

int obterAncestraisComunsMaisProxOrdemAlfabetica(Pessoa *lst){
    // fazer algoritmo que vai subindo desde os vertices v1 e v2
    // utilizando a lista de adjacencias tranposta e a nao transposta
    // verifica a cada ascestral comum de v1 e v2 se nao há nenhum ancestral v1 e v2 
    // que é ancestral desse ancestral também
    return 0;
}
