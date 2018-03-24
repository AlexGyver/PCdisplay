//  Раскомментируйте для программной реализации шины I2C:              //
//  #define pin_SW_SDA 3                                               // Назначение любого вывода Arduino для работы в качестве линии SDA программной шины I2C.
//  #define pin_SW_SCL 9                                               // Назначение любого вывода Arduino для работы в качестве линии SCL программной шины I2C.
//  Раскомментируйте для совместимости с большинством плат:            //
//  #include <Wire.h>                                                  // Библиотека iarduino_OLED_txt будет использовать методы и функции библиотеки Wire.
//  Ссылки для ознакомления:                                           //
//  Подробная информация о подключении модуля к шине I2C:              // http://wiki.iarduino.ru/page/i2c_connection/
//  Подробная информация о функциях и методах библиотеки:              // http://wiki.iarduino.ru/page/OLED_trema/
//  Библиотека  iarduino_OLED_txt (текстовая) экономит ОЗУ:            // http://iarduino.ru/file/341.html
//  Бибилиотека iarduino_OLED     (графическая):                       // http://iarduino.ru/file/340.html
                                                                       // 
#include <iarduino_OLED_txt.h>                                         // Подключаем библиотеку iarduino_OLED_txt.
iarduino_OLED_txt myOLED(0x3C);                                        // Объявляем объект myOLED, указывая адрес дисплея на шине I2C: 0x3C или 0x3D.
                                                                       //
extern uint8_t SmallFontRus[];                                         // Подключаем шрифт SmallFontRus.
                                                                       // Если Вы не используете Кириллицу, то лучше подключить шрифт SmallFont, он займет меньше места в памяти программ.
void setup(){                                                          //
    myOLED.begin();                                                    // Инициируем работу с дисплеем.
    myOLED.setFont(SmallFontRus);                                      // Указываем шрифт который требуется использовать для вывода цифр и текста.
    myOLED.invText();                                                  // Указываем что цвет текста выводимого после данной функции должен быть инвертирован.
    myOLED.print("*** iarduino.ru ***", OLED_C, 3);                    // Выводим текст по центру 3 строки. Текст будет написан чёрными буквами на белом фоне.
    myOLED.invText(false);                                             // Указываем что цвет текста выводимого после данной функции не требуется инвертировать.
    myOLED.print("*** iarduino.ru ***", OLED_C, 5);                    // Выводим текст по центру 5 строки. Текст будет написан белыми буквами на чёрном фоне.
}                                                                      //
                                                                       //
void loop(){}                                                          //