# QML app navigation

Simple QML module providing application navigation based on StackView.

## How to use

To integrate it with your application:

1. Create and add `AppNavigationData.h` file to your project (HEADERS variable in *.pro). The easiest would be to copy and rename `AppNavigation/AppNavigationData.h.sample` file.
2. Include `AppNavigation/AppNavigation.pri` in your application .pro file. It verifies whether `AppNavigationData.h` is one of a project headers, therefore it should be included after `HEADERS` definition in your *.pro file.
3. Define your application pages and popups in `AppNavigationData.h`. Sample file includes comments with hints how to do so.
4. In order to use AppStackView, AppPage and AppPopup in your QML code, use `import "qrc:/AppNavigation"`.
5. In order to use PageID/PopupID enumerations and AppNavigationController (registered as singleton) use `import com.milosolutions.AppNavigation 1.0`.
