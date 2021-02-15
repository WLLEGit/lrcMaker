#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->selectAudio, &QPushButton::clicked, [=](){
        selectAudio();
    });
    connect(ui->selectLrc, &QPushButton::clicked, [=](){
        selectLrc();
    });

    connect(ui->addTime, &QPushButton::clicked, [=](){
        addTimeStamp();
    });

    connect(ui->timeSlider, &QSlider::valueChanged, [=](int progress){
        setTimeLabel(progress);
    });

    connect(ui->playButton, &QPushButton::clicked, [=](){
        play();
    });

    connect(ui->saveButton, &QPushButton::clicked, [=](){
        saveLrc();
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::selectLrc(){
    lrcPath = QFileDialog::getOpenFileName(this, "选择歌词文件", QString(), "*.txt *.lrc");
    QFile lrcFile(lrcPath, this);
    lrcFile.open(QFileDevice::ReadOnly);
    QTextStream lrc(&lrcFile);
    lrcText = lrc.readAll();
    ui->textEdit->setText(lrcText);
}

void MainWindow::selectAudio(){
    audioPath = QFileDialog::getOpenFileName(this, "选择音频文件", QString(), "*.mp3 *.wav *.flac");
    music->setMedia(QUrl::fromLocalFile(audioPath));
}

void MainWindow::addTimeStamp(){

}

void MainWindow::play(){

}

void MainWindow::saveLrc(){

}

void MainWindow::setTimeLabel(int progress){

}
