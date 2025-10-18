/*
 * =================================================================================
 * Desafio Nível Mestre: Detective Quest (v2 - Com História e Opções)
 * =================================================================================
 * Este programa implementa a lógica completa do jogo Detective Quest, integrando
 * três estruturas de dados para gerenciar o mapa, as pistas e os suspeitos.
 * Adiciona um resumo da história e opções numeradas para acusação.
 *
 * - Árvore Binária: Representa o mapa da mansão para exploração.
 * - Árvore Binária de Busca (BST): Armazena e ordena as pistas coletadas.
 * - Tabela Hash com Encadeamento: Associa cada pista a um suspeito.
 *
 * O código segue os requisitos funcionais e não funcionais do desafio.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAMANHO_TABELA 10 // Define o tamanho da Tabela Hash

// --- ESTRUTURAS DE DADOS (STRUCTS) ---

// Estrutura para a Árvore Binária do Mapa da Mansão
typedef struct Sala {
    char nome[50];
    struct Sala* esquerda;
    struct Sala* direita;
} Sala;

// Estrutura para a Árvore Binária de Busca (BST) de Pistas
typedef struct PistaNode {
    char pista[100];
    struct PistaNode* esquerda;
    struct PistaNode* direita;
} PistaNode;

// Estrutura para a Tabela Hash (nó da lista encadeada)
typedef struct HashNode {
    char pista[100];    // Chave
    char suspeito[50];  // Valor
    struct HashNode* proximo;
} HashNode;

// --- VARIÁVEIS GLOBAIS ---

// Ponteiro para a raiz da árvore de pistas coletadas
PistaNode* arvorePistas = NULL;

// A Tabela Hash é um vetor de ponteiros para os nós
HashNode* tabelaHash[TAMANHO_TABELA];

// Lista de suspeitos para a fase de acusação
const char* LISTA_SUSPEITOS[] = {"Sr. Mostarda", "Sra. Branca", "Professor Black", "Dona Violeta"};
const int NUM_SUSPEITOS = 4; // Quantidade de suspeitos na lista

// --- PROTÓTIPOS DAS FUNÇÕES ---

// Funções de Documentação Obrigatória
Sala* criarSala(const char* nome);
void explorarSalas(Sala* salaAtual);
PistaNode* inserirPista(PistaNode* raiz, const char* pista);
void inserirNaHash(const char* pista, const char* suspeito);
const char* encontrarSuspeito(const char* pista);
void verificarSuspeitoFinal();

// Funções Auxiliares
void limparBuffer();
void inicializarHash();
int funcao_hash(const char* chave);
const char* obterPistaDaSala(const char* nomeSala);
void popularTabelaHash();
Sala* montarMapa();
void exibirPistas(PistaNode* raiz);
int contarPistasParaSuspeito(PistaNode* no, const char* suspeito);
void liberarMapa(Sala* raiz);
void liberarPistas(PistaNode* raiz);
void liberarHash();
void exibirResumoHistoria(); // <-- Nova função para a história


/*
 * =================================================================================
 * FUNÇÃO PRINCIPAL (main)
 * =================================================================================
 */
int main() {
    inicializarHash();      // Prepara a tabela hash para uso
    popularTabelaHash();    // Preenche a tabela com as associações pista-suspeito
    Sala* mapa = montarMapa(); // Monta o mapa fixo da mansão

    exibirResumoHistoria(); // <-- Chama a função para exibir a história

    printf("Pressione Enter para comecar a investigacao...");
    getchar(); // Pausa para o jogador ler a história

    // Inicia a exploração a partir da raiz da árvore (Hall de Entrada)
    explorarSalas(mapa);

    // Conduz a fase de julgamento final após a exploração
    verificarSuspeitoFinal();

    // Libera toda a memória alocada dinamicamente
    liberarMapa(mapa);
    liberarPistas(arvorePistas);
    liberarHash();

    printf("\nFim de jogo. Obrigado por jogar!\n");
    return 0;
}

// --- IMPLEMENTAÇÃO DAS FUNÇÕES ---

/**
 * @brief Exibe um breve resumo da história do jogo.
 */
void exibirResumoHistoria() {
    printf("--- Bem-vindo ao Detective Quest! ---\n\n");
    printf("Um crime misterioso ocorreu na Mansao Tudor!\n");
    printf("Como detetive encarregado(a) do caso, sua missao e explorar os comodos,\n");
    printf("coletar pistas cruciais e usar sua logica para desvendar quem,\n");
    printf("dentre os enigmaticos moradores, e o verdadeiro culpado.\n\n");
    printf("Cada comodo pode esconder uma evidencia. Preste atencao aos detalhes!\n");
    printf("Ao final da sua jornada, voce tera a chance de acusar um suspeito.\n");
    printf("Boa sorte, detetive!\n\n");
}

