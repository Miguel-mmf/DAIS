//******************************************************************************************
//
// main.c - Atividade Gira Motor da Disciplina de Sistema de Aquisição de Dados e Interfaces
//
//******************************************************************************************

#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"

#include "inc/tm4c129encpdt.h"

//*****************************************************************************
//
// minhas bibliotecas
#include "conf_gpio.h"
// #include "giraMotor.h" // não vou usar mais. decidi deixar no main.c e
// acessar a var next_step diretamente
//
//*****************************************************************************

void giraMotor(bool);

// variaveis auxiliares para o controle de movimento
int aux_count = 0;
bool aux_dir = true;

int curr_step = 1; // utilizei para a comparação entre atual e próximo passo
int next_step = 1; // utilizo essa var para incrementar ou decrementar respeitando
// os limites de 1 e 4 para o envio dos passos
int possible_steps[] = {0x8, 0x4, 0x2, 0x1};

//*****************************************************************************
//
// The error routine that is called if the driver library encounters an error.
//
//*****************************************************************************
#ifdef DEBUG
void __error__(char *pcFilename, uint32_t ui32Line)
{
}
#endif

//*****************************************************************************
//
// Main 'C' Language entry point.
//
//*****************************************************************************
int main(void)
{
    //
    // Run from the PLL at 120 MHz.
    //
    uint32_t ui32SysClock;
    ui32SysClock = SysCtlClockFreqSet((SYSCTL_XTAL_25MHZ |
                                       SYSCTL_OSC_MAIN |
                                       SYSCTL_USE_PLL |
                                       SYSCTL_CFG_VCO_480), 120000000);

    //
    // Configure the GPIO port
    //
    initGPIO(); // SysCtlPeripheralEnable(SYSCTL_PERIPH_GPION);

    while(1)
    {
        //
        // Turn on the motor
        //
        giraMotor(aux_dir);
        //
        // Delay for a bit
        //
        SysCtlDelay(ui32SysClock/6);
        //
        // Controle do sentido de giro com 5 passos no sentido horário
        // e 5 passos no sentido anti-horário
        if (aux_count < 8) aux_dir = true;
        else if (aux_count >= 8 & aux_count < 16) aux_dir = false;
        else if (aux_count == 16)
        {
            aux_count = 0;
            aux_dir = true;
        }
        aux_count++;

    }
}


void giraMotor(bool sentido)
{
    //
    // sentido (True): horário
    // sentido (False): anti-horário
    //
    curr_step = next_step;

    switch (next_step)
    {
        case 4:
            GPIO_PORTK_DATA_R |= (1<<0);
            GPIO_PORTK_DATA_R &= ~(1<<1 | 1<<2 | 1<<3);
            break;
        case 3:
            GPIO_PORTK_DATA_R |= (1<<1);
            GPIO_PORTK_DATA_R &= ~(1<<0 | 1<<2 | 1<<3);
            break;
        case 2:
            GPIO_PORTK_DATA_R |= (1<<2);
            GPIO_PORTK_DATA_R &= ~(1<<0 | 1<<1 | 1<<3);
            break;
        case 1:
            GPIO_PORTK_DATA_R |= (1<<3);
            GPIO_PORTK_DATA_R &= ~(1<<0 | 1<<1 | 1<<2);
            break;
    }

    // next step update
    if (sentido)
        next_step++;
    else
        next_step--;

    // bounds
    if (next_step > 4) // horário
        next_step = 1;
    else if (next_step < 1) // anti-horário
        next_step = 4;
}
