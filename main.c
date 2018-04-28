#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#define inf -1
#define True 1
#define False 0

/*2 PROJECTO ASA 2017/2018
	GRUPO 26 - LEIC-T
Catarina Pedreira n.87524
Miguel Oliveira n.87689
*/


/*fifo*/

typedef struct {
	int *buff;
	int head;
	int tail;
	int size;
} fifo;

/*list*/

typedef struct node { 
	int vNum;
	int weight;
	struct node *next; 
}LLElem;

/*graph*/

typedef struct {
	int from;
	int to;
	int weight;
	int flow;
	int residual;
} link;

typedef struct {
	int d;
	int low;
	int nTo;
	link* tos; //static
} vertex;

typedef struct {
	int nVert;
	int N, M;
	vertex* v; //static
} graph;

graph graph1;
fifo fifo1;

/*fifo functions*/
int initFifo(int size) {
	if (fifo1.buff == NULL)
		fifo1.buff = (int*) malloc(size*sizeof(int));
	if (fifo1.buff == NULL) {
		printf("Erro malloc fifo\n");
		exit(1);
	}
	fifo1.head = 0;
	fifo1.tail = 0;
	fifo1.size = size;
	return 0;
}

void add(int vertNum) {
	fifo1.buff[fifo1.head--] = vertNum;
	if (fifo1.head<0) fifo1.head = fifo1.size-1;
}

int remove() {
	if (fifo1.head == fifo1.tail) {
		printf("Erro add fifo\n");
		exit(1);
	}
	int temp = fifo1.buff[fifo1.tail--];
	if (fifo1.tail<0) fifo1.tail = fifo1.size-1;
	return temp;
}

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


int createLink(int from, int to, int weight) {
	graph1.v[from].tos[graph1.v[from].nTo].to = to;
	graph1.v[from].tos[graph1.v[from].nTo].from = from;
	graph1.v[from].tos[graph1.v[from].nTo].weight = weight;
	graph1.v[from].tos[graph1.v[from].nTo].flow = 0;
	graph1.v[from].tos[graph1.v[from].nTo].residual = 0;
}



