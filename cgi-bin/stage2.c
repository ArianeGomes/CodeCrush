#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define tamanho 8     // Define o tamanho padrão da matriz
#define qtdePecas 5   // Define a quantidade de peças diferentes que existirão no jogo

// Procedimento para armazenar parâmetros passados do estágio anterior
void armazenarDados(char *html, int *linha1, int *coluna1, int *linha2, int *coluna2, int *pontuacao, char *nome);

// Procedimentos para construção da página HTML
void construirHead();
void construirBody();

// Procedimento para a impressão da tabela do jogo na página HTML, passando os parâmetros para o próximo estágio
void preencherImagens( int jogo[tamanho][tamanho], char *nome, int pontuacao );

// Subrotina para verificar se existem jogadas disponíveis
// Retorna 1 se existir jogada. Caso contrário, retorna 0
int continuarJogo( int jogo[tamanho][tamanho] );

// Procedimentos que alteram linhas e colunas que possuem 3 ou mais peças iguais (substituindo-as)
void alteraLinha( int matriz[tamanho][tamanho], int *pontuacao );
void alteraColuna( int matriz[tamanho][tamanho], int *pontuacao );

// Subrotina para verificar se existem 3 ou mais peças iguais em sequência
int compararPecas( int matriz[tamanho][tamanho] );

// Subrotina para trocar peças, caso existam 3 peças iguais em sequência
void trocarPeca( int jogo[tamanho][tamanho], int linha1, int coluna1, int linha2, int coluna2, int *pontuacao );

int main() {
    int jogo[tamanho][tamanho], i = 0, j = 0;
    int pontuacao = 0, linha1, coluna1, linha2, coluna2;
    char *html, aux, nome[29], arq[29];
    FILE *arquivo;

    printf( "%s%c%c\n", "Content-Type:text/html;charset=UTF-8", 13, 10 );
    html = getenv( "QUERY_STRING" );
    construirHead();
    construirBody();

    if( html == NULL ) {
        printf( "<br> Erro na passagem dos dados\n" );
        return 0;
    } else {	
	armazenarDados(html, &linha1, &coluna1, &linha2, &coluna2, &pontuacao, nome);

	// Copiando o nome digitado pelo usuário para a criação de um arquivo .txt com o mesmo nome
	strcpy( arq, nome);
        strcat( arq, ".txt" );

        arquivo = fopen( arq, "r" );

        if( arquivo == NULL ) {
            printf( "<br> Erro na passagem dos dados\n" );
            return 0;
        }

        for( i = 0; i < tamanho; i++ )
            for( j = 0; j < tamanho; j++ ) {
                aux = fgetc( arquivo );
                jogo[i][j] = atoi( &aux );
            }

        remove( arq );
        arquivo = fopen( arq, "w" );

	// Verificando se existem jogadas para serem feitas de início, para que o jogo comece sem nenhuma peça repetida
	// Caso existirem, as jogadas serão feitas antes de exibir a primeira matriz para o usuário
        if( continuarJogo( jogo ) ) {
            trocarPeca( jogo, linha1, coluna1, linha2, coluna2, &pontuacao );

	    // Salvando a matriz em um arquivo para que a mesma seja usada no próximo estágio
            for( i = 0; i < tamanho; i++ ) 
                for( j = 0; j < tamanho; j++ ) 
                    fprintf( arquivo, "%d", jogo[i][j] );

            preencherImagens( jogo, nome, pontuacao );
        } else {
	    // Se não houver nenhuma jogada disponível, redireciona para página perdeu.html
            preencherImagens( jogo, nome, pontuacao );
            printf( "<meta http-equiv=\"refresh\" content=\"2; url=../perdeu.html\">" );
        }
    }
    printf( "<br><br>" );
    printf( "<h1> <font color=\"#97bdb8\"> Pontuação: %d </font> </h1>", pontuacao );
    printf( "</div>\n" );
    printf( "</body>\n" );
    printf( "</html>\n" );

    fclose( arquivo );

    return 0;
}

