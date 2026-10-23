<?xml version="1.0" encoding="utf-8"?>
<!DOCTYPE TS>
<TS version="2.1" language="ru_RU">
<context>
    <name>AlgoCatalog</name>
    <message>
        <location filename="../src/flash/algocatalog.cpp" line="453" />
        <source>Keil built-in algorithms</source>
        <translation>Встроенные алгоритмы Keil</translation>
    </message>
    <message>
        <location filename="../src/flash/algocatalog.cpp" line="459" />
        <source>Additional folder</source>
        <translation>Дополнительная папка</translation>
    </message>
    <message>
        <location filename="../src/flash/algocatalog.cpp" line="597" />
        <source>Flash Algorithms (*.flm *.FLM *.flx *.xldr);;All Files (*.*)</source>
        <translation>Алгоритмы программирования (*.flm *.FLM *.flx *.xldr);;Все файлы (*.*)</translation>
    </message>
</context>
<context>
    <name>AlgorithmPicker</name>
    <message>
        <location filename="../src/algorithmpicker.cpp" line="135" />
        <location filename="../src/algorithmpicker.cpp" line="380" />
        <source>Filter algorithms ...</source>
        <translation>Фильтр алгоритмов...</translation>
    </message>
    <message>
        <location filename="../src/algorithmpicker.cpp" line="368" />
        <source>Select algorithm ...</source>
        <translation>Выбор алгоритма...</translation>
    </message>
</context>
<context>
    <name>CmsisDap</name>
    <message>
        <location filename="../src/dap/cmsisdap.cpp" line="195" />
        <source>No probe is open.</source>
        <translation>Нет открытого пробника.</translation>
    </message>
    <message>
        <location filename="../src/dap/cmsisdap.cpp" line="206" />
        <source>The probe sent an empty answer.</source>
        <translation>Пробник вернул пустой ответ.</translation>
    </message>
    <message>
        <location filename="../src/dap/cmsisdap.cpp" line="234" />
        <source>The probe answered with a malformed DAP_Info response.</source>
        <translation>Пробник вернул некорректный ответ DAP_Info.</translation>
    </message>
    <message>
        <location filename="../src/dap/cmsisdap.cpp" line="245" />
        <source>The probe sent a truncated DAP_Info response.</source>
        <translation>Пробник вернул обрезанный ответ DAP_Info.</translation>
    </message>
    <message>
        <location filename="../src/dap/cmsisdap.cpp" line="299" />
        <source>The probe answered with a malformed DAP_Connect response.</source>
        <translation>Пробник вернул некорректный ответ DAP_Connect.</translation>
    </message>
    <message>
        <location filename="../src/dap/cmsisdap.cpp" line="305" />
        <source>The probe refused the SWD connection.</source>
        <translation>Пробник отклонил подключение по SWD.</translation>
    </message>
    <message>
        <location filename="../src/dap/cmsisdap.cpp" line="378" />
        <source>The DP did not acknowledge the power up request within %1 ms.</source>
        <translation>DP не подтвердил запрос включения питания за %1 мс.</translation>
    </message>
    <message>
        <location filename="../src/dap/cmsisdap.cpp" line="425" />
        <source>Setting the SWD clock failed.</source>
        <translation>Не удалось установить частоту SWD.</translation>
    </message>
    <message>
        <location filename="../src/dap/cmsisdap.cpp" line="448" />
        <source>Configuring the DAP transfer failed.</source>
        <translation>Не удалось настроить передачу DAP.</translation>
    </message>
    <message>
        <location filename="../src/dap/cmsisdap.cpp" line="468" />
        <source>The probe answered with a malformed DAP_SWJ_Pins response.</source>
        <translation>Пробник вернул некорректный ответ DAP_SWJ_Pins.</translation>
    </message>
    <message>
        <location filename="../src/dap/cmsisdap.cpp" line="488" />
        <source>The probe answered with a malformed DAP_Reset_Target response.</source>
        <translation>Пробник вернул некорректный ответ DAP_Reset_Target.</translation>
    </message>
    <message>
        <location filename="../src/dap/cmsisdap.cpp" line="584" />
        <source>The SWJ sequence failed.</source>
        <translation>Сбой последовательности SWJ.</translation>
    </message>
    <message>
        <location filename="../src/dap/cmsisdap.cpp" line="619" />
        <source>SWD clock: %1</source>
        <translation>Частота SWD: %1</translation>
    </message>
    <message>
        <location filename="../src/dap/cmsisdap.cpp" line="624" />
        <source>The SWD clock could not be set: %1</source>
        <translation>Не удалось установить частоту SWD: %1</translation>
    </message>
    <message>
        <location filename="../src/dap/cmsisdap.cpp" line="649" />
        <source>The SWD DP did not answer, trying to revive the link.</source>
        <translation>DP SWD не отвечает, попытка восстановить связь.</translation>
    </message>
    <message>
        <location filename="../src/dap/cmsisdap.cpp" line="706" />
        <source>The SWD link came back at %1 Hz.</source>
        <translation>Связь SWD восстановлена на частоте %1 Гц.</translation>
    </message>
    <message>
        <location filename="../src/dap/cmsisdap.cpp" line="725" />
        <source>The SWD DP does not answer. Check the wiring (SWDIO, SWCLK, GND), the target power supply and the SWD clock. A target program that switched the debug interface off, or that put the core into a low power state, only comes back on a power cycle. If it comes back with BOOT0 held high, that program is what takes the debug pins over: start the ROM bootloader that way, which leaves the debug pins alone, and erase the chip from there.</source>
        <translation>DP SWD не отвечает. Проверьте подключение (SWDIO, SWCLK, GND), питание цели и частоту SWD. Целевая программа, отключившая отладочный интерфейс или переведшая ядро в режим пониженного энергопотребления, восстанавливается только после цикла питания. Если она восстанавливается при удерживаемом высоком уровне BOOT0, значит именно она забирает отладочные выводы: запустите так загрузчик из ПЗУ, который не трогает отладочные выводы, и сотрите чип оттуда.</translation>
    </message>
    <message>
        <location filename="../src/dap/cmsisdap.cpp" line="757" />
        <source>This probe does not drive an nRESET line.</source>
        <translation>Этот пробник не управляет линией nRESET.</translation>
    </message>
    <message>
        <location filename="../src/dap/cmsisdap.cpp" line="764" />
        <source>Holding nRESET low and connecting under reset.</source>
        <translation>Удержание nRESET в низком уровне и подключение под сбросом.</translation>
    </message>
    <message>
        <location filename="../src/dap/cmsisdap.cpp" line="799" />
        <source>The SWD link came back under reset at %1 Hz.</source>
        <translation>Связь SWD восстановлена под сбросом на частоте %1 Гц.</translation>
    </message>
    <message>
        <location filename="../src/dap/cmsisdap.cpp" line="813" />
        <source>Connecting under reset did not bring the DP back either.</source>
        <translation>Подключение под сбросом тоже не восстановило DP.</translation>
    </message>
    <message>
        <location filename="../src/dap/cmsisdap.cpp" line="818" />
        <source>The SWD DP did not answer while nRESET was being driven.</source>
        <translation>DP SWD не отвечал при активной линии nRESET.</translation>
    </message>
    <message>
        <location filename="../src/dap/cmsisdap.cpp" line="832" />
        <source>The SWD DP reported an invalid IDCODE.</source>
        <translation>DP SWD сообщил недопустимый IDCODE.</translation>
    </message>
    <message>
        <location filename="../src/dap/cmsisdap.cpp" line="873" />
        <source>The DP read returned no data.</source>
        <translation>Чтение DP не вернуло данных.</translation>
    </message>
    <message>
        <location filename="../src/dap/cmsisdap.cpp" line="902" />
        <source>The AP read returned no data.</source>
        <translation>Чтение AP не вернуло данных.</translation>
    </message>
    <message>
        <location filename="../src/dap/cmsisdap.cpp" line="945" />
        <source>Empty transfer.</source>
        <translation>Пустая передача.</translation>
    </message>
    <message>
        <location filename="../src/dap/cmsisdap.cpp" line="959" />
        <source>Internal error: the write value count does not match the request count.</source>
        <translation>Внутренняя ошибка: число записываемых значений не совпадает с числом в запросе.</translation>
    </message>
    <message>
        <location filename="../src/dap/cmsisdap.cpp" line="980" />
        <source>The probe answered with a truncated transfer response.</source>
        <translation>Пробник вернул обрезанный ответ передачи.</translation>
    </message>
    <message>
        <location filename="../src/dap/cmsisdap.cpp" line="990" />
        <source>The probe answered with a malformed DAP_Transfer response.</source>
        <translation>Пробник вернул некорректный ответ DAP_Transfer.</translation>
    </message>
    <message>
        <location filename="../src/dap/cmsisdap.cpp" line="1009" />
        <source>SWD fault (the target did not acknowledge the access).</source>
        <translation>Ошибка SWD (цель не подтвердила доступ).</translation>
    </message>
    <message>
        <location filename="../src/dap/cmsisdap.cpp" line="1015" />
        <source>Protocol error during the SWD transfer.</source>
        <translation>Ошибка протокола при передаче SWD.</translation>
    </message>
    <message>
        <location filename="../src/dap/cmsisdap.cpp" line="1021" />
        <source>The probe performed %1 of %2 transfers.</source>
        <translation>Пробник выполнил %1 из %2 передач.</translation>
    </message>
    <message>
        <location filename="../src/dap/cmsisdap.cpp" line="1030" />
        <source>The probe sent a truncated transfer payload.</source>
        <translation>Пробник передал обрезанные данные передачи.</translation>
    </message>
    <message>
        <location filename="../src/dap/cmsisdap.cpp" line="1040" />
        <source>The SWD transfer was retried too often (WAIT response).</source>
        <translation>Слишком много повторов передачи SWD (ответ WAIT).</translation>
    </message>
    <message>
        <location filename="../src/dap/cmsisdap.cpp" line="1051" />
        <source>Empty block transfer.</source>
        <translation>Пустая блочная передача.</translation>
    </message>
    <message>
        <location filename="../src/dap/cmsisdap.cpp" line="1078" />
        <source>The probe answered with a truncated block transfer response.</source>
        <translation>Пробник вернул обрезанный ответ блочной передачи.</translation>
    </message>
    <message>
        <location filename="../src/dap/cmsisdap.cpp" line="1084" />
        <source>The probe answered with a malformed DAP_TransferBlock response.</source>
        <translation>Пробник вернул некорректный ответ DAP_TransferBlock.</translation>
    </message>
    <message>
        <location filename="../src/dap/cmsisdap.cpp" line="1098" />
        <source>SWD fault during the block transfer.</source>
        <translation>Ошибка SWD при блочной передаче.</translation>
    </message>
    <message>
        <location filename="../src/dap/cmsisdap.cpp" line="1104" />
        <source>Protocol error during the block transfer.</source>
        <translation>Ошибка протокола при блочной передаче.</translation>
    </message>
    <message>
        <location filename="../src/dap/cmsisdap.cpp" line="1110" />
        <source>The probe performed %1 of %2 block transfers.</source>
        <translation>Пробник выполнил %1 из %2 блочных передач.</translation>
    </message>
    <message>
        <location filename="../src/dap/cmsisdap.cpp" line="1119" />
        <source>The probe sent a truncated block transfer payload.</source>
        <translation>Пробник передал обрезанные данные блочной передачи.</translation>
    </message>
    <message>
        <location filename="../src/dap/cmsisdap.cpp" line="1297" />
        <source>The probe answered a poll with no data.</source>
        <translation>Пробник ответил на опрос без данных.</translation>
    </message>
