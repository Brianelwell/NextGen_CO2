

enum ADC_CHANNEL
{
  ADC_CHANNEL_IO_CONFIG     = 0,
  ADC_CHANNEL_VIN_ADC       = 1,
  ADC_CHANNEL_V_LAMP_MINUS  = 5,
  ADC_CHANNEL_V_LAMP_PLUS   = 7,
  ADC_CHANNEL_GAS_SIGNAL    = 8,
  ADC_CHANNEL_TEMP_SIGNAL   = 9
};

/* Exported functions prototypes ---------------------------------------------*/
void ADC1_Init(void);
void ADC1_Activate(void);
void AdcGrpRegularUnitaryConvComplete_Callback(void);
void AdcGrpRegularOverrunError_Callback(void);
uint16_t ADC_Measure(uint8_t);


extern uint32_t ulADC_Data;
extern uint16_t uiCO2_Measure_Tmr;

//extern uint16_t uiADC_CO2_Reading;
//extern uint16_t uiADC_Temp_Reading;
extern uint16_t uiADC_VIN_Reading;
extern uint16_t uiADC_VLamp_Plus_Reading;
extern uint16_t uiADC_VLamp_Minus_Reading;
extern uint16_t uiADC_Config_Reading;



