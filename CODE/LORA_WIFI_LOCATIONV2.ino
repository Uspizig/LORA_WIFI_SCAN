//Features:
// - V3 Stack TTN Kompatibel
// - Macht OTAA Join einmal im Leben, dann nur noch ABP
// - OTAA Join Daten werden im RTC und EEPROM gespeichert
//- Join typisch nach 2-3 Versuchen
// Works: Saves Join Data in RTC Memory and EEPROM
// Works: Reads Join Daten from RTC Memory or EEPROM.
// Geraet5 is without Display! TTGO V2 Mini ESP32 mit /*LORA SHIELD https://www.banggood.com/de/LILYGO-TTGO-MINI-32-V2_0-ESP32-WiFi-bluetooth-Module-Development-Board-p-1286046.html */
// Geraet4 is without Display! but equipped with an ESP CAM - DONT FORGET TO CHANGE PROCESSOR TO CAM AI MODULE!!! //https://www.ebay.de/itm/274654663732?
// Geraet3 is without Display! but equipped with BME280, and CCS811
// Geraet2 with BME280 //https://de.aliexpress.com/item/4000804336457.html mit LORA SHIELD von USPIZIG
// Geraet1 with BMP280 //https://de.aliexpress.com/item/4000804336457.html mit LORA SHIELD von USPIZIG


//Here some OPTIONAL Changes in the Stack:
//#define LMICbandplan_getInitialDrJoin() (EU868_DR_SF8) in lmic_bandplan_eu868.h
//#define LMIC_PRINTF_TO Serial //USpizig  in config.h bringt aber keinerlei zusätzliche debug Ausgaben
//in config.h Debug Messages an und RFM95 definiert
//in oslmic.h ersetze #define OSTICKS_PER_SEC von 32768 in 50000 //Uspizig bringt aber nix

//MANDATORY CHANGES to MAKE IT WORK
//in hal.cpp replaced spi.begin(); to spi.begin(14, 2, 15, 26); to match YOUR pinning
/*
static void hal_spi_init () {
    //SPI.begin();//Original Änderung in hal.cpp
  //SPI.begin(lmic_pins.sck, lmic_pins.miso, lmic_pins.mosi, lmic_pins.nss);
  SPI.begin(14, 2, 15, 26);
}
*/

//for Debugging
//#define SingleChannelMode 1 //to check on own gateway Join Behaviour
#define LMIC_DEBUG_LEVEL 1
//#define geraet1 //soldered DIO0 directly to GPIO 34 and DIO01 to GPIO25  == Board identifies as COM26
//#define geraet2 //Both DIO00 and DIO01 soldered with a Diode to GPIO 25  == Board identifies as COM7
//#define geraet3 //ESP32 TTGO Mini V2 COM9
//#define geraet4 //ESP32 CAM
//#define geraet5 //TTGO MINI V2 
#define geraet6 //TTGO MINI V2  -> TTN Stack V3

//const unsigned long UpdateInterval = (60L * 20L - 03) * 1000000L; // Update delay in microseconds
#define uS_TO_S_FACTOR 1000000  /* Conversion factor for micro seconds to seconds */
#define TIME_TO_SLEEP  60        /* Time ESP32 will go to sleep (in seconds) */

//Battery Pin
#define BATTERY_PIN 35

#include "lorapindef_and_features.h"  
#include <lmic.h>
#include <hal/hal.h>
#include <SPI.h>
#include <EEPROM.h>


//Neopixel
#include <Adafruit_NeoPixel.h>
#define LED_PIN 2
#define wartezeit 5
#define intense_max 255
Adafruit_NeoPixel strip = Adafruit_NeoPixel(3, LED_PIN, NEO_GRB + NEO_KHZ800);
//Adafruit_NeoPixel strip = Adafruit_NeoPixel(120, PIN, NEO_GRB + NEO_KHZ800);//60 LEDs/meter


#include <LoraMessage.h>

//Temperatur:
#include <Wire.h>
#ifdef BMP280_CONNECTED
  #include <Adafruit_BMP280.h>
  Adafruit_BMP280 bmp; // I2C
#endif

#ifdef BME280_CONNECTED
  #define SEALEVELPRESSURE_HPA (1013.25)
  #include <Adafruit_BME280.h>
  Adafruit_BME280 bme;
#endif

#ifdef CCS811_CONNECTED 
  #include <SparkFunCCS811.h> //Click here to get the library: http://librarymanager/All#SparkFun_CCS811
  CCS811 myCCS811(CCS811_ADRESS);
#endif


#ifdef geraet3
  #define sda 21 ///* I2C Pin Definition */
  #define scl 22 ///* I2C Pin Definition */
#else
  #define sda 21 ///* I2C Pin Definition */
  #define scl 22 ///* I2C Pin Definition */
#endif

#ifdef geraet6
  long lastMsg = 0;
  #define updaterateWIFI 3000
#else
#endif

float temp = 23;
float pressure = 980;
float humidity = 50;
float ccs811_CO2 = 450;
float ccs811_TVOC = 2;
int battery = 50;
int port = 0;

typedef struct {
   int Temp;
   int Feuchte;
   int Temp_max;
   int Temp_min;
   int Condition_Code;
   int Muell_Code;
} payload_record_type;
  
payload_record_type vorhersage[1];





#include "esp_timer.h"
#include "Arduino.h"
#include "fb_gfx.h"
#include "fd_forward.h"
#include "fr_forward.h"
#include "soc/soc.h"           // Disable brownout problems
#include "soc/rtc_cntl_reg.h"  // Disable brownout problems
#include "driver/rtc_io.h"
#include "WiFi.h"
#include <WiFiClientSecure.h>
#include <ArduinoJson.h> 
#include "credentials.h"  
char ssid[] = mySSID;     // your network SSID (name)
char password[] = myPASSWORD; // your network key


WiFiClientSecure client;

/*Wifi Sort Test*/
#define WIFI_DELAY        500
#define MAX_SSID_LEN      32 /* Max SSID octets. */
char ssid2[MAX_SSID_LEN] = "";/* SSID that to be stored to connect. */
#define WIFI_DELAY        500/* Delay paramter for connection. */

typedef struct { /*Struct für einfache Speicherung der Daten*/
  
   byte mac0;
   byte mac1;
   byte mac2;
   byte mac3;
   byte mac4;
   byte mac5;
   String mac;
   int EmpfangsPegel;
   int Encrypted;
} Wifi_record_type;
Wifi_record_type Found_Wifi[5];




uint32_t start_time;
uint32_t next_time;
uint32_t previous_time;
uint32_t previous_full_update;

uint32_t total_seconds = 0;
uint32_t startup_seconds = 0;
uint32_t seconds, minutes, hours, days;

uint32_t join_total_seconds = 0;
uint32_t join_seconds, join_minutes, join_hours, join_days;

//Partial Update:
const uint32_t partial_update_period_s = 30;
//const uint32_t full_update_period_s = 60 * 60 * 60;//alle 2.5 Tage
const uint32_t full_update_period_s = 60 * 60;//(jede Stunde)
//const uint32_t full_update_period_s = 60;(jede Minute)
/*EINK Teile ENDE*/

