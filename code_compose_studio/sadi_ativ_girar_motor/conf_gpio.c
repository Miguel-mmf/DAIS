/*
 * conf_gpio.c é a implementação da função
 *
 *  Created on: 07 de mar de 2024
 *      Author: Miguel Marques Ferreira
 */

#include <stdint.h>
#include <stdbool.h>
#include "inc/tm4c129encpdt.h"
#include "driverlib/sysctl.h"


void initGPIO(void)
{
    //
    // set o valor 1 para o registrador
    // SYSCTL_RCGCGPIO_R = (SYSCTL_RCGCGPIO_R|0x1000); é correto
    SYSCTL_RCGCGPIO_R = (SYSCTL_RCGCGPIO_R|(1<<9)); // habilitando a porta K, para usar os pinos PK_0, PK_1, PK_2, PK_3
    // estou utilizando a mascara acima para evitar que as outras portas 4
    // sejam desabilitadas pelo 000 do numero em hexa;
    //

    //
    // Configuração do pino como saída
    //
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOK))
    {
    }

    // Configuração da porta K
    GPIO_PORTK_DIR_R |= 0xF; // 1111 pois irei utilizar 4 pinos da porta K
    GPIO_PORTK_AFSEL_R &= ~((1<<3)|(1<<2)|(1<<1)|(1<<0)); // ou GPIO_PORTN_AFSEL_R &= 0xF;
    // 1 1 1 1 1 1 1 1
    // and
    // 0 0 0 0 1 1 1 1 -> not -> 1 1 1 1 0 0 0 0
    // 1 1 1 1 0 0 0 0 -> configuração dos pinos como básicos

    GPIO_PORTK_DEN_R |= 0xF; // ativando os pinos 0, 1, 2 e 3 da porta K no modo digital
}


void meuDigitalWrite(int pin, bool val)
{
    if (val == 1)
        GPIO_PORTK_DATA_R |= (1<<pin);
    else if (val == 0)
        GPIO_PORTK_DATA_R &= ~(1<<pin);
}