</context>
<context>
    <name>CortexM</name>
    <message>
        <location filename="../src/target/cortexm.cpp" line="78" />
        <source>Unknown Cortex-M core</source>
        <translation>Неизвестное ядро Cortex-M</translation>
    </message>
    <message>
        <location filename="../src/target/cortexm.cpp" line="101" />
        <source>No Cortex-M core answered at CPUID (0x%1).</source>
        <translation>Ядро Cortex-M не ответило по CPUID (0x%1).</translation>
    </message>
    <message>
        <location filename="../src/target/cortexm.cpp" line="302" />
        <source>The core is locked up (the flash algorithm crashed).</source>
        <translation>Ядро заблокировано (алгоритм программирования отказал).</translation>
    </message>
    <message>
        <location filename="../src/target/cortexm.cpp" line="320" />
        <source>The core did not halt within %1 ms.</source>
        <translation>Ядро не остановилось за %1 мс.</translation>
    </message>
    <message>
        <location filename="../src/target/cortexm.cpp" line="460" />
        <source>The probe has no nRESET line; use the software reset instead.</source>
        <translation>У пробника нет линии nRESET; используйте программный сброс.</translation>
    </message>
    <message>
        <location filename="../src/target/cortexm.cpp" line="578" />
        <source>The core register transfer timed out.</source>
        <translation>Тайм-аут передачи регистров ядра.</translation>
    </message>
    <message>
        <location filename="../src/target/cortexm.cpp" line="585" />
        <source>Internal error: no register buffer.</source>
        <translation>Внутренняя ошибка: нет буфера регистров.</translation>
    </message>
