#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_NAME 50
#define MAX_PROD 50

//Structs//
typedef struct s_chaves {
    char nome[MAX_NAME];
    int index; 
} chaves;

typedef struct s_infs {
    float valorCompra;
    float valorVenda;
    int estoque;
    int vendidos;
    bool deletado;
} infs;
////////////

//Funções//
void QuicksortPart3(int Left, int Right,int *i,int *j, chaves *keys);
void QuicksortPart2(int Left, int Right, chaves *keys);
void QuicksortPart1(chaves *keys, int tam);

int busca_binaria1(chaves *keys, char *k, int inf, int sup);
int busca_binaria2(chaves *keys, int n, char *k);

void Vender(FILE *fp, chaves *keys, infs *infos, int tam);
void Cadastrar(FILE *fp,chaves *keys, infs *infos, int *tam);
void RemoverProduto(FILE *fp,chaves *keys, infs *infos, int *tam);
void ConsultarEstoque(chaves *keys, infs *infos, int tam);
void ConsultarLucro(chaves *keys, infs *infos, int tam);
void ReporEstoque(FILE *fp,chaves *keys, infs *infos, int tam);
void Encerramento(chaves *keys, infs *infos, int tam);
////////////

int main() {
    FILE *fp;
    chaves keys[MAX_PROD];
    infs infos[MAX_PROD];
    int op,i,tam=0;

    for(i=0;i<MAX_PROD;i++)  
        keys[i].index = 0; 
    
    for(i=0;i<MAX_PROD;i++) 
        infos[i].deletado = true;
    

    Cadastrar(fp,keys,infos,&tam); //Deve cadastrar quando entrar pela primeira vez. 
    do
    {
        printf("\nMenu:\n");
        printf("1-Vender\n");
        printf("2-Cadastrar novo produto\n");
        printf("3-Remover produto do estoque\n");
        printf("4-Consultar estoque\n");
        printf("5-Consultar lucro\n");
        printf("6-Repor estoque\n");
        printf("0-Sair\n");

        scanf("%d",&op);
        switch(op) {
            case 1: Vender(fp,keys,infos,tam); break;
            case 2: Cadastrar(fp,keys,infos,&tam); break;
            case 3: RemoverProduto(fp,keys,infos,&tam); break;
            case 4: ConsultarEstoque(keys,infos,tam); break;
            case 5: ConsultarLucro(keys,infos,tam); break;
            case 6: ReporEstoque(fp,keys,infos,tam); break;
            case 0: Encerramento(keys,infos,tam); break;
            default: printf("Opcao invalida!\n"); break;
        }
    } while (op != 0);      
}

void QuicksortPart3(int Left, int Right,int *i,int *j, chaves *keys) {
    chaves x,aux;
    *i = Left; *j = Right;

    x = keys[(*i+*j)/2]; //Pivô
    do
    {
        while (strcmp(x.nome,keys[*i].nome) > 0)
            (*i)++;
        while (strcmp(x.nome,keys[*j].nome) < 0)
            (*j)--;
        if(*i <= *j) {
            aux = keys[*i];
            keys[*i] = keys[*j];           
            keys[*j] = aux;
            (*i)++;
            (*j)--;
        }        
    } while (*i <= *j);   
}
void QuicksortPart2(int Left, int Right, chaves *keys) {
    int i,j;

    QuicksortPart3(Left,Right,&i,&j,keys); 
    if(Left < j)
        QuicksortPart2(Left,j,keys); 
    if(i < Right)
        QuicksortPart2(i,Right,keys);
}
void QuicksortPart1(chaves *keys, int tam) {
    QuicksortPart2(0,tam-1,keys);
}

int busca_binaria1(chaves *keys, char *k, int inf, int sup) {
    if(inf <= sup) {
        int meio = (inf + sup)/2;

        if(strcmp(k,keys[meio].nome) < 0)
            return busca_binaria1(keys,k,inf,meio-1);
        else if(strcmp(k,keys[meio].nome) > 0)
                return busca_binaria1(keys,k,meio + 1,sup);
        else return meio;
    }

    return -1;
}
int busca_binaria2(chaves *keys, int n, char *k) {
    return busca_binaria1(keys,k,0,n-1);
}

void Vender(FILE *fp, chaves *keys, infs *infos, int tam) {
    char name[MAX_NAME];
    int i,qnt;

    do
    {
        printf("Informe o produto que deseja comprar:\n");
        scanf(" %[^\n]",name);
        for(i=0;i<tam;i++)
            if(!strcmp(keys[i].nome,name))
                break;
        if(i == tam)
        printf("O produto informado nao existe no sistema\n");
    } while (i == tam);
    
    printf("Esse produto custa: %.2f reais\n",infos[(keys[i].index)-1].valorVenda);
    printf("Informe a quantidade desejada:\n");
    scanf("%d",&qnt);

    if(qnt > infos[(keys[i].index)-1].estoque)
        printf("Nao ha estoque disponivel para atender a venda\n");
        else {
            printf("Valor a pagar: %.2f",qnt*infos[(keys[i].index)-1].valorVenda);
            infos[(keys[i].index)-1].estoque = infos[(keys[i].index)-1].estoque - qnt;
            infos[(keys[i].index)-1].vendidos++;

    fp = fopen("infs.bin","wb");
    for(i=0;i<tam;i++)
        fprintf(fp,"%.2f %.2f %d %d\n",infos[i].valorCompra,infos[i].valorVenda,infos[i].estoque,infos[i].vendidos); 
    
    fclose(fp);
    fflush(fp);
    }
}

