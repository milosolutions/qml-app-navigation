# QML app navigation

Simple QML module providing application navigation based on StackView.

## How to use

To integrate it with your application:

1. Include `AppNavigation/AppNavigation.pri` in your application .pro file.
2. Copy `AppNavigation/AppNavigationData.h.sample` as `AppNavigation/AppNavigationData.h`, and define your application pages and popups.
3. In order to use AppStackView in your QML code, use `import "qrc:/AppNavigation"`.
4. In order to use PageID/PopupID enumerations and AppNavigationController (registered as singleton) use `import com.milosolutions.AppNavigation 1.0`.

