<?xml version="1.0" encoding="utf-8"?>
<!DOCTYPE TS>
<TS version="2.1" language="es_ES">
<context>
    <name>AlgoCatalog</name>
    <message>
        <location filename="../src/flash/algocatalog.cpp" line="453" />
        <source>Keil built-in algorithms</source>
        <translation>Algoritmos integrados de Keil</translation>
    </message>
    <message>
        <location filename="../src/flash/algocatalog.cpp" line="459" />
        <source>Additional folder</source>
        <translation>Carpeta adicional</translation>
    </message>
    <message>
        <location filename="../src/flash/algocatalog.cpp" line="597" />
        <source>Flash Algorithms (*.flm *.FLM *.flx *.xldr);;All Files (*.*)</source>
        <translation>Algoritmos de programación (*.flm *.FLM *.flx *.xldr);;Todos los archivos (*.*)</translation>
    </message>
</context>
<context>
    <name>AlgorithmPicker</name>
    <message>
        <location filename="../src/algorithmpicker.cpp" line="135" />
        <location filename="../src/algorithmpicker.cpp" line="380" />
        <source>Filter algorithms ...</source>
        <translation>Filtrar algoritmos...</translation>
    </message>
    <message>
        <location filename="../src/algorithmpicker.cpp" line="368" />
        <source>Select algorithm ...</source>
        <translation>Seleccionar algoritmo...</translation>
    </message>
</context>
<context>
    <name>CmsisDap</name>
    <message>
        <location filename="../src/dap/cmsisdap.cpp" line="195" />
        <source>No probe is open.</source>
        <translation>No hay ninguna sonda abierta.</translation>
    </message>
    <message>
        <location filename="../src/dap/cmsisdap.cpp" line="206" />
        <source>The probe sent an empty answer.</source>
        <translation>La sonda devolvió una respuesta vacía.</translation>
    </message>
    <message>
        <location filename="../src/dap/cmsisdap.cpp" line="234" />
        <source>The probe answered with a malformed DAP_Info response.</source>
        <translation>La sonda devolvió una respuesta DAP_Info mal formada.</translation>
    </message>
    <message>
        <location filename="../src/dap/cmsisdap.cpp" line="245" />
        <source>The probe sent a truncated DAP_Info response.</source>
        <translation>La sonda devolvió una respuesta DAP_Info truncada.</translation>
    </message>
    <message>
        <location filename="../src/dap/cmsisdap.cpp" line="299" />
        <source>The probe answered with a malformed DAP_Connect response.</source>
        <translation>La sonda devolvió una respuesta DAP_Connect mal formada.</translation>
    </message>
    <message>
        <location filename="../src/dap/cmsisdap.cpp" line="305" />
        <source>The probe refused the SWD connection.</source>
        <translation>La sonda rechazó la conexión SWD.</translation>
    </message>
    <message>
        <location filename="../src/dap/cmsisdap.cpp" line="378" />
        <source>The DP did not acknowledge the power up request within %1 ms.</source>
        <translation>El DP no confirmó la solicitud de encendido en %1 ms.</translation>
    </message>
    <message>
        <location filename="../src/dap/cmsisdap.cpp" line="425" />
        <source>Setting the SWD clock failed.</source>
        <translation>No se pudo establecer la frecuencia de SWD.</translation>
    </message>
    <message>
        <location filename="../src/dap/cmsisdap.cpp" line="448" />
        <source>Configuring the DAP transfer failed.</source>
        <translation>No se pudo configurar la transferencia DAP.</translation>
    </message>
    <message>
        <location filename="../src/dap/cmsisdap.cpp" line="468" />
        <source>The probe answered with a malformed DAP_SWJ_Pins response.</source>
        <translation>La sonda devolvió una respuesta DAP_SWJ_Pins mal formada.</translation>
    </message>
    <message>
        <location filename="../src/dap/cmsisdap.cpp" line="488" />
        <source>The probe answered with a malformed DAP_Reset_Target response.</source>
        <translation>La sonda devolvió una respuesta DAP_Reset_Target mal formada.</translation>
    </message>
    <message>
        <location filename="../src/dap/cmsisdap.cpp" line="584" />
        <source>The SWJ sequence failed.</source>
        <translation>Falló la secuencia SWJ.</translation>
    </message>
    <message>
        <location filename="../src/dap/cmsisdap.cpp" line="619" />
        <source>SWD clock: %1</source>
        <translation>Frecuencia SWD: %1</translation>
    </message>
    <message>
        <location filename="../src/dap/cmsisdap.cpp" line="624" />
        <source>The SWD clock could not be set: %1</source>
        <translation>No se pudo establecer la frecuencia SWD: %1</translation>
    </message>
    <message>
        <location filename="../src/dap/cmsisdap.cpp" line="649" />
        <source>The SWD DP did not answer, trying to revive the link.</source>
        <translation>El DP SWD no responde, intentando restablecer el enlace.</translation>
    </message>
    <message>
        <location filename="../src/dap/cmsisdap.cpp" line="706" />
        <source>The SWD link came back at %1 Hz.</source>
        <translation>El enlace SWD se restableció a %1 Hz.</translation>
    </message>
    <message>
        <location filename="../src/dap/cmsisdap.cpp" line="725" />
        <source>The SWD DP does not answer. Check the wiring (SWDIO, SWCLK, GND), the target power supply and the SWD clock. A target program that switched the debug interface off, or that put the core into a low power state, only comes back on a power cycle. If it comes back with BOOT0 held high, that program is what takes the debug pins over: start the ROM bootloader that way, which leaves the debug pins alone, and erase the chip from there.</source>
        <translation>El DP SWD no responde. Compruebe el cableado (SWDIO, SWCLK, GND), la alimentación del objetivo y la frecuencia SWD. Un programa del objetivo que haya desactivado la interfaz de depuración o que haya puesto el núcleo en bajo consumo solo vuelve tras un ciclo de alimentación. Si vuelve con BOOT0 a nivel alto, ese programa es el que se queda con los pines de depuración: arranque así el bootloader de la ROM, que no toca los pines de depuración, y borre el chip desde ahí.</translation>
    </message>
    <message>
        <location filename="../src/dap/cmsisdap.cpp" line="757" />
        <source>This probe does not drive an nRESET line.</source>
        <translation>Esta sonda no controla ninguna línea nRESET.</translation>
    </message>
    <message>
        <location filename="../src/dap/cmsisdap.cpp" line="764" />
        <source>Holding nRESET low and connecting under reset.</source>
        <translation>Manteniendo nRESET a nivel bajo y conectando en reset.</translation>
    </message>
    <message>
        <location filename="../src/dap/cmsisdap.cpp" line="799" />
        <source>The SWD link came back under reset at %1 Hz.</source>
        <translation>El enlace SWD se restableció en reset a %1 Hz.</translation>
    </message>
    <message>
        <location filename="../src/dap/cmsisdap.cpp" line="813" />
        <source>Connecting under reset did not bring the DP back either.</source>
        <translation>Conectar en reset tampoco recuperó el DP.</translation>
    </message>
    <message>
        <location filename="../src/dap/cmsisdap.cpp" line="818" />
        <source>The SWD DP did not answer while nRESET was being driven.</source>
        <translation>El DP SWD no respondió mientras se accionaba nRESET.</translation>
    </message>
    <message>
        <location filename="../src/dap/cmsisdap.cpp" line="832" />
        <source>The SWD DP reported an invalid IDCODE.</source>
        <translation>El DP SWD informó de un IDCODE no válido.</translation>
    </message>
    <message>
        <location filename="../src/dap/cmsisdap.cpp" line="873" />
        <source>The DP read returned no data.</source>
        <translation>La lectura del DP no devolvió datos.</translation>
    </message>
    <message>
        <location filename="../src/dap/cmsisdap.cpp" line="902" />
        <source>The AP read returned no data.</source>
        <translation>La lectura del AP no devolvió datos.</translation>
    </message>
    <message>
        <location filename="../src/dap/cmsisdap.cpp" line="945" />
        <source>Empty transfer.</source>
        <translation>Transferencia vacía.</translation>
    </message>
    <message>
        <location filename="../src/dap/cmsisdap.cpp" line="959" />
        <source>Internal error: the write value count does not match the request count.</source>
        <translation>Error interno: el número de valores de escritura no coincide con el de la solicitud.</translation>
    </message>
    <message>
        <location filename="../src/dap/cmsisdap.cpp" line="980" />
        <source>The probe answered with a truncated transfer response.</source>
        <translation>La sonda devolvió una respuesta de transferencia truncada.</translation>
    </message>
    <message>
        <location filename="../src/dap/cmsisdap.cpp" line="990" />
        <source>The probe answered with a malformed DAP_Transfer response.</source>
        <translation>La sonda devolvió una respuesta DAP_Transfer mal formada.</translation>
    </message>
    <message>
        <location filename="../src/dap/cmsisdap.cpp" line="1009" />
        <source>SWD fault (the target did not acknowledge the access).</source>
        <translation>Fallo SWD (el objetivo no confirmó el acceso).</translation>
    </message>
    <message>
        <location filename="../src/dap/cmsisdap.cpp" line="1015" />
        <source>Protocol error during the SWD transfer.</source>
        <translation>Error de protocolo durante la transferencia SWD.</translation>
    </message>
    <message>
        <location filename="../src/dap/cmsisdap.cpp" line="1021" />
        <source>The probe performed %1 of %2 transfers.</source>
        <translation>La sonda realizó %1 de %2 transferencias.</translation>
    </message>
    <message>
        <location filename="../src/dap/cmsisdap.cpp" line="1030" />
        <source>The probe sent a truncated transfer payload.</source>
        <translation>La sonda envió datos de transferencia truncados.</translation>
    </message>
    <message>
        <location filename="../src/dap/cmsisdap.cpp" line="1040" />
        <source>The SWD transfer was retried too often (WAIT response).</source>
        <translation>La transferencia SWD se reintentó demasiadas veces (respuesta WAIT).</translation>
    </message>
    <message>
        <location filename="../src/dap/cmsisdap.cpp" line="1051" />
        <source>Empty block transfer.</source>
        <translation>Transferencia por bloques vacía.</translation>
    </message>
    <message>
        <location filename="../src/dap/cmsisdap.cpp" line="1078" />
        <source>The probe answered with a truncated block transfer response.</source>
        <translation>La sonda devolvió una respuesta de transferencia por bloques truncada.</translation>
    </message>
    <message>
        <location filename="../src/dap/cmsisdap.cpp" line="1084" />
        <source>The probe answered with a malformed DAP_TransferBlock response.</source>
        <translation>La sonda devolvió una respuesta DAP_TransferBlock mal formada.</translation>
    </message>
    <message>
        <location filename="../src/dap/cmsisdap.cpp" line="1098" />
        <source>SWD fault during the block transfer.</source>
        <translation>Fallo SWD durante la transferencia por bloques.</translation>
    </message>
    <message>
        <location filename="../src/dap/cmsisdap.cpp" line="1104" />
        <source>Protocol error during the block transfer.</source>
        <translation>Error de protocolo durante la transferencia por bloques.</translation>
    </message>
    <message>
        <location filename="../src/dap/cmsisdap.cpp" line="1110" />
        <source>The probe performed %1 of %2 block transfers.</source>
        <translation>La sonda realizó %1 de %2 transferencias por bloques.</translation>
    </message>
    <message>
        <location filename="../src/dap/cmsisdap.cpp" line="1119" />
        <source>The probe sent a truncated block transfer payload.</source>
        <translation>La sonda envió datos de transferencia por bloques truncados.</translation>
    </message>
    <message>
        <location filename="../src/dap/cmsisdap.cpp" line="1297" />
        <source>The probe answered a poll with no data.</source>
        <translation>La sonda respondió a una consulta sin datos.</translation>
    </message>
