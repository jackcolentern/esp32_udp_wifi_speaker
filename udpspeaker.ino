#include <WiFi.h>
#include <WiFiUdp.h>

const char * ssid = "SSID";
const char * pwd = "PASSWD";


const int udpPort = 44444;

hw_timer_t * timer = NULL;


uint8_t packetBuffer[81920]; //buffer to hold incoming packet

unsigned long readptr = 0;
unsigned long ptr = 0;
unsigned long oldtime = 0;
unsigned long oldreadudp = 0;
unsigned long changepoint = 0;

long diff = 0;


WiFiUDP udp;

void IRAM_ATTR onTimer() {
  dacWrite(25, packetBuffer[ptr]);
  ptr++;
  diff--;
  if (ptr == changepoint || ptr == 81919) ptr = 0;
}

void setup() {
  delay(1000);
  Serial.begin(115200);
  //Connect to the WiFi network
  WiFi.begin(ssid, pwd);
  Serial.println("");
  WiFi.setSleep(false);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  udp.begin(udpPort);

  delay(1000);


  timer = timerBegin(0, 80, true); // use a prescaler of 80
  timerAttachInterrupt(timer, &onTimer, true);
  timerAlarmWrite(timer, 25, true);
  timerAlarmEnable(timer);

}


void loop() {
  /* Serial.print(",");
    Serial.print(readptr);
    Serial.print(",");
    Serial.print(changepoint);
    Serial.println(",");*/
  uint16_t packetSize = udp.parsePacket();
  if (readptr + packetSize < 81919) {
    udp.read(&packetBuffer[readptr], packetSize);
    readptr += packetSize;
  }
  else {
    changepoint = readptr;
    readptr = 0;
    udp.read(&packetBuffer[readptr], packetSize);
  }
  diff += packetSize;


}
