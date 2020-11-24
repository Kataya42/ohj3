#include "gamewindow.hh"
#include "ui_simplegamewindow.h"
#include <QDebug>
#include <QKeyEvent>

const int PADDING = 10;


GameWindow::GameWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::SimpleGameWindow)
{
    ui->setupUi(this);
    ui->gameView->setFixedSize(width_, height_);
    ui->centralwidget->setFixedSize(width_ + ui->startButton->width() + PADDING, height_ + PADDING);

    ui->startButton->move(width_ + PADDING , PADDING);

    map = new QGraphicsScene(this);
    ui->gameView->setScene(map);
    map->setSceneRect(0,0,width_,height_);

    resize(minimumSizeHint());
    //ui->gameView->fitInView(0,0, MAPWIDTH, MAPHEIGHT, Qt::KeepAspectRatio);

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, map, &QGraphicsScene::advance);
    timer->start(tick_);
}

GameWindow::~GameWindow()
{
    delete ui;
}

void GameWindow::setSize(int w, int h)
{
    width_ = w;
    height_ = h;
}

void GameWindow::setTick(int t)
{
    tick_ = t;
}

void GameWindow::addActor(int locX, int locY, int type)
{
    OwnActorItem* nActor = new OwnActorItem(locX, locY, type);
    actors_.push_back(nActor);
    map->addItem(nActor);
    last_ = nActor;
}

void GameWindow::updateCoords()
{
    std::vector<std::shared_ptr<Interface::IActor>> buses = city_->getBuses();
    int counter = 0;
    for ( auto i : buses){
        if (counter < actors_.size()){
            int nx = i->giveLocation().giveX() + 350;
            int ny = 450 - i->giveLocation().giveY();
            actors_.at(counter)->setCoord(nx, ny);
            counter++;
        }
    }
}

void GameWindow::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == 16777235) {
        emit moveUp;
    } else if (event->key() == 16777237) {
        emit moveDown;
    } else if (event->key() == 16777234) {
        emit moveLeft
    } else if (event->key() == 16777236) {
        emit moveRight
    }
}

void GameWindow::setPicture(QImage &img)
{
    map->setBackgroundBrush(img);
}

bool GameWindow::takeCity(std::shared_ptr<Manse> city)
{
    city_ = city;
}

void GameWindow::drawBuses()
{
    std::vector<std::shared_ptr<Interface::IActor>> buses = city_->getBuses();
    for ( auto i : buses){
        int x = i->giveLocation().giveX() + 350;
        int y = 450 - i->giveLocation().giveY();

        addActor(x,y,1000);
        std::cout << "x:" << x << " y:" << y << std::endl;
    }
}


void GameWindow::on_startButton_clicked()
{
    qDebug() << "Start clicked";
    connect(timer, SIGNAL(timeout()), this, SLOT(updateCoords()));
    emit gameStarted();
    grabKeyboard();

}
