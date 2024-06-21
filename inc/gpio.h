

/* Exported functions prototypes ---------------------------------------------*/
void GPIO_Init(void);
void ClearGPIO_Struct(LL_GPIO_InitTypeDef*);
void LED_On(void);
void LED_Off(void);

/* Private defines -----------------------------------------------------------*/
#define USER_BUTTON_Pin LL_GPIO_PIN_13
#define USER_BUTTON_GPIO_Port GPIOC
#define USER_BUTTON_EXTI_IRQn EXTI4_15_IRQn
#define LED4_Pin LL_GPIO_PIN_5
#define LED4_GPIO_Port GPIOA

/**
  * @brief Toggle periods for various blinking modes
  */

#define LED_BLINK_FAST  200
#define LED_BLINK_SLOW  500
#define LED_BLINK_ERROR 1000
