include(../plugin.pri)

DEFINES += PHYSICSPLIST_LIBRARY

greaterThan(QT_MAJOR_VERSION, 4) {
    OTHER_FILES = plugin.json
}

SOURCES += physics_plist_plugin.cpp

HEADERS += physics_plist_global.h \
		physics_plist_plugin.h

RESOURCES += \
    physicsplist.qrc
