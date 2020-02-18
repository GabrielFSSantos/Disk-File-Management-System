#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include "Estruturas.h"


// FUN��ES BASE SETORES

void FLVaziaSetores(TListaSetores *Lista) // Inicializa a Lista de Setores e Cria a Primeira Celula (Cabe�a).
{
    Lista->primeiro = (TSetor *) malloc(sizeof(TSetor)); // (TSetor *) � um Cast, for�a a fun��o da direita a retornar o tipo dele.
    Lista->ultimo = Lista->primeiro;                     // Faz o Ponteiro Ultimo Receber o Endere�o que esta no Primeiro.
    Lista->primeiro->proxSetor = NULL;                   // Aterramento do Prox da Primeira Cela para n�o Haver conflito de endere�o.
    Lista->tam = T/2;                                    // Define a Quantidade de Setores que vai haver para o Disco.
}

int VaziaSetores(TListaSetores Lista) // Verifica se a Lista de Setores esta Vazia.
{
    return (Lista.primeiro == Lista.ultimo); // Verifica se Somente Possui a Celula Cabe�a, ou Seja a Lista esta Vazia.
}

void CriarSetores(TDisco *Disco, TListaSetores *Lista) // Divide o Disco em Setores, cada Setor ter� 2 posi��es do Disco.
{
    int i,j; // Variaveis para Rodar o La�o e Atribuir o Intervalo.

    for(j=0,i=0;i<T/2;i++,j+=2) //Cria (T/2) Setores para Lista de Setores Livres.
    {
        Lista->ultimo->proxSetor = (TSetor *) malloc(sizeof(TSetor)); // (TSetor *) � um Cast, for�a a fun��o da direita a retornar o tipo dele.
        Lista->ultimo = Lista->ultimo->proxSetor;                     // Faz o Ultimo Receber o Novo Ultimo.
        Lista->ultimo->inicio = j;                                    // Da o Inicio de Intervalo para o Setor.
        Lista->ultimo->fim = j+1;                                     // Da o Fim de Intervalo para o Setor.
        Lista->ultimo->proxSetor = NULL;                              // Faz o Aterramento do Prox do Ultimo.
    }

}

void UsarSetores(TListaSetores *Lista, TArquivo *Arquivo) // Seleciona os Setores Necessarios para um Arquivo, de Forma Aleatoria.
{
    time_t t;                  // Variavel do Tipo Time, que muda de a cordo com o Tempo Atual para sempre ter seeds de sorteio diferentes.
    int i,j;                   // Int's Para Rodar os La�os de Repeti��o.
    int falta;                 // Variavel que Contabiliza o Quanto falta do Tamanho do Arquivo que Ainda Ncessita de Setores.
    TSetor *aux1;              // Varialvel Auxiliar para Percorrer a Lista de Setores Livres.
    TSetor *aux2;              // Varialvel Auxiliar para Acompanhar a Variavel aux1.

    falta = Arquivo->tamanho;  // Falta Recebe o Tamanho do Arquivo.
    aux1 = Lista->primeiro;    // aux1 Recebe a Cabe�a da Lista de Setores Vazios.


    if (VaziaSetores(*Lista) || aux1 == NULL || aux1->proxSetor == NULL) // Verifica se Lista esta Vazia.
    {
        printf("\nO Disco nao Possue Espaco Suficiente...\n\n");
        return;
    }


    for(i=0;i<ceil(ceil((float)(falta/3))/2);i++) // Verifica se a Lista tem Espa�o para o Arquivo.
    {
        aux1 = aux1->proxSetor; // aux1 Salta para o Proximo Setor.

        if(aux1 == NULL) // Verifica se Chegou ao Final da Lista.
        {
            printf("\nO Disco nao Possue Espaco Suficiente...\n\n");
            return;
        }
    }


    //Faz o Arquivo Receber os Setores a ser Armazenado
    while(falta>0) //Conta quantos Caracterem ainda faltam armazenar
    {
        srand((unsigned) time(&t)); // Fun��o Para toda vez que rodarmos o programa, a rand() pega um n�mero de tempo diferente.
        j = rand()%Lista->tam;      // Sorteia um Numero dentre a quantidade de Setores.
        aux1 = Lista->primeiro;     // aux1 Recebe a Cabe�a da Lista de Setores Vazios.

        for(i=0; i<=j && aux1->proxSetor!=NULL ; i++) // Escolhe um Setor Aleatorio na Lista
        {
            aux2 = aux1;                              // aux2 recebe aux1 para Estar Sempre o Acompanhando.
            aux1 = aux1->proxSetor;                   // aux1 Salta para o Proximo Setor.
        }


        if(aux1!=NULL) //Faz o Arquivo Receber um Setor
        {
            aux2->proxSetor = aux1->proxSetor; // Retira o Setor da Lista de Setores Livres.

            if(aux2->proxSetor==NULL) // Se for Tirada a Ultima Celula da Lista Atualiza o Ponteiro Ultimo.
            {
                Lista->ultimo = aux2; // Faz o Ponteiro Ultimo Receber a Nova Celula que � a Ultima, ao Tirar a que era Anteriormente.
            }

            aux1->proxSetor = NULL;   // Aterra o Prox do Setor que o Arquivo vai Receber.

            Arquivo->Conteudo.ultimo->proxSetor = aux1; // Adiciona o Setor no Arquivo.
            Arquivo->Conteudo.ultimo = aux1;            // E Atualiza o Ponteiro Ultimo dos Setores do Arquivo.

            falta = falta-6; // Faz Falta -6 Pois cada Setor Recebido Comporta 6 Caracteres
            Lista->tam--;    // Atualiza a Quantidade de Setores Dispon�veis
        }

        if(falta == (Arquivo->tamanho)-6) // O Prox da Cabe�a da Lista de Setores do Arquivo Recebe o Endere�o do Primeiro Setor.
        {
            Arquivo->Conteudo.primeiro->proxSetor = Arquivo->Conteudo.ultimo; // Atualiza o Prox da Cabe�a da Lista de Setores do Arquivo.
        }

    }

    aux1 = NULL;    // Faz o Aterramento do Auxiliar.
    aux2 = NULL;    // Faz o Aterramento do Auxiliar.
    free(aux1);     // Faz o Liberamento do Auxiliar.
    free(aux2);     // Faz o Liberamento do Auxiliar.

}

