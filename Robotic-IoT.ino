
/*
 * Algobel Technologies & Innovations ( Algobel TI)
 * Aurth: Niwantha 
 * Email: niwantha@algobel.com
 * Free to use 
 * Thanks for the community providing free Libraries 
 * APP developed using MIT Inventor 2
 * This is to control your servo and motors with suitable hardware interfaces 
 * Never connect directly to the NodeMcu or any ESP wifi module 
 * Good Luck
 */



#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

const char* ssid = "TP";
const char* password = "12345678";

ESP8266WebServer server(80);
void handleRoot();
const int fwd = 16;
const int bk = 4;
int t = 0;
void handleRoot() {
  t++;
  if (t == 1) {
    digitalWrite(fwd, 0);
    digitalWrite(bk, 0);
  }
  String page = "<h1>IoT Based Robotic Control</h1>";
  page += "<p>(FWD-Function) <a href=\"pin=ON1\"><button>FWD</button></a>&nbsp;<a href=\"pin=OFF1\"><button>Stop-FWD</button></a></p>";
  page += "<p>(BACK-Function) <a href=\"pin=ON2\"><button>BK</button></a>&nbsp;<a href=\"pin=OFF2\"><button>Stop-BK</button></a></p>";
  server.send(200, "text/html", page);
}

void handleNotFound() {
  String message = "File Not Found\n\n";
  server.send(404, "text/plain", message);
}

void setup(void) {
  pinMode(fwd, OUTPUT);
  pinMode(bk, OUTPUT);
  digitalWrite(fwd, 0);
  digitalWrite(bk, 0);
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    digitalWrite(fwd, 0);
    digitalWrite(bk, 0);
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  if (MDNS.begin("algobel-robotic", WiFi.localIP())) {
    Serial.println("MDNS responder started");
  }

  server.on("/", handleRoot);

  server.on("/pin=ON1", []() {
    digitalWrite(fwd, HIGH);
    digitalWrite(bk, LOW);
    Serial.println("Move Foward");
    String page = "<p>(FWD-Function) <a href=\"pin=ON1\"><button><font color=\"RED\">FWD</button></font></a>&nbsp;<a href=\"pin=OFF1\"><button><font color=\"GREEN\">Stop-FWD</button></font></a></p>";
    page += "<p>(BACK-Function) <a href=\"pin=ON2\"><button><font color=\"GREEN\">BK</button></font></a>&nbsp;<a href=\"pin=OFF2\"><button><font color=\"GREEN\">Stop-BK</button></font></a></p>";
    server.send(200, "text/html", page);
  });

  server.on("/pin=OFF1", []() {
    digitalWrite(fwd, LOW);
    Serial.println("Stop Move Foward");
    String page = "<p>(FWD-Function) <a href=\"pin=ON1\"><button><font color=\"GREEN\">FWD</button></font></a>&nbsp;<a href=\"pin=OFF1\"><button><font color=\"RED\">Stop-FWD</button></font></a></p>";
    page += "<p>(BACK-Function) <a href=\"pin=ON2\"><button><font color=\"GREEN\">BK</button></font></a>&nbsp;<a href=\"pin=OFF2\"><button><font color=\"GREEN\">Stop-BK</button></font></a></p>";
    server.send(200, "text/html", page);
  });

  server.on("/pin=ON2", []() {
    digitalWrite(bk, HIGH);
    digitalWrite(fwd, LOW);
    Serial.println("Move Back");
    String page = "<p>(FWD-Function) <a href=\"pin=ON1\"><button><font color=\"GREEN\">FWD</button></font></a>&nbsp;<a href=\"pin=OFF1\"><button><font color=\"GREEN\">Stop-FWD</button></font></a></p>";
    page += "<p>(BACK-Function) <a href=\"pin=ON2\"><button><font color=\"RED\">BK</button></font></a>&nbsp;<a href=\"pin=OFF2\"><button><font color=\"GREEN\">Stop-BK</button></font></a></p>";
    server.send(200, "text/html", page);
  });
  server.on("/pin=OFF2", []() {
    digitalWrite(bk, LOW);
    Serial.println("Stop Move Back");
    String page = "<p>(FWD-Function) <a href=\"pin=ON1\"><button><font color=\"GREEN\">FWD</button></font></a>&nbsp;<a href=\"pin=OFF1\"><button><font color=\"GREEN\">Stop-FWD</button></font></a></p>";
    page += "<p>(BACK-Function) <a href=\"pin=ON2\"><button><font color=\"GREEN\">BK</button></font></a>&nbsp;<a href=\"pin=OFF2\"><button><font color=\"RED\">Stop-BK</button></font></a></p>";
    server.send(200, "text/html", page);
  });


  server.onNotFound(handleNotFound);
  server.begin();
  Serial.println("HTTP server started");
  Serial.setDebugOutput(true);
  WiFi.printDiag(Serial);
}

void loop(void) {
  server.handleClient();

}
