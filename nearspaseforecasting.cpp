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

#include "nearspaseforecasting.h"

NearSpaseForecasting::NearSpaseForecasting(std::string ferstline, std::string seckondline)
{
    int year = 0, month = 0, day = 0, hour = 0, min = 0, dayoftheyear = 0, i = 1, j = 0, inttmp = 0;
    double tmp = 0.0, epoch = 0.0, sec = 0.0, pinvsq = 0.0, tmp_1 = 0.0, cc3 = 0.0, s4 = 0.0;
    double a = 0.0, bta = 0.0, del = 0.0, b = 0.0, tsi = 0.0, tmp_2 = 0.0, qoms24 = 0.0;
    std::string textval;

    ferstline[8] = '\0';
    ferstline[17] = '\0';
    ferstline[32] = '\0';
    ferstline[43] = '\0';
    ferstline[52] = '\0';
    ferstline[61] = '\0';
    ferstline[63] = '\0';

    for (auto it = ferstline.begin() + 53; it != ferstline.begin() + 61; it++)
    {
        if (it == ferstline.begin() + 53)
        {
            if (*it == '-') textval += *it;
            textval += '0';
            textval += '.';
        }
        else if (it == (ferstline.begin() + 59))
        {
            textval += 'e';
            textval += *it;
        }
        else textval += *it;
    }

    datafromtle->brakingratio_B = std::atof(textval.c_str());
    textval.clear();
    textval += ferstline[18];
    textval += ferstline[19];
    year = std::atof(textval.c_str());
    textval.clear();

    if (year < 57) year += 2000;
    else year += 1900;

    epoch = std::atof(ferstline.c_str() + 20);
    dayoftheyear = (int)floor(epoch);
    if ((year % 4) == 0) j = 1;
    while ((dayoftheyear > inttmp + daysmonth[j][i]) && (i < 12))
    {
        inttmp = inttmp + daysmonth[j][i];
        i++;
    }
    month = i;
    day = dayoftheyear - inttmp;
    tmp = (epoch - dayoftheyear) * 24.0;
    hour = (int)floor(tmp);
    tmp = (tmp - hour) * 60.0;
    min = (int)floor(tmp);
    sec = (tmp - min) * 60.0;

    datafromtle->timeepocha = (dayoftheyear - 1 + 365 * (year - 1) + (year - 1) / 4 - (year - 1) / 100 + (year - 1) / 400) * 86400000000LL + (hour * 3600LL + min * 60LL + sec) * 1000000LL;

    seckondline[7] = '\0';
    seckondline[16] = '\0';
    seckondline[25] = '\0';
    seckondline[33] = '\0';
    seckondline[42] = '\0';
    seckondline[51] = '\0';

    datafromtle->meananomoly = static_cast<double>(std::atof(seckondline.c_str() + 43)) * pi / half_circle;
    datafromtle->ascendingnodelon = static_cast<double>(std::atof(seckondline.c_str() + 17)) * pi / half_circle;
    datafromtle->pericenterargument = static_cast<double>(std::atof(seckondline.c_str() + 34)) * pi / half_circle;
    textval = "0.";
    for (int i = 26; i <= 32; i++) textval += seckondline[i];
    datafromtle->eccentricity = static_cast<double>(std::atof(textval.c_str()));
    textval.clear();
    datafromtle->inclination = static_cast<double>(std::atof(seckondline.c_str() + 8)) * pi / half_circle;
    for (int i = 52; i <= 62; i++) textval += seckondline[i];
    datafromtle->meanmotion = static_cast<double>(std::atof(textval.c_str())) * (2.0 * pi) / 1440.0;
    textval.clear();

    a = pow((60.0 / sqrt(radius_erch_km * radius_erch_km * radius_erch_km / 398600.8)) / datafromtle->meanmotion, 2.0 / 3.0);
    outside_cos_inclination = cos(datafromtle->inclination);
    outside_x3thm1 = 3.0 * outside_cos_inclination * outside_cos_inclination - 1.0;
    bta = 1.0 - (datafromtle->eccentricity * datafromtle->eccentricity);
    del = ((1.5 * 0.5 * 1.082616e-3) * outside_x3thm1 / (sqrt(bta) * bta)) / (a * a);
    b = a * (1.0 - del * (1.0 / 3.0 + del * (1.0 + del * 134.0 / 81.0)));

    outside_calc_meanmotion = datafromtle->meanmotion / (1.0 + (((1.5 * 0.5 * 1.082616e-3) * outside_x3thm1 / (sqrt(bta) * bta)) / (b * b)));
    outside_semimajoraxis = b / (1.0 - (((1.5 * 0.5 * 1.082616e-3) * outside_x3thm1 / (sqrt(bta) * bta)) / (b * b)));
    outside_perigee = (outside_semimajoraxis * (1.0 - datafromtle->eccentricity) - 1.0) * radius_erch_km;
    outside_period = (2.0 * pi) / outside_calc_meanmotion;

    if (outside_period >= 225.0)
    {
        err = true;
        error = "Not near space";
        return;
    }

    if (datafromtle->eccentricity < 0.0 || datafromtle->eccentricity > 0.999)
    {
        err = true;
        error = "Error in tle. Eccentricity";
        return;
    }

    if (datafromtle->inclination < 0.0 || datafromtle->inclination > pi)
    {
        err = true;
        error = "Error in tle. Inclination";
        return;
    }

    outside_sin_inclination = sin(datafromtle->inclination);
    outside_x1mth2 = 1.0 - outside_cos_inclination * outside_cos_inclination;
    outside_x7thm1 = 7.0 * outside_cos_inclination * outside_cos_inclination - 1.0;

    if (fabs(outside_cos_inclination + 1.0) > 1.5e-12) outside_xlc = 0.125 * (2.53881e-6 / (0.5 * 1.082616e-3)) * outside_sin_inclination * (3.0 + 5.0 * outside_cos_inclination) / (1.0 + outside_cos_inclination);
    else outside_xlc = 0.125 * (2.53881e-6 / (0.5 * 1.082616e-3)) * outside_sin_inclination * (3.0 + 5.0 * outside_cos_inclination) / 1.5e-12;

    outside_ayc = 0.25 * (2.53881e-6 / (0.5 * 1.082616e-3)) * outside_sin_inclination;
    s4 = 1.0 + 78.0 / radius_erch_km;
    qoms24 = pow(((120.0 - 78.0) / radius_erch_km), 4.0);

    if (outside_perigee < 156.0)
    {
        s4 = outside_perigee - 78.0;
        if (outside_perigee < 98.0)
        {
            s4 = 20.0;
        }
        qoms24 = pow((120.0 - s4) / radius_erch_km, 4.0);
        s4 = s4 / radius_erch_km + 1.0;
    }

    pinvsq = 1.0 / (outside_semimajoraxis * outside_semimajoraxis * bta * bta);
    tsi = 1.0 / (outside_semimajoraxis - s4);

    outside_eta = outside_semimajoraxis * datafromtle->eccentricity * (1.0 / (outside_semimajoraxis - s4));
    outside_cc1 = datafromtle->brakingratio_B * ((qoms24 * pow(tsi, 4.0) / pow(fabs(1.0 - outside_eta * outside_eta), 3.5)) * outside_calc_meanmotion
        * (outside_semimajoraxis * (1.0 + 1.5 * outside_eta * outside_eta + datafromtle->eccentricity * outside_eta * (4.0 + outside_eta * outside_eta))
            + 0.75 * 0.5 * 1.082616e-3 * tsi / fabs(1.0 - outside_eta * outside_eta) * outside_x3thm1 * (8.0 + 3.0 * outside_eta * outside_eta * (8.0 + outside_eta * outside_eta))));

    outside_cc4 = 2.0 * outside_calc_meanmotion * (qoms24 * pow(tsi, 4.0) / pow(fabs(1.0 - outside_eta * outside_eta), 3.5)) * outside_semimajoraxis * bta
        * (outside_eta * (2.0 + 0.5 * outside_eta * outside_eta) + datafromtle->eccentricity * (0.5 + 2.0 * outside_eta * outside_eta)
            - 2.0 * 0.5 * 1.082616e-3 * tsi / (outside_semimajoraxis * fabs(1.0 - outside_eta * outside_eta))
            * (-3.0 * outside_x3thm1 * (1.0 - 2.0 * datafromtle->eccentricity * outside_eta + outside_eta * outside_eta
                * (1.5 - 0.5 * datafromtle->eccentricity * outside_eta)) + 0.75 * outside_x1mth2 * (2.0 * outside_eta * outside_eta - datafromtle->eccentricity
                    * outside_eta * (1.0 + outside_eta * outside_eta)) * cos(2.0 * datafromtle->pericenterargument)));

    tmp_1 = 3.0 * 0.5 * 1.082616e-3 * pinvsq * outside_calc_meanmotion * 0.5 * 1.082616e-3 * pinvsq;
    tmp_2 = 1.25 * -0.375 * -1.65597e-6 * pinvsq * pinvsq * outside_calc_meanmotion;

    outside_xmdot = outside_calc_meanmotion + 0.5 * 3.0 * 0.5 * 1.082616e-3 * pinvsq * outside_calc_meanmotion * sqrt(bta) *
        outside_x3thm1 + 0.0625 * tmp_1 * sqrt(bta) * (13.0 - 78.0 * outside_cos_inclination * outside_cos_inclination
            + 137.0 * outside_cos_inclination * outside_cos_inclination * outside_cos_inclination * outside_cos_inclination);

    outside_argpdot = -0.5 * 3.0 * 0.5 * 1.082616e-3 * pinvsq * outside_calc_meanmotion * (1.0 - 5.0 * outside_cos_inclination * outside_cos_inclination) + 0.0625 * tmp_1 *
        (7.0 - 114.0 * outside_cos_inclination * outside_cos_inclination + 395.0 * outside_cos_inclination * outside_cos_inclination * outside_cos_inclination * outside_cos_inclination) + tmp_2 *
        (3.0 - 36.0 * outside_cos_inclination * outside_cos_inclination + 49.0 * outside_cos_inclination * outside_cos_inclination * outside_cos_inclination * outside_cos_inclination);

    outside_xnodedot = -(3.0 * 0.5 * 1.082616e-3 * pinvsq * outside_calc_meanmotion) * outside_cos_inclination + (0.5 * tmp_1 * (4.0 - 19.0 * outside_cos_inclination * outside_cos_inclination)
        + 2.0 * tmp_2 * (3.0 - 7.0 * outside_cos_inclination * outside_cos_inclination)) * outside_cos_inclination;

    outside_xnodecoef = 3.5 * bta * -(3.0 * 0.5 * 1.082616e-3 * pinvsq * outside_calc_meanmotion) * outside_cos_inclination * outside_cc1;
    outside_t2coef = 1.5 * outside_cc1;

    if (datafromtle->eccentricity > 1.0e-4)
    {
        cc3 = qoms24 * pow(tsi, 4.0) * tsi * (2.53881e-6 / (0.5 * 1.082616e-3)) * outside_calc_meanmotion * outside_sin_inclination / datafromtle->eccentricity;
    }

    valnearsp->cc5 = 2.0 * (qoms24 * pow(tsi, 4.0) / pow(fabs(1.0 - outside_eta * outside_eta), 3.5)) * outside_semimajoraxis * bta *
        (1.0 + 2.75 * (outside_eta * outside_eta + datafromtle->eccentricity * outside_eta) + datafromtle->eccentricity * outside_eta * outside_eta * outside_eta);
    valnearsp->omgcoef = datafromtle->brakingratio_B * cc3 * cos(datafromtle->pericenterargument);

    valnearsp->xmcoef = 0.0;
    if (datafromtle->eccentricity > 1.0e-4)
    {
        valnearsp->xmcoef = -(2.0 / 3.0) * (qoms24 * pow(tsi, 4.0)) * datafromtle->brakingratio_B / (datafromtle->eccentricity * outside_eta);
    }

    valnearsp->del_meananomoly = pow(1.0 + outside_eta * (cos(datafromtle->meananomoly)), 3.0);
    valnearsp->sin_meananomoly = sin(datafromtle->meananomoly);

    if (outside_perigee >= 220.0)
    {
        valnearsp->d2 = 4.0 * outside_semimajoraxis * tsi * outside_cc1 * outside_cc1;
        valnearsp->d3 = (17.0 * outside_semimajoraxis + s4) * (valnearsp->d2 * tsi * outside_cc1 / 3.0);
        valnearsp->d4 = 0.5 * (valnearsp->d2 * tsi * outside_cc1 / 3.0) * outside_semimajoraxis * tsi * (221.0 * outside_semimajoraxis + 31.0 * s4) * outside_cc1;
        valnearsp->t3coef = valnearsp->d2 + 2.0 * outside_cc1 * outside_cc1;
        valnearsp->t4coef = 0.25 * (3.0 * valnearsp->d3 + outside_cc1 * (12.0 * valnearsp->d2 + 10.0 * outside_cc1 * outside_cc1));
        valnearsp->t5coef = 0.2 * (3.0 * valnearsp->d4 + 12.0 * outside_cc1 * valnearsp->d3 + 6.0 * valnearsp->d2 * valnearsp->d2
            + 15.0 * outside_cc1 * outside_cc1 * (2.0 * valnearsp->d2 + outside_cc1 * outside_cc1));
    }
}

