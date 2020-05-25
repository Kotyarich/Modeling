#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QtCharts/QChartView>
#include "model.h"
#include "methods.h"
#include "chart.h"

QT_BEGIN_NAMESPACE
class QMouseEvent;
class QResizeEvent;
QT_END_NAMESPACE

class Callout;

namespace Ui {
class Widget;
}

class Widget : public QWidget {
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = nullptr);
    ~Widget();

public slots:
    void current_tooltip(QPointF point, bool state);

private slots:
    void count();

private:
    Ui::Widget *ui;

    Callout *tooltip;
    void init_chart();
    void chart_tooltip(Callout *&tooltip, Chart *chart, QPointF point, bool state);
};

#endif // WIDGET_H
