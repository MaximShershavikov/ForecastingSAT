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

#ifndef NEARSPASEFORECASTING_H
#define NEARSPASEFORECASTING_H

#include <string>
#include <math.h>

class NearSpaseForecasting
{
private:
    const double pi = 3.14159265358979323846264338327950288419716939937510582;
    const double min_radius_erch_wgs84 = 6356752.314;
    const double max_radius_erch_wgs84 = 6378137.0;
    const double max_radius_erch_km = 6378.137;
    const double radius_erch_km = 6378.135;
    const double full_angle = 360.0;
    const double half_circle = 180.0;

    struct DataFromTLE
    {
        double meananomoly = 0.0;
        double ascendingnodelon = 0.0;
        double pericenterargument = 0.0;
        double eccentricity = 0.0;
        double inclination = 0.0;
        double meanmotion = 0.0;
        double brakingratio_B = 0.0;
        long long timeepocha = 0;
    };

    struct ValueNearSpace
    {
        double cc5 = 0.0;
        double omgcoef = 0.0;
        double xmcoef = 0.0;
        double del_meananomoly = 0.0;
        double sin_meananomoly = 0.0;
        double d2 = 0.0;
        double d3 = 0.0;
        double d4 = 0.0;
        double t3coef = 0.0;
        double t4coef = 0.0;
        double t5coef = 0.0;
    };

    DataFromTLE* datafromtle = new DataFromTLE;
    ValueNearSpace* valnearsp = new ValueNearSpace;

    double outside_semimajoraxis = 0.0;
    double outside_calc_meanmotion = 0.0;
    double outside_perigee = 0.0;
    double outside_period = 0.0;

    double outside_cos_inclination = 0.0;
    double outside_sin_inclination = 0.0;
    double outside_eta = 0.0;
    double outside_t2coef = 0.0;
    double outside_x1mth2 = 0.0;
    double outside_x3thm1 = 0.0;
    double outside_x7thm1 = 0.0;
    double outside_ayc = 0.0;
    double outside_xlc = 0.0;
    double outside_xnodecoef = 0.0;
    double outside_cc1 = 0.0;
    double outside_cc4 = 0.0;
    double outside_argpdot = 0.0;
    double outside_xnodedot = 0.0;
    double outside_xmdot = 0.0;

    const int daysmonth[2][13] = {
        { 0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 },
        { 0, 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 } };

    const int daysmonthfrombeginyear[2][13] = {
        { 0, 0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334 },
        { 0, 0, 31, 60, 91, 121, 152, 182, 213, 244, 274, 305, 335 } };
public:
    double lat_deg = 0.0;
    double lon_deg = 0.0;
    double latearth = 0.0;
    double lonearth = 0.0;
    double Height_angle_SC_grad = 0.0;
    bool err = false;
    std::string error;

    NearSpaseForecasting(std::string ferstline, std::string seckondline);
    std::string AzimuthAndAngleSun(int day, int month, int year, int hour, int min);
    std::string AzimutAndDistance(double lat_sat, double lon_sat);
    std::string FindPositionSPG4(std::string dataandtime, int day, int month, int year, int hour, int min, int sec, int mode);
    std::string SatAnglesAndDistances(double theta, double x, double y, double z, double altitude);
};

#endif
