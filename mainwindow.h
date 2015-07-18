#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QCloseEvent>
#include <QStringListModel>
#include <QItemSelection>
#include <QHash>
#include "irc.h"
#include "textdialog.h"

namespace Ui {
class MainWindow;
}

class TextDialog;
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void dialogFinished(QString value);

private slots:
    void closeEvent(QCloseEvent *event);
    void debugOutput(QString text);
    void networkStatus(QString text);
    void connect();
    void disconnect();
    void nickDialog();
    void joinDialog();
    void changeChannel(QModelIndex chanSelected);
    void closeChannel();
    void toggleFullscreen(bool checked);
    void sendMessage();
    void onMessage(QString sender, QString target, QString message);
    void onJoin(QString nick, QString target, QString host, QString realname);
    void displayUsers(QString channel, QStringList users);

private:
    Ui::MainWindow *ui;
    Irc *irc;
    QString chan, dialog;
    QHash<QString,QString> chanContent;
    QHash<QString,QStringList> chanUserlist;
    QStringList chanList;
    QStringListModel *chanListModel, *userListModel;
    TextDialog *textDialog;
};

#endif // MAINWINDOW_H
