typedef struct Node Node;
typedef struct NodeList NodeList;

/**
 * DESCRI��O:
 * O n� na lista vinculada. Note que como uma conven��o, o anterior
 * o n� da cabe�a da lista � NULL e o pr�ximo n� da lista � a extremidade.
**/
typedef struct ListNode {
    Node *currNode;
    struct ListNode *prevNode; //o n� antes da inst�ncia `this`
    struct ListNode *nextNode; //o pr�ximo n� na lista vinculada
} ListNode;

/**

* DESCRI��O:
 * Cont�m a lista encadeada de n�s, bem como outras informa��es da lista.
**/
struct NodeList {
    unsigned int nodeCount;    //o n�mero de n�s na lista
    ListNode *head;            //ponteiro para o primeiro n� da lista
    ListNode *tail;            //ponteiro para o �ltimo n� na lista
};

/**

 * DESCRI��O:
 * Uma estrutura para manter a solu��o.

**/
typedef struct SolutionPath {
    Move action;
    struct SolutionPath *next;
} SolutionPath;

/**
 * DESCRI��O:
 * Esta fun��o � usada para desalocar uma lista do tipo `SolutionPath`.
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
 DESCRI��O:
 * Esta fun��o envia um n� para a lista de n�s.
 * PAR�METRO:
 * node - o n� para adicionar � lista
 * lista - um ponteiro para o ponteiro da lista para adicionar o n�
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
* DESCRI��O:
 * Isso detecta o n� na cauda da lista. O anterior
 * o n� do pr�ximo n� a ser detectado ser� definido como NULL.
 * PAR�METRO:
 * lista - a lista para pop
 * RETORNA:
 * Retorna o endere�o do n� detectado; NULL se a lista
 *    est� vazia.
**/
Node* popNode(NodeList** const list) {
    if(!*list || (*list)->nodeCount == 0)
        return NULL;

    Node *popped = (*list)->tail->currNode;
    ListNode *prevNode = (*list)->tail->prevNode;

    //liberar o n� da lista apontando para o n� a ser exibido
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
 DESCRI��O:
 * Esta fun��o adiciona uma lista de n�s para `list`. Conecta
 * o n� principal da lista a ser anexado, na parte final da lista
 * para acrescentar. Conectar uma lista a si mesmo ser� ignorado. Depois de
 * a conex�o, `toAppend` ser� atribu�da a NULL. Realoca��o de
 * n�s de `toAppend` ser�o tratados a partir de` list`. Isso � principalmente
 * para evitar pendurar o ponteiro, ou chamar v�rias vezes para o
 * mesmo endere�o.
 * PAR�METROS:
 * toAppend - a lista a ser anexada
 * lista - a lista para acrescentar `toAppend` em
**/
void pushList(NodeList **toAppend, NodeList *list) {   
	// se qualquer uma das listas for NULL, o cabe�alho da lista a ser anexada ser� NULL,
    // ou a lista aponta para o mesmo n� inicial
    if(!*toAppend || !list || !(*toAppend)->head || (*toAppend)->head == list->head) {
        return;
    }

    
// se a lista a anexar n�o tiver atualmente nenhum elemento
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

    
// atualiza informa��es da lista
    list->nodeCount += (*toAppend)->nodeCount;

    free(*toAppend);
    *toAppend = NULL;
}

int totalCost(Node *); // encaminha a declara��o para a pr�xima fun��o

/**
 * DESCRI��O:
 * Esta � uma fun��o especial de inser��o de n�s usada para A *. Ao contr�rio de `insertNode ()`,
 * isto insere o n� em ordem baseado na soma da heur�stica do n� e
 * valores de custo do caminho. O n� com o menor valor � colocado no final da fila.
 * PAR�METROS:
 * toAppend - a lista a ser anexada
 * lista - a lista para acrescentar `toAppend` em
**/
void pushListInOrder(NodeList **toAppend, NodeList *list) {
    // se qualquer uma das listas for NULL, o cabe�alho da lista a ser anexada ser� NULL,
    // ou a lista aponta para o mesmo n� inicial
    if(!*toAppend || !list || !(*toAppend)->head || (*toAppend)->head == list->head) {
        return;
    }

// se a lista a anexar n�o tiver atualmente nenhum elemento
    if(!list->nodeCount) {
        pushNode(popNode(toAppend), &list);
    }

    ListNode *toAppendNode; // listar o n� para colocar em list
    ListNode *listNode;     //para percorrer cada n� em `list`
    Node *node;

     while((toAppendNode = (*toAppend)->head)) {
        listNode = list->head;

        while(listNode && totalCost(toAppendNode->currNode) < totalCost(listNode->currNode)) {
            listNode = listNode->nextNode;
        }

        ListNode *temp = toAppendNode->nextNode;

        //se o n� principal de `toAppend` deve ser inserido ap�s a atual cauda de` list`
        if(!listNode) {
            list->tail->nextNode = toAppendNode;
            toAppendNode->prevNode = list->tail;
            toAppendNode->nextNode = NULL;
            list->tail = toAppendNode;
        }
        else {
            //if se o n� principal de `toAppend` for inserido em algum lugar de` list`, exceto antes de sua cabe�a
            if(listNode->prevNode) {
                toAppendNode->prevNode = listNode->prevNode;
                toAppendNode->nextNode = listNode;
                listNode->prevNode->nextNode = toAppendNode;
                listNode->prevNode = toAppendNode;
            }
            //se o n� principal de `toAppend` deve ser inserido antes do cabe�alho de` list
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