void Cadastrar(FILE *fp,chaves *keys, infs *infos, int *tam) {
    int i,j;

    for(i=0;i<MAX_PROD;i++)
        if(!keys[i].index)
        break;

    for(j=0;j<MAX_PROD;j++)
        if(infos[j].deletado) //descrubo em qual posição está o primeiro deletado
        break;

    //Aloca infos nas structs//
    printf("Digite o nome do produto:\n");
    scanf(" %[^\n]",keys[i].nome); 

    printf("Digite o valor de compra:\n");
    scanf("%f",&infos[j].valorCompra); 
   
    printf("Digite o valor de venda:\n");
    scanf("%f",&infos[j].valorVenda); 
    
    printf("Digite o estoque do produto:\n");
    scanf("%d",&infos[j].estoque); 
    
    printf("Digite a quantidade vendida:\n");
    scanf("%d",&infos[j].vendidos); 

    keys[i].index = j+1; //Indica a posição em que se encontra o restante das informações em infs.bin
    infos[j].deletado = false; //Indica o registro adicionado como existente
    (*tam)++; 

    QuicksortPart1(keys,*tam);
    
    //Aloca infos nos arquivos//
    fp = fopen("chaves.bin","wb"); 
    for(i=0;i<*tam;i++)
        if(keys[i].index!=0)
        fprintf(fp,"%s %d\n",keys[i].nome,keys[i].index); 

    fclose(fp);       
    fflush(fp);

    fp = fopen("infs.bin","wb");
    for(j=0;j<*tam;j++)
        fprintf(fp,"%.2f %.2f %d %d\n",infos[j].valorCompra,infos[j].valorVenda,infos[j].estoque,infos[j].vendidos); 
    
    fclose(fp);
    fflush(fp);
    
}

void RemoverProduto(FILE *fp,chaves *keys, infs *infos, int *tam) {
    char product[MAX_NAME];
    int i,j;

    printf("Digite o produto que voce deseja remover do estoque:\n");
    scanf(" %[^\n]",product);

    for(i=0;i<MAX_PROD;i++) 
        if(strcmp(keys[i].nome,product)==0)
            break;   

    if(i == MAX_PROD) {
        printf("Nome de produto invalido!\n");
        return;
    }
    strcpy(keys[i].nome,"\0"); //remove o nome desejado da lista de chaves

    infos[(keys[i].index)-1].deletado = true; //Faz a linha na tabela de infos ser considerada deletada
    keys[i].index = 0;

    QuicksortPart1(keys,*tam);
    fp = fopen("chaves.bin","wb");

    for(i=0;i<*tam;i++) 
        if(keys[i].index != 0)
        fprintf(fp,"%s %d\n",keys[i].nome,keys[i].index);  
         
    fclose(fp);   
    fflush(fp);
    (*tam)--;
}

void ConsultarEstoque(chaves *keys, infs *infos, int tam) {
    int i;
    for(i=0;i<tam;i++) {
        printf("Produto: %s\n",keys[i].nome);
        printf("Estoque: %d\n",infos[(keys[i].index)-1].estoque);
        printf("Valor unitario: %.2f\n",infos[(keys[i].index)-1].valorVenda);
        printf("Valor Total: %.2f\n\n",infos[(keys[i].index)-1].estoque*infos[(keys[i].index)-1].valorVenda);
    }
}

void ConsultarLucro(chaves *keys, infs *infos, int tam) {
    int i;
    for(i=0;i<tam;i++) {
        printf("Produto: %s\n",keys[i].nome);
        printf("Total vendido: %.2f reais\n\n",infos[(keys[i].index)-1].vendidos*infos[(keys[i].index)-1].valorVenda);
    }
}

void ReporEstoque(FILE *fp,chaves *keys, infs *infos, int tam) {
    char product[MAX_NAME];
    int i,qnt,pos;

    do
    {
        printf("Informe o produto que deseja repor:\n");
        scanf(" %[^\n]",product);

        pos = busca_binaria2(keys,tam,product);       
        if(pos==-1)
        printf("O produto informado nao existe no sistema\n");

    } while (pos==-1);

    do
    {
        printf("Informe a quantidade a ser reposta:\n");
        scanf("%d",&qnt);  

        if(qnt <= 0)
        printf("Informe um estoque valido.\n");     
    } while (qnt <= 0);

    infos[(keys[pos].index)-1].estoque += qnt;

    fp = fopen("infs.bin","wb");
    for(i=0;i<tam;i++)
        fprintf(fp,"%.2f %.2f %d %d\n",infos[i].valorCompra,infos[i].valorVenda,infos[i].estoque,infos[i].vendidos); 
    
    fclose(fp);
    fflush(fp);

    printf("O estoque atualizado de %s e %d\n",keys[pos].nome,infos[(keys[pos].index)-1].estoque);
}

void Encerramento(chaves *keys, infs *infos, int tam) {
    int i,total=0;
    float preju=0,ganho=0;

    for(i=0;i<tam;i++) {
        total += infos[(keys[i].index)-1].vendidos;
        ganho += infos[(keys[i].index)-1].vendidos*infos[(keys[i].index)-1].valorVenda;
    }
    printf("Total de vendas: %d\n\n",total);

    for(i=0;i<tam;i++) {
        printf("Produto %s:\n",keys[i].nome);
        printf("Quantidade vendida: %d\n",infos[(keys[i].index)-1].vendidos);
        printf("Quantidade em estoque: %d\n",infos[(keys[i].index)-1].estoque);
        printf("Valor: %.2f\n reais",infos[(keys[i].index)-1].valorCompra);
        printf("Valor total em estoque: %.2f\n reais",infos[(keys[i].index)-1].valorCompra*infos[(keys[i].index)-1].estoque);

        preju += infos[(keys[i].index)-1].estoque*infos[(keys[i].index)-1].valorCompra;
    }

    printf("O lucro total do dia foi de: %.2f reais",ganho - preju);  
    
}