</context>
<context>
    <name>CortexM</name>
    <message>
        <location filename="../src/target/cortexm.cpp" line="78" />
        <source>Unknown Cortex-M core</source>
        <translation>Núcleo Cortex-M desconocido</translation>
    </message>
    <message>
        <location filename="../src/target/cortexm.cpp" line="101" />
        <source>No Cortex-M core answered at CPUID (0x%1).</source>
        <translation>Ningún núcleo Cortex-M respondió en CPUID (0x%1).</translation>
    </message>
    <message>
        <location filename="../src/target/cortexm.cpp" line="302" />
        <source>The core is locked up (the flash algorithm crashed).</source>
        <translation>El núcleo está bloqueado (el algoritmo de programación se ha caído).</translation>
    </message>
    <message>
        <location filename="../src/target/cortexm.cpp" line="320" />
        <source>The core did not halt within %1 ms.</source>
        <translation>El núcleo no se detuvo en %1 ms.</translation>
    </message>
    <message>
        <location filename="../src/target/cortexm.cpp" line="460" />
        <source>The probe has no nRESET line; use the software reset instead.</source>
        <translation>La sonda no tiene línea nRESET; use el reset por software.</translation>
    </message>
    <message>
        <location filename="../src/target/cortexm.cpp" line="578" />
        <source>The core register transfer timed out.</source>
        <translation>Se agotó el tiempo de espera en la transferencia de registros del núcleo.</translation>
    </message>
    <message>
        <location filename="../src/target/cortexm.cpp" line="585" />
        <source>Internal error: no register buffer.</source>
        <translation>Error interno: no hay búfer de registros.</translation>
    </message>