void LibeSetores(TListaSetores *Lista, TArquivo *Arquivo) // Retorna os Vetores de um Arquivo para Lista de Setores Livres.
{
    TSetor *aux; // Auxiliar para Percorrer Lista de Setores do Arquivo.

    aux = Arquivo->Conteudo.primeiro; // Auxiliar Recebe a Cabe�a da Lista de Setores do Arquivo.

    while(aux->proxSetor != NULL) // Faz o Auxliar Receber o Endere�o do Ultimo Setor do Arquivo.
    {
        aux = aux->proxSetor; // aux Salta para o Proximo Setor.
        Lista->tam++;         // Atualiza a Quantidade de Setores
    }

    Lista->ultimo->proxSetor = Arquivo->Conteudo.primeiro->proxSetor; // Faz a Lista de Setores Vazios Receber os Setores do Arquivo

    Lista->ultimo = aux;                                              // Faz o Ultimo da Lista de Setores vazios Receber o Ultimo Setor Colocado

    Arquivo->Conteudo.ultimo = Arquivo->Conteudo.primeiro;            // Torna a Lista de Setores do Arquivo Somente com a Cabe�a

    Arquivo->Conteudo.primeiro->proxSetor = NULL;                     // Faz o Aterramento da Cabe�a.

}

void LimpaListaSetores(TListaSetores *Setores) // Limpa a Lista de Setores (Apaga Todos Setore).
{
    TSetor *aux; // Auxiliar para Percorrer a Lista de Setores.

    while (Setores->primeiro->proxSetor != NULL) // La�o para Apagar Todos Setores (Menos a Cabe�a).
    {
        aux = Setores->primeiro->proxSetor;                // O Auxiliar Recebe o Primeiro Setor.
        Setores->primeiro->proxSetor = aux->proxSetor;     // O Primeiro Setor � Retirado da Lista.
        aux->proxSetor = NULL;                             // O Prox do Primeiro Setor � Aterrado.
        free(aux);                                         // O Primeiro Setor � Liberado.
    }

    Setores->ultimo = Setores->primeiro;                    // Atualiza o Ponteiro Ultimo da Lista para a Cabe�a.

}


// FUN��ES BASE ARQUIVO

void FLVaziaArquivo(TListaArquivos *Lista) // Inicializa a Lista de Arquivos e o Indice e Cria a Primeira Celula (Cabe�a).
{
    Lista->indice = 49;                                      // Indice Usado para colocar um ID para cada Arquivo, Come�a em 49 para Ficar de Acordo com a Tabela ASCII
    Lista->primeiro = (TArquivo *) malloc(sizeof(TArquivo)); // (TArquivo *) � um Cast, for�a a fun��o da direita a retornar o tipo dele.
    Lista->ultimo = Lista->primeiro;                         // Faz o Ultimo Receber o Novo Ultimo.
    Lista->primeiro->proxArquivo = NULL;                     // Aterramento para n�o aver conflito de endere�o.
}

