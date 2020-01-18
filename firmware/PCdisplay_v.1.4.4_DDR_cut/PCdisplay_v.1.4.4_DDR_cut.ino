/*
  Блок электроники для крутого моддинга вашего ПК, возможности:
  - Вывод основных параметров железа на внешний LCD дисплей
  - Температура: CPU, GPU, материнская плата, самый горячий HDD
  - Уровень загрузки: CPU, GPU, RAM, видеопамять
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

  Что идёт в порт: 0-CPU temp, 1-GPU temp, 2-mother temp, 3-max HDD temp, 4-CPU load, 5-GPU load, 6-RAM use, 7-GPU memory use, 8-maxFAN, 
  9-minFAN, 10-maxTEMP, 11-minTEMP, 12-manualFAN, 13-manualCOLOR, 14-fanCtrl, 15-colorCtrl, 16-brightCtrl, 17-LOGinterval, 18-tempSource, 19-AltCPU temp
*/
// ------------------------ НАСТРОЙКИ ----------------------------
// настройки пределов скорости и температуры по умолчанию (на случай отсутствия связи)
byte speedMIN = 10, speedMAX = 90, tempMIN = 30, tempMAX = 70;
#define DRIVER_VERSION 1    // 0 - маркировка драйвера кончается на 4АТ, 1 - на 4Т
#define CPU_TEMP_SENSOR 1     // 0 или 1, выбрать перебором тот датчик, с которым температура процессора будет ближе к реальной
#define COLOR_ALGORITM 0    // 0 или 1 - разные алгоритмы изменения цвета (строка 222)
#define ERROR_DUTY 90       // скорость вентиляторов при потере связи
#define ERROR_BACKLIGHT 0   // 0 - гасить подсветку при потере сигнала, 1 - не гасить
#define ERROR_TEMP 1        // 1 - показывать температуру при потере связи. 0 - нет
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

#if (CPU_TEMP_SENSOR)
int CPUtemp = 19;
#else
int CPUtemp = 0;
#endif

#define printByte(args)  write(args);
#define TEMPERATURE_PRECISION 9
// настройка даьчтков
OneWire oneWire(SENSOR_PIN);
DallasTemperature sensors(&oneWire);
DeviceAddress Thermometer1, Thermometer2;

// стартовый логотип DeltaDesignlab
byte logo0[8] = {0b11111,  0b11111,  0b11111,  0b11111,  0b11111,  0b11111,  0b11111,  0b11111}; //Белый прямоугольник
byte logo1[8] = {0b00001,  0b00001,  0b00011,  0b00111,  0b00111,  0b01111,  0b11111,  0b11111}; //Правый скос
byte logo2[8] = {0b10000,  0b10000,  0b11000,  0b11100,  0b11100,  0b11110,  0b11111,  0b11111}; //Левый скос
byte logo3[8] = {0b11110,  0b11110,  0b11100,  0b11000,  0b11000,  0b10000,  0b00000,  0b00000}; //Обратный скос
byte logo4[8] = {0b11110,  0b10001,  0b10001,  0b10001,  0b10001,  0b10001,  0b11110,  0b00000}; //Кастомный символ D
byte logo5[8] = {0b00000,  0b00000,  0b01111,  0b10001,  0b10001,  0b01111,  0b00001,  0b01110}; //Кастомный символ g

byte right_empty[8] = {0b11111,  0b00001,  0b00001,  0b00001,  0b00001,  0b00001,  0b00001,  0b11111};
byte left_empty[8] = {0b11111,  0b10000,  0b10000,  0b10000,  0b10000,  0b10000,  0b10000,  0b11111};
byte center_empty[8] = {0b11111, 0b00000,  0b00000,  0b00000,  0b00000,  0b00000,  0b00000,  0b11111};
byte bar2[] = {0b11111, 0b11000, 0b11000, 0b11000, 0b11000, 0b11000, 0b11000, 0b11111};
byte bar3[] = {  B11111,  B11100,  B11100,  B11100,  B11100,  B11100,  B11100,  B11111};
byte bar4[] = {  B11111,  B11110,  B11110,  B11110,  B11110,  B11110,  B11110,  B11111};
byte rightbar1[] = {  B11111,  B10001,  B10001,  B10001,  B10001,  B10001,  B10001,  B11111};
//byte rightbar2[] = {  B11111,  B11001,  B11001,  B11001,  B11001,  B11001,  B11001,  B11111}; //жертва оптимизации
byte rightbar3[] = {  B11111,  B11101,  B11101,  B11101,  B11101,  B11101,  B11101,  B11111};