</context>
<context>
    <name>DapTransport</name>
    <message>
        <location filename="../src/dap/daptransport.cpp" line="123" />
        <source>unknown HID error</source>
        <translation>неизвестная ошибка HID</translation>
    </message>
    <message>
        <location filename="../src/dap/daptransport.cpp" line="169" />
        <source>The HID probe could not be opened. It is probably used by another program (Keil, OpenOCD, a debug session).</source>
        <translation>Не удалось открыть пробник HID. Вероятно, его использует другая программа (Keil, OpenOCD, отладочная сессия).</translation>
    </message>
    <message>
        <location filename="../src/dap/daptransport.cpp" line="206" />
        <location filename="../src/dap/probes.cpp" line="321" />
        <source>USB HID (CMSIS-DAP v1)</source>
        <translation>USB HID (CMSIS-DAP v1)</translation>
    </message>
    <message>
        <location filename="../src/dap/daptransport.cpp" line="228" />
        <source>HID write failed: %1</source>
        <translation>Ошибка записи HID: %1</translation>
    </message>
    <message>
        <location filename="../src/dap/daptransport.cpp" line="249" />
        <source>HID read failed: %1</source>
        <translation>Ошибка чтения HID: %1</translation>
    </message>
    <message>
        <location filename="../src/dap/daptransport.cpp" line="254" />
        <location filename="../src/dap/daptransport.cpp" line="588" />
        <source>The probe did not answer (timeout).</source>
        <translation>Пробник не ответил (тайм-аут).</translation>
    </message>
    <message>
        <location filename="../src/dap/daptransport.cpp" line="321" />
        <source>The USB (libusb) layer could not be initialised.</source>
        <translation>Не удалось инициализировать слой USB (libusb).</translation>
    </message>
    <message>
        <location filename="../src/dap/daptransport.cpp" line="328" />
        <source>USB enumeration failed.</source>
        <translation>Сбой перечисления USB-устройств.</translation>
    </message>
    <message>
        <location filename="../src/dap/daptransport.cpp" line="343" />
        <source>The USB probe was not found any more. Replug it and refresh the list.</source>
        <translation>USB-пробник больше не найден. Подключите его заново и обновите список.</translation>
    </message>
    <message>
        <location filename="../src/dap/daptransport.cpp" line="364" />
        <source>Access to the USB probe was denied. It is either used by another program, or no WinUSB driver is bound to it.</source>
        <translation>Доступ к USB-пробнику запрещён. Либо его использует другая программа, либо для него не привязан драйвер WinUSB.</translation>
    </message>
    <message>
        <location filename="../src/dap/daptransport.cpp" line="369" />
        <source>The probe is not bound to a WinUSB (or libusbK) driver. Install the driver, or select the same probe over its USB HID (CMSIS-DAP v1) interface.</source>
        <translation>Для пробника не привязан драйвер WinUSB (или libusbK). Установите драйвер или выберите тот же пробник через его интерфейс USB HID (CMSIS-DAP v1).</translation>
    </message>
    <message>
        <location filename="../src/dap/daptransport.cpp" line="375" />
        <source>Opening the USB probe failed: %1</source>
        <translation>Не удалось открыть USB-пробник: %1</translation>
    </message>
    <message>
        <location filename="../src/dap/daptransport.cpp" line="387" />
        <source>Claiming the USB interface failed: %1</source>
        <translation>Не удалось захватить интерфейс USB: %1</translation>
    </message>
    <message>
        <location filename="../src/dap/daptransport.cpp" line="512" />
        <source>CMSIS-DAP bulk transport, spec framing (no length prefix)</source>
        <translation>Транспорт CMSIS-DAP bulk, кадры по спецификации (без префикса длины)</translation>
    </message>
    <message>
        <location filename="../src/dap/daptransport.cpp" line="517" />
        <source>CMSIS-DAP bulk transport, length-prefixed framing (2 byte prefix)</source>
        <translation>Транспорт CMSIS-DAP bulk, кадры с префиксом длины (префикс 2 байта)</translation>
    </message>
    <message>
        <location filename="../src/dap/daptransport.cpp" line="559" />
        <source>USB bulk write failed: %1</source>
        <translation>Ошибка записи USB bulk: %1</translation>
    </message>
    <message>
        <location filename="../src/dap/daptransport.cpp" line="592" />
        <source>USB bulk read failed: %1</source>
        <translation>Ошибка чтения USB bulk: %1</translation>
    </message>
    <message>
        <location filename="../src/dap/daptransport.cpp" line="601" />
        <location filename="../src/dap/daptransport.cpp" line="611" />
        <source>The probe sent a malformed packet.</source>
        <translation>Пробник передал некорректный пакет.</translation>
    </message>
    <message>
        <location filename="../src/dap/daptransport.cpp" line="683" />
        <source>The probe did not answer DAP_Info. Check that the right interface of the device was selected and that the firmware runs a CMSIS-DAP implementation.</source>
        <translation>Пробник не ответил на DAP_Info. Проверьте, что выбран нужный интерфейс устройства и что прошивка реализует CMSIS-DAP.</translation>
    </message>
    <message>
        <location filename="../src/dap/daptransport.cpp" line="723" />
        <source>The command does not fit into one packet (%1 &gt; %2 bytes).</source>
        <translation>Команда не помещается в один пакет (%1 &gt; %2 байт).</translation>
    </message>
    <message>
        <location filename="../src/dap/probes.cpp" line="320" />
        <source>USB bulk (WinUSB, CMSIS-DAP v2)</source>
        <translation>USB bulk (WinUSB, CMSIS-DAP v2)</translation>
    </message>
</context>
<context>
    <name>ElfFile</name>
    <message>
        <location filename="../src/flash/elffile.cpp" line="61" />
        <source>The algorithm file could not be opened: %1</source>
        <translation>Не удалось открыть файл алгоритма: %1</translation>
    </message>
    <message>
        <location filename="../src/flash/elffile.cpp" line="69" />
        <source>The file is too small to be an ELF image.</source>
        <translation>Файл слишком мал для образа ELF.</translation>
    </message>
    <message>
        <location filename="../src/flash/elffile.cpp" line="74" />
        <source>The file is not an ELF image (a Keil .FLM algorithm is expected).</source>
        <translation>Файл не является образом ELF (ожидается алгоритм Keil .FLM).</translation>
    </message>
    <message>
        <location filename="../src/flash/elffile.cpp" line="79" />
        <source>Only 32 bit ELF images are supported (this one is 64 bit).</source>
        <translation>Поддерживаются только 32-битные образы ELF (этот — 64-битный).</translation>
    </message>
    <message>
        <location filename="../src/flash/elffile.cpp" line="84" />
        <source>Only little endian ELF images are supported.</source>
        <translation>Поддерживаются только образы ELF с порядком байтов little endian.</translation>
    </message>
    <message>
        <location filename="../src/flash/elffile.cpp" line="90" />
        <source>The image is not an ARM image (e_machine = %1).</source>
        <translation>Образ не является образом ARM (e_machine = %1).</translation>
    </message>
    <message>
        <location filename="../src/flash/elffile.cpp" line="123" />
        <source>The image is truncated (segment %1 leaves the file).</source>
        <translation>Образ обрезан (сегмент %1 выходит за пределы файла).</translation>
    </message>
    <message>
        <location filename="../src/flash/elffile.cpp" line="133" />
        <source>The image carries no loadable segment.</source>
        <translation>Образ не содержит загружаемых сегментов.</translation>
    </message>
</context>
<context>
    <name>FlashAlgo</name>
    <message>
        <location filename="../src/flash/flashalgo.cpp" line="162" />
        <source>The algorithm does not export an Init() function.</source>
        <translation>Алгоритм не экспортирует функцию Init().</translation>
    </message>
    <message>
        <location filename="../src/flash/flashalgo.cpp" line="168" />
        <source>The algorithm does not export a ProgramPage() function.</source>
        <translation>Алгоритм не экспортирует функцию ProgramPage().</translation>
    </message>
    <message>
        <location filename="../src/flash/flashalgo.cpp" line="174" />
        <source>The algorithm does not export EraseSector() nor EraseChip().</source>
        <translation>Алгоритм не экспортирует ни EraseSector(), ни EraseChip().</translation>
    </message>
    <message>
        <location filename="../src/flash/flashalgo.cpp" line="209" />
        <source>The FlashDevice structure could not be located in the algorithm.</source>
        <translation>Структура FlashDevice не найдена в алгоритме.</translation>
    </message>
    <message>
        <location filename="../src/flash/flashalgo.cpp" line="217" />
        <source>The FlashDevice structure at offset 0x%1 is not valid.</source>
        <translation>Структура FlashDevice по смещению 0x%1 некорректна.</translation>
    </message>
    <message>
        <location filename="../src/flash/flashalgo.cpp" line="300" />
        <source>The algorithm carries an empty sector table.</source>
        <translation>Алгоритм содержит пустую таблицу секторов.</translation>
    </message>
    <message>
        <location filename="../src/flash/flashalgo.cpp" line="339" />
        <location filename="../src/flash/flashalgo.cpp" line="347" />
        <source>Unknown</source>
        <translation>Неизвестно</translation>
    </message>
    <message>
        <location filename="../src/flash/flashalgo.cpp" line="340" />
        <source>On-chip flash</source>
        <translation>Встроенная флеш-память</translation>
    </message>
    <message>
        <location filename="../src/flash/flashalgo.cpp" line="341" />
        <source>External flash, 8 bit bus</source>
        <translation>Внешняя флеш-память, 8-битная шина</translation>
    </message>
    <message>
        <location filename="../src/flash/flashalgo.cpp" line="342" />
        <source>External flash, 16 bit bus</source>
        <translation>Внешняя флеш-память, 16-битная шина</translation>
    </message>
    <message>
        <location filename="../src/flash/flashalgo.cpp" line="343" />
        <source>External flash, 32 bit bus</source>
        <translation>Внешняя флеш-память, 32-битная шина</translation>
    </message>
    <message>
        <location filename="../src/flash/flashalgo.cpp" line="344" />
        <source>External SPI flash</source>
        <translation>Внешняя SPI флеш-память</translation>
    </message>
