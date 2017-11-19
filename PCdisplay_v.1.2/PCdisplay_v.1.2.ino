/*
  Блок электроники для крутого моддинга вашего ПК, возможности:
  - Вывод основных параметров железа на внешний LCD дисплей
  - Температура: CPU, GPU, материнская плата, самый горячий HDD
  - Уровень загрузки: CPU, GPU, RAM, видеопамять
  - Графики изменения вышеперечисленных параметров по времени
  - Температура с внешних датчиков (DS18B20)
  - Текущий уровень скорости внешних вентиляторов
  - Управление большим количеством 12 вольтовых 2, 3, 4 проводных вентиляторов
  - Автоматическое управление скоростью пропорционально температуре
  - Ручное управление скоростью из интерфейса программы
  - Управление RGB светодиодной лентой
  - Управление цветом пропорционально температуре (синий - зелёный - жёлтый - красный)
  - Ручное управление цветом из интерфейса программы

  Программа HardwareMonitorPlus  https://github.com/AlexGyver/PCdisplay
  - Запустить OpenHardwareMonitor.exe
  - Options/Serial/Run - запуск соединения с Ардуиной
  - Options/Serial/Config - настройка параметров работы
    - PORT address - адрес порта, куда подключена Ардуина
    - TEMP source - источник показаний температуры (процессор, видеокарта, максимум проц+видео, датчик 1, датчик 2)
    - FAN min, FAN max - минимальные и максимальные обороты вентиляторов, в %
    - TEMP min, TEMP max - минимальная и максимальная температура, в градусах Цельсия
    - Manual FAN - ручное управление скоростью вентилятора в %
    - Manual COLOR - ручное управление цветом ленты
    - LED brightness - управление яркостью ленты
    - CHART interval - интервал обновления графиков

   Что идёт в порт: 0-CPU temp, 1-GPU temp, 2-mother temp, 3-max HDD temp, 4-CPU load, 5-GPU load, 6-RAM use, 7-GPU memory use,
   8-maxFAN, 9-minFAN, 10-maxTEMP, 11-minTEMP, 12-manualFAN, 13-manualCOLOR, 14-fanCtrl, 15-colorCtrl, 16-brightCtrl, 17-LOGinterval, 18-tempSource
*/
// ------------------------ НАСТРОЙКИ ----------------------------
// настройки пределов скорости и температуры по умолчанию (на случай отсутствия связи)
byte speedMIN = 10, speedMAX = 90, tempMIN = 30, tempMAX = 70;
#define DRIVER_VERSION 1    // 0 - маркировка драйвера кончается на 4АТ, 1 - на 4Т
#define COLOR_ALGORITM 0    // 0 или 1 - разные алгоритмы изменения цвета (строка 222)
// ------------------------ НАСТРОЙКИ ----------------------------

// ----------------------- ПИНЫ ---------------------------
#define FAN_PIN 9           // на мосфет вентиляторов
#define R_PIN 5             // на мосфет ленты, красный
#define G_PIN 3             // на мосфет ленты, зелёный
#define B_PIN 6             // на мосфет ленты, синий
#define BTN1 A3             // первая кнопка
#define BTN2 A2             // вторая кнопка
#define SENSOR_PIN 14       // датчик температуры
// ----------------------- ПИНЫ ---------------------------

// -------------------- БИБЛИОТЕКИ ---------------------
#include <OneWire.h>            // библиотека протокола датчиков
#include <DallasTemperature.h>  // библиотека датчика
#include <string.h>             // библиотека расширенной работы со строками
#include <Wire.h>               // библиотека для соединения
#include <LiquidCrystal_I2C.h>  // библтотека дислея
#include <TimerOne.h>           // библиотека таймера
// -------------------- БИБЛИОТЕКИ ---------------------

// -------- АВТОВЫБОР ОПРЕДЕЛЕНИЯ ДИСПЛЕЯ-------------
// Если кончается на 4Т - это 0х27. Если на 4АТ - 0х3f
#if (DRIVER_VERSION)
LiquidCrystal_I2C lcd(0x27, 20, 4);
#else
LiquidCrystal_I2C lcd(0x3f, 20, 4);
#endif
// -------- АВТОВЫБОР ОПРЕДЕЛЕНИЯ ДИСПЛЕЯ-------------

