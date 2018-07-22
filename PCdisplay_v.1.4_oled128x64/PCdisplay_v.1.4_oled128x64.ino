
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

  ---------- Касаемо OLED экрана ----------
  
  1.  На OLED экране очень мало места, как выяснилось)) По-этому были убраны знаки градусов у температур, дабы заполучить
  хотя бы ещё один свободный символ для построения полос загрузки.
  Все элементы были по-максимуму адаптированы под данный экран (в т.ч. полосы загрузки), и распределены для комфортного восприятия.
  2.  Для экрана так же дописана функция отключения экарана по истечению 25 секунд, когда данные не поступают...объясню зачем:
  Часто кидаю комп в режим сна вместо полного отключения. USB порты при этом остаются работать, благодаря чему
  после отключения светится надпись о том, что связь потеряна. Дабы можно было класть комп в режим сна, и не беспокоится о
  системном мониторе - как раз и дописана функция отключения экрана. Точнее по факту это функция очистки, но т.к. oled не
  имеют подсветки (светятся сами пиксели), то дисплей по факту выключен.
  3.  Для OLED-дисплея была использована текстовая библиотека. Чтобы рисовать различные картинки, лого, графики и т.п. - 
  рекомендуется использовать графическую. Но графическая крайне много жрет памяти ардуины. Чтобы не нарушать стабильность
  выбор пал именно на текстовую.
  По-этому из прошивки удалены (точнее закоментированы) функции вывода логотипов и графиков.
  Для желающих по-эксперементировать графическая библиотека тоже приложена. Но там немного другая адресация
  функции отрисовки надписей надо будет немного переделать.
  4.  Остальные функции прошивки алекса по максимуму сохранены

  PS:  Хотел написать так же скетч для работы с экраном 128x32 - но и тут закралась подства. Библиотека iarduino (по крайней мере
  шрифты в ней), по ходу, расчитана только на 128х64. Зато на удивление этот скетч залился и работал с экраном 128x32, правда
  мелкие надписи были нечитаемы...
  
  PPS:  Да, я прекрасно знаю что есть похожая библиотека от Adafrut для ssd1306. Но она с моим дисплеем 128х64 напрочь отказалась
  работать. Ни один пример не завёлся. Зато с 128х32 отлично работает...
  Но т.к. всё таки первый экран больше инфы вмещает - выбрал его, и собственно библиотеку от iarduino.
  
  ---------- Касаемо OLED экрана ----------


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
#define ERROR_DUTY 90       // скорость вентиляторов при потере связи
#define ERROR_BACKLIGHT 0   // 0 - гасить подсветку при потере сигнала, 1 - не гасить
#define ERROR_UPTIME 0      // 1 - сбрасывать uptime при потере связи, 0 - нет
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
#include <iarduino_OLED_txt.h>  // Подключаем библиотеку iarduino_OLED_txt.
#include <TimerOne.h>           // библиотека таймера
// -------------------- БИБЛИОТЕКИ ---------------------

//---------- ОПРЕДЕЛЕНИЕ ДИСПЛЕЯ И ШРИФТОВ -------------
iarduino_OLED_txt lcd(0x78);    // Объявляем объект myOLED, указывая адрес дисплея на шине I2C: 0x78 (если учитывать бит RW=0).                                                  //
extern uint8_t MediumFontRus[]; // Подключаем шрифт MediumFontRusntRus. 
extern uint8_t SmallFontRus[];  // Подключаем шрифт SmallFontRus. 
//---------- ОПРЕДЕЛЕНИЕ ДИСПЛЕЯ И ШРИФТОВ -------------

#define TEMPERATURE_PRECISION 9
// настройка датчтков
OneWire oneWire(SENSOR_PIN);
DallasTemperature sensors(&oneWire);
DeviceAddress Thermometer1, Thermometer2;

/* стартовый логотип
Логотипы не использовал, т.к. для дисплея используется текстовая библиотека. Чтобы рисовать любые картинки - нужна графическая
Но она крайне прожорлива в плане ресурсов (на nano появляется предупреждение о нехватке памяти). Надо было чем-то жертвовать.
Но библиотеку графическую на всякий случай приложил. Желающие могут поэксперементировать.

*/
char inData[82];       // массив входных значений (СИМВОЛЫ)
int PCdata[20];        // массив численных значений показаний с компьютера
byte PLOTmem[6][16];   // массив для хранения данных для построения графика (16 значений для 6 параметров)
byte blocks, halfs;
byte index = 0;
int display_mode = 6;
String string_convert;
unsigned long timeout, uptime_timer, plot_timer;
boolean lightState, reDraw_flag = 1, updateDisplay_flag, updateTemp_flag, timeOut_flag = 1, sleep_flag = 1;;
int duty, LEDcolor;
int k, b, R, G, B, Rf, Gf, Bf;
byte mainTemp;
byte lines[] = {4, 5, 7, 6};
byte plotLines[] = {0, 1, 4, 5, 6, 7};    // 0-CPU temp, 1-GPU temp, 2-CPU load, 3-GPU load, 4-RAM load, 5-GPU memory
String perc;
unsigned long sec, mins, hrs;
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
  lcd.begin();                                    // Инициируем работу с дисплеем.
  lcd.setFont(MediumFontRus);                     // Указываем шрифт который требуется использовать для вывода цифр и текста.
  lcd.setCoding(TXT_UTF8);                        //кодировка может не подойти, если вместо русских букв "кракозябры" пробуйте 1 из вариантов TXT_UTF8 TXT_CP866 TXT_WIN1251
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
//  lcd.init();
//  lcd.backlight();
  lcd.clrScr();            // очистить дисплей
  show_logo();            // показать логотип

  Timer1.pwm(FAN_PIN, 400);  // включить вентиляторы на 40%
  delay(2000);               // на 2 секунды
  lcd.clrScr();               // очистить дисплей
  PCdata[8] = speedMAX;
  PCdata[9] = speedMIN;
  PCdata[10] = tempMAX;
  PCdata[11] = tempMIN;
}
// 8-maxFAN, 9-minFAN, 10-maxTEMP, 11-minTEMP, 12-mnlFAN

