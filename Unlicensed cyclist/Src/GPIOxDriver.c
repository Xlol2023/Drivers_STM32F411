/**
 ******************************************************************************
 * @file           : main.c
 * @author         : osmunozga
 * @brief          : configuracion basica del proyecto
 ******************************************************************************
Se establece el control lo más generico posible para usarlo en cualquier PIN seleccionado del puerto GPIO
 *
 ******************************************************************************
 */

#include "GPIOxDriver.h"

/*
 * PAra cualquier periferico se deben seguir los siguientes pasos
 * Activar la señal de reloj (RCC)
 */
void GPIO_Config (GPIO_Handler_t *pGPIOHandler){

	//Variable para hacer todo paso a paso
	uint32_t auxConfig = 0;
	uint32_t auxPosition = 0;

	//1) Activar el periferico
	//Verificamos para GPIOA
	if(pGPIOHandler ->pGPIOx == GPIOA){
		//Escribimos 1 (SET) en la posicion correspondiente al GPIOA
		RCC ->AHB1ENR |= (RCC_AHB1ENR_GPIOAEN);
	}
	//Verificamos para GPIOB
	else if (pGPIOHandler ->pGPIOx == GPIOB){
		//Escribimos 1 (SET) en la posicion correspondiente al GPIOB
		RCC ->AHB1ENR |= (RCC_AHB1ENR_GPIOBEN);
	}
	//Verificamos para GPIOC
	else if (pGPIOHandler ->pGPIOx ==GPIOC){
		//Escribimos 1 (SET) en la posición correspondiente al GPIOC
		RCC -> AHB1ENR |= (RCC_AHB1ENR_GPIOCEN);
	}
	else if (pGPIOHandler ->pGPIOx ==GPIOD){
		RCC -> AHB1ENR |= (RCC_AHB1ENR_GPIODEN);
	}
	else if (pGPIOHandler ->pGPIOx ==GPIOE){
		RCC -> AHB1ENR |= (RCC_AHB1ENR_GPIOEEN);
	}
	else if (pGPIOHandler ->pGPIOx ==GPIOH){
		RCC -> AHB1ENR |= (RCC_AHB1ENR_GPIOHEN);
	}

	//Después de activado, podemos comenzar a configurar.
	// 2)Configurando el registro GPIOx_MODER
	//Acá estamos leyendo la configuración, moviendo "PinNumber" veces hacia la izquierda ese valor (shitf left)
	//y todo eso lo cargamos en la variable auxConfig
	auxConfig = (pGPIOHandler ->GPIO_PinConfig.GPIO_PinMode << 2 * pGPIOHandler->GPIO_PinConfig.GPIO_PinNumber);

	//Antes de cargar el nuevo valor, limpiamos los bits especificos de ese registro (debemos escribir 0b00)
	//para lo cual aplicamos una máscara y una operacion bitwise AND
	pGPIOHandler ->pGPIOx->MODER &= ~(0b11 << 2 * pGPIOHandler->GPIO_PinConfig.GPIO_PinNumber);

	//Cargamos a auxConfig en el registro MODER
	pGPIOHandler ->pGPIOx->MODER |= auxConfig;

	// 3) Configurando el registro GPIOx_OTYPER
	//De nuevo, leemos y movemos el valor un numero "PinNumber" de veces
	auxConfig = (pGPIOHandler->GPIO_PinConfig.GPIO_PinOPType << pGPIOHandler->GPIO_PinConfig.GPIO_PinNumber);

	//Limpiamos antes de cargar
	pGPIOHandler ->pGPIOx->OTYPER &= ~(SET << pGPIOHandler->GPIO_PinConfig.GPIO_PinNumber);

	//Cargamos el resultado sobre el registro adecuado
	pGPIOHandler ->pGPIOx ->OTYPER |= auxConfig;

	// 4) Configurando ahora la velocidad
	auxConfig = (pGPIOHandler ->GPIO_PinConfig.GPIO_PinSpeed << 2*pGPIOHandler ->GPIO_PinConfig.GPIO_PinNumber);

	//limpiando la posicion antes de cargar la nueva configuración
	pGPIOHandler-> pGPIOx ->OSPEEDR &= ~(0b11 << 2 * pGPIOHandler->GPIO_PinConfig.GPIO_PinNumber);

	//Cargamos el resultado sobre el registro adecuado
	pGPIOHandler ->pGPIOx ->OSPEEDR |= auxConfig;

	// 5) Configurando si se desea pull Up, Pull Down o flotante
	auxConfig = (pGPIOHandler ->GPIO_PinConfig.GPIO_PinPuPdControl << 2*pGPIOHandler ->GPIO_PinConfig.GPIO_PinNumber);
	pGPIOHandler-> pGPIOx ->PUPDR &= ~(0b11 << 2 * pGPIOHandler->GPIO_PinConfig.GPIO_PinNumber);
	pGPIOHandler ->pGPIOx ->PUPDR |= auxConfig;

	//Funciones alternativas
	if(pGPIOHandler -> GPIO_PinConfig.GPIO_PinMode == GPIO_MODE_ALTFN){

		//Seleccionamos primero si se debe utilizar el registro bajo(AFRL) o el alto(AFRH)
		if(pGPIOHandler->GPIO_PinConfig.GPIO_PinNumber < 8){
			//Registro AFRL - PIN 0 al 7
			auxPosition= 4* pGPIOHandler ->GPIO_PinConfig.GPIO_PinNumber;

			//Limpiamos primero la posicion del registro que deseamos escribir
			pGPIOHandler ->pGPIOx ->AFR[0] &= ~(0b1111 << auxPosition);

			//Escribimos el valor configurado en la posicion seleccionada
			pGPIOHandler ->pGPIOx ->AFR[0] |= (pGPIOHandler ->GPIO_PinConfig.GPIO_PinAltFunMode << auxPosition);

		}
		else{
			//Estamos en el registro AFRH, que controla los pines del PIN 8 al 15
			auxPosition = 4 * (pGPIOHandler -> GPIO_PinConfig.GPIO_PinNumber -8);

			//Limpiamos primero la posicion del registro que deseamos escribir a continuacion
			pGPIOHandler ->pGPIOx->AFR[1] &= ~(0b1111 << auxPosition);

			// Y escribimos el valor configurado en la posicion seleccionada
			pGPIOHandler ->pGPIOx ->AFR[1] |= (pGPIOHandler ->GPIO_PinConfig.GPIO_PinAltFunMode << auxPosition);
		}
	}
} //Fin del GPIO Config


