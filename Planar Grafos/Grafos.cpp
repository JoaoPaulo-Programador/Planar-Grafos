#include <stdio.h>  
#include <stdlib.h>
#include <string.h>

#define MAXVERTICES 100

typedef struct { //representar o grafo de vertices 
    int matriz[MAXVERTICES][MAXVERTICES]; //armazena arestas 
    int num_vertices; //numero de vertices
} grafoplanar; 

int num_arestas(grafoplanar* g) { //conta o numero de arestas do grafo
    int count = 0;
    for (int i = 0; i < g->num_vertices; i++) {
        for (int j = i + 1; j < g->num_vertices; j++) {
            if (g->matriz[i][j] == 1) { //verifica se existe aresta entre i e j
                count++; //conta aresta 
            }
        }
    }
    return count; //retorna numero total de arestas
} // Complexidade: O(n^2)

grafoplanar matriz(const char* nomearquivo) { //carregar o grafo de um arquivo a partir da matriz 
    grafoplanar g;
    g.num_vertices = 0;
    char linha[512]; //armazena cada linha do arquivo 

    FILE* arquivo = fopen(nomearquivo, "r"); //abre o arquivo para leitura 
    if (arquivo == NULL) { //arquivo aberto da forma certa
        fprintf(stderr, "erro ao abrir o arquivo\n");
        g.num_vertices = -1; //caso de erro em abrir o arquivo
        return g;  
    } 

    while (fgets(linha, sizeof(linha), arquivo) != NULL) { //le arquivo linha por linha
        if (linha[0] == '\n') { 
            break; //acaba de ler se tiver uma linha em branco
        }

        char* token = strtok(linha, " "); //separa linha em espaço
        int j = 0;
        while (token != NULL && j < MAXVERTICES) { //converter para valor inteiro
            g.matriz[g.num_vertices][j++] = atoi(token); //converte e armazena 
            token = strtok(NULL, " ");
        }
        g.num_vertices++; //conta o numero de vertices lido
        if (g.num_vertices >= MAXVERTICES) { //para se atingir 100 vertices
            break;
        }
    }

    fclose(arquivo); //fecha o arquivo
    return g;
} //Complexidade: O(N)

int verificaK5(grafoplanar* g, int vertices[5]) { //verifica se a matriz é um K5
    for (int i = 0; i < 5; i++) { //verifica se cada par esta conectado
        for (int j = 0; j < 5; j++) {
            if (i != j && g->matriz[vertices[i]][vertices[j]] != 1) {
                return 0; //se não for um k5 retorna 0 (falso)
            }
        }
    }
    return 1; //se for um k5 retorna 1 (verdadeiro)
} //Complexidade: O(5²) = O(25)

int verificaK33(grafoplanar* g, int vertices[6]) { //verifica se a matriz é um K3,3
    for (int i = 0; i < 3; i++) {
        for (int j = 3; j < 6; j++) { //verifica se estão dividido em 2 grupos de 3 
            if (g->matriz[vertices[i]][vertices[j]] != 1 || g->matriz[vertices[j]][vertices[i]] != 1) { //se ha arestas nos grupos 1 e 2
                return 0; //se não for um k3,3 retorna 0 (falso)
            }
        }
    }
    return 1; //se for um k3,3 retorna 1 (verdadeiro)
} // Complexidade: O(3 * 3) = O(9)

int verificaK33sub(grafoplanar* g) { //Vai olhar se a Matriz tem um subgrafo k3,3
    int vertices[6]; //vai fazer uma combnações de 6 vertices
    for (int i = 0; i < g->num_vertices - 5; i++) { //percorre os 6 primeiros vertices // Complexidade: O(N)
        for (int j = i + 1; j < g->num_vertices - 4; j++) { //segundo vertice apos o 1º // Complexidade: O(N)
            for (int k = j + 1; k < g->num_vertices - 3; k++) { // Complexidade: O(N)
                for (int l = k + 1; l < g->num_vertices - 2; l++) { // Complexidade: O(N)
                    for (int m = l + 1; m < g->num_vertices - 1; m++) { // Complexidade: O(N)
                        for (int n = m + 1; n < g->num_vertices; n++) { // Complexidade: O(N)
                            vertices[0] = i; vertices[1] = j; vertices[2] = k;
                            vertices[3] = l; vertices[4] = m; vertices[5] = n;
                            if (verificaK33(g, vertices)) { //se a matriz tem um subgrafo k3,3
                                return 1; //o codigo retorna 1 (verdadeiro)
                            }
                        }
                    }
                }
            }
        }
    }
    return 0; //se não for um subgrafo k3,3 -> retorna 0 (falso)
} // Complexidade: O(N^6)

int verificaK5sub(grafoplanar* g) { //Vai olhar se a Matriz tem um subgrafo k5
    int vertices[5]; //vai fazer uma combnações de 5 vertices
    for (int i = 0; i < g->num_vertices - 4; i++) { //percorre os 5 primeiros vertices // Complexidade: O(N)
        for (int j = i + 1; j < g->num_vertices - 3; j++) { //segundo vertice apos o 1º // Complexidade: O(N)
            for (int k = j + 1; k < g->num_vertices - 2; k++) { // Complexidade: O(N)
                for (int l = k + 1; l < g->num_vertices - 1; l++) { // Complexidade: O(N)
                    for (int m = l + 1; m < g->num_vertices; m++) { // Complexidade: O(N)
                        vertices[0] = i; vertices[1] = j; vertices[2] = k;
                        vertices[3] = l; vertices[4] = m;
                        if (verificaK5(g, vertices)) { //se a matriz tem um subgrafo k5
                            return 1; //o codigo retorna 1 (verdadeiro)
                        }
                    }
                }
            }
        }
    }
    return 0; //o codigo retorna 0 (falso)
} // Complexidade: O(N^5)

int planar(grafoplanar* g) { //verifica se o grafo é planar ou não
    if (g->num_vertices < 5) { //matrizes até 4 vertices 
        return 1; //planar
    } // Complexidade: O(1)

    if (verificaK5sub(g)) { //verifica se existe um subgrafo k5
        return 0; //não planar
    } // Complexidade: O(N^5)

    if (verificaK33sub(g)) { //verifica se existe um subgrafo k3,3
        return 0; //não planar
    } // Complexidade: O(N^6)

    return 1; //se não existir nenhuma dessas o grafo é planar -> Complexidade: O(1)
} // Complexidade: O(N^6)

int main() {
    char nomearquivo[100];
    char continuar[10];

    do {
        printf("\nnome do arquivo: ");
        scanf("%s", nomearquivo); //pega o nome do arquivo 

        char caminho_arquivo[120];
        snprintf(caminho_arquivo, sizeof(caminho_arquivo), "%s.txt", nomearquivo);

        grafoplanar g = matriz(caminho_arquivo); //le a matriz do arquivo

        if (g.num_vertices == -1) {
            printf("\nler outro arquivo? (s/n): ");
            scanf(" %s", continuar);
            continue;  
        }

        if (planar(&g)) { //verifica se o grafo é planar 
            printf("planar.\n");
        } else {
            printf("nao planar.\n");
        }

        printf("\nvoce deseja ler outro arquivo? (s/n): ");
        scanf(" %s", continuar);
        
        while (strcmp(continuar, "s") != 0 && strcmp(continuar, "n") != 0) {
            printf("resposta invalida. digite 's' ou 'n': ");
            scanf(" %s", continuar);
        }

    } while (strcmp(continuar, "s") == 0);

    return 0;
}



