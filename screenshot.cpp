#include "screenshot.h"
#include <QPainter>

Screenshot::Screenshot(QWidget *parent) : QWidget(parent)
{
    crosshair = QCursor(Qt::CrossCursor);
    setCursor(crosshair);
    setAutoFillBackground(false);
//    setAttribute(Qt::WA_TranslucentBackground);
//    setAttribute(Qt::WA_OpaquePaintEvent);
//    setAttribute(Qt::WA_NoSystemBackground);
    setWindowState(Qt::WindowActive | Qt::WindowFullScreen);
    setWindowOpacity(0.3);
    enableSelectionFrame = false;
}

void Screenshot::normalizeSelectionFrame()
{
//  2 release
//   \
//    \
//     1 click

    if((startPoint.x() > endPoint.x()) && (startPoint.y() > endPoint.y()))
    {
        selectionFrame = QRect(endPoint, startPoint);
    }

//        1 click
//       /
//      /
//     /
//    /
//   /
//  2 release

    if((startPoint.x() > endPoint.x()) && (startPoint.y() < endPoint.y()))
    {
        selectionFrame = QRect(endPoint.x(),
                               startPoint.y(),
                               startPoint.x() - endPoint.x(),
                               endPoint.y() - startPoint.y());
    }
//        2 release
//       /
//      /
//     /
//    /
//   /
//  1 click

    if((startPoint.x() < endPoint.x()) && (startPoint.y() > endPoint.y()))
    {
        selectionFrame = QRect(startPoint.x(),
                               endPoint.y(),
                               endPoint.x() - startPoint.x(),
                               startPoint.y() - endPoint.y());
    }
//  1 click
//   \
//    \
//     2 release

    if((startPoint.x() < endPoint.x()) && (startPoint.y() < endPoint.y()))
    {
        selectionFrame = QRect(startPoint, endPoint);
    }
}

void Screenshot::paintEvent(QPaintEvent *e)
{
    QPainter painter(this);
    QBrush brush(Qt::SolidPattern);

    if(enableSelectionFrame)
    {
        painter.drawRect(QRect(startPoint, endPoint));
        painter.fillRect(QRect(startPoint, endPoint), brush);
    }
}

void Screenshot::mousePressEvent(QMouseEvent *e)
{
    startPoint = e->pos();
    endPoint = e->pos();
    enableSelectionFrame = true;
}

void Screenshot::mouseMoveEvent(QMouseEvent *e)
{
    endPoint = e->pos();
    update();
}

void Screenshot::mouseReleaseEvent(QMouseEvent *e)
{
    enableSelectionFrame = false;
    setWindowOpacity(0.0);
    QTimer::singleShot(100, this, SLOT(slot_get_screenshot()));
}

//void ScreenshotDesktopWidget::keyPressEvent(QKeyEvent *e)
//{
//    if(e->key() == Qt::Key_Enter || e->key() == Qt::Key_Return) {
//        emit signal_screenArea(QRect(startPoint, endPoint));
//        close();
//    }
//}

void Screenshot::slot_get_screenshot()
{
    screen =  QPixmap::grabWindow(QApplication::desktop()->winId());
    normalizeSelectionFrame();
    screen = screen.copy(selectionFrame);
    screen.save(QString(QDateTime::currentDateTime().toString()), "PNG", 100);
    close();
}