std::string NearSpaseForecasting::AzimuthAndAngleSun(int day, int month, int year, int hour, int min)
{
    unsigned int d = 0;
    double longitude_perihelion = 0.0, eccentricity = 0.0, mean_anomaly = 0.0, ecliptic_tilt = 0.0;
    double mean_longitude = 0.0, eccentric_anomaly = 0.0, lon = 0.0, xequat = 0.0, yequat = 0.0, zequat = 0.0;
    double x = 0.0, y = 0.0, z = 0.0, ra = 0.0, declination = 0.0, ut = 0.0, sidereal_time = 0.0, r = 0.0;
    double hour_angle = 0.0, v = 0.0, xhor = 0.0, yhor = 0.0, zhor = 0.0, azimuth_sun = 0.0, angle_sun = 0.0;
    double lat_earth = latearth * pi / half_circle;

    d = 367 * year - floor((7 * (year + (floor((month + 9) / 12)))) / 4) + floor((275 * month) / 9) + day - 730530;
    longitude_perihelion = 282.9404 + 4.70935 * pow(10, -5) * d;
    eccentricity = 0.016709 - 1.151 * pow(10, -9) * d;

    mean_anomaly = 356.0470 + 0.9856002585 * d;
    if (mean_anomaly > 360) mean_anomaly = mean_anomaly - floor(mean_anomaly / full_angle) * full_angle;
    else if (mean_anomaly < 0) mean_anomaly = mean_anomaly + (floor(-mean_anomaly / 360) + 1) * full_angle;
    ecliptic_tilt = 23.4393 - 3.563 * pow(10, -7) * d;

    mean_longitude = longitude_perihelion + mean_anomaly;
    if (mean_longitude > full_angle) mean_longitude = mean_longitude - floor(mean_longitude / full_angle) * full_angle;
    else if (mean_longitude < 0) mean_longitude = mean_longitude + (floor(-mean_longitude / full_angle) + 1) * full_angle;
    eccentric_anomaly = mean_anomaly + (half_circle / pi) * eccentricity * sin(mean_anomaly * pi / half_circle) * (1 + eccentricity * cos(mean_anomaly * pi / half_circle));

    x = cos(eccentric_anomaly * pi / half_circle) - eccentricity;
    y = sin(eccentric_anomaly * pi / half_circle) * sqrt(1 - eccentricity * eccentricity);
    r = sqrt(x * x + y * y);
    v = (half_circle / pi) * atan2(y, x);

    lon = v + longitude_perihelion;
    if (lon > full_angle) lon = lon - floor(lon / full_angle) * full_angle;
    else if (lon < 0) lon = lon + (floor(-lon / full_angle) + 1) * full_angle;
    x = r * cos(lon * pi / half_circle);
    y = r * sin(lon * pi / half_circle);
    xequat = x;
    yequat = y * cos(ecliptic_tilt * pi / half_circle) + z * sin(ecliptic_tilt * pi / half_circle);
    zequat = y * sin(ecliptic_tilt * pi / half_circle) + z * cos(ecliptic_tilt * pi / half_circle);

    ra = (half_circle / pi) * atan2(yequat, xequat);
    declination = (half_circle / pi) * asin(zequat / r);
    ut = hour + static_cast<double>(min) / 60;
    sidereal_time = ((mean_longitude / 15 + 12) + ut + lonearth / 15) - 24 * floor(((mean_longitude / 15 + 12) + ut + lonearth / 15) / 24);

    hour_angle = 15 * (sidereal_time - ra / 15);
    if (hour_angle > full_angle) hour_angle = hour_angle - floor(hour_angle / full_angle) * full_angle;
    else if (hour_angle < 0) hour_angle = hour_angle + (floor(-hour_angle / full_angle) + 1) * full_angle;

    x = cos(hour_angle * pi / half_circle) * cos(declination * pi / half_circle);
    y = sin(hour_angle * pi / half_circle) * cos(declination * pi / half_circle);
    z = sin(declination * pi / half_circle);
    xhor = x * sin(lat_earth) - z * cos(lat_earth);
    yhor = y;
    zhor = x * cos(lat_earth) + z * sin(lat_earth);

    azimuth_sun = atan2(yhor, xhor) * (half_circle / pi) + half_circle;
    if (azimuth_sun > full_angle) azimuth_sun = azimuth_sun - floor(azimuth_sun / full_angle) * full_angle;
    else if (azimuth_sun < 0) azimuth_sun = azimuth_sun + (floor(-azimuth_sun / full_angle) + 1) * full_angle;
    angle_sun = asin(zhor) * (half_circle / pi);

    return "Angle of the sun: " + std::to_string(angle_sun) + " Sun azimuth: " + std::to_string(azimuth_sun);
}

