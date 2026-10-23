# -*- coding: utf-8 -*-
"""German dictionary: English source text -> German translation."""

DE = {
    # ---------------------------------------------------------------- AlgoCatalog
    "Keil built-in algorithms": "In Keil enthaltene Algorithmen",
    "Additional folder": "Zusätzlicher Ordner",
    "Flash Algorithms (*.flm *.FLM *.flx *.xldr);;All Files (*.*)":
        "Flash-Algorithmen (*.flm *.FLM *.flx *.xldr);;Alle Dateien (*.*)",

    # ------------------------------------------------------------ AlgorithmPicker
    "Filter algorithms ...": "Algorithmen filtern...",
    "Select algorithm ...": "Algorithmus auswählen...",

    # ---------------------------------------------------------------------- CmsisDap
    "No probe is open.": "Es ist keine Probe geöffnet.",
    "The probe sent an empty answer.": "Die Probe hat eine leere Antwort gesendet.",
    "The probe answered with a malformed DAP_Info response.":
        "Die Probe hat eine fehlerhafte DAP_Info-Antwort gesendet.",
    "The probe sent a truncated DAP_Info response.":
        "Die Probe hat eine abgeschnittene DAP_Info-Antwort gesendet.",
    "The probe answered with a malformed DAP_Connect response.":
        "Die Probe hat eine fehlerhafte DAP_Connect-Antwort gesendet.",
    "The probe refused the SWD connection.": "Die Probe hat die SWD-Verbindung abgelehnt.",
    "The DP did not acknowledge the power up request within %1 ms.":
        "Der DP hat die Einschaltanforderung nicht innerhalb von %1 ms bestätigt.",
    "Setting the SWD clock failed.":
        "Die SWD-Taktfrequenz konnte nicht gesetzt werden.",
    "Configuring the DAP transfer failed.":
        "Die DAP-Übertragung konnte nicht konfiguriert werden.",
    "The probe answered with a malformed DAP_SWJ_Pins response.":
        "Die Probe hat eine fehlerhafte DAP_SWJ_Pins-Antwort gesendet.",
    "The probe answered with a malformed DAP_Reset_Target response.":
        "Die Probe hat eine fehlerhafte DAP_Reset_Target-Antwort gesendet.",
    "The SWJ sequence failed.": "Die SWJ-Sequenz ist fehlgeschlagen.",
    "SWD clock: %1": "SWD-Takt: %1",
    "The SWD clock could not be set: %1": "Der SWD-Takt konnte nicht gesetzt werden: %1",
    "The SWD DP did not answer, trying to revive the link.":
        "Der SWD-DP antwortet nicht, die Verbindung wird wiederhergestellt.",
    "The SWD link came back at %1 Hz.":
        "Die SWD-Verbindung ist mit %1 Hz wieder da.",
    "The SWD DP does not answer. Check the wiring (SWDIO, SWCLK, GND), the target "
    "power supply and the SWD clock. A target program that switched the debug "
    "interface off, or that put the core into a low power state, only comes back "
    "on a power cycle. If it comes back with BOOT0 held high, that program is what "
    "takes the debug pins over: start the ROM bootloader that way, which leaves the "
    "debug pins alone, and erase the chip from there.":
        "Der SWD-DP antwortet nicht. Prüfen Sie die Verdrahtung (SWDIO, SWCLK, GND), "
        "die Versorgung des Ziels und den SWD-Takt. Ein Zielprogramm, das die "
        "Debug-Schnittstelle abgeschaltet oder den Kern in einen Energiesparzustand "
        "versetzt hat, kommt erst nach einem Neustart der Versorgung zurück. Kommt "
        "es mit hochgehaltenem BOOT0 zurück, übernimmt genau dieses Programm die "
        "Debug-Pins: Starten Sie auf diesem Weg den ROM-Bootloader, der die "
        "Debug-Pins unberührt lässt, und löschen Sie den Chip von dort.",
    "This probe does not drive an nRESET line.":
        "Diese Probe treibt keine nRESET-Leitung.",
    "Holding nRESET low and connecting under reset.":
        "nRESET wird niedrig gehalten und unter Reset verbunden.",
    "The SWD link came back under reset at %1 Hz.":
        "Die SWD-Verbindung kam unter Reset mit %1 Hz zurück.",
    "Connecting under reset did not bring the DP back either.":
        "Auch das Verbinden unter Reset hat den DP nicht zurückgebracht.",
    "The SWD DP did not answer while nRESET was being driven.":
        "Der SWD-DP hat bei aktiver nRESET-Leitung nicht geantwortet.",
    "The SWD DP reported an invalid IDCODE.":
        "Der SWD-DP hat eine ungültige IDCODE gemeldet.",
    "The DP read returned no data.": "Das Lesen des DP lieferte keine Daten.",
    "The AP read returned no data.": "Das Lesen des AP lieferte keine Daten.",
    "Empty transfer.": "Leere Übertragung.",
    "Internal error: the write value count does not match the request count.":
        "Interner Fehler: Die Anzahl der Schreibwerte stimmt nicht mit der Anzahl "
        "in der Anforderung überein.",
    "The probe answered with a truncated transfer response.":
        "Die Probe hat eine abgeschnittene Übertragungsantwort gesendet.",
    "The probe answered with a malformed DAP_Transfer response.":
        "Die Probe hat eine fehlerhafte DAP_Transfer-Antwort gesendet.",
    "SWD fault (the target did not acknowledge the access).":
        "SWD-Fehler (das Ziel hat den Zugriff nicht bestätigt).",
    "Protocol error during the SWD transfer.":
        "Protokollfehler bei der SWD-Übertragung.",
    "The probe performed %1 of %2 transfers.":
        "Die Probe hat %1 von %2 Übertragungen ausgeführt.",
    "The probe sent a truncated transfer payload.":
        "Die Probe hat abgeschnittene Übertragungsdaten gesendet.",
    "The SWD transfer was retried too often (WAIT response).":
        "Die SWD-Übertragung wurde zu oft wiederholt (WAIT-Antwort).",
    "Empty block transfer.": "Leere Blockübertragung.",
    "The probe answered with a truncated block transfer response.":
        "Die Probe hat eine abgeschnittene Blockübertragungsantwort gesendet.",
    "The probe answered with a malformed DAP_TransferBlock response.":
        "Die Probe hat eine fehlerhafte DAP_TransferBlock-Antwort gesendet.",
    "SWD fault during the block transfer.": "SWD-Fehler bei der Blockübertragung.",
    "Protocol error during the block transfer.":
        "Protokollfehler bei der Blockübertragung.",
    "The probe performed %1 of %2 block transfers.":
        "Die Probe hat %1 von %2 Blockübertragungen ausgeführt.",
    "The probe sent a truncated block transfer payload.":
        "Die Probe hat abgeschnittene Blockübertragungsdaten gesendet.",
    "The probe answered a poll with no data.":
        "Die Probe hat eine Abfrage ohne Daten beantwortet.",

    # --------------------------------------------------------------------- CortexM
    "Unknown Cortex-M core": "Unbekannter Cortex-M-Kern",
    "No Cortex-M core answered at CPUID (0x%1).":
        "Kein Cortex-M-Kern hat auf CPUID geantwortet (0x%1).",
    "The core is locked up (the flash algorithm crashed).":
        "Der Kern ist blockiert (der Flash-Algorithmus ist abgestürzt).",
    "The core did not halt within %1 ms.":
        "Der Kern hat nicht innerhalb von %1 ms angehalten.",
    "The probe has no nRESET line; use the software reset instead.":
        "Die Probe hat keine nRESET-Leitung; verwenden Sie stattdessen den "
        "Software-Reset.",
    "The core register transfer timed out.":
        "Zeitüberschreitung bei der Kernregister-Übertragung.",
    "Internal error: no register buffer.": "Interner Fehler: kein Registerpuffer.",

    # ----------------------------------------------------------------- DapTransport
    "unknown HID error": "unbekannter HID-Fehler",
    "The HID probe could not be opened. It is probably used by another program "
    "(Keil, OpenOCD, a debug session).":
        "Die HID-Probe konnte nicht geöffnet werden. Vermutlich wird sie von einem "
        "anderen Programm benutzt (Keil, OpenOCD, eine Debug-Sitzung).",
    "USB HID (CMSIS-DAP v1)": "USB HID (CMSIS-DAP v1)",
    "HID write failed: %1": "HID-Schreiben fehlgeschlagen: %1",
    "HID read failed: %1": "HID-Lesen fehlgeschlagen: %1",
    "The probe did not answer (timeout).":
        "Die Probe hat nicht geantwortet (Zeitüberschreitung).",
    "The USB (libusb) layer could not be initialised.":
        "Die USB-Schicht (libusb) konnte nicht initialisiert werden.",
    "USB enumeration failed.": "USB-Aufzählung fehlgeschlagen.",
    "The USB probe was not found any more. Replug it and refresh the list.":
        "Die USB-Probe wurde nicht mehr gefunden. Stecken Sie sie neu an und "
        "aktualisieren Sie die Liste.",
    "Access to the USB probe was denied. It is either used by another program, "
    "or no WinUSB driver is bound to it.":
        "Der Zugriff auf die USB-Probe wurde verweigert. Entweder benutzt sie ein "
        "anderes Programm, oder es ist kein WinUSB-Treiber dafür gebunden.",
    "The probe is not bound to a WinUSB (or libusbK) driver. Install the driver, "
    "or select the same probe over its USB HID (CMSIS-DAP v1) interface.":
        "An die Probe ist kein WinUSB- (oder libusbK-) Treiber gebunden. "
        "Installieren Sie den Treiber, oder wählen Sie dieselbe Probe über ihre "
        "USB-HID-Schnittstelle (CMSIS-DAP v1).",
    "Opening the USB probe failed: %1": "Öffnen der USB-Probe fehlgeschlagen: %1",
    "Claiming the USB interface failed: %1":
        "Belegen der USB-Schnittstelle fehlgeschlagen: %1",
    "CMSIS-DAP bulk transport, spec framing (no length prefix)":
        "CMSIS-DAP-Bulk-Transport, Rahmen nach Spezifikation (ohne Längenpräfix)",
    "CMSIS-DAP bulk transport, length-prefixed framing (2 byte prefix)":
        "CMSIS-DAP-Bulk-Transport, Rahmen mit Längenpräfix (2-Byte-Präfix)",
    "USB bulk write failed: %1": "USB-Bulk-Schreiben fehlgeschlagen: %1",
    "USB bulk read failed: %1": "USB-Bulk-Lesen fehlgeschlagen: %1",
    "The probe sent a malformed packet.":
        "Die Probe hat ein fehlerhaftes Paket gesendet.",
    "The probe did not answer DAP_Info. Check that the right interface of the "
    "device was selected and that the firmware runs a CMSIS-DAP implementation.":
        "Die Probe hat DAP_Info nicht beantwortet. Prüfen Sie, ob die richtige "
        "Schnittstelle des Geräts gewählt wurde und ob die Firmware CMSIS-DAP "
        "implementiert.",
    "The command does not fit into one packet (%1 > %2 bytes).":
        "Der Befehl passt nicht in ein Paket (%1 > %2 Byte).",
    "USB bulk (WinUSB, CMSIS-DAP v2)": "USB Bulk (WinUSB, CMSIS-DAP v2)",

    # ---------------------------------------------------------------------- ElfFile
    "The algorithm file could not be opened: %1":
        "Die Algorithmusdatei konnte nicht geöffnet werden: %1",
    "The file is too small to be an ELF image.":
        "Die Datei ist zu klein für ein ELF-Abbild.",
    "The file is not an ELF image (a Keil .FLM algorithm is expected).":
        "Die Datei ist kein ELF-Abbild (erwartet wird ein Keil-.FLM-Algorithmus).",
    "Only 32 bit ELF images are supported (this one is 64 bit).":
        "Es werden nur 32-Bit-ELF-Abbilder unterstützt (dieses ist 64 Bit).",
    "Only little endian ELF images are supported.":
        "Es werden nur ELF-Abbilder mit Little-Endian-Byte-Reihenfolge unterstützt.",
    "The image is not an ARM image (e_machine = %1).":
        "Das Abbild ist kein ARM-Abbild (e_machine = %1).",
    "The image is truncated (segment %1 leaves the file).":
        "Das Abbild ist abgeschnitten (Segment %1 reicht über die Datei hinaus).",
    "The image carries no loadable segment.":
        "Das Abbild enthält kein ladbares Segment.",

    # --------------------------------------------------------------------- FlashAlgo
    "The algorithm does not export an Init() function.":
        "Der Algorithmus exportiert keine Funktion Init().",
    "The algorithm does not export a ProgramPage() function.":
        "Der Algorithmus exportiert keine Funktion ProgramPage().",
    "The algorithm does not export EraseSector() nor EraseChip().":
        "Der Algorithmus exportiert weder EraseSector() noch EraseChip().",
    "The FlashDevice structure could not be located in the algorithm.":
        "Die Struktur FlashDevice wurde im Algorithmus nicht gefunden.",
    "The FlashDevice structure at offset 0x%1 is not valid.":
        "Die Struktur FlashDevice bei Offset 0x%1 ist ungültig.",
    "The algorithm carries an empty sector table.":
        "Der Algorithmus enthält eine leere Sektortabelle.",
    "Unknown": "Unbekannt",
    "On-chip flash": "Interner Flash",
    "External flash, 8 bit bus": "Externer Flash, 8-Bit-Bus",
    "External flash, 16 bit bus": "Externer Flash, 16-Bit-Bus",
    "External flash, 32 bit bus": "Externer Flash, 32-Bit-Bus",
    "External SPI flash": "Externer SPI-Flash",

    # ------------------------------------------------------------------- FlashWorker
    "Loading the flash algorithm ...": "Flash-Algorithmus wird geladen...",
    "Algorithm %1: %2, %3, page %4 bytes, %5 sectors":
        "Algorithmus %1: %2, %3, Seite %4 Byte, %5 Sektoren",
    "%1: %2 in %3 blocks, 0x%4 - 0x%5, checksum 0x%6":
        "%1: %2 in %3 Blöcken, 0x%4 - 0x%5, Prüfsumme 0x%6",
    "Intel HEX": "Intel HEX",
    "BIN": "BIN",
    "Firmware %1": "Firmware %1",
    "The firmware (0x%1 - 0x%2) does not fit into the flash of the algorithm "
    "(0x%3 - 0x%4). Check the loading address.":
        "Die Firmware (0x%1 - 0x%2) passt nicht in den Flash des Algorithmus "
        "(0x%3 - 0x%4). Prüfen Sie die Ladeadresse.",
    "The algorithm has no sector at 0x%1.":
        "Der Algorithmus hat keinen Sektor bei 0x%1.",
    "No firmware given: the whole chip is erased and the target is left halted.":
        "Keine Firmware angegeben: Der ganze Chip wird gelöscht, das Ziel bleibt "
        "angehalten.",
    "Cancelled.": "Abgebrochen.",
    "Probe: %1": "Probe: %1",
    "CMSIS-DAP firmware: %1": "CMSIS-DAP-Firmware: %1",
    "Target: %1 (CPUID 0x%2, DP IDCODE 0x%3)":
        "Ziel: %1 (CPUID 0x%2, DP IDCODE 0x%3)",
    "The whole chip was erased in %1 s; the target is left halted.":
        "Der ganze Chip wurde in %1 s gelöscht; das Ziel bleibt angehalten.",
    "Programming finished in %1 s: %2 pages, %3 bytes, %4 sectors erased.":
        "Programmierung in %1 s beendet: %2 Seiten, %3 Byte, %4 Sektoren gelöscht.",
    "The algorithm used 0x%1 of RAM at %2.":
        "Der Algorithmus hat 0x%1 RAM bei %2 belegt.",
    "The SWD link failed at %1; retrying at %2.":
        "Die SWD-Verbindung ist bei %1 ausgefallen; neuer Versuch mit %2.",
    "Retrying at a slower clock ...": "Neuer Versuch mit langsamerem Takt...",

    # ----------------------------------------------------------------------- Flasher
    "The algorithm image is empty.": "Das Algorithmus-Abbild ist leer.",
    "The algorithm reports an unusable page size (%1 bytes).":
        "Der Algorithmus meldet eine unbrauchbare Seitengröße (%1 Byte).",
    "The RAM base address is too high for the algorithm.":
        "Die RAM-Basisadresse ist für den Algorithmus zu hoch.",
    "The algorithm could not be written to the target RAM at %1: %2 of %3 bytes "
    "came back different, the first one at %4 (wrote 0x%5, read 0x%6). The base "
    "address has to be RAM the debug unit can write to; if it is, the probe did "
    "not move the data reliably.":
        "Der Algorithmus konnte bei %1 nicht in den RAM des Ziels geschrieben "
        "werden: %2 von %3 Byte kamen anders zurück, das erste bei %4 (geschrieben "
        "0x%5, gelesen 0x%6). Die Basisadresse muss RAM sein, in das die "
        "Debug-Einheit schreiben kann; ist sie das, hat die Probe die Daten nicht "
        "zuverlässig übertragen.",
    "The algorithm does not provide %1().":
        "Der Algorithmus stellt %1() nicht bereit.",
    "%1() did not return: %2 (PC 0x%3, DFSR 0x%4, CFSR 0x%5, HFSR 0x%6)":
        "%1() ist nicht zurückgekehrt: %2 (PC 0x%3, DFSR 0x%4, CFSR 0x%5, HFSR 0x%6)",
    "%1() stopped at 0x%2 instead of returning to 0x%3 (DFSR = 0x%4).":
        "%1() blieb bei 0x%2 stehen, statt nach 0x%3 zurückzukehren (DFSR = 0x%4).",
    "Erasing the whole chip ...": "Der ganze Chip wird gelöscht...",
    "EraseChip() failed with code %1.":
        "EraseChip() ist mit Code %1 fehlgeschlagen.",
    "Chip erase done.": "Chiplöschung abgeschlossen.",
    "The algorithm provides neither EraseChip() nor EraseSector().":
        "Der Algorithmus stellt weder EraseChip() noch EraseSector() bereit.",
    "Address 0x%1 is not covered by the sector table of the algorithm.":
        "Die Adresse 0x%1 ist von der Sektortabelle des Algorithmus nicht abgedeckt.",
    "Nothing to erase.": "Nichts zu löschen.",
    "Erasing sector %1 of %2 (0x%3) ...":
        "Sektor %1 von %2 wird gelöscht (0x%3)...",
    "Erasing the sector at 0x%1 failed with code %2.":
        "Das Löschen des Sektors bei 0x%1 ist mit Code %2 fehlgeschlagen.",
    "The firmware file carries no data.": "Die Firmwaredatei enthält keine Daten.",
    "Programming 0x%1 (%2 / %3 bytes) ...":
        "0x%1 wird programmiert (%2 / %3 Byte)...",
    "Programming the page at 0x%1 failed with code %2.":
        "Das Programmieren der Seite bei 0x%1 ist mit Code %2 fehlgeschlagen.",
    "The algorithm has no Verify() routine; the flash is read back through the "
    "debug port instead.":
        "Der Algorithmus hat keine Routine Verify(); der Flash wird stattdessen "
        "über den Debug-Port zurückgelesen.",
    "Verifying 0x%1 ...": "0x%1 wird geprüft...",
    "Verification failed at 0x%1 (code %2).":
        "Prüfung bei 0x%1 fehlgeschlagen (Code %2).",
    "Reading back 0x%1 failed; the verification was skipped.":
        "Das Zurücklesen von 0x%1 ist fehlgeschlagen; die Prüfung wurde übersprungen.",
    "Verification failed at 0x%1: expected 0x%2, read 0x%3.":
        "Prüfung bei 0x%1 fehlgeschlagen: erwartet 0x%2, gelesen 0x%3.",
    "No flash algorithm is selected.": "Es ist kein Flash-Algorithmus ausgewählt.",
    "No firmware data to program.": "Keine Firmwaredaten zum Programmieren.",
    "Preparing ...": "Vorbereitung...",
    "Algorithm: %1 (%2, %3 bytes)": "Algorithmus: %1 (%2, %3 Byte)",
    "Reset: %1": "Reset: %1",
    "RAM transfer: %1 words (%2 bytes) per DAP_TransferBlock, command packet %3 bytes.":
        "RAM-Übertragung: %1 Wörter (%2 Byte) pro DAP_TransferBlock, Befehlspaket "
        "%3 Byte.",
    "Writing the algorithm to the RAM failed, attempt %1 of %2: %3":
        "Schreiben des Algorithmus in den RAM fehlgeschlagen, Versuch %1 von %2: %3",
    "Algorithm loaded at 0x%1 (%2 bytes of RAM).":
        "Algorithmus bei 0x%1 geladen (%2 Byte RAM).",
    "Init() did not complete, restarting the algorithm (%1 of %2): %3":
        "Init() wurde nicht beendet, Algorithmus wird neu gestartet (%1 von %2): %3",
    "Init() failed with code %1.": "Init() ist mit Code %1 fehlgeschlagen.",
    "Initialised.": "Initialisiert.",
    "UnInit() failed: %1": "UnInit() fehlgeschlagen: %1",
    "UnInit() returned code %1.": "UnInit() hat Code %1 zurückgegeben.",
    "Resetting the target failed: %1": "Zurücksetzen des Ziels fehlgeschlagen: %1",
    "Done.": "Fertig.",

    # --------------------------------------------------------------- LanguageManager
    "Follow System(&A)": "Systemeinstellung(&A)",
    "English(&E)": "English(&E)",
    "Simplified Chinese(&C)": "Chinesisch (vereinfacht)(&C)",
    "Russian(&R)": "Русский(&R)",
    "German(&G)": "Deutsch(&G)",
    "Spanish(&S)": "Español(&S)",
    "Simplified Chinese": "Chinesisch (vereinfacht)",
    "Russian": "Русский",
    "German": "Deutsch",
    "Spanish": "Español",
    "English": "English",

    # -------------------------------------------------------------------- MainWindow
    "WeAct Studio DAP Flasher": "WeAct Studio DAP Flasher",
    "Debug Probe": "Debug-Probe",
    "Flash Algorithm": "Flash-Algorithmus",
    "Firmware File": "Firmware-Datei",
    "Options": "Optionen",
    "Probe:": "Probe:",
    "Connection:": "Verbindung:",
    "Target:": "Ziel:",
    "Algorithm File:": "Algorithmusdatei:",
    "File Path:": "Dateipfad:",
    "Firmware:": "Firmware:",
    "BIN Load Address: 0x": "BIN-Ladeadresse: 0x",
    "SWD Clock:": "SWD-Takt:",
    "Algorithm RAM: 0x": "Algorithmus-RAM: 0x",
    "Reset Mode:": "Reset-Modus:",
    "Refresh(&R)": "Aktualisieren(&R)",
    "Browse(&B)...": "Durchsuchen(&B)...",
    "Whole chip erase": "Ganzen Chip löschen",
    "Verify": "Prüfen",
    "Reset and run": "Zurücksetzen und starten",
    "Cancel(&C)": "Abbrechen(&C)",
    "Start Programming(&S)": "Programmierung starten(&S)",
    "Halt(&P)": "Anhalten(&P)",
    "Run(&R)": "Starten(&R)",
    "Reset(&T)": "Reset(&T)",
    "Exit(&X)": "Beenden(&X)",
    "Select or drop a .hex / .bin firmware file":
        "Wählen Sie eine .hex-/.bin-Firmwaredatei oder ziehen Sie sie hierher",
    "RAM area the flash algorithm is copied to while it runs. 0x20000000 is "
    "correct for most Cortex-M devices.":
        "RAM-Bereich, in den der Flash-Algorithmus während der Ausführung kopiert "
        "wird. 0x20000000 ist für die meisten Cortex-M-Geräte richtig.",
    "Loading address used for .bin files (ignored for .hex)":
        "Ladeadresse für .bin-Dateien (bei .hex ignoriert)",
    "Reset used by the Reset button and by \"Reset and run\". Software reset needs "
    "no extra wire, hardware reset pulses the nRESET line, automatic uses nRESET "
    "when the probe has one.":
        "Reset für die Schaltfläche „Reset“ und für „Zurücksetzen und starten“. Der "
        "Software-Reset braucht keine zusätzliche Leitung, der Hardware-Reset legt "
        "einen Impuls auf die nRESET-Leitung, Automatisch nutzt nRESET, wenn die "
        "Probe eine hat.",
    "Log": "Protokoll",
    "File(&F)": "Datei(&F)",
    "Open Firmware(&O)...": "Firmware öffnen(&O)...",
    "Select Algorithm(&A)...": "Algorithmus auswählen(&A)...",
    "Tools(&T)": "Werkzeuge(&T)",
    "Refresh Probes(&R)": "Probes aktualisieren(&R)",
    "Rescan Algorithms(&L)": "Algorithmen neu suchen(&L)",
    "Show All USB HID Devices(&H)": "Alle USB-HID-Geräte anzeigen(&H)",
    "Clear Log(&C)": "Protokoll leeren(&C)",
    "View(&V)": "Ansicht(&V)",
    "Language(&L)": "Sprache(&L)",
    "Help(&H)": "Hilfe(&H)",
    "About(&A)": "Info(&A)",
    "(no probe found)": "(keine Probe gefunden)",
    "Identifying the target over %1 ...": "Ziel wird über %1 identifiziert...",
    "<font color='%1'>Identifying the target ...</font>":
        "<font color='%1'>Ziel wird identifiziert...</font>",
    "<font color='%1'>No probe found</font>":
        "<font color='%1'>Keine Probe gefunden</font>",
    "Connect a CMSIS-DAP probe and press Refresh.":
        "Schließen Sie eine CMSIS-DAP-Probe an und drücken Sie Aktualisieren.",
    "<font color='%1'>Connected (%2)</font>":
        "<font color='%1'>Verbunden (%2)</font>",
    "SWD": "SWD",
    "-": "-",
    "unknown": "unbekannt",
    "CPUID %1, DP IDCODE %2, core %3": "CPUID %1, DP IDCODE %2, Kern %3",
    "halted": "angehalten",
    "running": "läuft",
    "<font color='%1'>Probe present, target not identified</font>":
        "<font color='%1'>Probe vorhanden, Ziel nicht identifiziert</font>",
    "Scanning flash algorithms (%1 pack folders) ...":
        "Flash-Algorithmen werden gesucht (%1 Paketordner)...",
    "%1 flash algorithms found in %2 groups.":
        "%1 Flash-Algorithmen in %2 Gruppen gefunden.",
    "%1 file(s) were skipped: they carry no usable FlashDevice structure.":
        "%1 Datei(en) übersprungen: Sie enthalten keine brauchbare "
        "FlashDevice-Struktur.",
    "Recently used": "Zuletzt verwendet",
    "Additional folder: %1": "Zusätzlicher Ordner: %1",
    "Last used": "Zuletzt benutzt",
    "Select a Keil Flash Algorithm": "Keil-Flash-Algorithmus auswählen",
    "The algorithm %1 could not be loaded: %2":
        "Der Algorithmus %1 konnte nicht geladen werden: %2",
    "Algorithm loaded: %1 (%2)": "Algorithmus geladen: %1 (%2)",
    "<font color='%1'>%2</font>": "<font color='%1'>%2</font>",
    "<font color='%1'>No algorithm selected</font>":
        "<font color='%1'>Kein Algorithmus ausgewählt</font>",
    "<b>%1</b><br>%2, flash 0x%3 - 0x%4 (%5), page %6 bytes, %7 sectors":
        "<b>%1</b><br>%2, Flash 0x%3 - 0x%4 (%5), Seite %6 Byte, %7 Sektoren",
    "<br><font color='%1'>This algorithm has no chip erase; the sectors are "
    "erased instead.</font>":
        "<br><font color='%1'>Dieser Algorithmus kann nicht den ganzen Chip löschen; "
        "stattdessen werden die Sektoren gelöscht.</font>",
    "Select Firmware File": "Firmware-Datei auswählen",
    "Firmware Files (*.hex *.bin *.ihex);;Intel HEX (*.hex *.ihex);;"
    "Binary Files (*.bin);;All Files (*.*)":
        "Firmware-Dateien (*.hex *.bin *.ihex);;Intel HEX (*.hex *.ihex);;"
        "Binärdateien (*.bin);;Alle Dateien (*.*)",
    "<font color='%1'>No valid firmware file selected</font>":
        "<font color='%1'>Keine gültige Firmwaredatei ausgewählt</font>",
    "%1  Size: %2 in %3 blocks  Address: 0x%4 - 0x%5  Checksum: 0x%6":
        "%1  Größe: %2 in %3 Blöcken  Adresse: 0x%4 - 0x%5  Prüfsumme: 0x%6",
    "No Probe": "Keine Probe",
    "Select a debug probe first.": "Wählen Sie zuerst eine Debug-Probe.",
    "Halt Failed": "Anhalten fehlgeschlagen",
    "The core is halted.": "Der Kern ist angehalten.",
    "Run Failed": "Starten fehlgeschlagen",
    "The core is running.": "Der Kern läuft.",
    "Reset Failed": "Reset fehlgeschlagen",
    "The target was reset and is running.":
        "Das Ziel wurde zurückgesetzt und läuft.",
    "Select a CMSIS-DAP probe first.": "Wählen Sie zuerst eine CMSIS-DAP-Probe.",
    "No Algorithm": "Kein Algorithmus",
    "Select the Keil .FLM flash algorithm that matches the device.":
        "Wählen Sie den zum Gerät passenden Keil-.FLM-Flash-Algorithmus.",
    "No Firmware": "Keine Firmware",
    "Select a valid firmware file first.":
        "Wählen Sie zuerst eine gültige Firmwaredatei.",
    "Cancelling ...": "Wird abgebrochen...",
    "Programming complete": "Programmierung abgeschlossen",
    "Complete": "Fertig",
    "The firmware was programmed successfully.\n\n%1":
        "Die Firmware wurde erfolgreich programmiert.\n\n%1",
    "Cancelled": "Abgebrochen",
    "Programming failed": "Programmierung fehlgeschlagen",
    "Failed": "Fehlgeschlagen",
    "Ready": "Bereit",
    "About": "Info",
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
        "<h3>WeAct Studio DAP Flasher</h3><p>Version: %1</p><p><b>WeAct Studio</b>"
        "<br>Copyright (C) 2026 WeAct Studio. Alle Rechte vorbehalten.</p>"
        "<p>Programmiert Cortex-M-Geräte über eine CMSIS-DAP-Probe (USB HID oder "
        "USB Bulk / WinUSB) mit den Keil-.FLM-Flash-Algorithmen.</p><p>Dieses "
        "Programm ist freie Software: Sie können es unter den Bedingungen der GNU "
        "General Public License, wie von der Free Software Foundation "
        "veröffentlicht, weitergeben und/oder modifizieren, entweder gemäß "
        "Version 3 der Lizenz oder (nach Ihrer Wahl) jeder späteren Version. Die "
        "Verbreitung erfolgt in der Hoffnung, dass es nützlich ist, aber OHNE JEDE "
        "GEWÄHRLEISTUNG, sogar ohne die implizite Gewährleistung der "
        "MARKTFÄHIGKEIT oder EIGNUNG FÜR EINEN BESTIMMTEN ZWECK. Details finden "
        "Sie in der GNU General Public License.</p><p>Lizenz: GPL-3.0-or-later<br>"
        "<a href='https://www.gnu.org/licenses/gpl-3.0.html'>"
        "https://www.gnu.org/licenses/gpl-3.0.html</a></p><p>Erstellt mit Qt %2, "
        "statisch kompiliert mit MSYS2 (MINGW64).</p>",
    "Programming in Progress": "Programmierung läuft",
    "Programming is still running; stopping now can leave the device with an "
    "incomplete firmware.\n\nExit anyway?":
        "Die Programmierung läuft noch; ein Abbruch kann das Gerät mit "
        "unvollständiger Firmware zurücklassen.\n\nTrotzdem beenden?",

    # ------------------------------------------------------------------------ Probes
    "The HID interface could not be initialised.":
        "Die HID-Schnittstelle konnte nicht initialisiert werden.",
    "HID init failed: %1": "HID-Initialisierung fehlgeschlagen: %1",
    "HID init failed.": "HID-Initialisierung fehlgeschlagen.",
    "HID device": "HID-Gerät",
    " (in use)": " (in Benutzung)",
    "Transport: %1": "Transport: %1",
    "Product: %1": "Produkt: %1",
    "Manufacturer: %1": "Hersteller: %1",
    "Serial: %1": "Seriennummer: %1",
    "USB: bus %1 address %2, interface %3, EP IN 0x%4 / OUT 0x%5":
        "USB: Bus %1, Adresse %2, Schnittstelle %3, EP IN 0x%4 / OUT 0x%5",
    "Interface: %1": "Schnittstelle: %1",
    "Usage page: 0x%1  Usage: 0x%2": "Usage Page: 0x%1  Usage: 0x%2",

    # --------------------------------------------------------------------- ResetMode
    "Hardware reset": "Hardware-Reset",
    "Automatic": "Automatisch",
    "Software reset": "Software-Reset",

    # ----------------------------------------------------------------- TargetSession
    "The target program was holding the debug access; the core is now halted "
    "under reset.":
        "Das Zielprogramm hielt den Debug-Zugriff; der Kern ist jetzt unter Reset "
        "angehalten.",
    "Reset: %1 (the DP did not answer, so this was a pin reset only).":
        "Reset: %1 (der DP antwortete nicht, daher nur ein Reset über die Leitung).",

    # ------------------------------------------------------------------ ThemeManager
    "Follow System(&S)": "Systemeinstellung(&S)",
    "Light(&L)": "Hell(&L)",
    "Dark(&D)": "Dunkel(&D)",
}
