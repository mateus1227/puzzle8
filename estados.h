#define BLANK_CHARACTER '0'

//enumera os movimentos disponíveis no jogo em relação ao caractere em branco
typedef enum Move {
    UP, DOWN, LEFT, RIGHT, //valores para cima, baixo, esquerda, direita, respectivamente
    NOT_APPLICABLE         //valor atribuído para estados de entrada inicial e objetivo
} Move;

typedef struct State {
    Move action;           //ação que resultou no estado de bordo
    char board[3][3];      //configuração da placa resultante após a aplicação da ação
} State;

/**
 ** Isso cria um estado se `move` for um movimento válido do estado da placa` state`.
 * PARÂMETROS:
 * estado - ponteiro para o estado inicial
 * move - ação a ser aplicada ao estado dado
 * RETORNA:
 * Retorna um ponteiro para um novo `Estado` depois que o movimento é aplicado. Retorna NULL
 * após o fracasso.
**/
State* createState(State *state, Move move) {
    State *newState = malloc(sizeof(State));

     // copia a configuração da placa de `state` para` newState`
    // enquanto procura pela linha e coluna do caractere em branco
    char i, j;        //usado para percorrer as matrizes 3x3
    char row, col;    //coordenadas do caractere em branco

    for(i = 0; i < 3; ++i) {
        for(j = 0; j < 3; ++j) {
            if(state->board[i][j] == BLANK_CHARACTER) {
                row = i;
                col = j;
            }

            newState->board[i][j] = state->board[i][j];
        }
    }
     // testa se as coordenadas são válidas após a tradução com base no movimento
    // se for, troque os valores do tabuleiro em questão para refletir o movimento
    if(move == UP && row - 1 >= 0) {
        char temp = newState->board[row - 1][col];
        newState->board[row - 1][col] = BLANK_CHARACTER;
        newState->board[row][col] = temp;
        newState->action = UP;
        return newState;
    }
    else if(move == DOWN  && row + 1 < 3) {
        char temp = newState->board[row + 1][col];
        newState->board[row + 1][col] = BLANK_CHARACTER;
        newState->board[row][col] = temp;
        newState->action = DOWN;
        return newState;
    }
    else if(move == LEFT  && col - 1 >= 0) {
        char temp = newState->board[row][col - 1];
        newState->board[row][col - 1] = BLANK_CHARACTER;
        newState->board[row][col] = temp;
        newState->action = LEFT;
        return newState;
    }
    else if(move == RIGHT && col + 1 < 3) {
        char temp = newState->board[row][col + 1];
        newState->board[row][col + 1] = BLANK_CHARACTER;
        newState->board[row][col] = temp;
        newState->action = RIGHT;
        return newState;
    }

    free(newState);
    return NULL;
}

/**
 Isso libera a memória de `s` e é redesignado para NULL
**/
void destroyState(State **state) {
    free(*state);
    state = NULL;
}

/**
  Uma função heurística que atribui o custo-h aos nós. Valores mais baixos
 * indique o mais próximo do objetivo.
 * PARÂMETRO:
 * curr - a configuração atual da placa
 * meta - um ponteiro para a configuração do objetivo da placa
 * RETORNA:
 * Retorna um valor heurístico maior ou igual a 0.
**/
int manhattanDist(State * const curr, State * const goal) {
    int x0, y0; //usado para indexar cada símbolo em `curr`
    int x1, y1; //corresponde linha e coluna de símbolo de curr [y0, x0] em estado final
    int dx, dy; //mudar em x0 e x1 e y0 e y1, respectivamente
    int sum = 0;

    //para cada símbolo em `curr`
    for(y0 = 0; y0 < 3; ++y0) {
        for(x0 = 0; x0 < 3; ++x0) {
            //encontra as coordenadas do mesmo símbolo em estado final
            for(y1 = 0; y1 < 3; ++y1) {
                for(x1 = 0; x1 < 3; ++x1) {
                    if(curr->board[y0][x0] == goal->board[y1][x1]) {
                        dx = (x0 - x1 < 0)? x1 - x0 : x0 - x1;
                        dy = (y0 - y1 < 0)? y1 - y0 : y0 - y1;
                        sum += dx + dy;
                    }
                }
            }
        }
    }

    return sum;
}

/**
 Isso verifica se os dois estados indicados correspondem uns aos outros. Uma alternativa
 * O caminho é verificar se a distância manhattan entre esses dois estados é zero, mas
 * levaria uma quantidade mais significativa de tempo e recurso computacional
 * PARÂMETROS:
 * state1 - state para combinar com `state2`
 * state2 - state para combinar com `state1`
 * RETORNA:
 * Retorna 1 se os estados coincidirem, 0 caso contrário.
**/
char statesMatch(State const *testState, State const *goalState) {
    char row = 3, col;

    while(row--) {
        col = 3;
        while(col--) {
            if(testState->board[row][col] != goalState->board[row][col])
                return 0;
        }
    }

    return 1;
}
