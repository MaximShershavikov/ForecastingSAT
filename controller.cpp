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

#include "controller.h"

Controller::Controller(std::string TlePath, std::string OpenPath, std::string SavePath) : tlepath(TlePath), openpath(OpenPath), savepath(SavePath) { }

void Controller::starttxt(int mode)
{
    std::ifstream openfiletle, openfiletxt;
    std::ofstream savefile;
    std::string name, ferstline, secondline, res, time;
    double count_next = 0.0, co = 0.0;

    NearSpaseForecasting *nrspasefor;

    openfiletle.exceptions(std::ifstream::badbit | std::ifstream::failbit);
    openfiletxt.exceptions(std::ifstream::badbit | std::ifstream::failbit);
    savefile.exceptions(std::ofstream::badbit | std::ofstream::failbit);

    try
    {
        openfiletle.open(tlepath);
        std::getline(openfiletle, name);
        name += '\n';
        std::getline(openfiletle, ferstline);
        std::getline(openfiletle, secondline);
        openfiletle.close();

        nrspasefor = new NearSpaseForecasting(ferstline, secondline);
        if (nrspasefor->err == true)
        {
            emit tomessagedlg_error(QString::fromStdString(nrspasefor->error));
            return;
        }

        savefile.open(savepath);
        savefile << name;
        openfiletxt.open(openpath);

        openfiletxt.seekg(0, std::ios::end);
        co = (openfiletxt.tellg() / 21);
        if (co == 0) throw QString("Empty file").toStdString();
        co = 100 / co;
        openfiletxt.seekg(0, std::ios::beg);
        emit toprogressbarvisible(true);
        while (!openfiletxt.eof())
        {
            std::getline(openfiletxt, time);
            if (time == "") continue;
            res = nrspasefor->FindPositionSPG4(time, 0, 0, 0, 0, 0, 0, mode);
            res += '\n';
            savefile << res;
            count_next += co;
            if (count_next < 100.0) emit count(static_cast<int>(count_next));
        }
        emit count(100);
        savefile.close();
        delete nrspasefor;
        openfiletxt.close();

        emit tomessagedlg_mes("Calculation of coordinates completed");
        emit processflag(false);
        emit thread_del();
    }
    catch (const std::exception &ex)
    {
        emit tomessagedlg_error("Error reading or creating file. Check the location of the file, the file may not exist");
        emit processflag(false);
        emit thread_del();
    }
    catch (const std::string &err)
    {
        emit tomessagedlg_error(QString::fromStdString(err));
        emit processflag(false);
        emit thread_del();
    }
}

