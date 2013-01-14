include(../plugin.pri)

DEFINES += SVINOTA_LIBRARY

greaterThan(QT_MAJOR_VERSION, 4) {
    OTHER_FILES = plugin.json
}

SOURCES += svinotaplugin.cpp
HEADERS += svinotaplugin.h\
		   svinota_global.h\
		   svinota_ffobjects.hpp

RESOURCES += \
    svinota.qrc