std::string NearSpaseForecasting::AzimutAndDistance(double lat_sat, double lon_sat)
{
    double len = 0.0, len360 = 0.0, len_semimajor_axis = 0.0, len_minor_semiaxis = 0.0;
    double distance = 0.0, azimut = 0.0, spheroid_eccentricity = 0.0, meridional_lat_erch = 0.0;
    double deltalon = 0.0, deltalon360 = 0.0, meridional_lat_sat = 0.0, geoid_lat_sat = 0.0;
    double geoid_lat_erch = 0.0, Az = 0.0, Az360 = 0.0;
    double lat_earth = latearth * pi / half_circle;
    double lon_earth = lonearth * pi / half_circle;

    len_semimajor_axis = max_radius_erch_wgs84 * max_radius_erch_wgs84;
    len_minor_semiaxis = min_radius_erch_wgs84 * min_radius_erch_wgs84;
    spheroid_eccentricity = sqrt((len_semimajor_axis - len_minor_semiaxis) / len_semimajor_axis);

    deltalon = lon_deg - lonearth;
    deltalon360 = (lon_deg + full_angle) - lonearth;
    if (deltalon > half_circle) deltalon -= full_angle;
    if (deltalon360 > half_circle) deltalon360 -= full_angle;

    deltalon = deltalon * pi / half_circle;
    deltalon360 = deltalon360 * pi / half_circle;

    meridional_lat_sat = tan((pi / 4) + (lat_sat / 2));
    meridional_lat_erch = tan((pi / 4) + (lat_earth / 2));

    geoid_lat_sat = pow(((1 - (spheroid_eccentricity * sin(lat_sat))) / (1 + (spheroid_eccentricity * sin(lat_sat)))), spheroid_eccentricity / 2);
    geoid_lat_erch = pow(((1 - (spheroid_eccentricity * sin(lat_earth))) / (1 + (spheroid_eccentricity * sin(lat_earth)))), spheroid_eccentricity / 2);

    Az = atan2(deltalon, log(meridional_lat_sat * geoid_lat_sat) - log(meridional_lat_erch * geoid_lat_erch));
    Az360 = atan2(deltalon360, log(meridional_lat_sat * geoid_lat_sat) - log(meridional_lat_erch * geoid_lat_erch));

    if (lat_deg - latearth)
    {
        len = (((1 - ((spheroid_eccentricity * spheroid_eccentricity) / 4)) * (lat_sat - lat_earth)) - (((3 / 8) *
              (spheroid_eccentricity * spheroid_eccentricity)) * (sin(2 * lat_sat) - sin(2 * lat_earth)))) / cos(Az);
    }
    else if (lon_deg - lonearth)
    {
        len = cos(lat_earth) * abs(lon_sat - lon_earth);
    }
    else len = 0.0;

    if (lat_deg - latearth)
    {
        len360 = (((1 - ((spheroid_eccentricity * spheroid_eccentricity) / 4)) * (lat_sat - lat_earth)) - (((3 / 8) *
                 (spheroid_eccentricity * spheroid_eccentricity)) * (sin(2 * lat_sat) - sin(2 * lat_earth)))) / cos(Az360);
    }
    else if ((lon_deg + full_angle) - lonearth)
    {
        len360 = cos(lat_earth) * abs((lon_sat + (full_angle * pi / half_circle)) - lon_earth);
    }
    else len360 = 0.0;

    if (len360 < len)
    {
        azimut = Az360 * half_circle / pi;
        distance = (len360 * max_radius_erch_wgs84) / 1000;
    }
    else
    {
        azimut = Az * half_circle / pi;
        distance = (len * max_radius_erch_wgs84) / 1000;
    }

    if (azimut < 0) azimut += full_angle;
    return "Azimuth spacecraft: " + std::to_string(azimut) + " Distance between coordinates: " + std::to_string(distance) + " km";
}