void Controller::startlanlot(int mode, int from, QString lat, QString lon, QString date_start, QString date_end, QString slat, QString slon)
{
    std::ifstream openfiletle;
    std::ofstream savefile;
    std::string name, ferstline, secondline, res, rezerv, da_start, da_end;
    double longi, lati;
    int year_start, month_start, day_start;
    int year_end, month_end, day_end;
    int hour_start = 0, min_start = 0, sec_start = 0;
    double salon, sblon, salat, sdlat;
    double min = 0.0;
    double co = 0.0;
    double count_next = 0.0;
    bool y = 0;
    NearSpaseForecasting *nrspasefor;

    openfiletle.exceptions(std::ifstream::badbit | std::ifstream::failbit);
    savefile.exceptions(std::ofstream::badbit | std::ofstream::failbit);

    try
    {
        if (lon == "") throw QString("Longitude not entered").toStdString();
        if (lat == "") throw QString("Latitude not entered").toStdString();
        if (date_start == "") throw QString("Start date not entered").toStdString();
        if (date_end == "") throw QString("End date not entered").toStdString();

        da_start = date_start.toStdString();
        da_end = date_end.toStdString();

        longi = lon.toDouble();
        lati = lat.toDouble();

        if (longi < -180.0 || longi > 180.0) throw QString("Out of normal longitude range").toStdString();
        if (lati < -90.0 || lati > 180.0) throw QString("Out of normal latitude range").toStdString();

        if (da_start.size() != 10) throw QString("Date error").toStdString();
        if (da_end.size() != 10) throw QString("Date error").toStdString();

        if (da_start[2] == '.') da_start[2] = '\0';
        else throw QString("Date error").toStdString();
        if (da_start[5] == '.') da_start[5] = '\0';
        else throw QString("Date error").toStdString();

        if (da_end[2] == '.') da_end[2] = '\0';
        else throw QString("Date error").toStdString();
        if (da_end[5] == '.') da_end[5] = '\0';
        else throw QString("Date error").toStdString();

        day_start = std::atoi(da_start.c_str());
        month_start = std::atoi(da_start.c_str() + 3);
        year_start = std::atoi(da_start.c_str() + 6);

        day_end = std::atoi(da_end.c_str());
        month_end = std::atoi(da_end.c_str() + 3);
        year_end = std::atoi(da_end.c_str() + 6);

        if (month_start > 12 || month_start < 1) throw QString("The start month is not entered correctly").toStdString();
        if (month_end > 12 || month_end < 1) throw QString("End month entered incorrectly").toStdString();

        if ((year_start % 4) == 0) {
            day_month[2] = 29;
            if (day_start > day_month[month_start] || day_start <= 0) throw QString("The start day is not entered correctly").toStdString();
        }
        else {
            day_month[2] = 28;
            if (day_start > day_month[month_start] || day_start <= 0) throw QString("The start day is not entered correctly").toStdString();
        }

        if ((year_end % 4) == 0) {
            day_month[2] = 29;
            if (day_end > day_month[month_end] || day_end <= 0) throw QString("End day entered incorrectly").toStdString();
        }
        else {
            day_month[2] = 28;
            if (day_end > day_month[month_end] || day_end <= 0) throw QString("End day entered incorrectly").toStdString();
        }

        if (year_end < year_start) throw QString("End year is less than start year").toStdString();
        if (year_start == year_end && month_end < month_start)
            throw QString("End month less than start").toStdString();
        if (year_start == year_end && month_end == month_start && day_end < day_start)
            throw QString("End day is less than start day").toStdString();

        switch (from)
        {
        case 0:
        {
            salat = lati + sector_lat;
            salon = longi - sector_lon;
            sblon = longi + sector_lon;
            sdlat = lati - sector_lat;
        }
        break;
        case 1:
        {
            if (slat == "") throw QString("Latitude offset not entered").toStdString();
            if (slon == "") throw QString("Longitude offset not entered").toStdString();
            salat = lati + slat.toDouble();
            salon = longi - slon.toDouble();
            sblon = longi + slon.toDouble();
            sdlat = lati - slat.toDouble();
        }
        break;
        }

        openfiletle.open(tlepath);
        std::getline(openfiletle, name);
        name += '\n';
        std::getline(openfiletle, ferstline);
        std::getline(openfiletle, secondline);
        openfiletle.close();

        nrspasefor = new NearSpaseForecasting(ferstline, secondline);
        if (nrspasefor->err == true) throw nrspasefor->error;

        savefile.open(savepath);
        savefile << name;

        nrspasefor->latearth = lati;
        nrspasefor->lonearth = longi;

        for (int i = year_start; i <= year_end; i++)
        {
            if ((year_start % 4) == 0) {
                day_month[2] = 29;
                y = 1;
            }
            else {
                day_month[2] = 28;
                y = 0;
            }
            if (i == year_start && i < year_end)
                co += (daysmonthfrombeginyear[y][12] - daysmonthfrombeginyear[y][month_start]) + (day_month[month_start] - day_start);
            else if (i == year_start && i == year_end)
                co = ((day_month[month_start] - day_start) + day_end) + (daysmonthfrombeginyear[y][month_end-1] - daysmonthfrombeginyear[y][month_start]);
            else if (i < year_end)
                co += daysmonthfrombeginyear[y][12];
            else if (i == year_end)
                co += daysmonthfrombeginyear[y][month_end-1] + day_end;
        }
        co = 100 / (co * 24);

        emit toprogressbarvisible(true);
        do
        {
            if ((year_start % 4) == 0) day_month[2] = 29;
            else day_month[2] = 28;
            do
            {
                do
                {
                    do
                    {
                        do
                        {
                            do
                            {
                                res = nrspasefor->FindPositionSPG4("", day_start, month_start, year_start, hour_start, min_start, sec_start, mode);
                                res += '\n';
                                if (nrspasefor->lat_deg <= salat && nrspasefor->lat_deg >= sdlat && nrspasefor->lon_deg <= sblon && nrspasefor->lon_deg >= salon)
                                {
                                    savefile << res;
                                    if (min < nrspasefor->Height_angle_SC_grad)
                                    {
                                        min = nrspasefor->Height_angle_SC_grad;
                                        rezerv = res;
                                    }
                                }
                                sec_start++;
                            } while (sec_start <= 59);
                            sec_start = 0;
                            min_start++;
                        } while (min_start <= 59);
                        min_start = 0;
                        hour_start++;
                        count_next += co;
                        if (count_next < 100.0) emit count(static_cast<int>(count_next));
                     } while (hour_start <= 23);
                     hour_start = 0;
                     day_start++;
                } while ((day_start <= day_end || month_start < month_end || year_start < year_end) && day_start <= day_month[month_start]);
                day_start = 1;
                month_start++;
             } while ((month_start <= month_end || year_start < year_end) && month_start <= 12);
             month_start = 1;
             year_start++;
        } while (year_start <= year_end);
        res = "\nMinimum elevation angle:\n";
        savefile << res;
        savefile << rezerv;
        emit count(100);
        savefile.close();
        delete nrspasefor;

        emit tomessagedlg_mes("Calculation completed");
        emit processflag(false);
        emit thread_del();
    }
    catch (const std::exception &ex)
    {
        emit tomessagedlg_error("Error reading or creating file. Check the location of the file, the file may not exist");
        emit processflag(false);
        emit thread_del();
    }
    catch (const std::string &err)
    {
        emit tomessagedlg_error(QString::fromStdString(err));
        emit processflag(false);
        emit thread_del();
    }
}
