#include "stm32f10x.h"
#include "stm32f1xx_nucleo.h"

// Define pin names and ports for outputs
// Cathodes
#define K0_PIN   GPIO_Pin_13
#define K0_PORT  GPIOB
#define K1_PIN   GPIO_Pin_14
#define K1_PORT  GPIOB
#define K2_PIN   GPIO_Pin_15
#define K2_PORT  GPIOB

// Anodes
#define A0_PIN   GPIO_Pin_10
#define A0_PORT  GPIOB
#define A1_PIN   GPIO_Pin_8
#define A1_PORT  GPIOA
#define A2_PIN   GPIO_Pin_9
#define A2_PORT  GPIOA
#define A3_PIN   GPIO_Pin_7
#define A3_PORT  GPIOC
#define A4_PIN   GPIO_Pin_6
#define A4_PORT  GPIOB
#define A5_PIN   GPIO_Pin_7
#define A5_PORT  GPIOA
#define A6_PIN   GPIO_Pin_6
#define A6_PORT  GPIOA




// Function prototypes
void delay();
void configureGPIOOut(uint16_t pins, GPIO_TypeDef *port);
void writeToSingleLED(uint8_t led_num, uint8_t display_num);

			
int main(void)
{
	// Configure GPIO pins for output
	configureGPIOOut(A1_PIN | A2_PIN | A5_PIN | A6_PIN, GPIOA);
	configureGPIOOut(K0_PIN | K1_PIN | K2_PIN | A0_PIN | A4_PIN, GPIOB);
	configureGPIOOut(A3_PIN, GPIOC);

	uint8_t counter = 0;

	// Infinite loop
	for(;;) {
		writeToSingleLED(counter, 0);
		counter++;
		if (counter >= 3) { counter = 0; }
		delay();
	}
}


// Delay about one second
void delay() {
	for (long i=0; i<4000000; i++) {
		__NOP();
	}
}

// Configure the given GPIOA, B, or C pins to digital out at 2MHz push pull output
void configureGPIOOut(uint16_t pins, GPIO_TypeDef *port) {
	// Connect clock to GPIOs
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);

	// Create and init GPIOInitStruct
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Pin   = pins;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_Out_PP;

	// Use GPIO_InitStruct to init GPIO
    GPIO_Init(port, &GPIO_InitStruct);
}

