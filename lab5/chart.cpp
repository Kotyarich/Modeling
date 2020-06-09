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
    QChart (QChart::ChartTypeCartesian, parent, w_flags), _color_blue(252), _color_red(0) {
    QPen red(Qt::blue);
    red.setWidth(1);

    setAnimationOptions(QChart::AllAnimations);
    setAcceptHoverEvents(true);

    auto axis_x = new QValueAxis(this);
    auto axis_y = new QValueAxis(this);
    _axis_x.push_back(axis_x);
    _axis_y.push_back(axis_y);

    addAxis(axis_x, Qt::AlignBottom);
    addAxis(axis_y, Qt::AlignLeft);

    axis_x->setTickCount(5);
    axis_x->setRange(0, 10);
    axis_y->setRange(250, 350);

    legend()->show();
    legend()->setAlignment(Qt::AlignRight);

    grabGesture(Qt::PanGesture);
    grabGesture(Qt::PinchGesture);
}

Chart::~Chart() {}

void Chart::add_values(const std::vector<double> &x, const std::vector<double> &y, const QString &label) {
    createNewSeries();
    auto series = _series[_series.size() - 1];

    series->clear();
    for (auto i = 0ul; i < x.size(); i+=1) {
        series->append(x[i], y[i]);
    }        

    series->setName(label);
    if (x[x.size() - 1] > 11) {
        _axis_x[0]->setRange(0, x[x.size() - 1]);
    }

    auto min = y[0];
    auto max = y[0];
    for (auto i = 0ul; i < y.size(); i++) {
        min = std::min(min, y[i]);
        max = std::max(max, y[i]);
    }
    auto delta = std::abs(max - min) / 10;

    auto old_min = _axis_y[0]->min();
    auto old_max = _axis_y[0]->max();
    _axis_y[0]->setRange(std::min(old_min, min - delta), std::max(old_max, max + delta));
}

bool Chart::sceneEvent(QEvent *event) {
    if (event->type() == QEvent::Gesture)
        return gestureEvent(static_cast<QGestureEvent *>(event));
    return QChart::event(event);
}

bool Chart::gestureEvent(QGestureEvent *event) {
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

void Chart::createNewSeries() {
    auto series = new QLineSeries(this);
    QPen red(QColor(_color_red, 0, _color_blue));
    _color_blue -= 20;
    _color_red += 20;
    red.setWidth(2);
    series->setPen(red);
    addSeries(series);

    _series.push_back(series);

    series->attachAxis(_axis_x[0]);
    series->attachAxis(_axis_y[0]);
}
