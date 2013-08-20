#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "sp_xmlread.h"
#include <QFile>
#include <QVector>
#include <cmath>
using namespace std;
#include <QPainter>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    freopen("./output.txt","w",stdout);
    Sp_xmlread xml_info("../tmp/");

    QString fileName = xml_info.names.at(0);

    QFile nom_file(QString("../tmp/%1_NomPoints.txt").arg(fileName));
    QFile act_file(QString("../tmp/%1_ActPoints.txt").arg(fileName));


    nom_file.open(QFile::ReadOnly|QFile::Text);
    act_file.open(QFile::ReadOnly|QFile::Text);

    QTextStream in_nom(&nom_file);
    QTextStream in_act(&act_file);

    QTextStream out(stdout);

    QVector <double> rn;
    while(1)
    {
        QString buf2;
        buf2 = in_act.readLine();
        if(buf2.isNull())
            break;
        QStringList temp = buf2.split(" ");
        double tx,ty;
        tx = temp[1].toDouble();
        ty = temp[2].toDouble();
        rn.push_back(sqrt(tx*tx+ty*ty));
    }
    int size = rn.size();
    int channel;
    double re,im;

    double divi = double(size) / (2 * 3.14159265359);
    for (channel = 0;channel<127;channel++)
    {
        for(int i = 0;i<size;++i)
        {
            re +=  (sin((i / divi) * channel) * rn[i]) / size; // 'realanteil integration
            im +=  (cos((i / divi) * channel) * rn[i]) / size; // 'imaginäranteil
        }
         yfft.push_back(sqrt((re * re) + (im * im))); //            'Amplitude berechen
         re = 0;
         im = 0;
         qDebug()<<channel<<"\t"<<yfft[channel];
    }

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.save();
    painter.setPen(Qt::SolidLine);

    painter.translate(100,200);
    //painter.scale(3,2);
    painter.drawLine(0,0,400,0);
    painter.drawLine(0,0,100,0);
    painter.drawText(0,10,QString("0"));
    painter.drawText(384,10,QString("128"));

    for(int i=0;i<128;++i)
    {
        painter.drawLine(i*3,0,i*3,-yfft[i]*3);
    }
}