/*
 * Funcion utilizada para cambiar de estado el pin entregado en el Handler, asignado el valor entregado en la valiarble
 * newState
 */
void GPIO_WritePin(GPIO_Handler_t *pPinHandler, uint8_t newState){
	//Limpiamos la posicion que deseamos
	//pPinHandler ->pGPIOx ->ODR &= ~(SET << pPinHandler -> GPIO_PinConfig.GPIO_PinNumber;
	if(newState == SET){
		//Trabajamos con la parte baja del registro
		pPinHandler ->pGPIOx ->BSRR |= (SET << pPinHandler ->GPIO_PinConfig.GPIO_PinNumber);
	}
	else{
		//Trabajando con la parte alta del registro
		pPinHandler ->pGPIOx ->BSRR |= (SET << (pPinHandler->GPIO_PinConfig.GPIO_PinNumber +16));
	}
}
/*
 * Funcion para leer un pin en especifico
 */
uint32_t GPIO_ReadPin(GPIO_Handler_t *pPinHandler){
	//Creamos una variable auxiliar la cual luego retornaremos
	uint32_t pinValue = 0;

	//Cargamos el valor del registro IDR, desplazado a derecha tantas veces como la ubicacion del pin especifico
	pinValue = (pPinHandler ->pGPIOx->IDR >> pPinHandler -> GPIO_PinConfig.GPIO_PinNumber);
	//Aplicando mascara de 0b01
	pinValue &= (0b01);


	return pinValue;
}
//Funcion para cambiar el estado del PIN
uint32_t GPIO_TooglePin(GPIO_Handler_t *pPinHandler){
	//Crear una variable
	uint32_t pinValue = 0;

	//Leer el valor actual del PIN
	pinValue = pPinHandler ->pGPIOx -> ODR;

	// Desplazando el bit 1 pin veces hasta la posicion del pin seleccionado
	//luego aplicar el operador lógico XOR
	pinValue ^= (0b01 << pPinHandler -> GPIO_PinConfig.GPIO_PinNumber);

	// EScribir el nuevo valor en el registro ODR
	pPinHandler -> pGPIOx ->ODR = pinValue;

	return pinValue;

}

//fin
