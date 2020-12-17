#include "matrixwidget.h"
#include "qmatrix_solver_mainwindow.h"

MatrixWidget::MatrixWidget(QWidget *parent) : QWidget(parent)
{

}

MatrixWidget::MatrixWidget(int dimension, QWidget *parent) : QWidget(parent)
{
    this->parent = parent;
    QString solveBtnTxt = tr("Solve!");
    QStringList unknownXLabels;
    for (int i = 1; i <= dimension; i++)
    {
        unknownXLabels << QString("x%1").arg(i);
    }
    unknownXLabels << "b";
    QStringList solveHeaders;
    solveHeaders << "Solves";

    A.resize(dimension, dimension);
    b.resize(dimension);
    table = new QTableWidget(dimension, dimension + 1);
    solveBtn = new QPushButton(solveBtnTxt);
    solveTable = new QTableWidget(dimension, 1);
    tablesLayout = new QHBoxLayout();
    mainLayout = new QVBoxLayout();
    solveTable->hide();
    connect(solveBtn, SIGNAL(clicked()), this, SLOT(on_solveBtn_clicked()));

    tablesLayout->addWidget(table);
    tablesLayout->addWidget(solveTable);
    tablesLayout->setStretchFactor(table, 4);
    tablesLayout->setStretchFactor(solveTable, 1);

    table->setHorizontalHeaderLabels(unknownXLabels);
    solveTable->setVerticalHeaderLabels(unknownXLabels);
    solveTable->setHorizontalHeaderLabels(solveHeaders);
    solveTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    mainLayout->addLayout(tablesLayout);
    mainLayout->addWidget(solveBtn);
    setLayout(mainLayout);
}

void MatrixWidget::setPointer()
{
    for (int i = 0; i < this->layout()->count(); i++)
    {
        if (this->layout()->itemAt(i)->widget()->inherits("QPushButton"))
        {
            solveBtn = (QPushButton*)this->layout()->itemAt(i)->widget();
        }
        else if (this->layout()->itemAt(i)->widget()->inherits("QTableWidget"))
        {
            table = (QTableWidget*)this->layout()->itemAt(i)->widget();
        }
    }
}

bool MatrixWidget::setMatrix()
{
    if (solveBtn == nullptr || table == nullptr)
    {
        setPointer();
    }

    for (int i = 0; i < table->rowCount(); i++)
    {
        for (int j = 0; j < table->columnCount() - 1; j++)
        {
            QTableWidgetItem *item = table->item(i, j);
            if (item == nullptr)
            {
                QString info = QString(tr("Empty Cell at [%1,%2]")).arg(i).arg(j);
                QMessageBox::critical(this, tr("Empty Cell Error"), info);
                return false;
            }
            QString str = item->text();
            bool isConvertOK = false;
            double num = str.toDouble(&isConvertOK);
            if (isConvertOK)
            {
                A(i, j) = num;
            }
            else
            {
                QString info = QString(tr("Invaild Number %1 at Matrix [%2, %3]")).arg(str).arg(i).arg(j);
                QMessageBox::critical(this, tr("Error Numeral"), info);
                return false;
            }
        }
        QTableWidgetItem *item = table->item(i, table->columnCount() - 1);
        QString str = item->text();
        bool isConvertOK = false;
        double num = str.toDouble(&isConvertOK);
        if (isConvertOK)
        {
            b(i) = num;
        }
        else
        {
            QString info = QString(tr("Invaild Number %1 at Matrix [%2, %3]")).arg(str).arg(i).arg(table->columnCount() - 1);
            QMessageBox::critical(this, tr("Error Numeral"), info);
            return false;
        }
    }
    return true;
}

void MatrixWidget::setTable()
{
    if (solveBtn == nullptr || table == nullptr)
    {
        setPointer();
    }

    for (int i = 0; i < A.rows(); i++)
    {
        for (int j = 0; j < A.cols(); j++)
        {
            QTableWidgetItem *item = new QTableWidgetItem(QString::number(A(i, j), 8, 10));
            table->setItem(i, j, item);
        }
        QTableWidgetItem *item = new QTableWidgetItem(QString::number(b(i), 8, 10));
        table->setItem(i, A.cols(), item);
    }
}

bool MatrixWidget::solve()
{
    if (!rankAequalAb())
    {
        QString info = QString(tr("We found that Rank(A) NOT equals to Rank(A,b). There is no solve for this matrix."));
        QMessageBox::critical(this, tr("No Solve Error"), info);
        return false;
    }
    if (A.fullPivLu().rank() < A.rows())
    {
        QString info = QString(tr("Your matrix seems to have more than one solve.\nIf you continue operation, this program will give a solve.\nBut that's not the only one solve."));
        QMessageBox::StandardButton reply;
        reply = QMessageBox::warning(this, tr("No only one solve"),  info, QMessageBox::Yes | QMessageBox::Abort, QMessageBox::Abort);
        if (reply == QMessageBox::Abort)
        {
            return false;
        }
    }

    sl = A.fullPivLu().solve(b);
    double relative_error = (A*sl - b).norm() / b.norm();
    QString error_info = QString(tr("Relative Error: %1")).arg(relative_error);
    QMatrix_Solver_MainWindow *mainwin = (QMatrix_Solver_MainWindow*)parent;
    mainwin->statusBar()->showMessage(error_info);
//    qDebug() << "Error: " << relative_error;
    return true;
}

void MatrixWidget::showSolve()
{
    if (solveBtn == nullptr || table == nullptr)
    {
        setPointer();
    }

    for (int i = 0; i < sl.rows(); i++)
    {
        QTableWidgetItem *item = new QTableWidgetItem(QString::number(sl(i), 10, 16));
        solveTable->setItem(i, 0, item);
    }

    solveTable->show();
}

void MatrixWidget::on_solveBtn_clicked()
{
    if (setMatrix() && solve())
    {
        showSolve();
    }

}

bool MatrixWidget::rankAequalAb()
{
    using namespace Eigen;
    MatrixXd Ab;
    Ab.resize(A.rows(), A.rows()+1);
    for (int i = 0; i < A.rows(); i++)
    {
        for (int j = 0; j < A.cols(); j++)
        {
            Ab(i, j) = A(i, j);
        }
        Ab(i, A.cols()) = b(i);
    }

    return A.fullPivLu().rank() == Ab.fullPivLu().rank();
}