int VaziaArquivo(TListaArquivos Lista) // Verifica se a Lista de Arquivos esta Vazia.
{
    return (Lista.primeiro == Lista.ultimo); // Verifica se Somente Possui a Celula Cabe�a, ou Seja a Lista esta Vazia.
}

void CriarArquivo(TListaArquivos *Lista) // Cria uma nova Celula de Arquivo e Ela Recebe seu ID.
{
    Lista->ultimo->proxArquivo = (TArquivo *) malloc(sizeof(TArquivo)); // (TArquivo *) � um Cast, for�a a fun��o da direita a retornar o tipo dele.
    Lista->ultimo = Lista->ultimo->proxArquivo;                         // Faz o Ultimo Receber o Novo Ultimo.
    Lista->ultimo->id = Lista->indice;                                  // Adiciona um ID para o Arquivo.
    Lista->ultimo->proxArquivo = NULL;                                  // Faz o Aterramento do Prox do Ultimo.
    Lista->indice++;                                                    // Incrementa��o do Indice para os Proximos ID's Arquivos.
}

void GravarArquivo(TDisco *Disco, TArquivo *Arquivo) //Grava o ID do Arquivo no Disco, nos seus Respectivos Setores.
{
    int i,j,t;      // Variaveis para Rodar o La�o, e Atualizar o Tamnhodo Arquivo.
    TSetor *aux;    // Auxiliar do Tipo Setor para Percorrer a Lista de Setores do Arquivo.

    t = Arquivo->tamanho;                           // t recebe o Tamanho do Arquivo.
    aux = Arquivo->Conteudo.primeiro->proxSetor;    // aux Recebe o Primeiro Setor da Lista de Setores do Arquivo.

    while(aux!=NULL)                                // La�o para percorrer Todos Setores da Lista de Setores do Arquivo.
    {
        for(i=aux->inicio;i!=aux->fim+1;i++)        // La�o que Indica em Qual Setor esta
        {
            for(j=0;j<3&&t>0;j++,t--)               // La�o Roda at� o Final da Posi��o do Disco e at� o Arquivo Acabar
            {
                Disco->Vetor[i][j] = Arquivo->id;   // O disco Recebe o ID do Arquivo nas Posi��es dos Intervalos Determinados.
            }
        }
        aux = aux->proxSetor;                       // Aux Salta para o Proximo Arquivo.
    }

    aux = NULL;    // Faz o Aterramento do Auxiliar.
    free(aux);     // Faz o Liberamento do Auxiliar.

}

TArquivo* PesquisarArquivo(TListaArquivos *Lista, char *nome) // Pesquisa por um Arquivo no Disco e Retorna seu Endere�o.
{
    TArquivo *aux; // Auxiliar de Arquivo para Percorrer a Lista de Arquivos.

    aux = Lista->primeiro->proxArquivo; // Auxiliar de Arquivo Recebe o Primeiro Arquivo.

    while (aux != NULL)                 // La�o para Percorrer Toda a Lista de Arquivo.
    {
        if(strcmp(aux->nome,nome)==0)   // Compara o Nome Passado com os Nomes dos Arquivos Existentes.
        {
            return aux;                 // Quando o nome � Encontrado ele Retorna o Endere�o do Arquivo.
        }
        else
        {
            aux = aux->proxArquivo;     // Quando o nome � Diferente, Aux Salta para Proximo Arquivo.
        }
    }

    return aux;

}

