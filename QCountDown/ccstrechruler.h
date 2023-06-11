#ifndef CCSTRECHRULER_H
#define CCSTRECHRULER_H

#include <QWidget>

class CCRulerHeader;

class CCStrechRuler : public QWidget {
    Q_OBJECT
public:
    explicit CCStrechRuler(QWidget* parent = nullptr);
    virtual ~CCStrechRuler();
    void UpdateRulerHeight(float aValue);
    void SetRulerReset();

private:
    CCRulerHeader* m_pRulerHeader;
    QPointF        m_originalPosition;
    int            m_currentPosY;

signals:
    void rulerStrechSignal(float value);
    void rulerStrechDoneSignal(float value);

private slots:
    void OnRulerHeaderMove(int aValue);
    void OnRulerHeaderMoveDone();

protected:
    void paintEvent(QPaintEvent* event);
};

#endif  // CCSTRECHRULER_H
