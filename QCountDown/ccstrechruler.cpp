#include "ccstrechruler.h"

#include <QApplication>
#include <QDebug>
#include <QPalette>
#include <QScreen>
#include <Qpainter>

#include "ccrulerheader.h"

#define RULERWIDGET_STARTX 351
#define RULERWIDGET_STARTY 100

#define RULERWIDGET_WIDTH  82
#define RULERWIDGET_HEIGHT 574

#define RULER_MAX_HEIGHT 486

CCStrechRuler::CCStrechRuler(QWidget* parent) : QWidget{parent} {
    qreal scaleFactor = QApplication::primaryScreen()->devicePixelRatio();
    setGeometry(RULERWIDGET_STARTX / scaleFactor,
                RULERWIDGET_STARTY / scaleFactor,
                RULERWIDGET_WIDTH / scaleFactor,
                RULERWIDGET_HEIGHT / scaleFactor);
    setStyleSheet("background-color: rgdb(0,0,0,0)");

    // 测试
    // setAutoFillBackground(true);
    // QPalette palette;
    // palette.setColor(QPalette::Window, Qt::green);
    // setPalette(palette);

    m_pRulerHeader = new CCRulerHeader(this);
    connect(m_pRulerHeader, SIGNAL(rulerHeaderMoveSignal(int)), SLOT(OnRulerHeaderMove(int)));
    connect(m_pRulerHeader, SIGNAL(rulerHeaderMoveDoneSignal()), SLOT(OnRulerHeaderMoveDone()));

    m_originalPosition = m_pRulerHeader->pos();
    m_currentPosY      = 0;
}

CCStrechRuler::~CCStrechRuler() {
    if (m_pRulerHeader != nullptr) {
        delete m_pRulerHeader;
        m_pRulerHeader = nullptr;
    }
}

void CCStrechRuler::UpdateRulerHeight(float aValue) {
    qreal scaleFactor = QApplication::primaryScreen()->devicePixelRatio();
    int   currentY    = aValue * (RULER_MAX_HEIGHT / scaleFactor);
    m_pRulerHeader->move(m_originalPosition.rx(), currentY);

    float dialRatio = m_pRulerHeader->pos().y() / (float)((int)(RULER_MAX_HEIGHT / scaleFactor));

    emit rulerStrechSignal(dialRatio);

    repaint();
}

void CCStrechRuler::SetRulerReset() {
    m_pRulerHeader->move(m_originalPosition.rx(), 0);
    m_currentPosY = 0;
    repaint();
}

void CCStrechRuler::OnRulerHeaderMove(int aValue) {
    // qDebug() << "posY:" << aValue;
    if (m_pRulerHeader == nullptr) {
        return;
    }

    int   realPosY    = aValue + m_currentPosY;
    qreal scaleFactor = QApplication::primaryScreen()->devicePixelRatio();
    if ((realPosY <= RULER_MAX_HEIGHT / scaleFactor) && (realPosY >= 0)) {
        m_pRulerHeader->move(m_originalPosition.rx(), realPosY);
    }

    float dialRatio = m_pRulerHeader->pos().y() / (float)((int)(RULER_MAX_HEIGHT / scaleFactor));

    emit rulerStrechSignal(dialRatio);

    // 每次header move的时候重新调用paintEvent
    repaint();
}

void CCStrechRuler::OnRulerHeaderMoveDone() {
    qreal scaleFactor = QApplication::primaryScreen()->devicePixelRatio();
    m_currentPosY     = m_pRulerHeader->pos().y();
    float dialRatio   = m_pRulerHeader->pos().y() / (float)((int)(RULER_MAX_HEIGHT / scaleFactor));

    emit rulerStrechDoneSignal(dialRatio);
}

void CCStrechRuler::paintEvent(QPaintEvent* event) {
    Q_UNUSED(event);

    qreal scaleFactor = QApplication::primaryScreen()->devicePixelRatio();

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);  // 反走样

    painter.save();
    painter.setPen(QPen(QColor(127, 159, 124), 1));  // 设置矩形边框
    painter.setBrush(QColor(127, 159, 124));         // 设置矩形填充
    painter.drawRect(15 / scaleFactor, 3 / scaleFactor, 52 / scaleFactor, m_pRulerHeader->pos().y());
    painter.restore();

    int sScale             = 5 / scaleFactor;
    int lScale             = 25 / scaleFactor;
    int currentRulerHeight = m_pRulerHeader->pos().y();

    int sScale_count = currentRulerHeight / sScale;
    int lScale_count = currentRulerHeight / lScale;

    painter.setPen(QPen(QColor(255, 255, 255), 1));

    for (int i = 0; i <= sScale_count; ++i) {
        if (i % 5 != 0)
            painter.drawLine(QPointF(62 / scaleFactor, 5 * i / scaleFactor),
                             QPointF(67 / scaleFactor, 5 * i / scaleFactor));
    }

    for (int i = 0; i <= lScale_count; ++i) {
        painter.drawLine(QPointF(57 / scaleFactor, 25 * i / scaleFactor),
                         QPointF(67 / scaleFactor, 25 * i / scaleFactor));
    }
}
