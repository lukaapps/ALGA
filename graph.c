/*
graph.c

Set of vertices and edges implementation.

Implementations for helper functions for graph construction and manipulation.

Skeleton written by Grady Fitzpatrick for COMP20007 Assignment 1 2021
*/
#include <stdlib.h>
#include <assert.h>
#include <limits.h>
#include "graph.h"
#include "list.h"
#include "utils.h"
#include "adj.h"
#include "pq.h"

#define INITIALEDGES 32
#define INFIN 999

struct edge;
int count = 0;

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

struct graph *newGraph(int numVertices){
  struct graph *g = (struct graph *) malloc(sizeof(struct graph));
  assert(g);
  /* Initialise edges. */
  g->numVertices = numVertices;
  g->numEdges = 0;
  g->allocedEdges = 0;
  g->edgeList = NULL;
  return g;
}

/* Adds an edge to the given graph. */
void addEdge(struct graph *g, int start, int end){
  assert(g);
  struct edge *newEdge = NULL;
  /* Check we have enough space for the new edge. */
  if((g->numEdges + 1) > g->allocedEdges){
    if(g->allocedEdges == 0){
      g->allocedEdges = INITIALEDGES;
    } else {
      (g->allocedEdges) *= 2;
    }
    g->edgeList = (struct edge **) realloc(g->edgeList,
      sizeof(struct edge *) * g->allocedEdges);
    assert(g->edgeList);
  }

  /* Create the edge */
  newEdge = (struct edge *) malloc(sizeof(struct edge));
  assert(newEdge);
  newEdge->start = start;
  newEdge->end = end;

  /* Add the edge to the list of edges. */
  g->edgeList[g->numEdges] = newEdge;
  (g->numEdges)++;
}

void removeVertex(int * adjMat[], int vertex, int n){
  int i;
  while (vertex < n){
    for(i = 0; i<n;i++){
      adjMat[i][vertex] = adjMat[i][vertex+1];
    }
    for(i = 0; i <n; i++){
      adjMat[vertex][i] = adjMat[vertex+1][i];
    }
    vertex++;
  }
}
/* Frees all memory used by graph. */
void freeGraph(struct graph *g){
  int i;
  for(i = 0; i < g->numEdges; i++){
    free((g->edgeList)[i]);
  }
  if(g->edgeList){
    free(g->edgeList);
  }
  free(g);
}

