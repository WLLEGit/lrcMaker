#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTextCursor>
#include <QTextBlock>
#include <QTextLayout>
#include <QDebug>
#include "mytextedit.h"
#include <QMessageBox>
#include <QString>
#include <QScrollBar>

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

    connect(ui->timeSlider, &QSlider::valueChanged, [=](int position){
        double t = position / 1000.0;
        int min = (int)t / 60;
        double sec = t - min*60;

        timeStamp = "[" + QString("%1").arg(min, 2, 10, QChar('0')) + ":" + QString("%1").arg(sec, 5, 'f', 2, '0') + "]";
        ui->timeLabel->setText(timeStamp.mid(1, 5));
        music->setPosition(position);
    });

    connect(ui->playButton, &QPushButton::clicked, [=](){
        play();
    });

    connect(ui->saveButton, &QPushButton::clicked, [=](){
        saveLrc();
    });

    connect(ui->textEdit, &QTextEdit::textChanged, [=](){
        lrcText = ui->textEdit->toPlainText();
    });

    connect(ui->textEdit, &MyTextEdit::focusIn, [=](){
        outFocusClick=0;qDebug() << "focusIn";
    });
    connect(ui->textEdit, &MyTextEdit::focusOut, [=](){
        qDebug() << "focusOut";
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
    lrc.setCodec("utf-8");
    lrcText = lrc.readAll();
    ui->textEdit->setText(lrcText);
    lrcFile.close();
}

void MainWindow::selectAudio(){
    audioPath = QFileDialog::getOpenFileName(this, "选择音频文件", QString(), "*.mp3 *.wav *.flac");
    music->setMedia(QUrl::fromLocalFile(audioPath));
    connect(ui->timeSlider, &QSlider::valueChanged, music, &QMediaPlayer::setPosition);
    connect(music, &QMediaPlayer::positionChanged, ui->timeSlider, [=](int pos){
        ui->timeSlider->setValue(pos);
    });
}

void MainWindow::addTimeStamp(){
    QTextCursor tc;
    int sliderPos = ui->textEdit->verticalScrollBar()->value();

    if(!outFocusClick){
        qDebug() << "focusInAddTimeStamp";
        tc = ui->textEdit->textCursor();
        cursorPosition = tc.position();
        ui->textEdit->setText(lrcText.mid(0, tc.position()) + timeStamp + lrcText.mid(tc.position()));
    }
    else{
//        qDebug() << "focusOutAddTimeStamp";
//        QTextCursor tc = ui->textEdit->textCursor();
//        tc.setPosition(cursorPosition);
//        ui->textEdit->setTextCursor(tc);
//        qDebug() << ui->textEdit->textCursor().position();
//        ui->textEdit->moveCursor(QTextCursor::Down);
//        qDebug() << ui->textEdit->textCursor().position();
//        tc = ui->textEdit->textCursor();
//        cursorPosition = tc.position();
//        ui->textEdit->setText(lrcText.mid(0, tc.position()) + timeStamp + lrcText.mid(tc.position()));
        int nextPos = lrcText.indexOf('\n', cursorPosition) + 1;
        if(!nextPos)
            return;
        else
            cursorPosition = nextPos;

        tc = ui->textEdit->textCursor();
        tc.setPosition(cursorPosition);
        ui->textEdit->setTextCursor(tc);
        ui->textEdit->setText(lrcText.mid(0, tc.position()) + timeStamp + lrcText.mid(tc.position()));
    }
    ui->textEdit->verticalScrollBar()->setSliderPosition(sliderPos);

    outFocusClick++;
}

void MainWindow::play(){
    if(music->state() == QMediaPlayer::StoppedState || music->state() == QMediaPlayer::PausedState){
        music->play();
        duration = music->duration();
        ui->timeSlider->setMaximum(duration);
    }
    else if(music->state() == QMediaPlayer::PlayingState)
        music->pause();
}

void MainWindow::saveLrc(){
    QString savePath;
    savePath = QFileDialog::getSaveFileName(this, "选择保存路径和文件名", QString(), "*.lrc");

    if(savePath.isEmpty())
        return;

    QFile toSaveLrc(savePath, this);
    QTextStream lrcStream(&toSaveLrc);
    if(!toSaveLrc.open(QFileDevice::WriteOnly | QFileDevice::Truncate)){
        QMessageBox::warning(this, "打开写入文件", "打开要写入的文件失败，请检查文件名和是否具有写入权限！");
        return;
    }
    lrcStream << lrcText;
    toSaveLrc.close();
}
