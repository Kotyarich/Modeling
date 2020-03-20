#include "widget.h"
#include "../callout.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget),
    curr_tooltip(nullptr),
    resist_tooltip(nullptr),
    cap_volt_tooltip(nullptr),
    lamp_volt_tooltip(nullptr),
    temp_tooltip(nullptr) {
    ui->setupUi(this);

    init_current_chart();
    init_resistance_chart();
    init_cap_voltage_chart();
    init_temperature_chart();
    init_lamp_voltage_chart();

    this->setMouseTracking(true);
}

Widget::~Widget() {
    delete ui;
}

void Widget::current_tooltip(QPointF point, bool state) {
    chart_tooltip(curr_tooltip,
                  static_cast<Chart*>(ui->current_view->chart()),
                  point, state);
}

void Widget::resistance_tooltip(QPointF point, bool state) {
    chart_tooltip(resist_tooltip,
                  static_cast<Chart*>(ui->resistance_view->chart()),
                  point, state);
}

void Widget::cap_voltage_tooltip(QPointF point, bool state) {
    chart_tooltip(cap_volt_tooltip,
                  static_cast<Chart*>(ui->cap_voltage_view->chart()),
                  point, state);
}

void Widget::lamp_voltage_tooltip(QPointF point, bool state) {
    chart_tooltip(lamp_volt_tooltip,
                  static_cast<Chart*>(ui->lamp_voltage_view->chart()),
                  point, state);
}

void Widget::temperature_tooltip(QPointF point, bool state) {
    chart_tooltip(temp_tooltip,
                  static_cast<Chart*>(ui->temp_view->chart()),
                  point, state);
}

void Widget::count() {
    auto lk = ui->inductivity_inp->value();
    auto ck = ui->cap_inp->value();
    auto rk = ui->resistance_inp->value();
    auto uc0 = ui->voltage_inp->value();
    auto i0 = ui->current_inp->value();

    Model model(lk, ck, rk, uc0, i0, runge_kutt_2nd_iteration);
    model.start_modeling();

    Model model_4th(lk, ck, rk, uc0, i0, runge_kutt_iteration);
    model_4th.start_modeling();

    auto curr_chart = static_cast<Chart*>(ui->current_view->chart());
    curr_chart->add_values(model_4th.get_time_vec(), model_4th.get_cur_vec());
    curr_chart->add_series(model.get_time_vec(), model.get_cur_vec());

    auto resistance_chart = static_cast<Chart*>(ui->resistance_view->chart());
    resistance_chart->add_values(model.get_time_vec(), model.get_rp_vec());
    resistance_chart->add_series(model_4th.get_time_vec(), model_4th.get_rp_vec());

    auto cap_volt_chart = static_cast<Chart*>(ui->cap_voltage_view->chart());
    cap_volt_chart->add_values(model.get_time_vec(), model.get_uc_vec());
    cap_volt_chart->add_series(model_4th.get_time_vec(), model_4th.get_uc_vec());

    auto lamp_volt_chart = static_cast<Chart*>(ui->lamp_voltage_view->chart());
    lamp_volt_chart->add_values(model.get_time_vec(), model.get_lamp_vec());
    lamp_volt_chart->add_series(model_4th.get_time_vec(), model_4th.get_lamp_vec());

    auto temp_chart = static_cast<Chart*>(ui->temp_view->chart());
    temp_chart->add_values(model.get_time_vec(), model.get_t0_vec());   
    temp_chart->add_series(model_4th.get_time_vec(), model_4th.get_t0_vec());
}

void Widget::init_current_chart() {
    auto current_chart = new Chart;

    auto axis_y = current_chart->axes(Qt::Vertical).first();
    auto axis_x = current_chart->axes(Qt::Horizontal).first();
    axis_y->setTitleText("I, A");
    axis_x->setTitleText("t, mks");

    ui->current_view->setChart(current_chart);
    ui->current_view->setRenderHint(QPainter::Antialiasing);

    auto series = current_chart->get_series();
    auto series2 = current_chart->get_series2();
    connect(series, &QSplineSeries::hovered, this, &Widget::current_tooltip);
    connect(series2, &QSplineSeries::hovered, this, &Widget::current_tooltip);
}

void Widget::init_resistance_chart() {
    auto resistance_chart = new Chart;

    auto axis_y = resistance_chart->axes(Qt::Vertical).first();
    auto axis_x = resistance_chart->axes(Qt::Horizontal).first();
    axis_y->setTitleText("Rp, Om");
    axis_x->setTitleText("t, mks");

    ui->resistance_view->setChart(resistance_chart);
    ui->resistance_view->setRenderHint(QPainter::Antialiasing);

    auto series = resistance_chart->get_series();
    auto series2 = resistance_chart->get_series2();
    connect(series, &QSplineSeries::hovered, this, &Widget::resistance_tooltip);
    connect(series2, &QSplineSeries::hovered, this, &Widget::resistance_tooltip);
}

void Widget::init_cap_voltage_chart() {
    auto cap_voltage_chart = new Chart;

    auto axis_y = cap_voltage_chart->axes(Qt::Vertical).first();
    auto axis_x = cap_voltage_chart->axes(Qt::Horizontal).first();
    axis_y->setTitleText("Uc, V");
    axis_x->setTitleText("t, mks");

    ui->cap_voltage_view->setChart(cap_voltage_chart);
    ui->cap_voltage_view->setRenderHint(QPainter::Antialiasing);

    auto series = cap_voltage_chart->get_series();
    auto series2 = cap_voltage_chart->get_series2();
    connect(series, &QSplineSeries::hovered, this, &Widget::cap_voltage_tooltip);
    connect(series2, &QSplineSeries::hovered, this, &Widget::cap_voltage_tooltip);
}

void Widget::init_lamp_voltage_chart() {
    auto lamp_voltage_chart = new Chart;

    auto axis_y = lamp_voltage_chart->axes(Qt::Vertical).first();
    auto axis_x = lamp_voltage_chart->axes(Qt::Horizontal).first();
    axis_y->setTitleText("Up, V");
    axis_x->setTitleText("t, mks");

    ui->lamp_voltage_view->setChart(lamp_voltage_chart);
    ui->lamp_voltage_view->setRenderHint(QPainter::Antialiasing);

    auto series = lamp_voltage_chart->get_series();
    auto series2 = lamp_voltage_chart->get_series();
    connect(series, &QSplineSeries::hovered, this, &Widget::lamp_voltage_tooltip);
    connect(series2, &QSplineSeries::hovered, this, &Widget::lamp_voltage_tooltip);
}

void Widget::init_temperature_chart() {
    auto temperature_chart = new Chart;

    auto axis_y = temperature_chart->axes(Qt::Vertical).first();
    auto axis_x = temperature_chart->axes(Qt::Horizontal).first();
    axis_y->setTitleText("T, K");
    axis_x->setTitleText("t, mks");

    ui->temp_view->setChart(temperature_chart);
    ui->temp_view->setRenderHint(QPainter::Antialiasing);

    auto series = temperature_chart->get_series();
    auto series2 = temperature_chart->get_series();
    connect(series, &QSplineSeries::hovered, this, &Widget::temperature_tooltip);
    connect(series2, &QSplineSeries::hovered, this, &Widget::temperature_tooltip);
}

void Widget::chart_tooltip(Callout *&tooltip, Chart *chart,
                           QPointF point, bool state) {
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