</context>
<context>
    <name>FlashWorker</name>
    <message>
        <location filename="../src/flashworker.cpp" line="72" />
        <source>Loading the flash algorithm ...</source>
        <translation>Загрузка алгоритма программирования...</translation>
    </message>
    <message>
        <location filename="../src/flashworker.cpp" line="81" />
        <source>Algorithm %1: %2, %3, page %4 bytes, %5 sectors</source>
        <translation>Алгоритм %1: %2, %3, страница %4 байт, секторов %5</translation>
    </message>
    <message>
        <location filename="../src/flashworker.cpp" line="104" />
        <source>%1: %2 in %3 blocks, 0x%4 - 0x%5, checksum 0x%6</source>
        <translation>%1: %2 в %3 блоках, 0x%4 - 0x%5, контрольная сумма 0x%6</translation>
    </message>
    <message>
        <location filename="../src/flashworker.cpp" line="105" />
        <source>Intel HEX</source>
        <translation>Intel HEX</translation>
    </message>
    <message>
        <location filename="../src/flashworker.cpp" line="105" />
        <source>BIN</source>
        <translation>BIN</translation>
    </message>
    <message>
        <location filename="../src/flashworker.cpp" line="112" />
        <source>Firmware %1</source>
        <translation>Прошивка %1</translation>
    </message>
    <message>
        <location filename="../src/flashworker.cpp" line="117" />
        <source>The firmware (0x%1 - 0x%2) does not fit into the flash of the algorithm (0x%3 - 0x%4). Check the loading address.</source>
        <translation>Прошивка (0x%1 - 0x%2) не помещается во флеш-память алгоритма (0x%3 - 0x%4). Проверьте адрес загрузки.</translation>
    </message>
    <message>
        <location filename="../src/flashworker.cpp" line="127" />
        <source>The algorithm has no sector at 0x%1.</source>
        <translation>В алгоритме нет сектора по адресу 0x%1.</translation>
    </message>
    <message>
        <location filename="../src/flashworker.cpp" line="132" />
        <source>No firmware given: the whole chip is erased and the target is left halted.</source>
        <translation>Прошивка не указана: весь чип стирается, цель остаётся остановленной.</translation>
    </message>
    <message>
        <location filename="../src/flashworker.cpp" line="137" />
        <location filename="../src/flashworker.cpp" line="244" />
        <location filename="../src/flashworker.cpp" line="245" />
        <source>Cancelled.</source>
        <translation>Отменено.</translation>
    </message>
    <message>
        <location filename="../src/flashworker.cpp" line="155" />
        <source>Probe: %1</source>
        <translation>Пробник: %1</translation>
    </message>
    <message>
        <location filename="../src/flashworker.cpp" line="159" />
        <source>CMSIS-DAP firmware: %1</source>
        <translation>Прошивка CMSIS-DAP: %1</translation>
    </message>
    <message>
        <location filename="../src/flashworker.cpp" line="183" />
        <source>Target: %1 (CPUID 0x%2, DP IDCODE 0x%3)</source>
        <translation>Цель: %1 (CPUID 0x%2, DP IDCODE 0x%3)</translation>
    </message>
    <message>
        <location filename="../src/flashworker.cpp" line="254" />
        <source>The whole chip was erased in %1 s; the target is left halted.</source>
        <translation>Весь чип стёрт за %1 с; цель остаётся остановленной.</translation>
    </message>
    <message>
        <location filename="../src/flashworker.cpp" line="257" />
        <source>Programming finished in %1 s: %2 pages, %3 bytes, %4 sectors erased.</source>
        <translation>Программирование завершено за %1 с: страниц %2, байт %3, стёрто секторов %4.</translation>
    </message>
    <message>
        <location filename="../src/flashworker.cpp" line="264" />
        <source>The algorithm used 0x%1 of RAM at %2.</source>
        <translation>Алгоритм использовал 0x%1 ОЗУ по адресу %2.</translation>
    </message>
    <message>
        <location filename="../src/flashworker.cpp" line="300" />
        <source>The SWD link failed at %1; retrying at %2.</source>
        <translation>Связь SWD потеряна на частоте %1; повтор на %2.</translation>
    </message>
    <message>
        <location filename="../src/flashworker.cpp" line="306" />
        <source>Retrying at a slower clock ...</source>
        <translation>Повтор на более низкой частоте...</translation>
    </message>
