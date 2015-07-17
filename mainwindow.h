#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QCloseEvent>
#include <QStringListModel>
#include <QItemSelection>
#include <QHash>
#include "irc.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void closeEvent(QCloseEvent *event);
    void debugOutput(QString text);
    void networkDisplay(QString text);
    void changeChannel(QModelIndex chanSelected);
    void closeChannel();
    void sendMessage();
    void onMessage(QString sender, QString target, QString message);
    void onJoin(QString nick, QString target, QString host, QString realname);

private:
    Ui::MainWindow *ui;
    Irc *irc;
    QString chan;
    QHash<QString,QString> chanContent;
    QStringList chanList;
    QStringListModel* chanListModel;
};

#endif // MAINWINDOW_H
