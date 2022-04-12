!exists($$PWD/AppNavigationData.h) {
    error("AppNavigationData.h does not exist! Please see README.md for details." )
}

INCLUDEPATH += $$PWD

HEADERS += \
    $$PWD/AppNavigationController.h \
    $$PWD/AppNavigationData.h

SOURCES += \
    $$PWD/AppNavigationController.cpp

RESOURCES += \
    $$PWD/AppNavigation.qrc

DISTFILES += \
    $$PWD/AppStackView.qml \
    $$PWD/qmldir

OTHER_FILES += \
    $$PWD/$$PWD/AppNavigationData.h.sample