</context>
<context>
    <name>Flasher</name>
    <message>
        <location filename="../src/flash/flasher.cpp" line="110" />
        <source>The algorithm image is empty.</source>
        <translation>Образ алгоритма пуст.</translation>
    </message>
    <message>
        <location filename="../src/flash/flasher.cpp" line="116" />
        <source>The algorithm reports an unusable page size (%1 bytes).</source>
        <translation>Алгоритм сообщает непригодный размер страницы (%1 байт).</translation>
    </message>
    <message>
        <location filename="../src/flash/flasher.cpp" line="128" />
        <source>The RAM base address is too high for the algorithm.</source>
        <translation>Базовый адрес ОЗУ слишком высок для алгоритма.</translation>
    </message>
    <message>
        <location filename="../src/flash/flasher.cpp" line="230" />
        <source>The algorithm could not be written to the target RAM at %1: %2 of %3 bytes came back different, the first one at %4 (wrote 0x%5, read 0x%6). The base address has to be RAM the debug unit can write to; if it is, the probe did not move the data reliably.</source>
        <translation>Не удалось записать алгоритм в ОЗУ цели по адресу %1: %2 из %3 байт не совпали, первый — по адресу %4 (записано 0x%5, прочитано 0x%6). Базовый адрес должен указывать на ОЗУ, доступное для записи отладочному блоку; если это так, пробник передал данные ненадёжно.</translation>
    </message>
    <message>
        <location filename="../src/flash/flasher.cpp" line="260" />
        <source>The algorithm does not provide %1().</source>
        <translation>Алгоритм не предоставляет %1().</translation>
    </message>
    <message>
        <location filename="../src/flash/flasher.cpp" line="293" />
        <source>%1() did not return: %2 (PC 0x%3, DFSR 0x%4, CFSR 0x%5, HFSR 0x%6)</source>
        <translation>%1() не вернула управление: %2 (PC 0x%3, DFSR 0x%4, CFSR 0x%5, HFSR 0x%6)</translation>
    </message>
    <message>
        <location filename="../src/flash/flasher.cpp" line="313" />
        <source>%1() stopped at 0x%2 instead of returning to 0x%3 (DFSR = 0x%4).</source>
        <translation>%1() остановилась на 0x%2 вместо возврата в 0x%3 (DFSR = 0x%4).</translation>
    </message>
    <message>
        <location filename="../src/flash/flasher.cpp" line="340" />
        <source>Erasing the whole chip ...</source>
        <translation>Стирание всего чипа...</translation>
    </message>
    <message>
        <location filename="../src/flash/flasher.cpp" line="351" />
        <source>EraseChip() failed with code %1.</source>
        <translation>EraseChip() завершилась с кодом %1.</translation>
    </message>
    <message>
        <location filename="../src/flash/flasher.cpp" line="356" />
        <source>Chip erase done.</source>
        <translation>Стирание чипа выполнено.</translation>
    </message>
    <message>
        <location filename="../src/flash/flasher.cpp" line="361" />
        <source>The algorithm provides neither EraseChip() nor EraseSector().</source>
        <translation>Алгоритм не предоставляет ни EraseChip(), ни EraseSector().</translation>
    </message>
    <message>
        <location filename="../src/flash/flasher.cpp" line="376" />
        <source>Address 0x%1 is not covered by the sector table of the algorithm.</source>
        <translation>Адрес 0x%1 не покрыт таблицей секторов алгоритма.</translation>
    </message>
    <message>
        <location filename="../src/flash/flasher.cpp" line="396" />
        <source>Nothing to erase.</source>
        <translation>Нечего стирать.</translation>
    </message>
    <message>
        <location filename="../src/flash/flasher.cpp" line="402" />
        <location filename="../src/flash/flasher.cpp" line="471" />
        <location filename="../src/flash/flasher.cpp" line="585" />
        <source>Cancelled.</source>
        <translation>Отменено.</translation>
    </message>
    <message>
        <location filename="../src/flash/flasher.cpp" line="411" />
        <source>Erasing sector %1 of %2 (0x%3) ...</source>
        <translation>Стирание сектора %1 из %2 (0x%3)...</translation>
    </message>
    <message>
        <location filename="../src/flash/flasher.cpp" line="425" />
        <source>Erasing the sector at 0x%1 failed with code %2.</source>
        <translation>Стирание сектора по адресу 0x%1 завершилось с кодом %2.</translation>
    </message>
    <message>
        <location filename="../src/flash/flasher.cpp" line="455" />
        <source>The firmware file carries no data.</source>
        <translation>Файл прошивки не содержит данных.</translation>
    </message>
    <message>
        <location filename="../src/flash/flasher.cpp" line="507" />
        <source>Programming 0x%1 (%2 / %3 bytes) ...</source>
        <translation>Программирование 0x%1 (%2 / %3 байт)...</translation>
    </message>
    <message>
        <location filename="../src/flash/flasher.cpp" line="533" />
        <source>Programming the page at 0x%1 failed with code %2.</source>
        <translation>Ошибка программирования страницы по адресу 0x%1, код %2.</translation>
    </message>
    <message>
        <location filename="../src/flash/flasher.cpp" line="569" />
        <source>The algorithm has no Verify() routine; the flash is read back through the debug port instead.</source>
        <translation>В алгоритме нет процедуры Verify(); флеш-память считывается через отладочный порт.</translation>
    </message>
    <message>
        <location filename="../src/flash/flasher.cpp" line="605" />
        <source>Verifying 0x%1 ...</source>
        <translation>Проверка 0x%1...</translation>
    </message>
    <message>
        <location filename="../src/flash/flasher.cpp" line="622" />
        <source>Verification failed at 0x%1 (code %2).</source>
        <translation>Ошибка проверки по адресу 0x%1 (код %2).</translation>
    </message>
    <message>
        <location filename="../src/flash/flasher.cpp" line="630" />
        <source>Reading back 0x%1 failed; the verification was skipped.</source>
        <translation>Не удалось прочитать 0x%1; проверка пропущена.</translation>
    </message>
    <message>
        <location filename="../src/flash/flasher.cpp" line="644" />
        <source>Verification failed at 0x%1: expected 0x%2, read 0x%3.</source>
        <translation>Ошибка проверки по адресу 0x%1: ожидалось 0x%2, прочитано 0x%3.</translation>
    </message>
    <message>
        <location filename="../src/flash/flasher.cpp" line="671" />
        <source>No flash algorithm is selected.</source>
        <translation>Алгоритм программирования не выбран.</translation>
    </message>
    <message>
        <location filename="../src/flash/flasher.cpp" line="681" />
        <source>No firmware data to program.</source>
        <translation>Нет данных прошивки для записи.</translation>
    </message>
    <message>
        <location filename="../src/flash/flasher.cpp" line="693" />
        <source>Preparing ...</source>
        <translation>Подготовка...</translation>
    </message>
    <message>
        <location filename="../src/flash/flasher.cpp" line="696" />
        <source>Algorithm: %1 (%2, %3 bytes)</source>
        <translation>Алгоритм: %1 (%2, %3 байт)</translation>
    </message>
    <message>
        <location filename="../src/flash/flasher.cpp" line="729" />
        <location filename="../src/flash/flasher.cpp" line="867" />
        <source>Reset: %1</source>
        <translation>Сброс: %1</translation>
    </message>
    <message>
        <location filename="../src/flash/flasher.cpp" line="738" />
        <source>RAM transfer: %1 words (%2 bytes) per DAP_TransferBlock, command packet %3 bytes.</source>
        <translation>Передача в ОЗУ: %1 слов (%2 байт) на DAP_TransferBlock, командный пакет %3 байт.</translation>
    </message>
    <message>
        <location filename="../src/flash/flasher.cpp" line="751" />
        <source>Writing the algorithm to the RAM failed, attempt %1 of %2: %3</source>
        <translation>Ошибка записи алгоритма в ОЗУ, попытка %1 из %2: %3</translation>
    </message>
    <message>
        <location filename="../src/flash/flasher.cpp" line="763" />
        <source>Algorithm loaded at 0x%1 (%2 bytes of RAM).</source>
        <translation>Алгоритм загружен по адресу 0x%1 (%2 байт ОЗУ).</translation>
    </message>
    <message>
        <location filename="../src/flash/flasher.cpp" line="782" />
        <source>Init() did not complete, restarting the algorithm (%1 of %2): %3</source>
        <translation>Init() не завершилась, перезапуск алгоритма (%1 из %2): %3</translation>
    </message>
    <message>
        <location filename="../src/flash/flasher.cpp" line="807" />
        <source>Init() failed with code %1.</source>
        <translation>Init() завершилась с кодом %1.</translation>
    </message>
    <message>
        <location filename="../src/flash/flasher.cpp" line="816" />
        <source>Initialised.</source>
        <translation>Инициализировано.</translation>
    </message>
    <message>
        <location filename="../src/flash/flasher.cpp" line="854" />
        <source>UnInit() failed: %1</source>
        <translation>Ошибка UnInit(): %1</translation>
    </message>
    <message>
        <location filename="../src/flash/flasher.cpp" line="856" />
        <source>UnInit() returned code %1.</source>
        <translation>UnInit() вернула код %1.</translation>
    </message>
    <message>
        <location filename="../src/flash/flasher.cpp" line="865" />
        <source>Resetting the target failed: %1</source>
        <translation>Не удалось сбросить цель: %1</translation>
    </message>
    <message>
        <location filename="../src/flash/flasher.cpp" line="874" />
        <source>Done.</source>
        <translation>Готово.</translation>
    </message>
