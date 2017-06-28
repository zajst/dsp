#include "stdio.h"
#include "usbstk5515.h"
#include "aic3204.h"
#include "PLL.h"
#include "PLL.h"
#include "dsplib.h"

Int16 left_input;
Int16 right_input;
Int16 left_output;
Int16 right_output;

#define SAMPLES_PER_SECOND 48000
#define GAIN_IN_dB         0


int deltawy[51];

unsigned long int i = 0;
short int tab[51]= {0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

// nasz filtr do naszej funkcji
int Dorociak_Low_Pass_Filter_48kHz[rozmiar] = {
            13,        -11,    -90,    -57,    169,    212,    -186,    -416,
            81,     525,    73,        -412,     -46,    133,    -415,    -1,
            1338,    438,    -2369,    -1655,     2894,    3387,     -2418,    -4936,
            945,     5558,     945,    -4936,    -2418,    3387,     2894,    -1655,
            -2369,    438,    1338,    -1,     -415,    133,    -46,     -412,
            73,     525,      81,     -416,     -186,     212,    169,    -57,
            -90,    -11,    13
    };
// filtr niezbędny do odpalenia z biblioteki
DATA Dorociak_Low_Pass_Filter_48kHz1[rozmiar] = {
            13,        -11,    -90,    -57,    169,    212,    -186,    -416,
            81,     525,    73,        -412,     -46,    133,    -415,    -1,
            1338,    438,    -2369,    -1655,     2894,    3387,     -2418,    -4936,
            945,     5558,     945,    -4936,    -2418,    3387,     2894,    -1655,
            -2369,    438,    1338,    -1,     -415,    133,    -46,     -412,
            73,     525,      81,     -416,     -186,     212,    169,    -57,
            -90,    -11,    13
    };

short int filtr_c(short int sample, int N)
{
    long int result=0;
    i=i%N;
    tab[i]=sample;
    int iter;
    long int wynik=0;

        for(iter=0; iter<N; iter++)
        {
            wynik = (long) tab[((N+i)-iter)%N]*Dorociak_Low_Pass_Filter_48kHz[iter];
            result += wynik;
        }

    i++;
    return (result >> 15);
}

void main( void ) 
{
    short int x[51]={32767, 0, 0, 0, 0, 0, 0, 0, 0, 0,
         0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
         0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
         0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

    int q=0;
    for(q=0; q<51; q++)
    {
        short int result1=0;
        result1 = filtr_c(x[q], 51);
        deltawy[q]=result1;
    }
}