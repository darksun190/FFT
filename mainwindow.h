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
        QVector<double> yfft;
        QVector<double> zmfft;
        QStringListModel *model;
        QStringListModel *model2;
private:
    Ui::MainWindow *ui;
    void paintEvent(QPaintEvent *);
};

#endif // MAINWINDOW_H
