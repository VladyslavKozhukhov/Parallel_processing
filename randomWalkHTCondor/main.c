
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

typedef struct {
    double x;
    double y;
} Point;
#define N 1.0
#define TOTAL 10000
#define step 0.1
/*
 * x: 0.0->1.00   y: -0.5->0.5
 */


int rndVal(int arr[], int r, int l, int h)
{
    int mid;
    while (l < h)
    {
        mid = l + ((h - l) >> 1);
        (r > arr[mid]) ? (l = mid + 1) : (h = mid);
    }
    return (arr[l] >= r) ? l : -1;
}

int myRand(int arr[], int freq[], int n)
{
    int prefix[n], i;
    prefix[0] = freq[0];
    for (i = 1; i < n; ++i)
        prefix[i] = prefix[i - 1] + freq[i];
    int r = (rand() % prefix[n - 1]) + 1;
    int indexc = rndVal(prefix, r, 0, n - 1);
    return arr[indexc];
}
int checkLimit(double x, double y){
    if(x<0 || y>=0.5 || y<=-0.5)
        return 0;
    return 1;
}
int main()
{
    double  passY [1];
    int directions[]  = {1, 2, 3, 4};//UP=1 DOWN =2 LEFT=3 RIGHT=4
    int freq[] = {15, 15, 5, 65};
    double limit = 1.0;
    int  n = sizeof(directions) / sizeof(directions[0]);
    int counter = TOTAL;
    srand(time(NULL));
    Point* listParticle = (Point* ) malloc(sizeof(Point) * TOTAL);
    for(int j = 0; j < TOTAL; j++){
        listParticle[j].x = 0.0;
        listParticle[j].y = 0.0;
    }

    while(counter > 0){
        for(int k = 0; k < TOTAL; k++){
            if(listParticle[k].x != 1.0) {
                int nextStep = myRand(directions, freq, n);
                switch (nextStep) {
                    case 1://UP
                        if (checkLimit(listParticle[k].x, listParticle[k].y + 0.1) != 1) {
                            listParticle[k].x = 0;
                            listParticle[k].y = 0;
                        } else {
                            listParticle[k].y += 0.1;
//                            printf("x = %lf y = %lf  index = %d \n", listParticle[k].x, listParticle[k].y, k);

                        }
                        break;
                    case 2://DOWN
                        if (checkLimit(listParticle[k].x, listParticle[k].y - 0.1) != 1) {
                            listParticle[k].x = 0;
                            listParticle[k].y = 0;
                        } else {
                            listParticle[k].y -= 0.1;
//                            printf("x = %lf y = %lf  index = %d \n", listParticle[k].x, listParticle[k].y, k);

                        }
                        break;
                    case 3://LEFT
                        if (checkLimit(listParticle[k].x - 0.1, listParticle[k].y) != 1) {
                            listParticle[k].x = 0;
                            listParticle[k].y = 0;
                        } else {
                            listParticle[k].x -= 0.1;
//                            printf("x = %lf y = %lf  index = %d \n", listParticle[k].x, listParticle[k].y, k);

                        }
                        break;
                    case 4://RIGHT
                        if (checkLimit(listParticle[k].x + 0.1, listParticle[k].y) != 1) {
                            listParticle[k].x = 0;
                            listParticle[k].y = 0;
                        } else {
                            listParticle[k].x += 0.1;


                            if (fabs(listParticle[k].x-limit) < 0.00001 ) {

                                printf("x = %lf y = %lf  index = %d \n", listParticle[k].x, listParticle[k].y, k);
                                counter--;
                            }

                        }
                        break;
                    default:
                        exit(-1);//error
                }
            }

        }


    }

    return 0;
}


