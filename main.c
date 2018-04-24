#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define inf -1
#define True 1
#define False 0

/*2 PROJECTO ASA 2017/2018
	GRUPO 26 - LEIC-T
Catarina Pedreira n.87524
Miguel Oliveira n.87689
*/


/*list*/

typedef struct node { 
	int vNum;
	int weight;
	struct node *next; 
}LLElem;

/*graph*/

typedef struct {
	int to;
	int weight;
} link;

typedef struct {
	int d;
	int low;
	int nTo;
	link* tos; //static
} vertex;

typedef struct {
	int nVert;
	vertex* v; //static
} graph;

graph graph1;

/*list functions*/


void push(LLElem **head, int vNum, int L) {
	LLElem* new = (LLElem*) malloc(sizeof(LLElem)); 
	if (new==NULL) {
		printf("Erro malloc pilha\n");
		exit(1);
	}
	new->vNum= vNum; 
	new->next = *head; 
	*head = new; 
} 

int pop(LLElem **head, int L) {
	int vNum = (*head)->vNum;
	LLElem *temp = (*head)->next;
	*head = temp;
	return vNum;
}

/*graph functions*/

int createGraph() {
	int i,j;
	int N, M;
	int Ntemp, Mtemp;
	int tempWeight;

	scanf("%d %d", &N, &M);
	graph1.nVert = 0; /*we increment this as we add more vertexes*/

	graph1.v = (vertex*) malloc(sizeof(vertex)*((N*M)+2)); /*+2 to have s and t*/
	if (graph1.v == NULL) {
		printf("ERRO malloc 1 graph\n");
		exit(1);
	}

	/*all vertexes have 5, 4 or 3 tos, except vertexes s (with N*M) and t (with 0)*/

	/*create vertex t*/
	graph1.v[0].d = 0;
	graph1.v[0].low = 0;
	graph1.v[0].nTo = 0;
	graph1.v[0].tos = NULL;

	/*create vertex s*/
	graph1.v[1].d = 0;
	graph1.v[1].low = 0;
	graph1.v[1].nTo = 0;
	graph1.v[1].tos = (link*) malloc(N*M*sizeof(link));

	/*create all other vertexes*/
	for (i=2; i<(N*M)+2; i++) {
		graph1.v[i].d = 0;
		graph1.v[i].low = 0;
		graph1.v[i].nTo = 0;
		Ntemp = (i-2)/N;
		Mtemp = (i-2)%N;
		if (Ntemp==0 || Ntemp==(N-1))
			if (Mtemp==0 || Mtemp==(M-1))
				graph1.v[i].tos = (link*) malloc(3*sizeof(link));
			else
				graph1.v[i].tos = (link*) malloc(4*sizeof(link));
		else if (Mtemp==0 || Mtemp==(M-1)) {
			if (Ntemp!=0 || Ntemp!=0)
				graph1.v[i].tos = (link*) malloc(3*sizeof(link));
		}
		else
			graph1.v[i].tos = (link*) malloc(5*sizeof(link));
	}

	for (i=0; i<N; i++) {
		for (j=0; j<M; j++) {
			scanf("%d", &tempWeight); /*??????*/
			if (tempWeight!=0) {
				graph1.v[1].tos[graph1.v[1].nTo].to = (M*i+j)+2;
				graph1.v[1].tos[graph1.v[1].nTo++].weight = tempWeight;
			}
		}
	}


	for (i=0; i<N; i++) {
		for (j=0; j<M; j++) {
			scanf("%d", &tempWeight); /*??????*/
			if (tempWeight!=0) {
				graph1.v[(M*i+j)+2].tos[graph1.v[(M*i+j)+2].nTo].to = 0; /*to vertex t*/
				graph1.v[(M*i+j)+2].tos[graph1.v[(M*i+j)+2].nTo++].weight = tempWeight;
			}
		}
	}

	for (i=0; i<N; i++) {
		for (j=0;j<M-1; j++) {
			scanf("%d", &tempWeight);
			if (tempWeight!=0) {
				graph1.v[(M*i+j)+2].tos[graph1.v[(M*i+j)+2].nTo].to = 1;
				graph1.v[(M*i+j)+2].tos[graph1.v[(M*i+j)+2].nTo++].weight = tempWeight;

				graph1.v[(M*(i+1ç)+j)+2].tos[graph1.v[(M*(i+1)+j)+2].nTo].to = 1;
				graph1.v[(M*(i+1)+j)+2].tos[graph1.v[(M*(i+1)+j)+2].nTo++].weight = tempWeight;
			}
		}
	}

	for (i=0; i<N-1; i++) {
		for (j=0;j<M; j++) {
			scanf("%d", &tempWeight);
			if (tempWeight!=0) {
				graph1.v[(M*i+j)+2].tos[graph1.v[(M*i+j)+2].nTo].to = 1;
				graph1.v[(M*i+j)+2].tos[graph1.v[(M*i+j)+2].nTo++].weight = tempWeight;

				graph1.v[(M*i+(j+1))+2].tos[graph1.v[(M*i+(j+1))+2].nTo].to = 1;
				graph1.v[(M*i+(j+1))+2].tos[graph1.v[(M*i+(j+1))+2].nTo++].weight = tempWeight;
			}
		}
	}
	return 0;
}


int getNumAdjs(int vertNum) {
	return graph1.v[vertNum].nTo; 
}

int getNumVer() {
	return graph1.nVert;
}
 

int main(int argc, char** argv) {
	createGraph();

	return 0;
}
