#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include "estados.h"
#include "lista.h"
#include "nos_heuristica.h"
#include "io.h"

unsigned int nodesExpanded;  //nos expandidos
unsigned int nodesGenerated; //nos gerados
unsigned int solutionLength; //tamanho da solucao
double runtime;              //tempo gasto

SolutionPath* BFS_search(State *, State *);
SolutionPath* AStar_search(State *, State *);

int main(void) {
	
    printInstructions();     //instrucoes iniciais

    State initial;           //estado inicial da placa
    State goalState;         //estado final da placa
    
    //algoritmos usados pra achar a solucao
    SolutionPath *bfs;
    SolutionPath *aStar;

    //digitar estado inicial
    printf("ESTADO INICIAL:\n");
    inputState(&initial);

    //digitar estado final
    printf("\nESTADO FINAL:\n");
    inputState(&goalState);

    printf("ESTADO INICIAL DA PLACA:\n");
    printBoard(initial.board);

    printf("ESTADO FINAL DA PLACA:\n");
    printBoard(goalState.board);

    //perfomance do  algoritmo a*
    aStar = AStar_search(&initial, &goalState);
    printf("\n-------------------------- RESOLUCAO USANDO ALGORITMO A* --------------------------\n");
    printSolution(aStar);

    //resetar os contadores
    nodesExpanded = 0;
    nodesGenerated = 0;
    solutionLength = 0;
    runtime = 0;

    //perfomance usando algoritmo busca em largura
    bfs = BFS_search(&initial, &goalState);
    printf("\n------------------------- RESOLUCAO USANDO ALGORITMO BUSCA EM LARGURA --------------------------\n");
    printSolution(bfs);

    //limpar as configuracoes
    destroySolution(&bfs);
    destroySolution(&aStar);

    return 0;
}

/**
 * DESCRIÇÃO:
 * Nossa implementação inicial de pesquisa.
 * PARÂMETROS:
 * inicial - endereço para o estado inicial
 * meta - endereço para o estado do objetivo
 * RETORNA:
 * Retorna a solução em uma lista vinculada; NULL se a solução não for encontrada.
**/
SolutionPath* BFS_search(State *initial, State *goal) {
    NodeList *queue = NULL;
    NodeList *children = NULL;
    Node *node = NULL;

    //inicia contagem do tempo gasto
    clock_t start = clock();

    //inicializar a fila com o nó raiz da árvore de pesquisa
    pushNode(createNode(0, manhattanDist(initial, goal), initial, NULL), &queue);
    Node *root = queue->head->currNode; //desalocar a árvore gerada

    //enquanto há um nó na fila para expandir
    while(queue->nodeCount > 0) {
        //pop o último nó (final) da fila
        node = popNode(&queue);

        //se o estado do nó for o estado do objetivo
        if(statesMatch(node->state, goal))
            break;

        //se nao, expanda o nó e atualize o contador de nós expandidos
        children = getChildren(node, goal);
        ++nodesExpanded;

        //adicione os filhos do nó à fila
        pushList(&children, queue);
    }

    //determina o tempo gasto
    runtime = (double)(clock() - start) / CLOCKS_PER_SEC;

    //obter o caminho da solução na ordem da raiz, se existir
    SolutionPath *pathHead = NULL;
    SolutionPath *newPathNode = NULL;

    while(node) {
        newPathNode = malloc(sizeof(SolutionPath));
        newPathNode->action = node->state->action;
        newPathNode->next = pathHead;
        pathHead = newPathNode;

        //atualize o tamanho da solução e mova o próximo nó
        ++solutionLength;
        node = node->parent;
    }

    --solutionLength; //descobrir o nó raiz

    //desalocar a árvore gerada
    destroyTree(root);

    return pathHead;
}

/**
 DESCRIÇÃO:
 * Nossa implementação A *.
 * PARÂMETROS:
 * inicial - endereço para o estado inicial
 * meta - endereço para o estado do objetivo
 * RETORNA:
 * Retorna a solução em uma lista vinculada; NULL se a solução não for encontrada.
**/
SolutionPath* AStar_search(State *initial, State *goal) {
    NodeList *queue = NULL;
    NodeList *children = NULL;
    Node *node = NULL;

    //inicia contagem do tempo gasto
    clock_t start = clock();

    //inicializar a fila com o nó raiz da árvore de pesquisa
    pushNode(createNode(0, manhattanDist(initial, goal), initial, NULL), &queue);
    Node *root = queue->head->currNode; //para desalocar a árvore gerada

    //enquanto há um nó na fila para expandir
    while(queue->nodeCount > 0) {
        //pop o último nó (final) da fila
        node = popNode(&queue);

        //se o estado do nó for o estado do objetivo
        if(statesMatch(node->state, goal))
            break;

        //se nao, expanda o nó e atualize o contador de nós expandido
        children = getChildren(node, goal);
        ++nodesExpanded;

        //adicione os filhos do nó à fila
        pushListInOrder(&children, queue);
    }

    //determina o tempo gasto para chegar na solucao
    runtime = (double)(clock() - start) / CLOCKS_PER_SEC;

    //obter o caminho da solução na ordem da raiz, se existir
    SolutionPath *pathHead = NULL;
    SolutionPath *newPathNode = NULL;

    while(node) {
        newPathNode = malloc(sizeof(SolutionPath));
        newPathNode->action = node->state->action;
        newPathNode->next = pathHead;
        pathHead = newPathNode;

        //atualize o tamanho da solução e mova o próximo nó
        ++solutionLength;
        node = node->parent;
    }

    --solutionLength; //descobrir o nó raiz

    //desalocar a árvore gerada
    destroyTree(root);

    return pathHead;
}
