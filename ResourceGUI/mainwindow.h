#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtCore>
#include <QtGui>

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

    QTreeWidgetItem* addRoot(QString name);
    void addChild(QTreeWidgetItem* parent, QString name);
    void writeFile(QString filename);
    void readFile(QString filename);

private slots:
    void on_drawButton_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