/**
 * @brief Cria dinamicamente um cômodo (nó da árvore do mapa).
 * Aloca memória para uma nova sala, atribui seu nome e inicializa
 * os filhos como NULL.
 * @param nome O nome do cômodo a ser criado.
 * @return Ponteiro para a nova Sala criada.
 */
Sala* criarSala(const char* nome) {
    Sala* novaSala = (Sala*) malloc(sizeof(Sala));
    if (novaSala == NULL) { // Verifica se malloc falhou
        perror("Erro ao alocar memoria para Sala");
        exit(EXIT_FAILURE);
    }
    strcpy(novaSala->nome, nome);
    novaSala->esquerda = NULL;
    novaSala->direita = NULL;
    return novaSala;
}

/**
 * @brief Navega pela árvore do mapa e ativa o sistema de pistas.
 * Controla o loop de exploração interativa, permitindo que o jogador
 * se mova para a esquerda (e) ou direita (d), ou saia (s).
 * A cada sala visitada, verifica e coleta a pista associada.
 * @param salaAtual A sala inicial da exploração (raiz do mapa).
 */
void explorarSalas(Sala* salaAtual) {
    char escolha;
    while (salaAtual != NULL) {
        printf("\n----------------------------------------\n");
        printf("Voce esta em: %s\n", salaAtual->nome);

        // Identifica e coleta a pista da sala atual
        const char* pistaEncontrada = obterPistaDaSala(salaAtual->nome);
        if (pistaEncontrada != NULL) {
            printf(">> Pista encontrada: \"%s\" <<\n", pistaEncontrada);
            arvorePistas = inserirPista(arvorePistas, pistaEncontrada);
        } else {
            printf("Nenhuma pista encontrada aqui.\n");
        }

        // Verifica se é um beco sem saída (nó folha)
        if (salaAtual->esquerda == NULL && salaAtual->direita == NULL) {
            printf("Este comodo nao tem mais saidas. Fim da exploracao neste caminho.\n");
            break;
        }

        // Opções de navegação
        printf("\nCaminhos disponiveis:\n");
        if (salaAtual->esquerda) printf(" (e) Esquerda: %s\n", salaAtual->esquerda->nome);
        if (salaAtual->direita)  printf(" (d) Direita:  %s\n", salaAtual->direita->nome);
        printf(" (s) Sair da exploracao\n");
        printf("Para onde voce quer ir? ");

        // Loop para garantir entrada válida (e, d ou s)
        while (scanf(" %c", &escolha) != 1 || (escolha != 'e' && escolha != 'd' && escolha != 's')) {
             limparBuffer(); // Limpa entrada inválida
             printf("Opcao invalida. Digite e, d ou s: ");
        }
        limparBuffer(); // Limpa o '\n' restante


        if (escolha == 'e') {
            if (salaAtual->esquerda != NULL) {
                salaAtual = salaAtual->esquerda;
            } else {
                printf("Nao ha caminho para a esquerda.\n");
            }
        } else if (escolha == 'd') {
            if (salaAtual->direita != NULL) {
                salaAtual = salaAtual->direita;
            } else {
                printf("Nao ha caminho para a direita.\n");
            }
        } else if (escolha == 's') {
            printf("Voce decidiu parar a exploracao por enquanto.\n");
            break; // Termina a exploração
        }
        // Não precisa de 'else' aqui por causa do loop de validação
    }
}


/**
 * @brief Insere a pista coletada na árvore de pistas (BST).
 * A função é recursiva e insere a nova pista na posição correta
 * para manter a ordem alfabética da árvore.
 * @param raiz A raiz da árvore (ou sub-árvore) onde a pista será inserida.
 * @param pista A string da pista a ser adicionada.
 * @return A nova raiz da árvore (ou sub-árvore).
 */
PistaNode* inserirPista(PistaNode* raiz, const char* pista) {
    if (raiz == NULL) {
        PistaNode* novoNo = (PistaNode*) malloc(sizeof(PistaNode));
        if (novoNo == NULL) {
             perror("Erro ao alocar memoria para PistaNode");
             exit(EXIT_FAILURE);
        }
        strcpy(novoNo->pista, pista);
        novoNo->esquerda = novoNo->direita = NULL;
        return novoNo;
    }

    int cmp = strcmp(pista, raiz->pista);
    if (cmp < 0) {
        raiz->esquerda = inserirPista(raiz->esquerda, pista);
    } else if (cmp > 0) {
        raiz->direita = inserirPista(raiz->direita, pista);
    }
    // Se cmp == 0, a pista já existe, não faz nada.

    return raiz;
}

