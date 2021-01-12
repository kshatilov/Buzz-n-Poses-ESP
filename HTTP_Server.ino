char resp[256] = "123";

// prepare message for ESP HTTP server
/*
void serialise_message() {
	if (NULL == stmErrorResponse) {
		return;
	}

	String res = "";

	for (int i = 0; i < 5; ++i) {
		res += (String)stmErrorResponse[i];
		res += " ";
	}

	res += (String)pressure_1 + " ";
	res += (String)pressure_2 + " ";
	res += (String)pressure_3 + " ";

	Serial.println(res);
	res.toCharArray(resp, res.length() + 1);

	
	DynamicJsonDocument doc(1024);
	for (int i = 0; i < 5; ++i) {
		doc["error"][i] = stmErrorResponce[i];
	}
	doc["pressure_1"] = pressure_1;
	doc["pressure_2"] = pressure_2;
	doc["pressure_3"] = pressure_3;

	serializeJson(doc, resp);


}
*/

esp_err_t get_handler(httpd_req_t *req)
{

	//serialise_message();
	
    httpd_resp_send(req, resp, -1);
    return ESP_OK;
}

/* URI handler structure for GET /uri */
httpd_uri_t uri_get = {
    .uri      = "/",
    .method   = HTTP_GET,
    .handler  = get_handler,
    .user_ctx = NULL
};

/* Function for starting the webserver */
httpd_handle_t start_webserver()
{
    /* Generate default configuration */
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();
	config.server_port = 5000;

    /* Empty handle to esp_http_server */
    httpd_handle_t server = NULL;

    /* Start the httpd server */
    if (httpd_start(&server, &config) == ESP_OK) {
        /* Register URI handlers */
        httpd_register_uri_handler(server, &uri_get);
    }
    /* If server failed to start, handle will be NULL */
    return server;
}
