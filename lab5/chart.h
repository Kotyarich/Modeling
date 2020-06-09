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

    void add_values(const std::vector<double> &x, const std::vector<double> &y, const QString &label);

protected:
    bool sceneEvent(QEvent *event);

private:
    std::vector<QLineSeries*> _series;
    std::vector<QValueAxis*> _axis_x;
    std::vector<QValueAxis*> _axis_y;

    int _color_blue;
    int _color_red;

    bool gestureEvent(QGestureEvent *event);
    void createNewSeries();
};

#endif // CHART_H
