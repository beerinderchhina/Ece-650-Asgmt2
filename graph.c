/**
 * This program reads graph spec from standard input
 * and print shortest path between vertices.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* assume at most 1000 vertices in any graph in the input */
#define MAX_SIZE 1000

/* assume the edge line has at most 4000 characters */
#define MAX_LINE 4000

/* function to read edges for current graph of size V */
void readEdges(int V, char adj[][MAX_SIZE]);

/* calculate the shortest path between the given two vertices
 * using bread-first search.
 * The parent array stores the parent vertex for each vertices
 * visited during BFS, so the path can be recovered. */
void BFS(int V, char adj[][MAX_SIZE], int parent[], int src, int des);

/* recover path for the given destination recursively */
void recoverPath(int parent[], int des);

int main()
{
  /* use adjacent matrix to store the undirected graph */
  int V;
  char adj[MAX_SIZE][MAX_SIZE];
  int parent[MAX_SIZE];

  int src, des; /* the vertices to calculate shortest path */
  char command[2];

  /* read the first character as the command.
   * until EOF */
  while (scanf("%s", command) == 1) {
    char c = command[0];
    switch (c) {

      case 'V':
        /* start a new graph, need to reset adjacent matrix */
        scanf("%d", &V);
        memset(adj, 0, sizeof(adj));
        break;

      case 'E':
        /* load edges for the current graph */
        memset(adj, 0, sizeof(adj));
        readEdges(V, adj);
        break;

      case 's':
        /* display shortest path between vertices */
        scanf("%d %d", &src, &des);
        memset(parent, -1, sizeof(parent));
        BFS(V, adj, parent, src, des);
        if (parent[des] == -1) {
          /* no path found, display an error */
          printf("Error: no path is found between %d and %d.\n", src, des);
        } else {
          recoverPath(parent, des);
          printf("\n");
        }
        break;
    }
  }
}


/* function to read edges for current graph of size V */
void readEdges(int V, char adj[][MAX_SIZE])
{
  char *spec, *token;
  int from, to;

  /* read the entire line as the edges */
  char line[MAX_LINE];
  fgets(line, sizeof(line), stdin);

  /* find the edge part in { and } */
  strtok(line, "{");
  spec = strtok(NULL, "}");
  /* use , to split into different pairs */
  token = strtok(spec, ",");
  while (token != NULL) {
    /* read the two vertices of this edge */
    from = atoi(&token[1]);
    token = strtok(NULL, ",");
    to = atoi(token);
    /* add the pair into the adjacent list
     * make sure the vertices are valid (0 to V - 1)*/
    if (from < 0 || from >= V || to < 0 || to >= V) {
      printf("Error: the pair <%d,%d> is invalid.\n", from, to);
    } else { /* otherwise, add into the adjacent matrix */
      adj[from][to] = 1;
      adj[to][from] = 1;
    }
    /* get next pair */
    token = strtok(NULL, ",");
  }
}

/* calculate the shortest path between the given two vertices
 * using bread-first search.
 * The parent array stores the parent vertex for each vertices
 * visited during BFS, so the path can be recovered. */
void BFS(int V, char adj[][MAX_SIZE], int parent[], int src, int des)
{

  int i, j;
  int queue[MAX_SIZE]; /* use queue to perform BFS */
  int size = 0;

  parent[src] = src;
  queue[size ++] = src;
  for (i = 0; i < size; i++) {
    /* remove the first one from the queue */
    int curr = queue[i];

    if (curr == des) {
      /* found the destination */
      return;
    }

    /* visit all adjacent vertices if not visited yet */
    for (j = 0; j < V; j++) {
      if (adj[curr][j] && parent[j] == -1) {
        /* update the parent of the neighbor and put it into the queue */
        parent[j] = curr;
        queue[size ++] = j;
      }
    }
  }
}

/* recover path for the given destination recursively */
void recoverPath(int parent[], int des)
{
  if (parent[des] == des) {
    printf("%d", des);
  } else {
    recoverPath(parent, parent[des]);
    printf("-%d", des);
  }
}


