#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QTextCodec>
#include <QDateTime>
#include <QLabel>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::getCurrentPosition()
{
    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    locReply = manager->get(QNetworkRequest(QUrl(QString("https://ip.nf/me.json"))));
    connect(manager,SIGNAL(finished(QNetworkReply*)),this,SLOT(parseLocation()));
}

void MainWindow::parseLocation()
{
    connect(this,SIGNAL(locFinish()),this,SLOT(getWeather()));
    QTextCodec *codec = QTextCodec::codecForName("utf8");
    QString raw = codec->toUnicode(locReply->readAll());
    QJsonParseError jsonError;
    QJsonDocument replyJson=QJsonDocument::fromJson(raw.toUtf8(),&jsonError);
    if(jsonError.error==QJsonParseError::NoError)
    {
        QVariantMap replyMap=replyJson.toVariant().toMap();
        ip=replyMap["ip"].toMap();
        ui->label->setWordWrap(true);
        ui->label->setText(QString("当前位置为（基于网络）：\n%1°N，%2°E").arg(ip["latitude"].toDouble()).arg(ip["longitude"].toDouble()));
        locFinish();
    }
    else{
        QMessageBox::critical(this,"位置获取失败","嘤嘤嘤～位置获取失败了～");
    }
}

void MainWindow::getWeather()
{
    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    QDateTime currentDateTime = QDateTime::currentDateTime();
    connect(manager,SIGNAL(finished(QNetworkReply*)),this,SLOT(displayWeather()));
    uint unixtime = currentDateTime.toTime_t();
    reply = manager->get(QNetworkRequest(QUrl(QString("https://api.caiyunapp.com/v2/Y2FpeXVuIGFuZHJpb2QgYXBp/%1,%2/weather?lang=zh_CN&span=6&begin=%3&hourlysteps=24&alert=true&version=3.0.1"
                                                      ).arg(ip["longitude"].toDouble()).arg(ip["latitude"].toDouble()).arg(unixtime))));
}

void MainWindow::displayWeather()
{
    QTextCodec *codec = QTextCodec::codecForName("utf8");
    QString raw = codec->toUnicode(reply->readAll());
    QJsonParseError jsonError;
    QJsonDocument replyJson=QJsonDocument::fromJson(raw.toUtf8(),&jsonError);
    if(jsonError.error==QJsonParseError::NoError)
    {
        QVariantMap replyMap=replyJson.toVariant().toMap();
        QVariantMap result=replyMap["result"].toMap();
        QVariantMap hourly=result["hourly"].toMap();
        ui->label->setWordWrap(true);
        ui->labelWeather->setText(hourly["description"].toString());
    }
    else
        QMessageBox::critical(this,"天气获取失败","嘤嘤嘤～天气获取失败了～");
}