//Variables for ABP after OTAA Join
u1_t NWKSKEY[16] ;                               // LoRaWAN NwkSKey, network session key.
u1_t APPSKEY[16] ;                               // LoRaWAN AppSKey, application session key.
u4_t DEVADDR ;



//RealTimeVariables which don't get erased during Sleep
  struct Daten_struct {//https://esp32.com/viewtopic.php?t=11984
    uint16_t Stack[15];
    uint8_t Counter_History;
  };
  RTC_DATA_ATTR uint32_t SAVED_dataValid ;                           // DATAVALID if valid data (joined)
  RTC_DATA_ATTR uint8_t  SAVED_devaddr[4] ;                          // Device address after join
  RTC_DATA_ATTR uint8_t  SAVED_nwkKey[16] ;                          // Network session key after join
  RTC_DATA_ATTR uint8_t  SAVED_artKey[16] ;                          // Aplication session key after join
  RTC_DATA_ATTR uint32_t SAVED_seqnoUp ;                             // Sequence number       
  bool OTAA = true ;   //startup with OTAA if No EEPROM was Saved before
  //History of Data
  RTC_DATA_ATTR  struct Daten_struct Druck_Werte;                 
  RTC_DATA_ATTR  struct Daten_struct Temp_Werte;
  RTC_DATA_ATTR  struct Daten_struct CO2_Werte;
  RTC_DATA_ATTR  struct Daten_struct TVOC_Werte;                                                   
  RTC_DATA_ATTR  struct Daten_struct Feuchte_Werte;


// Boot Counter
RTC_DATA_ATTR uint32_t SAVED_boot_counter =0;

//variables for OTAA JOIN
  static const u1_t PROGMEM APPEUI[8]={ APP_EUI_LORA };//lsb
  void os_getArtEui (u1_t* buf) { memcpy_P(buf, APPEUI, 8);}
  
  static const u1_t PROGMEM DEVEUI[8]={ DEV_EUI_LORA };//lsb
  void os_getDevEui (u1_t* buf) { memcpy_P(buf, DEVEUI, 8);}

  //msb
  static const u1_t PROGMEM APPKEY[16] = { APP_KEY_LORA };
  void os_getDevKey (u1_t* buf) {  memcpy_P(buf, APPKEY, 16);}  



int verbunden_indicator = 0;
int Packet_Transmission_ongoing = 0; // Display wird nur aktualisiert wenn ein Paket gesendet wird
int packet_counter =0;
int packet_counter_rx =0;
static osjob_t sendjob;
devaddr_t DeviceName = 0;

// Pin mapping

const lmic_pinmap lmic_pins = {
    .nss = LORA_PIN_SPI_NSS,
    .rxtx = LMIC_UNUSED_PIN,
    .rst = LMIC_UNUSED_PIN,
    #ifdef geraet1
      .dio = {LORA_PIN_SPI_DIO0, LORA_PIN_SPI_DIO1, LMIC_UNUSED_PIN}, 
    #else
      .dio = {LORA_PIN_SPI_DIO1, LORA_PIN_SPI_DIO1, LMIC_UNUSED_PIN}, 
    #endif
    .rxtx_rx_active = 0,//kopiert von TTGO Board
    .rssi_cal = 10,//kopiert von TTGO Board
    //.spi_freq = 8000000, /* 8MHz */ //kopiert von TTGO Board
    .spi_freq = 4000000, //4 MHZ from GxEPD2 https://github.com/ZinggJM/GxEPD2/tree/master/extras/sw_spi
};


//Hilfsfunktion um Hex Vafriablen auszugeben
void printHex2(unsigned v) {
    v &= 0xff;
    if (v < 16)
        Serial.print('0');
    Serial.print(v, HEX);
}

void onEvent (ev_t ev) {
    Serial.print(os_getTime());
    Serial.print(": ");
    switch(ev) {
        case EV_SCAN_TIMEOUT:
            Serial.println(F("EV_SCAN_TIMEOUT"));
            break;
        case EV_BEACON_FOUND:
            Serial.println(F("EV_BEACON_FOUND"));
            break;
        case EV_BEACON_MISSED:
            Serial.println(F("EV_BEACON_MISSED"));
            break;
        case EV_BEACON_TRACKED:
            Serial.println(F("EV_BEACON_TRACKED"));
            break;
        case EV_JOINING:
            Serial.println(F("EV_JOINING"));
            verbunden_indicator = 1;
            break;
        case EV_JOINED:
            verbunden_indicator = 2;
            total_seconds =0;
            Serial.println(F("EV_JOINED"));
            {
              u4_t netid = 0;
              devaddr_t devaddr = 0;
              u1_t nwkKey[16];
              u1_t artKey[16];
              LMIC_getSessionKeys(&netid, &devaddr, nwkKey, artKey);
              Serial.print("netid: "); Serial.println(netid, DEC);
              Serial.print("devaddr: "); Serial.println(devaddr, HEX);
              DeviceName = devaddr;
              Serial.print("AppSKey: ");
              for (size_t i=0; i<sizeof(artKey); ++i) {
                if (i != 0)
                  Serial.print("-");
                printHex2(artKey[i]);
              }
              Serial.println("");
              Serial.print("NwkSKey: ");
              for (size_t i=0; i<sizeof(nwkKey); ++i) {
                      if (i != 0)
                              Serial.print("-");
                      printHex2(nwkKey[i]);
              }
              Serial.println();
              saveToRTC(); // Speichere Werte
              //SAVED_dataValid = 0;// Setze zu Testzwecken Datavalid = 0 damit Daten aus dem EEPROM Gelesen werden
              //retrieveKeys(); /Rücklesen der Werte aus EEPROM
            }
            /* Disable link check validation (automatically enabled during join, but because slow data rates change max TX size, we don't use it in this example.*/
            LMIC_setLinkCheckMode(0);
            break;
        
        case EV_JOIN_FAILED:
            Serial.println(F("EV_JOIN_FAILED"));
            break;
        case EV_REJOIN_FAILED:
            Serial.println(F("EV_REJOIN_FAILED"));
            break;
        case EV_TXCOMPLETE:
            verbunden_indicator = 4;
            packet_counter++;
            Serial.println(F("EV_TXCOMPLETE (includes RX time)"));
            if (LMIC.txrxFlags & TXRX_ACK)
              Serial.println(F("Received ack"));
            if (LMIC.dataLen) {
              Serial.print(LMIC.dataLen); Serial.println(F("Bytes of Payload-Data Received "));
              for (int i = 0; i < LMIC.dataLen; i++) {
                /*if (LMIC.frame[LMIC.dataBeg + i] < 0x10) {Serial.print(F("0"));}
                Serial.print(LMIC.frame[LMIC.dataBeg + i], HEX);*/
                //recieved_payload[i] = LMIC.frame[LMIC.dataBeg + i]; 
                packet_counter_rx++;
              }
              //decode_rec_payload();
            }
            Packet_Transmission_ongoing = 0;
            saveToRTC();// Tryout to save every time to RTC Memory once a packet has been transmitted correct.
            os_setTimedCallback(&sendjob, os_getTime()+sec2osticks(TX_INTERVAL), do_send); // Schedule next transmission
            //kurzschlaf();
            break;
        case EV_LOST_TSYNC:
            Serial.println(F("EV_LOST_TSYNC"));
            break;
        case EV_RESET:
            Serial.println(F("EV_RESET"));
            break;
        case EV_RXCOMPLETE:
            // data received in ping slot
            Serial.println(F("EV_RXCOMPLETE"));
            break;
        case EV_LINK_DEAD:
            Serial.println(F("EV_LINK_DEAD"));
            break;
        case EV_LINK_ALIVE:
            Serial.println(F("EV_LINK_ALIVE"));
            break;
        /*
        || This event is defined but not used in the code. No
        || point in wasting codespace on it.
        ||
        || case EV_SCAN_FOUND:
        ||    Serial.println(F("EV_SCAN_FOUND"));
        ||    break;
        */
        case EV_TXSTART:
            Serial.println(F("EV_TXSTART"));
            verbunden_indicator = 3;
            break;
        case EV_TXCANCELED:
            Serial.println(F("EV_TXCANCELED"));
            break;
        case EV_RXSTART:
            /* do not print anything -- it wrecks timing */
            break;
        case EV_JOIN_TXCOMPLETE:
            Serial.println(F("EV_JOIN_TXCOMPLETE: No JoinAccept"));
            break;
        default:
            Serial.print(F("Unknown event: "));
            Serial.println((unsigned) ev);
            break;
    }
}



