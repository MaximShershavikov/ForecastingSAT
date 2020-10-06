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

#ifndef HUB_H
#define HUB_H

#include <QObject>
#include <QString>
#include <qqml.h>
#include <fstream>
#include <string>
#include <QThread>
#include "nearspaseforecasting.h"
#include "controller.h"

class Hub : public QObject
{
    Q_OBJECT

public:
    explicit Hub(QObject *parent = nullptr);

signals:
    void totextfield_tle(QString open_tle);
    void totextfield_open(QString open_txt);
    void totextfield_save(QString save_txt);
    void tomessagedialog_mes(QString message);
    void tomessagedialog_error(QString error);
    void toprogressbar_visible(bool visible);
    void toflag(bool flag);
    void count(int count_next);

    void start_lan_lot(int mode, int from, QString lat, QString lon, QString date_start, QString date_end, QString slat, QString slon);
    void start_txt(int mode);

public slots:
    void pathfile_tle(QString open_tle);
    void pathfile_open(QString open_txt);
    void pathfile_save(QString save_txt);

    void FromMessageDlgMes(QString mes);
    void FromMessageDlgErr(QString err);
    void FromCount(int next_count);
    void FromProgressBarVisible(bool vi);
    void FromFlag(bool fl);
    void ThreadDel();

    void startmain(int mode, int from, QString lat, QString lon, QString date_start, QString date_end, QString slat, QString slon);

private:
    std::string tlepath;
    std::string openpath;
    std::string savepath;

    QThread *thread;
    Controller *controller;
};

#endif
