#ifndef CCRULERHEADER_H
#define CCRULERHEADER_H

#include <QMouseEvent>
#include <QWidget>

class CCRulerHeader : public QWidget {
    Q_OBJECT
public:
    explicit CCRulerHeader(QWidget* parent = nullptr);
    virtual ~CCRulerHeader();

private:
    QPointF last_mouse_position;

signals:
    void rulerHeaderMoveSignal(int posY);
    void rulerHeaderMoveDoneSignal();

protected:
    void mousePressEvent(QMouseEvent* event);
    void mouseMoveEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent* event);

    void paintEvent(QPaintEvent* event);
};

#endif  // CCRULERHEADER_H
