/*
 * conf_gpio.c é a implementação da função
 *
 *  Created on: 08 de mar de 2024
 *      Author: Miguel Marques Ferreira
 */

#include <stdint.h>
#include <stdbool.h>
#include "inc/tm4c129encpdt.h"
#include "driverlib/sysctl.h"

// variavel externa e definida no arquivo principal
extern bool status_button;
extern int first_button;
extern bool second_button;


void initGPIO(void)
{

    SYSCTL_RCGCGPIO_R = (SYSCTL_RCGCGPIO_R|(1<<12)); // habilitando a porta N
    SYSCTL_RCGCGPIO_R = (SYSCTL_RCGCGPIO_R|(1<<8)); // habilitando a porta J
    // estou utilizando a mascara acima para evitar que as outras portas 4
    // sejam desabilitadas pelo 000 do numero em hexa;

    //
    // Configuração do pino como saída
    //
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPION))
    {
    }

    // Configuração da porta N
    GPIO_PORTN_DIR_R |= 0x3; // ou GPIO_PORTN_DIR |= (1<<0 | 1<<1);
    GPIO_PORTN_AFSEL_R &= ~((1<<1)|(1<<0)); // ou GPIO_PORTN_AFSEL_R &= 0x3;
    GPIO_PORTN_DEN_R |= 0x3;


    // Configuração da porta J
    GPIO_PORTJ_AHB_DIR_R &= ~((1<<1)|(1<<0)); // Estou ativando os dois botões
    /* Lógica
     * Estou deslocando dois bits 1 para a primeira e segunda posição
     * 00000011
     * Depois disso, é necessário inverter:
     * 11111100
     * Dessa forma, estarei ativando o pino 0 e 1 da porta J como entradas.
     */
    GPIO_PORTJ_AHB_AFSEL_R &= ~((1<<1)|(1<<0));
    GPIO_PORTJ_AHB_DEN_R |= 0x3;

    // Habilitando resistor de pullup nos pinos 0 e 1 da porta J
    GPIO_PORTJ_AHB_PUR_R |= 0x3; // ou ((1<<1)|(1<<0))

    //
    // Configuraçao da interrupcao para a porta J e os pinos 0 e 1
    //
    GPIO_PORTJ_AHB_IS_R &= ~(0x3); // interrupção tipo borda de descida para o pino 0 e 1;
    GPIO_PORTJ_AHB_IEV_R &= ~(0x3); // The GPIOIEV register is the interrupt event register
    GPIO_PORTJ_AHB_IM_R |= 0x3; // The GPIOIM register is the interrupt mask register.

    // habilitando dentro do controlador a porta J para interrupção
    NVIC_EN1_R |= (1<<19);
}


void intPortJ(void)
{

    if (!first_button)
    {
        //
        // Informando que um botão foi pressionado o proximo
        // botao acionado sera responsavel por desativar a bomba
        //
        status_button = true;

        if (GPIO_PORTJ_AHB_MIS_R == 0x1) // detecta PUSH1 (PJ_0)
            first_button = 0x1;
        else if (GPIO_PORTJ_AHB_MIS_R == 0x2) // detecta PUSH2 (PJ_1)
            first_button = 0x2;
        else
            first_button = 0x0;
    }
    else if (first_button)
    {
        //
        // se o botão oposto for acionado, a bomba acionada será desligada
        //
        if (GPIO_PORTJ_AHB_MIS_R != first_button)
        {
            second_button = true;
            status_button = false;
        }
    }

    //
    // limpa a interrupção para não ficar travado nela na próxima execução
    //
    GPIO_PORTJ_AHB_ICR_R |= (0x3);
}