// combines 2 Bytes high and low Bytes and combines it to one integer
int BitShiftCombine( unsigned char x_low, unsigned char x_high)
{
  int combined;
  combined = x_high;              //send x_high to rightmost 8 bits
  combined = combined<<8;         //shift x_high over to leftmost 8 bits
  combined |= x_low;                 //logical OR keeps x_high intact in combined and fills in                                                             //rightmost 8 bits
  return combined;
}


// Transmits Data to Lora
void do_send(osjob_t* j){
    // Check if there is not a current TX/RX job running
    if (LMIC.opmode & OP_TXRXPEND) {
        Serial.println(F("OP_TXRXPEND, not sending"));
    } else {        
        /*LoraMessage message;
        message.addTemperature(temp);//1
        message.addHumidity(humidity);//2
        message.addUint16(int(pressure));//4
        message.addUint16(int(ccs811_CO2));//5
        message.addUint16(int(ccs811_TVOC));//6
        LMIC_setTxData2(1, message.getBytes(), message.getLength(), 0);
        
        Serial.println(F("Packet queued"));
        if (port<3){
          port++;
        }
        else{
          port=0;
        } */ 
          Serial.println(F("Packet queued"));
          port=0;
          uint8_t buffer[7];
          uint8_t buffer2[7];
          uint8_t buffer3[7];
          uint8_t payload[21];
          int wifi_signalstaerke = Found_Wifi[port].EmpfangsPegel + 140;
          memcpy(buffer, &Found_Wifi[port].mac0, sizeof(Found_Wifi[port].mac0));
          memcpy(buffer+sizeof(Found_Wifi[port].mac0), &Found_Wifi[port].mac1, sizeof(Found_Wifi[port].mac1));
          memcpy(buffer+sizeof(Found_Wifi[port].mac0)+sizeof(Found_Wifi[port].mac1), &Found_Wifi[port].mac2, sizeof(Found_Wifi[port].mac2));
          memcpy(buffer+sizeof(Found_Wifi[port].mac0)+sizeof(Found_Wifi[port].mac1)+sizeof(Found_Wifi[port].mac2), &Found_Wifi[port].mac3, sizeof(Found_Wifi[port].mac3));
          memcpy(buffer+sizeof(Found_Wifi[port].mac0)+sizeof(Found_Wifi[port].mac1)+sizeof(Found_Wifi[port].mac2)+sizeof(Found_Wifi[port].mac3), &Found_Wifi[port].mac4, sizeof(Found_Wifi[port].mac4));
          memcpy(buffer+sizeof(Found_Wifi[port].mac0)+sizeof(Found_Wifi[port].mac1)+sizeof(Found_Wifi[port].mac2)+sizeof(Found_Wifi[port].mac3)+sizeof(Found_Wifi[port].mac4), &Found_Wifi[port].mac5, sizeof(Found_Wifi[port].mac5));
          memcpy(buffer+sizeof(Found_Wifi[port].mac0)+sizeof(Found_Wifi[port].mac1)+sizeof(Found_Wifi[port].mac2)+sizeof(Found_Wifi[port].mac3)+sizeof(Found_Wifi[port].mac4)+sizeof(Found_Wifi[port].mac5), &wifi_signalstaerke, sizeof(wifi_signalstaerke)); 

          wifi_signalstaerke = Found_Wifi[port+1].EmpfangsPegel + 140;
          memcpy(buffer2, &Found_Wifi[port+1].mac0, sizeof(Found_Wifi[port+1].mac0));
          memcpy(buffer2+sizeof(Found_Wifi[port+1].mac0), &Found_Wifi[port+1].mac1, sizeof(Found_Wifi[port+1].mac1));
          memcpy(buffer2+sizeof(Found_Wifi[port+1].mac0)+sizeof(Found_Wifi[port+1].mac1), &Found_Wifi[port+1].mac2, sizeof(Found_Wifi[port+1].mac2));
          memcpy(buffer2+sizeof(Found_Wifi[port+1].mac0)+sizeof(Found_Wifi[port+1].mac1)+sizeof(Found_Wifi[port+1].mac2), &Found_Wifi[port+1].mac3, sizeof(Found_Wifi[port+1].mac3));
          memcpy(buffer2+sizeof(Found_Wifi[port+1].mac0)+sizeof(Found_Wifi[port+1].mac1)+sizeof(Found_Wifi[port+1].mac2)+sizeof(Found_Wifi[port+1].mac3), &Found_Wifi[port+1].mac4, sizeof(Found_Wifi[port+1].mac4));
          memcpy(buffer2+sizeof(Found_Wifi[port+1].mac0)+sizeof(Found_Wifi[port+1].mac1)+sizeof(Found_Wifi[port+1].mac2)+sizeof(Found_Wifi[port+1].mac3)+sizeof(Found_Wifi[port+1].mac4), &Found_Wifi[port+1].mac5, sizeof(Found_Wifi[port+1].mac5));
          memcpy(buffer2+sizeof(Found_Wifi[port+1].mac0)+sizeof(Found_Wifi[port+1].mac1)+sizeof(Found_Wifi[port+1].mac2)+sizeof(Found_Wifi[port+1].mac3)+sizeof(Found_Wifi[port+1].mac4)+sizeof(Found_Wifi[port+1].mac5), &wifi_signalstaerke, sizeof(wifi_signalstaerke)); 
          
          wifi_signalstaerke = Found_Wifi[port+2].EmpfangsPegel + 140;
          memcpy(buffer3, &Found_Wifi[port+2].mac0, sizeof(Found_Wifi[port+2].mac0));
          memcpy(buffer3+sizeof(Found_Wifi[port+2].mac0), &Found_Wifi[port+2].mac1, sizeof(Found_Wifi[port+2].mac1));
          memcpy(buffer3+sizeof(Found_Wifi[port+2].mac0)+sizeof(Found_Wifi[port+2].mac1), &Found_Wifi[port+2].mac2, sizeof(Found_Wifi[port+2].mac2));
          memcpy(buffer3+sizeof(Found_Wifi[port+2].mac0)+sizeof(Found_Wifi[port+2].mac1)+sizeof(Found_Wifi[port+2].mac2), &Found_Wifi[port+2].mac3, sizeof(Found_Wifi[port+2].mac3));
          memcpy(buffer3+sizeof(Found_Wifi[port+2].mac0)+sizeof(Found_Wifi[port+2].mac1)+sizeof(Found_Wifi[port+2].mac2)+sizeof(Found_Wifi[port+2].mac3), &Found_Wifi[port+2].mac4, sizeof(Found_Wifi[port+2].mac4));
          memcpy(buffer3+sizeof(Found_Wifi[port+2].mac0)+sizeof(Found_Wifi[port+2].mac1)+sizeof(Found_Wifi[port+2].mac2)+sizeof(Found_Wifi[port+2].mac3)+sizeof(Found_Wifi[port+2].mac4), &Found_Wifi[port+2].mac5, sizeof(Found_Wifi[port+2].mac5));
          memcpy(buffer3+sizeof(Found_Wifi[port+2].mac0)+sizeof(Found_Wifi[port+2].mac1)+sizeof(Found_Wifi[port+2].mac2)+sizeof(Found_Wifi[port+2].mac3)+sizeof(Found_Wifi[port+2].mac4)+sizeof(Found_Wifi[port+2].mac5), &wifi_signalstaerke, sizeof(wifi_signalstaerke)); 

          memcpy(payload, &buffer, sizeof(buffer));
          memcpy(payload+sizeof(buffer), &buffer2, sizeof(buffer2));
          memcpy(payload+sizeof(buffer2)+sizeof(buffer), &buffer3, sizeof(buffer3));
           
           /*Serial.print("0:");  Serial.println(Found_Wifi[port].mac0);
           Serial.print("1:");  Serial.println(Found_Wifi[port].mac1);
           Serial.print("2:");  Serial.println(Found_Wifi[port].mac2);
           Serial.print("3:");  Serial.println(Found_Wifi[port].mac3);
           Serial.print("4:");  Serial.println(Found_Wifi[port].mac4);
           Serial.print("5:");  Serial.println(Found_Wifi[port].mac5);*/
           Serial.print("Port:");  Serial.println(port);
           Serial.print("Pegel:"); Serial.println(Found_Wifi[port].EmpfangsPegel);
          LMIC_setTxData2(port+1, payload, sizeof(payload), 0);
   
        
        Packet_Transmission_ongoing = 1;
        Serial.println(F("Packet ongoing"));
    }
    // Next TX is scheduled after TX_COMPLETE event.
}