// Write a given number from 0 to 9 to a given LED (from 0 to 2)
void writeToSingleLED(uint8_t led_num, uint8_t display_num) {
	// Pull cathode low for selected LED while other two are high
	// Cathodes on the CN10 connector
	switch (led_num) {
		case 0:
			// Pin K0 low, pins K1 and K2 high
			GPIO_ResetBits(K0_PORT, K0_PIN);
			GPIO_SetBits(K1_PORT, K1_PIN);
			GPIO_SetBits(K2_PORT, K2_PIN);
			break;
		case 1:
			// Pin K1 low, pins K0 and K1 high
			GPIO_SetBits(K0_PORT, K0_PIN);
			GPIO_ResetBits(K1_PORT, K1_PIN);
			GPIO_SetBits(K2_PORT, K2_PIN);
			break;
		case 2:
			// Pin K2 low, pins K0 and K1 high
			GPIO_SetBits(K0_PORT, K0_PIN);
			GPIO_SetBits(K1_PORT, K1_PIN);
			GPIO_ResetBits(K2_PORT, K2_PIN);
			break;
		default:
			// For led_num out of range, set all cathodes high (LED off)
			GPIO_SetBits(GPIOB, K0_PIN | K1_PIN | K2_PIN);
			break;
	}

	// Set anodes high for given number, a = D6, b = D7 ... g = D12
	switch (display_num) {
		case 0:
			// 1 1 1 1 1 1 0
			GPIO_SetBits(A0_PORT, A0_PIN);
			GPIO_SetBits(A1_PORT, A1_PIN);
			GPIO_SetBits(A2_PORT, A2_PIN);
			GPIO_SetBits(A3_PORT, A3_PIN);
			GPIO_SetBits(A4_PORT, A4_PIN);
			GPIO_SetBits(A5_PORT, A5_PIN);
			GPIO_ResetBits(A6_PORT, A6_PIN);
			break;
		case 1:
			// 0 1 1 0 0 0 0
			GPIO_ResetBits(A0_PORT, A0_PIN);
			GPIO_SetBits(A1_PORT, A1_PIN);
			GPIO_SetBits(A2_PORT, A2_PIN);
			GPIO_ResetBits(A3_PORT, A3_PIN);
			GPIO_ResetBits(A4_PORT, A4_PIN);
			GPIO_ResetBits(A5_PORT, A5_PIN);
			GPIO_ResetBits(A6_PORT, A6_PIN);
			break;
		case 2:
			// 1 1 0 1 1 0 1
			GPIO_SetBits(A0_PORT, A0_PIN);
			GPIO_SetBits(A1_PORT, A1_PIN);
			GPIO_ResetBits(A2_PORT, A2_PIN);
			GPIO_SetBits(A3_PORT, A3_PIN);
			GPIO_SetBits(A4_PORT, A4_PIN);
			GPIO_ResetBits(A5_PORT, A5_PIN);
			GPIO_SetBits(A6_PORT, A6_PIN);
			break;
		case 3:
			// 1 1 1 1 0 0 1
			GPIO_SetBits(A0_PORT, A0_PIN);
			GPIO_SetBits(A1_PORT, A1_PIN);
			GPIO_SetBits(A2_PORT, A2_PIN);
			GPIO_SetBits(A3_PORT, A3_PIN);
			GPIO_ResetBits(A4_PORT, A4_PIN);
			GPIO_ResetBits(A5_PORT, A5_PIN);
			GPIO_SetBits(A6_PORT, A6_PIN);
			break;
		case 4:
			// 0 1 1 0 0 1 1
			GPIO_ResetBits(A0_PORT, A0_PIN);
			GPIO_SetBits(A1_PORT, A1_PIN);
			GPIO_SetBits(A2_PORT, A2_PIN);
			GPIO_ResetBits(A3_PORT, A3_PIN);
			GPIO_ResetBits(A4_PORT, A4_PIN);
			GPIO_SetBits(A5_PORT, A5_PIN);
			GPIO_SetBits(A6_PORT, A6_PIN);
			break;
		case 5:
			// 1 0 1 1 0 1 1
			GPIO_SetBits(A0_PORT, A0_PIN);
			GPIO_ResetBits(A1_PORT, A1_PIN);
			GPIO_SetBits(A2_PORT, A2_PIN);
			GPIO_SetBits(A3_PORT, A3_PIN);
			GPIO_ResetBits(A4_PORT, A4_PIN);
			GPIO_SetBits(A5_PORT, A5_PIN);
			GPIO_SetBits(A6_PORT, A6_PIN);
			break;
		case 6:
			// 1 0 1 1 1 1 1
			GPIO_SetBits(A0_PORT, A0_PIN);
			GPIO_ResetBits(A1_PORT, A1_PIN);
			GPIO_SetBits(A2_PORT, A2_PIN);
			GPIO_SetBits(A3_PORT, A3_PIN);
			GPIO_SetBits(A4_PORT, A4_PIN);
			GPIO_SetBits(A5_PORT, A5_PIN);
			GPIO_SetBits(A6_PORT, A6_PIN);
			break;
		case 7:
			// 1 1 1 0 0 0 0
			GPIO_SetBits(A0_PORT, A0_PIN);
			GPIO_SetBits(A1_PORT, A1_PIN);
			GPIO_SetBits(A2_PORT, A2_PIN);
			GPIO_ResetBits(A3_PORT, A3_PIN);
			GPIO_ResetBits(A4_PORT, A4_PIN);
			GPIO_ResetBits(A5_PORT, A5_PIN);
			GPIO_ResetBits(A6_PORT, A6_PIN);
			break;
		case 8:
			// 1 1 1 1 1 1 1
			GPIO_SetBits(A0_PORT, A0_PIN);
			GPIO_SetBits(A1_PORT, A1_PIN);
			GPIO_SetBits(A2_PORT, A2_PIN);
			GPIO_SetBits(A3_PORT, A3_PIN);
			GPIO_SetBits(A4_PORT, A4_PIN);
			GPIO_SetBits(A5_PORT, A5_PIN);
			GPIO_SetBits(A6_PORT, A6_PIN);
			break;
		case 9:
			// 1 1 1 0 0 1 1
			GPIO_SetBits(A0_PORT, A0_PIN);
			GPIO_SetBits(A1_PORT, A1_PIN);
			GPIO_SetBits(A2_PORT, A2_PIN);
			GPIO_ResetBits(A3_PORT, A3_PIN);
			GPIO_ResetBits(A4_PORT, A4_PIN);
			GPIO_SetBits(A5_PORT, A5_PIN);
			GPIO_SetBits(A6_PORT, A6_PIN);
			break;
		default:
			// off
			GPIO_ResetBits(A0_PORT, A0_PIN);
			GPIO_ResetBits(A1_PORT, A1_PIN);
			GPIO_ResetBits(A2_PORT, A2_PIN);
			GPIO_ResetBits(A3_PORT, A3_PIN);
			GPIO_ResetBits(A4_PORT, A4_PIN);
			GPIO_ResetBits(A5_PORT, A5_PIN);
			GPIO_ResetBits(A6_PORT, A6_PIN);
			break;

	}
}

