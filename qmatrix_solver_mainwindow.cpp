#include "qmatrix_solver_mainwindow.h"
#include "ui_qmatrix_solver_mainwindow.h"

QMatrix_Solver_MainWindow::QMatrix_Solver_MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::QMatrix_Solver_MainWindow)
{
    ui->setupUi(this);
    tabWidget = new QTabWidget();
//    translator = new QTranslator();
//    QString transPath = qApp->applicationDirPath() + QString("/qmatrix_solver_zh_CN.qm");

//    QFile *transFile = new QFile(QString(":/qmatrix_solver_zh_CN.qm"));
//    qDebug() << QFile(transPath).exists();

//    qApp->installTranslator(translator);
//    translator->load(transPath);

    QLabel *tip = new QLabel(tr("Get help by 'Help'-> 'Get Help' at any time."));
    tip->setAlignment(Qt::AlignLeft);
    ui->statusbar->addPermanentWidget(tip);
    ui->statusbar->showMessage(tr("Start Calculate by 'Matrix' -> 'New Matrix'."), 20000);
}

QMatrix_Solver_MainWindow::~QMatrix_Solver_MainWindow()
{
    delete ui;
}


void QMatrix_Solver_MainWindow::on_actionAbout_triggered()
{
    QString info = QString(tr("QMatrix Solver\nBy DHao at Central South University\n"));
    QMessageBox::information(this, tr("About QMatrix Solver"), info);
}

void QMatrix_Solver_MainWindow::on_actionNew_Matrix_triggered()
{
    int minDimension = 1;
    int maxDimension = 20;
    int defDimension = 2;
    int getDimensionStep = 1;

    QString getDimWinTitle = tr("New Matrix");
    QString getDimWinHelpTxt = tr("New Matrix's Dimension (1~20): ");

    bool getDimensionOK = false;
    int matDim = QInputDialog::getInt(this, getDimWinTitle, getDimWinHelpTxt, defDimension, minDimension, maxDimension, getDimensionStep, &getDimensionOK);
    if (!getDimensionOK)
    {
        return;
    }

    if (centralWidget() != this->tabWidget)
    {
        setCentralWidget(this->tabWidget);
    }

    QWidget *matWidget = new MatrixWidget(matDim, this);

    QString tabName = QString("New Matrix %1").arg(tabWidget->count()+1);
    tabWidget->addTab(matWidget, tabName);
    tabWidget->setCurrentWidget(matWidget);
}

void QMatrix_Solver_MainWindow::on_actionGetHelp_triggered()
{
    QDialog *dialog = new QDialog(this);
    QDialogButtonBox *btnBox = new QDialogButtonBox(dialog);
    QHBoxLayout *dlLayout = new QHBoxLayout(dialog);
    QPlainTextEdit *txtEdit = new QPlainTextEdit(dialog);

    txtEdit->setReadOnly(true);
    txtEdit->setPlainText(tr("To Start a Matrix Solve, there is only 3 steps:\n"));

    btnBox->setOrientation(Qt::Vertical);
    btnBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
    connect(btnBox, SIGNAL(accepted()), dialog, SLOT(accept()));
    connect(btnBox, SIGNAL(rejected()), dialog, SLOT(reject()));

    dlLayout->addWidget(txtEdit);
    dlLayout->addWidget(btnBox);

    dialog->resize(800,600);
    dialog->setLayout(dlLayout);
    dialog->show();
}