void setup_air_sensor(void){
  #ifdef BMP280_CONNECTED
      bool status2 = BMP_Start();//alles ok?
      BMP_Test();
      delay(2000);
    #endif
    
    #ifdef BME280_CONNECTED
      bool status2 = BME_Start();//alles ok?
      BME_Test();
      delay(2000);
    #endif

    #ifdef CCS811_CONNECTED
      CCS811Core::CCS811_Status_e returnCode = myCCS811.beginWithStatus();
      Serial.print("CCS811 begin exited with: ");
      Serial.println(myCCS811.statusString(returnCode));
      CCS811_Test();
    #endif
}



//Init Lora Stack, sets ADR Mode, 
void setup_lora(void){
  pinMode(LORA_PIN_SPI_DIO1, INPUT_PULLDOWN);//to enable PullDown but update your ESP32 Lib to avoid https://esp32.com/viewtopic.php?t=439
  pinMode(LORA_PIN_SPI_DIO0, INPUT_PULLDOWN);
  // LMIC init
    os_init();
    
    LMIC_reset(); // Reset the MAC state. Session and pending data transfers will be discarded.
    
    LMIC_setupChannel(0, 868100000, DR_RANGE_MAP(DR_SF12, DR_SF7),  BAND_CENTI);      // g-band
    LMIC_setupChannel(1, 868300000, DR_RANGE_MAP(DR_SF12, DR_SF7B), BAND_CENTI);      // g-band
    LMIC_setupChannel(2, 868500000, DR_RANGE_MAP(DR_SF12, DR_SF7),  BAND_CENTI);      // g-band
    LMIC_setupChannel(3, 867100000, DR_RANGE_MAP(DR_SF12, DR_SF7),  BAND_CENTI);      // g-band
    LMIC_setupChannel(4, 867300000, DR_RANGE_MAP(DR_SF12, DR_SF7),  BAND_CENTI);      // g-band
    LMIC_setupChannel(5, 867500000, DR_RANGE_MAP(DR_SF12, DR_SF7),  BAND_CENTI);      // g-band
    LMIC_setupChannel(6, 867700000, DR_RANGE_MAP(DR_SF12, DR_SF7),  BAND_CENTI);      // g-band
    LMIC_setupChannel(7, 867900000, DR_RANGE_MAP(DR_SF12, DR_SF7),  BAND_CENTI);      // g-band
    LMIC_setupChannel(8, 868800000, DR_RANGE_MAP(DR_FSK,  DR_FSK),  BAND_MILLI);      // g2-band
    
    //Debugging Purpose for Single Channel
    #ifdef SingleChannelMode
      #define CHANNEL  1
      for (uint8_t i = 0; i < 9; i++) {
        if (i != CHANNEL) {
          LMIC_disableChannel(i);
        }
      }
    #endif
    
    //Adaptive Data Rate Mode https://www.thethingsnetwork.org/docs/lorawan/adaptive-data-rate.html 
    LMIC_setAdrMode(1); //Adaptiert Datenrate nach 64 Paketen // war ausgeschaltet
    LMIC_setLinkCheckMode(1);//war 0

    
    // TTN uses SF9 for its RX2 window.
    LMIC.dn2Dr = DR_SF9;
    
    // Set data rate and transmit power for uplink moved depending if OTAA or ABP
    //LMIC_setDrTxpow(DR_SF12,14);//Langsamster Modus: elendslange AirTime ~820ms aber sichere Übertragung
    //LMIC_setDrTxpow(DR_SF8,14); //Kurz schnell unzuverlässig 
    
    LMIC_setClockError(MAX_CLOCK_ERROR * 10 / 100);//https://www.thethingsnetwork.org/forum/t/need-help-with-mcci-lmic-and-ttn-join-wait-issue/30846

    if ( OTAA )
    {
      Serial.println ( "Start joining" ) ;
      //LMIC_setDrTxpow(DR_SF12,14);//Langsamster Modus: elendslange AirTime ~820ms aber sichere Übertragung
      LMIC_setDrTxpow(DR_SF9,14); //with SF9 initial Join is faster... yes I know for this Reset Cycle it will stay on SF9 and consume more airtime
      do_send (&sendjob) ;
    }
    else
    {
      Serial.printf ( "starte mit SF8: gespeichertes SAVED_seqnoUp: %d\n", SAVED_seqnoUp ) ;
      LMIC_setDrTxpow(DR_SF8,14); //otherwise stupid ABP will start with SF12!!! -> Test
      memdmp ( "No OTAA needed - Set Session, DEVADDR:", (uint8_t*)&DEVADDR, 4 ) ;
      memdmp ( "NWKSKEY:", NWKSKEY, 16 ) ;
      memdmp ( "APPSKEY:", APPSKEY, 16 ) ;
      LMIC_setSession ( 0x13, DEVADDR, NWKSKEY, APPSKEY ) ;
      //Serial.printf ( "Seqnr set to %d\n", SAVED_seqnoUp ) ;
      LMIC_setupChannel(0, 868100000, DR_RANGE_MAP(DR_SF12, DR_SF7),  BAND_CENTI);      // g-band
      LMIC_setupChannel(1, 868300000, DR_RANGE_MAP(DR_SF12, DR_SF7B), BAND_CENTI);      // g-band
      LMIC_setupChannel(2, 868500000, DR_RANGE_MAP(DR_SF12, DR_SF7),  BAND_CENTI);      // g-band
      LMIC_setupChannel(3, 867100000, DR_RANGE_MAP(DR_SF12, DR_SF7),  BAND_CENTI);      // g-band
      LMIC_setupChannel(4, 867300000, DR_RANGE_MAP(DR_SF12, DR_SF7),  BAND_CENTI);      // g-band
      LMIC_setupChannel(5, 867500000, DR_RANGE_MAP(DR_SF12, DR_SF7),  BAND_CENTI);      // g-band
      LMIC_setupChannel(6, 867700000, DR_RANGE_MAP(DR_SF12, DR_SF7),  BAND_CENTI);      // g-band
      LMIC_setupChannel(7, 867900000, DR_RANGE_MAP(DR_SF12, DR_SF7),  BAND_CENTI);      // g-band
      LMIC_setupChannel(8, 868800000, DR_RANGE_MAP(DR_FSK,  DR_FSK),  BAND_MILLI);      // g2-band
      LMIC_setLinkCheckMode(1); //test Disable Link Check Mode
      LMIC.dn2Dr = DR_SF9;//test
      LMIC.seqnoUp = SAVED_seqnoUp ;  
      do_send (&sendjob) ;
    }
    
    
    //Eingebaut in /src/lmic.c geht nicht
    //LMIC_dn2dr = EU868_DR_SF9;//https://github.com/mcci-catena/arduino-lmic/issues/455
    //LMIC_selectSubBand(0); //https://github.com/mcci-catena/arduino-lorawan/issues/74
    // Start job (sending automatically starts OTAA too)
}
//***************************************************************************************************
//                                      INIT DATA 2 RTC                                                 *
//***************************************************************************************************
// Sets basic Value for RTC Data fields
// Set all Values of Temp Pressure on first startup to a typical value
// If this is a Deep Sleep wakeup and 
//***************************************************************************************************
void INIT_DATA_2_RTC(int value, struct Daten_struct *daten){
  if (daten->Counter_History != 0){ // Da war schonmal ein Wert im RTC Memory. kein erster Startup
    Serial.println("RTC Stack war schon befüllt mit Werten... nur Sleep");
  }
  else{//in dem RTC Memory war noch nie was drin
    Serial.print("\n RTC Stack wird initalisiert mit Werten:");
    for (int i = 0; i < 15; i++){
      daten->Stack[i] = value+i;
      Serial.print(i);Serial.print(":");Serial.print(daten->Stack[i]);Serial.print("/");
    }
    daten->Counter_History = 14;
    Serial.print(" DatenCounter:");Serial.println(daten->Counter_History);
  }
}
//***************************************************************************************************
//                                      SAVE DATA 2 RTC                                                 *
//***************************************************************************************************
// Sets basic Value for RTC Data fields
// Set all Values of Temp Pressure on first startup to a typical value
// Fills in Values according to LiFo, each value is shifted one higher. latest entries gets number 0
//***************************************************************************************************
void SAVE_DATA_2_RTC(int value, struct Daten_struct *daten){
   //Serial.println("Gespeicherte RTC-Daten:");
   for (int i = 14; i > 0; i--){
    daten->Stack[i] = daten->Stack[i-1];
    //Serial.print(i);Serial.print(":");Serial.print(daten->Stack[i]);Serial.print("/");
   }
   daten->Stack[0] = value;
   //Serial.print("0:");Serial.println(daten->Stack[0]);
}

