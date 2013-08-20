#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

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
private:
    Ui::MainWindow *ui;
    void paintEvent(QPaintEvent *);
};

#endif // MAINWINDOW_H
