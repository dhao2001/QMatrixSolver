#ifndef MATRIXWIDGET_H
#define MATRIXWIDGET_H

#include <QWidget>
#include <QTableWidget>
#include <QPushButton>
#include <QLayout>
#include <QHeaderView>
#include <QMessageBox>
#include <QStatusBar>
#include <QLabel>
#include <eigen3/Eigen/Dense>
#include <qdebug.h>

class MatrixWidget : public QWidget
{
        Q_OBJECT
    public:
        explicit MatrixWidget(QWidget *parent = nullptr);
        explicit MatrixWidget(int, QWidget *parent = nullptr);
        bool setMatrix();
        void setTable();
        void showSolve();
        bool solve();
        bool rankAequalAb();
        void fillZero();

    public slots:
        void on_solveBtn_clicked();

    signals:

    private:
        QWidget* parent;
        Eigen::MatrixXd A;
        Eigen::VectorXd b;
        Eigen::VectorXd sl;

        QPushButton *solveBtn = nullptr;
        QTableWidget *table = nullptr;
        QTableWidget *solveTable = nullptr;
        QHBoxLayout *tablesLayout = nullptr;
        QVBoxLayout *mainLayout = nullptr;

        void setPointer();

};

#endif // MATRIXWIDGET_H
