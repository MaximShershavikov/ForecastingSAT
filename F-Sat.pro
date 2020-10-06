#********************************************************************************
#    ForecastingSAT version 1.0. Calculation of the position and angles
#    of near space satellites
#    Copyright (C) 2020  Maxim Shershavikov

#    This file is part of ForecastingSAT version 1.0.

#    ForecastingSAT v1.0 is free software: you can redistribute it and/or modify
#    it under the terms of the GNU General Public License as published by
#    the Free Software Foundation, either version 3 of the License, or
#    (at your option) any later version.

#    ForecastingSAT v1.0 is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#    GNU General Public License for more details.

#    You should have received a copy of the GNU General Public License
#    along with this program.  If not, see <https://www.gnu.org/licenses/>.

#    Email m.shershavikov@yandex.ru
#    To read a copy of the GNU General Public License in a file COPYING.txt,
#    to do this, click the AbautProgram button.
#**********************************************************************************

QT += quick

CONFIG += c++11

DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += \
        controller.cpp \
        hub.cpp \
        main.cpp \
        nearspaseforecasting.cpp

RESOURCES += qml.qrc

TRANSLATIONS += \
    F-Sat_ru_RU.ts

QML_IMPORT_PATH =

QML_DESIGNER_IMPORT_PATH =

CONFIG += qmltypes
QML_IMPORT_NAME = hub
QML_IMPORT_MAJOR_VERSION = 1

QMAKE_LFLAGS_RELEASE += -static -static-libgcc

qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    controller.h \
    hub.h \
    nearspaseforecasting.h

RC_FILE = Appiconrc.rc
