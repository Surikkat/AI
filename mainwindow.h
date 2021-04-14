#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QPixmap>
#include <QRandomGenerator>
#include <QColor>
#include <cmath>
#include <QRect>
#include <QVector2D>


namespace Ui { class MainWindow; }


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void step();//Шаг алгоритма
    int getWidth()const;//Геттер ширины
    int getHeight()const;//Геттер высоты

private slots:
    void on_LoadFile_clicked();//Загрузка изображения
    void on_start_clicked();

private:
    Ui::MainWindow *ui;
    QString fileName;//Имя файла с изображением
    QImage original;//Оригинальное изображение
    QImage result;//Изображение, сгенерированное алгоритмом
    int width;//Ширина изображения
    int height;//Высота изображения
    int quality;

    void draw();//Функция рисования
    bool isBetter(const QRect& rect,const QColor& color)const;//Получилось лучше?
    double similatingPix(const QColor& a,const QColor& b)const;//Насколько похожи пиксели
    QRect createRandomRectangle() const;//Создать случайный прямоугольник
};

#endif // MAINWINDOW_H
