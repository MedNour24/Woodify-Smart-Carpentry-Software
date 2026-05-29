QT       += core gui sql charts multimedia printsupport network serialport \
            3dcore 3drender 3dextras 3dinput
# --- OpenCV Configuration ---
# Commented out by default to allow compilation without installation.
# Uncomment and adjust paths if you want to use the Robust OpenCV detector.
# INCLUDEPATH += C:/opencv/build/include
# LIBS += -LC:/opencv/build/x64/vc16/lib -lopencv_world4100


greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

QMAKE_LFLAGS -= -fuse-linker-plugin

win32 {
    !isEmpty(QMAKE_CC):!exists($$QMAKE_CC) {
        message("QMAKE_CC not found at '$$QMAKE_CC' - fallback to gcc from PATH")
        QMAKE_CC = gcc
    }
    !isEmpty(QMAKE_CXX):!exists($$QMAKE_CXX) {
        message("QMAKE_CXX not found at '$$QMAKE_CXX' - fallback to g++ from PATH")
        QMAKE_CXX = g++
    }
}

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    aiassistantservice.cpp \
    airesponse.cpp \
    bois.cpp \
    connsql.cpp \
    employee.cpp \
    fabrication.cpp \
    faceid.cpp \
    login.cpp \
    main.cpp \
    modele.cpp \
    modelecostestimator.cpp \
    nestingengine.cpp \
    nestingwidget.cpp \
    panel.cpp \
    phase.cpp \
    piecerect.cpp \
    piece.cpp \
    smartcarpentry.cpp \
    budgetgltfviewer.cpp \
    woodexpert.cpp \
    aigenerationdashboard.cpp \
    generatedpiececard.cpp \
    modelpreviewwidget.cpp \
    arduinomanager.cpp \
    arduinohandler.cpp \
    arduinoranim.cpp \
    rfidmanager.cpp \

HEADERS += \
    aiassistantservice.h \
    airesponse.h \
    bois.h \
    connsql.h \
    employee.h \
    fabrication.h \
    faceid.h \
    login.h \
    modele.h \
    modelecostestimator.h \
    nestingengine.h \
    nestingwidget.h \
    panel.h \
    phase.h \
    piecerect.h \
    piece.h \
    smartcarpentry.h \
    budgetgltfviewer.h \
    woodexpert.h \
    aigenerationdashboard.h \
    generatedpiececard.h \
    modelpreviewwidget.h \
    arduinomanager.h \
    arduinohandler.h \
    arduinoranim.h \
    rfidmanager.h \

FORMS += \
    faceid.ui \
    login.ui \
    smartcarpentry.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc

DISTFILES +=
