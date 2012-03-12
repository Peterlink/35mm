#ifndef SCREENSHOT_H
#define SCREENSHOT_H

#include <QApplication>
#include <QTimer>
#include <QDesktopWidget>
#include <QWidget>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QDateTime>
#include <QRect>

class Screenshot : public QWidget
{
    Q_OBJECT

public:
    explicit Screenshot(QWidget *parent = 0);

private:
    QCursor crosshair;
    QPixmap screen;
    QPoint startPoint;
    QPoint endPoint;
    QRect selectionFrame;
    bool enableSelectionFrame;

    void normalizeSelectionFrame();
protected:
    void paintEvent(QPaintEvent *e);
    void mousePressEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);
//    void keyPressEvent(QKeyEvent *);

signals:
    void signal_screenArea(const QRect&);

private slots:
    void slot_get_screenshot();
};

#endif
