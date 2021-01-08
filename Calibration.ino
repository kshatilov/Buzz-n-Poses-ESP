#define DATA_SIZE			  5 * sizeof(float) 
#define FRAMEID_CMD_POS       (uint16_t)  0x0003

float* calData = (float[]){0., 0., 0., 0., 0.};

float calibration_step[] = {0.002, 0.002, 0.002, 0.002, 0.002}; 

void calibrateFinger(int index, int dir, float steps) {
  if (index < 0 || index > 4) {
     Serial.println("Only five fingers [0 .. 4], cmon"); 
     return;
  } 
  
  calData[index] += dir * calibration_step[index] * steps;
  
  Serial.print("Sending ");
  Serial.print("[");
  for (int i = 0; i < 5; i++){
    Serial.print(calData[i], 4);
    if (i != 4) {
      Serial.print(", ");
    }
  }
  Serial.print("]");  
  Serial.println(" to STM");
  
  uartSendBuffer(calData, DATA_SIZE, FRAMEID_CMD_POS);
}


//float step_count[] = {6, 6., 7., 7.5, 7.};
float step_count[] = {7, 7., 6., 8.5, 8.};
//float step_count[] = {3.0, 3.5, 3.5, 5.5, 5.5};

/*
* Get a position command for a finger to fully bend
*/
float getFingerFBPos(int i) {
	return calibration_step[i] * step_count[i];
}

void calibrate() {
  Serial.println("Please enter command to tune initial finger position");

  String cmdExit = "done";
  String cmdBend = "b";
  String cmdFBend = "fb";
  String cmdFRelax = "fr";
  String cmdRelax = "r";
  String cmdZeros = "zz";
  String cmdRespect = "ff";

  while (true) {
    Serial.print("~/");  
    while (Serial.available() == 0) {}  
    String cmd = Serial.readString();  
    Serial.print(cmd);
    cmd.trim();
    if (cmdExit.equalsIgnoreCase(cmd)) {
      Serial.println("Exiting calibation ...");  
      break;
    } 
    
    if (cmdBend.equalsIgnoreCase(String(cmd[0])) ) {
      Serial.println("Bending finger no." + String(cmd[1]) + " ...");
      calibrateFinger(((String)cmd[1]).toInt(), 1, 1);
      
    } else if (cmdRelax.equalsIgnoreCase(String(cmd[0]))) {
      Serial.println("Relaxing finger no." + String(cmd[1]) + " ...");
      calibrateFinger(((String)cmd[1]).toInt(), -1, 1);
      
    } else if (cmdFRelax.equalsIgnoreCase(String(cmd[0]) + String(cmd[1]))) {
      Serial.println("Fully relaxing finger no." + String(cmd[2]) + " ...");
      int index = ((String)cmd[2]).toInt();
      calibrateFinger(index, -1, step_count[index]);
      
    } else if (cmdFBend.equalsIgnoreCase(String(cmd[0]) + String(cmd[1]))) {
      Serial.println("Fully bending finger no." + String(cmd[2]) + " ...");
      int index = ((String)cmd[2]).toInt();
      calibrateFinger(index, 1, step_count[index]);
      
    } else if (cmdZeros.equalsIgnoreCase(String(cmd[0]) + String(cmd[1]))) {
      Serial.println("Reseting to zero ...");
      float zeros[] = {0., 0., 0., 0., 0.};
      for (int i = 0; i < 5; ++i) {
        calData[i] = 0.;
      }
      uartSendBuffer(zeros, DATA_SIZE, FRAMEID_CMD_POS);
      
    } else if (cmdRespect.equalsIgnoreCase(String(cmd[0]) + String(cmd[1]))) {
      Serial.println("Sending our respects ...");
      float respect[] = {0., 0., 0., 0., 0.};
      for (int i = 0; i < 5; ++i) {
        if(i != 2 && i != 1) {
          respect[i] = calibration_step[i] * step_count[i];
        }
      }
      uartSendBuffer(respect, DATA_SIZE, FRAMEID_CMD_POS);
      
    } else {
      Serial.println("Unknown command");  
      continue;
    }
  }
}

