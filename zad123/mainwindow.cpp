#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "cmath"
#include "iostream"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Создаем сцену и вид
    QGraphicsScene* scene = new QGraphicsScene();
    QGraphicsView* view = new QGraphicsView(scene, this);
    view->setRenderHint(QPainter::Antialiasing);
    view->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
    view->setViewportUpdateMode(QGraphicsView::SmartViewportUpdate);
    view->setRenderHint(QPainter::Antialiasing, true);
    view->setRenderHint(QPainter::SmoothPixmapTransform, true);
    setCentralWidget(view);

    // Устанавливаем размеры карты
    int mapWidth = 1400;
    int mapHeight = 700;

    // Создаем массив стен на карте
    std::vector<std::vector<bool>> walls(mapWidth, std::vector<bool>(mapHeight, false));
    for(int x = 0; x < mapWidth; x++){
        walls[x][0] = true;
        walls[x][mapHeight-1] = true;
    }
    for(int y = 0; y < mapHeight; y++){
        walls[0][y] = true;
        walls[mapWidth-1][y] = true;
    }
    for(int x = 98; x < 1302; x++){
        walls[x][100] = true;
        walls[x][600] = true;
    }
    for(int y = 100; y < 600; y++){
        walls[98][y] = true;
        walls[1302][y] = true;
    }
    // Создаем массив силы сигнала от каждой базовой станции до каждой точки на карте
    std::vector<std::vector<float>> signalStrengths(mapWidth, std::vector<float>(mapHeight, 0.0));
    for(int i = 0; i < 12; i++){
        int cellPosX, cellPosY;
        if(i < 2) {
            if(i == 0){
                cellPosX = 200;
                cellPosY = 250;
            }
            else if(i == 1){
                cellPosX = 400;
                cellPosY = 250;
            }
        }
        else if(i < 4) {
            if(i == 2){
                cellPosX = 200;
                cellPosY = mapHeight - 250;
            }
            else if(i == 3){
                cellPosX = 400;
                cellPosY = mapHeight - 250;
            }
        }
        else if(i < 6) {
            if(i == 4){
                cellPosX = 600;
                cellPosY = 250;
            }
            else if(i == 5){
                cellPosX = 800;
                cellPosY = 250;
            }
        }
        else if(i < 8) {
            if(i == 6){
                cellPosX = 600;
                cellPosY = mapHeight - 250;
            }
            else if(i == 7){
                cellPosX = 800;
                cellPosY = mapHeight - 250;
            }
        }
        else if(i < 10){
            if(i == 8){
                cellPosX = 1000;
                cellPosY = 250;
            }
            else if(i == 9){
                cellPosX = 1200;
                cellPosY = 250;
            }
        }
        else {
            if(i == 10){
                cellPosX = 1000;
                cellPosY = mapHeight - 250;
            }
            else if(i == 11){
                cellPosX = 1200;
                cellPosY = mapHeight - 250;
            }
        }
        // Далее идет ваш код для обработки текущей базовой станции

        for(int x = 0; x < mapWidth; x++){
            for(int y = 0; y < mapHeight; y++){
                // Вычисляем расстояние между текущей базовой станцией и текущей точкой на карте
                float distance = sqrt(pow(cellPosX-x, 2) + pow(cellPosY-y, 2));
                // Вычисляем силу сигнала на основе расстояния
                float signalStrength = 1.0/(1.0 + pow(distance/50.0, 2));
                // Добавляем силу сигнала к массиву для текущей точки на карте
                signalStrengths[x][y] += signalStrength;
            }
        }
    }


    // Определяем цвет и прозрачность каждой точки на карте на основе массива силы сигнала
    for(int x = 0; x < mapWidth; x++){
        for(int y = 0; y < mapHeight; y++){
            float signalStrength = signalStrengths[x][y];
            // Определяем цвет на основе силы сигнала
            QColor color = QColor::fromHsvF(0.6 - signalStrength*0.6, 1.0, 1.0);
            // Определяем прозрачность на основе силы сигнала
            int alpha = std::min(75, static_cast<int>(signalStrength*255));
            // Создаем прямоугольник для текущей точки на карте
            QGraphicsRectItem* rect = new QGraphicsRectItem(x, y, 1, 1);
            // Устанавливаем цвет и прозрачность прямоугольника
            rect->setBrush(QBrush(color));
            rect->setOpacity(alpha/255.0);
            // Добавляем прямоугольник на сцену
            scene->addItem(rect);
        }
    }

    // Создаем стены на сцене
    for(int x = 0; x < mapWidth; x++){
        for(int y = 0; y < mapHeight; y++){
            if(walls[x][y]){
                QGraphicsRectItem* wall = new QGraphicsRectItem(x, y, 1, 1);
                wall->setBrush(QBrush(Qt::black));
                scene->addItem(wall);
            }
        }
    }

    //Создаем базовые станции на сцене


    //Устанавливаем размеры и показываем вид
    view->setSceneRect(0, 0, mapWidth, mapHeight);
    view->show();
}

MainWindow::~MainWindow()
{
    delete ui;
}
