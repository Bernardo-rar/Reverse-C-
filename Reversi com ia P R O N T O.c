#include <stdlib.h>
#include <stdio.h>
#include <math.h>
//#define MAX_NIVEL 3

int nivelmax=3;
struct posicao{
int tabuleiro[8][8];
int jogadorVez;
};


struct jogada{
int linha,coluna;

};
struct elemento{
	struct jogada jog;
	struct elemento *prox,*ant;//trocar jogada por elemento, colocar executa por referencia.
};

struct posicao IniciaTabuleiro(){
    struct posicao tab;
	for (int i=0;i<8;i++){
        for (int j=0;j<8;j++){
            tab.tabuleiro[i][j]=0;
        }
    }
    tab.tabuleiro[3][3] = tab.tabuleiro[4][4] = 1;
    tab.tabuleiro[3][4] = tab.tabuleiro[4][3] = -1;
    tab.jogadorVez=-1;
    return tab;
}




void DesenhaTabuleiro(struct posicao tab){
    system("cls");
	printf("  0 1 2 3 4 5 6 7\n");
    for (int i=0;i<8;i++){
        printf("%d",i);
        for (int j=0;j<8;j++){
            if (tab.tabuleiro[i][j]==0){
                printf("  ");
            }else if (tab.tabuleiro[i][j]==1){
                printf(" W");
            }else if (tab.tabuleiro[i][j]==-1)
			printf(" B");
        }
        printf("\n");
    }
}

struct elemento *CriaLista(){
    struct elemento *sentinela=(struct elemento*)malloc(sizeof(struct elemento));
	sentinela->prox=sentinela;
	sentinela->ant=sentinela;
	return sentinela;
}


struct elemento *CriaElemento(struct elemento *p,int i, int j){
	struct elemento *resp=(struct elemento*)malloc(sizeof(struct elemento));
	resp->jog.linha=i;
	resp->jog.coluna=j;
	resp->ant=NULL;
	resp->prox=NULL;
	return resp;	
}


struct elemento *InsereInicio(struct elemento *b,int i,int j){
	struct elemento *novo=CriaElemento(b,i,j);	
	novo->jog.linha=i;
	novo->jog.coluna=j;
	novo->prox=b->prox;
	novo->ant=b;
	b->prox->ant=novo;
	b->prox=novo;	
}

int TestaDum(struct posicao a,int b,int c, int d, int e){
    int i=b+d;
    int j=c+e;
    int cont=0;

    while (i>=0 && i<8 && j>=0 && j<8 && a.tabuleiro[i][j]==-a.jogadorVez){
        cont++;
        i += d;
        j += e;
    }

    if (i>=8||i<0||j>=8||j<0){
        cont = 0;
    }else if (a.tabuleiro[i][j]==0)
        cont = 0;

    return cont;
}

int TestaDirecao(struct posicao *a, struct jogada jog, int deltaL, int deltaC){
    int i=jog.linha+deltaL;
    int j=jog.coluna+deltaC;
    int cont=0;

    while (i>=0 && i<8 && j>=0 && j<8 && a->tabuleiro[i][j]==-a->jogadorVez){
        cont++;
        i += deltaL;
        j += deltaC;
    }

    if (i>=8||i<0||j>=8||j<0){
        cont = 0;
    }else if (a->tabuleiro[i][j]==0)
        cont = 0;

    return cont;
}


struct elemento *CalculaJogadasValidas(struct posicao a){//aparentemente consertado
	struct elemento *b=CriaLista();
	int verifica=0;
	int cont=0;
	for(int i=0;i<8;i++){
		for(int j=0;j<8;j++){
			cont=0;
			if(a.tabuleiro[i][j]==0){
				for(int c=-1;c<2;c++){
					for(int d=-1;d<2;d++){
						if(TestaDum(a,i,j,c,d)>0 ){
							cont++;
							verifica=1;
							}
					}
				}
			if(cont>0)
			b=InsereInicio(b,i,j);	
			}
		}
	}
if(verifica==0){
	//printf("n ha jogadas validas\n");
	//calcula vencedor
	return NULL;
}
return b;
}


