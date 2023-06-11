#include "ccwindowwidget.h"

#include <QApplication>
#include <QDebug>
#include <QPainter>
#include <QScreen>
#include <iostream>

#include "ccdialclock.h"
#include "ccstrechruler.h"

#define LAUNCHWIDGET_WIDTH  435
#define LAUNCHWIDGET_HEIGHT 775

#define TOOLBUTTON_WIDTH  75
#define TOOLBUTTON_HEIGHT 75

CCWindowWidget::CCWindowWidget(QWidget* parent) : QWidget{parent} {
    qreal scaleFactor = QApplication::primaryScreen()->devicePixelRatio();

    // 设置窗口大小不可变
    setWindowFlags(windowFlags() & ~Qt::WindowMinMaxButtonsHint);
    setMinimumSize(LAUNCHWIDGET_WIDTH / scaleFactor, LAUNCHWIDGET_HEIGHT / scaleFactor);
    setMaximumSize(LAUNCHWIDGET_WIDTH / scaleFactor, LAUNCHWIDGET_HEIGHT / scaleFactor);
    setAutoFillBackground(true);

    // 获取屏幕数据
    QScreen* screen         = QApplication::primaryScreen();
    QRect    screenGeometry = screen->geometry();

    // 居中坐标
    int startX = (screenGeometry.width() - LAUNCHWIDGET_WIDTH / scaleFactor) / 2;
    int startY = (screenGeometry.height() - LAUNCHWIDGET_HEIGHT / scaleFactor) / 2;
    // 设置居中
    setGeometry(startX, startY, LAUNCHWIDGET_WIDTH / scaleFactor, LAUNCHWIDGET_HEIGHT / scaleFactor);

    m_nRulerSettedSeconds = 0;

    loadFrontWidgets();

    m_pCountDownTimer = new QTimer(this);
    connect(m_pCountDownTimer, SIGNAL(timeout()), this, SLOT(OnCountDownTimerProcess()));
}

CCWindowWidget::~CCWindowWidget() {
    if (m_pStrechRuler != nullptr) {
        delete m_pStrechRuler;
        m_pStrechRuler = nullptr;
    }

    if (m_pDialClock != nullptr) {
        delete m_pDialClock;
        m_pDialClock = nullptr;
    }
}

void CCWindowWidget::loadFrontWidgets() {
    qreal scaleFactor = QApplication::primaryScreen()->devicePixelRatio();

    m_pStrechRuler = new CCStrechRuler(this);
    m_pDialClock   = new CCDialClock(this);

    connect(m_pStrechRuler, SIGNAL(rulerStrechSignal(float)), m_pDialClock, SLOT(SetupClockTime(float)));
    connect(m_pStrechRuler, SIGNAL(rulerStrechDoneSignal(float)), this, SLOT(OnRulerStrechDone(float)));

    QFont font;
    font.setFamily("PingFang TC");
    font.setWeight(QFont::Weight(25 / scaleFactor));
    font.setPixelSize(QFont::Weight(35 / scaleFactor));

    m_pTimeLabel = new QLabel(this);
    m_pTimeLabel->setGeometry(138 / scaleFactor, 525 / scaleFactor, 160 / scaleFactor, 60 / scaleFactor);
    m_pTimeLabel->setFont(font);
    m_pTimeLabel->setStyleSheet("background:transparent; color: #d16b5d");
    m_pTimeLabel->setAlignment(Qt::AlignCenter);
    m_pTimeLabel->setText("00:00");

    m_pPlayButton = new QPushButton(this);
    m_pPlayButton->setGeometry(
        111 / scaleFactor, 630 / scaleFactor, TOOLBUTTON_WIDTH / scaleFactor, TOOLBUTTON_WIDTH / scaleFactor);
    m_pPlayButton->setIcon(QIcon(":/images/ImageResources/play.png"));
    m_pPlayButton->setIconSize(QSize(TOOLBUTTON_WIDTH / scaleFactor, TOOLBUTTON_HEIGHT / scaleFactor));
    m_pPlayButton->setAutoFillBackground(true);
    m_pPlayButton->setFlat(true);
    m_pPlayButton->setStyleSheet("QPushButton{background: transparent;}");

    connect(m_pPlayButton, SIGNAL(clicked(bool)), SLOT(OnPlayButtonClicked()));

    m_pResetButton = new QPushButton(this);
    m_pResetButton->setGeometry(
        251 / scaleFactor, 630 / scaleFactor, TOOLBUTTON_WIDTH / scaleFactor, TOOLBUTTON_WIDTH / scaleFactor);
    m_pResetButton->setIcon(QIcon(":/images/ImageResources/reset.png"));
    m_pResetButton->setIconSize(QSize(TOOLBUTTON_WIDTH / scaleFactor, TOOLBUTTON_HEIGHT / scaleFactor));
    m_pResetButton->setAutoFillBackground(true);
    m_pResetButton->setFlat(true);
    m_pResetButton->setStyleSheet("QPushButton{background: transparent;}");

    connect(m_pResetButton, SIGNAL(clicked(bool)), SLOT(OnResetButtonClicked()));
}

