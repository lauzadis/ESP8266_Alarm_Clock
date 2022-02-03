#include "config.h" // Where project configurations and secrets are stored

#include <ESP8266WiFi.h> // To connect to a WiFi network
#include <DS3231.h> // For the DateTime object, originally was planning to use this RTC module
#include <NTPClient.h> // To get accurate time from a remote server
#include <WiFiUdp.h>  // Helper for NTPClient

// WiFi
String ssid = WIFI_SSID;
String password = WIFI_PASSWORD;
WiFiServer server(80); // Set web server port number to 80

// Web Host
String header; // Variable to store the HTTP request
unsigned long currentTime = millis(); // Current time
unsigned long previousTime = 0; // Previous time
const long timeoutTime = 2000; // Define timeout time in milliseconds (example: 2000ms = 2s)

// NTP Client
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org");

// Alarm State
// * DateTime objects cannot be modified after creation, so I get the current DateTime and use it to create the default 7AM alarm *
DateTime today = DateTime(timeClient.getEpochTime());
DateTime alarm = DateTime(22, today.month(), today.day(), 7, 0, 0); // defaults to 7 AM, my normal waking time
String alarmState = "on"; // Default to having the alarm enabled

void setup() {
  Serial.begin(115200);
  
  pinMode(5, OUTPUT); // Initialize the buzzer pin as output
  digitalWrite(5, LOW); // Set buzzer to LOW
  
  pinMode(4, INPUT); // Initialize the button pin as input
  
  // Connect to Wi-Fi network with SSID and password
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  // Print local IP address
  Serial.println("\nWiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  timeClient.begin();
  timeClient.setTimeOffset(TIMEZONE*3600); // Offset time from the GMT standard
  server.begin(); // Start web server!
}

void loop(){
  timeClient.update(); // Update the latest time from the NTP server
  WiFiClient client = server.available();   // Listen for incoming clients

  if (client) {                             // If a new client connects,
    Serial.println("New Client.");          // print a message out in the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    currentTime = millis();
    previousTime = currentTime;
    while (client.connected() && currentTime - previousTime <= timeoutTime) { // loop while the client is connected
      currentTime = millis();         
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        header += c;
        if (c == '\n') {                    // if the byte is a newline character
          if (currentLine.length() == 0) { // If the current line is blank, you got two newline characters in a row. That's the end of the client HTTP request, so send a response:
            client.println("HTTP/1.1 200 OK"); // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            client.println("Content-type:text/html"); // and a content-type so the client knows what's coming, then a blank line:
            client.println("Connection: close");
            client.println();
            
            if (header.indexOf("GET /alarm/on") >= 0) { // If the user clicked the alarm's on button
              Serial.println("Alarm enabled");
              alarmState = "on";
            } 
            else if (header.indexOf("GET /alarm/off") >= 0) { // If the user clicked the alarm's off button
              Serial.println("Alarm disabled");
              alarmState = "off";
            }

            else if (header.indexOf("GET /time") >= 0) { // If the user submitted the time input form
              // Strip the data from the GET request
              int index = header.indexOf("GET /time");
              String timeData = header.substring(index + 15, index + 22);
              int hour = timeData.substring(0, 2).toInt();
              int minute = timeData.substring(5,7).toInt();
              
              Serial.println(timeData);
              // Update our alarm DateTime with the user selected time, using the current date.
              // Since we just compare the hours and minutes on each loop, I do not think the day or month matters.
              DateTime temp = DateTime(timeClient.getEpochTime());
              alarm = DateTime(22, temp.month(), temp.day(), hour, minute, 0);
            }
            
            // Display the HTML web page
            // Head
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            client.println("<link rel=\"stylesheet\" href=\"//stackpath.bootstrapcdn.com/bootstrap/4.4.1/css/bootstrap.min.css\">"); // Bootstrap
            client.println("</head>");
            
            // Body
            client.println("<body>");
            client.println("<h1 class=\"text-center mt-3\">Alarm Clock</h1>"); // Title

            // Current Time
            client.print("<h4 class=\"text-center\">"); 
            client.print(timeClient.getFormattedTime());
            client.println("</h4>");

            // Alarm
            client.println("<form action=\"/time\" method=\"GET\">");
            // Time Input
            client.print("<p class=\"text-center\"><input type=\"time\" id=\"time\" name=\"time\" value=\"");

            // Prepend a "0" to the hour and minute if needed, otherwise HTML gets cranky 
            String hour = String(alarm.hour());
            if(hour.length() == 1){
              hour = "0" + hour;
            }
            String minute = String(alarm.minute());
            if(minute.length() == 1){
              minute = "0" + minute;
            }

            // Pre-fill the value of the form with the current alarm time
            client.print(hour);
            client.print(":");
            client.print(minute);
            client.println("\" required>"); // Make the input required, so user cannot submit a null time
            
            // Submit Button
            client.println("<input type=\"submit\" class=\"btn btn-sm btn-success\" value=\"Submit\">");
            client.println("</form>");
            
            // Display current state, and ON/OFF buttons for Alarm  
            client.println("<h6 class=\"text-center\">Alarm State - " + alarmState + "</h6>");
            if (alarmState=="off") {
              client.println("<p class=\"text-center\"><a href=\"/alarm/on\"><button class=\"btn btn-sm btn-danger\">ON</button></a></p>");
            }
            else {
              client.println("<p class=\"text-center\"><a href=\"/alarm/off\"><button class=\"btn btn-success btn-sm\">OFF</button></a></p>");
            }
            client.println("</body></html>");
            client.println(); // The HTTP response ends with another blank line
            break; // Break out of the while loop
            
          } else { // if you got a newline, then clear currentLine
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else,
          currentLine += c;      // add it to the end of the currentLine
        }
      }
    }
    
    header = ""; // Clear the header variable
    client.stop(); // Close the connection
    Serial.println("Client disconnected.");
    Serial.println("");
  }
  // Check if the alarm should be running
  if(alarmState == "on" && timeClient.getHours() == alarm.hour() && alarm.minute() <= timeClient.getMinutes() && timeClient.getMinutes() <= alarm.minute()+ALARM_LENGTH_MINUTES){
    // Play a beautiful tone 
    
    tone(5,1400);
    delay(300);
    noTone(5);
    
    tone(5, 1600);
    delay(300);
    noTone(5);
    
    tone(5, 1800);
    delay(300);
    noTone(5);

    tone(5, 1600);
    delay(300);
    noTone(5);
  }

  // If the button was pressed
  if(digitalRead(4) == HIGH){
    if(alarmState == "off" || timeClient.getHours() != alarm.hour()){ // If the alarm is already off, give a feedback tone
      tone(5, 1400);
      delay(100);
      noTone(5);
      
      tone(5, 1000);
      delay(100);
      noTone(5);
    }
    else { // Turn the alarm off
      alarmState = "off";
    } 
    digitalWrite(4, LOW); // Turn the button off, in case the user is holding it down, it won't spam and break things
    delay(500);
  }
}