void RemoverArquivo(TListaArquivos *Lista, TArquivo *Arquivo) // Apaga o Arquivo do Disco e Torna o seu Intervalo Livre.
{
    TArquivo *aux1;             // Variavel que chega at� o Arquivo a ser Excluido.
    TArquivo *aux2;             // Variavel que acompanha at� chegar ao arquivo.

    aux1 = Lista->primeiro;     // aux1 Recebe a Cabe�a da Lista de Arquivos.

    while (aux1 != Arquivo)     // La�o para Percorer Toda a Lista de Arquivos at� que Chegue ao Arquivo Desejado.
    {
        aux2 = aux1;                // aux2 recebe aux1 para Estar Sempre o Acompanhando.
        aux1 = aux1->proxArquivo;   // aux1 Salta para o Proximo Setor.
    }

    aux2->proxArquivo = aux1->proxArquivo;  // Ao Achar o Arquivo ele � Retirado da Lista.

    if(aux2->proxArquivo == NULL) // Se for Tirada a Ultima Celula da Lista Atualiza o Ponteiro Ultimo.
    {
        Lista->ultimo = aux2; // Faz o Ponteiro Ultimo Receber a Nova Celula que � a Ultima, ao Tirar a que era Anteriormente.
    }

    aux1->proxArquivo = NULL;               // Faz o Aterramento do Prox do Arquivo Para Liberar.

    free(aux1);                             // E o Arquivo Retirado � Liberado.

    aux2 = NULL;    // Faz o Aterramento do Auxiliar.
    free(aux2);     // Faz o Liberamento do Auxiliar.

}

void ImprimirArquivo(TArquivo *Arquivo) // Imprime as Informa��es de um Arquivo
{
    int i=1;                                          // Variavel para Imprimir Setores Organizados.
    TSetor *aux;                                    // Auxiliar para Percorrer a Lista de Setores do Arquivo.
    aux = Arquivo->Conteudo.primeiro->proxSetor;    // aux Recebe o Primeiro Setor da Lista de Setores do Arquivo.

    printf("\n\n---------------------------------------------------------\n");

    printf("\nInformacoes do Arquivo %c\n\n",Arquivo->id); // ID Arquivo.

    printf("Nome: ");                       // Nome do Arquivo.
    puts(Arquivo->nome);

    printf("Tamanho: ");                    // Tamnho do Arquivo.
    printf("%d\n\n", Arquivo->tamanho);

    printf("Alocacao no Disco: ");
    while(aux != NULL)                      // Mostra onde ele esta Alocado no Disco Pelo Inicio e Fim de Seus Setores
    {
        if((i)%4==0)
        {
            printf("\n\t");
        }

        printf("(%d a %d), ", aux->inicio+1, aux->fim+1);
        aux = aux->proxSetor;               // aux salta para o Proximo Setor.
        i++;

    }

    aux = NULL;    // Faz o Aterramento do Auxiliar.
    free(aux);     // Faz o Liberamento do Auxiliar.

}

void ImprimirListaArquivos(TListaArquivos *Lista) // Imprime Todos os Arquivos
{
    TArquivo *aux; // Auxiliar para Percorrer toda a Lista de Arquivo.

    aux = Lista->primeiro->proxArquivo; // aux Recebe o Primeiro Arquivo da Lista.

    if(aux==NULL)   // Verifica se a Lista esta Vazia.
    {
        printf("\nNao Existe Nenhum Arquivo!");
        return;
    }

    while (aux != NULL) // Passa Por Todos Arquivos Imprimindo
    {
        ImprimirArquivo(aux);       // Imprime o Arquivo Indicado.
        aux = aux->proxArquivo;     // aux Salta para o Proximo Arquivo.
    }

    aux = NULL;    // Faz o Aterramento do Auxiliar.
    free(aux);     // Faz o Liberamento do Auxiliar.

}



// FUN��ES BASE DISCO

