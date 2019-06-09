extern unsigned int nodesExpanded; 
extern unsigned int nodesGenerated;
extern unsigned int solutionLength;
extern double runtime;             


void printInstructions(void) {
    printf(
        "\t\t\t\t\t\t\ISTRUCOES INICIAIS\t\t\t\t\t\n\n\n"
        "Digite primeiro o estado inicial da placa, utilizando numeros interios de 0 a 8.\n\n"
        "Em seguida digite o estado final (objetivo) da placa, utilizando numeros interios de 0 a 8.\n\n\n"
        
    );
}

/**
 *DESCRIÇÃO:
 * Esta função preenche o `estado` com números não repetidos de 0 a 8.
**/
void inputState(State * const state) {
    state->action = NOT_APPLICABLE;
    char row, col;
    int symbol;

    //validação da entrada
    char isNumUsed[9] = { 0 };

    for(row = 0; row < 3; ++row) {
        for(col = 0; col < 3; ++col) {
            printf("    Placa[%i][%i]: ", row, col);
            scanf("%i", &symbol); //impedir a varredura de nova linha do fluxo de entrada
            if(symbol >= 0 && symbol < 9) {// verifica se a entrada é um caractere em branco ou se é um número maior que 0 e menor ou igual a 8.
        
                if(!isNumUsed[symbol]) {// verifica se a entrada é repetida
                    state->board[row][col] = symbol + '0';
                    isNumUsed[symbol] = 1;
                }
                else {
                    printf("\nO numero% c ja esta sendo usado. Digite outro.\n", symbol);
                    --col;
                }
            }
            else {
                printf("\nEntrada invalida. Digite um numero de 0 a 8.\n");
                --col;
            }
        }
    }
    printf("\n");
}


//Exibe o conteúdo da placa para a saída padrão
void printBoard(char const board[][3]) {
    char row, col;

    for(row = 0; row < 3; ++row) {
        printf("+---+---+---+\n");
        for(col = 0; col < 3; ++col) {
            printf("| %c ", board[row][col]);
        }
        printf("|\n");
    }
    printf("+---+---+---+\n");
}

/**
 DESCRIÇÃO:
 * Esta função interpreta instruções numéricas do movimento para fazer,
 * a sua contraparte verbal a ser exibida na tela.
 * PARÂMETRO:
 * solução - o caminho da solução que consiste em uma lista de nós da raiz
 * para o objetivo
**/
void printSolution(struct SolutionPath *path) {
	//verifica se a solucao existe
    if(!path) {
        printf("Solucao nao encontrada.\n");
        return;
    }

	//verifica se o estado incial ja é o estado final
	if(!path->next) {
		printf("Nenhum movimento e necessario. O estado inicial ja e o estado do objetivo.\n");
		return;
	}

    printf("SOLUCAO:  \n");

    char *move[4] = { "CIMA", "BAIXO", "ESQUERDA", "DIREITA" };
    int counter = 1;

    //estará ignorando o primeiro nó, pois representa o estado inicial sem ação
    for(path = path->next; path; path = path->next, ++counter) {
        printf("%i. Mover para %s\n", counter, move[path->action]);
    }

    printf(
        "DETALHES DA SOLUCAO:\n"
        " - TAMANHO : %i\n"
        " - NOS EXPANDIDOS  : %i\n"
        " - NOS GERADOS : %i\n"
        " - TEMPO GASTO         : %g milisegundos\n"
        " - MEMORIA USADA     : %i bytes\n", solutionLength, nodesExpanded, nodesGenerated, runtime, nodesGenerated * sizeof(Node));
}
