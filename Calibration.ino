#define DATA_SIZE			  5 * sizeof(float)
#define FRAMEID_CMD_POS       (uint16_t)  0x0003

float calData[5] = {0., 0., 0., 0., 0.};

float calibration_step[] = {0.002, 0.002, 0.002, 0.002, 0.002}; 
float step_count[] = {6., 3., 7., 12., 6.};

const int BUFF_SIZE = 5 * sizeof(float) + 256; // make it big enough to hold your longest command
static char buffer[BUFF_SIZE+1]; // +1 allows space for the null terminator


void calibrateFinger(int index, int dir, float steps) {
  if (index < 0 || index > 4) {
     Serial.println("Only five fingers [0 .. 4], cmon"); 
     return;
  }  
  calData[index] += dir * calibration_step[index] * steps;
}

void moveFinger(int index, float position) {
  
  if (index < 0 || index > 4) {
     Serial.println("Only five fingers [0 .. 4], cmon"); 
     return;
  } 
  if (position > 1 || position < 0) {
     Serial.println("move out of range: [0..1]"); 
     return;}
     
  calData[index] = getFingerFBPos(index) * position; // position should be between 0 and 1
}


void serialPrintPosition() {
  
  printf("Sending [");
  for (int i = 0; i < 5; i++){
    printf("%.3f, ", calData[i]);
  }
  printf("] to STM\n");  
}

void sendPosition() {
  serialPrintPosition();
  uartSendBuffer(calData, DATA_SIZE, FRAMEID_CMD_POS);  
}




/*
* Get a position command for a finger to fully bend
*/
float getFingerFBPos(int i) {
	return calibration_step[i] * step_count[i];
}


void calibrate() {
  printf("Please enter command to tune initial finger position\n");

  String cmdExit = "done";
  String cmdBend = "b";
  String cmdFBend = "fb";
  String cmdFRelax = "fr";
  String cmdRelax = "r";
  String cmdZeros = "zz"; // send all fingers to 0
  String cmdReset = "reset"; // set ESP position array to 0
  String cmdRespect = "ff";
  String cmdGrab = "grab";  
  
  while (true) {
    Serial.print("*** ~/");  
    while (Serial.available() == 0) {
  //    readResponse(); // strings only
    }  
    String cmd = Serial.readString();  
   // Serial.print(cmd);
    cmd.trim();

    
    if (cmdExit.equalsIgnoreCase(cmd)) {
      printf("*** Exiting calibation ...\n");  
      break;
    } 
    
    if (cmdReset.equalsIgnoreCase(cmd)) { // has to go before rXXX commands
      Serial.println("*** Reseting local to zero ...");
      float zeros[] = {0., 0., 0., 0., 0.};
      for (int i = 0; i < 5; ++i) {
       calData[i] = 0.;
      }
    } else if (cmdBend.equalsIgnoreCase(String(cmd[0])) ) {
      calibrateFinger(((String)cmd[1]).toInt(), 1, 1);
      sendPosition();  
      
    } else if (cmdRelax.equalsIgnoreCase(String(cmd[0]))) {
      calibrateFinger(((String)cmd[1]).toInt(), -1, 1);
      sendPosition();  
      
    } else if (cmdFRelax.equalsIgnoreCase(String(cmd[0]) + String(cmd[1]))) {
      int index = ((String)cmd[2]).toInt();
      moveFinger(index, 0.);
      sendPosition();  
      
    } else if (cmdFBend.equalsIgnoreCase(String(cmd[0]) + String(cmd[1]))) {
      int index = ((String)cmd[2]).toInt();
      moveFinger(index, 1.);
      sendPosition();  
      
    } else if (cmdZeros.equalsIgnoreCase(String(cmd[0]) + String(cmd[1]))) {
      for (int i = 0; i < 5; ++i) {
        moveFinger(i, 0.);
      }
      sendPosition();  
  
    } else if (cmdRespect.equalsIgnoreCase(String(cmd[0]) + String(cmd[1]))) {
      printf("*** Sending our respects ...\n");
      for (int i = 0; i < 5; ++i) {
        if(i != 2 && i != 1) {
          moveFinger(i, 1.);     
        }
      sendPosition();  
      }
    } else if (cmdGrab.equalsIgnoreCase(String(cmd))) {
      for (int i = 0; i < 5; ++i) {
          moveFinger(i, 1.);     
        }   
      sendPosition();  
    } else {

      printf("*** Unknown command\n");
 
      continue;
    }
  }
}
