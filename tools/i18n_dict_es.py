# -*- coding: utf-8 -*-
"""Spanish dictionary: English source text -> Spanish translation."""

ES = {
    # ---------------------------------------------------------------- AlgoCatalog
    "Keil built-in algorithms": "Algoritmos integrados de Keil",
    "Additional folder": "Carpeta adicional",
    "Flash Algorithms (*.flm *.FLM *.flx *.xldr);;All Files (*.*)":
        "Algoritmos de programación (*.flm *.FLM *.flx *.xldr);;Todos los archivos (*.*)",

    # ------------------------------------------------------------ AlgorithmPicker
    "Filter algorithms ...": "Filtrar algoritmos...",
    "Select algorithm ...": "Seleccionar algoritmo...",

    # ---------------------------------------------------------------------- CmsisDap
    "No probe is open.": "No hay ninguna sonda abierta.",
    "The probe sent an empty answer.": "La sonda devolvió una respuesta vacía.",
    "The probe answered with a malformed DAP_Info response.":
        "La sonda devolvió una respuesta DAP_Info mal formada.",
    "The probe sent a truncated DAP_Info response.":
        "La sonda devolvió una respuesta DAP_Info truncada.",
    "The probe answered with a malformed DAP_Connect response.":
        "La sonda devolvió una respuesta DAP_Connect mal formada.",
    "The probe refused the SWD connection.": "La sonda rechazó la conexión SWD.",
    "The DP did not acknowledge the power up request within %1 ms.":
        "El DP no confirmó la solicitud de encendido en %1 ms.",
    "Setting the SWD clock failed.": "No se pudo establecer la frecuencia de SWD.",
    "Configuring the DAP transfer failed.":
        "No se pudo configurar la transferencia DAP.",
    "The probe answered with a malformed DAP_SWJ_Pins response.":
        "La sonda devolvió una respuesta DAP_SWJ_Pins mal formada.",
    "The probe answered with a malformed DAP_Reset_Target response.":
        "La sonda devolvió una respuesta DAP_Reset_Target mal formada.",
    "The SWJ sequence failed.": "Falló la secuencia SWJ.",
    "SWD clock: %1": "Frecuencia SWD: %1",
    "The SWD clock could not be set: %1":
        "No se pudo establecer la frecuencia SWD: %1",
    "The SWD DP did not answer, trying to revive the link.":
        "El DP SWD no responde, intentando restablecer el enlace.",
    "The SWD link came back at %1 Hz.": "El enlace SWD se restableció a %1 Hz.",
    "The SWD DP does not answer. Check the wiring (SWDIO, SWCLK, GND), the target "
    "power supply and the SWD clock. A target program that switched the debug "
    "interface off, or that put the core into a low power state, only comes back "
    "on a power cycle. If it comes back with BOOT0 held high, that program is what "
    "takes the debug pins over: start the ROM bootloader that way, which leaves the "
    "debug pins alone, and erase the chip from there.":
        "El DP SWD no responde. Compruebe el cableado (SWDIO, SWCLK, GND), la "
        "alimentación del objetivo y la frecuencia SWD. Un programa del objetivo que "
        "haya desactivado la interfaz de depuración o que haya puesto el núcleo en "
        "bajo consumo solo vuelve tras un ciclo de alimentación. Si vuelve con BOOT0 "
        "a nivel alto, ese programa es el que se queda con los pines de depuración: "
        "arranque así el bootloader de la ROM, que no toca los pines de depuración, "
        "y borre el chip desde ahí.",
    "This probe does not drive an nRESET line.":
        "Esta sonda no controla ninguna línea nRESET.",
    "Holding nRESET low and connecting under reset.":
        "Manteniendo nRESET a nivel bajo y conectando en reset.",
    "The SWD link came back under reset at %1 Hz.":
        "El enlace SWD se restableció en reset a %1 Hz.",
    "Connecting under reset did not bring the DP back either.":
        "Conectar en reset tampoco recuperó el DP.",
    "The SWD DP did not answer while nRESET was being driven.":
        "El DP SWD no respondió mientras se accionaba nRESET.",
    "The SWD DP reported an invalid IDCODE.":
        "El DP SWD informó de un IDCODE no válido.",
    "The DP read returned no data.": "La lectura del DP no devolvió datos.",
    "The AP read returned no data.": "La lectura del AP no devolvió datos.",
    "Empty transfer.": "Transferencia vacía.",
    "Internal error: the write value count does not match the request count.":
        "Error interno: el número de valores de escritura no coincide con el de la "
        "solicitud.",
    "The probe answered with a truncated transfer response.":
        "La sonda devolvió una respuesta de transferencia truncada.",
    "The probe answered with a malformed DAP_Transfer response.":
        "La sonda devolvió una respuesta DAP_Transfer mal formada.",
    "SWD fault (the target did not acknowledge the access).":
        "Fallo SWD (el objetivo no confirmó el acceso).",
    "Protocol error during the SWD transfer.":
        "Error de protocolo durante la transferencia SWD.",
    "The probe performed %1 of %2 transfers.":
        "La sonda realizó %1 de %2 transferencias.",
    "The probe sent a truncated transfer payload.":
        "La sonda envió datos de transferencia truncados.",
    "The SWD transfer was retried too often (WAIT response).":
        "La transferencia SWD se reintentó demasiadas veces (respuesta WAIT).",
    "Empty block transfer.": "Transferencia por bloques vacía.",
    "The probe answered with a truncated block transfer response.":
        "La sonda devolvió una respuesta de transferencia por bloques truncada.",
    "The probe answered with a malformed DAP_TransferBlock response.":
        "La sonda devolvió una respuesta DAP_TransferBlock mal formada.",
    "SWD fault during the block transfer.":
        "Fallo SWD durante la transferencia por bloques.",
    "Protocol error during the block transfer.":
        "Error de protocolo durante la transferencia por bloques.",
    "The probe performed %1 of %2 block transfers.":
        "La sonda realizó %1 de %2 transferencias por bloques.",
    "The probe sent a truncated block transfer payload.":
        "La sonda envió datos de transferencia por bloques truncados.",
    "The probe answered a poll with no data.":
        "La sonda respondió a una consulta sin datos.",

    # --------------------------------------------------------------------- CortexM
    "Unknown Cortex-M core": "Núcleo Cortex-M desconocido",
    "No Cortex-M core answered at CPUID (0x%1).":
        "Ningún núcleo Cortex-M respondió en CPUID (0x%1).",
    "The core is locked up (the flash algorithm crashed).":
        "El núcleo está bloqueado (el algoritmo de programación se ha caído).",
    "The core did not halt within %1 ms.": "El núcleo no se detuvo en %1 ms.",
    "The probe has no nRESET line; use the software reset instead.":
        "La sonda no tiene línea nRESET; use el reset por software.",
    "The core register transfer timed out.":
        "Se agotó el tiempo de espera en la transferencia de registros del núcleo.",
    "Internal error: no register buffer.": "Error interno: no hay búfer de registros.",

    # ----------------------------------------------------------------- DapTransport
    "unknown HID error": "error HID desconocido",
    "The HID probe could not be opened. It is probably used by another program "
    "(Keil, OpenOCD, a debug session).":
        "No se pudo abrir la sonda HID. Probablemente la esté usando otro programa "
        "(Keil, OpenOCD, una sesión de depuración).",
    "USB HID (CMSIS-DAP v1)": "USB HID (CMSIS-DAP v1)",
    "HID write failed: %1": "Error al escribir HID: %1",
    "HID read failed: %1": "Error al leer HID: %1",
    "The probe did not answer (timeout).":
        "La sonda no respondió (tiempo de espera agotado).",
    "The USB (libusb) layer could not be initialised.":
        "No se pudo inicializar la capa USB (libusb).",
    "USB enumeration failed.": "Falló la enumeración USB.",
    "The USB probe was not found any more. Replug it and refresh the list.":
        "La sonda USB ya no se encuentra. Vuelva a conectarla y actualice la lista.",
    "Access to the USB probe was denied. It is either used by another program, "
    "or no WinUSB driver is bound to it.":
        "Se denegó el acceso a la sonda USB. O la está usando otro programa, o no "
        "tiene ningún controlador WinUSB asociado.",
    "The probe is not bound to a WinUSB (or libusbK) driver. Install the driver, "
    "or select the same probe over its USB HID (CMSIS-DAP v1) interface.":
        "La sonda no tiene asociado un controlador WinUSB (o libusbK). Instale el "
        "controlador o seleccione la misma sonda a través de su interfaz USB HID "
        "(CMSIS-DAP v1).",
    "Opening the USB probe failed: %1": "Error al abrir la sonda USB: %1",
    "Claiming the USB interface failed: %1":
        "Error al reclamar la interfaz USB: %1",
    "CMSIS-DAP bulk transport, spec framing (no length prefix)":
        "Transporte CMSIS-DAP bulk, tramas según la especificación (sin prefijo de "
        "longitud)",
    "CMSIS-DAP bulk transport, length-prefixed framing (2 byte prefix)":
        "Transporte CMSIS-DAP bulk, tramas con prefijo de longitud (prefijo de 2 "
        "bytes)",
    "USB bulk write failed: %1": "Error al escribir USB bulk: %1",
    "USB bulk read failed: %1": "Error al leer USB bulk: %1",
    "The probe sent a malformed packet.": "La sonda envió un paquete mal formado.",
    "The probe did not answer DAP_Info. Check that the right interface of the "
    "device was selected and that the firmware runs a CMSIS-DAP implementation.":
        "La sonda no respondió a DAP_Info. Compruebe que se ha seleccionado la "
        "interfaz correcta del dispositivo y que el firmware implementa CMSIS-DAP.",
    "The command does not fit into one packet (%1 > %2 bytes).":
        "El comando no cabe en un paquete (%1 > %2 bytes).",
    "USB bulk (WinUSB, CMSIS-DAP v2)": "USB bulk (WinUSB, CMSIS-DAP v2)",

    # ---------------------------------------------------------------------- ElfFile
    "The algorithm file could not be opened: %1":
        "No se pudo abrir el archivo del algoritmo: %1",
    "The file is too small to be an ELF image.":
        "El archivo es demasiado pequeño para ser una imagen ELF.",
    "The file is not an ELF image (a Keil .FLM algorithm is expected).":
        "El archivo no es una imagen ELF (se espera un algoritmo Keil .FLM).",
    "Only 32 bit ELF images are supported (this one is 64 bit).":
        "Solo se admiten imágenes ELF de 32 bits (esta es de 64 bits).",
    "Only little endian ELF images are supported.":
        "Solo se admiten imágenes ELF little endian.",
    "The image is not an ARM image (e_machine = %1).":
        "La imagen no es una imagen ARM (e_machine = %1).",
    "The image is truncated (segment %1 leaves the file).":
        "La imagen está truncada (el segmento %1 queda fuera del archivo).",
    "The image carries no loadable segment.":
        "La imagen no contiene ningún segmento cargable.",

    # --------------------------------------------------------------------- FlashAlgo
    "The algorithm does not export an Init() function.":
        "El algoritmo no exporta la función Init().",
    "The algorithm does not export a ProgramPage() function.":
        "El algoritmo no exporta la función ProgramPage().",
    "The algorithm does not export EraseSector() nor EraseChip().":
        "El algoritmo no exporta ni EraseSector() ni EraseChip().",
    "The FlashDevice structure could not be located in the algorithm.":
        "No se encontró la estructura FlashDevice en el algoritmo.",
    "The FlashDevice structure at offset 0x%1 is not valid.":
        "La estructura FlashDevice en el desplazamiento 0x%1 no es válida.",
    "The algorithm carries an empty sector table.":
        "El algoritmo contiene una tabla de sectores vacía.",
    "Unknown": "Desconocido",
    "On-chip flash": "Memoria flash interna",
    "External flash, 8 bit bus": "Memoria flash externa, bus de 8 bits",
    "External flash, 16 bit bus": "Memoria flash externa, bus de 16 bits",
    "External flash, 32 bit bus": "Memoria flash externa, bus de 32 bits",
    "External SPI flash": "Memoria flash SPI externa",

    # ------------------------------------------------------------------- FlashWorker
    "Loading the flash algorithm ...": "Cargando el algoritmo de programación...",
    "Algorithm %1: %2, %3, page %4 bytes, %5 sectors":
        "Algoritmo %1: %2, %3, página de %4 bytes, %5 sectores",
    "%1: %2 in %3 blocks, 0x%4 - 0x%5, checksum 0x%6":
        "%1: %2 en %3 bloques, 0x%4 - 0x%5, suma de comprobación 0x%6",
    "Intel HEX": "Intel HEX",
    "BIN": "BIN",
    "Firmware %1": "Firmware %1",
    "The firmware (0x%1 - 0x%2) does not fit into the flash of the algorithm "
    "(0x%3 - 0x%4). Check the loading address.":
        "El firmware (0x%1 - 0x%2) no cabe en la flash del algoritmo "
        "(0x%3 - 0x%4). Compruebe la dirección de carga.",
    "The algorithm has no sector at 0x%1.": "El algoritmo no tiene sector en 0x%1.",
    "No firmware given: the whole chip is erased and the target is left halted.":
        "No se ha indicado firmware: se borra todo el chip y el objetivo queda "
        "detenido.",
    "Cancelled.": "Cancelado.",
    "Probe: %1": "Sonda: %1",
    "CMSIS-DAP firmware: %1": "Firmware CMSIS-DAP: %1",
    "Target: %1 (CPUID 0x%2, DP IDCODE 0x%3)":
        "Objetivo: %1 (CPUID 0x%2, DP IDCODE 0x%3)",
    "The whole chip was erased in %1 s; the target is left halted.":
        "Se borró todo el chip en %1 s; el objetivo queda detenido.",
    "Programming finished in %1 s: %2 pages, %3 bytes, %4 sectors erased.":
        "Programación terminada en %1 s: %2 páginas, %3 bytes, %4 sectores "
        "borrados.",
    "The algorithm used 0x%1 of RAM at %2.": "El algoritmo usó 0x%1 de RAM en %2.",
    "The SWD link failed at %1; retrying at %2.":
        "El enlace SWD falló a %1; reintentando a %2.",
    "Retrying at a slower clock ...": "Reintentando con una frecuencia más baja...",

    # ----------------------------------------------------------------------- Flasher
    "The algorithm image is empty.": "La imagen del algoritmo está vacía.",
    "The algorithm reports an unusable page size (%1 bytes).":
        "El algoritmo indica un tamaño de página no utilizable (%1 bytes).",
    "The RAM base address is too high for the algorithm.":
        "La dirección base de RAM es demasiado alta para el algoritmo.",
    "The algorithm could not be written to the target RAM at %1: %2 of %3 bytes "
    "came back different, the first one at %4 (wrote 0x%5, read 0x%6). The base "
    "address has to be RAM the debug unit can write to; if it is, the probe did "
    "not move the data reliably.":
        "No se pudo escribir el algoritmo en la RAM del objetivo en %1: %2 de %3 "
        "bytes se leyeron distintos, el primero en %4 (escrito 0x%5, leído 0x%6). "
        "La dirección base debe ser RAM en la que la unidad de depuración pueda "
        "escribir; si lo es, la sonda no transmitió los datos de forma fiable.",
    "The algorithm does not provide %1().": "El algoritmo no proporciona %1().",
    "%1() did not return: %2 (PC 0x%3, DFSR 0x%4, CFSR 0x%5, HFSR 0x%6)":
        "%1() no regresó: %2 (PC 0x%3, DFSR 0x%4, CFSR 0x%5, HFSR 0x%6)",
    "%1() stopped at 0x%2 instead of returning to 0x%3 (DFSR = 0x%4).":
        "%1() se detuvo en 0x%2 en lugar de volver a 0x%3 (DFSR = 0x%4).",
    "Erasing the whole chip ...": "Borrando todo el chip...",
    "EraseChip() failed with code %1.": "EraseChip() falló con el código %1.",
    "Chip erase done.": "Borrado del chip completado.",
    "The algorithm provides neither EraseChip() nor EraseSector().":
        "El algoritmo no proporciona ni EraseChip() ni EraseSector().",
    "Address 0x%1 is not covered by the sector table of the algorithm.":
        "La dirección 0x%1 no está cubierta por la tabla de sectores del algoritmo.",
    "Nothing to erase.": "No hay nada que borrar.",
    "Erasing sector %1 of %2 (0x%3) ...": "Borrando el sector %1 de %2 (0x%3)...",
    "Erasing the sector at 0x%1 failed with code %2.":
        "El borrado del sector en 0x%1 falló con el código %2.",
    "The firmware file carries no data.": "El archivo de firmware no contiene datos.",
    "Programming 0x%1 (%2 / %3 bytes) ...": "Programando 0x%1 (%2 / %3 bytes)...",
    "Programming the page at 0x%1 failed with code %2.":
        "La programación de la página en 0x%1 falló con el código %2.",
    "The algorithm has no Verify() routine; the flash is read back through the "
    "debug port instead.":
        "El algoritmo no tiene rutina Verify(); la flash se lee de vuelta a través "
        "del puerto de depuración.",
    "Verifying 0x%1 ...": "Verificando 0x%1...",
    "Verification failed at 0x%1 (code %2).":
        "La verificación falló en 0x%1 (código %2).",
    "Reading back 0x%1 failed; the verification was skipped.":
        "No se pudo releer 0x%1; se omitió la verificación.",
    "Verification failed at 0x%1: expected 0x%2, read 0x%3.":
        "La verificación falló en 0x%1: se esperaba 0x%2, se leyó 0x%3.",
    "No flash algorithm is selected.":
        "No hay ningún algoritmo de programación seleccionado.",
    "No firmware data to program.": "No hay datos de firmware que programar.",
    "Preparing ...": "Preparando...",
    "Algorithm: %1 (%2, %3 bytes)": "Algoritmo: %1 (%2, %3 bytes)",
    "Reset: %1": "Reset: %1",
    "RAM transfer: %1 words (%2 bytes) per DAP_TransferBlock, command packet %3 bytes.":
        "Transferencia a RAM: %1 palabras (%2 bytes) por DAP_TransferBlock, paquete "
        "de comando de %3 bytes.",
    "Writing the algorithm to the RAM failed, attempt %1 of %2: %3":
        "Error al escribir el algoritmo en la RAM, intento %1 de %2: %3",
    "Algorithm loaded at 0x%1 (%2 bytes of RAM).":
        "Algoritmo cargado en 0x%1 (%2 bytes de RAM).",
    "Init() did not complete, restarting the algorithm (%1 of %2): %3":
        "Init() no terminó, reiniciando el algoritmo (%1 de %2): %3",
    "Init() failed with code %1.": "Init() falló con el código %1.",
    "Initialised.": "Inicializado.",
    "UnInit() failed: %1": "Error en UnInit(): %1",
    "UnInit() returned code %1.": "UnInit() devolvió el código %1.",
    "Resetting the target failed: %1": "No se pudo reiniciar el objetivo: %1",
    "Done.": "Hecho.",

    # --------------------------------------------------------------- LanguageManager
    "Follow System(&A)": "Seguir el sistema(&A)",
    "English(&E)": "English(&E)",
    "Simplified Chinese(&C)": "Chino simplificado(&C)",
    "Russian(&R)": "Русский(&R)",
    "German(&G)": "Deutsch(&G)",
    "Spanish(&S)": "Español(&S)",
    "Simplified Chinese": "Chino simplificado",
    "Russian": "Русский",
    "German": "Deutsch",
    "Spanish": "Español",
    "English": "English",

    # -------------------------------------------------------------------- MainWindow
    "WeAct Studio DAP Flasher": "WeAct Studio DAP Flasher",
    "Debug Probe": "Sonda de depuración",
    "Flash Algorithm": "Algoritmo de programación",
    "Firmware File": "Archivo de firmware",
    "Options": "Opciones",
    "Probe:": "Sonda:",
    "Connection:": "Conexión:",
    "Target:": "Objetivo:",
    "Algorithm File:": "Archivo de algoritmo:",
    "File Path:": "Ruta del archivo:",
    "Firmware:": "Firmware:",
    "BIN Load Address: 0x": "Dirección de carga BIN: 0x",
    "SWD Clock:": "Frecuencia SWD:",
    "Algorithm RAM: 0x": "RAM del algoritmo: 0x",
    "Reset Mode:": "Modo de reset:",
    "Refresh(&R)": "Actualizar(&R)",
    "Browse(&B)...": "Examinar(&B)...",
    "Whole chip erase": "Borrar todo el chip",
    "Verify": "Verificar",
    "Reset and run": "Reset y ejecutar",
    "Cancel(&C)": "Cancelar(&C)",
    "Start Programming(&S)": "Iniciar programación(&S)",
    "Halt(&P)": "Detener(&P)",
    "Run(&R)": "Ejecutar(&R)",
    "Reset(&T)": "Reset(&T)",
    "Exit(&X)": "Salir(&X)",
    "Select or drop a .hex / .bin firmware file":
        "Seleccione o arrastre un archivo de firmware .hex / .bin",
    "RAM area the flash algorithm is copied to while it runs. 0x20000000 is "
    "correct for most Cortex-M devices.":
        "Área de RAM a la que se copia el algoritmo de programación mientras se "
        "ejecuta. 0x20000000 es correcta para la mayoría de los dispositivos "
        "Cortex-M.",
    "Loading address used for .bin files (ignored for .hex)":
        "Dirección de carga usada para archivos .bin (se ignora en .hex)",
    "Reset used by the Reset button and by \"Reset and run\". Software reset needs "
    "no extra wire, hardware reset pulses the nRESET line, automatic uses nRESET "
    "when the probe has one.":
        "Reset usado por el botón Reset y por «Reset y ejecutar». El reset por "
        "software no necesita ningún cable adicional, el reset por hardware genera "
        "un pulso en la línea nRESET y el automático usa nRESET cuando la sonda "
        "dispone de ella.",
    "Log": "Registro",
    "File(&F)": "Archivo(&F)",
    "Open Firmware(&O)...": "Abrir firmware(&O)...",
    "Select Algorithm(&A)...": "Seleccionar algoritmo(&A)...",
    "Tools(&T)": "Herramientas(&T)",
    "Refresh Probes(&R)": "Actualizar sondas(&R)",
    "Rescan Algorithms(&L)": "Reexplorar algoritmos(&L)",
    "Show All USB HID Devices(&H)": "Mostrar todos los dispositivos USB HID(&H)",
    "Clear Log(&C)": "Borrar registro(&C)",
    "View(&V)": "Ver(&V)",
    "Language(&L)": "Idioma(&L)",
    "Help(&H)": "Ayuda(&H)",
    "About(&A)": "Acerca de(&A)",
    "(no probe found)": "(no se encontró ninguna sonda)",
    "Identifying the target over %1 ...": "Identificando el objetivo por %1...",
    "<font color='%1'>Identifying the target ...</font>":
        "<font color='%1'>Identificando el objetivo...</font>",
    "<font color='%1'>No probe found</font>":
        "<font color='%1'>No se encontró ninguna sonda</font>",
    "Connect a CMSIS-DAP probe and press Refresh.":
        "Conecte una sonda CMSIS-DAP y pulse Actualizar.",
    "<font color='%1'>Connected (%2)</font>":
        "<font color='%1'>Conectado (%2)</font>",
    "SWD": "SWD",
    "-": "-",
    "unknown": "desconocido",
    "CPUID %1, DP IDCODE %2, core %3": "CPUID %1, DP IDCODE %2, núcleo %3",
    "halted": "detenido",
    "running": "en ejecución",
    "<font color='%1'>Probe present, target not identified</font>":
        "<font color='%1'>Sonda presente, objetivo no identificado</font>",
    "Scanning flash algorithms (%1 pack folders) ...":
        "Explorando algoritmos de programación (%1 carpetas de paquetes)...",
    "%1 flash algorithms found in %2 groups.":
        "Se encontraron %1 algoritmos de programación en %2 grupos.",
    "%1 file(s) were skipped: they carry no usable FlashDevice structure.":
        "Se omitieron %1 archivo(s): no contienen una estructura FlashDevice "
        "utilizable.",
    "Recently used": "Usados recientemente",
    "Additional folder: %1": "Carpeta adicional: %1",
    "Last used": "Último usado",
    "Select a Keil Flash Algorithm": "Seleccionar un algoritmo de programación Keil",
    "The algorithm %1 could not be loaded: %2":
        "No se pudo cargar el algoritmo %1: %2",
    "Algorithm loaded: %1 (%2)": "Algoritmo cargado: %1 (%2)",
    "<font color='%1'>%2</font>": "<font color='%1'>%2</font>",
    "<font color='%1'>No algorithm selected</font>":
        "<font color='%1'>Ningún algoritmo seleccionado</font>",
    "<b>%1</b><br>%2, flash 0x%3 - 0x%4 (%5), page %6 bytes, %7 sectors":
        "<b>%1</b><br>%2, flash 0x%3 - 0x%4 (%5), página de %6 bytes, %7 sectores",
    "<br><font color='%1'>This algorithm has no chip erase; the sectors are "
    "erased instead.</font>":
        "<br><font color='%1'>Este algoritmo no permite borrar todo el chip; se "
        "borran los sectores.</font>",
    "Select Firmware File": "Seleccionar archivo de firmware",
    "Firmware Files (*.hex *.bin *.ihex);;Intel HEX (*.hex *.ihex);;"
    "Binary Files (*.bin);;All Files (*.*)":
        "Archivos de firmware (*.hex *.bin *.ihex);;Intel HEX (*.hex *.ihex);;"
        "Archivos binarios (*.bin);;Todos los archivos (*.*)",
    "<font color='%1'>No valid firmware file selected</font>":
        "<font color='%1'>No se ha seleccionado un archivo de firmware válido</font>",
    "%1  Size: %2 in %3 blocks  Address: 0x%4 - 0x%5  Checksum: 0x%6":
        "%1  Tamaño: %2 en %3 bloques  Dirección: 0x%4 - 0x%5  Suma de "
        "comprobación: 0x%6",
    "No Probe": "Sin sonda",
    "Select a debug probe first.": "Seleccione primero una sonda de depuración.",
    "Halt Failed": "Error al detener",
    "The core is halted.": "El núcleo está detenido.",
    "Run Failed": "Error al ejecutar",
    "The core is running.": "El núcleo está en ejecución.",
    "Reset Failed": "Error de reset",
    "The target was reset and is running.":
        "El objetivo se ha reiniciado y está en ejecución.",
    "Select a CMSIS-DAP probe first.": "Seleccione primero una sonda CMSIS-DAP.",
    "No Algorithm": "Sin algoritmo",
    "Select the Keil .FLM flash algorithm that matches the device.":
        "Seleccione el algoritmo de programación Keil .FLM que corresponda al "
        "dispositivo.",
    "No Firmware": "Sin firmware",
    "Select a valid firmware file first.":
        "Seleccione primero un archivo de firmware válido.",
    "Cancelling ...": "Cancelando...",
    "Programming complete": "Programación completada",
    "Complete": "Completado",
    "The firmware was programmed successfully.\n\n%1":
        "El firmware se programó correctamente.\n\n%1",
    "Cancelled": "Cancelado",
    "Programming failed": "Error de programación",
    "Failed": "Error",
    "Ready": "Listo",
    "About": "Acerca de",
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
        "<h3>WeAct Studio DAP Flasher</h3><p>Versión: %1</p><p><b>WeAct Studio</b>"
        "<br>Copyright (C) 2026 WeAct Studio. Todos los derechos reservados.</p>"
        "<p>Programa dispositivos Cortex-M a través de una sonda CMSIS-DAP (USB HID "
        "o USB bulk / WinUSB) usando los algoritmos de programación Keil .FLM.</p>"
        "<p>Este programa es software libre: puede redistribuirlo y/o modificarlo "
        "bajo los términos de la Licencia Pública General de GNU publicada por la "
        "Free Software Foundation, ya sea la versión 3 de la Licencia o (a su "
        "elección) cualquier versión posterior. Se distribuye con la esperanza de "
        "que sea útil, pero SIN NINGUNA GARANTÍA, ni siquiera la garantía implícita "
        "de COMERCIABILIDAD o IDONEIDAD PARA UN PROPÓSITO PARTICULAR. Consulte la "
        "Licencia Pública General de GNU para más detalles.</p><p>Licencia: "
        "GPL-3.0-or-later<br><a href='https://www.gnu.org/licenses/gpl-3.0.html'>"
        "https://www.gnu.org/licenses/gpl-3.0.html</a></p><p>Compilado con Qt %2, "
        "compilado estáticamente con MSYS2 (MINGW64).</p>",
    "Programming in Progress": "Programación en curso",
    "Programming is still running; stopping now can leave the device with an "
    "incomplete firmware.\n\nExit anyway?":
        "La programación sigue en curso; detenerla ahora puede dejar el dispositivo "
        "con un firmware incompleto.\n\n¿Salir de todos modos?",

    # ------------------------------------------------------------------------ Probes
    "The HID interface could not be initialised.":
        "No se pudo inicializar la interfaz HID.",
    "HID init failed: %1": "Error al inicializar HID: %1",
    "HID init failed.": "Error al inicializar HID.",
    "HID device": "Dispositivo HID",
    " (in use)": " (en uso)",
    "Transport: %1": "Transporte: %1",
    "Product: %1": "Producto: %1",
    "Manufacturer: %1": "Fabricante: %1",
    "Serial: %1": "Número de serie: %1",
    "USB: bus %1 address %2, interface %3, EP IN 0x%4 / OUT 0x%5":
        "USB: bus %1 dirección %2, interfaz %3, EP IN 0x%4 / OUT 0x%5",
    "Interface: %1": "Interfaz: %1",
    "Usage page: 0x%1  Usage: 0x%2": "Página de uso: 0x%1  Uso: 0x%2",

    # --------------------------------------------------------------------- ResetMode
    "Hardware reset": "Reset por hardware",
    "Automatic": "Automático",
    "Software reset": "Reset por software",

    # ----------------------------------------------------------------- TargetSession
    "The target program was holding the debug access; the core is now halted "
    "under reset.":
        "El programa del objetivo retenía el acceso de depuración; ahora el núcleo "
        "está detenido en reset.",
    "Reset: %1 (the DP did not answer, so this was a pin reset only).":
        "Reset: %1 (el DP no respondió, así que solo se hizo un reset por línea).",

    # ------------------------------------------------------------------ ThemeManager
    "Follow System(&S)": "Seguir el sistema(&S)",
    "Light(&L)": "Claro(&L)",
    "Dark(&D)": "Oscuro(&D)",
}
