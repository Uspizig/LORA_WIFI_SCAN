


// Pinning

#ifdef geraet1
//Check here your Version and Pinnings
  #define TTGOT5 1 //neded to indicate the E-INK Library a different pinning
  #define E_INK_PIN_SPI_BUSY 4//19
  #define E_INK_PIN_SPI_RST  16//21
  #define E_INK_PIN_SPI_DC   17//22
  #define E_INK_PIN_SPI_CS   5
  #define E_INK_PIN_SPI_DIN  23
  #define E_INK_PIN_SPI_SCK  18 
  #define E_INK_PIN_SPI_MISO 24//n/A

//Lora Pinning for TTGO T5 Addon Board
  #define LORA_PIN_SPI_MOSI 15//19
  #define LORA_PIN_SPI_MISO 2//n/A
  #define LORA_PIN_SPI_SCK  14
  #define LORA_PIN_SPI_NSS  26  
  #define LORA_PIN_SPI_RST  33  
  #define LORA_PIN_SPI_DIO1 25
  #define LORA_PIN_SPI_DIO0 34

  
  #define APP_EUI_LORA 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
  #define DEV_EUI_LORA 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
  #define APP_KEY_LORA 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
#endif
#ifdef geraet2
  #define TTGOT5 1 //neded to indicate the E-INK Library a different pinning
  #define E_INK_PIN_SPI_BUSY 4//19
  #define E_INK_PIN_SPI_RST  16//21
  #define E_INK_PIN_SPI_DC   17//22
  #define E_INK_PIN_SPI_CS   5
  #define E_INK_PIN_SPI_DIN  23
  #define E_INK_PIN_SPI_SCK  18 
  #define E_INK_PIN_SPI_MISO 24//n/A

//Lora Pinning for TTGO T5 Addon Board
  #define LORA_PIN_SPI_MOSI 15//19
  #define LORA_PIN_SPI_MISO 2//n/A
  #define LORA_PIN_SPI_SCK  14
  #define LORA_PIN_SPI_NSS  26  
  #define LORA_PIN_SPI_RST  33  
  #define LORA_PIN_SPI_DIO1 25
  #define LORA_PIN_SPI_DIO0 34

  #define APP_EUI_LORA 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
  #define DEV_EUI_LORA 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
  #define APP_KEY_LORA 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 
#endif
#ifdef geraet3 //ESP32 TTGO MINI V2
//Check here your Version and Pinnings
  #define TTGOT5 1 //neded to indicate the E-INK Library a different pinning
  #define E_INK_PIN_SPI_BUSY 4
  #define E_INK_PIN_SPI_RST  16
  #define E_INK_PIN_SPI_DC   17
  #define E_INK_PIN_SPI_CS   13
  #define E_INK_PIN_SPI_DIN  23
  #define E_INK_PIN_SPI_SCK  18 
  #define E_INK_PIN_SPI_MISO 24//n/A

//Lora Pinning for TTGO ESP32 Mini V2 mit EINK+Lora Addon Board
  #define LORA_PIN_SPI_MOSI 5
  #define LORA_PIN_SPI_MISO 19
  #define LORA_PIN_SPI_SCK  18//14 = 1 Jumperstellung //18 = 3 Jumperstellung
  #define LORA_PIN_SPI_NSS  26  
  #define LORA_PIN_SPI_RST  33  
  #define LORA_PIN_SPI_DIO1 27
  #define LORA_PIN_SPI_DIO0 27

  #define APP_EUI_LORA 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
  #define DEV_EUI_LORA 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
  #define APP_KEY_LORA 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
#endif

#ifdef geraet4 //ESP32 CAM
//Check here your Version and Pinnings
  #define TTGOT5 1 //neded to indicate the E-INK Library a different pinning
  #define E_INK_PIN_SPI_BUSY 4
  #define E_INK_PIN_SPI_RST  16
  #define E_INK_PIN_SPI_DC   17
  #define E_INK_PIN_SPI_CS   13
  #define E_INK_PIN_SPI_DIN  23
  #define E_INK_PIN_SPI_SCK  18 
  #define E_INK_PIN_SPI_MISO 24//n/A

//Lora Pinning for TTGO CAM
  #define LORA_PIN_SPI_MOSI 13
  #define LORA_PIN_SPI_MISO 12//n/A
  #define LORA_PIN_SPI_SCK  14
  #define LORA_PIN_SPI_NSS  00  
  #define LORA_PIN_SPI_RST  03//27   
  #define LORA_PIN_SPI_DIO1 16
  #define LORA_PIN_SPI_DIO0 16

  #define APP_EUI_LORA 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
  #define DEV_EUI_LORA 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
  #define APP_KEY_LORA 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
#endif