char inData[82];       // массив входных значений (СИМВОЛЫ)
int PCdata[20];        // массив численных значений показаний с компьютера
byte blocks, halfs;
byte index = 0;
int display_mode = 1;
String string_convert;
unsigned long timeout/*, uptime_timer*/;
boolean lightState, reDraw_flag = 1, updateDisplay_flag, updateTemp_flag, timeOut_flag = 1;
int duty, LEDcolor;
int k, b, R, G, B, Rf, Gf, Bf;
byte mainTemp;
byte lines[] = {4, 5, 7, 6}; // Lines - CPU, GPU, GPUmem, RAMuse
String perc;
unsigned long sec, mins, hrs;
byte temp1, temp2;
boolean btn1_sig, btn2_sig, btn1_flag, btn2_flag;

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
  lcd.backlight();        // включить подсветку
  lcd.clear();            // очистить дисплей
  show_logo();            // показать логотип

  Timer1.pwm(FAN_PIN, 400);  // включить вентиляторы на 40%
  delay(2000);               // на 2 секунды
  lcd.clear();               // очистить дисплей
  PCdata[8] = speedMAX;
  PCdata[9] = speedMIN;
  PCdata[10] = tempMAX;
  PCdata[11] = tempMIN;
  initBar3();
}
// 8-maxFAN, 9-minFAN, 10-maxTEMP, 11-minTEMP, 12-mnlFAN

// ------------------------------ ОСНОВНОЙ ЦИКЛ -------------------------------
void loop() {
  parsing();                          // парсим строки с компьютера
  getTemperature();                   // получить значения с датчиков температуры
  dutyCalculate();                    // посчитать скважность для вентиляторов
  Timer1.pwm(FAN_PIN, duty * 10);     // управлять вентиляторами
  LEDcontrol();                       // управлять цветом ленты
  buttonsTick();                      // опрос кнопок и смена режимов
  updateDisplay();                    // обновить показания на дисплее
  timeoutTick();                      // проверка таймаута
}
// ------------------------------ ОСНОВНОЙ ЦИКЛ -------------------------------

void initBar3() {
  lcd.createChar(0, left_empty);
  lcd.createChar(1, center_empty);
  lcd.createChar(2, right_empty);
  lcd.createChar(3, bar2);
  lcd.createChar(4, bar3);
  lcd.createChar(5, bar4);
  lcd.createChar(6, rightbar1);
  lcd.createChar(7, rightbar3);
}

void fillBar3(byte start_pos, byte row, byte bar_length, byte fill_percent) {
  byte infill = bar_length * fill_percent / 10;
  byte fract = infill % 10;
  infill = infill / 10;

  lcd.setCursor(start_pos, row);
  if (infill == 0) {
    if (fract >= 0 && fract < 2) lcd.write(0);
    else if (fract >= 2 && fract < 4) lcd.write(0);
    else if (fract >= 4 && fract < 6) lcd.write(3);
    else if (fract >= 6 && fract < 8) lcd.write(4);
    else if (fract >= 8) lcd.write(5);
  }
  else lcd.write(255);
  for (int n = 1; n < bar_length - 1; n++) {
    if (n < infill) lcd.write(255);
    if (n == infill) {
      if (fract >= 0 && fract < 2) lcd.write(1);
      else if (fract >= 2 && fract < 4) lcd.write(0);
      else if (fract >= 4 && fract < 6) lcd.write(3);
      else if (fract >= 6 && fract < 8) lcd.write(4);
      else if (fract >= 8) lcd.write(5);
    }
    if (n > infill) lcd.write(1);
  }
  if (infill == bar_length - 1) {
    if (fract >= 0 && fract < 2) lcd.write(2);
    else if (fract >= 2 && fract < 4) lcd.write(6);
    else if (fract >= 4 && fract < 6) lcd.write(6);
    else if (fract >= 6 && fract < 8) lcd.write(7);
    else if (fract >= 8) lcd.write(255);
  }
  else if (infill == bar_length) lcd.write(255);
  else lcd.write(2);
}

