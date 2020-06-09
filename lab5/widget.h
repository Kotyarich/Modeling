#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QtCharts/QChartView>
#include "model.h"
#include <algorithm>
#include "methods.h"
#include "chart.h"

QT_BEGIN_NAMESPACE
class QMouseEvent;
class QResizeEvent;
QT_END_NAMESPACE


namespace Ui {
class Widget;
}

using ModelResult = std::pair<std::vector<double>, std::vector<std::pair<double, std::vector<double>>>>;

class Widget : public QWidget {
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = nullptr);
    ~Widget();

private slots:
    void count();

private:
    Ui::Widget *ui;

    void init_chart();

    std::vector<double> form_time_array(const ModelResult &results) const;
    std::vector<std::vector<double>> form_temp_from_time_values(const ModelResult &results) const;
};

#endif // WIDGET_H
