extern unsigned int nodesGenerated; 

/**
 *Define a estrutura do nó usada para criar uma árvore de busca
**/
typedef struct Node Node;
struct Node {
    unsigned int depth; 
// profundidade do nó da raiz. Para pesquisa em A *, isso também representará o custo do caminho do nó
    unsigned int hCost; //custo heurístico do nó
    State *state;       //estado designado para um nó
    Node *parent;       //nó pai
    NodeList *children; //lista de nós filhos
};

/**
 * Esta função cria um nó, inicializa-o com o
 * seguindo os parâmetros e define seus filhos para NULL.
 * PARÂMETROS:
 * d - profundidade do nó
 * h - valor heurístico do nó
 * s - estado atribuído ao nó
 * p - nó pai
 * RETORNA:
 * Retorna um ponteiro `Node` para o nó alocado dinamicamente,
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
 Esta função é usada para desalocar todos os nós em uma árvore, incluindo
 * o nó raiz.
 * PARÂMETRO:
 * node - o nó raiz da árvore para desalocar
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
 *  Esta função 'expande' o nó, liga-o aos seus filhos e atualiza o
 * contador de expansão.
 * PARÂMETRO:
 * parent - o nó para expandir e procurar por crianças
 * goalState - ponteiro para o estado do objetivo onde os valores heurísticos de cada criança
 * ser baseado em
 * RETORNA:
 * Retorna um ponteiro para `NodeList` em sucesso, NULL em falha.
**/
NodeList* getChildren(Node *parent, State *goalState) {
    NodeList *childrenPtr = NULL;
    State *testState = NULL;
    Node *child = NULL;

    //tenta criar estados para cada movimento, e adicione à lista de crianças se verdadeiro
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
 Isso simplesmente avalia o custo total do nó, ou seja, custo do caminho + valor heurístico.
 * Criado para abstrair o processo e reduzir a confusão de código. Observe que
 * o custo do caminho de um nó em uma árvore depende puramente da profundidade do nó, de modo que o nó
 * profundidade representará o custo do caminho (para economizar espaço).
 * PARÂMETRO:
      nó - o nó para obter seu custo total
 * RETORNA:
 * Retorna a soma inteira do custo do caminho do nó e do valor heurístico
**/
int totalCost(Node * const node) {
    return node->depth + node->hCost;
}
