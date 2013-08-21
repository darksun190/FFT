#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "sp_xmlread.h"
#include <QFile>
#include <QVector>
#include <cmath>
#include <QPainter>
#include <QStringListModel>
#include <QListView>
#include <QTime>
#include <algorithm>

#include <iostream>
#include <cstdlib>
#include <vectormath.h>
#include <fft.h>
#include <sp_xmlread.h>
#include <cmath>
#include <qmath.h>
#include <math.h>

using namespace splab;
using namespace std;

const double deg_one = PI/180;
typedef double  Type;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    freopen("./output.txt","w",stdout);
    Sp_xmlread xml_info("../tmp/");

    //only deal first element in the special program

    //detect the type of the element
    QString geo_type = xml_info.names.at(0).type;

    //check if there's a circle or  cylinder
    if (geo_type == QString("Circle") or geo_type == QString("Cylinder"))
    {
        int size = xml_info.names.at(0).points.size();
        //check the direction
        if (size<9)
        {
            //means points was too less;
        }
        int direction;  //  -1 = clockwise  ;   1 = anticlockwise
        double angle1,angle3,angle5;
        angle1 = std::atan2(xml_info.names.at(0).points.at(0).y,xml_info.names.at(0).points.at(0).x);
        angle3 = std::atan2(xml_info.names.at(0).points.at(4).y,xml_info.names.at(0).points.at(4).x);
        angle5 = std::atan2(xml_info.names.at(0).points.at(8).y,xml_info.names.at(0).points.at(8).x);
        if ((angle5-angle3)*(angle3-angle1)<0)
        {
            //means the start point nearby the 180-degree
            double abs13,abs35;
            abs13 = std::fabs(angle1-angle3);
            abs35 = std::fabs(angle3-angle5);
            if (abs13<abs35)
            {
                direction = angle3>angle1? 1:-1;
            }
            else
            {
                direction = angle5>angle3? 1:-1;
            }
        }
        else
        {
            //normal situation
            direction = angle3>angle1? 1:-1;
        }

        //caculate the angle of the element
        double round_nr=0;
        double next_angle,last_angle;
        last_angle = angle1;
        for (int i=1;i<size;++i)
        {
            next_angle = std::atan2(xml_info.names.at(0).points.at(i).y,xml_info.names.at(0).points.at(i).x);
            if (std::fabs(next_angle-last_angle)>PI)
            {
                if (direction == 1)
                {
                    round_nr += next_angle+2*PI-last_angle;
                }
                else
                {
                    round_nr += next_angle-2*PI-last_angle;
                }
            }
            else
            {
                round_nr += next_angle-last_angle;
            }
            //qDebug()<<last_angle<<"\t"<<next_angle<<"\t"<<next_angle-last_angle<<"\t"<<round_nr;
            last_angle = next_angle;

        }
        double radius = xml_info.names.at(0).radius;
        {
            //calculating the FFT
            Vector< complex<double> >  Rk;
            Vector<double> rn;
            // get the origin data from special program
            rn.resize(size);
            for(int i=0;i<size;++i)
            {
                rn[i] = hypot(xml_info.names.at(0).points.at(i).y,xml_info.names.at(0).points.at(i).x)-radius;

            }
            Rk = fftr2c( rn );

            for (int j=0;j<200;j++)
            {
                zmfft.push_back(sqrt(pow(Rk[j].real(),2)+pow(Rk[j].imag(),2))/size);
            }

            double a;
            QStringList list2;

            foreach (a,zmfft)
            {
                list2.push_back(QString::number(a));
            }

            model2 = new QStringListModel();
            model2->setStringList(list2);
            ui->tableView_2->setModel(model2);
            //qDebug()<<"point number is: " << size;
        }
        qDebug()<<angle1<<"\t"<<round_nr/2/PI;
    }

    /*
     // here is the code for the old lib;
    QString fileName = xml_info.names.at(0).Identifier;

    QFile nom_file(QString("../tmp/%1_NomPoints.txt").arg(fileName));
    QFile act_file(QString("../tmp/%1_Points.txt").arg(fileName));


    nom_file.open(QFile::ReadOnly|QFile::Text);
    act_file.open(QFile::ReadOnly|QFile::Text);

    QTextStream in_nom(&nom_file);
    QTextStream in_act(&act_file);

    QTextStream out(stdout);

    QVector <double> real_n;
    int size;
    while(1)
    {
        QString buf2;
        buf2 = in_act.readLine();
        if(buf2.isNull())
            break;
        QStringList temp = buf2.split(" ");
        double tx,ty;
        tx = temp[0].toDouble();
        ty = temp[1].toDouble();
        real_n.push_back(sqrt(tx*tx+ty*ty));
    }
     size = real_n.size();
    int channel;
    double re,im;

    double divi = double(size) / (2 * 3.14159265359);
    QTime t;
    t.start();
    for (channel = 0;channel<127;channel++)
    {
        for(int i = 0;i<size;++i)
        {
            re +=  (sin((i / divi) * channel) * real_n[i]) / size; // 'realanteil integration
            im +=  (cos((i / divi) * channel) * real_n[i]) / size; // 'imaginäranteil
        }
        yfft.push_back(sqrt((re * re) + (im * im))); //            'Amplitude berechen
        re = 0;
        im = 0;
        //qDebug()<<channel<<"\t"<<yfft[channel]*2000;
    }
    //qDebug("Time elapsed: %d ms", t.elapsed());
    {
     Vector< complex<Type> >  sn,Sk,Rk;
     Vector<Type> rn, tn;
     // get the origin data from special program
     sn.resize(size);
     rn.resize(size);
     int i=0;
     for(i=0;i<size;++i)
     {
         rn[i] = real_n[i];
      }
    t.restart();
     Rk = fftr2c( rn );
      //qDebug("Time elapsed: %d ms", t.elapsed());

      for (int j=0;j<i;j++)
      {
          //qDebug()<<Rk[j].real()<<"\t"<<Rk[j].imag()<<"----\t----"<<sqrt(pow(Rk[j].real(),2)+pow(Rk[j].imag(),2));

          //qDebug()<<sn[i].real()<<"\t"<<sn[i].imag();
          //qDebug()<<Sk[j].real()<<"\t"<<Sk[j].imag();
          zmfft.push_back(sqrt(pow(Rk[j].real(),2)+pow(Rk[j].imag(),2))/size);

      }

    }
    double a;
    QStringList list,list2;
    foreach (a,yfft)
    {
        list.push_back(QString::number(a*2000));
    }
    foreach (a,zmfft)
    {
        list2.push_back(QString::number(a*3));
    }
    model = new QStringListModel();
    model->setStringList(list);
    ui->tableView->setModel(model);
    model2 = new QStringListModel();
    model2->setStringList(list2);
    ui->tableView_2->setModel(model2);
    //qDebug()<<"point number is: " << size;
    for(int i=0;i<127;++i)
    {
        ;
        //qDebug()<<QString("%1\t%2\t%3\t%4").arg(i).arg(zmfft[i]).arg(yfft[i]).arg(zmfft[i]/yfft[i]);
    }
    */
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
/*    //painter.scale(3,2);
    painter.drawLine(0,0,400,0);
    painter.drawLine(0,0,100,0);
    painter.drawText(0,10,QString("0"));
    painter.drawText(384,10,QString("128"));

    for(int i=1;i<128;++i)
    {
        painter.drawLine(i*3,0,i*3,-yfft[i]*100000);
    }
*/
    painter.translate(0,300);
    painter.drawLine(0,0,400,0);
    painter.drawLine(0,0,100,0);
    painter.drawText(0,10,QString("0"));
    painter.drawText(384,10,QString("128"));

    for(int i=1;i<128;++i)
    {
        painter.drawLine(i*3,0,i*3,-zmfft[i]*10000);
    }

}
