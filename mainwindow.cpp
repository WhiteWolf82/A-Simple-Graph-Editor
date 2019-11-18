#include "dialog.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDesktopWidget>
#include <QDialog>
#include <QMessageBox>
#include <QPainter>
#include <QMouseEvent>
#include <QToolButton>
#include <QColorDialog>
#include <QSpinBox>
#include <QLineEdit>
#include <QDebug>
#include <QFileDialog>
#include <QFont>
#include <QFontDialog>
#include <QCheckBox>
#include <QTime>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QWidgetAction>
#include <QClipboard>
#include <QImage>
#include <QScrollArea>
#include <QTableWidget>
#include <QLabel>
#include <QMimeData>
#include <QTextBrowser>
#include <QShortcut>
#include <QSignalMapper>
#include <QColorDialog>

class Dialog;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent), ui(new Ui::MainWindow)
{
        ui->setupUi(this);
        this->setWindowTitle(tr("OOP_CAD"));
        QToolBar *bar = this->addToolBar("Tools");
        QActionGroup *group = new QActionGroup(bar);
        setWindowIcon(QIcon(":/images/OOP_CAD.png"));

        QAction *drawSelectAction = new QAction("Select",bar);
        drawSelectAction->setIcon(QIcon(":/images/select.png"));
        drawSelectAction->setText(tr("Select"));
        drawSelectAction->setStatusTip(tr("Select an area."));
        drawSelectAction->setCheckable(true);
        group->addAction(drawSelectAction);
        bar->addAction(drawSelectAction);

        QAction *drawMoveAction = new QAction("Move",bar);
        drawMoveAction->setIcon(QIcon(":/images/move.png"));
        drawMoveAction->setText(tr("Move"));
        drawMoveAction->setStatusTip(tr("Move a selected area."));
        drawMoveAction->setCheckable(true);
        group->addAction(drawMoveAction);
        bar->addAction(drawMoveAction);

        QAction *drawResizeAction = new QAction("Resize",bar);
        drawResizeAction->setIcon(QIcon(":/images/resize.png"));
        drawResizeAction->setText(tr("Resize"));
        drawResizeAction->setStatusTip(tr("Resize a selected area."));
        drawResizeAction->setCheckable(true);
        group->addAction(drawResizeAction);
        bar->addAction(drawResizeAction);

        drawPenAction = new QAction("Pen",bar);
        drawPenAction->setIcon(QIcon(":/images/pen.png"));
        drawPenAction->setText(tr("Pen"));
        drawPenAction->setStatusTip(tr("Draw freely."));
        drawPenAction->setCheckable(true);
        drawPenAction->setChecked(true);
        group->addAction(drawPenAction);
        bar->addAction(drawPenAction);

        QAction *drawLineAction = new QAction("Line", bar);
        drawLineAction->setIcon(QIcon(":/images/line.png"));
        drawLineAction->setToolTip(tr("Line"));
        drawLineAction->setStatusTip(tr("Draw a line."));
        drawLineAction->setCheckable(true);
        group->addAction(drawLineAction);
        bar->addAction(drawLineAction);

        QAction *drawRectAction = new QAction("Rectangle", bar);
        drawRectAction->setIcon(QIcon(":/images/rectangle.png"));
        drawRectAction->setToolTip(tr("Rectangle"));
        drawRectAction->setStatusTip(tr("Draw a rectangle."));
        drawRectAction->setCheckable(true);
        group->addAction(drawRectAction);
        bar->addAction(drawRectAction);

        QAction *drawEllipseAction = new QAction("Ellipse",bar);
        drawEllipseAction->setIcon(QIcon(":/images/ellipse.png"));
        drawEllipseAction->setToolTip(tr("Ellipse"));
        drawEllipseAction->setStatusTip(tr("Draw an ellipse."));
        drawEllipseAction->setCheckable(true);
        group->addAction(drawEllipseAction);
        bar->addAction(drawEllipseAction);


        QAction *drawEraserAction = new QAction("Eraser",bar);
        drawEraserAction->setIcon(QIcon(":/images/eraser.png"));
        drawEraserAction->setText(tr("Eraser"));
        drawEraserAction->setStatusTip(tr("Use an eraser."));
        drawEraserAction->setCheckable(true);
        group->addAction(drawEraserAction);
        bar->addAction(drawEraserAction);

        QAction *drawTriangleAction = new QAction("Triangle",bar);
        drawTriangleAction->setIcon(QIcon(":/images/triangle.png"));
        drawTriangleAction->setText(tr("Triangle"));
        drawTriangleAction->setStatusTip(tr("Draw a triangle."));
        drawTriangleAction->setCheckable(true);
        group->addAction(drawTriangleAction);
        bar->addAction(drawTriangleAction);