void armazenarDados(char *html, int *linha1, int *coluna1, int *linha2, int *coluna2, int *pontuacao, char *nome) {
    int i = 0;
    char aux;

    aux = *html;
    *linha1 = atoi( &aux );

    html++;
    aux = *html;
    *coluna1 = atoi( &aux );

    html++;
    aux = *html;
    *linha2 = atoi( &aux );

    html++;
    aux = *html;
    *coluna2 = atoi( &aux );

    html++;
    *pontuacao = atoi( html );

    while( *html != '.' )
        html++;
    html++;

    while( *html != '.' ) {
        nome[i] = *html;
        i++;
        html++;
    }

    nome[i] = '\0';
}

void construirHead() {
    printf( "<head lang=\"pt-br\">\n" );
    printf( "<meta charset=\"utf-8 \" />\n" );
    printf( "<link href=\"../style.css\" rel=\"stylesheet\" type=\"text/css\" />\n" );
    printf( "<link rel=\"icon\" type=\"image/png\"href=\"../imagens/site.png\" />\n" );
    printf( "<title> Code Crush </title>\n" );
    printf( "</head>\n" );
}

void construirBody() {
    printf( "<body>\n" );
    printf( "<div id=\"body\" />\n" );
    printf( "<br> <br> <br> <br> <br> <br>\n" );
    printf( "<div id=\"jogo\">\n" );
}

void preencherImagens( int jogo[tamanho][tamanho], char *nome, int pontuacao ) {
    int i, j;

    // Verificando se o usuário venceu o jogo
    // Em caso positivo, imprime tabela não clicável e redireciona para página ganhou.html
    if( pontuacao >= 1000 ) {
    	printf( "<table>\n" );

        for( i = 0; i < tamanho; i++ ) {
            printf( "<tr>\n" );

            for( j = 0; j < tamanho; j++ ) {
            	printf( "<td>\n" );
                printf( "<img src=\"../imagens/%d.png\"\n", jogo[i][j] );
                printf( "</td>\n" );
            }

            printf( "</tr>\n" );
        }

        printf( "</table>\n" );
        printf( "<meta http-equiv=\"refresh\" content=\"2; url=../ganhou.html\">" );
    } else {
	// Em caso negativo, imprime tabela com imagens clicáveis e envia parâmetros para o próximo estágio
    	printf( "<table>\n" );

        for( i = 0; i < tamanho; i++ ) {
            printf( "<tr>\n" );

            for( j = 0; j < tamanho; j++ ) {
            	printf( "<td>\n" );
                printf( "<form action=\"stage1.cgi\">" );
                printf( "<input type=\"image\" src=\"../imagens/%d.png\" name=\"%d%d%d.%s\" value=\"submit\">\n", jogo[i][j], i, j, pontuacao, nome );
                printf( "</form>\n" );
                printf( "</td>\n" );
            }

            printf( "</tr>\n" );
        }

        printf( "</table>\n" );
    }
}

int continuarJogo( int jogo[tamanho][tamanho] ) {
    int linha, coluna;

    for( linha = 0; linha < tamanho; linha++ )
        for( coluna = 3; coluna < tamanho; coluna++ )
            if( jogo[linha][coluna] == jogo[linha][coluna - 2] && jogo[linha][coluna] == jogo[linha][coluna - 3] )
                return 1;

    for( linha = 0; linha < tamanho; linha++ )
        for( coluna = 0; coluna < tamanho - 3; coluna++ )
            if( jogo[linha][coluna] == jogo[linha][coluna + 2] && jogo[linha][coluna] == jogo[linha][coluna + 3] )
                return 1;

    for( linha = 3; linha < tamanho; linha++ )
        for( coluna = 0; coluna < tamanho; coluna++ )
            if( jogo[linha][coluna] == jogo[linha - 2][coluna] && jogo[linha][coluna] == jogo[linha - 3][coluna] ) 
                return 1;

    for( linha = 0; linha < tamanho - 3; linha++ ) 
        for( coluna = 0; coluna < tamanho; coluna++ ) 
            if( jogo[linha][coluna] == jogo[linha + 2][coluna] && jogo[linha][coluna] == jogo[linha + 3][coluna] )
                return 1;

    return 0;
}

