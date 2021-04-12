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

//Загрузка файла
void MainWindow::on_LoadFile_clicked()
{
    fileName = QFileDialog::getOpenFileName(this, "Open File", QDir::currentPath(), "txt-files (*.txt)");//Открываем диалоговое окно для загрузки файла
    original.load(fileName);//Создаём изображение
    width = original.width();
    height = original.height();
    randomWidth.bounded(0,width);
    randomHeight.bounded(0,height);
    randomColor.bounded(0,255);
    QPixmap pix = QPixmap::fromImage(original);//Создаём растровое изображение
    ui->OriginalIMG->setPixmap(pix);//Помещаем растровое изображение на левый лейбл
}

//Проверка, насколько похожи два пикселя
double MainWindow::similatingPix(const QColor& a,const QColor& b) const
{
    double simPixels = 0;//Изначально пиксели не похожи

    //Вычисляем насколько они похожи по каждому компоненту
    simPixels += abs(a.red()-b.red());//Красный
    simPixels += abs(a.green()-b.green());//Зелёный
    simPixels += abs(a.blue()-b.blue());//Голубой
    simPixels += abs(a.alpha()-b.alpha());//Прозрачность

    return 1 - (simPixels/(255*4.0f));//Возвращаем нормализованное значение
}

//Стало лучше?
bool MainWindow::isBetter(const QRect& rect, const QColor color) const
{
    double currentSim = 0;//Текущая схожеть
    double updateSim = 0;//Схожесть после обновления

    //Просматриваем прямоуггольник
    for(int y = rect.top();y != rect.top()+rect.height();y++){
        for(int x=rect.left();x!=rect.left()+rect.width();x++){
            currentSim += similatingPix(original.pixelColor(x,y), result.pixelColor(x,y));//Проверяем насколько похожи пиксели внутри этого прямоугольника до изменения
            updateSim += similatingPix(original.pixelColor(x,y), color);//Проверяем насколько похожи пиксели внутри этого прямоугольника после изменения
        }
    }

    //Нормализуем (0-1)
    currentSim = currentSim/(rect.height()*rect.width());
    updateSim = updateSim/(rect.height()*rect.width());

    return (updateSim>currentSim);//1 - лучше, 0 - хуже
}

//Создаём случччайный прямоугольник
QRect MainWindow::createRandomRectangle() const{
    QVector<int> topLeft(randomWidth.generate(),randomHeight.generate());//Координаты верхнего левого края
    QVector<int> bottomRight(randomWidth.generate(),randomHeight.generate());//Координаты правого нижнего края

    //Если верхний левый угол праве нижнего правого, то меняю их местами
    if(topLeft[0]>bottomRight[0]){
        std::swap(topLeft[0],bottomRight[0]);
    }

    //Если верхний левый угол
    if(topLeft[1]>bottomRight[1]){
        std::swap(topLeft[1],bottomRight[1]);
    }

    QRect rect{topLeft[0],topLeft[1],bottomRight[0]-topLeft[0],bottomRight[1]-topLeft[1]};

    if(rect.width()*rect.height()==0){
        return createRandomRectangle();
    }else{
        return rect;
    }
}

void MainWindow::step(){
    QRect rect = createRandomRectangle();

    QColor color(randomColor.generate(),randomColor.generate(),randomColor.generate());

    if(isBetter(rect,color)){
        for(int y = rect.top();y!=rect.top()+rect.height();y++){
            for(int x = rect.left(); x!=rect.left()+rect.width();x++){
                result.setPixel(x,y,color);
            }
        }
    }
}

//Геттер ширины изображения
int MainWindow::getWidth() const{
    return width;
}

//Геттер высоты изображения
int MainWindow::getHeight() const{
    return height;
}


