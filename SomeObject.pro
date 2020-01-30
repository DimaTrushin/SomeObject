QT -= gui

CONFIG += c++17 rtti_off warn_on console
CONFIG -= app_bundle
# CONFIG += precompile_header

# Use Precompiled headers (PCH)
# PRECOMPILED_HEADER  = stdafx.h

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

msvc-*{
  # Make warning level to be W4
  QMAKE_CXXFLAGS_WARN_ON -= -W3
  QMAKE_CXXFLAGS_WARN_ON += -W4
}

SOURCES += \
        Test.cpp \
        main.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
  SomeObject.h \
  SomePrint.h \
  Test.h
