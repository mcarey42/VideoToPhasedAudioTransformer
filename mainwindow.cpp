#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    if (player)
        delete player;

    if (theSurface)
        delete theSurface;

    delete ui;
}


void MainWindow::on_loadButton_clicked()
{
    // Get the filename
    videoFileName = QFileDialog::getOpenFileName(this, tr("Open Vido File"), "~", tr("Video Files (*.mp4 *mov)"));

    // Return if no file chosen.
    if (videoFileName == "" || videoFileName == nullptr)
        return;

    // Set state to file chosen.
    videoFileChosen = true;
}



void MainWindow::on_saveButton_clicked()
{
    // Get the filename
    audioFileName = QFileDialog::getSaveFileName(this, tr("Save Sound File"), "~", tr("Sound Files (*.wav)"));

    // Return if no file chosen.
    if (audioFileName == "" || audioFileName == nullptr)
        return;

    // Set state to file chosen.
    audioFileChosen = true;
}

void MainWindow::on_processButton_clicked()
{
    if (!audioFileChosen || !videoFileChosen)
    {
        ui->outputistWidget->addItem("You must chose both a audio and video file to process.");
        return;
    }
    else
        ui->outputistWidget->addItem("Beginning processing.");

    // Load the video and process it.
    if (player)
    {
        delete player;
        player=nullptr;
    }

    player = new QMediaPlayer;
    theSurface = new CustomVideoSurface;


    player->setMedia(QUrl::fromLocalFile(videoFileName));

    // CHOSE ONE:

    if(0)
    {
        // For proving that this works.
        player->setVideoOutput(ui->videoWidget);
    }
    else
    {
        // For recording to a sound file...
        player->setVideoOutput(theSurface);
        theSurface->SetAudioFileOutput(audioFileName, atoi(ui->oversampleEdit->toPlainText().toStdString().c_str()));
    }

    ui->videoWidget->show();
    player->play();
}