#ifdef geraet5 //ESP32 TTGO MINI V2
//Check here your Version and Pinnings
  #define TTGOT5 1 //neded to indicate the E-INK Library a different pinning
  #define E_INK_PIN_SPI_BUSY 4
  #define E_INK_PIN_SPI_RST  16
  #define E_INK_PIN_SPI_DC   17
  #define E_INK_PIN_SPI_CS   13
  #define E_INK_PIN_SPI_DIN  23
  #define E_INK_PIN_SPI_SCK  18 
  #define E_INK_PIN_SPI_MISO 24//n/A

//Lora Pinning for TTGO ESP32 Mini V2 mit EINK+Lora Addon Board
  #define LORA_PIN_SPI_MOSI 23
  #define LORA_PIN_SPI_MISO 19//n/A
  #define LORA_PIN_SPI_SCK  18
  #define LORA_PIN_SPI_NSS  26  
  #define LORA_PIN_SPI_RST  03//27   
  #define LORA_PIN_SPI_DIO1 05
  #define LORA_PIN_SPI_DIO0 05
  
  #define APP_EUI_LORA 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
  #define DEV_EUI_LORA 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
  #define APP_KEY_LORA 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
#endif

#ifdef geraet6 //ESP32 TTGO MINI V2
//Check here your Version and Pinnings
  #define TTGOT5 1 //neded to indicate the E-INK Library a different pinning
  #define E_INK_PIN_SPI_BUSY 4
  #define E_INK_PIN_SPI_RST  16
  #define E_INK_PIN_SPI_DC   17
  #define E_INK_PIN_SPI_CS   13
  #define E_INK_PIN_SPI_DIN  23
  #define E_INK_PIN_SPI_SCK  18 
  #define E_INK_PIN_SPI_MISO 24//n/A

//Lora Pinning for TTGO ESP32 Mini V2 mit EINK+Lora Addon Board
  #define LORA_PIN_SPI_MOSI 23
  #define LORA_PIN_SPI_MISO 19//n/A
  #define LORA_PIN_SPI_SCK  18
  #define LORA_PIN_SPI_NSS  26  
  #define LORA_PIN_SPI_RST  03//27   
  #define LORA_PIN_SPI_DIO1 05
  #define LORA_PIN_SPI_DIO0 05

  #define APP_EUI_LORA 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
  #define DEV_EUI_LORA 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
  #define APP_KEY_LORA 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
#endif

//EEPROM HASH für Joins OTAA gegen die geprüft wird

#ifdef geraet1
  #define DATAVALID 0xACF2AFC2//1                     // Pattern for data valid in EEPROM/RTC memory
#endif
#ifdef geraet2
  #define DATAVALID 0xACF2AFC2//3                     // Pattern for data valid in EEPROM/RTC memory
#endif
#ifdef geraet3
  #define DATAVALID 0xACF2AFCA                     // Pattern for data valid in EEPROM/RTC memory
#endif
#ifdef geraet4
  #define DATAVALID 0xACF2AFCA                     // Pattern for data valid in EEPROM/RTC memory
#endif
#ifdef geraet5
  #define DATAVALID 0xACF2AFCA                     // Pattern for data valid in EEPROM/RTC memory
#endif
#ifdef geraet6
  #define DATAVALID 0xACF2AFCB                     // Pattern for data valid in EEPROM/RTC memory
#endif
// Features

#ifdef geraet1
  const unsigned TX_INTERVAL = 20; 
  #define BMP_ADRESS 0x76
  #define BMP280_CONNECTED //geraet1
#endif
#ifdef geraet2 //limit uplinks on testboard
  const unsigned TX_INTERVAL = 180; 
  #define BMP_ADRESS 0x76
  #define BMP280_CONNECTED //geraet1
#endif
#ifdef geraet3 //limit uplinks on testboard
  const unsigned TX_INTERVAL = 45; 
  #define BME280_CONNECTED
  #define BME_ADRESS 0x77
  #define CCS811_CONNECTED
  #define CCS811_ADRESS 0x5A
#endif
#ifdef geraet4 //limit uplinks on testboard
  const unsigned TX_INTERVAL = 45; 
  /*#define BME280_CONNECTED
  #define BME_ADRESS 0x77
  #define CCS811_CONNECTED
  #define CCS811_ADRESS 0x5A*/
#endif
#ifdef geraet5 //limit uplinks on testboard
  const unsigned TX_INTERVAL = 45; 
  /*#define BME280_CONNECTED
  #define BME_ADRESS 0x77
  #define CCS811_CONNECTED
  #define CCS811_ADRESS 0x5A*/
#endif
#ifdef geraet6 //limit uplinks on testboard
  const unsigned TX_INTERVAL = 80; //180
  /*#define BME280_CONNECTED
  #define BME_ADRESS 0x77
  #define CCS811_CONNECTED
  #define CCS811_ADRESS 0x5A*/
#endif
