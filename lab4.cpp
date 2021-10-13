// Name: lab4.cpp
// Author: Brianna Drew
// ID: #0622446
// Date Created: 2021-10-04
// Last Modified: 2021-10-12

#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>

int main (int argc, char** argv)
{
  int procNum, procRank, m, n; // Number of processes, rank of calling process, size of array
  int sumProc = 0, sumAll = 0; // Sum of process, sum of all processes
  int** arr; // Array
  MPI_Status status;

  MPI_Init(NULL, NULL); // Initialize MPI
  MPI_Comm_size(MPI_COMM_WORLD, &procNum); // Get number of processes
  MPI_Comm_rank(MPI_COMM_WORLD, &procRank); // Get rank of calling process

  if (procRank == 0) {
    m = 4; // Number of rows
    n = 5; // Number of columns
  }
  // Broadcast size of array to nodes
  MPI_Bcast(&m, 1, MPI_INT, 0, MPI_COMM_WORLD);
  MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);

  // Create static array
  int sample_array[4][5] = {
  {50, 55, 62, 70, 85},
  {35, 42, 45, 47, 49},
  {32, 33, 36, 37, 38},
  {25, 30, 30, 35, 30}
  };

  // Create dynamic array
  arr = new int*[m];
  for (int i = 0; i < m; i++) {
    arr[i] = new int[n];
  }

  if (procRank == 0) {
    // Fill array with values of static array
    for (int i = 0; i < m; i++) {
      for (int j = 0; j < n; j++) {
        arr[i][j] = sample_array[i][j];
        printf("%i ", arr[i][j]);
      }
      printf("\n");
    }
  }

  // Broadcasts the array to each node
  for (int i = 0; i < m; i++) {
    MPI_Bcast(arr[i], n, MPI_INT, 0, MPI_COMM_WORLD);
  }

  // You need two lines of code here to sum the array (rows and columns)
  for (int i = procRank; i <= m; i += procNum) {
    for (int j = procRank; j <= n; j+= procNum) {
      sumProc += arr[i][j];
    }
  }

  sumProc += 1;
  MPI_Reduce(&sumProc, &sumAll, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

  if (procRank == 0) {
    printf("sumAll = %i \n", sumAll);
  }

  delete *arr; // delete dynamic array from memory

  MPI_Finalize(); // Finalize MPI
  return 0;
}