void ImprimirLista(struct elemento *ini){
    struct elemento *aux = ini->prox;
    printf("posicoes favoraveis:\n");
   while (aux!=ini){
        printf("linha %d coluna %d\n",aux->jog.linha,aux->jog.coluna);
        aux = aux->prox;
    }
}



struct elemento *ComparaJogada(struct elemento *a,int i, int j){
	struct elemento *aux=a->prox;
	while(aux!=a){
		if (aux->jog.linha==i && aux->jog.coluna == j){
			
			return aux;
		}
		aux=aux->prox;
	}
	return NULL;
}




struct elemento *EscolheJogada(struct elemento *a,struct posicao b){//ta pegando pelo visto
	
	if(a->prox==NULL) return NULL;
	int i,j;
	ImprimirLista(a);
	if(b.jogadorVez==-1)
	printf("Vez das pretas\n");
	else printf("Vez das brancas\n");
	printf("Escolha uma das jogadas\n");
	scanf("%d %d",&i,&j);
	while(ComparaJogada(a,i,j)==NULL){
		printf("Esta jogada n e possivel, pfvr tente novamente");//mudar para dps de um certo ponto, caso for jogada errada duas vezes, pularoplayer
		scanf("%d %d",&i,&j);
	}
	a=ComparaJogada(a,i,j);//ver se e o certo fazer essa bagaça
	//printf("apenas para ver se foi %d %d \n",a->linha,a->coluna);
	return a;
}


void ViraPedrasDirecao(struct posicao *a, struct jogada jog, int deltaL, int deltaC){
    int i=jog.linha+deltaL;
    int j=jog.coluna+deltaC;

    while (a->tabuleiro[i][j]==-a->jogadorVez){
        a->tabuleiro[i][j] = a->jogadorVez;
        i += deltaL;
        j += deltaC;
    }
    //return a;
}



void ExecutaJogada(struct posicao *a, struct jogada jog){
    int resposta=1;

    if (jog.linha>=0&&jog.linha<8&&jog.coluna>=0&&jog.linha<8&&a->tabuleiro[jog.linha][jog.coluna]==0){

        for (int deltaL=-1;deltaL<=1;deltaL++){
            for (int deltaC=-1;deltaC<=1;deltaC++){
                if (deltaL!=0||deltaC!=0){
                    if (TestaDirecao(a,jog,deltaL,deltaC)){
                        ViraPedrasDirecao(a,jog,deltaL,deltaC);
                        resposta=1;
                        
                    }
                }
            }
        }


        if (resposta==1){
            a->tabuleiro[jog.linha][jog.coluna] = a->jogadorVez;
        }
    }
    a->jogadorVez=-a->jogadorVez;
    //return a;
}

struct elemento *DestruirLista(struct elemento *b){//ver se esta deletando certinho
	struct elemento *aux=b;
	b->ant->prox=NULL;
	while(aux!=NULL){
		b=b->prox;
		free(aux);
		aux=b;
	}
	return NULL;
}

void CalculaVencedor(struct posicao b){
	int brancas=0;
	int pretas=0;
    for (int i=0;i<8;i++){
        for (int j=0;j<8;j++){
            if (b.tabuleiro[i][j]==1)
                brancas++;
                else{
                	if(b.tabuleiro[i][j]==-1)
                	pretas++;
				}
        }
    }

    if (brancas==pretas){
        printf("Empate\n");
    }else if (brancas>pretas){
        printf("Brancas vencem\n");
    }else printf("Pretas vencem\n");
}



