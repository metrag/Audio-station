#include <WiFi.h>

//настройки сети
const char* ssid = "smartpark_service";
const char* password =  "smartpark_2021";

const uint16_t port = 8686;
const char * host = "192.168.88.244";

//пины переферии
#define but 4
#define mic 35

//мгновенное значение амплитуды
volatile int val = 0;

//буфферизация данных
#define SIZE 500
int arr1 [SIZE] = {};
int arr2 [SIZE] = {};
int* ptr_inp = NULL;
int* ptr_sent = NULL;
int* ptr_t = NULL;
bool isSent = false;
int i = 0;

//период дискретизации
#define TIME 125 //мкС

//настройка прерывания
hw_timer_t * timer = NULL;
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;

//обработчик прерывания
void IRAM_ATTR onTimer() {
  portENTER_CRITICAL_ISR(&timerMux);

  //измеряем и помещаем в массив буфер
  val = analogRead(mic);
  ptr_inp[i] = val;

  //если буфер заполнен, оповещаем
  if (i == SIZE - 1) {
    isSent = true;
    i = 0;
  }

  portEXIT_CRITICAL_ISR(&timerMux);
}

//обработка кнопки
bool c_tick = false;
bool l_tick = false;

bool work = true;

//обработка границ пакета
bool isSt = true;
bool isEnd = false;

void setup()
{
  Serial.begin(115200);

  //подключение к сети
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  //настройка пинов переферии
  pinMode(but, INPUT_PULLUP);
  pinMode(mic, INPUT);

  //изначальный расклад массивов
  ptr_inp = arr1;
  ptr_sent = arr2;

  //натсрока таймера
  timer = timerBegin(0, 80, true);
  timerAttachInterrupt(timer, &onTimer, true);
  timerAlarmWrite(timer, TIME, true);
  timerAlarmEnable(timer);
}

//обработчик нажатия кнопки
void check() {
  c_tick = !digitalRead(but);
  if (c_tick != l_tick) {
    l_tick = c_tick;
    work = !work;
  }
}

void loop()
{
  //создвём пакет для отправки
  WiFiClient client;
  client.connect(host, port);

  //если кнопка нажата - начало пакета
  check();
  if (work) {
    //начало пакета
    if (isSt) {
      isSt = false;
      isEnd = true;
      client.print('#');
      client.stop();
    }

    //если буфер переполнен - отправляем пакет
    if (isSent) {
      //меняем указатели
      ptr_t = ptr_sent;
      ptr_sent = ptr_inp;
      ptr_inp = ptr_t;

      for (int j = 0; j < SIZE - 1; j++) {
        client.print(ptr_sent[j]);
        client.stop();
      }
      //заканчиваем отправку
      isSent = false;
    }
  }

  //если кнопка отжата - конец пакета
  if (!work && isEnd) {
    isEnd = false;
    isSt = true;
    client.print('#');
    client.stop();
  }
}
