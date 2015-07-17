#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    irc = new Irc;
    irc->getChans()->append(new Chan(irc, "#yairc"));
    connect(irc, SIGNAL(debugOutput(QString)), this, SLOT(debugOutput(QString)));
    connect(irc, SIGNAL(networkOutput(QString)), this, SLOT(networkDisplay(QString)));
    connect(irc, SIGNAL(onMessage(QString, QString, QString)), this, SLOT(onMessage(QString, QString, QString)));
    connect(irc, SIGNAL(onJoin(QString, QString, QString, QString)), this, SLOT(onJoin(QString, QString, QString, QString)));
    connect(ui->listView, SIGNAL(clicked(QModelIndex)), this, SLOT(changeChannel(QModelIndex)));
    connect(ui->lineEdit, SIGNAL(returnPressed()), this, SLOT(sendMessage()));
    connect(ui->actionFermer_l_onglet, SIGNAL(triggered()), this, SLOT(closeChannel()));
    irc->connect("YaIRC", "irc.t411.io", 6667, "YaIRC");
    chanList.append("Serveur");
    chanListModel = new QStringListModel;
    ui->listView->setModel(chanListModel);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::debugOutput(QString text)
{
    qDebug() << text;
}

void MainWindow::networkDisplay(QString text)
{
    chanContent["Serveur"] += text + "\n";
    if (chan == "Serveur") ui->textEdit->append(text);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    irc->disconnect();
    event->accept();
}

void MainWindow::changeChannel(QModelIndex chanSelected)
{
    chan = chanSelected.data().toString();
    ui->textEdit->setText(chanContent[chan]);
}

void MainWindow::closeChannel()
{
    irc->part(chan);
    chanList.removeAll(chan);
    chanListModel->setStringList(chanList);
    chan = chanList.at(0);
    ui->textEdit->setText(chanContent["Serveur"]);
}

void MainWindow::sendMessage()
{
    irc->sendMessage(chan, ui->lineEdit->text());
    chanContent[chan] += "<" + irc->getNick() + "> " + ui->lineEdit->text() + "\n";
    ui->textEdit->append("<" + irc->getNick() + "> " + ui->lineEdit->text());
    ui->lineEdit->clear();
}

void MainWindow::onMessage(QString sender, QString target, QString message)
{
    chanContent[target] += "<" + sender + "> " + message + "\n";
    if (target == chan) ui->textEdit->append("<" + sender + "> " + message);
}

void MainWindow::onJoin(QString nick, QString target, QString host, QString realname)
{
    if (nick == irc->getNick())
    {
        chanList.append(target);
        chanListModel->setStringList(chanList);
    }
    chanContent[target] += nick + " (" + realname + "@" + host + ") a rejoint " + target + "\n";
    if (target == chan) ui->textEdit->append(nick + " (" + realname + "@" + host + ") a rejoint " + target);
}