double AvaliaPosicao(struct posicao a){
 	struct posicao aux=a;
 	int refresco=0;
	 
	 for(int i=0;i<8;i++){
		for(int j=0;j<8;j++){
			if(a.tabuleiro[i][j]==1)
			refresco++;
			else if(a.tabuleiro[i][j]==-1)	//vai dizer o valor do tabuleiro,a ia vai testando ate achar 1 bm.
			refresco--;
		}
	}
	 for(int i=0;i<8;i++){
	 	if(a.tabuleiro[0][i]==a.jogadorVez || a.tabuleiro[7][i]==a.jogadorVez)
	 	refresco=refresco+5*a.jogadorVez;
	 	if(a.tabuleiro[i][0]==a.jogadorVez || a.tabuleiro[i][7]==a.jogadorVez)
	 	refresco=refresco+5*a.jogadorVez;
	 
	 	if(a.tabuleiro[0][i]==-a.jogadorVez || a.tabuleiro[7][i]==-a.jogadorVez)
	 	refresco=refresco+5*-a.jogadorVez;
	 	if(a.tabuleiro[i][0]==-a.jogadorVez || a.tabuleiro[i][7]==-a.jogadorVez)
	 	refresco=refresco+5*-a.jogadorVez;;
	 }
	 return refresco;
}





double definevalor(double melhorvalor,int nivel){
	if(nivel%2==0)
	melhorvalor=-INFINITY;
	else melhorvalor=INFINITY;
	return melhorvalor;
}


struct jogada ExecutaIA(struct posicao posAtual,int nivel,double alfa,double beta){//coloocar o fucking nivel igual a 1, dessa forma so analisara 1 vez :D
    ///declarar e inicializar as variáveis indicadas (item 1 do exercício)
    //printf("entrar entrou\n");
    double melhorvalor;
	melhorvalor=definevalor(melhorvalor,nivel);
	//struct posicao poscopia;
	struct jogada jogadaIA,melhorJogada;
	
	melhorJogada.linha=-1;
	melhorJogada.coluna=-1;
	int podado=0;
	double valorjogada;
	struct elemento *jogadaAux;
	struct elemento *lista;
	
	lista=CalculaJogadasValidas(posAtual);//ver se esta calculando tudo certo, qualquer coisa colocar -jog.
	 if(lista==NULL) return melhorJogada;
	
	//ImprimirLista(lista);
	
	 
	 jogadaAux=lista->prox;// ta passando os valores certinho
	 	
	 
	 while (jogadaAux!=lista && podado==0){///aqui vamos percorrer a lista de jogadas possíveis (ou das brancas ou das pretas) enquanto ainda for bom continuar avaliando a posiçao

        ///copiar o parâmetro "posAtual" para "posCopia" (item 3 do exercício)
		struct posicao poscopia=posAtual;
        ///executar a jogada "jogadaAux" em "posCopia" (item 3 do exercício)
        
		//if(jogadaAux->jog.linha!=-1 &&jogadaAux->jog.coluna!=-1 )
		ExecutaJogada(&poscopia,jogadaAux->jog);
	
		//DesenhaTabuleiro(poscopia);
        ///verificar se "nivel" é menor do que "MAX_NIVEL" (item 4 do exercício)
        if (nivel<nivelmax)
        {
            ///verificar se "nivel" é par (item 4a do exercício)
            if (nivel%2==0){
                ///chamar a função recursivamente e guardar a jogada retornada em "jogadaIA" (item 4a do exercício)
                jogadaIA=ExecutaIA(poscopia,nivel+1,melhorvalor,beta);
            }

            ///verificar se "nivel" é ímpar (item 4b do exercício) - pode ser usado "else" em relação ao item 4a
            else{
            	jogadaIA=ExecutaIA(poscopia,nivel+1,alfa,melhorvalor);
                ///chamar a função recursivamente e guardar a jogada retornada em "jogadaIA" (item 4b do exercício)
            }
            if(jogadaIA.linha!=-1 &&jogadaIA.coluna!=-1 )
			ExecutaJogada(&poscopia,jogadaIA);

            ///executar "jogadaIA" sobre "posCopia" (item 4c do exercício)

        }
		valorjogada=AvaliaPosicao(poscopia);// consertar o q n esta pegando quanto ao refresco, qualquer coisa colocar o codigo sem funçao dentro da f.
		//printf("avaliou a posicao %f\n",valorjogada);
        ///avaliar a posiçao "posCopia" (item 5 do exercício)
        if(valorjogada<alfa || valorjogada>beta)
		podado=1;
		//printf("podado %d",podado);
        ///verificar "jogadaIA" é a melhor jogada encontrada até o momento (item 6 do exercício)
        if(nivel%2==0 && valorjogada>=melhorvalor){
        melhorvalor=valorjogada;
        melhorJogada=jogadaAux->jog;
		
		}else{
			if(nivel%2==1 && valorjogada<=melhorvalor){
				melhorvalor=valorjogada;
				melhorJogada=jogadaAux->jog;
				;//testar tanto cm ele quanto cm jogada aux;
			}
		}
		
        jogadaAux = jogadaAux->prox;
    }

