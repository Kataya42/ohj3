#include <QApplication>
#include <QtWidgets>
#include <QMainWindow>
#include <QGraphicsScene>
#include <actors/nysse.hh>
#include <manse.h>
#include <actors/passenger.hh>
#include <actors/stop.hh>
#include <core/location.hh>
#include <core/logic.hh>
#include <errors/gameerror.hh>
#include <errors/initerror.hh>
#include <graphics/simpleactoritem.hh>
#include <graphics/simplemainwindow.hh>
#include <interfaces/iactor.hh>
#include <interfaces/icity.hh>
#include <interfaces/ipassenger.hh>
#include <interfaces/istatistics.hh>
#include <interfaces/istop.hh>
#include <interfaces/ivehicle.hh>
#include <creategame.hh>
#include <doxygeninfo.hh>
#include <offlinereader.hh>
#include <gamewindow.hh>
#include <chaser.h>
#include <statistics.hh>



int main(int argc, char *argv[])
{
    std::shared_ptr<Manse> map = nullptr;
    map = std::make_shared<Manse>();
    QApplication a(argc, argv);

    std::shared_ptr<GameWindow> w = nullptr;
    w = std::make_shared<GameWindow>();



    QImage kartta;
    Q_INIT_RESOURCE(offlinedata);
    Q_INIT_RESOURCE(ownGraphics);
    kartta.load(":/images/images/omaKartta.png");

    Statistics stats;

    w->setPicture(kartta);
    w->takeCity(map);
    w->takeStats(stats);

    w->show();

    CourseSide::Logic gamelogic;
    gamelogic.fileConfig();
    gamelogic.setTime(12,10);
    gamelogic.takeCity(map);
    gamelogic.finalizeGameStart();

    std::shared_ptr<Player> pelaaja = nullptr;
    pelaaja = std::make_shared<Player>();
    Interface::Location start;
    start.setXY(480, 316);
    pelaaja->move(start);
    map->addPlayer(pelaaja);

    std::shared_ptr<Chaser> enemy = nullptr;
    enemy = std::make_shared<Chaser>();
    Interface::Location enemyStart;
    enemyStart.setXY(500, 500);
    enemy->move(enemyStart);
    map->addEnemy(enemy);

    w->drawBuses();
    w->drawStops();


    return a.exec();
    

}