/* Finds:
  - Number of connected subnetworks (before outage) (Task 2)
*/
 void DFS(int vertex, int visited[], int numVerticies, int * adjMat[]){
  int j;
  visited[vertex] = 1;
  for(j = 0; j < numVerticies; j++){
    if(visited[j]!= 1 && adjMat[vertex][j] == 1){
      DFS(j, visited, numVerticies, adjMat);
    }
  }
  return;
  
}
void countComponents(struct graph *g, int numVerticies, int visited[], int * adjMat[]){
  int j;
  for(j= 0; j<numVerticies; j++){
      if(!visited[j]){
        count++;
        DFS(j, visited, numVerticies, adjMat);
      }
    }
    return;
}
 /*
  - Number of servers in largest subnetwork (before outage) (Task 3)
  - SIDs of servers in largest subnetwork (before outage) (Task 3)
  - Diameter of largest subnetworks (after outage) (Task 4)
  - Number of servers in path with largest diameter - should be one more than
    Diameter if a path exists (after outage) (Task 4)
  - SIDs in path with largest diameter (after outage) (Task 4)
  - Number of critical servers (before outage) (Task 7)
  - SIDs of critical servers (before outage) (Task 7)
*/
struct solution *graphSolve(struct graph *g, enum problemPart part,
  int numServers, int numOutages, int *outages){
  struct solution *solution = (struct solution *)
    malloc(sizeof(struct solution));
  assert(solution);
  /* Initialise solution values */
  
  initaliseSolution(solution);
  int numVertices = g->numVertices;
  int visited[numVertices];
  int * adjMat[numVertices];
  adjacentMatrix(numVertices, g, adjMat);
  int i;
  for(i = 0; i < numVertices; i++){
        visited[i] = 0;
    }
  if(part == TASK_2){
    /* IMPLEMENT TASK 2 SOLUTION HERE */
    countComponents(g, numVertices, visited, adjMat);
    solution->connectedSubnets = count;

  } else if(part == TASK_3) {
    /* IMPLEMENT TASK 3 SOLUTION HERE */
    int m = 0;
    int largest = 0;
    countComponents(g, numVertices, visited, adjMat);
    void *alloc = malloc(sizeof(int));
    int *trial = (int *)alloc;
    int j;
    for(i = 0; i < numVertices; i++){
        visited[i] = 0;
    }
    for(j= 0; j<numVertices; j++){
      if(!visited[j]){
        trial[m] = j;
        m++;
        DFS(j, visited, numVertices, adjMat);
      }
    }
    int index = 0;
    int n;
    int differenceList[count-1];
    trial[count] = numVertices;
    if(count > 1){      
      j = 0;
      for(i = 1; i <= count; i++){
        differenceList[j] = trial[i] - trial[j];
        j++;
      }
      largest = differenceList[0];
      for(n = 0; n < count; n++){
        if(differenceList[n] > largest){
          largest = differenceList[n];
          index = n;
        }

      }
    }
    else{
      largest = numVertices;
    }
    void *memory = malloc(sizeof(int));
    int *value = (int *)memory;
    int check = 0;
    check += largest;
    solution->largestSubnet = check;
    

    int b = 0;
    int k;
    
    for(k = trial[index]; k < trial[index+1]; k++){
      value[b] = k;
      b++;
      
    }
    solution->largestSubnetSIDs = value;
    
    
    
  } else if(part == TASK_4) {
    /* IMPLEMENT TASK 4 SOLUTION HERE */
    int distMat[numVertices][numVertices], n, j;
    for(i = 0; i < numVertices; i++){
      for(j = 0; j < numVertices; j++){
        if(adjMat[i][j] ==0 && j!=i){
          distMat[i][j] = INFIN;
        }
        else{
          distMat[i][j] = adjMat[i][j];
        }
    
      }
    }
    for(i =0; i < numVertices; i++){
      for(j = 0; j<numVertices; j++){
        for(n = 0; n < numOutages; n++){
          if(i == outages[n] || j == outages[n]){
            distMat[i][j] = INFIN;
            break;
          }
        }
      }
    }
    int k, g, h;
    for(k = 0; k < numVertices; k++){
      for(g = 0; g < numVertices; g++){
        for(h = 0; h < numVertices; h++){
          if(distMat[g][k] + distMat[k][h] < distMat[g][h]){
            distMat[g][h] = distMat[g][k] + distMat[k][h];
          }
        }
      }
    }
    int max = 0;
    int s, d;
    
    for(i = 0; i < numVertices; i++){
      for(j = 0; j < numVertices; j++){
        if(distMat[i][j] > max && distMat[i][j] != INFIN){
          max = distMat[i][j];
          s= i;
          d = j;
        }
      }
    }
    void *memory = malloc(sizeof(int));
    int *value = (int *)memory;
    int b = 0;
    value[b] = s;
    b++;
    for(i = s; i <= d; i++){
      for(j = i; j <= d; j++){
        if(distMat[i][j]==1){
          value[b] = j;
          b++;
          i = j;
          break;
        }
        else{
          continue;
        }
      }
    }
    value[b] = d;
    solution->postOutageDiameter = max;
    solution->postOutageDiameterCount = max + 1;
    solution->postOutageDiameterSIDs = value;
  } else if(part == TASK_7) {
    /* IMPLEMENT TASK 7 SOLUTION HERE */
    int artPoint = 0;
    void *collect = malloc(sizeof(int));
    int * criticalPoints = (int *)collect;
    int b = 0;
    int j;
    for (i = 0; i < numVertices; i++){
      visited[i] = 0;
    }
    void *memory = malloc(sizeof(int));
    int * temp = (int *)memory;
    countComponents(g, numVertices, visited, adjMat);
    int initial_val = count;
    count = 0;
    
    for(i=0; i < numVertices; i++){
      for(j = 0; j < numVertices; j++){
        visited[j] = 0;
        temp[j] = adjMat[i][j];
        adjMat[i][j] =0;
        adjMat[j][i] = 0;
      }
      int newCount = 0;
      for(j = 0; j<numVertices;j++){
        if(visited[j] == 0 && j != i){
          newCount++;
          DFS(j, visited, numVertices,adjMat );
        }

      }
      if(newCount > initial_val){
        artPoint++;
        criticalPoints[b] = i; 
        b++;
      }
      for(j = 0; j<numVertices; j++){
        adjMat[i][j] = temp[j];
        adjMat[j][i] = temp[j];
      }
    }
    
    solution->criticalServerCount = artPoint;
    solution->criticalServerSIDs = criticalPoints;
  }
  return solution;
  free(solution);
}