        QAction *drawArrowAction = new QAction("Arrow",bar);
        drawArrowAction->setIcon(QIcon(":/images/arrow.png"));
        drawArrowAction->setText(tr("Arrow"));
        drawArrowAction->setStatusTip(tr("Draw an arrow."));
        drawArrowAction->setCheckable(true);
        group->addAction(drawArrowAction);
        bar->addAction(drawArrowAction);

        QAction *drawFillAction = new QAction("Fill",bar);
        drawFillAction->setIcon(QIcon(":/images/fill.png"));
        drawFillAction->setText(tr("Fill"));
        drawFillAction->setStatusTip(tr("Fill a selected area."));
        drawFillAction->setCheckable(true);
        group->addAction(drawFillAction);
        bar->addAction(drawFillAction);

        QAction *drawRotateAction1 = new QAction("Rotate1",bar);
        drawRotateAction1->setIcon(QIcon(":/images/rightRotate.png"));
        drawRotateAction1->setText(tr("Rotate1"));
        drawRotateAction1->setStatusTip(tr("Rotate 90° clockwise."));
        drawRotateAction1->setCheckable(true);
        group->addAction(drawRotateAction1);
        bar->addAction(drawRotateAction1);

        QAction *drawRotateAction2 = new QAction("Rotate2",bar);
        drawRotateAction2->setIcon(QIcon(":/images/leftRotate.png"));
        drawRotateAction2->setText(tr("Rotate2"));
        drawRotateAction2->setStatusTip(tr("Rotate 90° counterclockwise."));
        drawRotateAction2->setCheckable(true);
        group->addAction(drawRotateAction2);
        bar->addAction(drawRotateAction2);

        QToolBar *bar_2 = this->addToolBar("Size");

        QLabel *lb = new QLabel("调整画笔粗细：");
        bar_2->addWidget(lb);

        QSpinBox *sp=new QSpinBox(bar);
        sp->setValue(3);
        bar_2->addWidget(sp);

        //this->addToolBarBreak();

        QToolBar *bar_3 = this->addToolBar("Color");

        QAction *lb_2 = new QAction();
        lb_2->setText("画笔颜色");
        bar_3->addAction(lb_2);

        qli = new QLineEdit();
        qli->setFocusPolicy(Qt::NoFocus);
        qli->setStyleSheet("background-color:rgb(0,0,0);");
        bar_3->addWidget(qli);

        QAction *lb_3 = new QAction();
        lb_3->setText("填充颜色");
        bar_3->addAction(lb_3);

        qli_2 = new QLineEdit();
        qli_2->setFocusPolicy(Qt::NoFocus);
        qli_2->setStyleSheet("background-color:rgb(255,255,255);");
        bar_3->addWidget(qli_2);

        ui->actionSolidLine->setChecked(true);

        statusBar();

        area = new DrawWidget;
        resize(1100,800);
        scrollArea = new QScrollArea;
        scrollArea->setBackgroundRole(QPalette::Dark);
        scrollArea->setWidget(area);
        scrollArea->widget()->setMinimumSize(QApplication::desktop()->width(), QApplication::desktop()->height() * 0.9);
        setCentralWidget(scrollArea);

        connect(drawLineAction,SIGNAL(triggered()),
                area,SLOT(drawLine()));

        connect(drawRectAction,SIGNAL(triggered()),
                area,SLOT(drawRectangle()));

        connect(drawEllipseAction,SIGNAL(triggered()),
                area,SLOT(drawEllipse()));

        connect(drawPenAction,SIGNAL(triggered()),
                area,SLOT(drawNone()));

        connect(drawEraserAction,SIGNAL(triggered()),
                area,SLOT(drawEraser()));

        connect(drawTriangleAction,SIGNAL(triggered()),
                area,SLOT(drawTriangle()));

        connect(drawSelectAction,SIGNAL(triggered()),
                area,SLOT(drawSelect()));

        connect(drawMoveAction,SIGNAL(triggered()),
                area,SLOT(drawMove()));

        connect(drawArrowAction,SIGNAL(triggered()),
                area,SLOT(drawArrow()));

        connect(drawFillAction,SIGNAL(triggered()),
                area,SLOT(drawFill()));

        connect(drawResizeAction,SIGNAL(triggered()),
                area,SLOT(drawResize()));

        connect(sp,SIGNAL(valueChanged(int)),area,SLOT(changePenWidth(int)));

        connect(lb_2,SIGNAL(triggered()),this,SLOT(changecolor()));

        connect(lb_3,SIGNAL(triggered()),this,SLOT(changeBrushcolor()));

        QSignalMapper* signalMapper = new QSignalMapper(area);

