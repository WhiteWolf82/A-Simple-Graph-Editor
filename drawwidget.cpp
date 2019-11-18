#include "drawwidget.h"
#include<queue>
#include<math.h>
#include<QMessageBox>

DrawWidget::DrawWidget(QWidget *parent) : QWidget(parent)
{
    //initialize some attributes
    penWidth = 3;
    penColor = Qt::black;
    penStyle = Qt::SolidLine;

    brushColor = Qt::transparent;
    bgdColor = Qt::white;

    img = QImage(1000, 600, QImage::Format_RGB32);
    img.fill(bgdColor);
    tmp = img;

    curShape = NONE;
    isDraw = false;
    isSelect = false;
    isMove = false;
    isResize = false;
    isRotate = false;
}

void DrawWidget::paintEvent(QPaintEvent* e)
{
    QPainter painter(this);

    if (isDraw == true)     //if is drawing, that is, the mouse is either moving or being pressed
    {
        painter.drawImage(0, 0, tmp);
    }
    else    //if drawing has been completed, that is, the mouse is being released
    {
        painter.drawImage(0, 0, img);
    }
}

void DrawWidget::mousePressEvent(QMouseEvent* e)
{
    if (e->button() == Qt::LeftButton)
    {
        if (curShape == FILL)
        {
            //use BFS to fill
            std::queue<QPoint> q;
            bool isVisit[1000][600] = {false};
            q.push(e->pos());
            while(!q.empty())
            {
                QPoint t = q.front();
                q.pop();
                if(isVisit[t.x()][t.y()])
                    continue;
                img.setPixelColor(t.x(), t.y(), brushColor);
                isVisit[t.x()][t.y()] = true;

                //if a point is in the image and bgdcolor and not visited, add to the queue
                if(t.x()-1 >= 0 && img.pixelColor(t.x()-1, t.y()) == bgdColor && !isVisit[t.x()-1][t.y()])
                    q.push(QPoint(t.x()-1, t.y()));
                if(t.y()-1 >= 0 && img.pixelColor(t.x(), t.y()-1) == bgdColor && !isVisit[t.x()][t.y()-1])
                    q.push(QPoint(t.x(), t.y()-1));
                if(t.x()+1 < img.width() && img.pixelColor(t.x()+1, t.y()) == bgdColor && !isVisit[t.x()+1][t.y()])
                    q.push(QPoint(t.x()+1, t.y()));
                if(t.y()+1 < img.height() && img.pixelColor(t.x(), t.y()+1) == bgdColor && !isVisit[t.x()][t.y()+1])
                    q.push(QPoint(t.x(), t.y()+1));
            }
        }
        else if (curShape == ROTATE)
        {
            if (selectArea.contains(e->pos()) && isRotate == false)
            {
                source = QRect(QPoint(0, 0), rotateImg.size());
                target = QRect(QPoint(center.x() - selectArea.height() / 2, center.y() - selectArea.width() / 2),
                               QPoint(center.x() + selectArea.height() / 2, center.y() + selectArea.width() / 2));
                isRotate = true;
            }
        }
        else
        {
            if (curShape == SELECT)
            {
                if (isSelect == true)   //if already selected, the old selected area should be deleted to select a new area
                {
                    QPainter painter(&img);
                    painter.setPen(QPen(bgdColor, 1));  //set pen color in order to delete the old rectangle
                    painter.drawRect(resizeTL.x(), resizeTL.y(), resizeBR.x() - resizeTL.x(), resizeBR.y() - resizeTL.y());
                    isSelect = false;
                }
            }
            if (curShape == RESIZE)     //determine type of resizing by the position of the mouse
            {
                QPoint center((resizeTL.x() + resizeTR.x()) / 2, (resizeTL.y() + resizeBL.y()) / 2);
                if (e->pos().x() < center.x() && e->pos().y() < center.y())
                {
                    resizeFlag = 1;
                    setCursor(Qt::SizeFDiagCursor);
                }
                else if (e->pos().x() > center.x() && e->pos().y() < center.y())
                {
                    resizeFlag = 2;
                    setCursor(Qt::SizeBDiagCursor);
                }
                else if (e->pos().x() < center.x() && e->pos().y() > center.y())
                {
                    resizeFlag = 3;
                    setCursor(Qt::SizeBDiagCursor);
                }
                else if (e->pos().x() > center.x() && e->pos().y() > center.y())
                {
                    resizeFlag = 4;
                    setCursor(Qt::SizeFDiagCursor);
                }
            }
            beginPos = e->pos();
            isDraw = true;
        }
    }
    else
        return;
}

