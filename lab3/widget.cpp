#include "widget.h"
#include "callout.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget),
    tooltip(nullptr) {
    ui->setupUi(this);

    init_chart();
    this->setMouseTracking(true);
}

Widget::~Widget() {
    delete ui;
}

void Widget::current_tooltip(QPointF point, bool state) {
    chart_tooltip(tooltip,
                  static_cast<Chart*>(ui->chartView->chart()),
                  point, state);
}

void Widget::count() {
    auto a0 = ui->a0_input->value();
    auto aN = ui->an_input->value();
    auto k0 = ui->k0_input->value();
    auto kN = ui->kN_input->value();
    auto F0 = ui->F0_input->value();

    Model model(a0, aN, k0, kN, F0);
    auto result = model.count_T();

    auto curr_chart = static_cast<Chart*>(ui->chartView->chart());
    curr_chart->add_values(result.first, result.second);
}

void Widget::init_chart() {
    auto current_chart = new Chart;

    auto axis_y = current_chart->axes(Qt::Vertical).first();
    auto axis_x = current_chart->axes(Qt::Horizontal).first();
    axis_y->setTitleText("T");
    axis_x->setTitleText("x");

    ui->chartView->setChart(current_chart);
    ui->chartView->setRenderHint(QPainter::Antialiasing);

    auto series = current_chart->get_series();
    connect(series, &QSplineSeries::hovered, this, &Widget::toolTip);
}

void Widget::chart_tooltip(Callout *&tooltip, Chart *chart, QPointF point, bool state) {
    if (tooltip == nullptr) {
        tooltip = new Callout(chart);
    }
    if (state) {
        tooltip->setText(QString("X: %1 \nY: %2 ").arg(point.x()).arg(point.y()));
        tooltip->setAnchor(point);
        tooltip->setZValue(11);
        tooltip->updateGeometry();
        tooltip->show();
    } else {
        tooltip->hide();
    }
}