</context>
<context>
    <name>LanguageManager</name>
    <message>
        <location filename="../src/languagemanager.cpp" line="129" />
        <source>Follow System(&amp;A)</source>
        <translation>Как в системе(&amp;A)</translation>
    </message>
    <message>
        <location filename="../src/languagemanager.cpp" line="130" />
        <source>English(&amp;E)</source>
        <translation>English(&amp;E)</translation>
    </message>
    <message>
        <location filename="../src/languagemanager.cpp" line="131" />
        <source>Simplified Chinese(&amp;C)</source>
        <translation>Китайский (упрощ.)(&amp;C)</translation>
    </message>
    <message>
        <location filename="../src/languagemanager.cpp" line="132" />
        <source>Russian(&amp;R)</source>
        <translation>Русский(&amp;R)</translation>
    </message>
    <message>
        <location filename="../src/languagemanager.cpp" line="133" />
        <source>German(&amp;G)</source>
        <translation>Deutsch(&amp;G)</translation>
    </message>
    <message>
        <location filename="../src/languagemanager.cpp" line="134" />
        <source>Spanish(&amp;S)</source>
        <translation>Español(&amp;S)</translation>
    </message>
    <message>
        <location filename="../src/languagemanager.cpp" line="178" />
        <source>Simplified Chinese</source>
        <translation>Китайский (упрощённый)</translation>
    </message>
    <message>
        <location filename="../src/languagemanager.cpp" line="180" />
        <source>Russian</source>
        <translation>Русский</translation>
    </message>
    <message>
        <location filename="../src/languagemanager.cpp" line="182" />
        <source>German</source>
        <translation>Deutsch</translation>
    </message>
    <message>
        <location filename="../src/languagemanager.cpp" line="184" />
        <source>Spanish</source>
        <translation>Español</translation>
    </message>
    <message>
        <location filename="../src/languagemanager.cpp" line="187" />
        <source>English</source>
        <translation>English</translation>
    </message>