// ------------------------------ ОСНОВНОЙ ЦИКЛ -------------------------------
void loop() {
  parsing();                          // парсим строки с компьютера
  //updatePlot();                       // обновляем массив данных графика
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
//      if (!ERROR_BACKLIGHT) lcd.backlight();            // включить подсветку при появлении сигнала, если разрешено
      if (ERROR_UPTIME) uptime_timer = millis();        // сбросить uptime, если разрешено
    }
    timeout = millis();
    timeOut_flag = 1;
    sleep_flag = 1;
  }
}

void updateDisplay() {
  if (updateDisplay_flag) {
    if (reDraw_flag) {
      lcd.clrScr();
      switch (display_mode) {
        case 0:
        case 1:
        case 2:
        case 3:
        case 4:
        case 5: //draw_plot_symb();
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
      case 5: //draw_plot();
        break;
      case 6: draw_stats_1();
        break;
      case 7: draw_stats_2();
        break;
      /*case 50: debug();
        break;*/
    }
    updateDisplay_flag = 0;
  }
}
void draw_stats_1() {
  lcd.print(PCdata[0], 24, 1); 
  lcd.print(PCdata[4], 92, 1);
  if (PCdata[4] < 10) perc = "% ";
  else if (PCdata[4] < 100) perc = "%";
  else perc = "";  lcd.print(perc);
  lcd.print(PCdata[1], 24, 3);
  lcd.print(PCdata[5], 92, 3);
  if (PCdata[5] < 10) perc = "% ";
  else if (PCdata[5] < 100) perc = "%";
  else perc = "";  lcd.print(perc);
  lcd.print(PCdata[7], 92, 5);
  if (PCdata[7] < 10) perc = "% ";
  else if (PCdata[7] < 100) perc = "%";
  else perc = "";  lcd.print(perc);
  lcd.print(PCdata[6], 92, 7);
  if (PCdata[6] < 10) perc = "% ";
  else if (PCdata[6] < 100) perc = "%";
  else perc = "";  lcd.print(perc);
  draw_stats_progress(1,map(PCdata[4],0,100,0,5));
  draw_stats_progress(3,map(PCdata[5],0,100,0,5));
  draw_stats_progress(5,map(PCdata[7],0,100,0,5));
  draw_stats_progress(7,map(PCdata[6],0,100,0,5));
}

void draw_stats_progress(int line, int num) {
  for (int i=1; i<=6; i++) {
    if (num>=i) lcd.print("|", 43+i*7, line); else if (i!=6) lcd.print(" ", 43+i*7, line);
  }
}

void draw_stats_2() {
  lcd.print(duty, 92, 1);
  if ((duty) < 10) perc = "% ";
  else if ((duty) < 100) perc = "%";
  else perc = "";  lcd.print(perc);
  draw_stats_progress(1,map(duty,0,100,0,5));

  lcd.print(temp1, 35, 3);
  lcd.print(temp2, 104, 3);
  lcd.print(PCdata[2], 35, 5);
  lcd.print(PCdata[3], 104, 5); 
  
  lcd.setCursor(64, 7);
  lcd.setFont(SmallFontRus);
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
  lcd.setFont(MediumFontRus);
}

void draw_labels_1() {
  lcd.setFont(SmallFontRus);  
  lcd.print("CPU:"    , 0, 1);
  lcd.print("GPU:"    , 0, 3);
  lcd.print("GPU mem:" , 0, 5);
  lcd.print("RAM mem:" , 0, 7);
  lcd.setFont(MediumFontRus);  
}

void draw_labels_2() {
  lcd.setFont(SmallFontRus);
  lcd.print("FANspeed"  , 0, 1);
  lcd.print("TMP1: "  , 0, 3);
  lcd.print("TMP2:"   , 64, 3);
  lcd.print("MOM:"    , 0, 5);
  lcd.print("HDDmx:" , 64, 5);
  lcd.print("UPTIME:" , 0, 7);
  lcd.setFont(MediumFontRus); 
}

/*void draw_legend() {
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
}*/

/*void draw_plot() {
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
}*/

void timeoutTick() {
  if ((millis() - timeout > 5000) && timeOut_flag) {
    lcd.clrScr();
    lcd.print("СВЯЗЬ"  , OLED_C, 2);
    lcd.print("ПОТЕРЯНА"     , OLED_C, 5);
    timeOut_flag = 0;
    reDraw_flag = 1;
  }
  if ((millis() - timeout > 25000) && sleep_flag) {
    lcd.clrScr();
    sleep_flag = 0;
    reDraw_flag = 1;
  }
}
void show_logo() {
  lcd.print("AlexGyver"  , OLED_C, 1);
  lcd.setFont(SmallFontRus);
  lcd.print("Technologies"    , OLED_C, 2);
  lcd.print("modified by"     , OLED_C, 5);
  lcd.setFont(MediumFontRus);
  lcd.print("nick7zmail"   , OLED_C, 7);
}
/*void debug() {
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
}*/