/**
 * @brief Insere uma associação pista/suspeito na tabela hash.
 * Calcula o índice hash para a pista e adiciona a associação
 * no início da lista encadeada (tratamento de colisão por encadeamento).
 * @param pista A chave da tabela hash.
 * @param suspeito O valor a ser associado à chave.
 */
void inserirNaHash(const char* pista, const char* suspeito) {
    int indice = funcao_hash(pista);

    HashNode* novoNo = (HashNode*) malloc(sizeof(HashNode));
     if (novoNo == NULL) {
        perror("Erro ao alocar memoria para HashNode");
        exit(EXIT_FAILURE);
    }
    strcpy(novoNo->pista, pista);
    strcpy(novoNo->suspeito, suspeito);

    novoNo->proximo = tabelaHash[indice];
    tabelaHash[indice] = novoNo;
}

/**
 * @brief Consulta o suspeito correspondente a uma pista na tabela hash.
 * Calcula o índice hash e percorre a lista encadeada naquele
 * índice até encontrar a pista correspondente.
 * @param pista A pista (chave) a ser procurada.
 * @return O nome do suspeito se encontrado, ou "Desconhecido" caso contrário.
 */
const char* encontrarSuspeito(const char* pista) {
    int indice = funcao_hash(pista);
    HashNode* atual = tabelaHash[indice];

    while (atual != NULL) {
        if (strcmp(atual->pista, pista) == 0) {
            return atual->suspeito; // Encontrou
        }
        atual = atual->proximo;
    }
    return "Desconhecido"; // Não encontrou ou pista não está na hash
}

/**
 * @brief Conduz a fase de julgamento final.
 * Lista as pistas coletadas, apresenta opções de suspeitos,
 * lê a escolha do jogador e verifica se a acusação é sustentada
 * por pelo menos duas pistas.
 */
void verificarSuspeitoFinal() {
    printf("\n--- FASE DE JULGAMENTO ---\n");

    if (arvorePistas == NULL) {
        printf("Voce nao coletou nenhuma pista. O misterio continua...\n");
        return;
    }

    printf("Pistas coletadas (em ordem):\n");
    exibirPistas(arvorePistas);

    // Apresenta as opções de suspeitos
    printf("\nSuspeitos:\n");
    for (int i = 0; i < NUM_SUSPEITOS; ++i) {
        printf("%d. %s\n", i + 1, LISTA_SUSPEITOS[i]);
    }

    int escolha = 0;
    const char* acusado = NULL; // Ponteiro para o nome do suspeito escolhido

    // Loop para garantir escolha válida
    while (1) {
        printf("Com base nas pistas, quem voce acusa? (Digite o numero): ");
        if (scanf("%d", &escolha) == 1 && escolha >= 1 && escolha <= NUM_SUSPEITOS) {
            limparBuffer();
            acusado = LISTA_SUSPEITOS[escolha - 1]; // Pega o nome do array
            break; // Sai do loop se a escolha for válida
        } else {
            limparBuffer(); // Limpa entrada inválida (não número ou número fora do range)
            printf("Escolha invalida. Por favor, digite um numero entre 1 e %d.\n", NUM_SUSPEITOS);
        }
    }


    printf("\nVoce acusou: %s\n", acusado);

    // Conta quantas das pistas coletadas apontam para o suspeito acusado
    int contagemPistas = contarPistasParaSuspeito(arvorePistas, acusado);

    printf("\nAnalisando as evidencias...\n");
    printf("Voce encontrou %d pista(s) apontando para %s.\n", contagemPistas, acusado);

    // Verifica se a acusação é válida (pelo menos 2 pistas)
    if (contagemPistas >= 2) {
        printf("\n========================================================\n");
        printf(" VEREDITO: Correto! As evidencias sao contundentes.\n");
        printf(" %s e o culpado!\n", acusado);
        printf("========================================================\n");
    } else {
        printf("\n===============================================================\n");
        printf(" VEREDITO: Incorreto. As evidencias nao sao suficientes.\n");
        printf(" O verdadeiro culpado escapou desta vez!\n");
        printf("===============================================================\n");
    }
}


// --- FUNÇÕES AUXILIARES ---

// Limpa o buffer de entrada do teclado
void limparBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// Inicializa a tabela hash com NULLs
void inicializarHash() {
    for (int i = 0; i < TAMANHO_TABELA; i++) {
        tabelaHash[i] = NULL;
    }
}