</context>
<context>
    <name>MainWindow</name>
    <message>
        <location filename="../src/mainwindow.cpp" line="479" />
        <source>WeAct Studio DAP Flasher</source>
        <translation>WeAct Studio DAP Flasher</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="481" />
        <source>Debug Probe</source>
        <translation>Отладочный пробник</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="482" />
        <source>Flash Algorithm</source>
        <translation>Алгоритм программирования</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="483" />
        <source>Firmware File</source>
        <translation>Файл прошивки</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="484" />
        <source>Options</source>
        <translation>Параметры</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="486" />
        <source>Probe:</source>
        <translation>Пробник:</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="487" />
        <source>Connection:</source>
        <translation>Подключение:</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="488" />
        <source>Target:</source>
        <translation>Цель:</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="489" />
        <source>Algorithm File:</source>
        <translation>Файл алгоритма:</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="490" />
        <source>File Path:</source>
        <translation>Путь к файлу:</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="491" />
        <source>Firmware:</source>
        <translation>Прошивка:</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="492" />
        <source>BIN Load Address: 0x</source>
        <translation>Адрес загрузки BIN: 0x</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="493" />
        <source>SWD Clock:</source>
        <translation>Частота SWD:</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="494" />
        <source>Algorithm RAM: 0x</source>
        <translation>ОЗУ алгоритма: 0x</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="495" />
        <source>Reset Mode:</source>
        <translation>Режим сброса:</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="497" />
        <source>Refresh(&amp;R)</source>
        <translation>Обновить(&amp;R)</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="498" />
        <location filename="../src/mainwindow.cpp" line="499" />
        <source>Browse(&amp;B)...</source>
        <translation>Обзор(&amp;B)...</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="501" />
        <source>Whole chip erase</source>
        <translation>Стирание всего чипа</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="502" />
        <source>Verify</source>
        <translation>Проверка</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="503" />
        <source>Reset and run</source>
        <translation>Сброс и запуск</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="513" />
        <source>Cancel(&amp;C)</source>
        <translation>Отмена(&amp;C)</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="513" />
        <source>Start Programming(&amp;S)</source>
        <translation>Начать программирование(&amp;S)</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="514" />
        <source>Halt(&amp;P)</source>
        <translation>Остановить(&amp;P)</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="515" />
        <source>Run(&amp;R)</source>
        <translation>Запустить(&amp;R)</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="516" />
        <source>Reset(&amp;T)</source>
        <translation>Сброс(&amp;T)</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="517" />
        <location filename="../src/mainwindow.cpp" line="536" />
        <source>Exit(&amp;X)</source>
        <translation>Выход(&amp;X)</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="519" />
        <source>Select or drop a .hex / .bin firmware file</source>
        <translation>Выберите или перетащите файл прошивки .hex / .bin</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="523" />
        <source>RAM area the flash algorithm is copied to while it runs. 0x20000000 is correct for most Cortex-M devices.</source>
        <translation>Область ОЗУ, в которую копируется алгоритм программирования во время работы. 0x20000000 подходит для большинства устройств Cortex-M.</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="525" />
        <source>Loading address used for .bin files (ignored for .hex)</source>
        <translation>Адрес загрузки для файлов .bin (для .hex игнорируется)</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="526" />
        <source>Reset used by the Reset button and by "Reset and run". Software reset needs no extra wire, hardware reset pulses the nRESET line, automatic uses nRESET when the probe has one.</source>
        <translation>Сброс, выполняемый кнопкой «Сброс» и командой «Сброс и запуск». Программному сбросу не нужен дополнительный провод, аппаратный сброс подаёт импульс на линию nRESET, автоматический использует nRESET, если он есть у пробника.</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="530" />
        <source>Log</source>
        <translation>Журнал</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="533" />
        <source>File(&amp;F)</source>
        <translation>Файл(&amp;F)</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="534" />
        <source>Open Firmware(&amp;O)...</source>
        <translation>Открыть прошивку(&amp;O)...</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="535" />
        <source>Select Algorithm(&amp;A)...</source>
        <translation>Выбрать алгоритм(&amp;A)...</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="538" />
        <source>Tools(&amp;T)</source>
        <translation>Инструменты(&amp;T)</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="539" />
        <source>Refresh Probes(&amp;R)</source>
        <translation>Обновить список пробников(&amp;R)</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="540" />
        <source>Rescan Algorithms(&amp;L)</source>
        <translation>Пересканировать алгоритмы(&amp;L)</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="541" />
        <source>Show All USB HID Devices(&amp;H)</source>
        <translation>Показать все устройства USB HID(&amp;H)</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="542" />
        <source>Clear Log(&amp;C)</source>
        <translation>Очистить журнал(&amp;C)</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="544" />
        <source>View(&amp;V)</source>
        <translation>Вид(&amp;V)</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="545" />
        <source>Language(&amp;L)</source>
        <translation>Язык(&amp;L)</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="546" />
        <source>Help(&amp;H)</source>
        <translation>Справка(&amp;H)</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="547" />
        <source>About(&amp;A)</source>
        <translation>О программе(&amp;A)</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="713" />
        <source>(no probe found)</source>
        <translation>(пробник не найден)</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="792" />
        <source>Identifying the target over %1 ...</source>
        <translation>Определение цели через %1...</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="863" />
        <source>&lt;font color='%1'&gt;Identifying the target ...&lt;/font&gt;</source>
        <translation>&lt;font color='%1'&gt;Определение цели...&lt;/font&gt;</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="865" />
        <location filename="../src/mainwindow.cpp" line="932" />
        <location filename="../src/mainwindow.cpp" line="958" />
        <source>-</source>
        <translation>-</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="930" />
        <source>&lt;font color='%1'&gt;No probe found&lt;/font&gt;</source>
        <translation>&lt;font color='%1'&gt;Пробник не найден&lt;/font&gt;</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="931" />
        <source>Connect a CMSIS-DAP probe and press Refresh.</source>
        <translation>Подключите пробник CMSIS-DAP и нажмите «Обновить».</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="939" />
        <source>&lt;font color='%1'&gt;Connected (%2)&lt;/font&gt;</source>
        <translation>&lt;font color='%1'&gt;Подключено (%2)&lt;/font&gt;</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="941" />
        <source>SWD</source>
        <translation>SWD</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="943" />
        <source>CMSIS-DAP firmware: %1</source>
        <translation>Прошивка CMSIS-DAP: %1</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="944" />
        <source>unknown</source>
        <translation>неизвестно</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="949" />
        <source>CPUID %1, DP IDCODE %2, core %3</source>
        <translation>CPUID %1, DP IDCODE %2, ядро %3</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="952" />
        <source>halted</source>
        <translation>остановлено</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="952" />
        <source>running</source>
        <translation>выполняется</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="955" />
        <source>&lt;font color='%1'&gt;Probe present, target not identified&lt;/font&gt;</source>
        <translation>&lt;font color='%1'&gt;Пробник подключён, цель не определена&lt;/font&gt;</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="1025" />
        <source>Scanning flash algorithms (%1 pack folders) ...</source>
        <translation>Сканирование алгоритмов программирования (папок пакетов: %1)...</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="1066" />
        <source>%1 flash algorithms found in %2 groups.</source>
        <translation>Найдено алгоритмов программирования: %1 в %2 группах.</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="1071" />
        <source>%1 file(s) were skipped: they carry no usable FlashDevice structure.</source>
        <translation>Пропущено файлов: %1 — в них нет пригодной структуры FlashDevice.</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="1158" />
        <source>Recently used</source>
        <translation>Недавно использованные</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="1179" />
        <source>Additional folder: %1</source>
        <translation>Дополнительная папка: %1</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="1206" />
        <source>Last used</source>
        <translation>Последний использованный</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="1251" />
        <source>Select a Keil Flash Algorithm</source>
        <translation>Выбор алгоритма программирования Keil</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="1287" />
        <source>The algorithm %1 could not be loaded: %2</source>
        <translation>Не удалось загрузить алгоритм %1: %2</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="1290" />
        <source>Algorithm loaded: %1 (%2)</source>
        <translation>Алгоритм загружен: %1 (%2)</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="1301" />
        <location filename="../src/mainwindow.cpp" line="1371" />
        <source>&lt;font color='%1'&gt;%2&lt;/font&gt;</source>
        <translation>&lt;font color='%1'&gt;%2&lt;/font&gt;</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="1308" />
        <source>&lt;font color='%1'&gt;No algorithm selected&lt;/font&gt;</source>
        <translation>&lt;font color='%1'&gt;Алгоритм не выбран&lt;/font&gt;</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="1314" />
        <source>&lt;b&gt;%1&lt;/b&gt;&lt;br&gt;%2, flash 0x%3 - 0x%4 (%5), page %6 bytes, %7 sectors</source>
        <translation>&lt;b&gt;%1&lt;/b&gt;&lt;br&gt;%2, флеш 0x%3 - 0x%4 (%5), страница %6 байт, секторов %7</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="1324" />
        <source>&lt;br&gt;&lt;font color='%1'&gt;This algorithm has no chip erase; the sectors are erased instead.&lt;/font&gt;</source>
        <translation>&lt;br&gt;&lt;font color='%1'&gt;В этом алгоритме нет стирания всего чипа; вместо этого стираются сектора.&lt;/font&gt;</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="1342" />
        <source>Select Firmware File</source>
        <translation>Выбор файла прошивки</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="1344" />
        <source>Firmware Files (*.hex *.bin *.ihex);;Intel HEX (*.hex *.ihex);;Binary Files (*.bin);;All Files (*.*)</source>
        <translation>Файлы прошивки (*.hex *.bin *.ihex);;Intel HEX (*.hex *.ihex);;Двоичные файлы (*.bin);;Все файлы (*.*)</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="1362" />
        <source>&lt;font color='%1'&gt;No valid firmware file selected&lt;/font&gt;</source>
        <translation>&lt;font color='%1'&gt;Не выбран корректный файл прошивки&lt;/font&gt;</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="1380" />
        <source>%1  Size: %2 in %3 blocks  Address: 0x%4 - 0x%5  Checksum: 0x%6</source>
        <translation>%1  Размер: %2 в %3 блоках  Адрес: 0x%4 - 0x%5  Контрольная сумма: 0x%6</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="1381" />
        <source>Intel HEX</source>
        <translation>Intel HEX</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="1381" />
        <source>BIN</source>
        <translation>BIN</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="1445" />
        <location filename="../src/mainwindow.cpp" line="1480" />
        <location filename="../src/mainwindow.cpp" line="1506" />
        <location filename="../src/mainwindow.cpp" line="1567" />
        <source>No Probe</source>
        <translation>Нет пробника</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="1445" />
        <location filename="../src/mainwindow.cpp" line="1480" />
        <location filename="../src/mainwindow.cpp" line="1506" />
        <source>Select a debug probe first.</source>
        <translation>Сначала выберите отладочный пробник.</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="1465" />
        <source>Halt Failed</source>
        <translation>Сбой остановки</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="1469" />
        <source>The core is halted.</source>
        <translation>Ядро остановлено.</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="1491" />
        <source>Run Failed</source>
        <translation>Сбой запуска</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="1495" />
        <source>The core is running.</source>
        <translation>Ядро выполняется.</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="1517" />
        <source>Reset Failed</source>
        <translation>Сбой сброса</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="1521" />
        <source>The target was reset and is running.</source>
        <translation>Цель сброшена и выполняется.</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="1568" />
        <source>Select a CMSIS-DAP probe first.</source>
        <translation>Сначала выберите пробник CMSIS-DAP.</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="1573" />
        <source>No Algorithm</source>
        <translation>Нет алгоритма</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="1574" />
        <source>Select the Keil .FLM flash algorithm that matches the device.</source>
        <translation>Выберите алгоритм программирования Keil .FLM, соответствующий устройству.</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="1580" />
        <source>No Firmware</source>
        <translation>Нет прошивки</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="1581" />
        <source>Select a valid firmware file first.</source>
        <translation>Сначала выберите корректный файл прошивки.</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="1602" />
        <source>Preparing ...</source>
        <translation>Подготовка...</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="1627" />
        <source>Cancelling ...</source>
        <translation>Отмена...</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="1650" />
        <location filename="../src/mainwindow.cpp" line="1687" />
        <source>Programming complete</source>
        <translation>Программирование завершено</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="1656" />
        <source>Complete</source>
        <translation>Готово</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="1657" />
        <source>The firmware was programmed successfully.

%1</source>
        <translation>Прошивка успешно записана.

