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
extern uint8_t SmallFontRus[];                                         // Подключаем шрифт SmallFontRus.
extern uint8_t MediumFont[];                                           // Подключаем шрифт MediumFont. Если Вы желаете использовать Кириллицу, то нужно подключить шрифт MediumFontRus.
extern uint8_t Img_Logo[];                                             // Подключаем изображение Img_Logo из встроенных картинок.
                                                                       //
void setup(){                                                          //
    myOLED.begin();                                                    // Инициируем работу с дисплеем.
                                                                       //
    myOLED.drawImage(Img_Logo, 0, 30);                                 // Выводим картинку Img_Logo в позицию 0x30.
                                                                       //
    myOLED.setFont(MediumFont);                                        // Указываем шрифт который требуется использовать для вывода цифр и текста.
    myOLED.print("iarduino", OLED_R);                                  // Выводим текст "iarduino", выравнивая его по правому краю. Нижний край текста совпадёт с нижним краем предыдущей картинки.
    myOLED.setFont(SmallFontRus);                                      // Указываем шрифт который требуется использовать для вывода цифр и текста.
    myOLED.print("всё для радиолюбителя", 0, 41);                      // Выводим текст "всё для радиолюбителя".
}                                                                      //
void loop(){}                                                          //

