//  Раскомментируйте для программной реализации шины I2C:              //
//  #define pin_SW_SDA 3                                               // Назначение любого вывода Arduino для работы в качестве линии SDA программной шины I2C.
//  #define pin_SW_SCL 9                                               // Назначение любого вывода Arduino для работы в качестве линии SCL программной шины I2C.
//  Раскомментируйте для совместимости с большинством плат:            //
//  #include <Wire.h>                                                  // Библиотека iarduino_OLED будет использовать методы и функции библиотеки Wire.
//  Ссылки для ознакомления:                                           //
//  Подробная информация о подключении модуля к шине I2C:              // http://wiki.iarduino.ru/page/i2c_connection/
//  Подробная информация о функциях и методах библиотеки:              // http://wiki.iarduino.ru/page/OLED_trema/
//  Библиотека  iarduino_OLED_txt (текстовая) экономит ОЗУ:            // http://iarduino.ru/file/341.html
//  Бибилиотека iarduino_OLED     (графическая):                       // http://iarduino.ru/file/340.html
                                                                       // 
#include <iarduino_OLED.h>                                             // Подключаем библиотеку iarduino_OLED.
iarduino_OLED  myOLED(0x3C);                                           // Объявляем объект myOLED, указывая адрес дисплея на шине I2C: 0x3C или 0x3D.
                                                                       //
extern uint8_t Img_Level_4[];                                          // Подключаем изображение Img_Level_4   из встроенных картинок.
extern uint8_t Img_Antenna[];                                          // Подключаем изображение Img_Antenna   из встроенных картинок.
extern uint8_t Img_Battery_0[];                                        // Подключаем изображение Img_Battery_0 из встроенных картинок.
extern uint8_t Img_Battery_1[];                                        // Подключаем изображение Img_Battery_1 из встроенных картинок.
extern uint8_t Img_Battery_2[];                                        // Подключаем изображение Img_Battery_2 из встроенных картинок.
extern uint8_t Img_Battery_3[];                                        // Подключаем изображение Img_Battery_3 из встроенных картинок.
extern uint8_t Img_BigBattery_low[];                                   // Подключаем изображение Img_BigBattery_low из встроенных картинок.
       uint8_t i=0;                                                    // Определяем переменную которая будет хранить номер от 0 до 3.
                                                                       //
void setup(){                                                          //
    myOLED.begin();                                                    // Инициируем работу с дисплеем.
    myOLED.drawImage(Img_Level_4       , 0     , 7     );              // Выводим картинку Img_Level_4, указав координату её левого нижнего угла 0x7.
    myOLED.drawImage(Img_Antenna       , 12    , 7     );              // Выводим картинку Img_Antenna, указав координату её левого нижнего угла 10x7.
    myOLED.drawImage(Img_BigBattery_low, OLED_C, OLED_C);              // Выводим картинку Img_BigBattery_low по центру экрана.
}                                                                      //
void loop(){                                                           //
    while(millis()%300==0){                                            // Выполняем код в теле оператора while через каждые 300 мс.
      i++; if(i>3){i=0;}                                               // Увеличиваем значение переменной i и обнуляем её если значение стало больше 3.
      switch(i){                                                       // Создаём анимацию из четырех картинок, меняя их по значению переменной i.
        case 0: myOLED.drawImage(Img_Battery_0, OLED_R,OLED_T); break; // Выводим изображение Img_Battery_0 в правом верхнем углу экрана.
        case 1: myOLED.drawImage(Img_Battery_1, OLED_R,OLED_T); break; // Выводим изображение Img_Battery_0 в правом верхнем углу экрана.
        case 2: myOLED.drawImage(Img_Battery_2, OLED_R,OLED_T); break; // Выводим изображение Img_Battery_0 в правом верхнем углу экрана.
        case 3: myOLED.drawImage(Img_Battery_3, OLED_R,OLED_T); break; // Выводим изображение Img_Battery_0 в правом верхнем углу экрана.
      }                                                                //
    }                                                                  //
}                                                                      //