    ///liberar a memória alocada nas listas de possíveis jogadas das peças brancas ou pretas (item 7 do exercício)
		DestruirLista(lista);
    ///retornar a melhor jogada encontrada "melhorJogada" (item 7 do exercício).
    if(jogadaAux->jog.linha!=-1 &&jogadaAux->jog.coluna!=-1 )
    return melhorJogada;
    else printf("jogada Invalida\n");
}

void SalvaJogada(char a[15],struct jogada *ponteiro){
   	FILE *arq;
	if(ponteiro==NULL){
	arq=fopen(a,"wb");
	fclose(arq);
	}
	else{
		arq=fopen(a,"ab");
		fwrite(ponteiro,sizeof(struct jogada),1,arq);
		fclose(arq);
	}
    ///abrir o arquivo de acordo com o valor do ponteiro
	
    ///salvar a jogada, se for o caso
	
    ///fechar o arquivo
    
}


int main(){
	//int MAX_NIVEL colocar pela mainfdc
	struct posicao tab;
	FILE *arq;
	char nome [15]="jogadas.bin";
	struct elemento *b=NULL;
	struct jogada *aux=NULL;
	struct jogada melhorjogada;
	int casasvazias=60;
	int verifica=0;
	SalvaJogada(nome,aux);
	tab=IniciaTabuleiro();//ok
	 while (casasvazias>0 && verifica<2){
	 	DesenhaTabuleiro(tab);
		if(tab.jogadorVez==-1){
		
			b=CalculaJogadasValidas(tab);//ok
			if(b==NULL){
				printf("nao ha jogadas validas para as pretas\n");
				tab.jogadorVez=-tab.jogadorVez;
				verifica++;
			}else{
				verifica=0;
				b=EscolheJogada(b,tab);//pegando certo, retornando o blele certinho
				ExecutaJogada(&tab,b->jog);
				aux=&b->jog;
				//aux->linha=b->jog.linha;
				//aux->coluna=b->jog.coluna;
				SalvaJogada(nome,aux);
				DestruirLista(b);
				casasvazias--;
				}
		}else{
			melhorjogada=ExecutaIA(tab,0,-INFINITY,+INFINITY);
			
			if(melhorjogada.linha!=-1 && melhorjogada.coluna!=-1 ){
			verifica=0;
			ExecutaJogada(&tab,melhorjogada);
			
			SalvaJogada(nome,&melhorjogada);
		
			casasvazias--;
			}
			else{
			//printf("caiu no else");
			verifica++;
			tab.jogadorVez=-tab.jogadorVez;
			} 
				
				}
	}
	CalculaVencedor(tab);
	/*
	arq=fopen("jogadas.bin","rb");
	while(fread(aux,sizeof(struct jogada),1,arq)>0){
		
		printf("%d %d \n",aux->linha,aux->coluna);
		//cont++;
	}
	fclose(arq);//apenas pra conferir se a lista foi printada certa.
	/*/
	
}
