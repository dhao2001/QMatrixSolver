#include "qmatrix_solver_mainwindow.h"

#include <QApplication>
#include <QTranslator>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QString transPath = qApp->applicationDirPath() + QString("/qmatrix_solver_zh_CN.qm");
    QTranslator *translator = new QTranslator();
    translator->load(transPath);
    qApp->installTranslator(translator);

    QMatrix_Solver_MainWindow w;
    w.show();
    return a.exec();
}
