/*
 * USARTxDriver.c
 *
 *  Created on: Apr 18, 2023
 *      Author: samuel
 */

#include <stm32f4xx.h>
#include <USARTxDriver.h>

uint8_t auxRxData = 0;
/**
 * Configurando el puerto Serial...
 * Recordar que siempre se debe comenzar con activar la señal de reloj
 * del periferico que se está utilizando.
 */
void USART_Config(USART_Handler_t *ptrUsartHandler){
	/* 1. Activamos la señal de reloj que viene desde el BUS al que pertenece el periferico */
	/* Lo debemos hacer para cada uno de las pisbles opciones que tengamos (USART1, USART2, USART6) */
    /* 1.1 Configuramos el USART1 */
	if(ptrUsartHandler->ptrUSARTx == USART1){
		// Escriba acá su código
		RCC->APB2ENR |= RCC_APB2ENR_USART1EN; //Habilitar el reloj para USART1
	}

    /* 1.2 Configuramos el USART2 */
    // Escriba acá su código
	else if(ptrUsartHandler->ptrUSARTx == USART2){
		RCC->APB1ENR |= RCC_APB1ENR_USART2EN; //Habilitar el reloj para el USART2
	}
    /* 1.3 Configuramos el USART6 */
    // Escriba acá su código
	else{
		RCC->APB2ENR |= RCC_APB2ENR_USART6EN; //Habilitar el reloj para el USART6
	}

	/* 2. Configuramos el tamaño del dato, la paridad y los bit de parada */
	/* En el CR1 estan parity (PCE y PS) y tamaño del dato (M) */
	/* Mientras que en CR2 estan los stopbit (STOP)*/
	/* Configuracion del Baudrate (registro BRR) */
	/* Configuramos el modo: only TX, only RX, o RXTX */
	/* Por ultimo activamos el modulo USART cuando todo esta correctamente configurado */

	// 2.1 Comienzo por limpiar los registros, para cargar la configuración desde cero
	ptrUsartHandler->ptrUSARTx->CR1 = 0;
	ptrUsartHandler->ptrUSARTx->CR2 = 0;

	// 2.2 Configuracion del Parity:
	// Verificamos si el parity esta activado o no
    // Tenga cuidado, el parity hace parte del tamaño de los datos...
	if(ptrUsartHandler->USART_Config.USART_parity != USART_PARITY_NONE){

		ptrUsartHandler->ptrUSARTx->CR1 &= ~(0b1 << 10); //Limpiar el bit del PCE
		ptrUsartHandler->ptrUSARTx->CR1 |= USART_CR1_PCE; //Se activa el control de paridad

		// Verificamos si se ha seleccionado ODD or EVEN
		if(ptrUsartHandler->USART_Config.USART_parity == USART_PARITY_EVEN){
			// Es even, entonces cargamos la configuracion adecuada
			// Escriba acá su código
			//Ecribir en el registro de USART_CR1 en el bit 9 (PS) un 0
			ptrUsartHandler->ptrUSARTx->CR1 |= (0b0 << 9);


		}else{
			// Si es "else" significa que la paridad seleccionada es ODD, y cargamos esta configuracion
			// Escriba acá su código
			//Ecribir en el registro de USART_CR1 en el bit 9 (PS) un 1
			ptrUsartHandler->ptrUSARTx->CR1 &= ~(0b1 << 9);
			ptrUsartHandler->ptrUSARTx->CR1 |= (0b1 << 9);
		}
	}else{
		// Si llegamos aca, es porque no deseamos tener el parity-check
		// Escriba acá su código
		//Escribir en el registro de USART_CR1 en el bit 10 (PCE) un 0 para desabiblitar el parity control enable
		ptrUsartHandler->ptrUSARTx->CR1 &= ~(0b1 << 10); //Se deshabilitar el PCE, este se coloca en 0
	}

	// 2.3 Configuramos el tamaño del dato
    // Escriba acá su código
	//Esta configuración se realiza en el USART_CR1 en el bit 12 (corresponde al bit M) 0: 8 bits de dato | 1: 9 bits de dato
	if(ptrUsartHandler->USART_Config.USART_datasize == USART_DATASIZE_8BIT){
		ptrUsartHandler->ptrUSARTx->CR1 &= ~(0b1 << 12); //Se coloca en 0 el bit M, para tener un tamaño de 8 bits
	}else{
		ptrUsartHandler->ptrUSARTx->CR1 &= ~(0b1 << 12); //Limpiar el bit M del USART_CR1
		ptrUsartHandler->ptrUSARTx->CR1 |= (0b1 << 12); //Se copia un 1 para tener un tamaño de 9 bits
	}

	// 2.4 Configuramos los stop bits (SFR USART_CR2)
	switch(ptrUsartHandler->USART_Config.USART_stopbits){
	case USART_STOPBIT_1: {
		// Debemos cargar el valor 0b00 en los dos bits de STOP
		// Escriba acá su código
		ptrUsartHandler->ptrUSARTx->CR2 &= ~(0b11 << 12); //Limpiar el registro de STOP
		break;
	}
	case USART_STOPBIT_0_5: {
		// Debemos cargar el valor 0b01 en los dos bits de STOP
		// Escriba acá su código
		ptrUsartHandler->ptrUSARTx->CR2 &= ~(0b11 << 12); //Limpiar el registro de STOP
		ptrUsartHandler->ptrUSARTx->CR2 |= (0b01 << 12); //Escribo en el registro STOP 0b01 para configurarlo como 0.5 Stop bit
		break;
	}
	case USART_STOPBIT_2: {
		// Debemos cargar el valor 0b10 en los dos bits de STOP
		// Escriba acá su código
		ptrUsartHandler->ptrUSARTx->CR2 &= ~(0b11 << 12); //Limpiar el registro de STOP
		ptrUsartHandler->ptrUSARTx->CR2 |= (0b10 << 12); //Escribo en el registro STOP 0b10 para configurarlo como 2 Stop bits
		break;
	}
	case USART_STOPBIT_1_5: {
		// Debemoscargar el valor 0b11 en los dos bits de STOP
		// Escriba acá su código
		ptrUsartHandler->ptrUSARTx->CR2 &= ~(0b11 << 12); //Limpiar el registro de STOP
		ptrUsartHandler->ptrUSARTx->CR2 |= (0b11 << 12); //Escribo en el registro STOP 0b11 para configurarlo como 1.5 Stop bits
		break;
	}
	default: {
		// En el casopor defecto seleccionamos 1 bit de parada
		// Escriba acá su código
		ptrUsartHandler->ptrUSARTx->CR2 &= ~(0b11 << 12); //Se escribe para que sea 1 Stop bit
		break;
	}
	}

	// 2.5 Configuracion del Baudrate (SFR USART_BRR)
	// Ver tabla de valores (Tabla 73), Frec = 16MHz, overr = 0;
	if(ptrUsartHandler->USART_Config.USART_baudrate == USART_BAUDRATE_9600){
		// El valor a cargar es 104.1875 -> Mantiza = 104,fraction = 0.1875
		// Mantiza = 104 = 0x68, fraction = 16 * 0.1875 = 3
		// Valor a cargar 0x0683
		// Configurando el Baudrate generator para una velocidad de 9600bps
		ptrUsartHandler->ptrUSARTx->BRR = 0x0683;
	}

	else if (ptrUsartHandler->USART_Config.USART_baudrate == USART_BAUDRATE_19200) {
		// El valor a cargar es 52.0625 -> Mantiza = 52,fraction = 0.0625
		// Mantiza = 52 = 0x34, fraction = 16 * 0.1875 = 1
		// BRR = 0x0341
		// Escriba acá su código y los comentarios que faltan
		ptrUsartHandler->ptrUSARTx->BRR = 0x0341;
	}

	else if(ptrUsartHandler->USART_Config.USART_baudrate == USART_BAUDRATE_115200){
		// El valor a cargar es 8.625 -> Mantiza = 8, fraction = 0.6805
		// Mantiza = 8 = 0x8, fraction = 16*0.6805 = 11 = 0xB
		// BRR = 0x008B
		// Escriba acá su código y los comentarios que faltan
		ptrUsartHandler->ptrUSARTx->BRR = 0x008B;
	}

	// 2.6 Configuramos el modo: TX only, RX only, RXTX, disable
	switch(ptrUsartHandler->USART_Config.USART_mode){
	case USART_MODE_TX:
	{
		// Activamos la parte del sistema encargada de enviar
		// Escriba acá su código
		ptrUsartHandler->ptrUSARTx->CR1 |= USART_CR1_TE; //Se escribe un 1 en el TE del CR1
		break;
	}
	case USART_MODE_RX:
	{
		// Activamos la parte del sistema encargada de recibir
		// Escriba acá su código
		ptrUsartHandler->ptrUSARTx->CR1 |= USART_CR1_RE; //Se escribe un 1 en el RE del CR1
		break;
	}
	case USART_MODE_RXTX:
	{
		// Activamos ambas partes, tanto transmision como recepcion
		// Escriba acá su código
		ptrUsartHandler->ptrUSARTx->CR1 |= (0b11 << 2); //Se escribe un 1 en RE del CR1 y un 1 en el TE del CR1
		break;
	}
	case USART_MODE_DISABLE:
	{
		// Desactivamos ambos canales
		// Escriba acá su código
		ptrUsartHandler->ptrUSARTx->CR1 &= ~USART_CR1_UE;
		ptrUsartHandler->ptrUSARTx->CR1 &= ~USART_CR1_RE;
		ptrUsartHandler->ptrUSARTx->CR1 &= ~USART_CR1_TE;
		break;
	}

	default:
	{
		// Actuando por defecto, desactivamos ambos canales
		ptrUsartHandler->ptrUSARTx->CR1 &= ~USART_CR1_RE;
		ptrUsartHandler->ptrUSARTx->CR1 &= ~USART_CR1_TE;
		// Escriba acá su código
		break;
	}
	}

	// 2.7 Activamos el modulo serial.
	if(ptrUsartHandler->USART_Config.USART_mode != USART_MODE_DISABLE){
		// Escriba acá su código
		ptrUsartHandler->ptrUSARTx->CR1 |= USART_CR1_UE;
	}

	/*Mirar si se activan las interrupciones*/
	if(ptrUsartHandler->USART_Config.USART_enableIntRX == USART_RX_INTERRUP_ENABLE){
		/*Desactivamos las configuraciones globales*/
		__disable_irq();

		/*Limpiamos la posicion de la interrupcion*/
		ptrUsartHandler->ptrUSARTx->CR1 &= ~USART_CR1_RXNEIE;
		/*Activamos la interrupcion del USART que se esta utilizando*/
		ptrUsartHandler->ptrUSARTx->CR1 |= USART_CR1_RXNEIE;

		/*Matriculamos la interrupcion en el NVIC*/
		if(ptrUsartHandler->ptrUSARTx == USART1){
			__NVIC_EnableIRQ(USART1_IRQn);
		}else if(ptrUsartHandler->ptrUSARTx == USART2){
			__NVIC_EnableIRQ(USART2_IRQn);
		}else if(ptrUsartHandler->ptrUSARTx == USART6){
			__NVIC_EnableIRQ(USART6_IRQn);
		}
		/*Volvemos a activar las interrupciones globales*/
		__enable_irq();
	}

}
/*funcion para escribir un string*/
void writeMsg(USART_Handler_t *ptrUsartHandler, char *msgToSend){
	while(*msgToSend != '\0'){
		writeChar(&*ptrUsartHandler, *msgToSend);
		msgToSend++;
	}
}

