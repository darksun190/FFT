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
#include <matrix.h>
#include <matrixmath.h>

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
        QVector<point> point_trans; //save the data after alignment transform
        double radius = xml_info.names.at(0).radius;
        double lead;         //actual lead for the cylinder
        QVector<double> angle_commu;
        {
            /* transform the coordinate of the data.
             * special program offer the zero position of the circle or cylinder
             * use it as new 0 zero point,
             * O = (x0,y0,z0)
             * special program offer the vector of the cylinder's axis, use it as Z axis
             * the formula is
             * if the vector is (l,m,n)
             * the matrix Mz is
             *                  Keep the X axis in XOY plane
             * |    - m / sqrt ( 1-n^2 )    ,   l / sqrt ( 1-n^2 )      ,   0               |
             * |    - n*l / sqrt ( 1-n^2 )  ,   - m*n / sqrt ( 1-n^2 )  ,   sqrt ( 1-n^2 )  |
             * |            l               ,           m               ,       n           |
             *
             *                  Keep the X axis in XOZ plane
             * |     n / sqrt ( 1-m^2 )     ,           0           ,  - l / sqrt( 1-m^2 )  |
             * |    - n*l / sqrt ( 1-m^2 )  ,   sqrt ( 1-m^2 )      ,  - m*n / sqrt ( 1-m^2)|
             * |            l               ,           m           ,           n           |
             *
             *
             * the new points' coordinate P' = Mz * (P-O)
             */
            //get the new Zero point
            Vector <double> zero_point(3);
            zero_point(1) = xml_info.names.at(0).p.x;
            zero_point(2) = xml_info.names.at(0).p.y;
            zero_point(3) = xml_info.names.at(0).p.z;
            qDebug()<<zero_point(1)<<zero_point(2)<<zero_point(3);
            Matrix <double> Mz;
            {
                //calculate the matrix Mz
                double l,m,n;
                l = xml_info.names.at(0).v.u;
                m = xml_info.names.at(0).v.v;
                n = xml_info.names.at(0).v.w;
                Mz.resize(3,3);
                Mz(1,1) = n / sqrt ( 1-m*m );
                Mz(1,2) = 0;
                Mz(1,3) = -l / sqrt ( 1-m*m );
                Mz(2,1) = - n*l / sqrt ( 1-m*m );
                Mz(2,2) = sqrt ( 1-m*m );
                Mz(2,3) = - m*n / sqrt ( 1-m*m );
                Mz(3,1) = l;
                Mz(3,2) = m;
                Mz(3,3) = n;
            }
            //start transform
            {
                point tmp;
                foreach (tmp, xml_info.names.at(0).points)
                {
                    Vector<double> o(3);
                    o(1) = tmp.x;
                    o(2) = tmp.y;
                    o(3) = tmp.z;
                    Vector<double> t;
                    t = Mz * (o-zero_point);
                    point buf_point;
                    buf_point.x = t(1);
                    buf_point.y = t(2);
                    buf_point.z = t(3);
                    point_trans.push_back(buf_point);
                    //qDebug()<<o(1)<<o(2)<<o(3)<<"\t"<<t(1)<<t(2)<<t(3);
                }
            }
        }
        //check the direction
        if (size<9)
        {
            //means points was too less;
        }
        int direction;  //  -1 = clockwise  ;   1 = anticlockwise
        double angle1,angle3,angle5;
        angle1 = std::atan2(point_trans.at(0).y,point_trans.at(0).x);
        angle3 = std::atan2(point_trans.at(4).y,point_trans.at(4).x);
        angle5 = std::atan2(point_trans.at(8).y,point_trans.at(8).x);
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
        angle_commu.push_back(round_nr);
        double next_angle,last_angle;
        last_angle = angle1;
        for (int i=1;i<size;++i)
        {
            next_angle = std::atan2(point_trans.at(i).y,point_trans.at(i).x);
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
            angle_commu.push_back(round_nr);

        }

        //calculate the lead
        lead = (point_trans.last().z-point_trans.first().z) / (round_nr/2/PI);
        {
            //calculating the axial FFT
            Vector< complex<double> >Rk;
            Vector<double> rn;
            // get the origin data from special program
            rn.resize(size);
            for(int i=0;i<size;++i)
            {
                rn[i] = point_trans.at(i).z - lead * (angle_commu.at(i)/2/PI);
                //rn[i] = hypot(point_trans.at(i).y,point_trans.at(i).x)-radius;
            }
            Rk = fftr2c( rn );

            for (int j=0;j<300;j++)
            {
                fft_axial.push_back(sqrt(pow(Rk[j].real(),2)+pow(Rk[j].imag(),2))/size);
            }

            double a;
            list2.clear();
            foreach (a,fft_axial)
            {
                list2.push_back(QString::number(a*1000));
            }

            model2 = new QStringListModel();
            model2->setStringList(list2);
            ui->tableView_2->setModel(model2);
            QVector<double>::Iterator ib=fft_axial.begin();
            ib++;

            scale_fft_axial_y = 250 / (*max_element(ib,fft_axial.end()));
        }
        {
            //calculating the radial FFT
            Vector< complex<double> >  Rk;
            Vector<double> rn;
            // get the origin data from special program
            rn.resize(size);
            for(int i=0;i<size;++i)
            {
                rn[i] = hypot(point_trans.at(i).y,point_trans.at(i).x)-radius;

            }
            Rk = fftr2c( rn );

            for (int j=0;j<300;j++)
            {
                fft_radial.push_back(sqrt(pow(Rk[j].real(),2)+pow(Rk[j].imag(),2))/size);
            }

            double a;
            list1.clear();
            foreach (a,fft_radial)
            {
                list1.push_back(QString::number(a*1000));
            }

            model1 = new QStringListModel();
            model1->setStringList(list1);
            ui->tableView->setModel(model1);
            QVector<double>::Iterator ib=fft_radial.begin();
            ib++;
            scale_fft_radial_y = 250 / (*max_element(ib,fft_radial.end()));
            //qDebug()<<"point number is: " << size;
        }
        qDebug()<<"The real Lead is "<<"\t"<<lead;
        qDebug()<<scale_fft_radial_y;
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

    //painter for radial
    // the drawing area is 576*250
    painter.translate(100,300);
       //painter.scale(3,2);
    painter.drawLine(0,0,400,0);
    painter.drawLine(0,0,100,0);
    painter.drawText(0,10,QString("0"));
    painter.drawText(384,10,QString("128"));

    for(int i=1;i<300;++i)
    {
        painter.drawLine(i,0,i,-fft_radial[i]*scale_fft_radial_y);
    }

    //painter for axial
    // the drawing area is 576*250
    painter.translate(0,300);
    painter.drawLine(0,0,400,0);
    painter.drawLine(0,0,100,0);
    painter.drawText(0,10,QString("0"));
    painter.drawText(384,10,QString("128"));

    for(int i=1;i<300;++i)
    {
        painter.drawLine(i,0,i,-fft_axial[i]*scale_fft_axial_y);
    }

}