//***************************************************************************************************
//                                      M E M D M P                                                 *
//***************************************************************************************************
// Dump memory for debug
//***************************************************************************************************
void memdmp ( const char* header, uint8_t* p, uint16_t len )
{
  uint16_t i ;                                                        // Loop control

  Serial.print ( header ) ;                                           // Show header
  for ( i = 0 ; i < len ; i++ )
  {
    if ( ( i & 0x0F ) == 0 )                                          // Continue opn next line?
    {
      if ( i > 0 )                                                    // Yes, continuation line?
      {
        Serial.printf ( "\n" ) ;                                      // Yes, print it
      }
      Serial.printf ( "%04X: ", i ) ;                                 // Print index
    }
    Serial.printf ( "%02X ", *p++ ) ;                                 // Print one data byte
  }
  Serial.println() ;
}


//***************************************************************************************************
//                                    S A V E T O R T C                                             *
//***************************************************************************************************
// Save data in RTC memory.  Every 100th call the data will also be saved in EEPROM memory.         *
// The EEPROM is also updates if OTAA was used.                                                     *
// The space in RTC memory is limited to 512 bytes.                                                 *
//***************************************************************************************************
void saveToRTC()
{
  uint16_t        eaddr ;                                  // Address in EEPROM
  uint8_t*        p ;                                      // Points into savdata

  Serial.printf ( "\n Save data to RTC memory:\n" ) ;
  memcpy ( SAVED_devaddr, &LMIC.devaddr, 4 ) ;           // Fill struct to save
  memcpy ( SAVED_nwkKey,  LMIC.nwkKey, 16 ) ;
  memcpy ( SAVED_artKey,  LMIC.artKey, 16 ) ;
  SAVED_seqnoUp = LMIC.seqnoUp ;
  SAVED_dataValid = DATAVALID ;
  /*memdmp ( "devaddr:", SAVED_devaddr, 4 ) ;  
  memdmp ( "artKey:",  SAVED_artKey, 16 ) ;
  memdmp ( "nwkKey:",  SAVED_nwkKey, 16 ) ;*/
  Serial.printf ( "Saved SeqnoUp is: %d Current SeqnoUp is: %d\n", SAVED_seqnoUp, LMIC.seqnoUp ) ;
  Serial.printf ( "SeqnoDown is %d\n", LMIC.seqnoDn ) ;
  if ( ( ( LMIC.seqnoUp % 50 ) == 0 ) || OTAA )           // Need to save data in EEPROM?
  {
    int EEPROM_Data_Counter =0;
    
    Serial.println ( "Saving to EEPROM" ) ;
    p = (uint8_t*)&SAVED_dataValid ;                               // set target pointer
    for ( eaddr = EEPROM_Data_Counter ; eaddr < sizeof(SAVED_dataValid) ; eaddr++ )
    {
      EEPROM.write ( eaddr, *p++ ) ;                       // Write to EEPROM
      EEPROM_Data_Counter++;
      
    }
    Serial.printf ( "\n Saved %d Bytes of datavalid to EEPROM", EEPROM_Data_Counter);
    p = (uint8_t*)&SAVED_devaddr ;                               // set target pointer
    for ( eaddr = EEPROM_Data_Counter ; eaddr < (sizeof(SAVED_devaddr)+sizeof(SAVED_dataValid)) ; eaddr++ )
    {
      EEPROM.write ( eaddr, *p++ ) ;                       // Write to EEPROM
      EEPROM_Data_Counter++;
    }
    Serial.printf ( "\n Saved %d Bytes of devadr to EEPROM", EEPROM_Data_Counter);   
    p = (uint8_t*)& SAVED_nwkKey ;                               // set target pointer
    for ( eaddr = EEPROM_Data_Counter ; eaddr < (sizeof(SAVED_devaddr)+sizeof(SAVED_dataValid)+ sizeof(SAVED_nwkKey)) ; eaddr++ )
    {
      EEPROM.write ( eaddr, *p++ ) ;                       // Write to EEPROM
      EEPROM_Data_Counter++;
    }
      Serial.printf ( "\n Saved %d Bytes of nwkey to EEPROM", EEPROM_Data_Counter);   
    p = (uint8_t*)& SAVED_artKey ;                               // set target pointer
    for ( eaddr = EEPROM_Data_Counter ; eaddr < (sizeof(SAVED_devaddr)+sizeof(SAVED_dataValid)+ sizeof(SAVED_nwkKey)+sizeof(SAVED_artKey)) ; eaddr++ )
    {
      EEPROM.write ( eaddr, *p++ ) ;                       // Write to EEPROM
      EEPROM_Data_Counter++;
    }
    Serial.printf ( "\n Saved %d Bytes of artkey to EEPROM", EEPROM_Data_Counter);   
    
    p = (uint8_t*)& SAVED_seqnoUp ;                               // set target pointer
    for ( eaddr = EEPROM_Data_Counter ; eaddr < (sizeof(SAVED_devaddr)+sizeof(SAVED_dataValid)+ sizeof(SAVED_nwkKey)+sizeof(SAVED_artKey)+sizeof(SAVED_seqnoUp)) ; eaddr++ )
    {
      EEPROM.write ( eaddr, *p++ ) ;                       // Write to EEPROM
      EEPROM_Data_Counter++;
    }
    Serial.printf ( "\n Saved %d Bytes of seqnr to EEPROM", EEPROM_Data_Counter);   
    
    EEPROM.commit() ;                                      // Commit data to EEPROM
    Serial.printf ( "\n EEPROM operation finished Number of bytes Written: %d", EEPROM_Data_Counter);   
  }
}