</context>
<context>
    <name>DapTransport</name>
    <message>
        <location filename="../src/dap/daptransport.cpp" line="123" />
        <source>unknown HID error</source>
        <translation>error HID desconocido</translation>
    </message>
    <message>
        <location filename="../src/dap/daptransport.cpp" line="169" />
        <source>The HID probe could not be opened. It is probably used by another program (Keil, OpenOCD, a debug session).</source>
        <translation>No se pudo abrir la sonda HID. Probablemente la esté usando otro programa (Keil, OpenOCD, una sesión de depuración).</translation>
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
        <translation>Error al escribir HID: %1</translation>
    </message>
    <message>
        <location filename="../src/dap/daptransport.cpp" line="249" />
        <source>HID read failed: %1</source>
        <translation>Error al leer HID: %1</translation>
    </message>
    <message>
        <location filename="../src/dap/daptransport.cpp" line="254" />
        <location filename="../src/dap/daptransport.cpp" line="588" />
        <source>The probe did not answer (timeout).</source>
        <translation>La sonda no respondió (tiempo de espera agotado).</translation>
    </message>
    <message>
        <location filename="../src/dap/daptransport.cpp" line="321" />
        <source>The USB (libusb) layer could not be initialised.</source>
        <translation>No se pudo inicializar la capa USB (libusb).</translation>
    </message>
    <message>
        <location filename="../src/dap/daptransport.cpp" line="328" />
        <source>USB enumeration failed.</source>
        <translation>Falló la enumeración USB.</translation>
    </message>
    <message>
        <location filename="../src/dap/daptransport.cpp" line="343" />
        <source>The USB probe was not found any more. Replug it and refresh the list.</source>
        <translation>La sonda USB ya no se encuentra. Vuelva a conectarla y actualice la lista.</translation>
    </message>
    <message>
        <location filename="../src/dap/daptransport.cpp" line="364" />
        <source>Access to the USB probe was denied. It is either used by another program, or no WinUSB driver is bound to it.</source>
        <translation>Se denegó el acceso a la sonda USB. O la está usando otro programa, o no tiene ningún controlador WinUSB asociado.</translation>
    </message>
    <message>
        <location filename="../src/dap/daptransport.cpp" line="369" />
        <source>The probe is not bound to a WinUSB (or libusbK) driver. Install the driver, or select the same probe over its USB HID (CMSIS-DAP v1) interface.</source>
        <translation>La sonda no tiene asociado un controlador WinUSB (o libusbK). Instale el controlador o seleccione la misma sonda a través de su interfaz USB HID (CMSIS-DAP v1).</translation>
    </message>
    <message>
        <location filename="../src/dap/daptransport.cpp" line="375" />
        <source>Opening the USB probe failed: %1</source>
        <translation>Error al abrir la sonda USB: %1</translation>
    </message>
    <message>
        <location filename="../src/dap/daptransport.cpp" line="387" />
        <source>Claiming the USB interface failed: %1</source>
        <translation>Error al reclamar la interfaz USB: %1</translation>
    </message>
    <message>
        <location filename="../src/dap/daptransport.cpp" line="512" />
        <source>CMSIS-DAP bulk transport, spec framing (no length prefix)</source>
        <translation>Transporte CMSIS-DAP bulk, tramas según la especificación (sin prefijo de longitud)</translation>
    </message>
    <message>
        <location filename="../src/dap/daptransport.cpp" line="517" />
        <source>CMSIS-DAP bulk transport, length-prefixed framing (2 byte prefix)</source>
        <translation>Transporte CMSIS-DAP bulk, tramas con prefijo de longitud (prefijo de 2 bytes)</translation>
    </message>
    <message>
        <location filename="../src/dap/daptransport.cpp" line="559" />
        <source>USB bulk write failed: %1</source>
        <translation>Error al escribir USB bulk: %1</translation>
    </message>
    <message>
        <location filename="../src/dap/daptransport.cpp" line="592" />
        <source>USB bulk read failed: %1</source>
        <translation>Error al leer USB bulk: %1</translation>
    </message>
    <message>
        <location filename="../src/dap/daptransport.cpp" line="601" />
        <location filename="../src/dap/daptransport.cpp" line="611" />
        <source>The probe sent a malformed packet.</source>
        <translation>La sonda envió un paquete mal formado.</translation>
    </message>
    <message>
        <location filename="../src/dap/daptransport.cpp" line="683" />
        <source>The probe did not answer DAP_Info. Check that the right interface of the device was selected and that the firmware runs a CMSIS-DAP implementation.</source>
        <translation>La sonda no respondió a DAP_Info. Compruebe que se ha seleccionado la interfaz correcta del dispositivo y que el firmware implementa CMSIS-DAP.</translation>
    </message>
    <message>
        <location filename="../src/dap/daptransport.cpp" line="723" />
        <source>The command does not fit into one packet (%1 &gt; %2 bytes).</source>
        <translation>El comando no cabe en un paquete (%1 &gt; %2 bytes).</translation>
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
        <translation>No se pudo abrir el archivo del algoritmo: %1</translation>
    </message>
    <message>
        <location filename="../src/flash/elffile.cpp" line="69" />
        <source>The file is too small to be an ELF image.</source>
        <translation>El archivo es demasiado pequeño para ser una imagen ELF.</translation>
    </message>
    <message>
        <location filename="../src/flash/elffile.cpp" line="74" />
        <source>The file is not an ELF image (a Keil .FLM algorithm is expected).</source>
        <translation>El archivo no es una imagen ELF (se espera un algoritmo Keil .FLM).</translation>
    </message>
    <message>
        <location filename="../src/flash/elffile.cpp" line="79" />
        <source>Only 32 bit ELF images are supported (this one is 64 bit).</source>
        <translation>Solo se admiten imágenes ELF de 32 bits (esta es de 64 bits).</translation>
    </message>
    <message>
        <location filename="../src/flash/elffile.cpp" line="84" />
        <source>Only little endian ELF images are supported.</source>
        <translation>Solo se admiten imágenes ELF little endian.</translation>
    </message>
    <message>
        <location filename="../src/flash/elffile.cpp" line="90" />
        <source>The image is not an ARM image (e_machine = %1).</source>
        <translation>La imagen no es una imagen ARM (e_machine = %1).</translation>
    </message>
    <message>
        <location filename="../src/flash/elffile.cpp" line="123" />
        <source>The image is truncated (segment %1 leaves the file).</source>
        <translation>La imagen está truncada (el segmento %1 queda fuera del archivo).</translation>
    </message>
    <message>
        <location filename="../src/flash/elffile.cpp" line="133" />
        <source>The image carries no loadable segment.</source>
        <translation>La imagen no contiene ningún segmento cargable.</translation>
    </message>
