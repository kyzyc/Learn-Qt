#ifndef CCDIALCLOCK_H
#define CCDIALCLOCK_H

#include <QWidget>

class CCDialClock : public QWidget {
    Q_OBJECT
public:
    explicit CCDialClock(QWidget* parent = nullptr);
    virtual ~CCDialClock();

public slots:
    void SetupClockTime(float aValue);

private:
    float m_rotateValue;

protected:
    void paintEvent(QPaintEvent* event);
};

#endif  // CCDIALCLOCK_H
