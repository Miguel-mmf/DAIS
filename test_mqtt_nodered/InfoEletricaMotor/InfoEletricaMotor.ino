#define MQTTCLIENT_QOS2 1
#define PI 3.14159265

#include <SPI.h>
#include <Ethernet.h>
#include <EthernetStack.h>
#include <Countdown.h>
#include <MQTTClient.h>
#include <math.h>
  
//double t = 0;
int resistencia = 100;
char printbuf[100];
int arrivedcount = 0;

void messageArrived(MQTT::MessageData& md)
{
  MQTT::Message &message = md.message;
  
  sprintf(printbuf, "Message %d arrived: qos %d, retained %d, dup %d, packetid %d\n", 
		++arrivedcount, message.qos, message.retained, message.dup, message.id);
  Serial.print(printbuf);
  sprintf(printbuf, "Payload %s\n", (char*)message.payload);
  Serial.print(printbuf);
}


EthernetStack ipstack;
MQTT::Client<EthernetStack, Countdown> client = MQTT::Client<EthernetStack, Countdown>(ipstack);

byte mac[] = { 0x00, 0x11, 0x22, 0x33, 0x44, 0x55 };  // replace with your device's MAC
const char* topic_correnteA = "mqtt/correnteA";
const char* topic_tensaoA = "mqtt/tensaoA";
/* const char* topic_correnteB = "mqtt/correnteB";
const char* topic_correnteC = "mqtt/correnteC";
const char* topic_tensaoB = "mqtt/tensaoB";
const char* topic_tensaoC = "mqtt/tensaoC";
*/

int valor = 0;
float Va = 0.0;
float Ia = 0.0;
/*
float Vb = 0;
float Vc = 0;
float Ib = 0;
float Ic = 0;
*/

void connect()
{
  char hostname[] = "broker.emqx.io";
  int port = 1883;
  sprintf(printbuf, "Connecting to %s:%d\n", hostname, port);
  Serial.print(printbuf);
  int rc = ipstack.connect(hostname, port);
  if (rc != 1)
  {
    sprintf(printbuf, "rc from TCP connect is %d\n", rc);
    Serial.print(printbuf);
  }
 
  Serial.println("MQTT connecting");
  MQTTPacket_connectData data = MQTTPacket_connectData_initializer;       
  data.MQTTVersion = 3;
  data.clientID.cstring = (char*)"energia-sample";
  rc = client.connect(data);
  if (rc != 0)
  {
    sprintf(printbuf, "rc from MQTT connect is %d\n", rc);
    Serial.print(printbuf);
  }
  Serial.println("MQTT connected");

  rc = client.subscribe(topic_tensaoA, MQTT::QOS2, messageArrived);   
  if (rc != 0)
  {
    sprintf(printbuf, "rc from MQTT subscribe is %d\n", rc);
    Serial.print(printbuf);
  }
  rc = client.subscribe(topic_correnteA, MQTT::QOS2, messageArrived);   
  if (rc != 0)
  {
    sprintf(printbuf, "rc from MQTT subscribe is %d\n", rc);
    Serial.print(printbuf);
  }
  Serial.println("MQTT subscribed");
}


void setup()
{
  Serial.begin(115200);

  Serial.println("Starting Ethernet");
  Ethernet.enableLinkLed();
  Ethernet.enableActivityLed();
  Ethernet.begin(0);

  Serial.println("\nIP Address obtained");
  // We are connected and have an IP address.
  Serial.println(Ethernet.localIP());
  
  // Serial.println("MQTT Hello example");
  connect();
}

void loop()
{
  if (!client.isConnected())
    connect();
  
  MQTT::Message message;
  
  arrivedcount = 0;

  /*
  float t = millis()/1000;
  Va = 380.0*sin(2*PI*60*t); // deveria ser 60Hz
  Serial.print(Va);
  */
  //Ia = Va/resistencia;
  //Ia = 10.0*sin(2*PI*60*t + PI/2); // defasamento de 90°
  //Serial.print(Ia);
  
  // Send and receive QoS 0 message
  char buf[100];
  // sprintf(buf, "%f", Va);
  if (valor < 100)
    valor++;
  else
    valor = -100;
    
  itoa(valor,buf,10);
  
  Serial.println(buf);
  // Serial.print(" --> ");
  
  message.qos = MQTT::QOS0;
  message.retained = false;
  message.dup = false;
  message.payload = (void*)buf;
  // Serial.println(strlen(buf));
  message.payloadlen = strlen(buf)+1;
  
  int rc = client.publish(topic_tensaoA, message);
  while (arrivedcount == 0)
    client.yield(1000);

  /*
  sprintf(buf, Ia);
  Serial.println(buf);
  message.payload = (void*)buf;
  message.payloadlen = strlen(buf)+1;
  rc = client.publish(topic_correnteA, message);
  while (arrivedcount == 0)
    client.yield(1000);

  t = t + 0.00001;
  */
  delay(10);
  //Serial.println(t);
}
