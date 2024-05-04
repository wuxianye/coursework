#include <stdio.h>
#include<stdlib.h>
#include <math.h>
#include <time.h>

#define NUMROWS	    9 // Number of rows in the map
#define NUMCOLS     9 //Number of columns in the map
#define NUMWALKS    1000 //Number of random walks to simulate
#define NUMSTEP     10//Number of steps per random walk


void random_step(int *x, int *y);
int calculate_status(int i, int j, char map[NUMROWS][NUMCOLS]);
void printResults(float results[NUMROWS][NUMCOLS], float s[NUMROWS][NUMCOLS]);

int main(void) {
    // Variable declarations 
    srand(123456);// Seed for random number generation
    int n, m = 0, a, b, e = 0; 
    int i, j;
    int k = 0;
    int number = 0, steps = 0, stat;
    int move;
    int END = 0;//#attempts
    float s[NUMROWS][NUMCOLS], ss[NUMROWS][NUMCOLS], ts[1000][81]; 
    float p[NUMROWS][NUMCOLS], mean[NUMROWS][NUMCOLS], sd[NUMROWS][NUMCOLS];
    float results[NUMROWS][NUMCOLS];
    float numstep[1000][81], temp;
    // File handling
    FILE *file = fopen("island_map.txt", "r");
    if(file == NULL){
        printf("Error!");
        return 1;
    }
    char map[NUMROWS][NUMCOLS];

    // Reading the map from file
    for(i = 0;i < NUMROWS;i++){
        for(j = 0;j < 17;j++){
            char c;
            fscanf(file,"%c",&c);
            if (c !=' '){
                map[i][j/2] = c;
            }
        }
        fscanf(file, "\n");
    }
    fclose(file);

    int loop = 0;



    for (i = 0; i < NUMROWS; i++) {
        for (j = 0; j < NUMCOLS; j++) {
            printf("%c",map[i][j]);
            if(j< NUMCOLS-1){
                printf(" ");
            }
        }
        printf("\n");
    }

    for (a = 0; a < NUMROWS; a++) {
        for (b = 0; b < NUMCOLS; b++) {
            END = 0; m = 0; steps = 0;
            while (END != NUMWALKS) {
                i = a; j = b;
                if ((map[i][j] == 'W') || (map[i][j] == 'V') || (map[i][j] == 'D')) { break; }

                else {
                    for (n = 1; n <= NUMSTEP; n++) {
                        random_step(&i, &j);
                        stat = calculate_status(i, j, map);
                        if (stat == 1) { stat = 2; break; }
                        if (stat == 0) { stat = 2; m = m + 1; numstep[m][number] = n; steps = steps + n; break; }
                    }
                }
                END = END + 1;
            }
            s[a][b] = m;
            ss[a][b] = steps;
            number = number + 1;
        }
    }
// Calculating probability, mean path length, and standard deviation

    for (i = 0; i < NUMROWS; i++) {
        for (j = 0; j < NUMCOLS; j++) {
            if (ss[i][j] == 0 || s[i][j] == 0) {}
            else {
                p[i][j] = (s[i][j] / 1000) * 100;
            }
        }
    }

    for (i = 0; i < NUMROWS; i++) {
        for (j = 0; j < NUMCOLS; j++) {
            if (ss[i][j] == 0 || s[i][j] == 0) {}
            else {
                mean[i][j] = (ss[i][j] / s[i][j]);
            }
        }
    }

    number = 0;
    for (i = 0; i < NUMROWS; i++) {
        for (j = 0; j < NUMCOLS; j++) {
            if (ss[i][j] == 0 || s[i][j] == 0) {}
            else {
                temp = 0;
                for (n = 1; n <= s[i][j]; n++) {
                    ts[n][number] = numstep[n][number] - mean[i][j];
                    temp = ts[n][number] * ts[n][number] + temp;
                }
                sd[i][j] = sqrt((temp) / s[i][j]);
            }
            number = number + 1;
        }
    }
    for (i = 0; i < NUMROWS; i++) {
        for (j = 0; j < NUMCOLS; j++) {
            if(map[i][j] == 'B'){
                p[i][j] = 100;
                mean[i][j] = 0;
                sd[i][j] = 0;
            }
        }
    }
    printf("\nProbability of escape\n");
    printResults(p, s);
    printf("\nMean path length\n");
    printResults(mean, s);
    printf("\nStandard deviation of path length\n");
    printResults(sd, s);

    return 0;
}

void random_step(int *x, int *y) {
    int move;
    int i, j;
    i = *x;
    j = *y;
    move = rand() % 8;
        switch (move) {
            case 0:
                i = i - 1;
                break;				
            case 1:
                i = i - 1;
                j = j + 1;
                break;	//ne
            case 2:
                j = j + 1;

                break;				
            case 3:
                i = i + 1;
                j = j + 1;

                break;	
            case 4:
                i = i + 1;

                break;				
            case 5:
                i = i + 1;
                j = j - 1;

                break;	
            case 6:
                j = j - 1;

                break;				
            case 7:
                i = i - 1;
                j = j - 1;

                break;	
        }
    if(i == -1){
        i = 0;
    }
    if(j == -1){
        j = 0;
    }
    if(i == 9){
        i = 8;
    }
    if(j == 9){
        j = 8;
    }
    *x = i;
    *y = j;
    return;
}


int calculate_status(int i, int j, char map[NUMROWS][NUMCOLS]) {
    int e = 2;
    if (map[i][j] == 'B') {
        e = 0;
    }

    else {
        switch (map[i][j]) {
            case 'W': e = 1; break;
            case 'D': e = 1; break;
            case 'V': e = 1; break;
        }
    }
    return e;
}

void printResults(float results[NUMROWS][NUMCOLS], float s[NUMROWS][NUMCOLS]) {
    float zero = 0;
    int i, j;
    for (i = 0; i < NUMROWS; i++) {
        for (j = 0; j < NUMCOLS; j++) {
            if (s[i][j] == 0) { printf("%.2f", zero); }
            else {
                printf("%.2f", results[i][j]);
            }
            if(j < 8){
                printf(" ");
            }
            if (j == 8) { printf("\n"); }
        }
    }
    return;
}