</context>
<context>
    <name>FlashAlgo</name>
    <message>
        <location filename="../src/flash/flashalgo.cpp" line="162" />
        <source>The algorithm does not export an Init() function.</source>
        <translation>El algoritmo no exporta la función Init().</translation>
    </message>
    <message>
        <location filename="../src/flash/flashalgo.cpp" line="168" />
        <source>The algorithm does not export a ProgramPage() function.</source>
        <translation>El algoritmo no exporta la función ProgramPage().</translation>
    </message>
    <message>
        <location filename="../src/flash/flashalgo.cpp" line="174" />
        <source>The algorithm does not export EraseSector() nor EraseChip().</source>
        <translation>El algoritmo no exporta ni EraseSector() ni EraseChip().</translation>
    </message>
    <message>
        <location filename="../src/flash/flashalgo.cpp" line="209" />
        <source>The FlashDevice structure could not be located in the algorithm.</source>
        <translation>No se encontró la estructura FlashDevice en el algoritmo.</translation>
    </message>
    <message>
        <location filename="../src/flash/flashalgo.cpp" line="217" />
        <source>The FlashDevice structure at offset 0x%1 is not valid.</source>
        <translation>La estructura FlashDevice en el desplazamiento 0x%1 no es válida.</translation>
    </message>
    <message>
        <location filename="../src/flash/flashalgo.cpp" line="300" />
        <source>The algorithm carries an empty sector table.</source>
        <translation>El algoritmo contiene una tabla de sectores vacía.</translation>
    </message>
    <message>
        <location filename="../src/flash/flashalgo.cpp" line="339" />
        <location filename="../src/flash/flashalgo.cpp" line="347" />
        <source>Unknown</source>
        <translation>Desconocido</translation>
    </message>
    <message>
        <location filename="../src/flash/flashalgo.cpp" line="340" />
        <source>On-chip flash</source>
        <translation>Memoria flash interna</translation>
    </message>
    <message>
        <location filename="../src/flash/flashalgo.cpp" line="341" />
        <source>External flash, 8 bit bus</source>
        <translation>Memoria flash externa, bus de 8 bits</translation>
    </message>
    <message>
        <location filename="../src/flash/flashalgo.cpp" line="342" />
        <source>External flash, 16 bit bus</source>
        <translation>Memoria flash externa, bus de 16 bits</translation>
    </message>
    <message>
        <location filename="../src/flash/flashalgo.cpp" line="343" />
        <source>External flash, 32 bit bus</source>
        <translation>Memoria flash externa, bus de 32 bits</translation>
    </message>
    <message>
        <location filename="../src/flash/flashalgo.cpp" line="344" />
        <source>External SPI flash</source>
        <translation>Memoria flash SPI externa</translation>
    </message>
</context>
<context>
    <name>FlashWorker</name>
    <message>
        <location filename="../src/flashworker.cpp" line="72" />
        <source>Loading the flash algorithm ...</source>
        <translation>Cargando el algoritmo de programación...</translation>
    </message>
    <message>
        <location filename="../src/flashworker.cpp" line="81" />
        <source>Algorithm %1: %2, %3, page %4 bytes, %5 sectors</source>
        <translation>Algoritmo %1: %2, %3, página de %4 bytes, %5 sectores</translation>
    </message>
    <message>
        <location filename="../src/flashworker.cpp" line="104" />
        <source>%1: %2 in %3 blocks, 0x%4 - 0x%5, checksum 0x%6</source>
        <translation>%1: %2 en %3 bloques, 0x%4 - 0x%5, suma de comprobación 0x%6</translation>
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
        <translation>Firmware %1</translation>
    </message>
    <message>
        <location filename="../src/flashworker.cpp" line="117" />
        <source>The firmware (0x%1 - 0x%2) does not fit into the flash of the algorithm (0x%3 - 0x%4). Check the loading address.</source>
        <translation>El firmware (0x%1 - 0x%2) no cabe en la flash del algoritmo (0x%3 - 0x%4). Compruebe la dirección de carga.</translation>
    </message>
    <message>
        <location filename="../src/flashworker.cpp" line="127" />
        <source>The algorithm has no sector at 0x%1.</source>
        <translation>El algoritmo no tiene sector en 0x%1.</translation>
    </message>
    <message>
        <location filename="../src/flashworker.cpp" line="132" />
        <source>No firmware given: the whole chip is erased and the target is left halted.</source>
        <translation>No se ha indicado firmware: se borra todo el chip y el objetivo queda detenido.</translation>
    </message>
    <message>
        <location filename="../src/flashworker.cpp" line="137" />
        <location filename="../src/flashworker.cpp" line="244" />
        <location filename="../src/flashworker.cpp" line="245" />
        <source>Cancelled.</source>
        <translation>Cancelado.</translation>
    </message>
    <message>
        <location filename="../src/flashworker.cpp" line="155" />
        <source>Probe: %1</source>
        <translation>Sonda: %1</translation>
    </message>
    <message>
        <location filename="../src/flashworker.cpp" line="159" />
        <source>CMSIS-DAP firmware: %1</source>
        <translation>Firmware CMSIS-DAP: %1</translation>
    </message>
    <message>
        <location filename="../src/flashworker.cpp" line="183" />
        <source>Target: %1 (CPUID 0x%2, DP IDCODE 0x%3)</source>
        <translation>Objetivo: %1 (CPUID 0x%2, DP IDCODE 0x%3)</translation>
    </message>
    <message>
        <location filename="../src/flashworker.cpp" line="254" />
        <source>The whole chip was erased in %1 s; the target is left halted.</source>
        <translation>Se borró todo el chip en %1 s; el objetivo queda detenido.</translation>
    </message>
    <message>
        <location filename="../src/flashworker.cpp" line="257" />
        <source>Programming finished in %1 s: %2 pages, %3 bytes, %4 sectors erased.</source>
        <translation>Programación terminada en %1 s: %2 páginas, %3 bytes, %4 sectores borrados.</translation>
    </message>
    <message>
        <location filename="../src/flashworker.cpp" line="264" />
        <source>The algorithm used 0x%1 of RAM at %2.</source>
        <translation>El algoritmo usó 0x%1 de RAM en %2.</translation>
    </message>
    <message>
        <location filename="../src/flashworker.cpp" line="300" />
        <source>The SWD link failed at %1; retrying at %2.</source>
        <translation>El enlace SWD falló a %1; reintentando a %2.</translation>
    </message>
    <message>
        <location filename="../src/flashworker.cpp" line="306" />
        <source>Retrying at a slower clock ...</source>
        <translation>Reintentando con una frecuencia más baja...</translation>
    </message>