// Função hash simples que soma os valores ASCII dos caracteres
int funcao_hash(const char* chave) {
    int soma = 0;
    for (int i = 0; chave[i] != '\0'; i++) {
        soma = (soma + chave[i]) % TAMANHO_TABELA; // Adiciona módulo dentro do loop para evitar overflow
    }
    return soma; // Retorna o valor final já dentro do range
}

// Retorna a pista associada a uma sala (mapa de pistas fixo)
const char* obterPistaDaSala(const char* nomeSala) {
    if (strcmp(nomeSala, "Sala de Jantar") == 0) return "Candelabro emborcado";
    if (strcmp(nomeSala, "Cozinha") == 0) return "Faca de cozinha faltando";
    if (strcmp(nomeSala, "Biblioteca") == 0) return "Livro fora do lugar";
    if (strcmp(nomeSala, "Jardim de Inverno") == 0) return "Vaso de planta quebrado";
    if (strcmp(nomeSala, "Escritorio") == 0) return "Carta amassada na lixeira";
    return NULL; // Sala sem pista
}

// Preenche a tabela hash com as associações pista-suspeito fixas
void popularTabelaHash() {
    // Garante que a tabela esteja vazia antes de popular (caso seja chamada mais de uma vez)
    liberarHash(); // Limpa primeiro
    inicializarHash(); // Reinicializa

    inserirNaHash("Candelabro emborcado", "Sr. Mostarda");
    inserirNaHash("Faca de cozinha faltando", "Sra. Branca");
    inserirNaHash("Livro fora do lugar", "Professor Black");
    inserirNaHash("Vaso de planta quebrado", "Sr. Mostarda"); // Segunda pista para Sr. Mostarda
    inserirNaHash("Carta amassada na lixeira", "Dona Violeta");
}

// Monta a estrutura fixa da árvore do mapa da mansão
Sala* montarMapa() {
    Sala* raiz = criarSala("Hall de Entrada");
    raiz->esquerda = criarSala("Sala de Jantar");
    raiz->direita = criarSala("Biblioteca");
    raiz->esquerda->esquerda = criarSala("Cozinha");
    raiz->direita->esquerda = criarSala("Jardim de Inverno");
    raiz->direita->direita = criarSala("Escritorio");
    return raiz;
}

// Exibe as pistas da BST em ordem alfabética (percurso em-ordem)
void exibirPistas(PistaNode* raiz) {
    if (raiz != NULL) {
        exibirPistas(raiz->esquerda);
        printf(" - \"%s\" (Suspeito associado: %s)\n", raiz->pista, encontrarSuspeito(raiz->pista)); // Mostra o suspeito
        exibirPistas(raiz->direita);
    }
}

// Função recursiva para contar, na BST de pistas, quantas apontam para um suspeito
int contarPistasParaSuspeito(PistaNode* no, const char* suspeito) {
    if (no == NULL) {
        return 0; // Caso base da recursão
    }

    int contagem = 0;
    const char* suspeitoDaPista = encontrarSuspeito(no->pista);

    // Compara o nome do suspeito encontrado na hash com o nome do suspeito acusado
    if (suspeitoDaPista != NULL && strcmp(suspeitoDaPista, suspeito) == 0) {
        contagem = 1; // Encontrou uma pista para o suspeito acusado neste nó
    }

    // Soma a contagem deste nó com as contagens das sub-árvores
    return contagem + contarPistasParaSuspeito(no->esquerda, suspeito) + contarPistasParaSuspeito(no->direita, suspeito);
}

// Libera a memória alocada para o mapa (percurso pós-ordem)
void liberarMapa(Sala* raiz) {
    if (raiz != NULL) {
        liberarMapa(raiz->esquerda);
        liberarMapa(raiz->direita);
        free(raiz);
    }
}

// Libera a memória alocada para as pistas (percurso pós-ordem)
void liberarPistas(PistaNode* raiz) {
    if (raiz != NULL) {
        liberarPistas(raiz->esquerda);
        liberarPistas(raiz->direita);
        free(raiz);
    }
}

// Libera a memória alocada para a tabela hash
void liberarHash() {
    for (int i = 0; i < TAMANHO_TABELA; i++) {
        HashNode* atual = tabelaHash[i];
        while (atual != NULL) {
            HashNode* temp = atual;
            atual = atual->proximo;
            free(temp);
        }
        tabelaHash[i] = NULL; // Garante que o ponteiro na tabela seja NULL após liberar
    }
}