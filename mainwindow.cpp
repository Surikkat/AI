#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_LoadFile_clicked()
{
    fileName = QFileDialog::getOpenFileName(this, "Open File", QDir::currentPath(), "txt-files (*.txt)");
    original.load(fileName);
    QPixmap pix = QPixmap::fromImage(original);
    ui->OriginalIMG->setPixmap(pix);
}

double MainWindow::similatingPix(const QColor& a,const QColor& b) const
{
    double simPixels = 0;

    simPixels += abs(a.red()-b.red());
    simPixels += abs(a.green()-b.green());
    simPixels += abs(a.blue()-b.blue());
    simPixels += abs(a.alpha()-b.alpha());

    return 1 - (simPixels/(255*4.0f));
}

bool MainWindow::isBetter(const QRect& rect, const QColor color) const
{
    double currentSim = 0;
    double updateSim = 0;

    for(int y = rect.top();y != rect.top()+rect.height();y++){
        for(int x=rect.left();x!=rect.left()+rect.width();x++){
            currentSim += similatingPix(original.pixelColor(x,y), result.pixelColor(x,y));
            updateSim += similatingPix(original.pixelColor(x,y), color);
        }
    }

    //Нормализуем (0-1)
    currentSim = currentSim/(rect.height()*rect.width());
    updateSim = updateSim/(rect.height()*rect.width());

    return (updateSim>currentSim);
}