std::string NearSpaseForecasting::SatAnglesAndDistances(double theta, double x, double y, double z, double altitude)
{
    double Departure_angle_from_nadir_rad = 0.0, Departure_angle_from_nadir_grad = 0.0;
    double Height_angle_SC_rad = 0.0, SC_tilt_angle = 0.0, s = 0.0, achcp = 0.0;
    double x_erche = 0.0, y_erche = 0.0, z_erche = 0.0, xr = 0.0, yr = 0.0, zr = 0.0, wr = 0.0;

    s = pow(1.0 - (1.0 / 298.26), 2.0) * (1.0 / sqrt(1.0 + (1.0 / 298.26) * ((1.0 / 298.26) - 2.0) * pow(sin(latearth * pi / half_circle), 2.0)));
    achcp = (radius_erch_km * (1.0 / sqrt(1.0 + (1.0 / 298.26) * ((1.0 / 298.26) - 2.0) * pow(sin(latearth * pi / half_circle), 2.0)))) * cos(latearth * pi / half_circle);

    x_erche = achcp * cos(theta);
    y_erche = achcp * sin(theta);
    z_erche = (radius_erch_km * s) * sin(latearth * pi / half_circle);

    xr = x - x_erche;
    yr = y - y_erche;
    zr = z - z_erche;
    wr = sqrt(xr * xr + yr * yr + zr * zr);

    Height_angle_SC_rad = asin((cos(latearth * pi / half_circle) * cos(theta) * xr + cos(latearth * pi / half_circle) * sin(theta) * yr + sin(latearth * pi / half_circle) * zr) / wr);
    Height_angle_SC_grad = Height_angle_SC_rad * half_circle / pi;
    SC_tilt_angle = 90 - Height_angle_SC_grad;

    Departure_angle_from_nadir_rad = asin((max_radius_erch_km * sin(Height_angle_SC_rad + ((90 * pi) / half_circle))) / (altitude + max_radius_erch_km));
    Departure_angle_from_nadir_grad = (Departure_angle_from_nadir_rad * half_circle) / pi;

    return "Spacecraft elevation angle: " + std::to_string(Height_angle_SC_grad) + " Spacecraft tilt angle: " + std::to_string(SC_tilt_angle) + " Departure angle from nadir "
            + std::to_string(Departure_angle_from_nadir_grad) + " Distance to spacecraft: " + std::to_string(wr) + " km";
}

