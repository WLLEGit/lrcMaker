#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMediaPlayer>
#include <QFile>
#include <QFileDialog>
#include <QTextStream>
#include <QStringList>
#include <QTextCursor>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QMediaPlayer *music = new QMediaPlayer(this);
    QString audioPath, lrcPath;
    QString lrcText;
    QString timeStamp;
    int duration;
    int cursorPosition=0;
    int outFocusClick=0;

    void selectAudio();
    void selectLrc();
    void play();
    void addTimeStamp();
    void saveLrc();

};
#endif // MAINWINDOW_H
