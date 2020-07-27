#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVideoWidget>
#include <QtMultimedia>
#include <QMediaPlayer>

#include "customvideosurface.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_loadButton_clicked();

    void on_saveButton_clicked();

    void on_processButton_clicked();

private:
    Ui::MainWindow *ui;

    QMediaPlayer *player = nullptr;
    CustomVideoSurface *theSurface = nullptr;

    QString videoFileName;
    QString audioFileName;

    bool videoFileChosen = false;
    bool audioFileChosen = false;
};
#endif // MAINWINDOW_H
