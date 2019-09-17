#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QNetworkReply *reply;
    QNetworkReply *locReply;
    QVariantMap ip;

private slots:
    void getCurrentPosition();
    void parseLocation();
    void getWeather();
    void displayWeather();

signals:
    bool locFinish();
};

#endif // MAINWINDOW_H
