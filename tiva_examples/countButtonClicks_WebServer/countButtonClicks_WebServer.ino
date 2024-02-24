#include "Ethernet.h"
#define button PJ_0

int statusConfig = 0;
int countClicks = 0;
bool buttonStage = false; // se o botão for pressionado, a flag indica que o cliente web deve atualizar
char format[128];

void printEthernetData();


// Criando um servido Web na porta 80
EthernetServer server(80);


void setup()
{
  Serial.begin(115200);    

  pinMode(button, INPUT_PULLUP);
  attachInterrupt(button, funcButton, FALLING);

  Serial.println("Connecting to Ethernet....");  
  IPAddress ip = IPAddress(146,252,242,129);
  IPAddress dns = IPAddress(146,252,242,12);
  IPAddress gw = IPAddress(146,252,242,254);
  IPAddress mask = IPAddress(255,255,255,0);

  Ethernet.begin(0); //  Ethernet.begin(0, ip, dns, gw);

  server.begin();
  printEthernetData();
}


EthernetClient client;


void loop()
{

  if (buttonStage)
  {
    client = server.available();

    if (client)
    {
      Serial.print("new client on port ");
      Serial.println(client.port());
      
      String currentLine = ""; // make a String to hold incoming data from the client
      boolean newConnection = true; // flag for new connections
      unsigned long connectionActiveTimer; // will hold the connection start time

      while (client.connected())
      {
        if (newConnection){
          connectionActiveTimer = millis();
          newConnection = false;
        }
        
        if (!newConnection && connectionActiveTimer + 1000 < millis()){ 
          // if this while loop is still active 1000ms after a web client connected, something is wrong
          break;  // leave the while loop, something bad happened
        }
  
        if (client.available())
        {
          char c = client.read();
          // Serial.print(c);
          
          if (c == '\n')
          {
            if (currentLine.length() == 0) {  
              break;         
            } 
            else {      // if you got a newline, then clear currentLine:
              currentLine = "";
            }
          }  
             
          else if (c != '\r') {
            currentLine += c;
          }
                
          if (currentLine.endsWith("GET /index.html ") || currentLine.endsWith("GET / ")) {
            statusConfig = 0;
            showMainPage();
          }
          
          // Check to see if the client request was "GET /H" or "GET /L":
          if (currentLine.endsWith("GET /LED1_H")) {
            digitalWrite(D1_LED, HIGH);
            showMainPage();
          }         
          
          if (currentLine.endsWith("GET /LED1_L")) {
            digitalWrite(D1_LED, LOW);
            showMainPage();
          }     
          
          if (currentLine.endsWith("GET /LED2_H")) {
            digitalWrite(D2_LED, HIGH);
            showMainPage();
          }       
          
          if (currentLine.endsWith("GET /LED2_L")) {
            digitalWrite(D2_LED, LOW);
            showMainPage();
          }
        }
      }
    }

    // close the connection:
    client.stop();
    // buttonStage = !buttonStage;
  }
}


void funcButton()
{
  countClicks++;
  buttonStage = !buttonStage;
}

void showMainPage()
{
  client.println("HTTP/1.1 200 OK");
  client.println("Content-type:text/html");
  client.println();
  client.println("<html><head><title>Test Energia Ethernet Web Server</title></head><body align=center>");
  client.println("<h1 align=center><font color=\"red\">Web Page para contagem de clicks no botao 1</font></h1>");

  // the content of the HTTP response follows the header:
  // Added: nicer buttons
  client.print("LED 1<button onclick=\"location.href='/LED1_H'\">HIGH</button>");
  client.println(" <button onclick=\"location.href='/LED1_L'\">LOW</button><br>");
  client.print("LED 2 <button onclick=\"location.href='/LED2_H'\">HIGH</button>");
  client.println(" <button onclick=\"location.href='/LED2_L'\">LOW</button><br><br>");

  client.println("<h2 align=center><font color=\"red\">Numero de clicks: </font></h2>");
  
  sprintf(format, "<h3 align=center><font color=\"blue\"> %d. </font></h3>", countClicks);

  client.print(format);
  client.println();
}

void printEthernetData() {
  // print your IP address:
  Serial.println();
  Serial.println("IP Address Information:");  
  IPAddress ip = Ethernet.localIP();
  Serial.print("IP Address:\t");
  Serial.println(ip);

  // print your MAC address:

  IPAddress subnet = Ethernet.subnetMask();
  Serial.print("NetMask:\t");
  Serial.println(subnet);

  // print your gateway address:
  IPAddress gateway = Ethernet.gatewayIP();
  Serial.print("Gateway:\t");
  Serial.println(gateway);

  // print your gateway address:
  IPAddress dns = Ethernet.dnsServerIP();
  Serial.print("DNS:\t\t");
  Serial.println(dns);

}
