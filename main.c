#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <limits.h>

#define min(u,v) (u<v) ? u: v;


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
	int flow;
	int residual;
} link;

typedef struct {
	int d; /*visited*/
	int low;
	int nTo;
	link* tos; /*static*/
	int nFrom;
	link ** froms;
} vertex;

typedef struct {
	int nVert;
	int N, M;
	vertex* v; /*static*/
} graph;


/*GLOBAL VARIABLES*/

graph graph1;
fifo fifo1;


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
}

int poll() {
	if (fifo1.head == fifo1.tail) {
		printf("Erro add fifo\n");
		exit(1);
	}
	int temp = fifo1.buff[fifo1.tail--];
	if (fifo1.tail<0) fifo1.tail = fifo1.size-1;
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
	graph1.v[from].tos[graph1.v[from].nTo].residual = 0;
	graph1.v[from].tos[graph1.v[from].nTo].flow = 0;
	graph1.v[to].froms[graph1.v[to].nFrom] = &(graph1.v[from].tos[graph1.v[from].nTo]);
	graph1.v[from].nTo++;
	graph1.v[to].nFrom++;
}


int createGraph() {
	int i,j;
	int N, M;
	int Ntemp, Mtemp;
	int tempCapacity;
	int err;

	err = scanf("%d %d", &N, &M);
	graph1.nVert = N*M+2;
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
	graph1.v[(N*M)+1].nFrom = 0;
	graph1.v[(N*M)+1].froms = (link**) malloc(N*M*sizeof(link*));


	/*create vertex s (first index)*/
	graph1.v[0].d = 0;
	graph1.v[0].low = 0;
	graph1.v[0].nTo = 0;
	graph1.v[0].tos = (link*) malloc(N*M*sizeof(link));
	graph1.v[0].froms = NULL;
	graph1.v[0].nFrom = 0;


	/*create all other vertexes*/
	for (i=1; i<(N*M)+1; i++) {
		graph1.v[i].d = 0;
		graph1.v[i].low = 0;
		graph1.v[i].nTo = 0;
		Ntemp = (i-1)/N;
		Mtemp = (i-1)%N;
		if (Ntemp==0 || Ntemp==(N-1))
			if (Mtemp==0 || Mtemp==(M-1)) {
				graph1.v[i].tos = (link*) malloc(3*sizeof(link));
				graph1.v[i].froms = (link**) malloc(3*sizeof(link*));
			}
			else {
				graph1.v[i].tos = (link*) malloc(4*sizeof(link));
				graph1.v[i].froms = (link**) malloc(4*sizeof(link*));
			}
		else if (Mtemp==0 || Mtemp==(M-1)) {
			if (Ntemp!=0 || Ntemp!=0) {
				graph1.v[i].tos = (link*) malloc(3*sizeof(link));
				graph1.v[i].froms = (link**) malloc(3*sizeof(link*));
			}
		}
		else {
			graph1.v[i].tos = (link*) malloc(5*sizeof(link));
			graph1.v[i].froms = (link**) malloc(5*sizeof(link*));
		}
	}

	for (i=0; i<N; i++) {  /*from vertex s (0) to all*/
		for (j=0; j<M; j++) {
			err = scanf("%d", &tempCapacity);
			if (tempCapacity!=0) {
				createLink(0, ((M*i)+j)+1, tempCapacity);
			}
		}
	}


	for (i=0; i<N; i++) { /*from all to vertex t*/
		for (j=0; j<M; j++) {
			err = scanf("%d", &tempCapacity);
			if (tempCapacity!=0) {
				createLink(((M*i)+j)+1, (N*M)+1, tempCapacity);
			}
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
    link **preds; /*array of link* */
    int cur;
    link *e;
    int df;

    do {
        /*(Run a bfs to find the shortest s-t path.
         We use 'pred' to store the edge taken to get to each vertex,
         so we can recover the path afterwards)*/

    	initFifo(graph1.N*graph1.M+2); /*empty fifo*/
        add(s); /* Adicionamos o vértice S*/
        preds = (link**) malloc((graph1.N*graph1.M+2)*sizeof(link*)); /*in or out?*/

        while (isEmpty()==0) {
            cur = poll();
			for (i=0; i<graph1.v[cur].nTo; i++) { /*i is the number of the edge inside the vertex*/
                 if(preds[graph1.v[cur].tos[i].to] == NULL && graph1.v[cur].tos[i].to != s && graph1.v[cur].tos[i].capacity > graph1.v[cur].tos[i].flow) {
                    preds[graph1.v[cur].tos[i].to] = &(graph1.v[cur].tos[i]);
                    add(graph1.v[cur].tos[i].to);
                 }
             }
        }

    
        if (preds[t] != NULL) {     
            /*(We found an augmenting path.
             See how much flow we can send) */
        	df = INT_MAX;
			for (e = preds[t]; e != NULL; e = preds[e->from]) {
				df = min(df, e->capacity - e->flow);
			}

            for (e = preds[t]; e != NULL; e = preds[e->from]) {
                e->flow += df;
                e->residual = e->capacity - e->flow;
            }
        }
    
    } while(preds[t] != NULL); /*(i.e., until no augmenting path was found)*/

    return 0;
}




void DFS(int vNum) {
	int i;
	graph1.v[vNum].d = 1;

	for (i=0; i<graph1.v[vNum].nTo; i++) {
		if (graph1.v[vNum].tos[i].residual != 0 && graph1.v[graph1.v[vNum].tos[i].to].d != 1) {
			DFS(graph1.v[vNum].tos[i].to);
		}
	}
}


int calcCut(){
	int i,j;
	int sum = 0;
	for(i = 0; i<graph1.nVert; i++){
		for(j=0; j<graph1.v[i].nTo; j++){
			if(graph1.v[i].d == 1 && graph1.v[graph1.v[i].tos[j].to].d == 0){ /*Somamos o peso da ligacao de um visitado p nao visitado*/
				sum += graph1.v[i].tos[j].flow;
			}
		}
	}

	return sum;
}


void printVert() {
	int i, j;
	int s = calcCut();

	printf("%d\n\n", s);
	for (i=0; i<graph1.N; i++) {
		for (j=0; j<graph1.M; j++) {
			if (graph1.v[(graph1.M*i+j)+1].d == 1)
				printf("C ");
			else
				printf("P ");
		}
		printf("\n");
	}
}


int main(int argc, char** argv) {
	createGraph();
	edmondsKarp(0, graph1.N*graph1.M+1);
	DFS(0);
	printVert();
	return 0;
}
