#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int validarArquivo(const char* nome) {
    int tamanho = strlen(nome);

    if (tamanho == 0 || tamanho > 20) {
        return 0;
    }

    for (int i = 0; i < tamanho; i++) {
        char caractere = nome[i];
        if (!((caractere >= 'A' && caractere <= 'Z') ||
              (caractere >= 'a' && caractere <= 'z') ||
              (caractere >= '0' && caractere <= '9') ||
              caractere == '_' || caractere == '-')) {
            return 0;
        }
    }

    return 1;
}

int main(void) {
    // ler arquivo
    FILE *arquivo;
    arquivo = fopen("dados.csv", "r");
    // conferir abertura do arquivo
    if (arquivo == NULL) {
        printf("Erro: nao foi possivel abrir o arquivo de leitura.");
        return 1;
    }
    int ano1;
    int ano2;

    // menu
    printf("\n************* MENU *************\n");
    printf("** CONSULTA POPULACIONAL IBGE **\n");
    printf("********************************\n");

    printf("\nConsulte as estatisticas oficiais do IBGE a respeito da populacao do Brasil!\n");

    // opcoes
    int opcao;
    while (1)
    {
        printf("\n1 - Calculo da diferenca entre nascimentos e obitos no intervalo\n2 - Exibir ordem decrescente de anos por quantidade de nascimentos\n3 - Sair\n\nDigite uma opcao: ");
        scanf("%d", &opcao);
        while (getchar() != '\n');
        if (opcao == 1 || opcao == 2 || opcao == 3)
        {
            break;
        }
        else 
        {
            printf("Opcao invalida\n");
        }
    }
        // loop para solicitar
    while (1)
    {
        printf("\nInforme o ano do inicio do periodo (2010-2060): ");
        scanf("%d", &ano1);
        while (getchar() != '\n');
        printf("Informe o ano do final do periodo (%d-2060): ", ano1);
        scanf("%d", &ano2);
        while (getchar() != '\n');
        if (ano1 <= 2060 && ano1 >= 2010 && ano2 <= 2060 && ano2 >= ano1)
        {
            break;
        }
        else
        {
            printf("Intervalo invalido\n");
        }
    }
    
    // ignorar cabecalho do arquivo
    rewind(arquivo);
    char linha[100];
    fgets(linha, sizeof(linha), arquivo);

    int ano, nascimentos, obitos, n, o;

    // para case 2
    int array[ano2 - ano1 + 1][2];
    int i, j;

    switch (opcao)
    {
    case 1:
        while (1)
        {
            // loop para somar nascimentos e obitos
            fscanf(arquivo, "%d,%*d,%d,%d", &ano, &nascimentos, &obitos);
            if (ano == ano1)
            {
                for (; ano < ano2; ano++)
                {
                    fscanf(arquivo, "%*d,%*d,%d,%d", &n, &o);
                    nascimentos += n;
                    obitos += o;
                    fgets(linha, sizeof(linha), arquivo);
                }
                printf("\n%-25s%d\n", "Total de nascimentos:", nascimentos);
                printf("%-25s%d\n", "Total de obitos:", obitos);
                printf("%-25s%d\n\n", "Saldo total:", nascimentos - obitos);
                break;
            } 
            else if (ano != ano1 && ano == 2060)
            {
                return 1;
            }
            fgets(linha, sizeof(linha), arquivo);
        }
        break;
    case 2:
        nascimentos = 0;
        for(int i = 0; i <= ano2 - ano1; i++)
        {
            fscanf(arquivo, "%d, %*d, %d, %*d", &ano, &n);
            nascimentos += n;
            array[i][0] = ano;
            array[i][1] = n;
            fgets(linha, sizeof(linha), arquivo);            
        }
        // ordenar por selecao
        for (i = 0; i <= ano2 - ano1; i++)
        {
            int maximo = i;
            for (j = i + 1; j <= ano2 - ano1; j++)
            {
                if (array[j][1] > array[maximo][1])
                {
                    maximo = j;
                }
            }
            // trocar ano e quantidade de nascimentos
            int temp_ano = array[i][0];
            int temp_nascimentos = array[i][1];
            array[i][0] = array[maximo][0];
            array[i][1] = array[maximo][1];
            array[maximo][0] = temp_ano;
            array[maximo][1] = temp_nascimentos;
        }
        printf("\n%-10s%-20s\n", "Ano", "Nascimentos");
        for (i = 0; i <= ano2 - ano1; i++)
        {
            int ano = array[i][0];
            int n = array[i][1];
            printf("%-10d%-20d\n", ano, n);
        }
        printf("%-10s%-20d\n\n", "Total", nascimentos);
        break;
    case 3:
        printf("\nConsulta cancelada\n\n");
        return 0;
    default:
        break;
    }
    // salvar
    printf("Deseja salvar esse resultado (S/N)? ");
    char resposta[2];
    scanf("%s", resposta);
    while (getchar() != '\n');
    if (resposta[0] == 's' || resposta[0] == 'S' || resposta[0] == 'y' || resposta[0] == 'Y')
    {
        char nome[21];
        while(1)
        {
            printf("Digite o nome do arquivo que sera salvo (sem extensao do formato .csv): ");
            scanf("%20s", nome);
            if (validarArquivo(nome)) {
                strcat(nome, ".csv");
                break;
            }
            else
            {
                printf("Nome de arquivo invalido\n\n");
            }
        }
        char caminho[25];
        sprintf(caminho, "arquivos/%s", nome);
        printf("%s\n", caminho);
        FILE *arquivo2;
        arquivo2 = fopen(caminho, "w");
        rewind(arquivo);
        fgets(linha, sizeof(linha), arquivo);
        if (opcao == 1)
        {
            fprintf(arquivo2, "Ano,Nascimentos,Obitos,Saldo\n");
            for (i = 0; i <= ano2 - ano1; i++)
            {
                fscanf(arquivo, "%d,%*d,%d,%d", &ano, &n, &o); 
                fprintf(arquivo2, "%d,%d,%d,%d\n", ano, n, o, n - o);
                nascimentos += n;
                obitos += o;
            }
            fprintf(arquivo2, "Total,%d,%d,%d", nascimentos, obitos, nascimentos - obitos);
        }
        if (opcao == 2)
        {
            fprintf(arquivo2, "Ano,Nascimentos\n");
            for (i = 0; i <= ano2 - ano1; i++)
            {
                int ano = array[i][0];
                int n = array[i][1];
                fprintf(arquivo2, "%d,%d\n", ano, n);
            }
            fprintf(arquivo2, "Total,%d", nascimentos);
        }
        fclose(arquivo2);
        fclose(arquivo);
    }
    printf("Deseja realizar nova consulta (S/N)? ");
    scanf("%s", resposta);
    if (resposta[0] == 's' || resposta[0] == 'S' || resposta[0] == 'y' || resposta[0] == 'Y')
    {
        while (getchar() != '\n');
        main();   
    }
    else {
        printf("\nConsulta encerrada.\n\n");
    }    
    return 0;
}
