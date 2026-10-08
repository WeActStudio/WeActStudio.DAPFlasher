# ============================================================================
#  WeAct Studio DAP Flasher - console companion
#
#  Same flash engine and command line as the graphical program, built as a
#  Windows console application: a calling shell waits for it, its output can be
#  redirected and its exit code is the result of the operation.
# ============================================================================

QT = core

TEMPLATE = app

TARGET   = "WeAct Studio DAP Flasher CLI"

VERSION  = 1.0.0
DEFINES += APP_VERSION=\\\"$$VERSION\\\"

CONFIG  -= debug_and_release debug_and_release_target

include($$PWD/qt6_static_link.pri)

greaterThan(QT_MAJOR_VERSION, 5) {
    CONFIG += c++17
    DEFINES += QT6_BUILD
} else {
    CONFIG += c++11
}

CONFIG  += release console
CONFIG  -= debug
CONFIG  -= app_bundle

QMAKE_CXXFLAGS_RELEASE += -Os -ffunction-sections -fdata-sections
QMAKE_CFLAGS_RELEASE   += -Os -ffunction-sections -fdata-sections
QMAKE_LFLAGS_RELEASE   += -Wl,-s -Wl,--gc-sections

QMAKE_CXXFLAGS += -Wa,-mbig-obj
QMAKE_CFLAGS   += -Wa,-mbig-obj

INCLUDEPATH += $$PWD $$PWD/src

SOURCES += \
    src/climain.cpp \
    src/cli.cpp \
    src/flashworker.cpp \
    src/targetsession.cpp \
    src/firmwareimage.cpp \
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
    src/cli.h \
    src/flashworker.h \
    src/targetsession.h \
    src/firmwareimage.h \
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

MSYS_PREFIX_WIN = D:/msys64/mingw64
INCLUDEPATH += $$MSYS_PREFIX_WIN/include
INCLUDEPATH += $$MSYS_PREFIX_WIN/include/hidapi
INCLUDEPATH += $$MSYS_PREFIX_WIN/include/libusb-1.0

# The Chinese translation is compiled with lrelease into
# src/translations/app_zh_CN.qm and packed into the executable, exactly like in
# the graphical program: climain.cpp installs it when the language resolves to
# Chinese, without needing QtWidgets.
RESOURCES += $$PWD/src/translations.qrc

LIBS += $$MSYS_PREFIX_WIN/lib/libhidapi.dll.a
LIBS += $$MSYS_PREFIX_WIN/lib/libusb-1.0.dll.a
LIBS += -lsetupapi -lhid -lcfgmgr32

# GetUserDefaultLocaleName needs Vista+ API declarations in MinGW headers
DEFINES += _WIN32_WINNT=0x0601
