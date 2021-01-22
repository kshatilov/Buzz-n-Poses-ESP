#include <HardwareSerial.h>
#include <WiFi.h>
  
#include "esp_http_client.h"
#include "esp_event.h"
#include <esp_http_server.h>

#include <driver/adc.h>


#define LED					  32U
#define BOOT				  18U
#define RST					  19U

#define FRAMEID_CMD_POS       (uint16_t)  0x0003
#define N_MESSAGES            20
#define N_DATA                5
#define DATA_SIZE             N_DATA * sizeof(float) 
#define DATA_BUFFER_SIZE	  N_MESSAGES * DATA_SIZE
#define START_OF_FRAME        (uint8_t) 0xA5
#define END_OF_FRAME          (uint8_t) 0xFF

#define MESSAGE_SIZE           22*sizeof(uint8_t) // raw message with SOF & EOF
HardwareSerial sparthan(2);           


const char* SSID = "SPARTHAN";
const char* PASS = "66668888";
const char* CONN = "http://192.168.137.66:5000/";

const int APP2SMP_DELAY = 30;
const int SMP2APP_DELAY = 5;

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

float errorData[5];
uint8_t errorDataBuffer[DATA_BUFFER_SIZE];

void * smp2appThread(void * p)
{
  
	Serial.printf("smp2app thread init\n");
	  
	start_webserver();
	int count = 0;
	uint8_t sof;
	while (1) {

		// look for message
		while (sparthan.available() >= 2*MESSAGE_SIZE) {
			pressure_1 = analogRead(34);
			pressure_2 = analogRead(35);
			pressure_3 = analogRead(32);
			sof = sparthan.read();
      
			if (sof == START_OF_FRAME) {  
				uint8_t* recBuffer = (uint8_t*)malloc(MESSAGE_SIZE-1);  // we have read the first byte already         
				sparthan.readBytes(recBuffer, MESSAGE_SIZE-1);

				if (recBuffer[MESSAGE_SIZE-2] == END_OF_FRAME) { // found message
					memcpy(errorDataBuffer + count * DATA_SIZE, recBuffer, MESSAGE_SIZE-1);
					count++;
          
					if (count == 20) { // errorsDataBuffer full     
						count = 0;
					}
				}  else {
					//Serial.println("Couldn't find EOF");      
				}  
				free(recBuffer); 
				break;
			} // found SOF
      
			if ( sparthan.available() == MESSAGE_SIZE) { // first half of 2x message doesn't have SOF - break while avail 
				break;   
			}
      
		} // while sparthan
		delay(SMP2APP_DELAY);
	} // while 1
	return NULL;
}

void setup() {
	Serial.begin(115200);
	sparthan.begin(500000, SERIAL_8N1, 16, 17);
  
	calibrate();
	init_comms();

	pthread_t smp2app;
	pthread_create(&smp2app, NULL, smp2appThread, NULL);
	

    pthread_t app2smp;
    pthread_create(&app2smp, NULL, app2smpThread, NULL);

 }

void loop(){
}