QString CCWindowWidget::caculateFormatTimeWithSeconds(int aSeconds) {
    int tMinutes = aSeconds / 60;
    int tSeconds = aSeconds % 60;

    //    qDebug() << "tMinutes:" << tMinutes;
    //    qDebug() << "tSeconds:" << tSeconds;

    QString strMinutes, strSeconds;

    if ((tMinutes < 10) && (tMinutes >= 0))
        strMinutes = QString("0%1").arg(tMinutes);
    else
        strMinutes = QString("%1").arg(tMinutes);

    if ((tSeconds < 10) && (tSeconds >= 0))
        strSeconds = QString("0%1").arg(tSeconds);
    else
        strSeconds = QString("%1").arg(tSeconds);

    QString formatTime = QString("%1:%2").arg(strMinutes, strSeconds);

    return formatTime;
}

void CCWindowWidget::OnCountDownTimerProcess() {
    QString formatTime;
    if (m_nRulerSettedSeconds <= 0) {
        OnResetButtonClicked();
        return;
    } else
        formatTime = caculateFormatTimeWithSeconds(--m_nRulerSettedSeconds);
    m_pTimeLabel->setText(formatTime);

    float currentRatio = m_nRulerSettedSeconds / 3600.0;

    m_pStrechRuler->UpdateRulerHeight(currentRatio);
}

void CCWindowWidget::OnRulerStrechDone(float aValue) {
    int mSeconds = (int)3600 * aValue;
    //    qDebug() << "mSeconds:" << mSeconds;
    QString formatTime = caculateFormatTimeWithSeconds(mSeconds);
    m_pTimeLabel->setText(formatTime);

    m_nRulerSettedSeconds = mSeconds;

    if (m_pCountDownTimer->isActive()) {
        m_pCountDownTimer->stop();
        m_bTimerPlaying = false;
    }
    m_pCountDownTimer->start(1000);
    m_bTimerPlaying = true;

    m_pPlayButton->setIcon(QIcon(":/images/ImageResources/pause.png"));
}

void CCWindowWidget::OnPlayButtonClicked() {
    if (m_bTimerPlaying == true) {
        if (m_pCountDownTimer->isActive()) {
            m_pCountDownTimer->stop();
            m_bTimerPlaying = false;
        }
        m_pPlayButton->setIcon(QIcon(":/images/ImageResources/play.png"));
    } else {
        m_pCountDownTimer->start(1000);
        m_bTimerPlaying = true;
        m_pPlayButton->setIcon(QIcon(":/images/ImageResources/pause.png"));
    }
}

void CCWindowWidget::OnResetButtonClicked() {
    if (m_bTimerPlaying == true) {
        if (m_pCountDownTimer->isActive()) {
            m_pCountDownTimer->stop();
        }
    }

    m_pPlayButton->setIcon(QIcon(":/images/ImageResources/play.png"));
    m_bTimerPlaying = false;

    m_nRulerSettedSeconds = 0;
    m_pTimeLabel->setText("00:00");
    m_pDialClock->SetupClockTime(0.0);
    m_pStrechRuler->SetRulerReset();
}

void CCWindowWidget::paintEvent(QPaintEvent* event) {
    Q_UNUSED(event);
    QPainter painter(this);
    painter.save();
    painter.drawPixmap(rect(), QPixmap(":/images/ImageResources/background.png"));
    painter.restore();
}
