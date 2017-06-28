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


int tablicawe[10000];
int tablicawy[10000];

unsigned long int iterator = 0;

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

short int filtr_c(short int sample, int N)
{
    long int result=0;
    iterator=iterator%N;
    tab[i]=sample;
    int iter;
    long int wynik=0;

        for(iter=0; iter<N; iter++)
        {
            wynik = (long) tab[((N+iiterator)-iter)%N]*Dorociak_Low_Pass_Filter_48kHz[iter];
            result += wynik;
        }

    iterator++;
    return (result >> 15);
}

void main( void ) 
{

/* bufor niezbędny do uruchomienia filtru z bibliotek */
    DATA bufor[51]; // wewnętrzny bufor kołowy

        int i;

        for (i = 0; i < 51; i++) {
            bufor[i] = 0;
        };


        DATA y;

    /* Initialize BSL */
    USBSTK5515_I2C_init( );

    /* Initialize the Phase Locked Loop in EEPROM */
    pll_frequency_setup(100);

    /* Initialise hardware interface and I2C for code */
    aic3204_hardware_init();

    /* Initialise the AIC3204 codec */
    aic3204_init();


    asm(" bclr XF");

     for ( i = 0  ; i < SAMPLES_PER_SECOND * 600L  ;i++  )
     {

         aic3204_codec_read(&left_input, &right_input); // Configured for one interrupt per two channels.

         if (i < 10000)
             tablicawe[i]=right_input;

        left_output = left_input;   
        right_output = filtr_c(right_input, 51);
        aic3204_codec_write(left_output, right_output);

         if (i < 10000)
             tablicawy[i]=right_output;

     }

   /* Disable I2S and put codec into reset */
    aic3204_disable();

    printf( "\n***Program has Terminated***\n" );
    SW_BREAKPOINT;
}