void DesfragmentarDisco(TDisco *Disco, TListaSetores *Setores, TListaArquivos *Arquivos) // Realoca os Arquivos no Disco de Forma Organizada.
{
    int falta;        // Variavel que Contabiliza o Quanto falta do Tamanho do Arquivo que Ainda Ncessita de Setores.
    TArquivo *aux1;   // Variavel para Percorrer toda Lista de Arquivo.
    TSetor *aux2;     // Varialvel Auxiliar para Percorrer a Lista de Setores Livres.


    aux1 = Arquivos->primeiro;  // aux1 Recebe a Cabe�a da Lista de Arquivo.

    while(aux1->proxArquivo!=NULL)  // Devolve Todos os Setores para Lista de Setores Livres
    {
        LibeSetores(Setores,aux1->proxArquivo); // Libera os Serores do Arquivo Indicado.
        aux1 = aux1->proxArquivo;               // aux1 Salta para o proximo Arquivo.
    }

    LimpaListaSetores(Setores);     // Limpa a Lista de Setores (Apaga Todos Setore).
    CriarSetores(Disco,Setores);    // Divide o Disco em Setores, cada Setor ter� 2 posi��es do Disco.


    aux1 = Arquivos->primeiro;      // aux1 Recebe a Cabe�a da Lista de Arquivo.

    while(aux1->proxArquivo!=NULL)  // La�o para Percorrer Toda Lista de Arquivos.
    {
        falta = aux1->proxArquivo->tamanho; // falta Recebe o Tamanho do Arquivo Indicado.

        while(falta>0) // La�o que Verifica se ainda � Caracteres que ainda faltam armazenar.
        {
            aux2 = Setores->primeiro;   // aux2 Recebe a Cabe�a da Lista de Setores.

            if(aux2->proxSetor!=NULL)   // Faz o Arquivo Receber o Setores, ou seja, o Realoca.
            {
                aux1->proxArquivo->Conteudo.ultimo->proxSetor = aux2->proxSetor;    // A Lista de Setores do Arquivo Recebe o Setor.
                aux1->proxArquivo->Conteudo.ultimo = aux2->proxSetor;               // Atualiza o Ponteiro Ultimo da Lista de Setores do Arquivo.
                aux2->proxSetor = aux2->proxSetor->proxSetor;                       // Retira o Setor da Lista de Setores Livres.
                aux1->proxArquivo->Conteudo.ultimo->proxSetor = NULL;               // Faz o Aterramento da Lista de Setores do Arquivo.

                falta = falta-6;        // Faz Falta -6 Pois cada Setor Recebido Comporta 6 Caracteres.
                Setores->tam--;         // Atualiza a Quantidade de Setores Dispon�veis.
            }

            if(falta == (aux1->proxArquivo->tamanho)-6) // A Lista de Setores do Arquivo Recebe seu Primeiro
            {
                aux1->proxArquivo->Conteudo.primeiro->proxSetor = aux1->proxArquivo->Conteudo.ultimo; // Atualiza o Prox da Cabe�a da Lista de Setores do Arquivo.
            }

        }

        GravarArquivo(Disco,aux1->proxArquivo); // Grava o Arquivo nos Novos Intervalos Recebidos.
        aux1 = aux1->proxArquivo;               // aux1 salta para o Proximo Arquivo.

    }

    aux1 = NULL;    // Faz o Aterramento do Auxiliar.
    aux2 = NULL;    // Faz o Aterramento do Auxiliar.
    free(aux1);     // Faz o Liberamento do Auxiliar.
    free(aux2);     // Faz o Liberamento do Auxiliar.

}

void FormatarDisco(TDisco *Disco, TListaSetores *Setores, TListaArquivos *Arquivos) // Limpa o Disco Apagando Todos Arquivos
{
    TArquivo *aux;
    int i,j;


    aux = Arquivos->primeiro;  // aux Recebe a Cabe�a da Lista de Arquivo.

    while(aux->proxArquivo!=NULL)  // Devolve Todos os Setores para Lista de Setores Livres
    {
        LibeSetores(Setores,aux->proxArquivo); // Libera os Serores do Arquivo Indicado.
        aux = aux->proxArquivo;               // aux Salta para o proximo Arquivo.
    }


    aux = Arquivos->primeiro;     // aux Recebe a Cabe�a da Lista de Arquivos.

    while(aux->proxArquivo!=NULL) // Limpa a Lista de Arquivos.
    {
        RemoverArquivo(Arquivos,aux->proxArquivo); // Remove o Arquivo Indicado.
        aux = Arquivos->primeiro;                  // aux Salta para o Proximo Arquivo.
    }


    // Limpa Todo o Lixo do Disco.
    for(i=0;i<T;i++)
    {
        for(j=0;j<3;j++)
        {
            Disco->Vetor[i][j] = ' ';
        }
    }

    //aux = NULL;    // Faz o Aterramento do Auxiliar.
    //free(aux);     // Faz o Liberamento do Auxiliar.

}

void ImprimirDisco(TDisco *Disco)
{
     int i,j;

    printf("\n\n\n\t>>>>>>>>>>>>>>>>>>>>>>>>>> DISCO <<<<<<<<<<<<<<<<<<<<<<<<<<\n\n");

    for(i=0;i<T;i++)
    {
        if((i+1)%1==0)
        {
            printf("\t");
        }

        //printf(" [Posicao: %d ",i+1);
        printf("[");
        for(j=0;j<3;j++)
        {
            printf("(%c)",Disco->Vetor[i][j]);
        }
        printf("]");

        if((i+1)%4==0)
        {
            printf("\n\n");
        }

    }
    printf("\t>>>>>>>>>>>>>>>>>>>>>>>>>>>>>O<<<<<<<<<<<<<<<<<<<<<<<<<<<<<");

}