</context>
<context>
    <name>Flasher</name>
    <message>
        <location filename="../src/flash/flasher.cpp" line="110" />
        <source>The algorithm image is empty.</source>
        <translation>La imagen del algoritmo está vacía.</translation>
    </message>
    <message>
        <location filename="../src/flash/flasher.cpp" line="116" />
        <source>The algorithm reports an unusable page size (%1 bytes).</source>
        <translation>El algoritmo indica un tamaño de página no utilizable (%1 bytes).</translation>
    </message>
    <message>
        <location filename="../src/flash/flasher.cpp" line="128" />
        <source>The RAM base address is too high for the algorithm.</source>
        <translation>La dirección base de RAM es demasiado alta para el algoritmo.</translation>
    </message>
    <message>
        <location filename="../src/flash/flasher.cpp" line="230" />
        <source>The algorithm could not be written to the target RAM at %1: %2 of %3 bytes came back different, the first one at %4 (wrote 0x%5, read 0x%6). The base address has to be RAM the debug unit can write to; if it is, the probe did not move the data reliably.</source>
        <translation>No se pudo escribir el algoritmo en la RAM del objetivo en %1: %2 de %3 bytes se leyeron distintos, el primero en %4 (escrito 0x%5, leído 0x%6). La dirección base debe ser RAM en la que la unidad de depuración pueda escribir; si lo es, la sonda no transmitió los datos de forma fiable.</translation>
    </message>
    <message>
        <location filename="../src/flash/flasher.cpp" line="260" />
        <source>The algorithm does not provide %1().</source>
        <translation>El algoritmo no proporciona %1().</translation>
    </message>
    <message>
        <location filename="../src/flash/flasher.cpp" line="293" />
        <source>%1() did not return: %2 (PC 0x%3, DFSR 0x%4, CFSR 0x%5, HFSR 0x%6)</source>
        <translation>%1() no regresó: %2 (PC 0x%3, DFSR 0x%4, CFSR 0x%5, HFSR 0x%6)</translation>
    </message>
    <message>
        <location filename="../src/flash/flasher.cpp" line="313" />
        <source>%1() stopped at 0x%2 instead of returning to 0x%3 (DFSR = 0x%4).</source>
        <translation>%1() se detuvo en 0x%2 en lugar de volver a 0x%3 (DFSR = 0x%4).</translation>
    </message>
    <message>
        <location filename="../src/flash/flasher.cpp" line="340" />
        <source>Erasing the whole chip ...</source>
        <translation>Borrando todo el chip...</translation>
    </message>
    <message>
        <location filename="../src/flash/flasher.cpp" line="351" />
        <source>EraseChip() failed with code %1.</source>
        <translation>EraseChip() falló con el código %1.</translation>
    </message>
    <message>
        <location filename="../src/flash/flasher.cpp" line="356" />
        <source>Chip erase done.</source>
        <translation>Borrado del chip completado.</translation>
    </message>
    <message>
        <location filename="../src/flash/flasher.cpp" line="361" />
        <source>The algorithm provides neither EraseChip() nor EraseSector().</source>
        <translation>El algoritmo no proporciona ni EraseChip() ni EraseSector().</translation>
    </message>
    <message>
        <location filename="../src/flash/flasher.cpp" line="376" />
        <source>Address 0x%1 is not covered by the sector table of the algorithm.</source>
        <translation>La dirección 0x%1 no está cubierta por la tabla de sectores del algoritmo.</translation>
    </message>
    <message>
        <location filename="../src/flash/flasher.cpp" line="396" />
        <source>Nothing to erase.</source>
        <translation>No hay nada que borrar.</translation>
    </message>
    <message>
        <location filename="../src/flash/flasher.cpp" line="402" />
        <location filename="../src/flash/flasher.cpp" line="471" />
        <location filename="../src/flash/flasher.cpp" line="585" />
        <source>Cancelled.</source>
        <translation>Cancelado.</translation>
    </message>
    <message>
        <location filename="../src/flash/flasher.cpp" line="411" />
        <source>Erasing sector %1 of %2 (0x%3) ...</source>
        <translation>Borrando el sector %1 de %2 (0x%3)...</translation>
    </message>
    <message>
        <location filename="../src/flash/flasher.cpp" line="425" />
        <source>Erasing the sector at 0x%1 failed with code %2.</source>
        <translation>El borrado del sector en 0x%1 falló con el código %2.</translation>
    </message>
    <message>
        <location filename="../src/flash/flasher.cpp" line="455" />
        <source>The firmware file carries no data.</source>
        <translation>El archivo de firmware no contiene datos.</translation>
    </message>
    <message>
        <location filename="../src/flash/flasher.cpp" line="507" />
        <source>Programming 0x%1 (%2 / %3 bytes) ...</source>
        <translation>Programando 0x%1 (%2 / %3 bytes)...</translation>
    </message>
    <message>
        <location filename="../src/flash/flasher.cpp" line="533" />
        <source>Programming the page at 0x%1 failed with code %2.</source>
        <translation>La programación de la página en 0x%1 falló con el código %2.</translation>
    </message>
    <message>
        <location filename="../src/flash/flasher.cpp" line="569" />
        <source>The algorithm has no Verify() routine; the flash is read back through the debug port instead.</source>
        <translation>El algoritmo no tiene rutina Verify(); la flash se lee de vuelta a través del puerto de depuración.</translation>
    </message>
    <message>
        <location filename="../src/flash/flasher.cpp" line="605" />
        <source>Verifying 0x%1 ...</source>
        <translation>Verificando 0x%1...</translation>
    </message>
    <message>
        <location filename="../src/flash/flasher.cpp" line="622" />
        <source>Verification failed at 0x%1 (code %2).</source>
        <translation>La verificación falló en 0x%1 (código %2).</translation>
    </message>
    <message>
        <location filename="../src/flash/flasher.cpp" line="630" />
        <source>Reading back 0x%1 failed; the verification was skipped.</source>
        <translation>No se pudo releer 0x%1; se omitió la verificación.</translation>
    </message>
    <message>
        <location filename="../src/flash/flasher.cpp" line="644" />
        <source>Verification failed at 0x%1: expected 0x%2, read 0x%3.</source>
        <translation>La verificación falló en 0x%1: se esperaba 0x%2, se leyó 0x%3.</translation>
    </message>
    <message>
        <location filename="../src/flash/flasher.cpp" line="671" />
        <source>No flash algorithm is selected.</source>
        <translation>No hay ningún algoritmo de programación seleccionado.</translation>
    </message>
    <message>
        <location filename="../src/flash/flasher.cpp" line="681" />
        <source>No firmware data to program.</source>
        <translation>No hay datos de firmware que programar.</translation>
    </message>
    <message>
        <location filename="../src/flash/flasher.cpp" line="693" />
        <source>Preparing ...</source>
        <translation>Preparando...</translation>
    </message>
    <message>
        <location filename="../src/flash/flasher.cpp" line="696" />
        <source>Algorithm: %1 (%2, %3 bytes)</source>
        <translation>Algoritmo: %1 (%2, %3 bytes)</translation>
    </message>
    <message>
        <location filename="../src/flash/flasher.cpp" line="729" />
        <location filename="../src/flash/flasher.cpp" line="867" />
        <source>Reset: %1</source>
        <translation>Reset: %1</translation>
    </message>
    <message>
        <location filename="../src/flash/flasher.cpp" line="738" />
        <source>RAM transfer: %1 words (%2 bytes) per DAP_TransferBlock, command packet %3 bytes.</source>
        <translation>Transferencia a RAM: %1 palabras (%2 bytes) por DAP_TransferBlock, paquete de comando de %3 bytes.</translation>
    </message>
    <message>
        <location filename="../src/flash/flasher.cpp" line="751" />
        <source>Writing the algorithm to the RAM failed, attempt %1 of %2: %3</source>
        <translation>Error al escribir el algoritmo en la RAM, intento %1 de %2: %3</translation>
    </message>
    <message>
        <location filename="../src/flash/flasher.cpp" line="763" />
        <source>Algorithm loaded at 0x%1 (%2 bytes of RAM).</source>
        <translation>Algoritmo cargado en 0x%1 (%2 bytes de RAM).</translation>
    </message>
    <message>
        <location filename="../src/flash/flasher.cpp" line="782" />
        <source>Init() did not complete, restarting the algorithm (%1 of %2): %3</source>
        <translation>Init() no terminó, reiniciando el algoritmo (%1 de %2): %3</translation>
    </message>
    <message>
        <location filename="../src/flash/flasher.cpp" line="807" />
        <source>Init() failed with code %1.</source>
        <translation>Init() falló con el código %1.</translation>
    </message>
    <message>
        <location filename="../src/flash/flasher.cpp" line="816" />
        <source>Initialised.</source>
        <translation>Inicializado.</translation>
    </message>
    <message>
        <location filename="../src/flash/flasher.cpp" line="854" />
        <source>UnInit() failed: %1</source>
        <translation>Error en UnInit(): %1</translation>
    </message>
    <message>
        <location filename="../src/flash/flasher.cpp" line="856" />
        <source>UnInit() returned code %1.</source>
        <translation>UnInit() devolvió el código %1.</translation>
    </message>
    <message>
        <location filename="../src/flash/flasher.cpp" line="865" />
        <source>Resetting the target failed: %1</source>
        <translation>No se pudo reiniciar el objetivo: %1</translation>
    </message>
    <message>
        <location filename="../src/flash/flasher.cpp" line="874" />
        <source>Done.</source>
        <translation>Hecho.</translation>
    </message>
