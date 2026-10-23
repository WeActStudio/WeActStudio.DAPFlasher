# ============================================================================
#  WeAct Studio DAP Flasher - graphical user interface
#
#  Programs Cortex-M devices through a CMSIS-DAP probe (USB HID or USB bulk)
#  using the Keil .FLM flash algorithms.
#
#  Build environment: MSYS2 (D:\msys64) MINGW64
#    pacman -S mingw-w64-x86_64-qt5-static mingw-w64-x86_64-hidapi \
#              mingw-w64-x86_64-libusb
#
#  Build: run ./build.sh in an MSYS2 MINGW64 shell (or double click build.bat)
# ============================================================================

QT += core gui widgets

TEMPLATE = app

# Executable name
TARGET   = "WeAct Studio DAP Flasher"

VERSION  = 1.0.0
DEFINES += APP_VERSION=\\\"$$VERSION\\\"

CONFIG  -= debug_and_release debug_and_release_target

include($$PWD/qt6_static_link.pri)

# Qt6 needs C++17; Qt5 is fine with C++11 (the same project file supports both
# the Qt5 and the Qt6 static libraries)
greaterThan(QT_MAJOR_VERSION, 5) {
    CONFIG += c++17
    DEFINES += QT6_BUILD
} else {
    CONFIG += c++11
}

CONFIG  += release
CONFIG  -= debug

QMAKE_CXXFLAGS_RELEASE += -Os -ffunction-sections -fdata-sections
QMAKE_CFLAGS_RELEASE   += -Os -ffunction-sections -fdata-sections
QMAKE_LFLAGS_RELEASE   += -Wl,-s -Wl,--gc-sections

# A static Qt links in every default plugin. This program only needs the
# platform plugin (qwindows) and, for the system dark mode, a style plugin.
# Dropping the image format and icon engine plugins is the single biggest win
# for the binary size (the UI artwork is PNG, which Qt handles itself).
QTPLUGIN.imageformats = -
QTPLUGIN.iconengines  = -

# Qt5 static builds need big object file support
QMAKE_CXXFLAGS += -Wa,-mbig-obj
QMAKE_CFLAGS   += -Wa,-mbig-obj

INCLUDEPATH += $$PWD $$PWD/src

SOURCES += \
    src/main.cpp \
    src/mainwindow.cpp \
    src/algorithmpicker.cpp \
    src/flashworker.cpp \
    src/targetsession.cpp \
    src/firmwareimage.cpp \
    src/cli.cpp \
    src/languagemanager.cpp \
    src/thememanager.cpp \
    src/dap/probes.cpp \
    src/dap/daptransport.cpp \
    src/dap/cmsisdap.cpp \
    src/target/cortexm.cpp \
    src/target/resetmode.cpp \
    src/flash/elffile.cpp \
    src/flash/flashalgo.cpp \
    src/flash/flasher.cpp \
    src/flash/algocatalog.cpp

HEADERS += \
    src/mainwindow.h \
    src/algorithmpicker.h \
    src/wrappedlabel.h \
    src/flashworker.h \
    src/targetsession.h \
    src/firmwareimage.h \
    src/cli.h \
    src/languagemanager.h \
    src/thememanager.h \
    src/dap/dapdefs.h \
    src/dap/probes.h \
    src/dap/daptransport.h \
    src/dap/cmsisdap.h \
    src/target/cortexm.h \
    src/target/resetmode.h \
    src/flash/elffile.h \
    src/flash/flashalgo.h \
    src/flash/flasher.h \
    src/flash/algocatalog.h

# ----------------------------------------------------------------------------
#  CMSIS-DAP transports
#
#  hidapi          : CMSIS-DAP v1 (USB HID)
#  libusb-1.0      : CMSIS-DAP v2 (USB bulk, WinUSB / libusbK driver)
# ----------------------------------------------------------------------------
INCLUDEPATH += $$MSYS_PREFIX_WIN/include/hidapi
INCLUDEPATH += $$MSYS_PREFIX_WIN/include/libusb-1.0

LIBS += -L$$MSYS_PREFIX_WIN/lib -lhidapi
LIBS += -L$$MSYS_PREFIX_WIN/lib -lusb-1.0
LIBS += -lsetupapi -lhid -lcfgmgr32

# ----------------------------------------------------------------------------
#  UI translations (Simplified Chinese, Russian, German, Spanish)
#
#  English is the source language of every tr() string in the sources;
#  translations/app_<lang>.ts is compiled with lrelease into
#  src/translations/app_<lang>.qm and packed into the executable through
#  src/translations.qrc (no external file needed at run time).
# ----------------------------------------------------------------------------
TRANSLATIONS += \
    $$PWD/translations/app_zh_CN.ts \
    $$PWD/translations/app_ru.ts \
    $$PWD/translations/app_de.ts \
    $$PWD/translations/app_es.ts

RESOURCES += $$PWD/src/translations.qrc
RESOURCES += $$PWD/resources/resources.qrc

# ----------------------------------------------------------------------------
#  Windows resources (icon + version information)
# ----------------------------------------------------------------------------
RC_FILE = src/app.rc
