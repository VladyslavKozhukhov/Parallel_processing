# include <stdlib.h>
# include <stdio.h>
# include "math.h"
# include "omp.h"
# include <time.h>

int main ( int argc, char **argv );
int i4_ceiling ( double x );
double potential ( double a, double b, double x, double y );
double r8_uniform_01 ( int *seed );
void timestamp ( );

int main ( int arc, char **argv )
{
    double a = 2.0;
    double b = 1.0;
    double chk;
    int dim = 2;
    double dx;
    double dy;
    double err;
    double h = 0.0001;
    int i;
    int j;
    int k;
    int N =10000;
    int n_inside;
    int ni;
    int nj;
    double rth;
    int seed = 123456789;
    int steps;
    int steps_ave;
    double us;
    double ut;
    double vh;
    double vs;
    double x;
    double x1;
    double x2;
    double y;
    double w;
    double w_exact;
    double we;
    double wt;


   // timestamp ( );
double start_time = omp_get_wtime();

    rth = sqrt ( ( double ) dim * h );
    if ( a < b )
    {
        ni = 11;
        nj = 1 + i4_ceiling ( b / a ) * ( ni - 1 );
    }
    else
    {
        nj = 11;
        ni = 1 + i4_ceiling ( a / b ) * ( nj - 1 );
    }

    err = 0.0;
    n_inside = 0;


    for ( j = 1; j <= nj; j++ )
    {
        x = ( ( double )(nj - j) * ( - a ) + (double) (j - 1 ) * a ) / ( double ) ( nj - 1 );

        for ( i = 1; i <= ni; i++ )
        {
            y = (( double)( ni - i) * (-b) + (double)(i - 1 ) * b ) / ( double ) ( ni - 1 );
            chk = pow ( x / a, 2 ) + pow ( y / b, 2 );
            if ( 1.0 < chk )
            {
                w_exact = 1.0;
                wt = 1.0;
                steps_ave = 0;
                //printf ( "  %7.4f  %7.4f  %10.4e  %10.4e  %10.4e  %8d\n", x, y, wt, w_exact, fabs ( w_exact - wt ), steps_ave );
                continue;
            }
            n_inside = n_inside + 1;
/*
  Compute the exact solution at this point (x,y).
*/
            w_exact = exp ( pow ( x / a, 2 ) + pow ( y / b, 2 ) - 1.0 );
/*
  Now try to estimate the solution at this point.
*/
            wt = 0.0;
            steps = 0;
/*
  Do N paths
*/

            //  printf("WT:%lf steps:%d x:%lf y:%lf \n", wt,steps,x,y);
            //printf("%lf %lf %lf %lf %lf %lf %lf %lf %lf\n ",ut,us,dx,dy,x1,vs,x2,w,chk);
	#pragma omp parallel for reduction(+:wt,steps) private(ut,us,dx,dy,x1,vs,x2,w,chk)
            for ( k = 0; k < N; k++ )
            {
                x1  = x;
                x2  = y;
                w = 1.0;
                chk = 0.0;

                //printf("Hello world from omp  %d\n", tid)
               // printf("k:%d thread: %d \n", k,omp_get_thread_num());
                while ( chk < 1.0 )
                {

                    ut = r8_uniform_01 ( & seed);
                //printf("seed: %d k:%d thread: %d \n", seed,k,omp_get_thread_num());
                    if ( ut < 1.0 / 2.0 )
                    {
                        us = r8_uniform_01 ( &seed ) - 0.5;
                        if ( us < 0.0)
                        {
                            dx = - rth;
                        }
                        else
                        {
                            dx = rth;
                        }
                    }
                    else
                    {
                        dx = 0.0;
                    }
                    ut = r8_uniform_01 ( &seed );
                    if ( ut < 1.0 / 2.0 )
                    {
                        us = r8_uniform_01 ( &seed ) - 0.5;
                        if ( us < 0.0 )
                        {
                            dy = - rth;
                        }
                        else
                        {
                            dy = rth;
                        }
                    }
                    else
                    {
                        dy = 0.0;
                    }
                    vs = potential ( a, b, x1, x2 );
                    x1 = x1 + dx;
                    x2 = x2 + dy;
                    steps = steps + 1;
                    vh = potential ( a, b, x1, x2 );
                    we = ( 1.0 - h * vs ) * w;
                    w = w - 0.5 * h * ( vh * we + vs * w );
                    chk = pow ( x1 / a, 2 )+ pow ( x2 / b, 2 );
                }

                wt = wt + w;
              //  printf("k:%d thread: %d WT:%lf steps:%d x:%lf y:%lf \n",k,omp_get_thread_num(), wt,steps,x,y);

            }


//            printf("WT:%lf steps:%d x:%lf y:%lf \n", wt,steps,x,y);

/*
  WT is the average of the sum of the different trials.
*/
            wt = wt / ( double ) ( N );
            steps_ave = steps / ( double ) ( N );
/*
  Add error in WT to the running L2 error in the solution.
*/
            err = err + pow ( w_exact - wt, 2 );

              //printf ( "  %7.4f  %7.4f  %10.4e  %10.4e  %10.4e  %8d\n", x, y, wt, w_exact, fabs ( w_exact - wt ), steps_ave );
        }
    }






/*
  Compute the RMS error for all the points.
*/
    err = sqrt ( err / ( double ) ( n_inside ) );

    printf ( "\n" );
    printf ( "  RMS absolute error in solution = %e\n", err );
/*
  Terminate.
*/
    printf ( "\n" );
    printf ( "FEYNMAN_KAC_2D:\n" );
    printf ( "  Normal end of execution.\n" );
    printf ( "\n" );
 //   timestamp ( );
   printf("time : %lf \n",omp_get_wtime()-start_time);

    return 0;
}











/******************************************************************************/

int i4_ceiling ( double x )
{
    int value;

    value = ( int ) x;

    if ( value < x )
    {
        value = value + 1;
    }

    return value;
}
/******************************************************************************/

double potential ( double a, double b, double x, double y )
{
    double value;

    value = 2.0 * ( pow ( x / a / a, 2 )
                    + pow ( y / b / b, 2 ) )
            + 1.0 / a / a
            + 1.0 / b / b;

    return value;
}
/******************************************************************************/

double r8_uniform_01 ( int *seed )
{
    int k;
    double r;

    k = *seed / 127773;

    *seed = 16807 * ( *seed - k * 127773 ) - k * 2836;

    if ( *seed < 0 )
    {
        *seed = *seed + 2147483647;
    }
/*
  Although SEED can be represented exactly as a 32 bit integer,
  it generally cannot be represented exactly as a 32 bit real number!
*/
    r = ( double ) ( *seed ) * 4.656612875E-10;

    return  r;//4.656612875E-10;
}
/******************************************************************************/

void timestamp ( void )

{
# define TIME_SIZE 40

    static char time_buffer[TIME_SIZE];
    const struct tm *tm;
    time_t now;

    now = time ( NULL );
    tm = localtime ( &now );

    strftime ( time_buffer, TIME_SIZE, "%d %B %Y %I:%M:%S %p", tm );

    printf ( "%s\n", time_buffer );

    return;
# undef TIME_SIZE
}
