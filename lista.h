typedef struct Node Node;
typedef struct NodeList NodeList;

/**
 * DESCRIÇÃO:
 * O nó na lista vinculada. Note que como uma convenção, o anterior
 * o nó da cabeça da lista é NULL e o próximo nó da lista é a extremidade.
**/
typedef struct ListNode {
    Node *currNode;
    struct ListNode *prevNode; //o nó antes da instância `this`
    struct ListNode *nextNode; //o próximo nó na lista vinculada
} ListNode;

/**

* DESCRIÇÃO:
 * Contém a lista encadeada de nós, bem como outras informações da lista.
**/
struct NodeList {
    unsigned int nodeCount;    //o número de nós na lista
    ListNode *head;            //ponteiro para o primeiro nó da lista
    ListNode *tail;            //ponteiro para o último nó na lista
};

/**

 * DESCRIÇÃO:
 * Uma estrutura para manter a solução.

**/
typedef struct SolutionPath {
    Move action;
    struct SolutionPath *next;
} SolutionPath;

/**
 * DESCRIÇÃO:
 * Esta função é usada para desalocar uma lista do tipo `SolutionPath`.
**/
void destroySolution(SolutionPath **list) {
    SolutionPath *next;
    while(*list) {
        next = (*list)->next;
        free(*list);
        *list = next;
    }
    *list = NULL;
}

/**
 DESCRIÇÃO:
 * Esta função envia um nó para a lista de nós.
 * PARÂMETRO:
 * node - o nó para adicionar à lista
 * lista - um ponteiro para o ponteiro da lista para adicionar o nó
 * RETORNA:
 * Retorna 1 em sucesso, 0 em falha.
**/
char pushNode(Node *node, NodeList** const list) {
    if(!node)
        return 0;

    ListNode *doublyNode = malloc(sizeof(ListNode));
    if(!doublyNode)
        return 0;

    doublyNode->currNode = node;

    if(*list && !(*list)->nodeCount) {
        (*list)->head = doublyNode;
        (*list)->tail = doublyNode;
        doublyNode->nextNode = NULL;
        doublyNode->prevNode = NULL;
        ++(*list)->nodeCount;
        return 1;
    }

    if(*list == NULL) {
        *list = malloc(sizeof(NodeList));
        if(*list == NULL)
            return 0;

        (*list)->nodeCount = 0;
        (*list)->head = NULL;
        (*list)->tail = doublyNode;
    }
    else {
        (*list)->head->prevNode = doublyNode;
    }

    doublyNode->nextNode = (*list)->head;
    doublyNode->prevNode = NULL;
    (*list)->head = doublyNode;

    ++(*list)->nodeCount;

    return 1;
}

/**
* DESCRIÇÃO:
 * Isso detecta o nó na cauda da lista. O anterior
 * o nó do próximo nó a ser detectado será definido como NULL.
 * PARÂMETRO:
 * lista - a lista para pop
 * RETORNA:
 * Retorna o endereço do nó detectado; NULL se a lista
 *    está vazia.
**/
Node* popNode(NodeList** const list) {
    if(!*list || (*list)->nodeCount == 0)
        return NULL;

    Node *popped = (*list)->tail->currNode;
    ListNode *prevNode = (*list)->tail->prevNode;

    //liberar o nó da lista apontando para o nó a ser exibido
    free((*list)->tail);

    if((*list)->nodeCount == 1) {
        (*list)->head = NULL;
    }
	else {
		prevNode->nextNode = NULL;
	}

    (*list)->tail = prevNode;
    --(*list)->nodeCount;
    return popped;
}

/**
 DESCRIÇÃO:
 * Esta função adiciona uma lista de nós para `list`. Conecta
 * o nó principal da lista a ser anexado, na parte final da lista
 * para acrescentar. Conectar uma lista a si mesmo será ignorado. Depois de
 * a conexão, `toAppend` será atribuída a NULL. Realocação de
 * nós de `toAppend` serão tratados a partir de` list`. Isso é principalmente
 * para evitar pendurar o ponteiro, ou chamar várias vezes para o
 * mesmo endereço.
 * PARÂMETROS:
 * toAppend - a lista a ser anexada
 * lista - a lista para acrescentar `toAppend` em
**/
void pushList(NodeList **toAppend, NodeList *list) {   
	// se qualquer uma das listas for NULL, o cabeçalho da lista a ser anexada será NULL,
    // ou a lista aponta para o mesmo nó inicial
    if(!*toAppend || !list || !(*toAppend)->head || (*toAppend)->head == list->head) {
        return;
    }

    
// se a lista a anexar não tiver atualmente nenhum elemento
    if(!list->nodeCount) {
        list->head = (*toAppend)->head;
        list->tail = (*toAppend)->tail;
    }
    else {
        //conecte as listas
        (*toAppend)->tail->nextNode = list->head;
        list->head->prevNode = (*toAppend)->tail;
		list->head = (*toAppend)->head;
    }

    
// atualiza informações da lista
    list->nodeCount += (*toAppend)->nodeCount;

    free(*toAppend);
    *toAppend = NULL;
}

int totalCost(Node *); // encaminha a declaração para a próxima função

/**
 * DESCRIÇÃO:
 * Esta é uma função especial de inserção de nós usada para A *. Ao contrário de `insertNode ()`,
 * isto insere o nó em ordem baseado na soma da heurística do nó e
 * valores de custo do caminho. O nó com o menor valor é colocado no final da fila.
 * PARÂMETROS:
 * toAppend - a lista a ser anexada
 * lista - a lista para acrescentar `toAppend` em
**/
void pushListInOrder(NodeList **toAppend, NodeList *list) {
    // se qualquer uma das listas for NULL, o cabeçalho da lista a ser anexada será NULL,
    // ou a lista aponta para o mesmo nó inicial
    if(!*toAppend || !list || !(*toAppend)->head || (*toAppend)->head == list->head) {
        return;
    }

// se a lista a anexar não tiver atualmente nenhum elemento
    if(!list->nodeCount) {
        pushNode(popNode(toAppend), &list);
    }

    ListNode *toAppendNode; // listar o nó para colocar em list
    ListNode *listNode;     //para percorrer cada nó em `list`
    Node *node;

     while((toAppendNode = (*toAppend)->head)) {
        listNode = list->head;

        while(listNode && totalCost(toAppendNode->currNode) < totalCost(listNode->currNode)) {
            listNode = listNode->nextNode;
        }

        ListNode *temp = toAppendNode->nextNode;

        //se o nó principal de `toAppend` deve ser inserido após a atual cauda de` list`
        if(!listNode) {
            list->tail->nextNode = toAppendNode;
            toAppendNode->prevNode = list->tail;
            toAppendNode->nextNode = NULL;
            list->tail = toAppendNode;
        }
        else {
            //if se o nó principal de `toAppend` for inserido em algum lugar de` list`, exceto antes de sua cabeça
            if(listNode->prevNode) {
                toAppendNode->prevNode = listNode->prevNode;
                toAppendNode->nextNode = listNode;
                listNode->prevNode->nextNode = toAppendNode;
                listNode->prevNode = toAppendNode;
            }
            //se o nó principal de `toAppend` deve ser inserido antes do cabeçalho de` list
            else {
                toAppendNode->nextNode = list->head;
                toAppendNode->prevNode = NULL;
                list->head->prevNode = toAppendNode;
                list->head = toAppendNode;
            }
        }

        (*toAppend)->head = temp;
        --(*toAppend)->nodeCount;
        ++list->nodeCount;
    }

    free(*toAppend);
    *toAppend = NULL;
}