/* funcion para escribir un solo char */
int writeChar(USART_Handler_t *ptrUsartHandler, char dataToSend ){
	while( !(ptrUsartHandler->ptrUSARTx->SR & USART_SR_TXE)){
		__NOP();
	}
	// Escriba acá su código
	ptrUsartHandler->ptrUSARTx->DR = dataToSend;

	return dataToSend;
}

/*Lectura del caracter que llega por la interface serial*/
uint8_t getRxData(void){
	return auxRxData;
}

/*Handler de la interrupcion del USART
 * Aca deben de estar todas las interrupciones asociadas a: TX, RX, PE.... */
void USART1_IRQHandler(void){
	//Evaluamos si la interrupcion que se dio es por RX
	if(USART1->SR & USART_SR_RXNE){
		auxRxData = (uint8_t) USART1->DR;
		usart1Rx_Callback();
	}
}
void USART2_IRQHandler(void){
	//Evaluamos si la interrupcion que se dio es por RX
	if(USART2->SR & USART_SR_RXNE){
		auxRxData = (uint8_t) USART2->DR;
		usart2Rx_Callback();
	}
}
void USART6_IRQHandler(void){
	//Evaluamos si la interrupcion que se dio es por RX
	if(USART6->SR & USART_SR_RXNE){
		auxRxData = (uint8_t) USART6->DR;
		usart6Rx_Callback();
	}
	//
}


__attribute__((weak)) void usart1Rx_Callback(void){
	/*NOTE: esta funcion should not be modified, when the callback is needed,
	 * 		the BasicTimer_Callback could be implemented in the main file
	 */
	__NOP();
}
__attribute__((weak)) void usart2Rx_Callback(void){
	/*NOTE: esta funcion should not be modified, when the callback is needed,
	 * 		the BasicTimer_Callback could be implemented in the main file
	 */
	__NOP();
}
__attribute__((weak)) void usart6Rx_Callback(void){
	/*NOTE: esta funcion should not be modified, when the callback is needed,
	 * 		the BasicTimer_Callback could be implemented in the main file
	 */
	__NOP();
}
