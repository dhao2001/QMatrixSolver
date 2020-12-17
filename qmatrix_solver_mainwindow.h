#ifndef QMATRIX_SOLVER_MAINWINDOW_H
#define QMATRIX_SOLVER_MAINWINDOW_H

#include <QMainWindow>
#include <QTabWidget>
#include <QTableWidget>
#include <QHeaderView>
#include <QStringLiteral>
#include <QInputDialog>
#include <qdebug.h>
#include <QVBoxLayout>
#include <QPushButton>
#include <QDialog>
#include <QFile>
#include <QDialogButtonBox>
#include <QPlainTextEdit>

#include "matrixwidget.h"

QT_BEGIN_NAMESPACE
namespace Ui { class QMatrix_Solver_MainWindow; }
QT_END_NAMESPACE

class QMatrix_Solver_MainWindow : public QMainWindow
{
        Q_OBJECT

    public:
        QMatrix_Solver_MainWindow(QWidget *parent = nullptr);
        ~QMatrix_Solver_MainWindow();

    private slots:
        void on_actionAbout_triggered();

        void on_actionNew_Matrix_triggered();

//        void on_solveBtn_clicked();

        void on_actionGetHelp_triggered();

    private:
        Ui::QMatrix_Solver_MainWindow *ui;

        QTabWidget *tabWidget;
};
#endif // QMATRIX_SOLVER_MAINWINDOW_H
