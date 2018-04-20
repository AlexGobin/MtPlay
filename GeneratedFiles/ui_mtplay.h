/********************************************************************************
** Form generated from reading UI file 'mtplay.ui'
**
** Created by: Qt User Interface Compiler version 5.9.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MTPLAY_H
#define UI_MTPLAY_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QWidget>
#include "Mt_VideoWidget.h"

QT_BEGIN_NAMESPACE

class Ui_mtplayClass
{
public:
    QGridLayout *gridLayout;
    Mt_VideoWidget *video;
    QPushButton *btn_open;
    QPushButton *btn_play;
    QSlider *mySlider;
    QLabel *label_time;
    QLabel *label_shu;
    QLabel *label_timeplay;

    void setupUi(QWidget *mtplayClass)
    {
        if (mtplayClass->objectName().isEmpty())
            mtplayClass->setObjectName(QStringLiteral("mtplayClass"));
        mtplayClass->resize(1000, 621);
        mtplayClass->setStyleSheet(QStringLiteral("background-color: rgb(161, 187, 215);"));
        gridLayout = new QGridLayout(mtplayClass);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setContentsMargins(5, 5, 5, 5);
        video = new Mt_VideoWidget(mtplayClass);
        video->setObjectName(QStringLiteral("video"));
        video->setStyleSheet(QStringLiteral(""));

        gridLayout->addWidget(video, 0, 0, 1, 6);

        btn_open = new QPushButton(mtplayClass);
        btn_open->setObjectName(QStringLiteral("btn_open"));
        btn_open->setEnabled(true);
        btn_open->setStyleSheet(QStringLiteral("border-image: url(:/mtplay/im/open.png);"));
        btn_open->setIconSize(QSize(48, 48));
        btn_open->setAutoDefault(false);
        btn_open->setFlat(false);

        gridLayout->addWidget(btn_open, 1, 0, 1, 1);

        btn_play = new QPushButton(mtplayClass);
        btn_play->setObjectName(QStringLiteral("btn_play"));
        btn_play->setStyleSheet(QStringLiteral("border-image: url(:/mtplay/im/play.png);"));

        gridLayout->addWidget(btn_play, 1, 1, 1, 1);

        mySlider = new QSlider(mtplayClass);
        mySlider->setObjectName(QStringLiteral("mySlider"));
        mySlider->setStyleSheet(QStringLiteral(""));
        mySlider->setMaximum(999);
        mySlider->setOrientation(Qt::Horizontal);

        gridLayout->addWidget(mySlider, 1, 2, 1, 1);

        label_time = new QLabel(mtplayClass);
        label_time->setObjectName(QStringLiteral("label_time"));

        gridLayout->addWidget(label_time, 1, 3, 1, 1);

        label_shu = new QLabel(mtplayClass);
        label_shu->setObjectName(QStringLiteral("label_shu"));

        gridLayout->addWidget(label_shu, 1, 4, 1, 1);

        label_timeplay = new QLabel(mtplayClass);
        label_timeplay->setObjectName(QStringLiteral("label_timeplay"));

        gridLayout->addWidget(label_timeplay, 1, 5, 1, 1);


        retranslateUi(mtplayClass);

        btn_open->setDefault(false);


        QMetaObject::connectSlotsByName(mtplayClass);
    } // setupUi

    void retranslateUi(QWidget *mtplayClass)
    {
        mtplayClass->setWindowTitle(QApplication::translate("mtplayClass", "mtplay", Q_NULLPTR));
        btn_open->setText(QString());
        btn_play->setText(QString());
        label_time->setText(QApplication::translate("mtplayClass", "000:00", Q_NULLPTR));
        label_shu->setText(QApplication::translate("mtplayClass", "|", Q_NULLPTR));
        label_timeplay->setText(QApplication::translate("mtplayClass", "000:00", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class mtplayClass: public Ui_mtplayClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MTPLAY_H
