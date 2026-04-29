#include <stdio.h>
#include <stdlib.h>

int m1[3][3] = { 
  { 1, 1, 1 }, 
  { 2, 2, 1 },
  { 0, 0, 0 }
};

int m2[3][3] = { 
  { 1, 1, 1 },
  { 2, 2, 2 },
  { 2, 2, 7 }
};

int result[3][3] = {0};


void multiplyMatrix(int m1[3][3], int m2[3][3], int result[3][3];) {
    for (int row = 0; row < 3; row++) {
        for (int col = 0; col < 3; col++) {
			
            result[row][col] = 0; // init to 0 ; clear from garbage

            for (int k = 0; k < 3; k++) {
                result[row][col] += m1[row][k] * m2[k][col];
            }
			
			printf("%d\t", result[row][col]);
        }
		printf("/n");
    }
}


int main() {
    multiplyMatrix(m1, m2, result);
    return 0;
}