#define printByte(args)  write(args);
#define TEMPERATURE_PRECISION 9
// настройка даьчтков
OneWire oneWire(SENSOR_PIN);
DallasTemperature sensors(&oneWire);
DeviceAddress Thermometer1, Thermometer2;

// стартовый логотип
byte logo0[8] = {0b00011, 0b00110,  0b01110,  0b11111,  0b11011,  0b11001,  0b00000,  0b00000};
byte logo1[8] = {0b10000, 0b00001,  0b00001,  0b00001,  0b00000,  0b10001,  0b11011,  0b11111};
byte logo2[8] = {0b11100, 0b11000,  0b10001,  0b11011,  0b11111,  0b11100,  0b00000,  0b00000};
byte logo3[8] = {0b00000, 0b00001,  0b00011,  0b00111,  0b01101,  0b00111,  0b00010,  0b00000};
byte logo4[8] = {0b11111, 0b11111,  0b11011,  0b10001,  0b00000,  0b00000,  0b00000,  0b00000};
byte logo5[8] = {0b00000, 0b10000,  0b11000,  0b11100,  0b11110,  0b11100,  0b01000,  0b00000};
// значок градуса!!!! lcd.write(223);
byte degree[8] = {0b11100,  0b10100,  0b11100,  0b00000,  0b00000,  0b00000,  0b00000,  0b00000};
// правый край полосы загрузки
byte right_empty[8] = {0b11111,  0b00001,  0b00001,  0b00001,  0b00001,  0b00001,  0b00001,  0b11111};
// левый край полосы загрузки
byte left_empty[8] = {0b11111,  0b10000,  0b10000,  0b10000,  0b10000,  0b10000,  0b10000,  0b11111};
// центр полосы загрузки
byte center_empty[8] = {0b11111, 0b00000,  0b00000,  0b00000,  0b00000,  0b00000,  0b00000,  0b11111};
// блоки для построения графиков
byte row8[8] = {0b11111,  0b11111,  0b11111,  0b11111,  0b11111,  0b11111,  0b11111,  0b11111};
byte row7[8] = {0b00000,  0b11111,  0b11111,  0b11111,  0b11111,  0b11111,  0b11111,  0b11111};
byte row6[8] = {0b00000,  0b00000,  0b11111,  0b11111,  0b11111,  0b11111,  0b11111,  0b11111};
byte row5[8] = {0b00000,  0b00000,  0b00000,  0b11111,  0b11111,  0b11111,  0b11111,  0b11111};
byte row4[8] = {0b00000,  0b00000,  0b00000,  0b00000,  0b11111,  0b11111,  0b11111,  0b11111};
byte row3[8] = {0b00000,  0b00000,  0b00000,  0b00000,  0b00000,  0b11111,  0b11111,  0b11111};
byte row2[8] = {0b00000,  0b00000,  0b00000,  0b00000,  0b00000,  0b00000,  0b11111,  0b11111};
byte row1[8] = {0b00000,  0b00000,  0b00000,  0b00000,  0b00000,  0b00000,  0b00000,  0b11111};

char inData[82];       // массив входных значений (СИМВОЛЫ)
int PCdata[20];        // массив численных значений показаний с компьютера
byte PLOTmem[6][16];   // массив для хранения данных для построения графика (16 значений для 6 параметров)
byte blocks, halfs;
byte index = 0;
int display_mode = 6;
String string_convert;
unsigned long timeout, blink_timer, plot_timer;
boolean lightState, reDraw_flag = 1, updateDisplay_flag, updateTemp_flag, timeOut_flag = 1;
int duty, LEDcolor;
int k, b, R, G, B, Rf, Gf, Bf;
byte mainTemp;
byte lines[] = {4, 5, 7, 6};
byte plotLines[] = {0, 1, 4, 5, 6, 7};    // 0-CPU temp, 1-GPU temp, 2-CPU load, 3-GPU load, 4-RAM load, 5-GPU memory
String perc;
unsigned long sec;
unsigned int mins, hrs;
byte temp1, temp2;
boolean btn1_sig, btn2_sig, btn1_flag, btn2_flag;

