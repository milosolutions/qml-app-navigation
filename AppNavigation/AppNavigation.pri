!contains(HEADERS, ".*/AppNavigationData.h") {
    error("AppNavigationData.h does not exist! Please see README.md for details." )
}

INCLUDEPATH += $$PWD

HEADERS += \
    $$PWD/AppNavigationController.h

SOURCES += \
    $$PWD/AppNavigationController.cpp

RESOURCES += \
    $$PWD/AppNavigation.qrc

DISTFILES += \
    $$PWD/qmldir \
    $$PWD/AppStackView.qml \
    $$PWD/AppPage.qml \
    $$PWD/AppPopup.qml

OTHER_FILES += \
    $$PWD/AppNavigationData.h.sample
