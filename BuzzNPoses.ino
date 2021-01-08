#include <ArduinoJson.hpp>
#include <ArduinoJson.h>
#include <HardwareSerial.h>
#include <WiFi.h>
  
#include "esp_http_client.h"
#include "esp_event.h"
#include <esp_http_server.h>

#define LED					  32U
#define BOOT				  18U
#define RST					  19U

#define FRAMEID_CMD_POS       (uint16_t)  0x0003
#define DATA_SIZE			  5 * sizeof(float) 
#define START_OF_FRAME        (uint8_t) 0xA5

HardwareSerial sparthan(2);           

const char* SSID = "SPARTHAN";
const char* PASS = "66668888";
const char* CONN = "http://192.168.137.243:5000/";

//const char* SSID = "kkb";
//const char* PASS = "2444666668888888";
//const char* CONN = "http://192.168.137.234:5000/";


//const char* SSID = "K&J";
//const char* PASS = "kirillandjia";
//const char* CONN = "http://192.168.40.99:5000/";

const int APP2SMP_DELAY = 100;
const int SMP2APP_DELAY = 50;

void * app2smpThread(void * p)
{
    Serial.println("app2smp thread");
	
	while (1) {
		http_get();
		delay(APP2SMP_DELAY);
	}
    
	return NULL;
}

int pressure_1 = 0;
int pressure_2 = 0;
int pressure_3 = 0;

float* stmErrorResponce = NULL;

void * smp2appThread(void * p)
{
    Serial.println("smp2app thread");
	
	start_webserver();

	while (1) {
		// poll Sensors
		pressure_1 = analogRead(25);
		pressure_2 = analogRead(26);
		pressure_3 = analogRead(27);
		//Serial.println(pressure_1);
		// below 40 is pressed
		
		// poll SMP
		uint8_t EOS = 0xFF;
		uint8_t sof = 0;

		
		sof = sparthan.read();
		if (START_OF_FRAME == sof) {
			stmErrorResponce = uartReceiveBufferFromKirill();
		}
		

		delay(SMP2APP_DELAY);
	}

    return NULL;
}

void setup() {

	//pinMode(BOOT, OUTPUT);
	//pinMode(LED, OUTPUT);
	//pinMode(RST, OUTPUT);

	//digitalWrite(BOOT, LOW);
	//digitalWrite(RST, LOW);
	//delay(300);
	pinMode(RST, INPUT);
	pinMode(25, 0xC0);

	Serial.begin(115200);
	sparthan.begin(500000, SERIAL_8N1, 16, 17);
  
	init_comms();
  
	//calibrate();

	pthread_t app2smp;
	pthread_t smp2app;

    //pthread_create(&app2smp, NULL, app2smpThread, NULL);
    //pthread_create(&smp2app, NULL, smp2appThread, NULL);

}

void loop(){
	int p = analogRead(25);
	delay(500);
	Serial.println(p);
}