</context>
<context>
    <name>LanguageManager</name>
    <message>
        <location filename="../src/languagemanager.cpp" line="129" />
        <source>Follow System(&amp;A)</source>
        <translation>Seguir el sistema(&amp;A)</translation>
    </message>
    <message>
        <location filename="../src/languagemanager.cpp" line="130" />
        <source>English(&amp;E)</source>
        <translation>English(&amp;E)</translation>
    </message>
    <message>
        <location filename="../src/languagemanager.cpp" line="131" />
        <source>Simplified Chinese(&amp;C)</source>
        <translation>Chino simplificado(&amp;C)</translation>
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
        <translation>Chino simplificado</translation>
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
        <translation>Sonda de depuración</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="482" />
        <source>Flash Algorithm</source>
        <translation>Algoritmo de programación</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="483" />
        <source>Firmware File</source>
        <translation>Archivo de firmware</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="484" />
        <source>Options</source>
        <translation>Opciones</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="486" />
        <source>Probe:</source>
        <translation>Sonda:</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="487" />
        <source>Connection:</source>
        <translation>Conexión:</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="488" />
        <source>Target:</source>
        <translation>Objetivo:</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="489" />
        <source>Algorithm File:</source>
        <translation>Archivo de algoritmo:</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="490" />
        <source>File Path:</source>
        <translation>Ruta del archivo:</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="491" />
        <source>Firmware:</source>
        <translation>Firmware:</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="492" />
        <source>BIN Load Address: 0x</source>
        <translation>Dirección de carga BIN: 0x</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="493" />
        <source>SWD Clock:</source>
        <translation>Frecuencia SWD:</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="494" />
        <source>Algorithm RAM: 0x</source>
        <translation>RAM del algoritmo: 0x</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="495" />
        <source>Reset Mode:</source>
        <translation>Modo de reset:</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="497" />
        <source>Refresh(&amp;R)</source>
        <translation>Actualizar(&amp;R)</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="498" />
        <location filename="../src/mainwindow.cpp" line="499" />
        <source>Browse(&amp;B)...</source>
        <translation>Examinar(&amp;B)...</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="501" />
        <source>Whole chip erase</source>
        <translation>Borrar todo el chip</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="502" />
        <source>Verify</source>
        <translation>Verificar</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="503" />
        <source>Reset and run</source>
        <translation>Reset y ejecutar</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="513" />
        <source>Cancel(&amp;C)</source>
        <translation>Cancelar(&amp;C)</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="513" />
        <source>Start Programming(&amp;S)</source>
        <translation>Iniciar programación(&amp;S)</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="514" />
        <source>Halt(&amp;P)</source>
        <translation>Detener(&amp;P)</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="515" />
        <source>Run(&amp;R)</source>
        <translation>Ejecutar(&amp;R)</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="516" />
        <source>Reset(&amp;T)</source>
        <translation>Reset(&amp;T)</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="517" />
        <location filename="../src/mainwindow.cpp" line="536" />
        <source>Exit(&amp;X)</source>
        <translation>Salir(&amp;X)</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="519" />
        <source>Select or drop a .hex / .bin firmware file</source>
        <translation>Seleccione o arrastre un archivo de firmware .hex / .bin</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="523" />
        <source>RAM area the flash algorithm is copied to while it runs. 0x20000000 is correct for most Cortex-M devices.</source>
        <translation>Área de RAM a la que se copia el algoritmo de programación mientras se ejecuta. 0x20000000 es correcta para la mayoría de los dispositivos Cortex-M.</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="525" />
        <source>Loading address used for .bin files (ignored for .hex)</source>
        <translation>Dirección de carga usada para archivos .bin (se ignora en .hex)</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="526" />
        <source>Reset used by the Reset button and by "Reset and run". Software reset needs no extra wire, hardware reset pulses the nRESET line, automatic uses nRESET when the probe has one.</source>
        <translation>Reset usado por el botón Reset y por «Reset y ejecutar». El reset por software no necesita ningún cable adicional, el reset por hardware genera un pulso en la línea nRESET y el automático usa nRESET cuando la sonda dispone de ella.</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="530" />
        <source>Log</source>
        <translation>Registro</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="533" />
        <source>File(&amp;F)</source>
        <translation>Archivo(&amp;F)</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="534" />
        <source>Open Firmware(&amp;O)...</source>
        <translation>Abrir firmware(&amp;O)...</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="535" />
        <source>Select Algorithm(&amp;A)...</source>
        <translation>Seleccionar algoritmo(&amp;A)...</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="538" />
        <source>Tools(&amp;T)</source>
        <translation>Herramientas(&amp;T)</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="539" />
        <source>Refresh Probes(&amp;R)</source>
        <translation>Actualizar sondas(&amp;R)</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="540" />
        <source>Rescan Algorithms(&amp;L)</source>
        <translation>Reexplorar algoritmos(&amp;L)</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="541" />
        <source>Show All USB HID Devices(&amp;H)</source>
        <translation>Mostrar todos los dispositivos USB HID(&amp;H)</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="542" />
        <source>Clear Log(&amp;C)</source>
        <translation>Borrar registro(&amp;C)</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="544" />
        <source>View(&amp;V)</source>
        <translation>Ver(&amp;V)</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="545" />
        <source>Language(&amp;L)</source>
        <translation>Idioma(&amp;L)</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="546" />
        <source>Help(&amp;H)</source>
        <translation>Ayuda(&amp;H)</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="547" />
        <source>About(&amp;A)</source>
        <translation>Acerca de(&amp;A)</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="713" />
        <source>(no probe found)</source>
        <translation>(no se encontró ninguna sonda)</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="792" />
        <source>Identifying the target over %1 ...</source>
        <translation>Identificando el objetivo por %1...</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="863" />
        <source>&lt;font color='%1'&gt;Identifying the target ...&lt;/font&gt;</source>
        <translation>&lt;font color='%1'&gt;Identificando el objetivo...&lt;/font&gt;</translation>
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
        <translation>&lt;font color='%1'&gt;No se encontró ninguna sonda&lt;/font&gt;</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="931" />
        <source>Connect a CMSIS-DAP probe and press Refresh.</source>
        <translation>Conecte una sonda CMSIS-DAP y pulse Actualizar.</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="939" />
        <source>&lt;font color='%1'&gt;Connected (%2)&lt;/font&gt;</source>
        <translation>&lt;font color='%1'&gt;Conectado (%2)&lt;/font&gt;</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="941" />
        <source>SWD</source>
        <translation>SWD</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="943" />
        <source>CMSIS-DAP firmware: %1</source>
        <translation>Firmware CMSIS-DAP: %1</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="944" />
        <source>unknown</source>
        <translation>desconocido</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="949" />
        <source>CPUID %1, DP IDCODE %2, core %3</source>
        <translation>CPUID %1, DP IDCODE %2, núcleo %3</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="952" />
        <source>halted</source>
        <translation>detenido</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="952" />
        <source>running</source>
        <translation>en ejecución</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="955" />
        <source>&lt;font color='%1'&gt;Probe present, target not identified&lt;/font&gt;</source>
        <translation>&lt;font color='%1'&gt;Sonda presente, objetivo no identificado&lt;/font&gt;</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="1025" />
        <source>Scanning flash algorithms (%1 pack folders) ...</source>
        <translation>Explorando algoritmos de programación (%1 carpetas de paquetes)...</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="1066" />
        <source>%1 flash algorithms found in %2 groups.</source>
        <translation>Se encontraron %1 algoritmos de programación en %2 grupos.</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="1071" />
        <source>%1 file(s) were skipped: they carry no usable FlashDevice structure.</source>
        <translation>Se omitieron %1 archivo(s): no contienen una estructura FlashDevice utilizable.</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="1158" />
        <source>Recently used</source>
        <translation>Usados recientemente</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="1179" />
        <source>Additional folder: %1</source>
        <translation>Carpeta adicional: %1</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="1206" />
        <source>Last used</source>
        <translation>Último usado</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="1251" />
        <source>Select a Keil Flash Algorithm</source>
        <translation>Seleccionar un algoritmo de programación Keil</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="1287" />
        <source>The algorithm %1 could not be loaded: %2</source>
        <translation>No se pudo cargar el algoritmo %1: %2</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="1290" />
        <source>Algorithm loaded: %1 (%2)</source>
        <translation>Algoritmo cargado: %1 (%2)</translation>
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
        <translation>&lt;font color='%1'&gt;Ningún algoritmo seleccionado&lt;/font&gt;</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="1314" />
        <source>&lt;b&gt;%1&lt;/b&gt;&lt;br&gt;%2, flash 0x%3 - 0x%4 (%5), page %6 bytes, %7 sectors</source>
        <translation>&lt;b&gt;%1&lt;/b&gt;&lt;br&gt;%2, flash 0x%3 - 0x%4 (%5), página de %6 bytes, %7 sectores</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="1324" />
        <source>&lt;br&gt;&lt;font color='%1'&gt;This algorithm has no chip erase; the sectors are erased instead.&lt;/font&gt;</source>
        <translation>&lt;br&gt;&lt;font color='%1'&gt;Este algoritmo no permite borrar todo el chip; se borran los sectores.&lt;/font&gt;</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="1342" />
        <source>Select Firmware File</source>
        <translation>Seleccionar archivo de firmware</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="1344" />
        <source>Firmware Files (*.hex *.bin *.ihex);;Intel HEX (*.hex *.ihex);;Binary Files (*.bin);;All Files (*.*)</source>
        <translation>Archivos de firmware (*.hex *.bin *.ihex);;Intel HEX (*.hex *.ihex);;Archivos binarios (*.bin);;Todos los archivos (*.*)</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="1362" />
        <source>&lt;font color='%1'&gt;No valid firmware file selected&lt;/font&gt;</source>
        <translation>&lt;font color='%1'&gt;No se ha seleccionado un archivo de firmware válido&lt;/font&gt;</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="1380" />
        <source>%1  Size: %2 in %3 blocks  Address: 0x%4 - 0x%5  Checksum: 0x%6</source>
        <translation>%1  Tamaño: %2 en %3 bloques  Dirección: 0x%4 - 0x%5  Suma de comprobación: 0x%6</translation>
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
        <translation>Sin sonda</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="1445" />
        <location filename="../src/mainwindow.cpp" line="1480" />
        <location filename="../src/mainwindow.cpp" line="1506" />
        <source>Select a debug probe first.</source>
        <translation>Seleccione primero una sonda de depuración.</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="1465" />
        <source>Halt Failed</source>
        <translation>Error al detener</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="1469" />
        <source>The core is halted.</source>
        <translation>El núcleo está detenido.</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="1491" />
        <source>Run Failed</source>
        <translation>Error al ejecutar</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="1495" />
        <source>The core is running.</source>
        <translation>El núcleo está en ejecución.</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="1517" />
        <source>Reset Failed</source>
        <translation>Error de reset</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="1521" />
        <source>The target was reset and is running.</source>
        <translation>El objetivo se ha reiniciado y está en ejecución.</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="1568" />
        <source>Select a CMSIS-DAP probe first.</source>
        <translation>Seleccione primero una sonda CMSIS-DAP.</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="1573" />
        <source>No Algorithm</source>
        <translation>Sin algoritmo</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="1574" />
        <source>Select the Keil .FLM flash algorithm that matches the device.</source>
        <translation>Seleccione el algoritmo de programación Keil .FLM que corresponda al dispositivo.</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="1580" />
        <source>No Firmware</source>
        <translation>Sin firmware</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="1581" />
        <source>Select a valid firmware file first.</source>
        <translation>Seleccione primero un archivo de firmware válido.</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="1602" />
        <source>Preparing ...</source>
        <translation>Preparando...</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="1627" />
        <source>Cancelling ...</source>
        <translation>Cancelando...</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="1650" />
        <location filename="../src/mainwindow.cpp" line="1687" />
        <source>Programming complete</source>
        <translation>Programación completada</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="1656" />
        <source>Complete</source>
        <translation>Completado</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="1657" />
        <source>The firmware was programmed successfully.