        connect(drawRotateAction1,SIGNAL(triggered()),
                signalMapper,SLOT(map()));
        connect(drawRotateAction2,SIGNAL(triggered()),
                signalMapper,SLOT(map()));

        connect(ui->action_2,SIGNAL(triggered()),this,SLOT(openfile()));

        connect(ui->action_3,SIGNAL(triggered()),this,SLOT(savefile()));

        connect(ui->action,SIGNAL(triggered()),this,SLOT(createfile()));

        connect(ui->actionSolidLine,SIGNAL(triggered()),this,SLOT(solidline()));

        connect(ui->actionDashLine,SIGNAL(triggered()),this,SLOT(dashline()));

        connect(ui->actionDashDotLine,SIGNAL(triggered()),this,SLOT(dashdotline()));

        connect(ui->actionDotLine,SIGNAL(triggered()),this,SLOT(dotline()));

        connect(ui->actionDashDotDotLine,SIGNAL(triggered()),this,SLOT(dashdotdotline()));

        connect(ui->actionCustomDashLine,SIGNAL(triggered()),this,SLOT(customdashline()));


        signalMapper->setMapping(drawRotateAction1, 90);
        signalMapper->setMapping(drawRotateAction2, -90);

        connect(signalMapper, SIGNAL(mapped(int)), area, SLOT(drawRotate(int)));

}

void MainWindow::changecolor()
{
    QColorDialog *m_pColor = new QColorDialog(this);
    m_pColor->setCurrentColor(area->penColor);
    m_pColor->show();
    connect(m_pColor,SIGNAL(colorSelected(QColor)),this,SLOT(SetColor(QColor)));
}

void MainWindow::changeBrushcolor()
{
    QColorDialog *m_pColor = new QColorDialog(this);
    m_pColor->setCurrentColor(area->brushColor);
    m_pColor->show();
    connect(m_pColor,SIGNAL(colorSelected(QColor)),this,SLOT(SetBrushColor(QColor)));
}

void MainWindow::SetColor(QColor a)
{
    area->changePenColor(a);
   // area->changeBrushColor(a);
    int r=a.red();
    int g=a.green();
    int b=a.blue();
    QString colorStr=QString("background-color:rgb(%1,%2,%3);").arg(r).arg(g).arg(b);
    qli->setStyleSheet(colorStr);
}

void MainWindow::SetBrushColor(QColor a)
{
    area->changeBrushColor(a);
    int r=a.red();
    int g=a.green();
    int b=a.blue();
    QString colorStr=QString("background-color:rgb(%1,%2,%3);").arg(r).arg(g).arg(b);
    qli_2->setStyleSheet(colorStr);
}
MainWindow::~MainWindow()
{

}

void MainWindow::openfile()
{
    QString fileName = QFileDialog::getOpenFileName(this,
                                                        tr("选择图片"),
                                                        "F:",
                                                        tr("图片文件(*png *jpg);;"
                                                           ));
    area->load(fileName);
}

void MainWindow::savefile()
{
    QString fileName = QFileDialog::getSaveFileName(this,
            tr("文件另存为"),
            "",
            tr("(*.jpg);;""(*.jpeg);;""(*.png);;""(*.bmp)"));
    area->save(fileName);
}

void MainWindow::createfile()
{
    Dialog *a = new Dialog();
    a->setWindowTitle(tr("新建画布"));
    a->setWindowIcon(QIcon(":/images/OOP_CAD.png"));

    a->setLineEditText(tr("800"));
    a->setLineEdit_2Text(tr("600"));
    a->resize(400,300);
    a->show();
    connect(a,SIGNAL(ok_down(Dialog *)),this,SLOT(createfinal(Dialog *)));

}

void MainWindow::createfinal(Dialog *a)
{
    QString l = a->getLineEditText();
    QString w = a->getLineEdit_2Text();
    int le=l.toInt();
    int we=w.toInt();
    area->img = QImage(le, we, QImage::Format_RGB32);
    area->img.fill(area->bgdColor);
    area->update();
    area->tmp = area->img;
    area->curShape = area->NONE;
    this->drawPenAction->setChecked(true);
}


void MainWindow::solidline()
{
    area->changePenStyle(Qt::SolidLine);
}
void MainWindow::dashline()
{
    area->changePenStyle(Qt::DashLine);
}
void MainWindow::dotline()
{
    area->changePenStyle(Qt::DotLine);
}
void MainWindow::dashdotline()
{
    area->changePenStyle(Qt::DashDotLine);
}
void MainWindow::dashdotdotline()
{
    area->changePenStyle(Qt::DashDotDotLine);
}
void MainWindow::customdashline()
{
    area->changePenStyle(Qt::CustomDashLine);
}
