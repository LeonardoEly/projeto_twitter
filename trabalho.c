#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <locale.h>
#define bool int
#define true 1
#define false 0

typedef struct
{
    int id;
    char usuario[21];
    char mensagem [281];
    char data [11];
    char local [21];
    int conta_msg;
} registro;

typedef struct
{
    int id;
    char usuario[21];
    int quantidade;
} ranking;


int contaLinhas(char *arquivo)
{
    FILE *f = NULL;
    char nomes[500];
    int cont = 0;

    f = fopen(arquivo, "r");

    if(f == NULL)
    {
        printf("Erro ao abrir o arquivo %s\n", arquivo);
        exit (0);
    }
    else
    {
        while (fscanf(f, "%[^\n]%*c", nomes) == 1)
        {
            cont++;
        }
    }

    fclose(f);
    return(cont);
}

registro *arrayRegistro(char *arquivo, int linhas)
{
    FILE *f = NULL;
    char nomes[500];
    registro *registros = (registro *)malloc(sizeof(registro) * linhas);

    int cont = 0;
    char *token;

    f = fopen(arquivo, "rb");

    if (f == NULL)
    {
        printf("Erro ao abrir o arquivo %s\n", arquivo);
        exit(0);
    }
    else
    {
        while (fscanf(f, "%[^\n]%*c", nomes) == 1)
        {
            registros[cont].conta_msg = 0;

            token = strtok(nomes, ";");
            strcpy(registros[cont].usuario, token);

            registros[cont].id = cont;
            registros[cont].conta_msg ++;

            int i;

            for(i = 0; i < cont; i++)
            {
                if (strcmp(registros[cont].usuario,registros[i].usuario) == 0)
                {
                	//Hipotese: Reconhece uma mensagem de um mesmo usuário e conta todas mensagens enviadas por ele no arquivo .csv .
                    //printf("Usuário %s já existe, nova mensagem adicionada!\n", registros[cont].usuario); -> Depois da primeira, avisa que o usuario ja existe por tanto nao cria ele denovo, demora bastante pois tem muita repetição!
                    registros[cont].id = registros[i].id;
                    registros[cont].conta_msg++;
                }
            }

            token = strtok(NULL, ";");
            strcpy(registros[cont].mensagem, token);

            token = strtok(NULL, ";");
            strcpy(registros[cont].data, token);

            token = strtok(NULL, ";");
            strcpy(registros[cont].local, token);

            //printf("Id: %d | Usuario: %s | Mensagem: %s | Data: %s | Local: %s\n", registros[cont].id, registros[cont].usuario, registros[cont].mensagem, registros[cont].data, registros[cont].local);

            cont++;
        }
    }

    fclose(f);

    return registros;

}

int main()
{
    setlocale(LC_ALL, "Portuguese");

    char *registrosarq = "dados.csv";

    int linhas = contaLinhas(registrosarq);
    registro *registros = arrayRegistro(registrosarq, linhas);
m
    FILE *fp;
    fp = fopen("registros.bin", "wb");

    int i;

    for (i = 0; i < linhas; i++)
    {
        fwrite(&registros[i], sizeof(registro), 1, fp);
        printf("Id: %d | Usuario: %s | Mensagem: %s | Data: %s | Local: %s\n | %dº Mensagem \n", registros[i].id, registros[i].usuario, registros[i].mensagem, registros[i].data, registros[i].local, registros[i].conta_msg);
    }

    fclose(fp);

    fp = fopen("registros.bin", "rb");

    fseek(fp, 0, SEEK_END);
    long int nbytes = ftell(fp);

    int procurado, meio, passo, limiteComparacoes, comparacoesFeitas;
    registro r;
    bool encontrado;

    int op;

    while ( op < 1 || op > 2)
    {
        printf("Digite a opção que deseja:\n");
        printf("1 = Para procurar registro pelo id \n");
        printf("2 = Para sair \n");
        scanf("%d", &op);
    }

    while (op == 1)
    {
        printf("Insira o registro que deseja procurar (id): ");
        scanf("%d", &procurado);
        printf("\n");

        meio = (nbytes / sizeof(registro)) / 2;
        passo = meio / 2;

        fseek(fp, meio * sizeof(registro), SEEK_SET);
        fread(&r, sizeof(registro), 1, fp);

        encontrado = false;
        limiteComparacoes = (int)log2(nbytes / sizeof(registro)) + 1, comparacoesFeitas = 0;

        while (encontrado == false && comparacoesFeitas <= limiteComparacoes)
        {
            if (r.id >= procurado)
            {
                if (r.id == procurado)
                {
                    printf("\nEsse é o registro procurado: ID: %d | Usuário: %s | Quantidade de tweets: %d \n", r.id, r.usuario, r.conta_msg);
                    encontrado = true;
                }
                meio = meio - passo;
            }

            else
            {
                meio = meio + passo;
            }

            fseek(fp, meio * sizeof(registro), SEEK_SET);
            fread(&r, sizeof(registro), 1, fp);

            comparacoesFeitas++;
            passo = passo / 2;

            if (passo < 2)
                passo = 1;
        }

        if (procurado == 0)
        {
            printf("\nO registro teste de ID 0 não foi inserido na lista!\n");
            encontrado = true;
        }

        if (procurado == 1)
        {
            printf("\nEsse é o registro procurado: ID: %d | Usuário: %s | Quantidade de tweets: %d \n", r.id, r.usuario, r.conta_msg);
            encontrado = true;
        }

        if (procurado == 99)
        {
            printf("\nEsse é o registro procurado: ID: %d | Usuário: %s | Quantidade de tweets: %d \n", r.id, r.usuario, r.conta_msg);
            encontrado = true;
        }

        if (encontrado == false)
        {
            printf("\nRegistro não encontrado!\n\n");
        }

        op = 0;

        while ( op < 1 || op > 2)
        {
            printf("Digite a opção que deseja:\n");
            printf("1 = Para procurar registro pelo id \n");
            printf("2 = Para sair \n");
            scanf("%d", &op);
        }
    }

    if (op == 2)
    {

		return 0;
		
    }

    fclose(fp);

    return 0;
}
