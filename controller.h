/**********************************************************************************
    ForecastingSAT version 1.0. Calculation of the position and angles
    of near space satellites
    Copyright (C) 2020  Maxim Shershavikov

    This file is part of ForecastingSAT version 1.0.

    ForecastingSAT v1.0 is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    ForecastingSAT v1.0 is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.

    Email m.shershavikov@yandex.ru
    To read a copy of the GNU General Public License in a file COPYING.txt,
    to do this, click the AbautProgram button.
**********************************************************************************/

#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QObject>
#include <QString>
#include <fstream>
#include <string>
#include <QThread>
#include "nearspaseforecasting.h"

class Controller : public QThread
{
    Q_OBJECT
public:
    std::string tlepath;
    std::string openpath;
    std::string savepath;

    Controller(std::string tlepath, std::string openpath, std::string savepath);

signals:
    void tomessagedlg_mes(QString message);
    void tomessagedlg_error(QString error);
    void toprogressbarvisible(bool visible);
    void count(int count_next);
    void processflag(bool flag);
    void thread_del();

public slots:
    void startlanlot(int mode, int from, QString lat, QString lon, QString date_start, QString date_end, QString slat, QString slon);
    void starttxt(int mode);

private:
    double sector_lat = 1.5;
    double sector_lon = 2.5;

    int day_month[13] = { 0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

    const int daysmonthfrombeginyear[2][13] =
        { { 0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334, 365 },
        { 0, 31, 60, 91, 121, 152, 182, 213, 244, 274, 305, 335, 366 } };
};

#endif