// Названия для легенды графиков
const char plot_0[] = "CPU";
const char plot_1[] = "GPU";
const char plot_2[] = "RAM";

const char plot_3[] = "temp";
const char plot_4[] = "load";
const char plot_5[] = "mem";
// названия ниже должны совпадать с массивами сверху и идти по порядку!
static const char *plotNames0[]  = {
  plot_0, plot_1, plot_0, plot_1, plot_2, plot_1
};
static const char *plotNames1[]  = {
  plot_3, plot_3, plot_4, plot_4, plot_4, plot_5
};
// 0-CPU temp, 1-GPU temp, 2-CPU load, 3-GPU load, 4-RAM load, 5-GPU memory

void setup() {
  Serial.begin(9600);
  Timer1.initialize(40);   // поставить частоту ШИМ 25 кГц (40 микросекунд)
  pinMode(R_PIN, OUTPUT);
  pinMode(G_PIN, OUTPUT);
  pinMode(B_PIN, OUTPUT);
  digitalWrite(R_PIN, 0);
  digitalWrite(G_PIN, 0);
  digitalWrite(B_PIN, 0);
  pinMode(BTN1, INPUT_PULLUP);
  pinMode(BTN2, INPUT_PULLUP);
  sensors.begin();
  sensors.getAddress(Thermometer1, 0);
  sensors.getAddress(Thermometer2, 1);
  sensors.setResolution(Thermometer1, TEMPERATURE_PRECISION);
  sensors.setResolution(Thermometer2, TEMPERATURE_PRECISION);
  // инициализация дисплея
  lcd.init();
  lcd.backlight();
  lcd.clear();            // очистить дисплей
  show_logo();            // показать логотип

  Timer1.pwm(FAN_PIN, 400);  // включить вентиляторы на 40%
  delay(2000);               // на 2 секунды
  lcd.clear();               // очистить дисплей
  PCdata[8] = speedMAX;
  PCdata[9] = speedMIN;
  PCdata[10] = tempMAX;
  PCdata[11] = tempMIN;
}
// 8-maxFAN, 9-minFAN, 10-maxTEMP, 11-minTEMP, 12-mnlFAN

// ------------------------------ ОСНОВНОЙ ЦИКЛ -------------------------------
void loop() {
  parsing();                          // парсим строки с компьютера
  updatePlot();                       // обновляем массив данных графика
  getTemperature();                   // получить значения с датчиков температуры
  dutyCalculate();                    // посчитать скважность для вентиляторов
  Timer1.pwm(FAN_PIN, duty * 10);     // управлять вентиляторами
  LEDcontrol();                       // управлять цветом ленты
  buttonsTick();                      // опрос кнопок и смена режимов
  updateDisplay();                    // обновить показания на дисплее
  timeoutTick();                      // проверка таймаута
}
// ------------------------------ ОСНОВНОЙ ЦИКЛ -------------------------------

void buttonsTick() {
  btn1_sig = !digitalRead(BTN1);
  btn2_sig = !digitalRead(BTN2);
  if (btn1_sig && !btn1_flag) {
    display_mode++;
    reDraw_flag = 1;
    if (display_mode > 7) display_mode = 0;
    btn1_flag = 1;
  }
  if (!btn1_sig && btn1_flag) {
    btn1_flag = 0;
  }
  if (btn2_sig && !btn2_flag) {
    display_mode--;
    reDraw_flag = 1;
    if (display_mode < 0) display_mode = 7;
    btn2_flag = 1;
  }
  if (!btn2_sig && btn2_flag) {
    btn2_flag = 0;
  }
}