//***************************************************************************************************
//                                R E T R I E V E K E Y S                                           *
//***************************************************************************************************
// Try to retrieve the keys en seqnr from non-volitile memory.                                      *
//***************************************************************************************************
void retrieveKeys()
{
  uint16_t eaddr ;                                          // Address in EEPROM
  uint8_t* p ;                                              // Pointer into savdata
  
  // return ;                                               // Return if OTAA is required
  
  //Hier Entscheidung ob RTC Gültig ist oder Defaultwerte drin und aus EEPROM Gelesen werden muss
  //z.B bei Reboot, battery getauscht usw.
  if ( SAVED_dataValid == DATAVALID )                     // DATA in RTC memory valid?
  {
    Serial.println ( "Keys retrieved from RTC memory\n" ) ; // Show retrieve result 
  }
  else
  {
    Serial.println ( "\n Reading Keys from EEPROM :\n" ) ; 
    // No data vailable in RTC memory.  Use EEPROM data. Hole alles aus EEPROM
    int EEPROM_Data_Counter =0;
    p = (uint8_t*)&SAVED_dataValid ;
    for ( eaddr = EEPROM_Data_Counter ; eaddr < sizeof(SAVED_dataValid) ; eaddr++ )
    {
      *p++ = EEPROM.read ( eaddr ) ;                        // Move one byte to savdata
      EEPROM_Data_Counter++;
    }
    Serial.printf ( "\n Recovered %d Bytes of datavalid from EEPROM", EEPROM_Data_Counter);   
    p = (uint8_t*)&SAVED_devaddr ;                               // set target pointer
    for ( eaddr = EEPROM_Data_Counter ; eaddr < (sizeof(SAVED_devaddr)+sizeof(SAVED_dataValid)) ; eaddr++ )
    {
      *p++ = EEPROM.read ( eaddr ) ;                        // Move one byte to savdata
      EEPROM_Data_Counter++;
    }
    Serial.printf ( "\n Recovered %d Bytes of devadr from EEPROM", EEPROM_Data_Counter);   
     p = (uint8_t*)& SAVED_nwkKey ;                               // set target pointer
    for ( eaddr = EEPROM_Data_Counter ; eaddr < (sizeof(SAVED_devaddr)+sizeof(SAVED_dataValid)+ sizeof(SAVED_nwkKey)) ; eaddr++ )
    {
      *p++ = EEPROM.read ( eaddr ) ;                        // Move one byte to savdata
      EEPROM_Data_Counter++;
    }
      Serial.printf ( "\n Recovered %d Bytes of nwkKey from EEPROM", EEPROM_Data_Counter);   
     p = (uint8_t*)& SAVED_artKey ;                               // set target pointer
    for ( eaddr = EEPROM_Data_Counter ; eaddr < (sizeof(SAVED_devaddr)+sizeof(SAVED_dataValid)+ sizeof(SAVED_nwkKey)+sizeof(SAVED_artKey)) ; eaddr++ )
    {
      *p++ = EEPROM.read ( eaddr ) ;                        // Move one byte to savdata
      EEPROM_Data_Counter++;
    }
    Serial.printf ( "\n Recovered %d Bytes of artKey from EEPROM", EEPROM_Data_Counter);   
     p = (uint8_t*)& SAVED_seqnoUp ;                               // set target pointer
    for ( eaddr = EEPROM_Data_Counter ; eaddr < (sizeof(SAVED_devaddr)+sizeof(SAVED_dataValid)+ sizeof(SAVED_nwkKey)+sizeof(SAVED_artKey)+sizeof(SAVED_seqnoUp)) ; eaddr++ )
    {
      *p++ = EEPROM.read ( eaddr ) ;                        // Move one byte to savdata
      EEPROM_Data_Counter++;
    }
    Serial.printf ( "\n Recovered %d Bytes of SeqnoUp from EEPROM", EEPROM_Data_Counter);    
    SAVED_seqnoUp += 50 ;                                // Counter may be not up-to-date
    Serial.println ( "Recovered Keys from EEPROM are:\n" ) ; 
    /*//Only for Debugging not needed
    memdmp ( "devaddr is:",
             SAVED_devaddr, 4 ) ;
    memdmp ( "appsKey is:",
             SAVED_artKey, 16 ) ;
    memdmp ( "nwksKey is:",
             SAVED_nwkKey, 16 ) ;
    */
  }

  //check ob jetzt nach EEPROM Lesen die Daten gültig
  if ( SAVED_dataValid == DATAVALID )                     // DATA in RTC or EEPROM memory valid?
  {
    Serial.printf ( "Valid data in NVS\n" ) ;               // Yes, show
    /*memdmp ( "devaddr is:",
             SAVED_devaddr, 4 ) ;
    memdmp ( "nwksKey is:",
             SAVED_nwkKey, 16 ) ;
    memdmp ( "appsKey is:",
             SAVED_artKey, 16 ) ;*/
    Serial.printf ( "seqnr is %d\n", SAVED_seqnoUp ) ;
    memcpy ( (uint8_t*)&DEVADDR,
             SAVED_devaddr, sizeof(DEVADDR) ) ;          // LoraWAN DEVADDR, end node device address
    memcpy ( NWKSKEY,
             SAVED_nwkKey,  sizeof(NWKSKEY) ) ;          // LoRaWAN NwkSKey, network session key.
    memcpy ( APPSKEY,
             SAVED_artKey,  sizeof(APPSKEY) ) ;          // LoRaWAN AppSKey, application session key.
    OTAA = false ;                                         // Do not use OTAA
  }
  else
  {
    Serial.printf ( "No saved data, using OTAA\n" ) ;
  }
}