void DrawWidget::mouseMoveEvent(QMouseEvent* e)
{
    if (e->buttons() & Qt::LeftButton)
    {
        if (curShape == RESIZE)     //determine corners by the position of the mouse
        {
            if (resizeFlag == 1)
            {
                if (e->pos().x() <= resizeBR.x() && e->pos().y() <= resizeBR.y())   //cannot go beyond the edge
                {
                    resizeTL = e->pos();
                }
            }
            else if (resizeFlag == 2)
            {
                if (e->pos().x() >= resizeBL.x() && e->pos().y() <= resizeBL.y())   //cannot go beyond the edge
                {
                    resizeTR = e->pos();
                }
            }
            else if (resizeFlag == 3)
            {
                if (e->pos().x() <= resizeTR.x() && e->pos().y() >= resizeTR.y())   //cannot go beyond the edge
                {
                    resizeBL = e->pos();
                }
            }
            else if (resizeFlag == 4)
            {
                if (e->pos().x() >= resizeTL.x() && e->pos().y() >= resizeTL.y())   //cannot go beyond the edge
                {
                    resizeBR = e->pos();
                }
            }
        }
        if (curShape == MOVE && isMove == true) //not move again
        {
            return;
        }
        if (curShape == RESIZE && isResize == true) //not resize again
        {
            return;
        }
        endPos = e->pos();
        tmp = img;
        if (curShape == NONE || curShape == ERASER)  //if is drawing freely or using the eraser
            drawImage(img);
        else    //if is drawing some shape or moving
            drawImage(tmp);
    }
    else
        return;
}

void DrawWidget::mouseReleaseEvent(QMouseEvent* e)
{
    if (e->button() == Qt::LeftButton)
    {
        isDraw = false;
        if (curShape == RESIZE) //determine points and type of resizing by the position of the mouse
        {
            setCursor(Qt::ArrowCursor);
        }
        if (curShape == SELECT)
        {
            isSelect = true;
        }
        if (curShape == MOVE)
        {
            QPainter painter(&img);
            painter.setPen(QPen(penColor, penWidth, penStyle));
            painter.drawImage(target, moveImg, source);
            isMove = true;
            return; //not move again
        }
        if (curShape == RESIZE)
        {
            QPainter painter(&img);
            painter.setPen(QPen(penColor, penWidth, penStyle));
            painter.drawImage(target, resizeImg, source);
            isResize = true;
            return; //not resize again
        }
        endPos = e->pos();
        if (curShape != NONE && curShape != ERASER)
            drawImage(img);
    }
}

