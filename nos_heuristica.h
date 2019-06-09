extern unsigned int nodesGenerated; 

/**
 *Define a estrutura do n� usada para criar uma �rvore de busca
**/
typedef struct Node Node;
struct Node {
    unsigned int depth; 
// profundidade do n� da raiz. Para pesquisa em A *, isso tamb�m representar� o custo do caminho do n�
    unsigned int hCost; //custo heur�stico do n�
    State *state;       //estado designado para um n�
    Node *parent;       //n� pai
    NodeList *children; //lista de n�s filhos
};

/**
 * Esta fun��o cria um n�, inicializa-o com o
 * seguindo os par�metros e define seus filhos para NULL.
 * PAR�METROS:
 * d - profundidade do n�
 * h - valor heur�stico do n�
 * s - estado atribu�do ao n�
 * p - n� pai
 * RETORNA:
 * Retorna um ponteiro `Node` para o n� alocado dinamicamente,
 * ou NULL na falha.
**/
Node* createNode(unsigned int d, unsigned int h, State *s, Node *p) {
    Node *newNode = malloc(sizeof(Node));
    if(newNode) {
        newNode->depth = d;
        newNode->hCost = h;
        newNode->state = s;
        newNode->parent = p;
        newNode->children = NULL;
        ++nodesGenerated; //atualiza contador
    }
    return newNode;
}

/**
 Esta fun��o � usada para desalocar todos os n�s em uma �rvore, incluindo
 * o n� raiz.
 * PAR�METRO:
 * node - o n� raiz da �rvore para desalocar
**/
void destroyTree(Node *node) {
    if(node->children == NULL) {
        free(node->state);
        free(node);
        return;
    }

    ListNode *listNode = node->children->head;
    ListNode *nextNode;

    while(listNode) {
        nextNode = listNode->nextNode;
        destroyTree(listNode->currNode);
        listNode = nextNode;
    }

    //deixa livre o no apontado par estado
    free(node->children);
    free(node);
}

/**
 *  Esta fun��o 'expande' o n�, liga-o aos seus filhos e atualiza o
 * contador de expans�o.
 * PAR�METRO:
 * parent - o n� para expandir e procurar por crian�as
 * goalState - ponteiro para o estado do objetivo onde os valores heur�sticos de cada crian�a
 * ser baseado em
 * RETORNA:
 * Retorna um ponteiro para `NodeList` em sucesso, NULL em falha.
**/
NodeList* getChildren(Node *parent, State *goalState) {
    NodeList *childrenPtr = NULL;
    State *testState = NULL;
    Node *child = NULL;

    //tenta criar estados para cada movimento, e adicione � lista de crian�as se verdadeiro
    if(parent->state->action != DOWN && (testState = createState(parent->state, UP))) {
        child = createNode(parent->depth + 1, manhattanDist(testState, goalState), testState, parent);
        pushNode(child, &parent->children);
        pushNode(child, &childrenPtr);
    }
    if(parent->state->action != UP && (testState = createState(parent->state, DOWN))) {
        child = createNode(parent->depth + 1, manhattanDist(testState, goalState), testState, parent);
        pushNode(child, &parent->children);
        pushNode(child, &childrenPtr);
    }
    if(parent->state->action != RIGHT && (testState = createState(parent->state, LEFT))) {
        child = createNode(parent->depth + 1, manhattanDist(testState, goalState), testState, parent);
        pushNode(child, &parent->children);
        pushNode(child, &childrenPtr);
    }
    if(parent->state->action != LEFT && (testState = createState(parent->state, RIGHT))) {
        child = createNode(parent->depth + 1, manhattanDist(testState, goalState), testState, parent);
        pushNode(child, &parent->children);
        pushNode(child, &childrenPtr);
    }

    return childrenPtr;
}

/**
 Isso simplesmente avalia o custo total do n�, ou seja, custo do caminho + valor heur�stico.
 * Criado para abstrair o processo e reduzir a confus�o de c�digo. Observe que
 * o custo do caminho de um n� em uma �rvore depende puramente da profundidade do n�, de modo que o n�
 * profundidade representar� o custo do caminho (para economizar espa�o).
 * PAR�METRO:
      n� - o n� para obter seu custo total
 * RETORNA:
 * Retorna a soma inteira do custo do caminho do n� e do valor heur�stico
**/
int totalCost(Node * const node) {
    return node->depth + node->hCost;
}
