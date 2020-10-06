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

#include "hub.h"

Hub::Hub(QObject *parent) : QObject(parent) { }

void Hub::pathfile_tle(QString open_tle)
{
    open_tle.remove(0, 8);
    for (int i = 0; i < open_tle.size(); i++)
        if (open_tle[i] == '/') open_tle[i] = '\\';
    tlepath = open_tle.toStdString();
    emit totextfield_tle(open_tle);
}

void Hub::pathfile_open(QString open_txt)
{
    open_txt.remove(0, 8);
    for (int i = 0; i < open_txt.size(); i++)
        if (open_txt[i] == '/') open_txt[i] = '\\';
    openpath = open_txt.toStdString();
    emit totextfield_open(open_txt);
}

void Hub::pathfile_save(QString save_txt)
{
    save_txt.remove(0, 8);
    for (int i = 0; i < save_txt.size(); i++)
        if (save_txt[i] == '/') save_txt[i] = '\\';
    if (save_txt[save_txt.size()-1] != 't' || save_txt[save_txt.size()-2] != 'x'
        || save_txt[save_txt.size()-3] != 't' || save_txt[save_txt.size()-4] != '.')
    {
        save_txt += ".txt";
    }
    savepath = save_txt.toStdString();
    emit totextfield_save(save_txt);
}

void Hub::FromMessageDlgMes(QString mes)
{
    emit tomessagedialog_mes(mes);
}

void Hub::FromMessageDlgErr(QString err)
{
    emit tomessagedialog_error(err);
}

void Hub::FromCount(int next_count)
{
    emit count(next_count);
}

void Hub::FromProgressBarVisible(bool vi)
{
    emit toprogressbar_visible(vi);
}

void Hub::FromFlag(bool fl)
{
    emit toflag(fl);
}

void Hub::ThreadDel()
{
    delete controller;
    thread->terminate();
}

void Hub::startmain(int mode, int from, QString lat, QString lon, QString date_start, QString date_end, QString slat, QString slon)
{
    controller = new Controller(tlepath, openpath, savepath);
    switch (mode)
    {
    case 0:
    {
        thread = new QThread;
        controller->moveToThread(thread);
        connect(this, SIGNAL(start_txt(int)), controller, SLOT(starttxt(int)), Qt::AutoConnection);
        connect(controller, SIGNAL(tomessagedlg_mes(QString)), this, SLOT(FromMessageDlgMes(QString)), Qt::AutoConnection);
        connect(controller, SIGNAL(tomessagedlg_error(QString)), this, SLOT(FromMessageDlgErr(QString)), Qt::AutoConnection);
        connect(controller, SIGNAL(toprogressbarvisible(bool)), this, SLOT(FromProgressBarVisible(bool)), Qt::AutoConnection);
        connect(controller, SIGNAL(count(int)), this, SLOT(FromCount(int)), Qt::AutoConnection);
        connect(controller, SIGNAL(processflag(bool)), this, SLOT(FromFlag(bool)), Qt::AutoConnection);
        connect(controller, SIGNAL(thread_del()), this, SLOT(ThreadDel()), Qt::AutoConnection);
        thread->start();
        emit start_txt(mode);
    }
    break;
    case 1:
    {
        thread = new QThread;
        controller->moveToThread(thread);
        connect(this, SIGNAL(start_lan_lot(int, int, QString, QString, QString, QString, QString, QString)), controller,
                SLOT(startlanlot(int, int, QString, QString, QString, QString, QString, QString)), Qt::AutoConnection);
        connect(controller, SIGNAL(tomessagedlg_mes(QString)), this, SLOT(FromMessageDlgMes(QString)), Qt::AutoConnection);
        connect(controller, SIGNAL(tomessagedlg_error(QString)), this, SLOT(FromMessageDlgErr(QString)), Qt::AutoConnection);
        connect(controller, SIGNAL(toprogressbarvisible(bool)), this, SLOT(FromProgressBarVisible(bool)), Qt::AutoConnection);
        connect(controller, SIGNAL(count(int)), this, SLOT(FromCount(int)), Qt::AutoConnection);
        connect(controller, SIGNAL(processflag(bool)), this, SLOT(FromFlag(bool)), Qt::AutoConnection);
        connect(controller, SIGNAL(thread_del()), this, SLOT(ThreadDel()), Qt::AutoConnection);
        thread->start();
        emit start_lan_lot(mode, from, lat, lon, date_start, date_end, slat, slon);
    }
    break;
    }
}
