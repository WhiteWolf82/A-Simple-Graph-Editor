#ifndef DRAWWIDGET_H
#define DRAWWIDGET_H

#include<QWidget>
#include<QtGui>
#include<QMouseEvent>
#include<QPaintEvent>
#include<QResizeEvent>
#include<QColor>
#include<QPen>
#include<QLabel>
#include<QPoint>
#include<QPainter>
#include<QPalette>
#include<QImage>
#include<QPixmap>

class DrawWidget : public QWidget
{
    Q_OBJECT
    friend class MainWindow;
public:
    explicit DrawWidget(QWidget *parent = nullptr);
    QColor penColor;    //pen color
signals:

public slots:
    void save(QString path);
    void load(QString fileName);
    void drawNone();
    void drawLine();
    void drawArrow();
    void drawRectangle();
    void drawEllipse();
    void drawTriangle();
    void drawEraser();
    void drawSelect();
    void drawMove();
    void drawResize();
    void drawFill();
    void drawRotate(int degree);
    void changePenWidth(int penWidth);
    void changePenStyle(Qt::PenStyle penStyle);
    void changePenColor(QColor penColor);
    void changeBrushColor(QColor brushColor);
    void deleteSelectArea();

private:
    enum{
        NONE,   //NONE represents drawing freely
        LINE,
        ARROW,
        RECTANGLE,
        ELLIPSE,
        TRIANGLE,
        ERASER,
        FILL,
        MOVE,
        SELECT,
        RESIZE,
        ROTATE
    }curShape;
    QImage img, tmp;    //double-buffering
    QImage moveImg;     //used to move some image
    QImage resizeImg;   //used to resize some image (zoom in or zoom out)
    QImage loadImg;     //used to load an existing image
    QImage rotateImg;   //used to rotate some image
    QPoint beginPos, endPos;    //begin and end positions
    QPoint resizeTL, resizeTR, resizeBL, resizeBR;  //points of the rectangle resized image
    QPoint center;  //the center of the selected area (rectangle)
    QRect selectArea;   //selected area (rectangle)
    QRect source, target;   //used to move and resize (each represents old area and new area, respectively)
    int resizeFlag;     //used to determine different types of resizing
    QColor bgdColor;    //background color

    QColor brushColor;  //brush color
    int penWidth;   //pen width
    Qt::PenStyle penStyle;  //pen style
    bool isDraw;    //whether is drawing
    bool isSelect;  //whether has selected
    bool isMove;    //whether has moved
    bool isResize;  //whether has resized
    bool isRotate;  //whether has rotated

private slots:
    void drawImage(QImage& img);
    void paintEvent(QPaintEvent* e);
    void mousePressEvent(QMouseEvent* e);
    void mouseMoveEvent(QMouseEvent* e);
    void mouseReleaseEvent(QMouseEvent* e);
};

#endif // DRAWWIDGET_H
