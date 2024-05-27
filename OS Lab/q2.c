#include <stdio.h>
#include <pthread.h>


int matrix1[4][4], matrix2[4][4], result[4][4];


void* sum_column(void* arg) {
  int col = *(int*)arg;
  int sum = 0;
  for(int i = 0; i < 4; i++) {
    result[i][col] = matrix1[i][col] + matrix2[i][col];
  }
}

int main() {
  
  FILE* file1 = fopen("matrix1.txt", "r");
  FILE* file2 = fopen("matrix2.txt", "r");
  
  
  for(int i = 0; i < 4; i++) {
    for(int j = 0; j < 4; j++) {
      fscanf(file1, "%d", &matrix1[i][j]);
      fscanf(file2, "%d", &matrix2[i][j]);
    }
  }
  
  
  fclose(file1);
  fclose(file2);
  
 
  pthread_t t1, t2, t3, t4;
  int col1 = 0, col2 = 1, col3 = 2, col4 = 3;
  pthread_create(&t1, NULL, sum_column, &col1);
  pthread_create(&t2, NULL, sum_column, &col2);
  pthread_create(&t3, NULL, sum_column, &col3);
  pthread_create(&t4, NULL, sum_column, &col4);
  
 
  pthread_join(t1, NULL);
  pthread_join(t2, NULL);
  pthread_join(t3, NULL);
  pthread_join(t4, NULL);
  
 
  printf("The sum of two matrices is:\n");
  for(int i = 0; i < 4; i++) {
    for(int j = 0; j < 4; j++) {
      printf("%d ", result[i][j]);
    }
    printf("\n");
  }
  
  return 0;
}

