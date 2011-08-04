#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtCore>
#include <QtGui>
#include <QFile>
#include <QString>
#include <QDebug>
#include <QTextStream>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //Set column width and label
    ui->treeWidget->setColumnCount(1);
    QStringList label;
    label.append("Resources");
    ui->treeWidget->setHeaderLabels(label);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::writeFile(QString filename)
{
//    QFile file(filename);

//    if(!file.open(QFile::WriteOnly | QFile::Text))
//    {
//        return;
//    }

//    QTextStream out(&file);
//    ui->listWidget->addItem("Hello");

//    file.close();
}

void MainWindow::readFile(QString filename)
{
    QFile file(filename);

    if(!file.open(QFile::ReadOnly | QFile::Text))
    {
        QMessageBox msgBox;
        msgBox.setText("File not found");
        msgBox.exec();

        return;
    }

    QTextStream in(&file);

    QString line;
    while((line = in.readLine()) != NULL)
    {
        QStringList wordList = line.split(" ");

        if(wordList.size()  < 1)
            continue;

        QTreeWidgetItem* root = addRoot(wordList.first());
        wordList.removeFirst();

        while(!wordList.isEmpty())
        {
            addChild(root, wordList.first());
            wordList.removeFirst();
        }
    }

    file.close();
}

QTreeWidgetItem* MainWindow::addRoot(QString name)
{
    QTreeWidgetItem* item = new QTreeWidgetItem(ui->treeWidget);
    item->setText(0, name);
    ui->treeWidget->addTopLevelItem(item);

    return item;
}

void MainWindow::addChild(QTreeWidgetItem* parent, QString name)
{
    QTreeWidgetItem* item = new QTreeWidgetItem();
    item->setText(0, name);
    parent->addChild(item);
}

void MainWindow::on_drawButton_clicked()
{
    (void)system("ResourceManager.exe");
}
