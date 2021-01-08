#define START_OF_FRAME        (uint8_t) 0xA5
#define OFFSET_SOF            (uint8_t) 0
#define OFFSET_DATA_LENGTH    (uint8_t) 1
#define OFFSET_FRAME_SEQ      (uint8_t) 3
#define OFFSET_CRC8           (uint8_t) 4
#define OFFSET_DATA_TYPE      (uint8_t) 5

#define SIZE_FRAMEHEAD        (uint8_t) 5
#define SIZE_FRAMEID          (uint8_t) 2
#define SIZE_FRAMETAIL        (uint8_t) 2
#define FRAMEID_CMD_POS       (uint16_t)  0x0003

#define DATA_SIZE 5 * sizeof(float) 

void uartSendBuffer(const float* data, const uint16_t dataSize, const uint16_t dataType) {
  uint8_t txBuffer[64];
  static uint8_t txCount = 0;
  txCount++;
  memset(txBuffer, 0, sizeof(txBuffer));
  txBuffer[0] = START_OF_FRAME;
  txBuffer[1] = (uint8_t) dataSize >> 8;
  txBuffer[2] = (uint8_t) dataSize;
  txBuffer[OFFSET_FRAME_SEQ] = txCount;
  Append_CRC8_Check_Sum(txBuffer, SIZE_FRAMEHEAD);
  txBuffer[5] = (uint8_t) dataType >> 8;
  txBuffer[6] = (uint8_t) dataType;
  uint16_t currentSize = OFFSET_DATA_TYPE + sizeof(uint16_t); //7
  memcpy(txBuffer + currentSize, data, dataSize);
  currentSize += dataSize;
  Append_CRC16_Check_Sum(txBuffer, currentSize + SIZE_FRAMETAIL);
  currentSize += 2;
  sparthan.write(txBuffer, currentSize);
}

void* uartReceiveBuffer(uint8_t* recBuffer) {
  uint16_t currentSize = 0;
  uint8_t recSOF = recBuffer[currentSize];

  // Finding the SOF
  while(START_OF_FRAME != recSOF) {
    ++ currentSize;
    recSOF = recBuffer[currentSize];
  }
  currentSize += sizeof recSOF;
  uint16_t recDataSize = 0;
  memcpy(&recDataSize, recBuffer + currentSize, sizeof recDataSize);
  currentSize += sizeof recDataSize;
  uint8_t recTxCount = recBuffer[OFFSET_FRAME_SEQ];
  currentSize += sizeof recTxCount;
  uint16_t recBufferSize = SIZE_FRAMEHEAD + SIZE_FRAMEID + SIZE_FRAMETAIL + recDataSize;
  Serial.println(recBufferSize);
  // Checking CRC
  if (Verify_CRC8_Check_Sum(recBuffer, SIZE_FRAMEHEAD) && 
      Verify_CRC16_Check_Sum(recBuffer, recBufferSize)) {
    currentSize +=  sizeof(uint8_t);
    uint16_t recDataType = 0;
    memcpy(&recDataType, recBuffer + currentSize, sizeof(uint16_t)); 
    currentSize +=  sizeof recDataType;
    void * recData;
    recData = (void *)malloc(recDataSize);
    memcpy(recData, recBuffer + currentSize, recDataSize);
    Serial.println("+");     
    return recData;
  }
}

float* uartReceiveBufferFromKirill() {

    /*
	do {
		sof = sparthan.read();           
		if (START_OF_FRAME == sof) {
		break;
		}
      
	} while (1);
	*/

	uint8_t* recBuffer = (uint8_t*)malloc(DATA_SIZE);    
	if (0 != recBuffer) {
		sparthan.readBytes(recBuffer, DATA_SIZE);
	
		float* recData;
		recData = (float*)malloc(DATA_SIZE);
		memcpy(recData, recBuffer, DATA_SIZE);
		
		//Serial.println(array2string(recData, 5));

		return recData;
	}
}