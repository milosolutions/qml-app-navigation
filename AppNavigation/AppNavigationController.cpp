#include "AppNavigationController.h"

#include <QCoreApplication>
#include <QGlobalStatic>
#include <QQmlEngine>
#include <QUuid>

Q_GLOBAL_STATIC(AppNavigationController, appNavigationControllerInstance)

namespace
{
    constexpr QStringView PAGE_ID_KEY(u"pageId");
    constexpr QStringView POPUP_ID_KEY(u"popupId");
    constexpr QStringView POPUP_UUID_KEY(u"popupUuid");

    void registerTypes()
    {
        qmlRegisterUncreatableMetaObject(AppNavigation::staticMetaObject,
                                         "com.milosolutions.AppNavigation",
                                         1, 0,
                                         "AppNavigation",
                                         "This is a C++ namespace");

        qmlRegisterSingletonType<AppNavigationController>("com.milosolutions.AppNavigation",
                                                          1, 0,
                                                          "AppNavigationController",
                                                          [](QQmlEngine *, QJSEngine *)
        {
            QQmlEngine::setObjectOwnership(appNavigationControllerInstance, QQmlEngine::CppOwnership);
            return appNavigationControllerInstance;
        });
    }
}

AppNavigationController::AppNavigationController(QObject *parent)
    : QObject(parent)
{

}

const AppNavigationController &AppNavigationController::instance() const
{
    return *appNavigationControllerInstance;
}

AppNavigation::PageID AppNavigationController::currentPage() const
{
    return m_pageStack.top();
}

QString AppNavigationController::pageIdKey() const
{
    return PAGE_ID_KEY.toString();
}

QString AppNavigationController::popupIdKey() const
{
    return POPUP_ID_KEY.toString();
}

QString AppNavigationController::popupUuidKey() const
{
    return POPUP_UUID_KEY.toString();
}

void AppNavigationController::enterPage(const AppNavigation::PageID pageId,
                                        QVariantMap properties,
                                        const bool immediate)
{
    if (m_pageStack.contains(pageId))
    {
        goBackToPage(pageId, properties, immediate);
        return;
    }

    properties.insert(pageIdKey(), QVariant::fromValue(pageId));

    m_pageStack.push(pageId);
    emit pushPageOnStackView(AppNavigation::pageUrls[pageId], properties, immediate, QPrivateSignal());
    emit currentPageChanged(pageId);
}

void AppNavigationController::replacePage(const AppNavigation::PageID pageId,
                                          QVariantMap properties,
                                          const bool immediate)
{
    if (m_pageStack.contains(pageId))
    {
        goBackToPage(pageId, properties, immediate);
        return;
    }

    properties.insert(pageIdKey(), QVariant::fromValue(pageId));

    m_pageStack.pop();
    m_pageStack.push(pageId);
    emit replacePageOnStackView(AppNavigation::pageUrls[pageId], properties, immediate, QPrivateSignal());
    emit currentPageChanged(pageId);
}

void AppNavigationController::replaceAllPages(const AppNavigation::PageID pageId,
                                              QVariantMap properties,
                                              const bool immediate)
{
    if (m_pageStack.contains(pageId) && m_pageStack.indexOf(pageId) == 0)
    {
        goBackToPage(pageId, properties, immediate);
        return;
    }

    properties.insert(pageIdKey(), QVariant::fromValue(pageId));

    m_pageStack.clear();
    m_pageStack.push(pageId);

    emit replaceAllPagesOnStackView(AppNavigation::pageUrls[pageId], properties, immediate, QPrivateSignal());
    emit currentPageChanged(pageId);
}

void AppNavigationController::replaceUpToPage(const AppNavigation::PageID pageToKeepId,
                                              const AppNavigation::PageID pageToAddId,
                                              QVariantMap properties,
                                              const bool immediate)
{
    if (m_pageStack.contains(pageToAddId) && m_pageStack.indexOf(pageToAddId) == 0)
    {
        goBackToPage(pageToAddId, properties, immediate);
        return;
    }

    if (!m_pageStack.contains(pageToKeepId))
    {
        replaceAllPages(pageToAddId, properties, immediate);
        return;
    }

    properties.insert(pageIdKey(), QVariant::fromValue(pageToAddId));

    while (m_pageStack.top() != pageToKeepId) {
        m_pageStack.pop();
    }
    m_pageStack.push(pageToAddId);

    emit replaceAllPagesUpToPageOnStackView(pageToKeepId,
                                            AppNavigation::pageUrls[pageToAddId],
                                            properties,
                                            immediate,
                                            QPrivateSignal());
    emit currentPageChanged(m_pageStack.top());
}

void AppNavigationController::goBack(const bool immediate)
{
    if (!m_popupStack.isEmpty())
    {
        return;
    }

    if (m_pageStack.count() <= 1)
    {
        return;
    }

    m_pageStack.pop();
    emit popPageFromStackView(immediate, QPrivateSignal());
    emit currentPageChanged(m_pageStack.top());
}

void AppNavigationController::goBackToPage(const AppNavigation::PageID pageId,
                                           const QVariantMap &properties,
                                           const bool immediate)
{
    if (!m_popupStack.isEmpty())
    {
        return;
    }

    if (m_pageStack.count() <= 1)
    {
        return;
    }

    if (!m_pageStack.contains(pageId))
    {
        emit replacePageOnStackView(AppNavigation::pageUrls[pageId], properties, immediate, QPrivateSignal());
        return;
    }

    AppNavigation::PageID current = m_pageStack.top();
    while (current != pageId)
    {
        m_pageStack.pop();
        current = m_pageStack.top();
    }

    emit popPageFromStackViewTo(pageId, properties, immediate, QPrivateSignal());
    emit currentPageChanged(m_pageStack.top());
}

void AppNavigationController::openPopup(const AppNavigation::PopupID popupId,
                                        QVariantMap properties)
{
    const QUuid popupUuid = QUuid::createUuid();

    properties.insert(popupIdKey(), QVariant::fromValue(popupId));
    properties.insert(popupUuidKey(), popupUuid);

    m_popupStack.push({popupId, popupUuid});
    emit pushPopupOnStackView(AppNavigation::popupUrls[popupId], properties, QPrivateSignal());
}

void AppNavigationController::closePopup()
{
    if (m_popupStack.isEmpty())
    {
        return;
    }

    m_popupStack.pop();
    emit popPopupFromStackView(QPrivateSignal());
}

Q_COREAPP_STARTUP_FUNCTION(registerTypes)