void DrawWidget::drawImage(QImage& thisImg)
{
    QPainter painter(&thisImg);
    QPen pen;
    QBrush brush(brushColor);
    pen.setColor(penColor);
    pen.setStyle(penStyle);
    pen.setWidth(penWidth);
    painter.setPen(pen);
    painter.setBrush(brush);

    switch(curShape)
    {
    case NONE: painter.drawLine(beginPos, endPos);
        beginPos = endPos;
        break;
    case LINE: painter.drawLine(beginPos, endPos);
        break;
    case ARROW:
        {
            //temporarily set brush color in order to fill the arrow
            brush.setColor(penColor);
            painter.setBrush(brush);

            //the arrow contains a line and a triangle
            painter.drawLine(beginPos, endPos);
            QLineF l1(beginPos, endPos);
            QLineF l2 = l1.unitVector();
            l2.setLength(l1.length() * 0.14);
            l2.translate(QPointF(l1.dx(), l1.dy()));
            QLineF l3 = l2.normalVector();
            l3.setLength(l3.length() * 0.5);
            QLineF l4 = l3.normalVector().normalVector();
            QPointF p1 = l2.p2(), p2 = l3.p2(), p3 = l4.p2();   //get the points of the triangle
            QPointF points[3] = {p1, p2, p3};
            painter.drawPolygon(points, 3);     //draw the triangle

            //set back brush color
            brush.setColor(brushColor);
            painter.setBrush(brush);
            break;
        }
    case RECTANGLE: painter.drawRect(beginPos.x(), beginPos.y(), endPos.x() - beginPos.x(), endPos.y() - beginPos.y());
        break;
    case ELLIPSE: painter.drawEllipse(beginPos.x(), beginPos.y(), endPos.x() - beginPos.x(), endPos.y() - beginPos.y());
        break;
    case TRIANGLE:
        {
            QPointF points[3] = {QPointF(beginPos.x(), beginPos.y()), QPointF(endPos.x(), endPos.y()),
                                 QPointF(beginPos.x(), endPos.y())};   //used to draw a triangle
            painter.drawPolygon(points, 3);
            break;
        }
    case SELECT:
        {
            //temporarily set pen and brush in order to select
            painter.setPen(QPen(Qt::black, 1, Qt::DashLine));
            painter.setBrush(QBrush(Qt::transparent, Qt::SolidPattern));

            painter.drawRect(beginPos.x(), beginPos.y(), endPos.x() - beginPos.x(), endPos.y() - beginPos.y());
            selectArea = QRect(beginPos, endPos);

            resizeTL = beginPos;
            resizeTR = QPoint(endPos.x(), beginPos.y());
            resizeBL = QPoint(beginPos.x(), endPos.y());
            resizeBR = endPos;

            //set back pen and brush
            painter.setPen(pen);
            painter.setBrush(brush);
            break;
        }
    case ERASER:
        {
            //temporarily set colors to white in order to erase
            pen.setColor(bgdColor);
            brush.setColor(bgdColor);
            //temporarily set new pen and brush
            painter.setPen(pen);
            painter.setBrush(brush);

            painter.drawEllipse(endPos.x(),endPos.y(),20,20);   //the shape of the eraser is a circle with radius 20

            //set back colors
            pen.setColor(penColor);
            brush.setColor(brushColor);
            //set back pen and brush
            painter.setPen(pen);
            painter.setBrush(brush);
            break;
        }
    case MOVE:
        {
            target = QRect(endPos, moveImg.size());
            source = QRect(QPoint(0, 0), moveImg.size());

            if (isMove == false)
            {
                //draws the rectangular portion source of the given image into the target rectangle in the paint device
                painter.drawImage(target, moveImg, source);
            }

            break;
        }
    case RESIZE:
        {
            QRect newRect;  //new image area
            if (resizeFlag == 1 || resizeFlag == 4)
            {
                newRect = QRect(resizeTL, resizeBR);
                target = QRect(resizeTL, newRect.size());
            }
            else if (resizeFlag == 2 || resizeFlag == 3)
            {
                newRect = QRect(QPoint(resizeBL.x(), resizeTR.y()), QPoint(resizeTR.x(), resizeBL.y()));
                target = QRect(QPoint(resizeBL.x(), resizeTR.y()), newRect.size());
            }
            resizeImg.scaled(newRect.size(), Qt::IgnoreAspectRatio, Qt::FastTransformation);    //resize the selected area of the image
            source = QRect(QPoint(0, 0), resizeImg.size());
            if (isResize == false)
            {
                painter.drawImage(target, resizeImg, source);   //copy this resized image back to the initial image
            }
            break;
        }
    case ROTATE:
        {
            painter.drawImage(target, rotateImg, source);
            break;
        }
    }
    update();
}

void DrawWidget::deleteSelectArea()
{
    QPainter painter(&img);
    painter.setPen(QPen(bgdColor, 1));  //set pen color in order to delete the old rectangle
    painter.drawRect(resizeTL.x(), resizeTL.y(), resizeBR.x() - resizeTL.x(), resizeBR.y() - resizeTL.y());
}

void DrawWidget::drawNone()
{
    deleteSelectArea();
    curShape = NONE;
    setCursor(Qt::ArrowCursor);
}

void DrawWidget::drawLine()
{
    deleteSelectArea();
    curShape = LINE;
    setCursor(Qt::CrossCursor);
}

