#ifndef CHART_H
#define CHART_H

#include <QtCharts/QChart>
#include <QtCharts/QAbstractAxis>
#include <QtCharts/QValueAxis>
#include <QtCharts/QSplineSeries>

QT_BEGIN_NAMESPACE
class QGestureEvent;
QT_END_NAMESPACE

QT_CHARTS_USE_NAMESPACE

class Chart : public QChart {
    Q_OBJECT
public:
    Chart(QGraphicsItem *parent = nullptr, Qt::WindowFlags w_flags = nullptr);
    virtual ~Chart();

    void add_values(const std::vector<double> &x, const std::vector<double> &y);
    QLineSeries *get_series() const;

protected:
    bool sceneEvent(QEvent *event);

private:
    QLineSeries *_series;
    QStringList m_titles;
    QValueAxis *_axis_x;
    QValueAxis *_axis_y;

    bool gestureEvent(QGestureEvent *event);
};

#endif // CHART_H
