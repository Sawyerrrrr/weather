#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "weatherdata.h"
#include <QMainWindow>
#include <QMenu>
#include <QEvent>
#include <QContextMenuEvent>
#include <QCursor>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QMessageBox>
#include <QDebug>
#include <QUrl>
#include <QString>
#include <QNetworkRequest>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDateTime>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
protected:
    void contextMenuEvent(QContextMenuEvent *event);  //void QWidget::contextMenuEvent
    void mousePressEvent(QMouseEvent* event);
    void mouseMoveEvent(QMouseEvent* event);
    void getWeatherInfo(QString cityCode);
    void parseJson(QByteArray byteArray);
    void updateUi();

private slots:
    void on_pushButton_clicked();

private:
    void onReplied(QNetworkReply *reply);

private:
    Ui::MainWindow *ui;

    QMenu* mExitMenu;  //右键退出菜单
    QAction* mExitAct;  //退出的行为
    QPoint mOffset;
    QNetworkAccessManager* mManager;

    Today mToday;  //今天和6天的天气
    Day mDay[6];

    QList<QLabel*>mWeekList;
    QList<QLabel*>mDateList;
    QList<QLabel*>mAqiList;

    QList<QLabel*>mTypeList;
    QList<QLabel*>mTypeIconList;

    QList<QLabel*>mFxList;
    QList<QLabel*>mFlList;
    QMap<QString,QString>mTypeMap;
};
#endif // MAINWINDOW_H