%1</source>
        <translation>El firmware se programó correctamente.

%1</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="1660" />
        <source>Cancelled</source>
        <translation>Cancelado</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="1664" />
        <location filename="../src/mainwindow.cpp" line="1690" />
        <source>Programming failed</source>
        <translation>Error de programación</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="1666" />
        <source>Failed</source>
        <translation>Error</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="1694" />
        <source>Ready</source>
        <translation>Listo</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="1761" />
        <source>About</source>
        <translation>Acerca de</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="1765" />
        <source>&lt;h3&gt;WeAct Studio DAP Flasher&lt;/h3&gt;&lt;p&gt;Version: %1&lt;/p&gt;&lt;p&gt;&lt;b&gt;WeAct Studio&lt;/b&gt;&lt;br&gt;Copyright (C) 2026 WeAct Studio. All rights reserved.&lt;/p&gt;&lt;p&gt;Programs Cortex-M devices through a CMSIS-DAP probe (USB HID or USB bulk / WinUSB) using the Keil .FLM flash algorithms.&lt;/p&gt;&lt;p&gt;This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version. It is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.&lt;/p&gt;&lt;p&gt;License: GPL-3.0-or-later&lt;br&gt;&lt;a href='https://www.gnu.org/licenses/gpl-3.0.html'&gt;https://www.gnu.org/licenses/gpl-3.0.html&lt;/a&gt;&lt;/p&gt;&lt;p&gt;Built with Qt %2, statically compiled with MSYS2 (MINGW64).&lt;/p&gt;</source>
        <translation>&lt;h3&gt;WeAct Studio DAP Flasher&lt;/h3&gt;&lt;p&gt;Versión: %1&lt;/p&gt;&lt;p&gt;&lt;b&gt;WeAct Studio&lt;/b&gt;&lt;br&gt;Copyright (C) 2026 WeAct Studio. Todos los derechos reservados.&lt;/p&gt;&lt;p&gt;Programa dispositivos Cortex-M a través de una sonda CMSIS-DAP (USB HID o USB bulk / WinUSB) usando los algoritmos de programación Keil .FLM.&lt;/p&gt;&lt;p&gt;Este programa es software libre: puede redistribuirlo y/o modificarlo bajo los términos de la Licencia Pública General de GNU publicada por la Free Software Foundation, ya sea la versión 3 de la Licencia o (a su elección) cualquier versión posterior. Se distribuye con la esperanza de que sea útil, pero SIN NINGUNA GARANTÍA, ni siquiera la garantía implícita de COMERCIABILIDAD o IDONEIDAD PARA UN PROPÓSITO PARTICULAR. Consulte la Licencia Pública General de GNU para más detalles.&lt;/p&gt;&lt;p&gt;Licencia: GPL-3.0-or-later&lt;br&gt;&lt;a href='https://www.gnu.org/licenses/gpl-3.0.html'&gt;https://www.gnu.org/licenses/gpl-3.0.html&lt;/a&gt;&lt;/p&gt;&lt;p&gt;Compilado con Qt %2, compilado estáticamente con MSYS2 (MINGW64).&lt;/p&gt;</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="1821" />
        <source>Programming in Progress</source>
        <translation>Programación en curso</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="1822" />
        <source>Programming is still running; stopping now can leave the device with an incomplete firmware.

