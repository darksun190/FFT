#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStringListModel>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
        QVector<double> fft_radial;
        QVector<double> fft_axial;
        QStringListModel *model1;
        QStringListModel *model2;

        QStringList list1;      // save the radial fft result for listmodel
        QStringList list2;      // save the axial fft result for listmodel

        double scale_fft_axial_y;
        double scale_fft_radial_y;
private:
    Ui::MainWindow *ui;
    void paintEvent(QPaintEvent *);
};

#endif // MAINWINDOW_H
