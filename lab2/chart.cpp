#include "chart.h"
#include <QtCharts/QChart>
#include <QtCharts/QAbstractAxis>
#include <QtCharts/QValueAxis>
#include <QtCharts/QSplineSeries>
#include <QtCharts/QLineSeries>
#include <QtWidgets/QGesture>
#include <QtWidgets/QGraphicsScene>
#include <QtWidgets/QGraphicsView>

Chart::Chart(QGraphicsItem *parent, Qt::WindowFlags w_flags):
    QChart (QChart::ChartTypeCartesian, parent, w_flags),
    _series(nullptr),
    _series2(nullptr),
    _axis_x(new QValueAxis()),
    _axis_y(new QValueAxis()) {

    _series2 = new QLineSeries(this);
    QPen green(Qt::green);
    green.setWidth(1);
    _series2->setPen(green);
    addSeries(_series2);

    _series = new QLineSeries(this);
    QPen red(Qt::blue);
    red.setWidth(1);
    _series->setPen(red);
    addSeries(_series);

    setAnimationOptions(QChart::AllAnimations);
    setAcceptHoverEvents(true);

    addAxis(_axis_x, Qt::AlignBottom);
    addAxis(_axis_y, Qt::AlignLeft);        

    _series->attachAxis(_axis_x);
    _series->attachAxis(_axis_y);
    _series2->attachAxis(_axis_x);
    _series2->attachAxis(_axis_y);

    _axis_x->setTickCount(5);
    _axis_x->setRange(0, 10);
    _axis_y->setRange(0, 10);

    legend()->hide();

    grabGesture(Qt::PanGesture);
    grabGesture(Qt::PinchGesture);
}

Chart::~Chart() {}

void Chart::add_values(const std::vector<double> &x, const std::vector<double> &y) {
    _series->clear();
    for (auto i = 0ul; i < x.size(); i++) {
        _series->append(x[i], y[i]);
    }

    auto y_max = y[10];
    auto y_min = y[10];
    for (auto i = 0ul; i < y.size(); i++) {
        if (y[i] > y_max) {
            y_max = y[i];
        } else if (y[i] < y_min) {
            y_min = y[i];
        }
    }
    auto delta = y_max - y_min;
    delta /= 5;
    if (delta <= 1e-6) delta++;

    _axis_x->setRange(0, x[x.size() - 1]);
    _axis_y->setRange(y_min - delta, y_max + delta);
}

void Chart::add_series(const std::vector<double> &x, const std::vector<double> &y) {
    _series2->clear();
    for (auto i = 0ul; i < x.size(); i++) {
        _series2->append(x[i], y[i]);
    }
}

QLineSeries *Chart::get_series() const {
    return _series;
}

bool Chart::sceneEvent(QEvent *event)
{
    if (event->type() == QEvent::Gesture)
        return gestureEvent(static_cast<QGestureEvent *>(event));
    return QChart::event(event);
}

QLineSeries *Chart::get_series2() const
{
    return _series2;
}

bool Chart::gestureEvent(QGestureEvent *event)
{
    if (QGesture *gesture = event->gesture(Qt::PanGesture)) {
        QPanGesture *pan = static_cast<QPanGesture *>(gesture);
        QChart::scroll(-(pan->delta().x()), pan->delta().y());
    }

    if (QGesture *gesture = event->gesture(Qt::PinchGesture)) {
        QPinchGesture *pinch = static_cast<QPinchGesture *>(gesture);
        if (pinch->changeFlags() & QPinchGesture::ScaleFactorChanged)
            QChart::zoom(pinch->scaleFactor());
    }

    return true;
}
