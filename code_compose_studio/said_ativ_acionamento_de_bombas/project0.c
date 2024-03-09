//*****************************************************************************************************
//
// main.c - Atividade Acionamento de Bombas da Disciplina de Sistema de Aquisição de Dados e Interfaces
//
//*****************************************************************************************************

#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"

#include "inc/tm4c129encpdt.h"

//
// minha biblioteca
//
#include "conf_gpio.h"


//*****************************************************************************
//
// The error routine that is called if the driver library encounters an error.
//
//*****************************************************************************
#ifdef DEBUG
void
__error__(char *pcFilename, uint32_t ui32Line)
{
}
#endif

//
// invertido, pois os botões sao invertidos em relação aos les
//
#define BOMBA1  GPIO_PIN_1 // bomba 1
#define BOMBA2  GPIO_PIN_0 // bomba 2
bool status_button = false;
bool second_button = false;
int first_button = 0x0; // var para detectar qual botão foi pressionado primeiro
// assim, o botão oposto poderá ser utilizado com a função de desligar

//*****************************************************************************
//
// Main 'C' Language entry point.  Toggle an LED using TivaWare.
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
    // Configure GPIO port
    //
    initGPIO();
    
    while(1)
    {
        //
        // se um botão for acionado, uma das bombas será acionada
        //
        if (status_button)
        {
            //
            // enquanto o botão de desligar nao for acionad, a bomba permanece ligada
            //
            if (first_button == 0x0)
            {
                //
                // quando o botão de desligar for acionado
                //
                if (second_button)
                {
                    //
                    // desligando as saídas, uma vez que só tem uma bomba ligada por vez
                    //
                    GPIO_PORTN_DATA_R = 0x0;
                    status_button = false;
                    second_button = false;
                }
                else
                    continue;
            }
            //
            // verificando qual dos botões foi acionado primeiro
            //
            else if (first_button == 0x1)
                GPIO_PORTN_DATA_R = BOMBA1;
            else if (first_button == 0x2)
                GPIO_PORTN_DATA_R = BOMBA2;
            //
            // uma vez a bomba acionada, é apenas necessário verificar se o desligamento foi acionado
            //
            first_button = 0x0;
        }
        //
        // Delay for a bit
        //
        SysCtlDelay(ui32SysClock/6);

    }
}
