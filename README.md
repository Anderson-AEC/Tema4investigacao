# Detective Quest - Nível Mestre

[cite_start]Este projeto é uma implementação em C da lógica do jogo **Detective Quest**, baseada no desafio "Nível Mestre" do material didático "Algoritmos Avançados" (Prof. Nathan Alves)[cite: 2599, 2600, 4094]. O programa simula uma investigação de mistério em uma mansão, integrando três estruturas de dados avançadas para gerenciar a exploração do mapa, a coleta de pistas e a associação de evidências a suspeitos.

[cite_start]O jogador assume o papel de um detetive, explora os cômodos, coleta pistas e, ao final, deve acusar o culpado com base nas evidências encontradas[cite: 2628, 4105].

## 🚀 Estruturas de Dados e Funcionalidades

[cite_start]O projeto é construído sobre a integração de três estruturas de dados fundamentais, cada uma com um papel específico no jogo[cite: 2610]:

1.  **Árvore Binária (Mapa da Mansão)**:
    * [cite_start]Representa os cômodos da mansão e os caminhos entre eles de forma hierárquica[cite: 2631, 3134].
    * [cite_start]O jogador navega interativamente por esta árvore, escolhendo ir para a esquerda ou direita em cada sala[cite: 3112].

2.  **Árvore Binária de Busca (BST - Diário de Pistas)**:
    * [cite_start]Armazena todas as pistas que o jogador encontra durante a exploração[cite: 2632, 4103, 4110].
    * [cite_start]Por ser uma BST, as pistas são mantidas em ordem alfabética, o que permite exibi-las de forma organizada ao final do jogo[cite: 2632, 3642].

3.  **Tabela Hash (Cérebro do Detetive)**:
    * [cite_start]Associa cada pista (chave) a um suspeito específico (valor) de forma extremamente eficiente[cite: 2633, 4104, 4111].
    * [cite_start]Utiliza a técnica de **encadeamento** para tratar colisões[cite: 3821].
    * [cite_start]É a estrutura central na fase de julgamento para verificar rapidamente quantas das pistas coletadas apontam para o suspeito acusado[cite: 4105, 4127].

### Funcionalidades do Jogo

* **História Introdutória**: Apresenta ao jogador o cenário do crime na Mansão Tudor.
* [cite_start]**Exploração Interativa**: Navegue pela mansão usando os comandos 'e' (esquerda), 'd' (direita) ou 's' (sair)[cite: 3112].
* [cite_start]**Coleta Automática de Pistas**: Ao entrar em um cômodo que contém uma pista, ela é automaticamente adicionada ao seu diário (a BST)[cite: 3641, 4109].
* [cite_start]**Fase de Julgamento**: Ao final da exploração, o jogador é apresentado a uma lista de suspeitos e deve fazer uma acusação[cite: 4113].
* [cite_start]**Verificação de Evidências**: O sistema utiliza a Tabela Hash para contar quantas pistas coletadas incriminam o suspeito acusado e determina se a acusação é válida (requer no mínimo 2 pistas para ser bem-sucedida)[cite: 4114].
* **Gerenciamento de Memória**: Todo o espaço alocado dinamicamente (`malloc`) para as estruturas de dados é liberado (`free`) ao final do programa para evitar vazamento de memória.

## 🔧 Como Compilar e Executar

1.  **Pré-requisitos**: É necessário ter um compilador C (como o GCC) instalado em seu sistema.
2.  **Salvar o Código**: Salve o código-fonte em um arquivo, por exemplo, `detective_quest.c`.
3.  **Compilar**: Abra um terminal, navegue até o diretório do arquivo e execute o comando:
    ```bash
    gcc detective_quest.c -o detective_quest
    ```
4.  **Executar**: Rode o programa compilado:
    * No Linux/macOS: `./detective_quest`
    * No Windows: `.\detective_quest.exe`

O jogo começará no terminal, e você poderá interagir digitando os comandos solicitados.

## 🎮 Exemplo de Uso