%1</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="1660" />
        <source>Cancelled</source>
        <translation>Отменено</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="1664" />
        <location filename="../src/mainwindow.cpp" line="1690" />
        <source>Programming failed</source>
        <translation>Сбой программирования</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="1666" />
        <source>Failed</source>
        <translation>Ошибка</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="1694" />
        <source>Ready</source>
        <translation>Готово</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="1761" />
        <source>About</source>
        <translation>О программе</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="1765" />
        <source>&lt;h3&gt;WeAct Studio DAP Flasher&lt;/h3&gt;&lt;p&gt;Version: %1&lt;/p&gt;&lt;p&gt;&lt;b&gt;WeAct Studio&lt;/b&gt;&lt;br&gt;Copyright (C) 2026 WeAct Studio. All rights reserved.&lt;/p&gt;&lt;p&gt;Programs Cortex-M devices through a CMSIS-DAP probe (USB HID or USB bulk / WinUSB) using the Keil .FLM flash algorithms.&lt;/p&gt;&lt;p&gt;This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version. It is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.&lt;/p&gt;&lt;p&gt;License: GPL-3.0-or-later&lt;br&gt;&lt;a href='https://www.gnu.org/licenses/gpl-3.0.html'&gt;https://www.gnu.org/licenses/gpl-3.0.html&lt;/a&gt;&lt;/p&gt;&lt;p&gt;Built with Qt %2, statically compiled with MSYS2 (MINGW64).&lt;/p&gt;</source>
        <translation>&lt;h3&gt;WeAct Studio DAP Flasher&lt;/h3&gt;&lt;p&gt;Версия: %1&lt;/p&gt;&lt;p&gt;&lt;b&gt;WeAct Studio&lt;/b&gt;&lt;br&gt;Copyright (C) 2026 WeAct Studio. Все права защищены.&lt;/p&gt;&lt;p&gt;Программирует устройства Cortex-M через пробник CMSIS-DAP (USB HID или USB bulk / WinUSB) с помощью алгоритмов программирования Keil .FLM.&lt;/p&gt;&lt;p&gt;Эта программа является свободным программным обеспечением: вы можете распространять её и (или) изменять на условиях GNU General Public License, опубликованной Фондом свободного программного обеспечения, версии 3 или любой более поздней версии. Программа распространяется в надежде, что она будет полезной, но БЕЗ КАКИХ-ЛИБО ГАРАНТИЙ, в том числе без подразумеваемой гарантии ТОВАРНОЙ ПРИГОДНОСТИ или ПРИГОДНОСТИ ДЛЯ КОНКРЕТНОЙ ЦЕЛИ. Подробности см. в GNU General Public License.&lt;/p&gt;&lt;p&gt;Лицензия: GPL-3.0-or-later&lt;br&gt;&lt;a href='https://www.gnu.org/licenses/gpl-3.0.html'&gt;https://www.gnu.org/licenses/gpl-3.0.html&lt;/a&gt;&lt;/p&gt;&lt;p&gt;Собрано с Qt %2, статически скомпилировано с MSYS2 (MINGW64).&lt;/p&gt;</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="1821" />
        <source>Programming in Progress</source>
        <translation>Программирование выполняется</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="1822" />
        <source>Programming is still running; stopping now can leave the device with an incomplete firmware.

Exit anyway?</source>
        <translation>Программирование ещё идёт; остановка может оставить устройство с неполной прошивкой.

Всё равно выйти?</translation>
    </message>
</context>
<context>
    <name>Probes</name>
    <message>
        <location filename="../src/dap/probes.cpp" line="73" />
        <source>The HID interface could not be initialised.</source>
        <translation>Не удалось инициализировать интерфейс HID.</translation>
    </message>
    <message>
        <location filename="../src/dap/probes.cpp" line="82" />
        <source>HID init failed: %1</source>
        <translation>Сбой инициализации HID: %1</translation>
    </message>
    <message>
        <location filename="../src/dap/probes.cpp" line="83" />
        <source>HID init failed.</source>
        <translation>Сбой инициализации HID.</translation>
    </message>
    <message>
        <location filename="../src/dap/probes.cpp" line="287" />
        <source>HID device</source>
        <translation>Устройство HID</translation>
    </message>
    <message>
        <location filename="../src/dap/probes.cpp" line="307" />
        <source> (in use)</source>
        <translation> (используется)</translation>
    </message>
    <message>
        <location filename="../src/dap/probes.cpp" line="318" />
        <source>Transport: %1</source>
        <translation>Транспорт: %1</translation>
    </message>
    <message>
        <location filename="../src/dap/probes.cpp" line="324" />
        <source>Product: %1</source>
        <translation>Изделие: %1</translation>
    </message>
    <message>
        <location filename="../src/dap/probes.cpp" line="326" />
        <source>Manufacturer: %1</source>
        <translation>Производитель: %1</translation>
    </message>
    <message>
        <location filename="../src/dap/probes.cpp" line="328" />
        <source>Serial: %1</source>
        <translation>Серийный номер: %1</translation>
    </message>
    <message>
        <location filename="../src/dap/probes.cpp" line="331" />
        <source>USB: bus %1 address %2, interface %3, EP IN 0x%4 / OUT 0x%5</source>
        <translation>USB: шина %1, адрес %2, интерфейс %3, EP IN 0x%4 / OUT 0x%5</translation>
    </message>
    <message>
        <location filename="../src/dap/probes.cpp" line="339" />
        <source>Interface: %1</source>
        <translation>Интерфейс: %1</translation>
    </message>
    <message>
        <location filename="../src/dap/probes.cpp" line="340" />
        <source>Usage page: 0x%1  Usage: 0x%2</source>
        <translation>Страница использования: 0x%1  Использование: 0x%2</translation>
    </message>
</context>
<context>
    <name>ResetMode</name>
    <message>
        <location filename="../src/target/resetmode.cpp" line="18" />
        <source>Hardware reset</source>
        <translation>Аппаратный сброс</translation>
    </message>
    <message>
        <location filename="../src/target/resetmode.cpp" line="20" />
        <source>Automatic</source>
        <translation>Автоматически</translation>
    </message>
    <message>
        <location filename="../src/target/resetmode.cpp" line="25" />
        <source>Software reset</source>
        <translation>Программный сброс</translation>
    </message>
</context>
<context>
    <name>TargetSession</name>
    <message>
        <location filename="../src/targetsession.cpp" line="30" />
        <source>Probe: %1</source>
        <translation>Пробник: %1</translation>
    </message>
    <message>
        <location filename="../src/targetsession.cpp" line="83" />
        <source>The target program was holding the debug access; the core is now halted under reset.</source>
        <translation>Целевая программа удерживала отладочный доступ; теперь ядро остановлено под сбросом.</translation>
    </message>
    <message>
        <location filename="../src/targetsession.cpp" line="191" />
        <source>Reset: %1</source>
        <translation>Сброс: %1</translation>
    </message>
    <message>
        <location filename="../src/targetsession.cpp" line="225" />
        <source>Reset: %1 (the DP did not answer, so this was a pin reset only).</source>
        <translation>Сброс: %1 (DP не ответил, поэтому выполнен только сброс по линии).</translation>
    </message>
</context>
<context>
    <name>ThemeManager</name>
    <message>
        <location filename="../src/thememanager.cpp" line="166" />
        <source>Follow System(&amp;S)</source>
        <translation>Как в системе(&amp;S)</translation>
    </message>
    <message>
        <location filename="../src/thememanager.cpp" line="168" />
        <source>Light(&amp;L)</source>
        <translation>Светлая(&amp;L)</translation>
    </message>
    <message>
        <location filename="../src/thememanager.cpp" line="171" />
        <source>Dark(&amp;D)</source>
        <translation>Тёмная(&amp;D)</translation>
    </message>
</context>
</TS>