#include "note_generator.h"

#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <fstream>
#include <string>
using std::ws;


Note_generator::Note_generator(QObject *parent) : QObject(parent)
{

}

void Note_generator::setScene(QGraphicsScene* scene)
{
    this->scene = scene;
}

void Note_generator::init(QString beatmap_path)
{
    beatmap_path = "F:/testing/1.orz";

    std::ifstream beatmap(beatmap_path.toStdString());

    std::string temp;
    beatmap >> temp;  // ignore title
    beatmap >> temp;  // ignore level

    beatmap >> num_of_notes;

    for(int i = 0; i < num_of_notes ; ++i){
        beatmap >> temp;
        qDebug() << temp.c_str(); // start time
        beatmap >> temp;
        qDebug() << temp.c_str(); // note type
        beatmap >> temp;
        qDebug() << temp.c_str(); // speed
    }

//    QFile inputFile(beatmap_path);

//    if (inputFile.open(QIODevice::ReadOnly))
//    {
//        QTextStream in(&inputFile);
//        while (!in.atEnd())
//        {
//            QString line = in.readLine();
//            qDebug() << line;
//        }
//        inputFile.close();
//    }
}

void Note_generator::start()
{

}