void alteraLinha( int jogo[tamanho][tamanho], int *pontuacao ) {
    int linha, coluna, contador;
    int i, j, k;

    for( linha = 0; linha < tamanho; linha++ )
        for( coluna = 0; coluna < tamanho - 2; coluna++ ) {

            contador = 1;
            i = coluna;

	    // A partir de 3 peças iguais na linha, em sequência, faz a troca de peças
            while( jogo[linha][coluna] == jogo[linha][coluna + 1] && jogo[linha][coluna + 1] == jogo[linha][coluna + 2] ) {

		// Conta quantas peças iguais existem em sequência
                while( jogo[linha][i] == jogo[linha][i + 1] ) {
                    contador++;
                    i++;
                }

		// Multiplica por 10 a quantidade de peças que serão eliminadas e soma à pontuação do usuário
                *pontuacao += contador * 10;

		// Abaixa todas as outras peças que estiverem em cima das que são iguais
                for( j = linha; j > 0; j-- ) 
                    for( k = coluna; k < coluna + contador; k++ ) 
                        jogo[j][k] = jogo[j - 1][k];

                // Gera números randoms para as peças do topo, a fim de completar a matriz novamente
                for( k = coluna; k < coluna + contador; k++ )
                    jogo[0][k] = rand() % qtdePecas;
            }
        }
}

void alteraColuna( int jogo[tamanho][tamanho], int *pontuacao ) {
    int linha, coluna, contador;
    int i, j, aux;

    for( coluna = 0; coluna < tamanho; coluna++ ) 
        for( linha = 0; linha < tamanho - 2; linha++ ) {

            contador = 0;
            i = linha;

	    // A partir de 3 peças iguais na coluna, em sequência, faz a troca de peças
            while( jogo[linha][coluna] == jogo[linha + 1][coluna] && jogo[linha + 1][coluna] == jogo[linha + 2][coluna] ) {

                aux = jogo[linha][coluna];

		// Conta quantas peças iguais existem em sequência
                while( jogo[i][coluna] == jogo[i + 1][coluna] ) {
                    contador++;
                    i++;
                }

		// Multiplica por 10 a quantidade de peças que serão eliminadas e soma à pontuação do usuário
                *pontuacao += (contador + 1) * 10;

		// Substitui as peças iguais pelas de cima e gera números randoms para completar a matriz
                while( jogo[linha + contador][coluna] == aux ) {
                    for( j = linha + contador; j > 0; j-- )
                        jogo[j][coluna] = jogo[j - 1][coluna];

                    jogo[0][coluna] = rand() % qtdePecas;
                }
            }
        }
}

int compararPecas( int jogo[tamanho][tamanho] ) {
    int linha, coluna;

    for( linha = 0; linha < tamanho; linha++ )
        for( coluna = 0; coluna < tamanho - 2; coluna++ )
            if( jogo[linha][coluna] == jogo[linha][coluna + 1] && jogo[linha][coluna + 1] == jogo[linha][coluna + 2] )
                return 1;

    for( coluna = 0; coluna < tamanho; coluna++ ) 
        for( linha = 0; linha < tamanho - 2; linha++ ) 
            if( jogo[linha][coluna] == jogo[linha + 1][coluna] && jogo[linha + 1][coluna] == jogo[linha + 2][coluna] )
                return 1;

    return 0;
}

void trocarPeca( int jogo[tamanho][tamanho], int linha1, int coluna1, int linha2, int coluna2, int *pontuacao ) {
    int alce;

    // Troca valores das peças selecionadas
    alce = jogo[linha1][coluna1];
    jogo[linha1][coluna1] = jogo[linha2][coluna2];
    jogo[linha2][coluna2] = alce;

    // Verificar se existem 3 ou mais peças iguais em sequência, após a troca ser feita
    // Se houver, substitui as peças até ter menos de 3 peças iguais em sequência
    if( compararPecas( jogo ) )
        while( compararPecas( jogo ) ) {
            alteraLinha( jogo, pontuacao );
            alteraColuna( jogo, pontuacao );
        }
    else {
        // Se não houver, os valores das peças selecionadas são trocados novamente (não altera a matriz)
        alce = jogo[linha2][coluna2];
        jogo[linha2][coluna2] = jogo[linha1][coluna1];
        jogo[linha1][coluna1] = alce;
    }
}