int createGraph() {
	int i,j;
	int N, M;
	int Ntemp, Mtemp;
	int tempWeight;

	scanf("%d %d", &N, &M);
	graph1.nVert = N*M+2; /*we increment this as we add more vertexes NOP*/
	graph1.N = N;
	graph1.M = M;

	graph1.v = (vertex*) malloc(sizeof(vertex)*((N*M)+2)); /*+2 to have s and t*/
	if (graph1.v == NULL) {
		printf("ERRO malloc 1 graph\n");
		exit(1);
	}

	/*all vertexes have 5, 4 or 3 tos, except vertexes s (with N*M) and t (with 0)*/

	/*create vertex t (last index)*/
	graph1.v[(N*M)+1].d = 0;
	graph1.v[(N*M)+1].low = 0;
	graph1.v[(N*M)+1].nTo = 0;
	graph1.v[(N*M)+1].tos = NULL;

	/*create vertex s (first index)*/
	graph1.v[0].d = 0;
	graph1.v[0].low = 0;
	graph1.v[0].nTo = 0;
	graph1.v[0].tos = (link*) malloc(N*M*sizeof(link));

	/*create all other vertexes*/
	for (i=1; i<(N*M)+1; i++) {
		graph1.v[i].d = 0;
		graph1.v[i].low = 0;
		graph1.v[i].nTo = 0;
		Ntemp = (i-1)/N;
		Mtemp = (i-1)%N;
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

	for (i=0; i<N; i++) {  /*from vertex s (0) to all*/
		for (j=0; j<M; j++) {
			scanf("%d", &tempWeight);
			if (tempWeight!=0) {
				graph1.v[0].tos[graph1.v[0].nTo].to = (M*i+j)+1;
				graph1.v[0].tos[graph1.v[0].nTo++].weight = tempWeight;
			}
		}
	}


	for (i=0; i<N; i++) { /*from all to vertex t*/
		for (j=0; j<M; j++) {
			scanf("%d", &tempWeight);
			if (tempWeight!=0) {
				graph1.v[(M*i+j)+1].tos[graph1.v[(M*i+j)+1].nTo].to = (N*M)+1;
				graph1.v[(M*i+j)+1].tos[graph1.v[(M*i+j)+1].nTo++].weight = tempWeight;
			}
		}
	}


	/*origin = (M*i+j)+1;  dest = (M*i+(j+1)+1)*/
	for (i=0; i<N; i++) {
		for (j=0;j<M-1; j++) {
			scanf("%d", &tempWeight);
			if (tempWeight!=0) {
				graph1.v[(M*i+j)+1].tos[graph1.v[(M*i+j)+1].nTo].to = (M*i+(j+1)+1);
				graph1.v[(M*i+j)+1].tos[graph1.v[(M*i+j)+1].nTo++].weight = tempWeight;

				graph1.v[M*i+(j+1)+1].tos[graph1.v[M*i+(j+1)+1].nTo].to = (M*i+j)+1;
				graph1.v[M*i+(j+1)+1].tos[graph1.v[M*i+(j+1)+1].nTo++].weight = tempWeight;
			}
		}
	}


	/*origin = (M*i+j)+1;  dest = (M*(i+1)+j)+1*/
	for (i=0; i<N-1; i++) {
		for (j=0;j<M; j++) {
			scanf("%d", &tempWeight);
			if (tempWeight!=0) {
				graph1.v[(M*i+j)+1].tos[graph1.v[(M*i+j)+1].nTo].to = (M*(i+1)+j)+1;
				graph1.v[(M*i+j)+1].tos[graph1.v[(M*i+j)+1].nTo++].weight = tempWeight;

				graph1.v[(M*(i+1)+j)+1].tos[graph1.v[(M*(i+1)+j)+1].nTo].to = (M*i+j)+1;
				graph1.v[(M*(i+1)+j)+1].tos[graph1.v[(M*(i+1)+j)+1].nTo++].weight = tempWeight;
			}
		}
	}
	return 0;
}

void printGraph() {
	/*ONLY FOR DEBUG*/
	int i, j;
	int N = graph1.N;
	int M = graph1.M;
	int **matrix_d = (int**) malloc(((N*M)+2)*sizeof(int*));
	for (i=0; i<(N*M)+2; i++)
		matrix_d[i]= (int*) malloc(((N*M)+2)*sizeof(int));

	for (i=0; i<(M*N)+2; i++)
		for(j=0; j<(M*N)+2; j++)
			matrix_d[i][j] = 0;

	for (i=0; i<graph1.nVert; i++) {
		for (j=0; j<graph1.v[i].nTo; j++) {
			matrix_d[i][graph1.v[i].tos[j].to] = graph1.v[i].tos[j].weight;
		}
	}
	for (i=0; i<graph1.nVert; i++) {
		for (j=0; j<graph1.nVert; j++) {
			printf("%d ", matrix_d[i][j]);
		}
		printf("\n");
	}
}


int EdmondsKarp() {
	/*
    input:
        graph   (graph[v] should be the list of edges coming out of vertex v.
                 Each edge should have a capacity, flow, source and sink as parameters,
                 as well as a pointer to the reverse edge.)
        s       (Source vertex)
        t       (Sink vertex)
    output:
        flow    (Value of maximum flow)*/
    
    do {
        /*(Run a bfs to find the shortest s-t path.
         We use 'pred' to store the edge taken to get to each vertex,
         so we can recover the path afterwards)*/
        add(fifo, 0); /* Adicionamos o vértice S*/
        link* preds[graph1.N * graph1.M];
        int cur;
        int df = INFINITY;
        int totalFlow = 0;
        link e;

        while notEmpty(fifo){
            cur = poll(fifo);
			for (i=0; i<graph1.v[cur].nTo; i++){
                 if(preds[graph1.v[cur].tos[i]]== NULL && preds[graph1.v[cur].tos[i]] != graph1.v[0] && preds[graph1.v[cur].tos[i]].weight > preds[graph1.v[cur].tos[i]].flow){
                    preds[graph1.v[cur].tos[i]] = preds[graph1.v[cur]];
                    add(fifo, i); /*adiciona esta aresta*/
                 }
             }

        }
    
        if !(preds[graph1.v[t]] == NULL){ /* depois pôr a posição do t certa */        
            /*(We found an augmenting path.
             See how much flow we can send) */
			for (e = preds[graph1.v[t]]; e != NULL; e = preds[graph1.v[t.from]]) {
				df = min(df, graph1.v[t].weight - graph1.v[t].flow);
			}

            for (e = preds[graph1.v[t]]; e != NULL; e = pred[graph1.v[t.from]]){
                e.flow += df;
                e.residual = e.residual - df;
            }

            totalflow += df;
        }
    
    } while(preds[graph1.v[t]] == NULL); /*(i.e., until no augmenting path was found)*/

    return 0;
}

int min(a,b){
	if(a < b) return a;
	else return b;
}

int main(int argc, char** argv) {
	createGraph();
	printGraph();
	return 0;
}
