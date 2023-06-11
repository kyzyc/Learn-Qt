#ifndef CCWINDOWWIDGET_H
#define CCWINDOWWIDGET_H

#include <QLabel>
#include <QPushButton>
#include <QTimer>
#include <QWidget>

class CCStrechRuler;
class CCDialClock;

class CCWindowWidget : public QWidget {
    Q_OBJECT
public:
    explicit CCWindowWidget(QWidget* parent = nullptr);
    virtual ~CCWindowWidget();

private:
    void    loadFrontWidgets();
    QString caculateFormatTimeWithSeconds(int aSeconds);

private:
    CCStrechRuler* m_pStrechRuler;
    CCDialClock*   m_pDialClock;

    QLabel* m_pTimeLabel;
    QTimer* m_pCountDownTimer;

    QPushButton* m_pPlayButton;
    QPushButton* m_pResetButton;

    bool m_bTimerPlaying;
    int  m_nRulerSettedSeconds;

private slots:
    void OnPlayButtonClicked();
    void OnResetButtonClicked();
    void OnCountDownTimerProcess();
    void OnRulerStrechDone(float aValue);

protected:
    void paintEvent(QPaintEvent* event);
};

#endif  // CCWINDOWWIDGET_H