void getTemperature() {
  if (updateTemp_flag) {
    sensors.requestTemperatures();
    temp1 = sensors.getTempC(Thermometer1);
    temp2 = sensors.getTempC(Thermometer2);
    updateTemp_flag = 0;
  }
}
void LEDcontrol() {
  b = PCdata[16];
  if (PCdata[13] == 1)          // если стоит галочка Manual Color
    LEDcolor = PCdata[15];      // цвет равен установленному ползунком
  else {                        // если нет
    LEDcolor = map(mainTemp, PCdata[11], PCdata[10], 0, 1000);
    LEDcolor = constrain(LEDcolor, 0, 1000);
  }

  if (COLOR_ALGORITM) {
    // алгоритм цвета 1
    // синий убавляется, зелёный прибавляется
    // зелёный убавляется, красный прибавляется
    if (LEDcolor <= 500) {
      k = map(LEDcolor, 0, 500, 0, 255);
      R = 0;
      G = k;
      B = 255 - k;
    }
    if (LEDcolor > 500) {
      k = map(LEDcolor, 500, 1000, 0, 255);
      R = k;
      G = 255 - k;
      B = 0;
    }

  } else {
    // алгоритм цвета 2
    // синий максимум, плавно прибавляется зелёный
    // зелёный максимум, плавно убавляется синий
    // зелёный максимум, плавно прибавляется красный
    // красный максимум, плавно убавляется зелёный

    if (LEDcolor <= 250) {
      k = map(LEDcolor, 0, 250, 0, 255);
      R = 0;
      G = k;
      B = 255;
    }
    if (LEDcolor > 250 && LEDcolor <= 500) {
      k = map(LEDcolor, 250, 500, 0, 255);
      R = 0;
      G = 255;
      B = 255 - k;
    }
    if (LEDcolor > 500 && LEDcolor <= 750) {
      k = map(LEDcolor, 500, 750, 0, 255);
      R = k;
      G = 255;
      B = 0;
    }
    if (LEDcolor > 750 && LEDcolor <= 1000) {
      k = map(LEDcolor, 750, 1000, 0, 255);
      R = 255;
      G = 255 - k;
      B = 0;
    }
  }

  Rf = (b * R / 100);
  Gf = (b * G / 100);
  Bf = (b * B / 100);
  analogWrite(R_PIN, Rf);
  analogWrite(G_PIN, Gf);
  analogWrite(B_PIN, Bf);
}

