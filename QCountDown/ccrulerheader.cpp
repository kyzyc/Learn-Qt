#include "ccrulerheader.h"

#include <Qdebug.h>

#include <QApplication>
#include <QPainter>
#include <QScreen>

#define RULERHEADER_WIDTH  82
#define RULERHEADER_HEIGHT 91

CCRulerHeader::CCRulerHeader(QWidget* parent) : QWidget{parent} {
    qreal scaleFactor = QApplication::primaryScreen()->devicePixelRatio();
    setGeometry(0, 0, RULERHEADER_WIDTH / scaleFactor, RULERHEADER_HEIGHT / scaleFactor);
}

CCRulerHeader::~CCRulerHeader() {
}

void CCRulerHeader::mousePressEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton) {
        last_mouse_position = event->globalPosition();
    }
}

void CCRulerHeader::mouseMoveEvent(QMouseEvent* event) {
    if (event->buttons() & Qt::LeftButton) {
        QPointF position = event->globalPosition() - last_mouse_position;
        // qDebug() << "POSY:" << last_mouse_position << "" << event->globalPos() << " " << position;

        emit rulerHeaderMoveSignal(position.y());
    }
}

void CCRulerHeader::mouseReleaseEvent(QMouseEvent* event) {
    Q_UNUSED(event);
    emit rulerHeaderMoveDoneSignal();
}

void CCRulerHeader::paintEvent(QPaintEvent* event) {
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.save();

    painter.drawPixmap(rect(), QPixmap(":/images/ImageResources/ruler_head.png"));
    painter.restore();
}
