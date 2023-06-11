#include "ccdialclock.h"

#include <QApplication>
#include <QDebug>
#include <QPainter>
#include <QScreen>

#define DIALCLOCK_STARTX 75
#define DIALCLOCK_STARTY 188

#define DIALCLOCK_WIDTH  285
#define DIALCLOCK_HEIGHT 285

#define NEEDLE_WIDTH  26
#define NEEDLE_HEIGHT 90

CCDialClock::CCDialClock(QWidget* parent) : QWidget{parent} {
    qreal scaleFactor = QApplication::primaryScreen()->devicePixelRatio();
    setGeometry(DIALCLOCK_STARTX / scaleFactor,
                DIALCLOCK_STARTY / scaleFactor,
                DIALCLOCK_WIDTH / scaleFactor,
                DIALCLOCK_HEIGHT / scaleFactor);

    setStyleSheet("background-color:rgba(0,0,0,0)");
    m_rotateValue = 0.0;
}

CCDialClock::~CCDialClock() {
}

void CCDialClock::SetupClockTime(float aValue) {
    //    qDebug() << "Clock:" << aValue;
    m_rotateValue = 360 * aValue;
    repaint();
}

void CCDialClock::paintEvent(QPaintEvent* event) {
    Q_UNUSED(event);

    qreal scaleFactor = QApplication::primaryScreen()->devicePixelRatio();

    QPainter painter(this);

    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setRenderHint(QPainter::SmoothPixmapTransform, true);

    painter.save();
    painter.drawPixmap(rect(), QPixmap(":/images/ImageResources/dial.png"));
    painter.restore();

    painter.save();
    painter.translate(width() / 2, height() / 2);  // 平移坐标系至中心点
    painter.rotate(m_rotateValue);
    painter.drawPixmap(-NEEDLE_WIDTH / 2 / scaleFactor,
                       -79 / scaleFactor,
                       NEEDLE_WIDTH / scaleFactor,
                       NEEDLE_HEIGHT / scaleFactor,
                       QPixmap(":/images/ImageResources/needle.png"));
    painter.restore();
}