void dutyCalculate() {
  if (PCdata[12] == 1)                  // если стоит галочка ManualFAN
    duty = PCdata[14];                  // скважность равна установленной ползунком
  else {                                // если нет
    switch (PCdata[18]) {
      case 0: mainTemp = PCdata[0];                   // взять опорную температуру как CPU
        break;
      case 1: mainTemp = PCdata[1];                   // взять опорную температуру как GPU
        break;
      case 2: mainTemp = max(PCdata[0], PCdata[1]);   // взять опорную температуру как максимум CPU и GPU
        break;
      case 3: mainTemp = temp1;
        break;
      case 4: mainTemp = temp2;
        break;
    }
    duty = map(mainTemp, PCdata[11], PCdata[10], PCdata[9], PCdata[8]);
    duty = constrain(duty, PCdata[9], PCdata[8]);
  }
}
void parsing() {
  while (Serial.available() > 0) {
    char aChar = Serial.read();
    if (aChar != 'E') {
      inData[index] = aChar;
      index++;
      inData[index] = '\0';
    } else {
      char *p = inData;
      char *str;
      index = 0;
      String value = "";
      while ((str = strtok_r(p, ";", &p)) != NULL) {
        string_convert = str;
        PCdata[index] = string_convert.toInt();
        index++;
      }
      index = 0;
      updateDisplay_flag = 1;
      updateTemp_flag = 1;
    }
    timeout = millis();
    timeOut_flag = 1;
  }
}
void updatePlot() {
  if ((millis() - plot_timer) > (PCdata[17] * 1000)) {
    for (int i = 0; i < 6; i++) {           // для каждой строки параметров
      for (int j = 0; j < 15; j++) {        // каждый столбец параметров (кроме последнего)
        PLOTmem[i][j] = PLOTmem[i][j + 1];  // сдвинуть весь массив на шаг ВЛЕВО
      }
    }
    for (int i = 0; i < 6; i++) {
      // запомнить общее число полосок графика в ПОСЛЕДНИЙ элемент массива
      PLOTmem[i][15] = ceil(PCdata[plotLines[i]] / 3);
    }
    plot_timer = millis();
  }
}
void updateDisplay() {
  if (updateDisplay_flag) {
    if (reDraw_flag) {
      lcd.clear();
      switch (display_mode) {
        case 0:
        case 1:
        case 2:
        case 3:
        case 4:
        case 5: draw_plot_symb();
          break;
        case 6: draw_labels_1();
          break;
        case 7: draw_labels_2();
          break;
      }
      reDraw_flag = 0;
    }
    switch (display_mode) {
      case 0:
      case 1:
      case 2:
      case 3:
      case 4:
      case 5: draw_plot();
        break;
      case 6: draw_stats_1();
        break;
      case 7: draw_stats_2();
        break;
      case 50: debug();
        break;
    }
    updateDisplay_flag = 0;
  }
}
void draw_stats_1() {
  lcd.setCursor(4, 0); lcd.print(PCdata[0]); lcd.write(223);
  lcd.setCursor(17, 0); lcd.print(PCdata[4]);
  if (PCdata[4] < 10) perc = "% ";
  else if (PCdata[4] < 100) perc = "%";
  else perc = "";  lcd.print(perc);
  lcd.setCursor(4, 1); lcd.print(PCdata[1]); lcd.write(223);
  lcd.setCursor(17, 1); lcd.print(PCdata[5]);
  if (PCdata[5] < 10) perc = "% ";
  else if (PCdata[5] < 100) perc = "%";
  else perc = "";  lcd.print(perc);
  lcd.setCursor(17, 2); lcd.print(PCdata[7]);
  if (PCdata[7] < 10) perc = "% ";
  else if (PCdata[7] < 100) perc = "%";
  else perc = "";  lcd.print(perc);
  lcd.setCursor(17, 3); lcd.print(PCdata[6]);
  if (PCdata[6] < 10) perc = "% ";
  else if (PCdata[6] < 100) perc = "%";
  else perc = "";  lcd.print(perc);

  for (int i = 0; i < 4; i++) {
    byte line = ceil(PCdata[lines[i]] / 10);
    lcd.setCursor(7, i);
    if (line == 0) lcd.printByte(1)
      else lcd.printByte(4);
    for (int n = 1; n < 9; n++) {
      if (n < line) lcd.printByte(4);
      if (n >= line) lcd.printByte(2);
    }
    if (line == 10) lcd.printByte(4)
      else lcd.printByte(3);
  }
}
void draw_stats_2() {
  lcd.setCursor(16, 0); lcd.print(duty);
  if ((duty) < 10) perc = "% ";
  else if ((duty) < 100) perc = "%";
  else perc = "";  lcd.print(perc);

  lcd.setCursor(6, 1); lcd.print(temp1); lcd.write(223);
  lcd.setCursor(16, 1); lcd.print(temp2); lcd.write(223);
  lcd.setCursor(4, 2); lcd.print(PCdata[2]); lcd.write(223);
  lcd.setCursor(16, 2); lcd.print(PCdata[3]); lcd.write(223);

  lcd.setCursor(9, 3);
  sec = (long)(millis() - blink_timer) / 1000;
  hrs = floor((sec / 3600));
  mins = floor(sec - (hrs * 3600)) / 60;
  sec = sec - (hrs * 3600 + mins * 60);
  if (hrs < 10) lcd.print(0);
  lcd.print(hrs);
  lcd.print(":");
  if (mins < 10) lcd.print(0);
  lcd.print(mins);
  lcd.print(":");
  if (sec < 10) lcd.print(0);
  lcd.print(sec);

  byte line = ceil(duty / 10);
  lcd.setCursor(6, 0);
  if (line == 0) lcd.printByte(1)
    else lcd.printByte(4);
  for (int n = 1; n < 9; n++) {
    if (n < line) lcd.printByte(4);
    if (n >= line) lcd.printByte(2);
  }
  if (line == 10) lcd.printByte(4)
    else lcd.printByte(3);
}
void draw_labels_1() {
  lcd.createChar(0, degree);
  lcd.createChar(1, left_empty);
  lcd.createChar(2, center_empty);
  lcd.createChar(3, right_empty);
  lcd.createChar(4, row8);
  lcd.setCursor(0, 0);
  lcd.print("CPU:");
  lcd.setCursor(0, 1);
  lcd.print("GPU:");
  lcd.setCursor(0, 2);
  lcd.print("GPUmem:");
  lcd.setCursor(0, 3);
  lcd.print("RAMuse:");
}
void draw_labels_2() {
  lcd.createChar(0, degree);
  lcd.createChar(1, left_empty);
  lcd.createChar(2, center_empty);
  lcd.createChar(3, right_empty);
  lcd.createChar(4, row8);

  lcd.setCursor(0, 0);
  lcd.print("FANsp:");
  lcd.setCursor(0, 1);
  lcd.print("TMP1: ");
  lcd.setCursor(10, 1);
  lcd.print("TMP2:");
  lcd.setCursor(0, 2);
  lcd.print("MOM:");
  lcd.setCursor(9, 2);
  lcd.print("HDDmax:");
  lcd.setCursor(0, 3);
  lcd.print("UPTIME:");
}
void draw_legend() {
  byte data = PCdata[plotLines[display_mode]];
  lcd.setCursor(16, 2); lcd.print(data);
  if (display_mode > 1) {
    if (data < 10) perc = "% ";
    else if (data < 100) perc = "%";
    else {
      perc = "";
    }
    lcd.print(perc);
  } else {
    if (data < 10) {
      lcd.write(223);
      lcd.print("  ");
    } else if (data < 100) {
      lcd.write(223); lcd.print(" ");
    } else {
      lcd.write(223);
    }
  }
}