void buttonsTick() {
  btn1_sig = !digitalRead(BTN1);
  btn2_sig = !digitalRead(BTN2);
  if (btn1_sig && !btn1_flag) {
    display_mode++;
    reDraw_flag = 1;
    if (display_mode > 2) display_mode = 0;
    btn1_flag = 1;
  }
  if (!btn1_sig && btn1_flag) {
    btn1_flag = 0;
  }
  if (btn2_sig && !btn2_flag) {
    display_mode--;
    reDraw_flag = 1;
    if (display_mode < 0) display_mode = 2;
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
      case 0: mainTemp = PCdata[CPUtemp];                   // взять опорную температуру как CPU
        break;
      case 1: mainTemp = PCdata[1];                   // взять опорную температуру как GPU
        break;
      case 2: mainTemp = max(PCdata[CPUtemp], PCdata[1]);   // взять опорную температуру как максимум CPU и GPU
        break;
      case 3: mainTemp = temp1;
        break;
      case 4: mainTemp = temp2;
        break;
    }
    duty = map(mainTemp, PCdata[11], PCdata[10], PCdata[9], PCdata[8]);
    duty = constrain(duty, PCdata[9], PCdata[8]);
  }
  if (!timeOut_flag) duty = ERROR_DUTY;               // если пропало соединение, поставить вентиляторы на ERROR_DUTY
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
    if (!timeOut_flag) {                                // если связь была потеряна, но восстановилась
      if (!ERROR_BACKLIGHT) lcd.backlight();            // включить подсветку при появлении сигнала, если разрешено
      //if (ERROR_UPTIME) uptime_timer = millis();        // сбросить uptime, если разрешено
    }
    timeout = millis();
    timeOut_flag = 1;
  }
}

void updateDisplay() {
  if (updateDisplay_flag) {
    if (reDraw_flag) {
      lcd.clear();
      switch (display_mode) {
        case 0:
        case 1: draw_labels_1();
          break;
        case 2: draw_labels_2();
          break;
      }
      reDraw_flag = 0;
    }
    switch (display_mode) {
      case 0:
      case 1: draw_stats_1();
        break;
      case 2: draw_stats_2();
        break;
      case 50: debug();
        break;
    }
    updateDisplay_flag = 0;
  }
}

void draw_stats_1() {
  lcd.setCursor(4, 0); lcd.print(PCdata[CPUtemp]); lcd.write(223); // CPU-temp
  lcd.setCursor(17, 0); lcd.print(PCdata[4]); // CPU_load
  if (PCdata[4] < 10) perc = "% ";
  else if (PCdata[4] < 100) perc = "%";
  else perc = "";  lcd.print(perc);
  lcd.setCursor(4, 1); lcd.print(PCdata[1]); lcd.write(223); // GPU_temp
  lcd.setCursor(17, 1); lcd.print(PCdata[5]); // GPU_load
  if (PCdata[5] < 10) perc = "% ";
  else if (PCdata[5] < 100) perc = "%";
  else perc = "";  lcd.print(perc);
  lcd.setCursor(17, 2); lcd.print(PCdata[7]); // GPU_mem
  if (PCdata[7] < 10) perc = "% ";
  else if (PCdata[7] < 100) perc = "%";
  else perc = "";  lcd.print(perc);
  lcd.setCursor(17, 3); lcd.print(PCdata[6]); // RAM_use
  if (PCdata[6] < 10) perc = "% ";
  else if (PCdata[6] < 100) perc = "%";
  else perc = "";  lcd.print(perc);

  //fillBar3 принимает аргументы (столбец, строка, длина полосы, значение в % (0 - 100) )
  fillBar3(7, 0, 10, PCdata[4]);
  fillBar3(7, 1, 10, PCdata[5]);
  fillBar3(7, 2, 10, PCdata[7]);
  fillBar3(7, 3, 10, PCdata[6]);
}

