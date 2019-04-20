#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define tamanho 8     // Define o tamanho padrão da matriz

// Procedimento para armazenar parâmetros passados do estágio anterior
void armazenarDados(char *html, int *linha1, int *coluna1, int *pontuacao, char *nome);

// Procedimentos para construção da página HTML
void construirHead(); 
void construirBody();

// Procedimento para a impressão da tabela do jogo na página HTML, passando os parâmetros para o próximo estágio
void preencherImagens( int jogo[tamanho][tamanho], char *nome, int linha1, int coluna1, int pontuacao );

// Subrotina para limitar escolha da segunda peça para as 4 opções possíveis para troca (direita, esquerda, em cima e embaixo)
// Retorna 0 se a peça for uma das 4 opções e 1 se não for.
int verificarTroca( int linha1, int coluna1, int linha2, int coluna2 );

int main() {
    int jogo[tamanho][tamanho], i, j;
    int linha1, coluna1, pontuacao = 1;
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
	armazenarDados(html, &linha1, &coluna1, &pontuacao, nome);

	// Copiando o nome digitado pelo usuário para a criação de um arquivo .txt com o mesmo nome
	strcpy( arq, nome);
        strcat( arq, ".txt" );

        arquivo = fopen( arq, "r" );

        if( arquivo == NULL ) {
            printf( "<br> Erro na passagem dos dados\n" );
            return 0;
        }

	// Recuperando matriz do arquivo salvo no estágio anterior
        for( i = 0; i < tamanho; i++ )
            for( j = 0; j < tamanho; j++ ) {
                aux = fgetc( arquivo );
                jogo[i][j] = atoi( &aux );
            }

        preencherImagens( jogo, nome, linha1, coluna1, pontuacao );

        printf( "<br><br>" );
        printf( "<h1> <font color=\"#97bdb8\"> Pontuação: %d </font> </h1>", pontuacao );
    }

    fclose( arquivo );
    return 0;
}

void armazenarDados(char *html, int *linha1, int *coluna1, int *pontuacao, char *nome) {
    int i = 0;
    char aux;

    // Passando os parâmetros da URL para variáveis específicas
    aux = *html;
    *linha1 = atoi( &aux );
    html++;
    aux = *html;
    *coluna1 = atoi( &aux );
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
    printf( "<link rel=\"icon\" type=\"image/png\" href=\"../imagens/site.png\" />\n" );
    printf( "<title> Code Crush </title>\n" );
    printf( "</head>\n" );
}

void construirBody() {
    printf( "<body>\n" );
    printf( "<div id=\"body\" />\n" );
    printf( "<br> <br> <br> <br> <br> <br>\n" );
    printf( "<div id=\"jogo\">\n" );
}

void preencherImagens( int jogo[tamanho][tamanho], char *nome, int linha1, int coluna1, int pontuacao ) {
    int i, j;

    printf( "<table>\n" );

    for( i = 0; i < tamanho; i++ ) {
        printf( "<tr>\n" );

        for( j = 0; j < tamanho; j++ ) {
            printf( "<td>\n" );

	    // Conferindo se a peça é uma das 4 a serem escolhidas ou não
	    // Se for, a peça poderá ser clicada e irá para o próximo estágio
	    // Se não, só será mostrada a imagem da peça e o usuário não poderá clicá-la
            if( !verificarTroca( linha1, coluna1, i, j ) ) {
                printf( "<form action=\"stage2.cgi\">" );
	       	printf( "<input type=\"image\" src=\"../imagens/%dcolor.png\" name=\"%d%d%d%d%d.%s\" value=\"submit\">\n", jogo[i][j], linha1, coluna1, i, j, pontuacao, nome );
		printf( "</form>\n" );
            } else
		if (i == linha1 && j == coluna1) 
                    printf( "<img src=\"../imagens/%dcolor.png\">\n", jogo[i][j] );
		else
                    printf( "<img src=\"../imagens/%d.png\">\n", jogo[i][j] );

            printf( "</td>\n" );
        }

        printf( "</tr>\n" );
    }

    printf( "</table>\n" );
}

int verificarTroca( int linha1, int coluna1, int linha2, int coluna2 ) {
    if( linha2 == linha1 + 1 || linha2 == linha1 - 1 )
        if( coluna2 == coluna1 )
            return 0;

    if( coluna2 == coluna1 + 1 || coluna2 == coluna1 - 1 )
        if( linha2 == linha1 ) 
            return 0;

    return 1;
}