std::string NearSpaseForecasting::FindPositionSPG4(std::string dataandtime, int day, int month, int year, int hour, int min, int sec, int mode)
{
    int dayoftheyear = 0, s = 0;
    double em = 0.0, am = 0.0, omega = 0.0, xnodef = 0.0, xmdf = 0.0, rr = 0.0, sinphi = 0.0;
    double tmpa = 0.0, tmpe = 0.0, tmpl = 0.0, aynl = 0.0, elsq = 0.0, e2 = 0.0;
    double latitude = 0.0, longitude = 0.0, newtonnaphson = 0.0, capu = 0.0, theta = 0.0;
    double epw = 0.0, sinepw = 0.0, cosepw = 0.0, ecose = 0.0, esine = 0.0, altitude = 0.0;
    double ts = 0.0, f = 0.0, deltaepw = 0.0, pl = 0.0, cosu = 0.0, sinu = 0.0;
    double mrt = 0.0, su = 0.0, xnode = 0.0, xinc = 0.0, time = 0.0, greenwich_time = 0.0;
    double x = 0.0, y = 0.0, z = 0.0, phi = 0.0, c = 0.0, julian_date_f = 0.0, wrtwo_pi = 0.0;

    std::string data;
    std::string lat;
    std::string lon;
    std::string result;

    long long tikfromin = 0;
    bool kepler = true;

    if (mode == 0)
    {
        data = dataandtime;

        if (data.size() != 19) return "Date error";

        if (data[2] == '.') data[2] = '\0';
        else return "Date error";
        if (data[5] == '.') data[5] = '\0';
        else return "Date error";
        if (data[10] == ' ') data[10] = '\0';
        else return "Date error";
        if (data[13] == ':') data[13] = '\0';
        else return "Date error";
        if (data[16] == ':') data[16] = '\0';
        else return "Date error";

        day = std::atoi(data.c_str());
        month = std::atoi(data.c_str() + 3);
        year = std::atoi(data.c_str() + 6);
        hour = std::atoi(data.c_str() + 11);
        min = std::atoi(data.c_str() + 14);
        sec = std::atoi(data.c_str() + 17);

        if (month > 12 || month < 1) return "Month entered incorrectly";
        if (hour > 23 || hour < 0) return "The hour is entered incorrectly";
        if (min > 59 || min < 0) return "Minute entered incorrectly";
        if (sec > 59 || sec < 0) return "Incorrectly entered second";

        if ((year % 4) == 0 && (day > daysmonth[1][month] || day <= 0))
            return "Incorrect day entered";
        else if ((year % 4) != 0 && (day > daysmonth[0][month] || day <= 0))
            return "Incorrect day entered";
    }
    else
    {
        dataandtime = std::to_string(day) + '.' + std::to_string(month) + '.' + std::to_string(year) + ' ' + std::to_string(hour) + ':' + std::to_string(min) + ':' + std::to_string(sec);        
    }

    if (((year % 4) == 0 && (year % 100) != 0) || (year % 400) == 0) dayoftheyear = day + daysmonthfrombeginyear[1][month];
    else dayoftheyear = day + daysmonthfrombeginyear[0][month];

    tikfromin = (dayoftheyear - 1 + 365 * (year - 1) + (year - 1) / 4 - (year - 1) / 100 + (year - 1) / 400) * 86400000000LL + (hour * 3600LL + min * 60LL + sec) * 1000000LL;
    ts = static_cast<double>(tikfromin - datafromtle->timeepocha) / 60000000LL;

    omega = datafromtle->pericenterargument + outside_argpdot * ts;
    xmdf = datafromtle->meananomoly + outside_xmdot * ts;

    xnodef = (datafromtle->ascendingnodelon + outside_xnodedot * ts) + outside_xnodecoef * ts * ts;
    tmpa = 1.0 - outside_cc1 * ts;
    tmpe = datafromtle->brakingratio_B * outside_cc4 * ts;
    tmpl = outside_t2coef * ts * ts;

    if (outside_perigee >= 220.0)
    {
        xmdf = xmdf + ((valnearsp->omgcoef * ts) + (valnearsp->xmcoef * (pow(1.0 + outside_eta * cos(datafromtle->meananomoly + outside_xmdot * ts), 3.0) - valnearsp->del_meananomoly)));
        omega = omega - ((valnearsp->omgcoef * ts) + (valnearsp->xmcoef * (pow(1.0 + outside_eta * cos(datafromtle->meananomoly + outside_xmdot * ts), 3.0) - valnearsp->del_meananomoly)));
        tmpa = tmpa - valnearsp->d2 * ts * ts - valnearsp->d3 * ts * ts * ts - valnearsp->d4 * ts * ts * ts * ts;
        tmpe += datafromtle->brakingratio_B * valnearsp->cc5 * (sin(xmdf) - valnearsp->sin_meananomoly);
        tmpl += valnearsp->t3coef * ts * ts * ts + ts * ts * ts * ts * (valnearsp->t4coef + ts * valnearsp->t5coef);
    }

    am = outside_semimajoraxis * tmpa * tmpa;
    em = datafromtle->eccentricity - tmpe;

    if (em <= -0.001) return "Received value error";
    else if (em < 1.0e-6) em = 1.0e-6;
    else if (em > (1.0 - 1.0e-6)) em = 1.0 - 1.0e-6;

    aynl = em * sin(omega) + ((1.0 / (am * (1.0 - em * em))) * outside_ayc);
    elsq = em * cos(omega) * em * cos(omega) + aynl * aynl;

    if (elsq >= 1.0) return "Received value error";

    newtonnaphson = 1.25 * fabs(sqrt(elsq));
    capu = fmod(((xmdf + omega + xnodef + outside_calc_meanmotion * tmpl) + ((1.0 / (am * (1.0 - em * em))) * outside_xlc * em * cos(omega))) - xnodef, (2.0 * pi));
    epw = capu;

    while (s < 10 && kepler)
    {
        sinepw = sin(epw);
        cosepw = cos(epw);
        ecose = em * cos(omega) * cosepw + aynl * sinepw;
        esine = em * cos(omega) * sinepw - aynl * cosepw;
        f = capu - epw + esine;
        if (fabs(f) < 1.0e-12) kepler = false;
        else
        {
            deltaepw = f / (1.0 - ecose);
            if (s == 0)
            {
                if (deltaepw > newtonnaphson) deltaepw = newtonnaphson;
                else if (deltaepw < -newtonnaphson) deltaepw = -newtonnaphson;
            }
            else  deltaepw = f / ((1.0 - ecose) + 0.5 * esine * deltaepw);
            epw += deltaepw;
        }
        s++;
    }
    s = 0;
    pl = am * (1.0 - elsq);
    if (pl < 0.0) return "Received value error";

    cosu = am * (1.0 / (am * (1.0 - ecose))) * (cosepw - (em * cos(omega)) + aynl * esine * (1.0 / (1.0 + sqrt(1.0 - elsq))));
    sinu = am * (1.0 / (am * (1.0 - ecose))) * (sinepw - aynl - (em * cos(omega)) * esine * (1.0 / (1.0 + sqrt(1.0 - elsq))));

    mrt = (am * (1.0 - ecose)) * (1.0 - 1.5 * ((0.5 * 1.082616e-3 * (1.0 / pl)) * (1.0 / pl)) * sqrt(1.0 - elsq) * outside_x3thm1) + 0.5
        * (0.5 * 1.082616e-3 * (1.0 / pl)) * outside_x1mth2 * (2.0 * cosu * cosu - 1.0);
    su = atan2(sinu, cosu) - 0.25 * ((0.5 * 1.082616e-3 * (1.0 / pl)) * (1.0 / pl)) * outside_x7thm1 * 2.0 * sinu * cosu;
    xnode = xnodef + 1.5 * ((0.5 * 1.082616e-3 * (1.0 / pl)) * (1.0 / pl)) * outside_cos_inclination * 2.0 * sinu * cosu;
    xinc = datafromtle->inclination + 1.5 * ((0.5 * 1.082616e-3 * (1.0 / pl)) * (1.0 / pl)) * outside_cos_inclination * outside_sin_inclination * (2.0 * cosu * cosu - 1.0);

    x = mrt * (-sin(xnode) * cos(xinc) * sin(su) + cos(xnode) * cos(su)) * radius_erch_km;
    y = mrt * (cos(xnode) * cos(xinc) * sin(su) + sin(xnode) * cos(su)) * radius_erch_km;
    z = mrt * (sin(xinc) * sin(su)) * radius_erch_km;

    if (mrt < 1.0) return "Received value error";

    rr = sqrt((x * x) + (y * y));
    e2 = (1.0 / 298.26) * (2.0 - (1.0 / 298.26));
    time = ((floor(((static_cast<double>(tikfromin) / 86400000000.0) + 1721425.5) + 0.5) - 0.5) - 2451545.0) / 36525.0;
    julian_date_f = ((static_cast<double>(tikfromin) / 86400000000.0) + 1721425.5) - (floor(((static_cast<double>(tikfromin) / 86400000000.0) + 1721425.5) + 0.5) - 0.5);
    greenwich_time = (24110.54841 + time * (8640184.812866 + time * (0.093104 - time * 6.2E-6))) + (julian_date_f * 1.00273790935 * 86400.0);
    wrtwo_pi = ((greenwich_time / 240.0) * pi / half_circle) - (2.0 * pi) * floor(((greenwich_time / 240.0) * pi / half_circle) / (2.0 * pi));
    if (mode == 1) theta = (wrtwo_pi + (lonearth * pi / half_circle)) - (2.0 * pi) * floor((wrtwo_pi + (lonearth * pi / half_circle)) / (2.0 * pi));

    if (x == 0.0)
    {
        if (y > 0.0)
        {
            longitude = ((((pi / 2.0) - wrtwo_pi) + pi) - (2.0 * pi) * floor((((pi / 2.0) - wrtwo_pi) + pi) / (2.0 * pi))) - pi;
            lon_deg = longitude * half_circle / pi;
            lon = std::to_string(lon_deg);
        }
        else
        {
            longitude = ((((3.0 * pi / 2.0) - wrtwo_pi) + pi) - (2.0 * pi) * floor((((3.0 * pi / 2.0) - wrtwo_pi) + pi) / (2.0 * pi))) - pi;
            lon_deg = longitude * half_circle / pi;
            lon = std::to_string(lon_deg);
        }
    }
    else
    {
        if (x > 0.0)
        {
            longitude = (((atan(y / x) - wrtwo_pi) + pi) - (2.0 * pi) * floor(((atan(y / x) - wrtwo_pi) + pi) / (2.0 * pi))) - pi;
            lon_deg = longitude * half_circle / pi;
            lon = std::to_string(lon_deg);
        }
        else
        {
            longitude = ((((pi + atan(y / x)) - wrtwo_pi) + pi) - (2.0 * pi) * floor((((pi + atan(y / x)) - wrtwo_pi) + pi) / (2.0 * pi))) - pi;
            lon_deg = longitude * half_circle / pi;
            lon = std::to_string(lon_deg);
        }
    }

    if (rr == 0.0)
    {
        if (z > 0.0) latitude = pi / 2.0;
        else latitude = 3.0 * pi / 2.0;
    }
    else
    {
        if (rr > 0.0) latitude = atan(z / rr);
        else latitude = pi + atan(z / rr);
    }

    do
    {
        phi = latitude;
        sinphi = sin(phi);
        c = 1.0 / sqrt(1.0 - e2 * sinphi * sinphi);
        if (rr == 0.0)
        {
            if ((z + radius_erch_km * c * e2 * sinphi) > 0.0) latitude = pi / 2.0;
            else latitude = 3.0 * pi / 2.0;
        }
        else
        {
            if (rr > 0.0) latitude = atan((z + radius_erch_km * c * e2 * sinphi) / rr);
            else latitude = pi + atan((z + radius_erch_km * c * e2 * sinphi) / rr);
        }
        s++;
    } while (fabs(latitude - phi) >= 1e-10 && s < 10);
    altitude = rr / cos(latitude) - radius_erch_km * c;
    lat_deg = latitude * half_circle / pi;
    lat = std::to_string(lat_deg);

    switch (mode)
    {
    case 0:
        result = dataandtime + " Latitude: " + lat + " Longitude: " + lon;
        break;
    case 1:
        result = dataandtime + " Latitude: " + lat + " Longitude: " + lon + " " + SatAnglesAndDistances(theta, x, y, z, altitude) + " Spacecraft height " + std::to_string(altitude) + " km "
                + AzimutAndDistance(latitude, longitude) + " " + AzimuthAndAngleSun(day, month, year, hour, min);
        break;
    }

    return result;
}
