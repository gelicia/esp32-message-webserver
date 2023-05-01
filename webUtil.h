void handleRoot() {
  webServer.send(200, "text/plain", "connected!!");
}

void handleNotFound() {
  webServer.send(404, "text/plain", "Not found");
}

void handleSetMetric() {
  jsonDocument.clear();
  if (webServer.hasArg("plain") == false) {}
  String body = webServer.arg("plain");
  deserializeJson(jsonDocument, body);
  String message = jsonDocument["message"];
  int red_value = jsonDocument["red"];
  int green_value = jsonDocument["green"];
  int blue_value = jsonDocument["blue"];

  displayString = message;
  displayColor = pixels.Color(red_value, green_value, blue_value);
  webServer.send(200, "application/json", "{\"metric\": \"" + message + "\"}");
}

void setupWeb() {
  webServer.on("/", HTTP_GET, handleRoot);
  webServer.on("/setMetric", HTTP_POST, handleSetMetric);
  webServer.onNotFound(handleNotFound);
  webServer.begin();
  Serial.println ( "HTTP server started" );
}

void handleWeb() {
  static bool webServerStarted = false;
  // check for connection
  if ( WiFi.status() == WL_CONNECTED ) {
    if (!webServerStarted) {
      Serial.println();
      Serial.println("WiFi connected");
      Serial.print("IP address: ");
      Serial.println(WiFi.localIP());
      webServerStarted = true;
      setupWeb();
    }
    webServer.handleClient();
  } else {
   WiFi.reconnect();
  }
}