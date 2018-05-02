#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>

#define min(u,v) (u<v) ? u: v;

#define white 0
#define grey 1
#define black 2


/*2 PROJECTO ASA 2017/2018
	GRUPO 26 - LEIC-T
Catarina Pedreira n.87524
Miguel Oliveira n.87689
*/


/*STRUCTURES*/

/*fifo*/

typedef struct {
	int *buff;
	int head;
	int tail;
	int size;
} fifo;

/*graph*/

typedef struct {
	int from;
	int to;
	int capacity;
} link;

typedef struct {
	int d; /*visited*/
	int nTo;
	link* tos; /*static*/
	link *pi;
} vertex;

typedef struct {
	int nVert;
	int N, M;
	vertex* v; /*static*/
} graph;


/*GLOBAL VARIABLES*/

graph graph1;
fifo fifo1;
int flow;


/*FUNCTIONS*/

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
	graph1.v[vertNum].d = grey;
}

int poll() {
	if (fifo1.head == fifo1.tail) {
		exit(1);
	}
	int temp = fifo1.buff[fifo1.tail--];
	if (fifo1.tail<0) fifo1.tail = fifo1.size-1;
	graph1.v[temp].d = black;
	return temp;
}

int isEmpty() {
	return (fifo1.head == fifo1.tail);
}



/*graph functions*/

void createLink(int from, int to, int capacity) {
	graph1.v[from].tos[graph1.v[from].nTo].to = to;
	graph1.v[from].tos[graph1.v[from].nTo].from = from;
	graph1.v[from].tos[graph1.v[from].nTo].capacity = capacity;
	graph1.v[from].nTo++;
}


int createGraph() {
	int i,j;
	int N, M;
	int Ntemp, Mtemp;
	int tempCapacity;
	int err;
	int *tempLinks;
	int cont = 0;
	int tempWeight = 0;

	err = scanf("%d %d", &N, &M);
	graph1.nVert = N*M+2;
	graph1.N = N;
	graph1.M = M;

	graph1.v = (vertex*) malloc(sizeof(vertex)*((N*M)+2)); /*+2 to have s and t*/
	tempLinks = (int*) malloc(M*N*sizeof(int));
	if (graph1.v == NULL) {
		printf("ERRO malloc 1 graph\n");
		exit(1);
	}

	/*all vertexes have 5, 4 or 3 tos, except vertexes s (with N*M) and t (with 0)*/

	/*create vertex t (last index)*/
	graph1.v[(N*M)+1].d = 0;
	graph1.v[(N*M)+1].nTo = 0;
	graph1.v[(N*M)+1].tos = NULL;
	graph1.v[(N*M)+1].pi = NULL;

	/*create vertex s (first index)*/
	graph1.v[0].d = 0;
	graph1.v[0].nTo = 0;
	graph1.v[0].tos = (link*) malloc(N*M*sizeof(link));
	graph1.v[0].pi = NULL;


	/*create all other vertexes*/
	for (i=1; i<(N*M)+1; i++) {
		graph1.v[i].d = 0;
		graph1.v[i].nTo = 0;
		graph1.v[i].pi = NULL;
		Ntemp = (i-1)/N;
		Mtemp = (i-1)%N;
		if (Ntemp==0 || Ntemp==(N-1))
			if (Mtemp==0 || Mtemp==(M-1)) {
				graph1.v[i].tos = (link*) malloc(3*sizeof(link));
			}
			else {
				graph1.v[i].tos = (link*) malloc(4*sizeof(link));
			}
		else if (Mtemp==0 || Mtemp==(M-1)) {
			if (Ntemp!=0 || Ntemp!=0) {
				graph1.v[i].tos = (link*) malloc(3*sizeof(link));
			}
		}
		else {
			graph1.v[i].tos = (link*) malloc(5*sizeof(link));
		}
	}

	for (i=0; i<N; i++) {  /*from vertex s (0) to all*/
		for (j=0; j<M; j++) {
			err = scanf("%d", &tempCapacity);
			tempLinks[cont++] = tempCapacity;
		}
	}

	cont = 0;
	for (i=0; i<N; i++) { /*from all to vertex t*/
		for (j=0; j<M; j++) {
			err = scanf("%d", &tempCapacity);
			tempWeight = min(tempCapacity, tempLinks[cont]);
			flow+=tempWeight;
			if (tempLinks[cont] != tempCapacity) {
				if ( tempCapacity-tempWeight != 0) {
					createLink(((M*i)+j)+1, (N*M)+1, tempCapacity-tempWeight);
				}
				if (tempLinks[cont]-tempWeight != 0) {
					createLink(0, ((M*i)+j)+1, tempLinks[cont]-tempWeight);
				}
			}
			cont++;
		}
	}

	/*origin = (M*i+j)+1;  dest = (M*i+(j+1)+1)*/
	for (i=0; i<N; i++) {
		for (j=0;j<M-1; j++) {
			err = scanf("%d", &tempCapacity);
			if (tempCapacity!=0) {
				createLink((M*i+j)+1, (M*i+(j+1)+1), tempCapacity);
				createLink(M*i+(j+1)+1, (M*i+j)+1, tempCapacity);
			}
		}
	}


	/*origin = (M*i+j)+1;  dest = (M*(i+1)+j)+1*/
	for (i=0; i<N-1; i++) {
		for (j=0;j<M; j++) {
			err = scanf("%d", &tempCapacity);
			if (tempCapacity!=0) {
				createLink((M*i+j)+1, (M*(i+1)+j)+1, tempCapacity);
				createLink((M*(i+1)+j)+1 , (M*i+j)+1, tempCapacity);
			}
		}
	}
	err++;
	return 0;
}


