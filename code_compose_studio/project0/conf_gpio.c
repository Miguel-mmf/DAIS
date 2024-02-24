/*
 * conf_gpio.c � a implementa��o da fun��o
 *
 *  Created on: 22 de fev de 2024
 *      Author: Miguel Marques
 */

#include <stdint.h>
#include <stdbool.h>
#include "inc/tm4c129encpdt.h"
#include "driverlib/sysctl.h"


void initGPIO(void)
{
    //
    // set o valor 1 para o registrador
    // SYSCTL_RCGCGPIO_R = (SYSCTL_RCGCGPIO_R|0x1000); � correto
    SYSCTL_RCGCGPIO_R = (SYSCTL_RCGCGPIO_R|(1<<12)); // tb � correto, mas � mais intuitivo
    // estou utilizando a mascara acima para evitar que as outras portas 4
    // sejam desabilitadas pelo 000 do numero em hexa;


    //
    // Configura��o do pino como sa�da
    //
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPION))
    {
    }
    
    GPIO_PORTN_DIR_R |= 0x3; // ou GPIO_PORTN_DIR |= (1<<0 | 1<<1);
    GPIO_PORTN_AFSEL_R &= ~((1<<1)|(1<<0)); // ou GPIO_PORTN_AFSEL_R &= 0x3;
    GPIO_PORTN_DEN_R |= 0x3;
}


void meuDigitalWrite(int pin, bool val)
{
    if (val == 1)
        GPIO_PORTN_DATA_R |= (1<<pin);
    else if (val == 0)
        GPIO_PORTN_DATA_R &= ~(1<<pin);
}
