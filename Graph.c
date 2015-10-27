/**
 * This program reads graph spec from standard input
 * and print shortest path between vertices.
 */

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>



/* function to read edges for current graph of size V */
void readEdges(int V, char *adj);

/* calculate the shortest path between the given two vertices
 * using bread-first search.
 * The parent array stores the parent vertex for each vertices
 * visited during BFS, so the path can be recovered. */
void BFS(int V, char *adj, int parent[], int src, int des);

/* recover path for the given destination recursively */
void recoverPath(int parent[], int des);

int main()
{
  /* use adjacent matrix to store the undirected graph */
  int V;
  char *adj = NULL;
  int *parent = NULL;

  int src, des; /* the vertices to calculate shortest path */
  char command[2];

  /* read the first character as the command.
   * until EOF */
  while (scanf("%s", command) == 1) {
    char c = command[0];
    switch (c) {

      case 'V':
      case 'v':
        /* start a new graph, need to reset adjacent matrix */
        scanf("%d", &V);
        if (V <= 0) {
          printf("Error: number of vertices must be positive.\n");
          break;
        } else {
          /* dynamic create the array for adjacent matrix
           * and parent array */
          if (adj != NULL) { /* free the old one */
            free(adj);
            free(parent);
          }
          adj = (char *)malloc(sizeof(char) * V * V);
          parent = (int *)malloc(sizeof(int) * V);
          if (adj == NULL || parent == NULL) {
            printf("Error: could not allocate memory for arrays.\n");
            exit(-1);
          }
        }
        break;

      case 'E':
      case 'e':
        /* load edges for the current graph */
        memset(adj, 0, V * V);
        readEdges(V, adj);
        break;

      case 's':
      case 'S':
        /* display shortest path between vertices */
        scanf("%d %d", &src, &des);
        if (src < 0 || src >= V || des < 0 || des >= V) {
          printf("Error: either vertex %d or %d is invalid.\n", src, des);
          break;
        }
        memset(parent, -1, sizeof(int) * V);
        BFS(V, adj, parent, src, des);
        if (parent[des] == -1) {
          /* no path found, display an error */
          printf("Error: no path is found between %d and %d.\n", src, des);
        } else {
          recoverPath(parent, des);
          printf("\n");
        }
        break;
      default:
        printf("Error: Not valid input");
    }
  }
  if (adj != NULL) { /* free the dynamic array */
    free(adj);
    free(parent);
  }
return 0;
}


/* function to read edges for current graph of size V */
void readEdges(int V, char *adj)
{
  char *spec, *token;
  int from, to;

  /* read the entire line as the edges */
  char *line = NULL;
  size_t n;
  getline(&line, &n, stdin);

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
      adj[from * V + to] = 1;
      adj[to * V + from] = 1;
    }
    /* get next pair */
    token = strtok(NULL, ",");
  }

  free(line);
}

/* calculate the shortest path between the given two vertices
 * using bread-first search.
 * The parent array stores the parent vertex for each vertices
 * visited during BFS, so the path can be recovered. */
void BFS(int V, char *adj, int parent[], int src, int des)
{

  int i, j;
  int *queue; /* use queue to perform BFS */
  int size = 0;

  queue = (int *)malloc(sizeof(int) * V);
  if (queue == NULL) {
    printf("Error: could not allocate memory for arrays.\n");
    exit(-1);
  }

  parent[src] = src;
  queue[size ++] = src;
  for (i = 0; i < size; i++) {
    /* remove the first one from the queue */
    int curr = queue[i];

    if (curr == des) {
      /* found the destination */
      break;
    }

    /* visit all adjacent vertices if not visited yet */
    for (j = 0; j < V; j++) {
      if (adj[curr * V + j] && parent[j] == -1) {
        /* update the parent of the neighbor and put it into the queue */
        parent[j] = curr;
        queue[size ++] = j;
      }
    }
  }
  free(queue);
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


