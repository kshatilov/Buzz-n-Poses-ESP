char resp[512] = "123";

// prepare message for ESP HTTP server

void serialise_message() {

	String res = "";
	int i = N_MESSAGES - 1;
	memcpy(errorData, errorDataBuffer + i * DATA_SIZE, DATA_SIZE);
	for (int j = 0; j < N_DATA; j++) {
		res += (String)(errorData[j]);
		res += " ";
	}
	res += ": ";
	res += (String)pressure_1 + " ";
	res += (String)pressure_2 + " ";
	res += (String)pressure_3 + "\n";
	
	res.toCharArray(resp, res.length() + 1);
}

esp_err_t get_handler(httpd_req_t *req)
{

	serialise_message();
	
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
