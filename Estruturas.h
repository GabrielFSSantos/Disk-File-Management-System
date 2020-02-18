#ifndef ESTRUTURAS_H_INCLUDED
#define ESTRUTURAS_H_INCLUDED
#define T 40

// ESTRUTURAS

typedef struct Disco // Vetor de Strings, que Armazena os Arquivos.
{
    char Vetor[T][3]; //Sendo T o Tamanho do Vetor e 3 a Quantidade de Caracteres Armazenadas em Cada Posi��o

}TDisco;

typedef struct Setor // Guarda Intervalos do Disco e o Endere�o do Proximo Setor.
{
    int inicio;              // Inicio do Intervalo.
    int fim;                 // Fim do Intervalo.
    struct Setor *proxSetor; // Ponteiro para o Proximo Setor.

}TSetor;

typedef struct ListaSetores // Uma Lista do Tipo Setor, que Armazena 2 Endere�os de Setor, o Primeiro e o Ultimo da Lista e a Quantidade de Setores Disponiveis.
{
    TSetor* primeiro; // Guarda o Endere�o do Primeiro Setor da Lista (Cabe�a).
    TSetor* ultimo;   // Guarda o Endere�o do Ultimo Setor da Lista.
    int tam;          // Mantem Atualizado a Quantidade de Setores Disponiveis na Lista.

}TListaSetores;

typedef struct Arquivo // Guarda as Informa��es do Arquivo o seu Conteudo e o Endere�o do Proximo Arquivo.
{
    char nome[20];               // String para Armazenar o Nome.
    int id;                      // Int que Armazena qual o Indice do Arquivo.
    int tamanho;                 // Numero de Caracteres que o Arquivo Possue, ou seja, seu Tamanho em Disco.
    TListaSetores Conteudo;      // Lista de Setores que Armazena os Intervalos onde se Encontra o Arquivo em Disco.
    struct Arquivo *proxArquivo; // Ponteiro para o Proximo Arquivo.

}TArquivo;

typedef struct ListaArquivos // Guarda o Primeiro e o Ultimo Endere�o de Celula de uma Lista.
{
    TArquivo* primeiro; // Guarda o Endere�o do Primeiro Arquivo da Lista (Cabe�a).
    TArquivo* ultimo;   // Guarda o Endere�o do Ultimo Arquivo da Lista.
    int indice;         // Mantem Atualizado a Quantidade de Arquivos que � na Lista.

}TListaArquivos;


// FUN��ES BASE SETORES
void FLVaziaSetores(TListaSetores *Lista); // Inicializa a Lista de Setores e Cria a Primeira Celula (Cabe�a).
int VaziaSetores(TListaSetores Lista); // Verifica se a Lista de Setores esta Vazia.
void CriarSetores(TDisco *Disco, TListaSetores *Lista); // Divide o Disco em Setores, cada Setor ter� 2 posi��es do Disco.
void UsarSetores(TListaSetores *Lista, TArquivo *Arquivo); // Seleciona os Setores Necessarios para um Arquivo, de Forma Aleatoria.
void LibeSetores(TListaSetores *Lista, TArquivo *Arquivo); // Retorna os Vetores de um Arquivo para Lista de Setores Livres.
void LimpaListaSetores(TListaSetores *Setores); // Limpa a Lista de Setores (Apaga Todos Setore).


// FUN��ES BASE ARQUIVO
void FLVaziaArquivo(TListaArquivos *Lista); // Inicializa a Lista de Arquivos e o Indice e Cria a Primeira Celula (Cabe�a).
int VaziaArquivo(TListaArquivos Lista); // Verifica se a Lista de Arquivos esta Vazia.
void CriarArquivo(TListaArquivos *Lista); // Cria uma nova Celula de Arquivo e Ela Recebe seu ID.
void GravarArquivo(TDisco *Disco, TArquivo *Arquivo); //Grava o ID do Arquivo no Disco, nos seus Respectivos Setores.
TArquivo* PesquisarArquivo(TListaArquivos *Lista, char *nome); // Pesquisa por um Arquivo no Disco e Retorna seu Endere�o.
void RemoverArquivo(TListaArquivos *Lista, TArquivo *Arquivo); // Apaga o Arquivo do Disco e Torna o seu Intervalo Livre.
void ImprimirArquivo(TArquivo *Arquivo); // Imprime as Informa��es de um Arquivo
void ImprimirListaArquivos(TListaArquivos *Lista); // Imprime Todos os Arquivos


// FUN��ES BASE DISCO
void DesfragmentarDisco(TDisco *Disco, TListaSetores *Setores, TListaArquivos *Arquivos); // Realoca os Arquivos no Disco de Forma Organizada.
void FormatarDisco(TDisco *Disco, TListaSetores *Setores, TListaArquivos *Arquivos); // Limpa o Disco Apagando Todos Arquivos
void ImprimirDisco(TDisco *Disco);

#endif // ESTRUTURAS_H_INCLUDED
