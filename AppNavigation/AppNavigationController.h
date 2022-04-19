#ifndef APPNAVIGATIONCONTROLLER_H
#define APPNAVIGATIONCONTROLLER_H

#include <QObject>
#include <QStack>
#include <QUuid>
#include <QVariantMap>

#include "AppNavigationData.h"

class AppNavigationController : public QObject
{
    Q_OBJECT

    Q_PROPERTY(AppNavigation::PageID currentPage READ currentPage NOTIFY currentPageChanged)

    Q_PROPERTY(QString pageIdKey READ pageIdKey CONSTANT)
    Q_PROPERTY(QString popupIdKey READ popupIdKey CONSTANT)
    Q_PROPERTY(QString popupUuidKey READ popupUuidKey CONSTANT)

    using PopupInfo = QPair<AppNavigation::PopupID, QUuid>;

public:
    explicit AppNavigationController(QObject *parent = nullptr);
    const AppNavigationController &instance() const;

    AppNavigation::PageID currentPage() const;

    QString pageIdKey() const;
    QString popupIdKey() const;
    QString popupUuidKey() const;

signals:
    void currentPageChanged(const AppNavigation::PageID pageId) const;

    // private signals used internally
    void pushPageOnStackView(const QUrl &url,
                             const QVariantMap &properties,
                             const bool immediate,
                             QPrivateSignal) const;
    void popPageFromStackView(const bool immediate,
                              QPrivateSignal) const;
    void popPageFromStackViewTo(const AppNavigation::PageID pageId,
                                const QVariantMap &properties,
                                const bool immediate,
                                QPrivateSignal) const;
    void replacePageOnStackView(const QUrl &url,
                                const QVariantMap &properties,
                                const bool immediate,
                                QPrivateSignal);
    void replaceAllPagesOnStackView(const QUrl &url,
                                    const QVariantMap &properties,
                                    const bool immediate,
                                    QPrivateSignal);
    void replaceAllPagesUpToPageOnStackView(const AppNavigation::PageID pageToKeep,
                                            const QUrl &pageToAdd,
                                            const QVariantMap &properties,
                                            const bool immediate,
                                            QPrivateSignal);

    void pushPopupOnStackView(const QUrl &url,
                              const QVariantMap &properties,
                              QPrivateSignal);
    void popPopupFromStackView(QPrivateSignal);

public slots:
    void enterPage(const AppNavigation::PageID pageId,
                   QVariantMap properties = {},
                   const bool immediate = false);
    void replacePage(const AppNavigation::PageID pageId,
                     QVariantMap properties = {},
                     const bool immediate = false);
    void replaceAllPages(const AppNavigation::PageID pageId,
                         QVariantMap properties = {},
                         const bool immediate = false);
    void replaceUpToPage(const AppNavigation::PageID pageToKeepId,
                         const AppNavigation::PageID pageToAddId,
                         QVariantMap properties = {},
                         const bool immediate = false);
    void goBack(const bool immediate = false);
    void goBackToPage(const AppNavigation::PageID pageId,
                      const QVariantMap &properties = {},
                      const bool immediate = false);

    void openPopup(const AppNavigation::PopupID popupId,
                   QVariantMap properties = {});
    void closePopup();

private:
    QStack<AppNavigation::PageID> m_pageStack;
    QStack<PopupInfo> m_popupStack;
};

#endif // APPNAVIGATIONCONTROLLER_H