void begin_sleep(){
  //esp_sleep_enable_timer_wakeup(UpdateInterval);
  esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);
  Serial.println(F("Starting deep-sleep period..."));
  esp_deep_sleep_start();         // Sleep for e.g. 30 minutes
}

void setup() {
    WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0); //disable brownout detector
    Serial.begin(115200);
    delay(2000);
    
    
    EEPROM.begin ( 514 ) ; //alt 512 +1 für Bildnummer  -> komisch mit 513 zickt EEPROM Funktion. Bild Nummer ist immer 0; deshalb 1 erhöht
    Serial.println(F("Starting"));
    Serial.print("Anzahl Resets: ");Serial.println(SAVED_boot_counter);
    pinMode(2, OUTPUT);//um SD karte besser zu lesen
    digitalWrite(2, HIGH);//https://github.com/espressif/esp-idf/issues/2025

    //Wifi Scan Location - Test
    scanAndSort();
    
          retrieveKeys(); //Stellt Lora Keys wieder her aus RTC oder EEPROM
          INIT_DATA_2_RTC((int)temp, &Temp_Werte);
          INIT_DATA_2_RTC((int)(pressure), &Druck_Werte); //machs mit uint16_t
          INIT_DATA_2_RTC((int)humidity, &Feuchte_Werte);
          INIT_DATA_2_RTC((int)ccs811_CO2, &CO2_Werte);
          INIT_DATA_2_RTC((int)ccs811_TVOC, &TVOC_Werte);
                   
          setup_air_sensor();
          setup_lora();  //
    
}

void init_wifi(void){
  //Init Wifi:
  // Set WiFi to station mode and disconnect from an AP if it was Previously connected
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);
   WiFi.begin(ssid, password);
    int wifi_retries = 0;
    while (WiFi.status() != WL_CONNECTED) {
        wifi_retries++;
        delay(200);
        Serial.print(".");
        if (wifi_retries > 35) {
          WiFi.mode(WIFI_STA); // WLAN auf Client setzen 
          WiFi.begin(ssid, password);
          delay(1500);
          if(WiFi.status() != WL_CONNECTED){
              Serial.println("WLAN geht gar nicht :-( ");
          }
          else{
            Serial.println("2. Anlauf WLAN geht :-) ");
          }
       }
        
    }
    Serial.println("");
    Serial.println("WiFi connected");
}

void loop() {
    os_runloop_once();
    
    if (verbunden_indicator == 4){
      verbunden_indicator = 2;
       #ifdef BMP280_CONNECTED
            Serial.print(" next_time:");Serial.println(next_time);
            BMP_Test();
          #endif
          #ifdef BME280_CONNECTED
            BME_Test();
       #endif
       #ifdef CCS811_CONNECTED
          CCS811_Test();
       #endif
       #ifdef geraet3
         //do not use display
       #else
          //Status_Info(status_symbol_x, status_symbol_y, false); //Zeigt Battery Status an
       #endif
       #ifdef geraet4
         //do not use display
       #else
          //Status_Info(status_symbol_x, status_symbol_y, false); //Zeigt Battery Status an
       #endif
       #ifdef geraet6
        long now = millis();
        if (now - lastMsg > updaterateWIFI) {
          lastMsg = now;
          scanAndSort();
        }
       #else
       #endif
       
       //display.powerOff(); //Gegen Fading des Displays bei Sleep
       //begin_sleep();//Deep Sleep Start ESP32
    }
}

//Temperaturmessungen

#ifdef BME280_CONNECTED
    int BME_Start(void){
      bool status;
        Wire.begin(sda, scl);
        status = bme.begin(BME_ADRESS);   
        Serial.print("BME check: "); Serial.print(status);
        if (!status) {
            Serial.println("No BME280 I2C sensor, check wiring!");
            return 0;
        }
        else{
          Serial.println("BME gefunden");
          return 1;
        }
    }
    
    void BME_Test(void){
         
          Serial.print("BME280 I2C: ");
          Serial.print("Temp = ");
          Serial.print(bme.readTemperature());
          Serial.print(" *C ");
      
          Serial.print("Druck = ");
          Serial.print(bme.readPressure() / 100.0F);
          Serial.print(" hPa ");
    
          Serial.print("Feuchte = ");
          Serial.print(bme.readHumidity());
          Serial.print(" % \n");
          
          temp = bme.readTemperature();
          pressure = bme.readPressure()/ 100.0F;
          humidity = bme.readHumidity();
          SAVE_DATA_2_RTC((int)temp, &Temp_Werte);
          SAVE_DATA_2_RTC((int)humidity, &Feuchte_Werte);
          //SAVE_DATA_2_RTC((int)(pressure-870.0), &Druck_Werte);//Alte Version mit Uint8_t 
          SAVE_DATA_2_RTC((int)(pressure), &Druck_Werte);
        
    }
#endif