void draw_stats_2() {
  lcd.setCursor(16, 0); lcd.print(duty);
  if ((duty) < 10) perc = "% ";
  else if ((duty) < 100) perc = "%";
  else perc = "";  lcd.print(perc);

  lcd.setCursor(6, 1); lcd.print(temp1); lcd.write(223);
  lcd.setCursor(16, 1); lcd.print(temp2); lcd.write(223);
  lcd.setCursor(4, 2); lcd.print(PCdata[2]); lcd.write(223); // MB_temp
  lcd.setCursor(16, 2); lcd.print(PCdata[3]); lcd.write(223); // HDD_Max_temp

  /* lcd.setCursor(9, 3);
    sec = (long)(millis() - uptime_timer) / 1000;
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
  */
  //fillBar3 принимает аргументы (столбец, строка, длина полосы, значение в % (0 - 100) )
  fillBar3(6, 0, 10, duty);
}

void draw_labels_1() {
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
  lcd.setCursor(0, 0);
  lcd.print("FANsp:");
  lcd.setCursor(0, 1);
  lcd.print("TMP1: ");
  lcd.setCursor(10, 1);
  lcd.print("TMP2:");
  lcd.setCursor(0, 2);
  lcd.print("MB:");
  lcd.setCursor(9, 2);
  lcd.print("HDDmax:");
  //lcd.setCursor(0, 3);
  //lcd.print("UPTIME:");
}

void timeoutTick() {
  if ((millis() - timeout > 5000) && timeOut_flag) {
    lcd.clear();
    lcd.setCursor(5, 1);
    lcd.print("CONNECTION");
    lcd.setCursor(7, 2);
    lcd.print("FAILED");
    timeOut_flag = 0;
    reDraw_flag = 1;
    if (!ERROR_BACKLIGHT) lcd.noBacklight();   // вырубить подсветку, если разрешено
  }
}
// Logo DeltaDesignlab
void show_logo() {
  lcd.createChar(0, logo0);
  lcd.createChar(1, logo1);
  lcd.createChar(2, logo2);
  lcd.createChar(3, logo3);
  lcd.createChar(4, logo4); //D
  lcd.createChar(5, logo5); //g
  lcd.setCursor(5, 0);
  lcd.printByte(1);
  lcd.printByte(2);
  lcd.print(" PC dispay by");
  lcd.setCursor(4, 1);
  lcd.printByte(1);
  lcd.printByte(0);
  lcd.printByte(0);
  lcd.printByte(2);
  lcd.print(" AlexGyver");
  lcd.setCursor(3, 2);
  lcd.printByte(1);
  lcd.printByte(0);
  lcd.printByte(0);
  lcd.printByte(0);
  lcd.printByte(0);
  lcd.printByte(2);
  lcd.setCursor(2, 3);
  lcd.printByte(1);
  lcd.setCursor(3, 3);
  lcd.printByte(3);
  lcd.printByte(4);
  lcd.print("elta");
  lcd.printByte(4);
  lcd.print("esi");
  lcd.printByte(5);
  lcd.print("nLab");
}


void debug() {
  lcd.clear();
  lcd.setCursor(0, 0);
  for (int j = 0; j < 5; j++) { // 0-CPU_temp, 1-GPU_temp, 2-MB_temp, 3-HDD_Max_temp, 4-CPU_load
    lcd.print(PCdata[j]); lcd.print("  ");
  }
  lcd.setCursor(0, 1);
  for (int j = 5; j < 10; j++) { // 5-GPU_load, 6-RAM_use, 7-GPU_mem, 8-FAN_speed, 9-?
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