void draw_plot() {
  draw_legend();

  for (byte i = 0; i < 16; i++) {                       // каждый столбец параметров
    blocks = floor(PLOTmem[display_mode][i] / 8);       // найти количество целых блоков
    halfs = PLOTmem[display_mode][i] - blocks * 8;      // найти число оставшихся полосок
    for (byte n = 0; n < 4; n++) {
      if (n < blocks) {
        lcd.setCursor(i, (3 - n));
        lcd.printByte(0);
      }
      if (n >= blocks) {
        if (n != 3) {
          lcd.setCursor(i, (3 - n));
          if (halfs > 0) lcd.printByte(halfs);
          for (byte k = n + 1; k < 4; k++) {
            lcd.setCursor(i, (3 - k));
            lcd.print(" ");
          }
          break;
        }
      }
    }
  }
}

void draw_plot_symb() {
  lcd.createChar(0, row8);
  lcd.createChar(1, row1);
  lcd.createChar(2, row2);
  lcd.createChar(3, row3);
  lcd.createChar(4, row4);
  lcd.createChar(5, row5);
  lcd.createChar(6, row6);
  lcd.createChar(7, row7);
  lcd.setCursor(16, 0);
  lcd.print(plotNames0[display_mode]);
  lcd.setCursor(16, 1);
  lcd.print(plotNames1[display_mode]);
}
void timeoutTick() {
  if ((millis() - timeout > 5000) && timeOut_flag) {
    duty = speedMIN;
    lcd.clear();
    lcd.setCursor(5, 1);
    lcd.print("CONNECTION");
    lcd.setCursor(7, 2);
    lcd.print("FAILED");
    timeOut_flag = 0;
    reDraw_flag = 1;
  }
}
void show_logo() {
  lcd.createChar(0, logo0);
  lcd.createChar(1, logo1);
  lcd.createChar(2, logo2);
  lcd.createChar(3, logo3);
  lcd.createChar(4, logo4);
  lcd.createChar(5, logo5);
  lcd.setCursor(2, 1);
  lcd.printByte(0);
  lcd.printByte(1);
  lcd.printByte(2);
  lcd.setCursor(2, 2);
  lcd.printByte(3);
  lcd.printByte(4);
  lcd.printByte(5);
  lcd.setCursor(6, 1);
  lcd.print("AlexGyver");
  lcd.setCursor(6, 2);
  lcd.print("Technologies");
}
void debug() {
  lcd.clear();
  lcd.setCursor(0, 0);
  for (int j = 0; j < 5; j++) {
    lcd.print(PCdata[j]); lcd.print("  ");
  }
  lcd.setCursor(0, 1);
  for (int j = 6; j < 10; j++) {
    lcd.print(PCdata[j]); lcd.print("  ");
  }
  lcd.setCursor(0, 2);
  for (int j = 10; j < 15; j++) {
    lcd.print(PCdata[j]); lcd.print("  ");
  }
  lcd.setCursor(0, 3);
  for (int j = 15; j < 18; j++) {
    lcd.print(PCdata[j]); lcd.print("  ");
  }
}
