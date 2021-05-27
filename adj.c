#include "adj.h"
#include <stdlib.h>
#include <assert.h>
#include "graph.h"
#include "pq.h"

struct edge;

/* Definition of a graph. */
struct graph {
  int numVertices;
  int numEdges;
  int allocedEdges;
  struct edge **edgeList;
};

/* Definition of an edge. */
struct edge {
  int start;
  int end;
};

void adjacentMatrix(int num, struct graph *g, int * adjMat[]){
    int i, j;
    //Assigning memory to the adjacency matrix
    for (i = 0; i<num; i++){
      adjMat[i] = (int *) malloc((num) * sizeof(int));
      
    }
    assert(adjMat);

    for (i = 0; i < num; i++){
      for(j = 0; j < num; j++){
        adjMat[i][j] = 0;
      }
    }
    //similar to the add edge function in graph.c we will create a two dimensional adj matrix
    for (i =0; i<g->numEdges;i++){
        int start = g->edgeList[i]->start;
        int end = g->edgeList[i]->end;
        //We get the trailing value to assess if the next edge exists 
        int trailValue = adjMat[start][end];
        if((trailValue > 0)||trailValue == 0){
            adjMat[start][end] = 1;
            adjMat[end][start] = 1;

        }

    }
}


