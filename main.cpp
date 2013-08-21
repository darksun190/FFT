#include "mainwindow.h"
#include <QApplication>
#include <QCoreApplication>
/*****************************************************************************
 *                               fftmr_test.cpp
 *
 * Mixed Radix Algorithm FFT testing.
 *
 * Zhang Ming, 2010-04, Xi'an Jiaotong University.
 *****************************************************************************/
#include <QDir>




int main(int argc, char *argv[])
{

    QApplication a(argc, argv);
    QDir::setCurrent(QCoreApplication::applicationDirPath());
    MainWindow w;
    w.show();
   /*
    Vector< complex<Type> >  sn,Sk,Rk;
    Vector<Type> rn, tn;
    // get the origin data from special program
    sn.resize(50);
    rn.resize(50);
    int i=0;
    while(1)
    {
        QString buf2;
        buf2 = in_act.readLine();
        if(buf2.isNull())
            break;
        QStringList temp = buf2.split(" ");
        sn[i] = complex<Type>( temp[1].toDouble(), temp[2].toDouble());
        rn[i] = sqrt(pow(temp[1].toDouble(),2)+pow(temp[2].toDouble(),2));
        qDebug()<<sn[i].real()<<"\t"<<sn[i].imag()<<'\t'<<rn[i];
        i++;
    }
    //sn.resize(i);
     Sk = fftc2c( sn );
     Rk = fftr2c( rn );
     for (int j=0;j<i;j++)
     {
         //qDebug()<<sn[i].real()<<"\t"<<sn[i].imag();
         //qDebug()<<Sk[j].real()<<"\t"<<Sk[j].imag();
         qDebug()<<Rk[j].real()<<"\t"<<Rk[j].imag()<<"----\t----"<<sqrt(pow(Rk[j].real(),2)+pow(Rk[j].imag(),2));

     }
     qDebug()<<Sk.size();
*/
/*
    Vector< complex<Type> >  sn, Rk, Sk, xn;
    Vector<Type> rn, tn;

    cout << "forward transform: complex to complex." << endl;
    cout << "inverse transform: complex to complex." << endl << endl;
    cout << "signal length" << "\t" << "mean(abs((sn-xn))" << endl;
    for( int len=MINLEN; len<MAXLEN; len+=STEP )
    {
        sn.resize(len);
        for( int i=0; i<len; ++i )
            sn[i] = complex<Type>( rand()%10, rand()%10 );

        Sk = fftc2c( sn );
        xn = ifftc2c( Sk );
//        Sk = fft( sn );
//        xn = ifft( Sk );
        cout << "    " << len << "\t\t" << "  " << sum(abs(sn-xn))/len << endl;
    }
    cout << endl << endl;

    cout << "forward transform: real to complex ." << endl;
    cout << "inverse transform: complex to real." << endl << endl;
    cout << "signal length" << "\t" << "mean(abs((rn-tn))" << endl;
    for( int len=MINLEN; len<MAXLEN; len+=STEP )
    {
        rn.resize(len);
        for( int i=0; i<len; ++i )
            rn[i] = rand()%10;

        Rk = fftr2c( rn );
        tn = ifftc2r( Rk );
//        Rk = fft( rn );
//        tn = real( ifft(Rk) );
        cout << "    " << len << "\t\t" << "  " << sum(abs(rn-tn))/len << endl;
    }
    cout << endl;
*/
    return a.exec();
}

