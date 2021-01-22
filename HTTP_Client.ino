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
float parsedCmd[5];

void sendCmd2STM(String cmd) {
	if (lastSentCmd == cmd) {
		// don't send the same command over again 
		return;	
	}
	
	parcCmdFromApp(cmd);
	Serial.println(cmd);

	finalCmD[0] = parsedCmd[4] * getFingerFBPos(0);
	finalCmD[1] = parsedCmd[0] * getFingerFBPos(1);
	finalCmD[2] = parsedCmd[2] * getFingerFBPos(2);
	finalCmD[3] = parsedCmd[1] * getFingerFBPos(3);
	finalCmD[4] = parsedCmd[3] * getFingerFBPos(4);

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
	char buf[64] = {0};
	read_len = esp_http_client_read(client, buf, 64);
	if (read_len < 10) {
		return;
	}
	String payload(buf);
	
	lastCmd = currCmd;
	currCmd = payload.substring(0, 64);		

	//if (lastCmd == currCmd) {
	sendCmd2STM(currCmd);
	//} 
}

