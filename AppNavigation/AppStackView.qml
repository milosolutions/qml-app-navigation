import QtQuick 2.12
import QtQuick.Controls 2.12

import com.milosolutions.AppNavigation 1.0

StackView {
    id: stackView

    required property var applicationWindow

    function operation(immediate)
    {
        return immediate ? StackView.Immediate : StackView.Transition
    }

    Connections {
        target: applicationWindow

        function onClosing(close)
        {
            if (appStackView.depth > 1)
            {
                close.accepted = false

                AppNavigationController.goBack()
            }
        }
    }

    Connections {
        target: AppNavigationController

        function onPushPageOnStackView(url, properties, immediate)
        {
            stackView.push(Qt.resolvedUrl(url), properties, operation(immediate))
        }

        function onPopPageFromStackView(immediate)
        {
            stackView.pop(operation(immediate))
        }

        function onPopPageFromStackViewTo(pageId, properties, immediate)
        {
            let targetPage = stackView.find(function(item, index) {
                return item[AppNavigationController.pageIdKey] === pageId
            })
            stackView.pop(targetPage, operation(immediate))

            let currentPage = stackView.currentItem

            for (let propertyName in properties)
            {
                currentPage[propertyName] = properties[propertyName]
            }
        }

        function onReplacePageOnStackView(url, properties, immediate)
        {
            stackView.replace(Qt.resolvedUrl(url), properties, operation(immediate))
        }

        function onReplaceAllPagesOnStackView(url, properties, immediate)
        {
            stackView.replace(null, Qt.resolvedUrl(url), properties, operation(immediate))
        }

        function onReplaceAllPagesUpToPageOnStackView(pageToKeepId, pageToAddUrl, properties, immediate)
        {
            let pageToKeep = stackView.find(function(item, index) {
                return item[AppNavigationController.pageIdKey] === pageToKeepId
            })

            let lastPageToToBeReplaced = stackView.get(pageToKeep.StackView.index + 1)

            stackView.replace(lastPageToToBeReplaced,
                              Qt.resolvedUrl(pageToAddUrl),
                              properties,
                              operation(immediate))
        }

        function onPushPopupOnStackView(url, properties)
        {
            popupFactory.createPopup(url, properties)
        }

        function onPopPopupFromStackView()
        {
            popupFactory.destroyPopup()
        }
    }

    QtObject {
        id: popupFactory

        property var popupStack: []

        function createPopup(url, properties)
        {
            let popupComponent = Qt.createComponent(url)

            if (popupComponent.status === Component.Error) {
                console.error("Error, while creating popup:", popupComponent.errorString())
                return
            }

            let popupItem = popupComponent.createObject(applicationWindow, properties)
            popupStack.push(popupItem)
            popupItem.open()

            popupItem.closed.connect(AppNavigationController.closePopup)
        }

        function destroyPopup()
        {
            let popupItem = popupStack.pop()

            popupItem.close()
            popupItem.destroy()
        }
    }
}