void DrawWidget::drawArrow()
{
    deleteSelectArea();
    curShape = ARROW;
    setCursor(Qt::CrossCursor);
}

void DrawWidget::drawRectangle()
{
    deleteSelectArea();
    curShape = RECTANGLE;
    setCursor(Qt::CrossCursor);
}

void DrawWidget::drawEllipse()
{
    deleteSelectArea();
    curShape = ELLIPSE;
    setCursor(Qt::CrossCursor);
}

void DrawWidget::drawTriangle()
{
    deleteSelectArea();
    curShape = TRIANGLE;
    setCursor(Qt::CrossCursor);
}

void DrawWidget::drawEraser()
{
    deleteSelectArea();
    curShape = ERASER;
    setCursor(Qt::ArrowCursor);
}

void DrawWidget::drawSelect()
{
    curShape = SELECT;
    setCursor(Qt::CrossCursor);
}

void DrawWidget::drawMove() //movement can only be completed AFTER selection
{
    if (curShape == SELECT)
    {
        //first delete the old image
        QRect old(beginPos, endPos);
        for (int x = old.x(); x < old.x() + old.width(); x++)
            for (int y = old.y(); y < old.y() + old.height(); y++)
                img.setPixelColor(x, y, bgdColor);

        moveImg = tmp.copy(beginPos.x() + 1, beginPos.y() + 1, endPos.x() - beginPos.x() - 1, endPos.y() - beginPos.y() - 1);
        curShape = MOVE;
        isMove = false;
    }
    else
        QMessageBox::warning(NULL, "warning", "Please select first!");
    setCursor(Qt::SizeAllCursor);
}

void DrawWidget::drawResize()   //resizing can only be completed AFTER selection
{
    if (curShape == SELECT)
    {
        //first delete the old image
        QRect old(beginPos, endPos);
        for (int x = old.x(); x < old.x() + old.width(); x++)
            for (int y = old.y(); y < old.y() + old.height(); y++)
                img.setPixelColor(x, y, bgdColor);

        resizeImg = tmp.copy(beginPos.x() + 1, beginPos.y() + 1,
                             endPos.x() - beginPos.x() - 1, endPos.y() - beginPos.y() - 1);
        curShape = RESIZE;
        isResize = false;
    }
    else
        QMessageBox::warning(NULL, "warning", "Please select first!");
}

void DrawWidget::drawFill()
{
    deleteSelectArea();
    curShape = FILL;
    setCursor(Qt::ArrowCursor);
}

void DrawWidget::drawRotate(int degree)   //rotation can only be completed AFTER selection
{
    if (curShape == SELECT)
    {
        //first delete the old image
        QRect old(beginPos, endPos);
        for (int x = old.x(); x < old.x() + old.width(); x++)
            for (int y = old.y(); y < old.y() + old.height(); y++)
                img.setPixelColor(x, y, bgdColor);

        QImage tmpImg = tmp.copy(beginPos.x() + 1, beginPos.y() + 1, endPos.x() - beginPos.x() - 1, endPos.y() - beginPos.y() - 1);

        QMatrix matrix;
        matrix.rotate(degree);
        rotateImg = tmpImg.transformed(matrix, Qt::FastTransformation);
        center = selectArea.center();
        curShape = ROTATE;
        isRotate = false;
    }
    else
        QMessageBox::warning(NULL, "warning", "Please select first!");
    setCursor(Qt::ArrowCursor);
}

void DrawWidget::changePenWidth(int penWidth)
{
    this->penWidth = penWidth;
}

void DrawWidget::changePenColor(QColor penColor)
{
    this->penColor = penColor;
}

void DrawWidget::changeBrushColor(QColor brushColor)
{
    this->brushColor = brushColor;
}

void DrawWidget::changePenStyle(Qt::PenStyle penStyle)
{
    this->penStyle = penStyle;
}

void DrawWidget::save(QString path)
{
    img.save(path);
}

void DrawWidget::load(QString fileName)
{
    loadImg.load(fileName);
    img = loadImg;
    tmp = loadImg;
    resize(loadImg.size());
    update();
}
