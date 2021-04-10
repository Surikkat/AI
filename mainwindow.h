#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QPixmap>

namespace Ui { class MainWindow; }


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_LoadFile_clicked();

private:
    Ui::MainWindow *ui;
    QString fileName;
    QImage original;
    QImage result;
};

#endif // MAINWINDOW_H
