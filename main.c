/* Includes ------------------------------------------------------------------*/
#include "board.h"
#include "stm32f4xx_hal_conf.h"
#include "debug_printf.h"
#include "Nokia_5110.h"
#include "string.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
ADC_HandleTypeDef AdcHandle;
ADC_ChannelConfTypeDef AdcChanConfig;
/* Private function prototypes -----------------------------------------------*/
void Delay(__IO unsigned long nCount);
void Hardware_init();
void ADC_Init();
uint16_t ADC_Get();
void Laser_Init();
uint16_t Laser_thing();

int main(void) {

	Hardware_init();	//Initalise hardware modules

	char buf[50] = "IOT Recycler\0";
	LCD_WriteLine(buf, 1);
	strcpy(buf, "Sorter\0");
	LCD_WriteLine(buf, 1);
	strcpy(buf, "Hackday.io\0");
	LCD_WriteLine(buf, 1);
	strcpy(buf, "Tim Hadwen\0");
	LCD_WriteLine(buf, 1);
	strcpy(buf, "Michael Smith\0");
	LCD_WriteLine(buf, 1);
	LCD_ProgressBar();
	/* Main processing loop */
  while (1) {
		char buffer[15];
		sprintf(buffer, "%d", Laser_thing());
		LCD_WriteLine(buffer, 0);
		Delay(0x7FFF00);
	}
}

void Hardware_init() {
	BRD_init();			//Initalise NP2 board.
	LCD_Init();
	ADC_Init();
	Laser_Init();
}

void Delay(__IO unsigned long nCount) {
  	while(nCount--) {
  	}
}

void ADC_Init(){
	GPIO_InitTypeDef GPIO_InitStructure;

	/* Enable A0 GPIO Clock */
	__BRD_A0_GPIO_CLK();

	/* Configure A0 as analog input */
	GPIO_InitStructure.Pin = BRD_A0_PIN;			//Set A0 pin
	GPIO_InitStructure.Mode = GPIO_MODE_ANALOG;		//Set to Analog input
	GPIO_InitStructure.Pull = GPIO_NOPULL ;			//No Pull up resister

	HAL_GPIO_Init(BRD_A0_GPIO_PORT, &GPIO_InitStructure);	//Initialise AO

	/* Enable ADC1 clock */
	__ADC1_CLK_ENABLE();

	/* Configure ADC1 */
	AdcHandle.Instance = ADC1;						//Use ADC1
	AdcHandle.Init.ClockPrescaler        = ADC_CLOCKPRESCALER_PCLK_DIV2;	//Set clock prescaler
	AdcHandle.Init.Resolution            = ADC_RESOLUTION12b;				//Set 12-bit data resolution
	AdcHandle.Init.ScanConvMode          = DISABLE;
	AdcHandle.Init.ContinuousConvMode    = DISABLE;
	AdcHandle.Init.DiscontinuousConvMode = DISABLE;
	AdcHandle.Init.NbrOfDiscConversion   = 0;
	AdcHandle.Init.ExternalTrigConvEdge  = ADC_EXTERNALTRIGCONVEDGE_NONE;	//No Trigger
	AdcHandle.Init.ExternalTrigConv      = ADC_EXTERNALTRIGCONV_T1_CC1;		//No Trigger
	AdcHandle.Init.DataAlign             = ADC_DATAALIGN_RIGHT;				//Right align data
	AdcHandle.Init.NbrOfConversion       = 1;
	AdcHandle.Init.DMAContinuousRequests = DISABLE;
	AdcHandle.Init.EOCSelection          = DISABLE;

	HAL_ADC_Init(&AdcHandle);		//Initialise ADC

	/* Configure ADC Channel */
	AdcChanConfig.Channel = BRD_A0_ADC_CHAN;							//Use AO pin
	AdcChanConfig.Rank         = 1;
	AdcChanConfig.SamplingTime = ADC_SAMPLETIME_3CYCLES;
	AdcChanConfig.Offset       = 0;

	HAL_ADC_ConfigChannel(&AdcHandle, &AdcChanConfig);		//Initialise ADC channel
}

void Laser_Init(){
	GPIO_InitTypeDef GPIO_InitStructure;
	__BRD_D2_GPIO_CLK();
  GPIO_InitStructure.Pin = BRD_D2_PIN;				//Pin
  GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;		//Output Mode
  GPIO_InitStructure.Pull = GPIO_NOPULL;			//Enable Pull up, down or no pull resister
  GPIO_InitStructure.Speed = GPIO_SPEED_MEDIUM;			//Pin latency
  HAL_GPIO_Init(BRD_D2_GPIO_PORT, &GPIO_InitStructure);	//Initialise Pin
}

uint16_t ADC_Get(){
	HAL_ADC_Start(&AdcHandle); // Start ADC conversion

	//Wait for ADC Conversion to complete
	while (HAL_ADC_PollForConversion(&AdcHandle, 10) != HAL_OK);
	return (uint16_t)(HAL_ADC_GetValue(&AdcHandle));
}

uint16_t Laser_thing(){
	// Turn the laser off
	HAL_GPIO_WritePin(BRD_D2_GPIO_PORT, BRD_D2_PIN, 0);
	Delay(0xFFFF);

	// Store the value
	int basis_value = ADC_Get();

	// Turn the laser on
	HAL_GPIO_WritePin(BRD_D2_GPIO_PORT, BRD_D2_PIN, 1);
	Delay(0xFFFF);

	// Store the value
	int diff = basis_value - ADC_Get();

	if(diff < 0){
		diff = -diff;
	}

	// Turn the laser off
	HAL_GPIO_WritePin(BRD_D2_GPIO_PORT, BRD_D2_PIN, 0);

	return diff;
}
