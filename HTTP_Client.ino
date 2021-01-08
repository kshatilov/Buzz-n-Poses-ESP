esp_http_client_handle_t client;

void init_comms() {
	WiFi.begin(SSID, PASS);
	Serial.print("Wait for WiFi...");
	while (WiFi.status()!= WL_CONNECTED) {
		Serial.print(".");
		delay(500);
	}
	Serial.println("WiFi connected");
	Serial.println("IP address: ");
	Serial.println(WiFi.localIP());
  
	// ESP HTTP
	esp_http_client_config_t config = {
		.url = CONN,
	};
	client = esp_http_client_init(&config);
	esp_http_client_set_method(client, HTTP_METHOD_GET);
}

String lastCmd = "";
String currCmd = "";
String lastSentCmd = "";

float finalCmD[5];
int intCmd[5];

void sendCmd2STM(String cmd) {
	if (lastSentCmd == cmd) {
		// don't send the same command over again 
		return;	
	}
	Serial.println("cmd: " + cmd);
	getIntCmd(cmd);

	finalCmD[0] = intCmd[4] * getFingerFBPos(0);
	finalCmD[1] = intCmd[0] * getFingerFBPos(1);
	finalCmD[2] = intCmd[2] * getFingerFBPos(2);
	finalCmD[3] = intCmd[1] * getFingerFBPos(3);
	finalCmD[4] = intCmd[3] * getFingerFBPos(4);

	uartSendBuffer(finalCmD, DATA_SIZE, FRAMEID_CMD_POS);
	lastSentCmd = cmd;
}

void http_get() {
	
	esp_err_t err = esp_http_client_perform(client); 
	if (err < 0) {
		Serial.println("Server Unavailable");
		return;
	}
	int read_len = 0;
	char buf[5] = {0};
	read_len = esp_http_client_read(client, buf, 5);
	String payload(buf);
	
	lastCmd = currCmd;
	currCmd = payload.substring(0, 5);		

	//if (lastCmd == currCmd) {
	sendCmd2STM(currCmd);
	//} 
}

