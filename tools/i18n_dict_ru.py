# -*- coding: utf-8 -*-
"""Russian dictionary: English source text -> Russian translation."""

RU = {
    # ---------------------------------------------------------------- AlgoCatalog
    "Keil built-in algorithms": "Встроенные алгоритмы Keil",
    "Additional folder": "Дополнительная папка",
    "Flash Algorithms (*.flm *.FLM *.flx *.xldr);;All Files (*.*)":
        "Алгоритмы программирования (*.flm *.FLM *.flx *.xldr);;Все файлы (*.*)",

    # ------------------------------------------------------------ AlgorithmPicker
    "Filter algorithms ...": "Фильтр алгоритмов...",
    "Select algorithm ...": "Выбор алгоритма...",

    # ---------------------------------------------------------------------- CmsisDap
    "No probe is open.": "Нет открытого пробника.",
    "The probe sent an empty answer.": "Пробник вернул пустой ответ.",
    "The probe answered with a malformed DAP_Info response.":
        "Пробник вернул некорректный ответ DAP_Info.",
    "The probe sent a truncated DAP_Info response.":
        "Пробник вернул обрезанный ответ DAP_Info.",
    "The probe answered with a malformed DAP_Connect response.":
        "Пробник вернул некорректный ответ DAP_Connect.",
    "The probe refused the SWD connection.": "Пробник отклонил подключение по SWD.",
    "The DP did not acknowledge the power up request within %1 ms.":
        "DP не подтвердил запрос включения питания за %1 мс.",
    "Setting the SWD clock failed.": "Не удалось установить частоту SWD.",
    "Configuring the DAP transfer failed.": "Не удалось настроить передачу DAP.",
    "The probe answered with a malformed DAP_SWJ_Pins response.":
        "Пробник вернул некорректный ответ DAP_SWJ_Pins.",
    "The probe answered with a malformed DAP_Reset_Target response.":
        "Пробник вернул некорректный ответ DAP_Reset_Target.",
    "The SWJ sequence failed.": "Сбой последовательности SWJ.",
    "SWD clock: %1": "Частота SWD: %1",
    "The SWD clock could not be set: %1": "Не удалось установить частоту SWD: %1",
    "The SWD DP did not answer, trying to revive the link.":
        "DP SWD не отвечает, попытка восстановить связь.",
    "The SWD link came back at %1 Hz.": "Связь SWD восстановлена на частоте %1 Гц.",
    "The SWD DP does not answer. Check the wiring (SWDIO, SWCLK, GND), the target "
    "power supply and the SWD clock. A target program that switched the debug "
    "interface off, or that put the core into a low power state, only comes back "
    "on a power cycle. If it comes back with BOOT0 held high, that program is what "
    "takes the debug pins over: start the ROM bootloader that way, which leaves the "
    "debug pins alone, and erase the chip from there.":
        "DP SWD не отвечает. Проверьте подключение (SWDIO, SWCLK, GND), питание цели "
        "и частоту SWD. Целевая программа, отключившая отладочный интерфейс или "
        "переведшая ядро в режим пониженного энергопотребления, восстанавливается "
        "только после цикла питания. Если она восстанавливается при удерживаемом "
        "высоком уровне BOOT0, значит именно она забирает отладочные выводы: "
        "запустите так загрузчик из ПЗУ, который не трогает отладочные выводы, "
        "и сотрите чип оттуда.",
    "This probe does not drive an nRESET line.": "Этот пробник не управляет линией nRESET.",
    "Holding nRESET low and connecting under reset.":
        "Удержание nRESET в низком уровне и подключение под сбросом.",
    "The SWD link came back under reset at %1 Hz.":
        "Связь SWD восстановлена под сбросом на частоте %1 Гц.",
    "Connecting under reset did not bring the DP back either.":
        "Подключение под сбросом тоже не восстановило DP.",
    "The SWD DP did not answer while nRESET was being driven.":
        "DP SWD не отвечал при активной линии nRESET.",
    "The SWD DP reported an invalid IDCODE.": "DP SWD сообщил недопустимый IDCODE.",
    "The DP read returned no data.": "Чтение DP не вернуло данных.",
    "The AP read returned no data.": "Чтение AP не вернуло данных.",
    "Empty transfer.": "Пустая передача.",
    "Internal error: the write value count does not match the request count.":
        "Внутренняя ошибка: число записываемых значений не совпадает с числом в запросе.",
    "The probe answered with a truncated transfer response.":
        "Пробник вернул обрезанный ответ передачи.",
    "The probe answered with a malformed DAP_Transfer response.":
        "Пробник вернул некорректный ответ DAP_Transfer.",
    "SWD fault (the target did not acknowledge the access).":
        "Ошибка SWD (цель не подтвердила доступ).",
    "Protocol error during the SWD transfer.": "Ошибка протокола при передаче SWD.",
    "The probe performed %1 of %2 transfers.": "Пробник выполнил %1 из %2 передач.",
    "The probe sent a truncated transfer payload.":
        "Пробник передал обрезанные данные передачи.",
    "The SWD transfer was retried too often (WAIT response).":
        "Слишком много повторов передачи SWD (ответ WAIT).",
    "Empty block transfer.": "Пустая блочная передача.",
    "The probe answered with a truncated block transfer response.":
        "Пробник вернул обрезанный ответ блочной передачи.",
    "The probe answered with a malformed DAP_TransferBlock response.":
        "Пробник вернул некорректный ответ DAP_TransferBlock.",
    "SWD fault during the block transfer.": "Ошибка SWD при блочной передаче.",
    "Protocol error during the block transfer.": "Ошибка протокола при блочной передаче.",
    "The probe performed %1 of %2 block transfers.":
        "Пробник выполнил %1 из %2 блочных передач.",
    "The probe sent a truncated block transfer payload.":
        "Пробник передал обрезанные данные блочной передачи.",
    "The probe answered a poll with no data.": "Пробник ответил на опрос без данных.",

    # --------------------------------------------------------------------- CortexM
    "Unknown Cortex-M core": "Неизвестное ядро Cortex-M",
    "No Cortex-M core answered at CPUID (0x%1).":
        "Ядро Cortex-M не ответило по CPUID (0x%1).",
    "The core is locked up (the flash algorithm crashed).":
        "Ядро заблокировано (алгоритм программирования отказал).",
    "The core did not halt within %1 ms.": "Ядро не остановилось за %1 мс.",
    "The probe has no nRESET line; use the software reset instead.":
        "У пробника нет линии nRESET; используйте программный сброс.",
    "The core register transfer timed out.": "Тайм-аут передачи регистров ядра.",
    "Internal error: no register buffer.": "Внутренняя ошибка: нет буфера регистров.",

    # ----------------------------------------------------------------- DapTransport
    "unknown HID error": "неизвестная ошибка HID",
    "The HID probe could not be opened. It is probably used by another program "
    "(Keil, OpenOCD, a debug session).":
        "Не удалось открыть пробник HID. Вероятно, его использует другая программа "
        "(Keil, OpenOCD, отладочная сессия).",
    "USB HID (CMSIS-DAP v1)": "USB HID (CMSIS-DAP v1)",
    "HID write failed: %1": "Ошибка записи HID: %1",
    "HID read failed: %1": "Ошибка чтения HID: %1",
    "The probe did not answer (timeout).": "Пробник не ответил (тайм-аут).",
    "The USB (libusb) layer could not be initialised.":
        "Не удалось инициализировать слой USB (libusb).",
    "USB enumeration failed.": "Сбой перечисления USB-устройств.",
    "The USB probe was not found any more. Replug it and refresh the list.":
        "USB-пробник больше не найден. Подключите его заново и обновите список.",
    "Access to the USB probe was denied. It is either used by another program, "
    "or no WinUSB driver is bound to it.":
        "Доступ к USB-пробнику запрещён. Либо его использует другая программа, "
        "либо для него не привязан драйвер WinUSB.",
    "The probe is not bound to a WinUSB (or libusbK) driver. Install the driver, "
    "or select the same probe over its USB HID (CMSIS-DAP v1) interface.":
        "Для пробника не привязан драйвер WinUSB (или libusbK). Установите драйвер "
        "или выберите тот же пробник через его интерфейс USB HID (CMSIS-DAP v1).",
    "Opening the USB probe failed: %1": "Не удалось открыть USB-пробник: %1",
    "Claiming the USB interface failed: %1": "Не удалось захватить интерфейс USB: %1",
    "CMSIS-DAP bulk transport, spec framing (no length prefix)":
        "Транспорт CMSIS-DAP bulk, кадры по спецификации (без префикса длины)",
    "CMSIS-DAP bulk transport, length-prefixed framing (2 byte prefix)":
        "Транспорт CMSIS-DAP bulk, кадры с префиксом длины (префикс 2 байта)",
    "USB bulk write failed: %1": "Ошибка записи USB bulk: %1",
    "USB bulk read failed: %1": "Ошибка чтения USB bulk: %1",
    "The probe sent a malformed packet.": "Пробник передал некорректный пакет.",
    "The probe did not answer DAP_Info. Check that the right interface of the "
    "device was selected and that the firmware runs a CMSIS-DAP implementation.":
        "Пробник не ответил на DAP_Info. Проверьте, что выбран нужный интерфейс "
        "устройства и что прошивка реализует CMSIS-DAP.",
    "The command does not fit into one packet (%1 > %2 bytes).":
        "Команда не помещается в один пакет (%1 > %2 байт).",
    "USB bulk (WinUSB, CMSIS-DAP v2)": "USB bulk (WinUSB, CMSIS-DAP v2)",

    # ---------------------------------------------------------------------- ElfFile
    "The algorithm file could not be opened: %1": "Не удалось открыть файл алгоритма: %1",
    "The file is too small to be an ELF image.": "Файл слишком мал для образа ELF.",
    "The file is not an ELF image (a Keil .FLM algorithm is expected).":
        "Файл не является образом ELF (ожидается алгоритм Keil .FLM).",
    "Only 32 bit ELF images are supported (this one is 64 bit).":
        "Поддерживаются только 32-битные образы ELF (этот — 64-битный).",
    "Only little endian ELF images are supported.":
        "Поддерживаются только образы ELF с порядком байтов little endian.",
    "The image is not an ARM image (e_machine = %1).":
        "Образ не является образом ARM (e_machine = %1).",
    "The image is truncated (segment %1 leaves the file).":
        "Образ обрезан (сегмент %1 выходит за пределы файла).",
    "The image carries no loadable segment.": "Образ не содержит загружаемых сегментов.",

    # --------------------------------------------------------------------- FlashAlgo
    "The algorithm does not export an Init() function.":
        "Алгоритм не экспортирует функцию Init().",
    "The algorithm does not export a ProgramPage() function.":
        "Алгоритм не экспортирует функцию ProgramPage().",
    "The algorithm does not export EraseSector() nor EraseChip().":
        "Алгоритм не экспортирует ни EraseSector(), ни EraseChip().",
    "The FlashDevice structure could not be located in the algorithm.":
        "Структура FlashDevice не найдена в алгоритме.",
    "The FlashDevice structure at offset 0x%1 is not valid.":
        "Структура FlashDevice по смещению 0x%1 некорректна.",
    "The algorithm carries an empty sector table.":
        "Алгоритм содержит пустую таблицу секторов.",
    "Unknown": "Неизвестно",
    "On-chip flash": "Встроенная флеш-память",
    "External flash, 8 bit bus": "Внешняя флеш-память, 8-битная шина",
    "External flash, 16 bit bus": "Внешняя флеш-память, 16-битная шина",
    "External flash, 32 bit bus": "Внешняя флеш-память, 32-битная шина",
    "External SPI flash": "Внешняя SPI флеш-память",

    # ------------------------------------------------------------------- FlashWorker
    "Loading the flash algorithm ...": "Загрузка алгоритма программирования...",
    "Algorithm %1: %2, %3, page %4 bytes, %5 sectors":
        "Алгоритм %1: %2, %3, страница %4 байт, секторов %5",
    "%1: %2 in %3 blocks, 0x%4 - 0x%5, checksum 0x%6":
        "%1: %2 в %3 блоках, 0x%4 - 0x%5, контрольная сумма 0x%6",
    "Intel HEX": "Intel HEX",
    "BIN": "BIN",
    "Firmware %1": "Прошивка %1",
    "The firmware (0x%1 - 0x%2) does not fit into the flash of the algorithm "
    "(0x%3 - 0x%4). Check the loading address.":
        "Прошивка (0x%1 - 0x%2) не помещается во флеш-память алгоритма (0x%3 - 0x%4). "
        "Проверьте адрес загрузки.",
    "The algorithm has no sector at 0x%1.": "В алгоритме нет сектора по адресу 0x%1.",
    "No firmware given: the whole chip is erased and the target is left halted.":
        "Прошивка не указана: весь чип стирается, цель остаётся остановленной.",
    "Cancelled.": "Отменено.",
    "Probe: %1": "Пробник: %1",
    "CMSIS-DAP firmware: %1": "Прошивка CMSIS-DAP: %1",
    "Target: %1 (CPUID 0x%2, DP IDCODE 0x%3)":
        "Цель: %1 (CPUID 0x%2, DP IDCODE 0x%3)",
    "The whole chip was erased in %1 s; the target is left halted.":
        "Весь чип стёрт за %1 с; цель остаётся остановленной.",
    "Programming finished in %1 s: %2 pages, %3 bytes, %4 sectors erased.":
        "Программирование завершено за %1 с: страниц %2, байт %3, стёрто секторов %4.",
    "The algorithm used 0x%1 of RAM at %2.":
        "Алгоритм использовал 0x%1 ОЗУ по адресу %2.",
    "The SWD link failed at %1; retrying at %2.":
        "Связь SWD потеряна на частоте %1; повтор на %2.",
    "Retrying at a slower clock ...": "Повтор на более низкой частоте...",

    # ----------------------------------------------------------------------- Flasher
    "The algorithm image is empty.": "Образ алгоритма пуст.",
    "The algorithm reports an unusable page size (%1 bytes).":
        "Алгоритм сообщает непригодный размер страницы (%1 байт).",
    "The RAM base address is too high for the algorithm.":
        "Базовый адрес ОЗУ слишком высок для алгоритма.",
    "The algorithm could not be written to the target RAM at %1: %2 of %3 bytes "
    "came back different, the first one at %4 (wrote 0x%5, read 0x%6). The base "
    "address has to be RAM the debug unit can write to; if it is, the probe did "
    "not move the data reliably.":
        "Не удалось записать алгоритм в ОЗУ цели по адресу %1: %2 из %3 байт "
        "не совпали, первый — по адресу %4 (записано 0x%5, прочитано 0x%6). "
        "Базовый адрес должен указывать на ОЗУ, доступное для записи отладочному "
        "блоку; если это так, пробник передал данные ненадёжно.",
    "The algorithm does not provide %1().": "Алгоритм не предоставляет %1().",
    "%1() did not return: %2 (PC 0x%3, DFSR 0x%4, CFSR 0x%5, HFSR 0x%6)":
        "%1() не вернула управление: %2 (PC 0x%3, DFSR 0x%4, CFSR 0x%5, HFSR 0x%6)",
    "%1() stopped at 0x%2 instead of returning to 0x%3 (DFSR = 0x%4).":
        "%1() остановилась на 0x%2 вместо возврата в 0x%3 (DFSR = 0x%4).",
    "Erasing the whole chip ...": "Стирание всего чипа...",
    "EraseChip() failed with code %1.": "EraseChip() завершилась с кодом %1.",
    "Chip erase done.": "Стирание чипа выполнено.",
    "The algorithm provides neither EraseChip() nor EraseSector().":
        "Алгоритм не предоставляет ни EraseChip(), ни EraseSector().",
    "Address 0x%1 is not covered by the sector table of the algorithm.":
        "Адрес 0x%1 не покрыт таблицей секторов алгоритма.",
    "Nothing to erase.": "Нечего стирать.",
    "Erasing sector %1 of %2 (0x%3) ...": "Стирание сектора %1 из %2 (0x%3)...",
    "Erasing the sector at 0x%1 failed with code %2.":
        "Стирание сектора по адресу 0x%1 завершилось с кодом %2.",
    "The firmware file carries no data.": "Файл прошивки не содержит данных.",
    "Programming 0x%1 (%2 / %3 bytes) ...": "Программирование 0x%1 (%2 / %3 байт)...",
    "Programming the page at 0x%1 failed with code %2.":
        "Ошибка программирования страницы по адресу 0x%1, код %2.",
    "The algorithm has no Verify() routine; the flash is read back through the "
    "debug port instead.":
        "В алгоритме нет процедуры Verify(); флеш-память считывается через "
        "отладочный порт.",
    "Verifying 0x%1 ...": "Проверка 0x%1...",
    "Verification failed at 0x%1 (code %2).": "Ошибка проверки по адресу 0x%1 (код %2).",
    "Reading back 0x%1 failed; the verification was skipped.":
        "Не удалось прочитать 0x%1; проверка пропущена.",
    "Verification failed at 0x%1: expected 0x%2, read 0x%3.":
        "Ошибка проверки по адресу 0x%1: ожидалось 0x%2, прочитано 0x%3.",
    "No flash algorithm is selected.": "Алгоритм программирования не выбран.",
    "No firmware data to program.": "Нет данных прошивки для записи.",
    "Preparing ...": "Подготовка...",
    "Algorithm: %1 (%2, %3 bytes)": "Алгоритм: %1 (%2, %3 байт)",
    "Reset: %1": "Сброс: %1",
    "RAM transfer: %1 words (%2 bytes) per DAP_TransferBlock, command packet %3 bytes.":
        "Передача в ОЗУ: %1 слов (%2 байт) на DAP_TransferBlock, командный пакет %3 байт.",
    "Writing the algorithm to the RAM failed, attempt %1 of %2: %3":
        "Ошибка записи алгоритма в ОЗУ, попытка %1 из %2: %3",
    "Algorithm loaded at 0x%1 (%2 bytes of RAM).":
        "Алгоритм загружен по адресу 0x%1 (%2 байт ОЗУ).",
    "Init() did not complete, restarting the algorithm (%1 of %2): %3":
        "Init() не завершилась, перезапуск алгоритма (%1 из %2): %3",
    "Init() failed with code %1.": "Init() завершилась с кодом %1.",
    "Initialised.": "Инициализировано.",
    "UnInit() failed: %1": "Ошибка UnInit(): %1",
    "UnInit() returned code %1.": "UnInit() вернула код %1.",
    "Resetting the target failed: %1": "Не удалось сбросить цель: %1",
    "Done.": "Готово.",

    # --------------------------------------------------------------- LanguageManager
    "Follow System(&A)": "Как в системе(&A)",
    "English(&E)": "English(&E)",
    "Simplified Chinese(&C)": "Китайский (упрощ.)(&C)",
    "Russian(&R)": "Русский(&R)",
    "German(&G)": "Deutsch(&G)",
    "Spanish(&S)": "Español(&S)",
    "Simplified Chinese": "Китайский (упрощённый)",
    "Russian": "Русский",
    "German": "Deutsch",
    "Spanish": "Español",
    "English": "English",

    # -------------------------------------------------------------------- MainWindow
    "WeAct Studio DAP Flasher": "WeAct Studio DAP Flasher",
    "Debug Probe": "Отладочный пробник",
    "Flash Algorithm": "Алгоритм программирования",
    "Firmware File": "Файл прошивки",
    "Options": "Параметры",
    "Probe:": "Пробник:",
    "Connection:": "Подключение:",
    "Target:": "Цель:",
    "Algorithm File:": "Файл алгоритма:",
    "File Path:": "Путь к файлу:",
    "Firmware:": "Прошивка:",
    "BIN Load Address: 0x": "Адрес загрузки BIN: 0x",
    "SWD Clock:": "Частота SWD:",
    "Algorithm RAM: 0x": "ОЗУ алгоритма: 0x",
    "Reset Mode:": "Режим сброса:",
    "Refresh(&R)": "Обновить(&R)",
    "Browse(&B)...": "Обзор(&B)...",
    "Whole chip erase": "Стирание всего чипа",
    "Verify": "Проверка",
    "Reset and run": "Сброс и запуск",
    "Cancel(&C)": "Отмена(&C)",
    "Start Programming(&S)": "Начать программирование(&S)",
    "Halt(&P)": "Остановить(&P)",
    "Run(&R)": "Запустить(&R)",
    "Reset(&T)": "Сброс(&T)",
    "Exit(&X)": "Выход(&X)",
    "Select or drop a .hex / .bin firmware file":
        "Выберите или перетащите файл прошивки .hex / .bin",
    "RAM area the flash algorithm is copied to while it runs. 0x20000000 is "
    "correct for most Cortex-M devices.":
        "Область ОЗУ, в которую копируется алгоритм программирования во время "
        "работы. 0x20000000 подходит для большинства устройств Cortex-M.",
    "Loading address used for .bin files (ignored for .hex)":
        "Адрес загрузки для файлов .bin (для .hex игнорируется)",
    "Reset used by the Reset button and by \"Reset and run\". Software reset needs "
    "no extra wire, hardware reset pulses the nRESET line, automatic uses nRESET "
    "when the probe has one.":
        "Сброс, выполняемый кнопкой «Сброс» и командой «Сброс и запуск». "
        "Программному сбросу не нужен дополнительный провод, аппаратный сброс "
        "подаёт импульс на линию nRESET, автоматический использует nRESET, "
        "если он есть у пробника.",
    "Log": "Журнал",
    "File(&F)": "Файл(&F)",
    "Open Firmware(&O)...": "Открыть прошивку(&O)...",
    "Select Algorithm(&A)...": "Выбрать алгоритм(&A)...",
    "Tools(&T)": "Инструменты(&T)",
    "Refresh Probes(&R)": "Обновить список пробников(&R)",
    "Rescan Algorithms(&L)": "Пересканировать алгоритмы(&L)",
    "Show All USB HID Devices(&H)": "Показать все устройства USB HID(&H)",
    "Clear Log(&C)": "Очистить журнал(&C)",
    "View(&V)": "Вид(&V)",
    "Language(&L)": "Язык(&L)",
    "Help(&H)": "Справка(&H)",
    "About(&A)": "О программе(&A)",
    "(no probe found)": "(пробник не найден)",
    "Identifying the target over %1 ...": "Определение цели через %1...",
    "<font color='%1'>Identifying the target ...</font>":
        "<font color='%1'>Определение цели...</font>",
    "<font color='%1'>No probe found</font>":
        "<font color='%1'>Пробник не найден</font>",
    "Connect a CMSIS-DAP probe and press Refresh.":
        "Подключите пробник CMSIS-DAP и нажмите «Обновить».",
    "<font color='%1'>Connected (%2)</font>":
        "<font color='%1'>Подключено (%2)</font>",
    "SWD": "SWD",
    "-": "-",
    "CMSIS-DAP firmware: %1": "Прошивка CMSIS-DAP: %1",
    "unknown": "неизвестно",
    "CPUID %1, DP IDCODE %2, core %3": "CPUID %1, DP IDCODE %2, ядро %3",
    "halted": "остановлено",
    "running": "выполняется",
    "<font color='%1'>Probe present, target not identified</font>":
        "<font color='%1'>Пробник подключён, цель не определена</font>",
    "Scanning flash algorithms (%1 pack folders) ...":
        "Сканирование алгоритмов программирования (папок пакетов: %1)...",
    "%1 flash algorithms found in %2 groups.":
        "Найдено алгоритмов программирования: %1 в %2 группах.",
    "%1 file(s) were skipped: they carry no usable FlashDevice structure.":
        "Пропущено файлов: %1 — в них нет пригодной структуры FlashDevice.",
    "Recently used": "Недавно использованные",
    "Additional folder: %1": "Дополнительная папка: %1",
    "Last used": "Последний использованный",
    "Select a Keil Flash Algorithm": "Выбор алгоритма программирования Keil",
    "The algorithm %1 could not be loaded: %2":
        "Не удалось загрузить алгоритм %1: %2",
    "Algorithm loaded: %1 (%2)": "Алгоритм загружен: %1 (%2)",
    "<font color='%1'>%2</font>": "<font color='%1'>%2</font>",
    "<font color='%1'>No algorithm selected</font>":
        "<font color='%1'>Алгоритм не выбран</font>",
    "<b>%1</b><br>%2, flash 0x%3 - 0x%4 (%5), page %6 bytes, %7 sectors":
        "<b>%1</b><br>%2, флеш 0x%3 - 0x%4 (%5), страница %6 байт, секторов %7",
    "<br><font color='%1'>This algorithm has no chip erase; the sectors are "
    "erased instead.</font>":
        "<br><font color='%1'>В этом алгоритме нет стирания всего чипа; вместо "
        "этого стираются сектора.</font>",
    "Select Firmware File": "Выбор файла прошивки",
    "Firmware Files (*.hex *.bin *.ihex);;Intel HEX (*.hex *.ihex);;"
    "Binary Files (*.bin);;All Files (*.*)":
        "Файлы прошивки (*.hex *.bin *.ihex);;Intel HEX (*.hex *.ihex);;"
        "Двоичные файлы (*.bin);;Все файлы (*.*)",
    "<font color='%1'>No valid firmware file selected</font>":
        "<font color='%1'>Не выбран корректный файл прошивки</font>",
    "%1  Size: %2 in %3 blocks  Address: 0x%4 - 0x%5  Checksum: 0x%6":
        "%1  Размер: %2 в %3 блоках  Адрес: 0x%4 - 0x%5  Контрольная сумма: 0x%6",
    "No Probe": "Нет пробника",
    "Select a debug probe first.": "Сначала выберите отладочный пробник.",
    "Halt Failed": "Сбой остановки",
    "The core is halted.": "Ядро остановлено.",
    "Run Failed": "Сбой запуска",
    "The core is running.": "Ядро выполняется.",
    "Reset Failed": "Сбой сброса",
    "The target was reset and is running.": "Цель сброшена и выполняется.",
    "Select a CMSIS-DAP probe first.": "Сначала выберите пробник CMSIS-DAP.",
    "No Algorithm": "Нет алгоритма",
    "Select the Keil .FLM flash algorithm that matches the device.":
        "Выберите алгоритм программирования Keil .FLM, соответствующий устройству.",
    "No Firmware": "Нет прошивки",
    "Select a valid firmware file first.":
        "Сначала выберите корректный файл прошивки.",
    "Cancelling ...": "Отмена...",
    "Programming complete": "Программирование завершено",
    "Complete": "Готово",
    "The firmware was programmed successfully.\n\n%1":
        "Прошивка успешно записана.\n\n%1",
    "Cancelled": "Отменено",
    "Programming failed": "Сбой программирования",
    "Failed": "Ошибка",
    "Ready": "Готово",
    "About": "О программе",
    "<h3>WeAct Studio DAP Flasher</h3><p>Version: %1</p><p><b>WeAct Studio</b><br>"
    "Copyright (C) 2026 WeAct Studio. All rights reserved.</p><p>Programs Cortex-M "
    "devices through a CMSIS-DAP probe (USB HID or USB bulk / WinUSB) using the "
    "Keil .FLM flash algorithms.</p><p>This program is free software: you can "
    "redistribute it and/or modify it under the terms of the GNU General Public "
    "License as published by the Free Software Foundation, either version 3 of the "
    "License, or (at your option) any later version. It is distributed in the hope "
    "that it will be useful, but WITHOUT ANY WARRANTY; without even the implied "
    "warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU "
    "General Public License for more details.</p><p>License: GPL-3.0-or-later<br>"
    "<a href='https://www.gnu.org/licenses/gpl-3.0.html'>"
    "https://www.gnu.org/licenses/gpl-3.0.html</a></p><p>Built with Qt %2, "
    "statically compiled with MSYS2 (MINGW64).</p>":
        "<h3>WeAct Studio DAP Flasher</h3><p>Версия: %1</p><p><b>WeAct Studio</b><br>"
        "Copyright (C) 2026 WeAct Studio. Все права защищены.</p><p>Программирует "
        "устройства Cortex-M через пробник CMSIS-DAP (USB HID или USB bulk / "
        "WinUSB) с помощью алгоритмов программирования Keil .FLM.</p><p>Эта "
        "программа является свободным программным обеспечением: вы можете "
        "распространять её и (или) изменять на условиях GNU General Public "
        "License, опубликованной Фондом свободного программного обеспечения, "
        "версии 3 или любой более поздней версии. Программа распространяется "
        "в надежде, что она будет полезной, но БЕЗ КАКИХ-ЛИБО ГАРАНТИЙ, в том "
        "числе без подразумеваемой гарантии ТОВАРНОЙ ПРИГОДНОСТИ или ПРИГОДНОСТИ "
        "ДЛЯ КОНКРЕТНОЙ ЦЕЛИ. Подробности см. в GNU General Public "
        "License.</p><p>Лицензия: GPL-3.0-or-later<br>"
        "<a href='https://www.gnu.org/licenses/gpl-3.0.html'>"
        "https://www.gnu.org/licenses/gpl-3.0.html</a></p><p>Собрано с Qt %2, "
        "статически скомпилировано с MSYS2 (MINGW64).</p>",
    "Programming in Progress": "Программирование выполняется",
    "Programming is still running; stopping now can leave the device with an "
    "incomplete firmware.\n\nExit anyway?":
        "Программирование ещё идёт; остановка может оставить устройство с "
        "неполной прошивкой.\n\nВсё равно выйти?",

    # ------------------------------------------------------------------------ Probes
    "The HID interface could not be initialised.":
        "Не удалось инициализировать интерфейс HID.",
    "HID init failed: %1": "Сбой инициализации HID: %1",
    "HID init failed.": "Сбой инициализации HID.",
    "HID device": "Устройство HID",
    " (in use)": " (используется)",
    "Transport: %1": "Транспорт: %1",
    "Product: %1": "Изделие: %1",
    "Manufacturer: %1": "Производитель: %1",
    "Serial: %1": "Серийный номер: %1",
    "USB: bus %1 address %2, interface %3, EP IN 0x%4 / OUT 0x%5":
        "USB: шина %1, адрес %2, интерфейс %3, EP IN 0x%4 / OUT 0x%5",
    "Interface: %1": "Интерфейс: %1",
    "Usage page: 0x%1  Usage: 0x%2":
        "Страница использования: 0x%1  Использование: 0x%2",

    # --------------------------------------------------------------------- ResetMode
    "Hardware reset": "Аппаратный сброс",
    "Automatic": "Автоматически",
    "Software reset": "Программный сброс",

    # ----------------------------------------------------------------- TargetSession
    "The target program was holding the debug access; the core is now halted "
    "under reset.":
        "Целевая программа удерживала отладочный доступ; теперь ядро остановлено "
        "под сбросом.",
    "Reset: %1 (the DP did not answer, so this was a pin reset only).":
        "Сброс: %1 (DP не ответил, поэтому выполнен только сброс по линии).",

    # ------------------------------------------------------------------ ThemeManager
    "Follow System(&S)": "Как в системе(&S)",
    "Light(&L)": "Светлая(&L)",
    "Dark(&D)": "Тёмная(&D)",
}
