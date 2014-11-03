/********************************************************************************
** Form generated from reading UI file 'scada.ui'
**
** Created by: Qt User Interface Compiler version 5.3.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SCADA_H
#define UI_SCADA_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ScadaClass
{
public:

    void setupUi(QWidget *ScadaClass)
    {
        if (ScadaClass->objectName().isEmpty())
            ScadaClass->setObjectName(QStringLiteral("ScadaClass"));
        ScadaClass->resize(600, 400);

        retranslateUi(ScadaClass);

        QMetaObject::connectSlotsByName(ScadaClass);
    } // setupUi

    void retranslateUi(QWidget *ScadaClass)
    {
        ScadaClass->setWindowTitle(QApplication::translate("ScadaClass", "Scada", 0));
    } // retranslateUi

};

namespace Ui {
    class ScadaClass: public Ui_ScadaClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SCADA_H