#ifdef BMP280_CONNECTED
    int BMP_Start(void){
      bool status;
        Wire.begin(sda, scl);
        
          status = bmp.begin(BMP_ADRESS,0x58); //alternative BMP Adress
          //status = bmp.begin();  
        
        
        Serial.print("BMP check: "); Serial.print(status);
        if (!status) {
            Serial.println("No BMP280 I2C sensor, check wiring!");
            return 0;
        }
        else{
          Serial.println(" BMP280 gefunden");
          /* Default settings from datasheet. */
          bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,     /* Operating Mode. */
                      Adafruit_BMP280::SAMPLING_X2,     /* Temp. oversampling */
                      Adafruit_BMP280::SAMPLING_X16,    /* Pressure oversampling */
                      Adafruit_BMP280::FILTER_X16,      /* Filtering. */
                      Adafruit_BMP280::STANDBY_MS_500); /* Standby time. */
          return 1;
        }
    }
    
    void BMP_Test(void){
                           
          temp = bmp.readTemperature();
          pressure = bmp.readPressure()/ 100.0F;
          //SAVE_DATA_2_RTC((int)temp, Temp_Werte);Serial.print("Temp:");Serial.println((int)temp);
          //SAVE_DATA_2_RTC((int)pressure, Druck_Werte);Serial.print("Druck:");Serial.println((int)pressure);
          SAVE_DATA_2_RTC((int)temp, &Temp_Werte);
          //SAVE_DATA_2_RTC((int)(pressure-870.0), &Druck_Werte); //870 = minimal jemals gemessener Druck-Alte Version
          SAVE_DATA_2_RTC((int)(pressure), &Druck_Werte);
    }
#endif 

#ifdef CCS811_CONNECTED
  void CCS811_Test(void){
      if (myCCS811.dataAvailable()){
          //Calling this function updates the global tVOC and eCO2 variables
          myCCS811.readAlgorithmResults();
          ccs811_CO2 = float(myCCS811.getCO2());
          ccs811_TVOC = float(myCCS811.getTVOC());
          
          Serial.print("CCS811 I2C: CO2 = "); Serial.print(ccs811_CO2); Serial.print(" ppm");
          Serial.print(" TVOC: "); Serial.print(ccs811_TVOC); Serial.println(" ppb");
          //Serial.print(myCCS811.getCO2());
          //Serial.print(myCCS811.getTVOC());
          
          #ifdef BME280_CONNECTED //for Calibration of Temp and humidity
            float BMEtempC = bme.readTemperature();
            float BMEhumid = bme.readHumidity();
            //Recalibration of CCS822 with BME280 Data
            myCCS811.setEnvironmentalData(BMEhumid, BMEtempC);
            Serial.print("Set CCS811 new values (deg C, %): ");
            Serial.print(BMEtempC); Serial.print(",");
            Serial.println(BMEhumid); Serial.println();
          #endif    
      }
  }
#endif

void kurzschlaf(void){
  Serial.println("Schlafe in 5 sec...ztztztzt");
  delay(5000);
  //esp_sleep_enable_ext0_wakeup(GPIO_NUM_13, 1);
  #define uS_TO_S_FACTOR 1000000 /* Conversion factor for micro seconds to seconds */ 
  #define TIME_TO_SLEEP 50 /* Time ESP32 will go to sleep (in seconds) */
  esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);
  
  Serial.println("Going to sleep now");
  delay(1000);
  esp_deep_sleep_start();
  delay(1000);
}

void rainbow(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256; j++) {
    for(i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel((i+j) & 255));
    }
    strip.show();
    delay(wait);
  }
}
// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}


/* Scan available networks and sort them in order to their signal strength. */
void scanAndSort() {
  WiFi.softAPdisconnect();
  WiFi.disconnect();
  WiFi.mode(WIFI_STA);
  delay(WIFI_DELAY);
  memset(ssid, 0, MAX_SSID_LEN);
  int n = WiFi.scanNetworks();
  Serial.println("Scan done!");
  if (n == 0) {
    Serial.println("No networks found!");
  } else {
    Serial.print(n);
    Serial.println(" networks found.");
    int indices[n];
    for (int i = 0; i < n; i++) {
      indices[i] = i;
    }
    for (int i = 0; i < n; i++) {
      for (int j = i + 1; j < n; j++) {
        if (WiFi.RSSI(indices[j]) > WiFi.RSSI(indices[i])) {
          std::swap(indices[i], indices[j]);
        }
      }
    }
    
    for (int i = 0; i < n; ++i) {
      Serial.print(WiFi.SSID(indices[i]));
      Serial.print(" ");
      Serial.print(WiFi.RSSI(indices[i]));
      Serial.print(" ");
      Serial.print(WiFi.encryptionType(indices[i]));
      Serial.print(" ");
      Serial.print(WiFi.BSSIDstr(indices[i]));
      Serial.println();
      
      if (i < 4 && n > 2){ //nur die stäksten 3 Wifis , nur sortieren ablegen wenn min 3 Wifis vorhanden
       
       Found_Wifi[i].mac = WiFi.BSSIDstr(indices[i]); 
       Found_Wifi[i].EmpfangsPegel = WiFi.encryptionType(indices[i]);
       Found_Wifi[i].Encrypted = 0;
       Found_Wifi[i].mac.replace(":", "");
       Serial.print("Länge: "); Serial.print(Found_Wifi[i].mac.length());Serial.print("MAC: "); Serial.println(Found_Wifi[i].mac);
       /*Serial.print("0:");  Serial.println(WiFi.BSSID(indices[i])[0]);
       Serial.print("1:");  Serial.println(WiFi.BSSID(indices[i])[1]);
       Serial.print("2:");  Serial.println(WiFi.BSSID(indices[i])[2]);
       Serial.print("3:");  Serial.println(WiFi.BSSID(indices[i])[3]);
       Serial.print("4:");  Serial.println(WiFi.BSSID(indices[i])[4]);
       Serial.print("5:");  Serial.println(WiFi.BSSID(indices[i])[5]);*/
       Found_Wifi[i].mac0 = WiFi.BSSID(indices[i])[0];
       Found_Wifi[i].mac1 = WiFi.BSSID(indices[i])[1];
       Found_Wifi[i].mac2 = WiFi.BSSID(indices[i])[2];
       Found_Wifi[i].mac3 = WiFi.BSSID(indices[i])[3];
       Found_Wifi[i].mac4 = WiFi.BSSID(indices[i])[4];
       Found_Wifi[i].mac5 = WiFi.BSSID(indices[i])[5];
       Found_Wifi[i].EmpfangsPegel = WiFi.RSSI(indices[i]);
      }
      /*if(WiFi.encryptionType(indices[i]) == WIFI_AUTH_OPEN) {
        Serial.println("Found non-encrypted network. Store it and exit to connect.");
        memset(ssid, 0, MAX_SSID_LEN);
        strncpy(ssid, WiFi.SSID(indices[i]).c_str(), MAX_SSID_LEN);
        break;
      }*/
    }
  }
}