int edmondsKarp(int s, int t) {
    
    int i;
    int cur;
    link *e;
    int df;

    do {
        /*(Run a bfs to find the shortest s-t path.
         We use 'pred' to store the edge taken to get to each vertex,
         so we can recover the path afterwards)*/
    	for (i=0; i<graph1.nVert; i++)
    		graph1.v[i].d = white;

    	initFifo(graph1.N*graph1.M+2); /*empty fifo*/
        add(s); /* Adicionamos o vértice S*/

        while (isEmpty()==0) {
            cur = poll();
			for (i=0; i<graph1.v[cur].nTo; i++) { /*i is the number of the edge inside the vertex*/
                 if(graph1.v[graph1.v[cur].tos[i].to].d == 0 && graph1.v[cur].tos[i].to != s && graph1.v[cur].tos[i].capacity > 0) {
                    graph1.v[graph1.v[cur].tos[i].to].pi = &(graph1.v[cur].tos[i]);
                    add(graph1.v[cur].tos[i].to);
                 }
             }
        }

        if (graph1.v[t].d != 0) {     
            /*(We found an augmenting path.
             See how much flow we can send) */
        	df = graph1.v[t].pi->capacity;
			for (e = graph1.v[t].pi; e != NULL; e = graph1.v[e->from].pi) {
				df = min(df, e->capacity);
			}

			flow+=df;
            for (e = graph1.v[t].pi; e != NULL; e = graph1.v[e->from].pi) {
                e->capacity -= df;
            }
        }
    
    } while(graph1.v[t].d != 0); /*(i.e., until no augmenting path was found)*/

    return 0;
}


void printVert() {
	int i, j;

	printf("%d\n\n", flow);
	for (i=0; i<graph1.N; i++) {
		for (j=0; j<graph1.M; j++) {
			if (graph1.v[(graph1.M*i+j)+1].d == 2)
				printf("C ");
			else
				printf("P ");
		}
		printf("\n");
	}
}


int main(int argc, char** argv) {
	flow = 0;
	createGraph();
	edmondsKarp(0, graph1.N*graph1.M+1);
	printVert();
	return 0;
}
