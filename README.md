[![AlexGyver YouTube](http://alexgyver.ru/git_banner.jpg)](https://www.youtube.com/channel/UCgtAOyEQdAyjvm9ATCi_Aig?sub_confirmation=1)
# Экран параметров железа ПК и автоматический реобас с RGB подсветкой
* [Описание проекта](#chapter-0)
* [Папки проекта](#chapter-1)
* [Схемы подключения](#chapter-2)
* [Материалы и компоненты](#chapter-3)
* [Как скачать и прошить](#chapter-4)
* [FAQ](#chapter-5)
* [Полезная информация](#chapter-6)

### Версии прошивки
- 1.3 от 15.12.2017 - добавлена установка скорости вентилятора при потере связи
- 1.4 от 28.01.2018
	* Исправлен баг с временем работы (спасибо Ивану Булгакову)
	* Добавлена настройка **выключать подсветку при потере связи**
	* Добавлена настройка **сбрасывать uptime при потере связи**
- 1.5 от 20.05.2018
	* Отображение показаний с внешних датчиков при потере связи (спасибо Ивану Булгакову)
- 18.07.2018 добавлена свежая сборка OHM с поддержкой Ryzen (спасибо Дмитрий Громов)
- 1.6 от 26.09.2018
	- Исправлены мелкие баги
	- Добавлена настройка ERROR_TEMP показывать температуру при потере связи

### Версии программы OHM
- 2019 - добавлена поддержка свежих процессоров от AMD и Intel, а также оптимизирована передача данных

<a id="chapter-0"></a>
## Описание проекта
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
  - Управление яркостью
- Подробности в видео: https://youtu.be/hCPOQI-eFYs
- **Поддержка двух типов дисплеев!** Подробности ниже
![DISPLAYS](https://github.com/AlexGyver/PCdisplay/blob/master/schemes%26PCBs/displays.jpg)

<a id="chapter-1"></a>
## Папки
**ВНИМАНИЕ! Если это твой первый опыт работы с Arduino, читай [инструкцию](#chapter-4)**
- **libraries** - библиотеки проекта. Заменить имеющиеся версии
- **firmware** - прошивка для Arduino, файл в папке открыть в Arduino IDE ([инструкция](#chapter-4))
- **HardwareMonitor** - программа, необходимая для работы устройства (закинуть куда угодно)
- **schemes&PCBs** - принципиальные схемы и печатки

<a id="chapter-2"></a>
## Схемы
![SCHEME](https://github.com/AlexGyver/PCdisplay/blob/master/schemes%26PCBs/scheme_fritzing.png)
![SCHEME](https://github.com/AlexGyver/PCdisplay/blob/master/schemes%26PCBs/scheme_EasyEDA.png)
### Исходники печаток на EasyEDA
* [Печатка версия с пинами](https://easyeda.com/editor#id=a6c8d85973e94f4ea4740a2bee59cc46)  
* [Печатка версия с терминалами](https://easyeda.com/editor#id=0d5864b51bac455bab4618c77582da25)  
* [Печатка мини версия с терминалами](https://easyeda.com/editor#id=328f6ff272be4d2ea978a43cd159fb93)  

<a id="chapter-3"></a>
## Материалы и компоненты
* Arduino NANO 328p – искать
* https://ali.ski/jLEx0
* https://ali.ski/9uVOe
* https://ali.ski/T_erQM
* https://ali.ski/4XIdAH
* Дисплей 2004 синий с драйвером https://ali.ski/Xe9La5
* Дисплей 1602 синий с драйвером https://ali.ski/jPj9PO
* Драйвер дисплея отдельно https://ali.ski/Nf_Mcg
* Дисплеи 1602 БЕЗ ДРАЙВЕРА (купить отдельно):
* Синий фон, белые буквы https://ali.ski/KU2GR
* Чёрный фон, рыжие буквы https://ali.ski/qpVrHn
* Чёрный фон, белые буквы https://ali.ski/vcFpF
* Серый фон, чёрные буквы https://ali.ski/ITdB98
* Провод https://ali.ski/S2sPx_
* Датчик температуры
* https://ali.ski/J6z88d
* https://ali.ski/RQHuvQ
* Лента светодиодная https://ali.ski/6CsQN
* Макетная плата https://ali.ski/j9EOuu
* Клеммники винтовые https://ali.ski/DX6D99
* 10 МОСФЕТов https://ali.ski/HshY9c
* 100 Ом https://ali.ski/uC8-pE
* 10 кОм https://ali.ski/uC8-pE
* Кнопки ищите на радиорынке, у китайцев можно 500 штук только купить
* Или тут смотрите https://alexgyver.ru/electronics/
* Или такие https://ali.ski/rPKjs7
* Корпус https://ali.ski/joq0OK https://ali.ski/Vl0kaR
РАССЫПУХА В РОССИИ

* Мосфет https://www.chipdip.ru/product/irf3205
* Или вот такой https://www.chipdip.ru/product/irfz24n
* 100 Ом https://www.chipdip.ru/product0/47324
* 10 кОм https://www.chipdip.ru/product0/41486
* Клеммник винтовой https://www.chipdip.ru/product/306-021-12
* Кнопка https://www.chipdip.ru/product/tyco-2-1825910-7-fsm14jh

## Вам скорее всего пригодится
* [Всё для пайки (паяльники и примочки)](http://alexgyver.ru/all-for-soldering/)
* [Недорогие инструменты](http://alexgyver.ru/my_instruments/)
* [Все существующие модули и сенсоры Arduino](http://alexgyver.ru/arduino_shop/)
* [Электронные компоненты](http://alexgyver.ru/electronics/)
* [Аккумуляторы и зарядные модули](http://alexgyver.ru/18650/)

<a id="chapter-4"></a>
## Как скачать и прошить
* [Первые шаги с Arduino](http://alexgyver.ru/arduino-first/) - ультра подробная статья по началу работы с Ардуино, ознакомиться первым делом!
* Скачать архив с проектом
> На главной странице проекта (где ты читаешь этот текст) вверху справа зелёная кнопка **Clone or download**, вот её жми, там будет **Download ZIP**
* Установить библиотеки в  
`C:\Program Files (x86)\Arduino\libraries\` (Windows x64)  
`C:\Program Files\Arduino\libraries\` (Windows x86)
* Подключить Ардуино к компьютеру
* Запустить файл прошивки (который имеет расширение .ino)
* Настроить IDE (COM порт, модель Arduino, как в статье выше)
* Настроить что нужно по проекту
* Нажать загрузить
* Пользоваться  
  
**Программа HardwareMonitorPlus**
* Запустить OpenHardwareMonitor.exe
* Options/Serial/Run - запуск соединения с Ардуиной
* Options/Serial/Config - настройка параметров работы
  - PORT address - адрес порта, куда подключена Ардуина
  - TEMP source - источник показаний температуры (процессор, видеокарта, максимум проц+видео, датчик 1, датчик 2)
  - FAN min, FAN max - минимальные и максимальные обороты вентиляторов, в %
  - TEMP min, TEMP max - минимальная и максимальная температура, в градусах Цельсия
  - Manual FAN - ручное управление скоростью вентилятора в %
  - Manual COLOR - ручное управление цветом ленты
  - LED brightness - управление яркостью ленты
  - CHART interval - интервал обновления графиков

## Настройки в коде
    #define DRIVER_VERSION 1    // 0 - маркировка драйвера кончается на 4АТ, 1 - на 4Т
    #define COLOR_ALGORITM 0    // 0 или 1 - разные алгоритмы изменения цвета (строка 222)
    #define ERROR_DUTY 90       // скорость вентиляторов при потере связи
    #define ERROR_BACKLIGHT 0   // 0 - гасить подсветку при потере сигнала, 1 - не гасить
	#define ERROR_UPTIME 0      // 1 - сбрасывать uptime при потере связи, 0 - нет

<a id="chapter-5"></a>
## FAQ
### Основные вопросы
В: Как скачать с этого грёбаного сайта?  
О: На главной странице проекта (где ты читаешь этот текст) вверху справа зелёная кнопка **Clone or download**, вот её жми, там будет **Download ZIP**

В: Скачался какой то файл .zip, куда его теперь?  
О: Это архив. Можно открыть стандартными средствами Windows, но думаю у всех на компьютере установлен WinRAR, архив нужно правой кнопкой и извлечь.

В: Я совсем новичок! Что мне делать с Ардуиной, где взять все программы?  
О: Читай и смотри видос http://alexgyver.ru/arduino-first/

В: Компьютер никак не реагирует на подключение Ардуины!  
О: Возможно у тебя зарядный USB кабель, а нужен именно data-кабель, по которому можно данные передавать

В: Ошибка! Скетч не компилируется!  
О: Путь к скетчу не должен содержать кириллицу. Положи его в корень диска.

В: Сколько стоит?  
О: Ничего не продаю.

### Вопросы по этому проекту

<a id="chapter-6"></a>
## Полезная информация
* [Мой сайт](http://alexgyver.ru/)
* [Основной YouTube канал](https://www.youtube.com/channel/UCgtAOyEQdAyjvm9ATCi_Aig?sub_confirmation=1)
* [YouTube канал про Arduino](https://www.youtube.com/channel/UC4axiS76D784-ofoTdo5zOA?sub_confirmation=1)
* [Мои видеоуроки по пайке](https://www.youtube.com/playlist?list=PLOT_HeyBraBuMIwfSYu7kCKXxQGsUKcqR)
* [Мои видеоуроки по Arduino](http://alexgyver.ru/arduino_lessons/)