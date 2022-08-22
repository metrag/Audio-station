#include <WiFi.h>

//настройки сети
const char* ssid = "smartpark_service";
const char* password =  "smartpark_2021";

const uint16_t port = 8686;
const char * host = "192.168.88.244";

#define but 4
#define mic 35
volatile int val = 0;

#define SIZE 100
int arr1 [SIZE] = {};
int arr2 [SIZE] = {};
int* ptr = NULL;
int i = 0;

//период дискретизации
#define TIME 125 //мкС
hw_timer_t * timer = NULL;
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;

//обработчик прерывания
void IRAM_ATTR onTimer() {
  portENTER_CRITICAL_ISR(&timerMux);

  val = analogRead(mic);
  
  portEXIT_CRITICAL_ISR(&timerMux);
}

bool c_tick = false;
bool l_tick = false;

bool work = true;
bool isSt = true;
bool isEnd = false;

void setup()
{
  Serial.begin(115200);
  WiFi.begin(ssid, password);

  /* wait until ESP32 connect to WiFi*/
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  pinMode(but, INPUT_PULLUP);
  pinMode(mic, INPUT);

  //натсрока таймера
  timer = timerBegin(0, 80, true);
  timerAttachInterrupt(timer, &onTimer, true);
  timerAlarmWrite(timer, TIME, true);
  timerAlarmEnable(timer);
}

void check() {
  c_tick = !digitalRead(but);
  if (c_tick != l_tick) {
    l_tick = c_tick;
    work = !work;
  }
}

void loop()
{
  WiFiClient client;
  client.connect(host, port);

  check();
  if (work) {
    if (isSt) {
      isSt = false;
      isEnd = true;
      client.print('#');
      client.stop();
    }
    client.print(val);
    client.stop();
  }
  
  if (!work && isEnd) {
    isEnd = false;
    isSt = true;
    client.print('#');
    client.stop();
  }
}
