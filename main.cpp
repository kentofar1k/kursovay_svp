#include <QApplication>
#include "qcustomplot.h"
#include <QFileDialog>
#include <fstream>
#include <iostream>
#include <vector>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // Открытие диалогового окна выбора файла
    QString fileName = QFileDialog::getOpenFileName(nullptr, "Выберите файл", "", "Текстовые файлы (*.txt)");

    if (!fileName.isEmpty()) {
        // Загрузка данных из файла
        std::ifstream file(fileName.toStdString());

        std::vector<double> signalData;
        double value;
        while (file >> value) {
            signalData.push_back(value);
        }

        // Определение периода сигнала
        int period = 0;
        for (unsigned int i = 1; i < signalData.size(); ++i) {
            if (signalData[i] == signalData[0]) { // Предполагаем, что периодический участок начинается с первого элемента
                period = i;
                break;
            }
        }

        // Вывод графика сигнала
        QCustomPlot customPlot;
        QVector<double> x(signalData.size()), y(signalData.size()),x1(signalData.size()), y1(signalData.size());
        for ( int i = 0; i < period; ++i) {
            x[i] = i;
            y[i] = signalData[i];
        }
        for (unsigned int i = period; i < signalData.size(); ++i) {
            x[i] = i;
            y1[i] = signalData[i];
        }
        customPlot.addGraph();
        customPlot.graph(0)->setData(x, y);
        customPlot.addGraph();
        customPlot.graph(1)->setData(x, y1);


        if (period != 0) {
            customPlot.graph(0)->setScatterStyle(QCPScatterStyle::ssDisc);
            customPlot.graph(1)->setScatterStyle(QCPScatterStyle::ssDisc);
            customPlot.graph(0)->setLineStyle(QCPGraph::lsLine);
            customPlot.graph(1)->setLineStyle(QCPGraph::lsLine);
            customPlot.graph(0)->setPen(QPen(Qt::red));
            customPlot.graph(1)->setPen(QPen(Qt::green));
            customPlot.graph(0)->setBrush(QBrush(QColor(255, 0, 0, 50)));
            customPlot.graph(0)->setAdaptiveSampling(true);
                        customPlot.graph(1)->setAdaptiveSampling(true);
            customPlot.graph(0)->rescaleAxes();
            customPlot.graph(1)->rescaleAxes();
        }

        customPlot.xAxis->setLabel("Time");
        customPlot.yAxis->setLabel("Amplitude");
        customPlot.axisRect()->setupFullAxesBox(true);


           customPlot.show();

        return QApplication::exec();
    } else {
        return 0;
    }
}
