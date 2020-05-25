#include "widget.h"
#include "ui_widget.h"
#include <iostream>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget) {
    ui->setupUi(this);

    init_chart();
}

Widget::~Widget() {
    delete ui;
}

void Widget::count() {  
    Model model;
    auto result = model.count_T();

    auto curr_chart = static_cast<Chart*>(ui->chartView->chart());
    for (auto i = 0ul, step = 1ul; i < result.second.size(); step *= 2, i += step) {
        auto label = QString("time = ") + QString::number(result.second[i].first);
        curr_chart->add_values(result.first, result.second[i].second, label);
    }

    auto time_chart = static_cast<Chart*>(ui->chartView_2->chart());
    auto times = form_time_array(result);
    auto temp_from_time = form_temp_from_time_values(result);
    for (auto i = 0ul; i < temp_from_time.size(); i++) {
        auto label = QString("len = ") + QString::number(i);
        time_chart->add_values(times, temp_from_time[i], label);
    }
}

void Widget::init_chart() {
    auto t_from_x_chart = new Chart;
    auto t_from_time_chart = new Chart;

    auto axis_y = t_from_x_chart->axes(Qt::Vertical).first();
    auto axis_x = t_from_x_chart->axes(Qt::Horizontal).first();
    axis_y->setTitleText("T");
    axis_x->setTitleText("x");

    auto t_from_time_y = t_from_time_chart->axes(Qt::Vertical).first();
    auto time_axis = t_from_time_chart->axes(Qt::Horizontal).first();
    t_from_time_y->setTitleText("T");
    time_axis->setTitleText("time");

    ui->chartView->setChart(t_from_x_chart);
    ui->chartView->setRenderHint(QPainter::Antialiasing);
    ui->chartView_2->setChart(t_from_time_chart);
    ui->chartView_2->setRenderHint(QPainter::Antialiasing);
}

std::vector<double> Widget::form_time_array(const ModelResult &results) const {
    std::vector<double> times(results.second.size());
    auto get_time = [](std::pair<double, std::vector<double>> item){return item.first;};
    std::transform(results.second.begin(), results.second.end(), times.begin(), get_time);
    return times;
}

std::vector<std::vector<double>> Widget::form_temp_from_time_values(const ModelResult &results) const {
    std::vector<std::vector<double>> result(11);
    auto step = static_cast<unsigned long>(1. / (results.first[1] - results.first[0]));

    for (auto i = 0ul; i <= 10; i++) {
        for (auto &item: results.second) {
            result[i].push_back(item.second[i * step]);
        }
    }

    return result;
}
