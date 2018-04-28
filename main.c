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


/*fifo*/

typedef struct {
	int *buff;
	int *head;
	int *tail;
} fifo;

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

/*fifo functions*/
int initFifo(int size) {
	
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
    
    int flow = 0;
    do {
        /*(Run a bfs to find the shortest s-t path.
         We use 'pred' to store the edge taken to get to each vertex,
         so we can recover the path afterwards)*/
        q := queue()
        q.push(s)
        pred := array(graph.length)
        while not empty(q)
            cur := q.poll()
            for Edge e in graph[cur]
                 if pred[e.to] = null and e.t ≠ s and e.cap > e.flow
                    pred[e.t] := e
                    q.push(e.t)
    
        if not (pred[t] = null)         
            (We found an augmenting path.
             See how much flow we can send) 
            df := ∞
            for (e := pred[t]; e ≠ null; e := pred[e.s])
                df := min(df, e.cap - e.flow)
            (And update edges by that amount)
            for (e := pred[t]; e ≠ null; e := pred[e.s])
                e.flow  := e.flow + df
                e.rev.flow := e.rev.flow - df
            flow := flow + df
    
    } while( pred[t] = NULL)  /*(i.e., until no augmenting path was found)*/
    return 0;
}

/*def edmonds_karp(C, source, sink):
    n = len(C) /*C is the capacity matrix
    F = [[0] * n for _ in xrange(n)]
    # residual capacity from u to v is C[u][v] - F[u][v]

    while (1) {
        path = bfs(C, F, source, sink)
        if not path:
            break
        # traverse path to find smallest capacity
        u,v = path[0], path[1]
        flow = C[u][v] - F[u][v]
        for i in xrange(len(path) - 2):
            u,v = path[i+1], path[i+2]
            flow = min(flow, C[u][v] - F[u][v])
        # traverse path to update flow
        for i in range(len(path) - 1):
            u,v = path[i], path[i+1]
            F[u][v] += flow
            F[v][u] -= flow
    }
    return sum([F[source][i] for i in xrange(n)])


def bfs(C, F, source, sink):
    P = [-1] * len(C) # parent in search tree
    P[source] = source
    queue = [source]
    while queue:
        u = queue.pop(0)
        for v in xrange(len(C)):
            if C[u][v] - F[u][v] > 0 and P[v] == -1:
                P[v] = u
                queue.append(v)
                if v == sink:
                    path = []
                    while True:
                        path.insert(0, v)
                        if v == source:
                            break
                        v = P[v]
                    return path
    return None
*/

int main(int argc, char** argv) {
	createGraph();
	printGraph();
	return 0;
}
