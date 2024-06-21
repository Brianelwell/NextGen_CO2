


#define BUFFER_SIZE	128

//  Exported Variables
typedef struct{
	uint16_t Buff[BUFFER_SIZE];
	uint16_t end;
	uint16_t ptr;
} Buffer;

//  Exported functions prototypes
void USART1_Init(void);
void USART1_CharReception_Callback(void);
void USART1_SendMessage(void);
void USART1_Error_Callback(void);
//uint16_t Calc_crc(Buffer, uint16_t);

extern Buffer RxBuff;
extern Buffer TxBuff;
extern uint8_t ucCommTimeout;
extern uint16_t uiCommit_Timer;

