#include "mainwindow.h"
#include "ui_mainwindow.h"

int getRandomNumber(int min, int max);

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
    fileName = QFileDialog::getOpenFileName(this, "Open File", QDir::currentPath(), "jpg-files (*.jpg)");//Открываем диалоговое окно для загрузки файла
    original.load(fileName);//Создаём изображение
    width = original.width();
    height = original.height();
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
bool MainWindow::isBetter(const QRect& rect, const QColor& color) const
{
    double currentSim = 0;//Текущая схожеть
    double updateSim = 0;//Схожесть после обновления

    //Просматриваем прямоугольник
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

//Создаём случайный прямоугольник
QRect MainWindow::createRandomRectangle() const{

    QVector<int> topLeft;//Координаты верхнего левого края
    QVector<int> bottomRight;//Координаты правого нижнего края

    topLeft.push_back(getRandomNumber(0,width/quality));
    topLeft.push_back(getRandomNumber(0,height/quality));
    bottomRight.push_back(getRandomNumber(0,width/quality));
    bottomRight.push_back(getRandomNumber(0,height/quality));


    if (topLeft[0] > bottomRight[0]){
        std::swap(topLeft[0], bottomRight[0]);
    }

    if (topLeft[1] > bottomRight[1]){
        std::swap(topLeft[1], bottomRight[1]);
    }

    QRect rect{topLeft[0],topLeft[1],abs(bottomRight[0]-topLeft[0]),abs(bottomRight[1]-topLeft[1])};

    if(rect.width()*rect.height()==0){
        return createRandomRectangle();
    }else{
        return rect;
    }
}

void MainWindow::step(){
    auto rect = createRandomRectangle();

    QColor color(getRandomNumber(0,255),getRandomNumber(0,255),getRandomNumber(0,255));

    if(isBetter(rect,color)){
        for(int y = rect.top();y!=rect.top()+rect.height();y++){
            for(int x = rect.left(); x!=rect.left()+rect.width();x++){
                result.setPixelColor(x,y,color);
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

void MainWindow::on_start_clicked()
{
    srand(static_cast<unsigned int>(time(0)));
    fileName = QFileDialog::getOpenFileName(this, "Open File", QDir::currentPath(), "jpg-files (*.jpg)");//Открываем диалоговое окно для загрузки файла
    result.load(fileName);//Создаём изображение
    for(int j=1;j<=20;j++){
        quality=j;
        for(int i=0;i<10000;i++){
            step();
        }
    }
    result.save(fileName,"JPG",-1);
    QCoreApplication::quit();
}

int getRandomNumber(int min, int max){
    static const double fraction = 1.0 / (static_cast<double>(RAND_MAX) + 1.0);
    return static_cast<int>(rand() * fraction * (max - min + 1) + min);
}
