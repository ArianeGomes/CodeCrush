#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define tamanho 8     // Define o tamanho padrão da matriz
#define qtdePecas 5   // Define a quantidade de peças diferentes que existirão no jogo

// Procedimento para armazenar parâmetros passados do index.html
void armazenarDados(char *html, char *nome);

// Procedimentos para construção da página HTML
void construirHead();
void construirBody();

// Procedimento para a impressão da tabela do jogo na página HTML, passando os parâmetros para o próximo estágio
void preencherImagens( int jogo[tamanho][tamanho], char *nome, int pontuacao );

// Procedimento para a criação de uma matriz random para o jogo
void criarJogo( int jogo[tamanho][tamanho] );

// Subrotina para verificar se existem jogadas disponíveis
// Retorna 1 se existir jogada. Caso contrário, retorna 0
int continuarJogo( int jogo[tamanho][tamanho] );

// Procedimentos que alteram linhas e colunas que possuem 3 ou mais peças iguais (substituindo-as)
void alteraLinha( int matriz[tamanho][tamanho] );
void alteraColuna( int matriz[tamanho][tamanho] );

// Subrotina para verificar se existem 3 ou mais peças iguais em sequência
int compararPecas( int matriz[tamanho][tamanho] );

int main() {
    int jogo[tamanho][tamanho], i = 0, j = 0, pontuacao = 0;
    char *html, nome[29], arq[29];
    FILE *arquivo;

    srand(time(NULL));
    printf( "%s%c%c\n", "Content-Type:text/html;charset=UTF-8", 13, 10 );
    html = getenv( "QUERY_STRING" );

    construirHead();
    construirBody();

    criarJogo( jogo );

    if( html == NULL ) {
        printf( "<br> Erro na passagem dos dados\n" );
        return 0;
    } else {
	armazenarDados(html, nome);

	// Copiando o nome digitado pelo usuário para a criação de um arquivo .txt com o mesmo nome
	strcpy( arq, nome);
        strcat( arq, ".txt" );

        arquivo = fopen( arq, "w" );

        if( arquivo == NULL ) {
            printf( "<br> Erro na passagem dos dados\n" );
            return 0;
        }

	// Verificando se existem jogadas para serem feitas de início, para que o jogo comece sem nenhuma peça repetida
	// Caso existirem, as jogadas serão feitas antes de exibir a primeira matriz para o usuário
        if( continuarJogo( jogo ) ) {
            while( compararPecas( jogo ) ) {
                alteraLinha( jogo );
                alteraColuna( jogo );
            }

	    // Salvando a matriz em um arquivo para que a mesma seja usada no próximo estágio
            for( i = 0; i < tamanho; i++ ) 
                for( j = 0; j < tamanho; j++ )
                    fprintf( arquivo, "%d", jogo[i][j] );

            preencherImagens( jogo, nome, pontuacao );

            printf( "<br><br>" );
            printf( "<h1> <font color=\"#97bdb8\"> Pontuação: %d </font> </h1>", pontuacao );
        } else {
	    // Se não houver nenhuma jogada disponível, redireciona para página perdeu.html
            preencherImagens( jogo, nome, pontuacao );
            printf( "<meta http-equiv=\"refresh\" content=\"3; url=../perdeu.html\">" );
	}
    }
    printf( "</body>" );
    printf( "</html>" );
    fclose( arquivo );

    return 0;
}

void armazenarDados(char *html, char *nome) {
    int i = 0;

    html += 5;

    while( *html != '&' ) {
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
    int i, j, k;
    printf( "<table>\n" );
    for( i = 0; i < tamanho; i++ ) {
        printf( "<tr>\n" );
        for( j = 0; j < tamanho; j++ ) {
            printf( "<td>\n" );
	    // Criando botões para cada célula da tabela e passando por parâmetro a linha, a coluna, a pontuação atual e o nome
            printf( "<form action=\"stage1.cgi\">" );
            printf( "<input type=\"image\" src=\"../imagens/%d.png\" name=\"%d%d%d.%s\" value=\"submit\">\n", jogo[i][j], i, j, pontuacao, nome );
            printf( "</form>\n" );
            printf( "</td>\n" );
        }
        printf( "</tr>\n" );
    }
    printf( "</table>\n" );
}

void criarJogo( int jogo[tamanho][tamanho] ) {
    int i, j;

    for( i = 0; i < tamanho; i++ )
        for( j = 0; j < tamanho; j++ )
            jogo[i][j] = rand() % qtdePecas;
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

void alteraLinha( int matriz[tamanho][tamanho] ) {
    int linha, coluna, contador;
    int i, j, k;

    for( linha = 0; linha < tamanho; linha++ )
        for( coluna = 0; coluna < tamanho - 2; coluna++ ) {

            contador = 1;
            i = coluna;
		
	    // A partir de 3 peças iguais na linha, em sequência, faz a troca de peças
            while( matriz[linha][coluna] == matriz[linha][coluna + 1] && matriz[linha][coluna + 1] == matriz[linha][coluna + 2] ) {

		// Conta quantas peças iguais existem em sequência
                while( matriz[linha][i] == matriz[linha][i + 1] ) {
                    contador++;
                    i++;
                }

		// Abaixa todas as outras peças que estiverem em cima das que são iguais
                for( j = linha; j > 0; j-- )
                    for( k = coluna; k < coluna + contador; k++ )
                        matriz[j][k] = matriz[j - 1][k];

                // Gera números randoms para as peças do topo, a fim de completar a matriz novamente
                for( k = coluna; k < coluna + contador; k++ )
                    matriz[0][k] = rand() % qtdePecas;
            }
        }
    
}

void alteraColuna( int matriz[tamanho][tamanho] ) {
    int linha, coluna, contador;
    int i, j, aux;

    for( coluna = 0; coluna < tamanho; coluna++ )
        for( linha = 0; linha < tamanho - 2; linha++ ) {

            contador = 0;
            i = linha;

	    // A partir de 3 peças iguais na coluna, em sequência, faz a troca de peças
            while( matriz[linha][coluna] == matriz[linha + 1][coluna] && matriz[linha + 1][coluna] == matriz[linha + 2][coluna] ) {

                aux = matriz[linha][coluna];

		// Conta quantas peças iguais existem em sequência
                while( matriz[i][coluna] == matriz[i + 1][coluna] ) {
                    contador++;
                    i++;
                }

		// Substitui as peças iguais pelas de cima e gera números randoms para completar a matriz
                while( matriz[linha + contador][coluna] == aux ) {
                    for( j = linha + contador; j > 0; j-- ) 
                        matriz[j][coluna] = matriz[j - 1][coluna];
                    
                    matriz[0][coluna] = rand() % qtdePecas;
                }
            }
        }
    
}

int compararPecas( int matriz[tamanho][tamanho] ) {
    int linha, coluna;

    for( linha = 0; linha < tamanho; linha++ ) 
        for( coluna = 0; coluna < tamanho - 2; coluna++ ) 
            if( matriz[linha][coluna] == matriz[linha][coluna + 1] && matriz[linha][coluna + 1] == matriz[linha][coluna + 2] )
                return 1;

    for( coluna = 0; coluna < tamanho; coluna++ ) 
        for( linha = 0; linha < tamanho - 2; linha++ ) 
            if( matriz[linha][coluna] == matriz[linha + 1][coluna] && matriz[linha + 1][coluna] == matriz[linha + 2][coluna] )
                return 1;

    return 0;
}