Exit anyway?</source>
        <translation>La programación sigue en curso; detenerla ahora puede dejar el dispositivo con un firmware incompleto.

¿Salir de todos modos?</translation>
    </message>
</context>
<context>
    <name>Probes</name>
    <message>
        <location filename="../src/dap/probes.cpp" line="73" />
        <source>The HID interface could not be initialised.</source>
        <translation>No se pudo inicializar la interfaz HID.</translation>
    </message>
    <message>
        <location filename="../src/dap/probes.cpp" line="82" />
        <source>HID init failed: %1</source>
        <translation>Error al inicializar HID: %1</translation>
    </message>
    <message>
        <location filename="../src/dap/probes.cpp" line="83" />
        <source>HID init failed.</source>
        <translation>Error al inicializar HID.</translation>
    </message>
    <message>
        <location filename="../src/dap/probes.cpp" line="287" />
        <source>HID device</source>
        <translation>Dispositivo HID</translation>
    </message>
    <message>
        <location filename="../src/dap/probes.cpp" line="307" />
        <source> (in use)</source>
        <translation> (en uso)</translation>
    </message>
    <message>
        <location filename="../src/dap/probes.cpp" line="318" />
        <source>Transport: %1</source>
        <translation>Transporte: %1</translation>
    </message>
    <message>
        <location filename="../src/dap/probes.cpp" line="324" />
        <source>Product: %1</source>
        <translation>Producto: %1</translation>
    </message>
    <message>
        <location filename="../src/dap/probes.cpp" line="326" />
        <source>Manufacturer: %1</source>
        <translation>Fabricante: %1</translation>
    </message>
    <message>
        <location filename="../src/dap/probes.cpp" line="328" />
        <source>Serial: %1</source>
        <translation>Número de serie: %1</translation>
    </message>
    <message>
        <location filename="../src/dap/probes.cpp" line="331" />
        <source>USB: bus %1 address %2, interface %3, EP IN 0x%4 / OUT 0x%5</source>
        <translation>USB: bus %1 dirección %2, interfaz %3, EP IN 0x%4 / OUT 0x%5</translation>
    </message>
    <message>
        <location filename="../src/dap/probes.cpp" line="339" />
        <source>Interface: %1</source>
        <translation>Interfaz: %1</translation>
    </message>
    <message>
        <location filename="../src/dap/probes.cpp" line="340" />
        <source>Usage page: 0x%1  Usage: 0x%2</source>
        <translation>Página de uso: 0x%1  Uso: 0x%2</translation>
    </message>
</context>
<context>
    <name>ResetMode</name>
    <message>
        <location filename="../src/target/resetmode.cpp" line="18" />
        <source>Hardware reset</source>
        <translation>Reset por hardware</translation>
    </message>
    <message>
        <location filename="../src/target/resetmode.cpp" line="20" />
        <source>Automatic</source>
        <translation>Automático</translation>
    </message>
    <message>
        <location filename="../src/target/resetmode.cpp" line="25" />
        <source>Software reset</source>
        <translation>Reset por software</translation>
    </message>
</context>
<context>
    <name>TargetSession</name>
    <message>
        <location filename="../src/targetsession.cpp" line="30" />
        <source>Probe: %1</source>
        <translation>Sonda: %1</translation>
    </message>
    <message>
        <location filename="../src/targetsession.cpp" line="83" />
        <source>The target program was holding the debug access; the core is now halted under reset.</source>
        <translation>El programa del objetivo retenía el acceso de depuración; ahora el núcleo está detenido en reset.</translation>
    </message>
    <message>
        <location filename="../src/targetsession.cpp" line="191" />
        <source>Reset: %1</source>
        <translation>Reset: %1</translation>
    </message>
    <message>
        <location filename="../src/targetsession.cpp" line="225" />
        <source>Reset: %1 (the DP did not answer, so this was a pin reset only).</source>
        <translation>Reset: %1 (el DP no respondió, así que solo se hizo un reset por línea).</translation>
    </message>
</context>
<context>
    <name>ThemeManager</name>
    <message>
        <location filename="../src/thememanager.cpp" line="166" />
        <source>Follow System(&amp;S)</source>
        <translation>Seguir el sistema(&amp;S)</translation>
    </message>
    <message>
        <location filename="../src/thememanager.cpp" line="168" />
        <source>Light(&amp;L)</source>
        <translation>Claro(&amp;L)</translation>
    </message>
    <message>
        <location filename="../src/thememanager.cpp" line="171" />
        <source>Dark(&amp;D)</source>
        <translation>Oscuro(&amp;D)</translation>
    </message>
</context>
</TS>