#include "astro.h"
#include <stdint.h>
#include <stdlib.h>
#include <math.h>
Astro::Astro() {

}

double Astro::dateToJD (const int dd, const int mm, const int yyyy)
{
    return (dateTimeToJD(dd, mm, yyyy, 0, 0, 0));
}

double Astro::dateTimeToJD (const int dd, const int mm, const int yyyy, const int hh, const int min, const int sec)
{
    int jy, ja, jm;
    int intgr, gregcal;
    double dayfrac, frac, jd0, jd;
    //if (yyyy == 0) {return 0.0;}
    //if ((yyyy == 1582) && (mm == 10) && ((dd > 4) && (dd < 15))) {return 0.0;} //"The dates 5 through 14 October, 1582, do not exist in the Gregorian system!"
    //if( era == "BCE" ) yyyy = -yyyy + 1;        
    if (mm > 2) {
        jy = yyyy;
        jm = mm + 1;
    } else {
        jy = yyyy - 1;
        jm = mm + 13;
    }
    intgr = (int)floor( floor(365.25 * jy) + floor(30.6001 * jm) + dd + 1720995 );
    //check for switch to Gregorian calendar
    gregcal = 15 + 31 * (10 + 12 * 1582);
    if ((dd + 31 * (mm + 12 * yyyy)) >= gregcal) {
        ja = (int)floor(0.01 * jy);
        intgr += 2 - ja + (int)floor(0.25 * ja);
    }
    //correct for half-day offset
    dayfrac = hh / 24.0 - 0.5;
    if (dayfrac < 0.0) {
        dayfrac += 1.0;
        intgr--;
    }
    //now set the fraction of a day
    frac = dayfrac + (min + sec/60.0) / 60.0 / 24.0;
    //round to nearest second
    jd0 = ((double)intgr + frac) * 100000;
    jd = floor(jd0);
    if ((jd0 - jd) > 0.5) {
        jd += 1.0;
    }
    jd = jd / 100000.0;
    return jd;
}

void Astro::jdToDateTime (const double jd, int *dd, int *mm, int *yyyy, int *hh, int *min, int *sec)
{
    //Attenzione. il valore 2488128.5 viene riporrtato come 29/02/2100 invece che 01/03/2100
    int j1, j2, j3, j4, j5;
    int intgr;
    double frac, dayfrac, f;

    intgr = (int)floor(jd);
    frac = jd - intgr;
    if (intgr >= 2299161) {
        int tmp = (int)floor(((double)intgr - 1867216.25) / 36524.25);
        j1 = intgr + 1 + tmp - (int)floor(0.25*(double)tmp);
    } else {
        j1 = intgr;
    }
    //correction for half day offset
    dayfrac = frac + 0.5;
    if (dayfrac >= 1.0) {
        dayfrac -= 1.0;
        j1++;
    }
    j2 = j1 + 1524;
    j3 = (int)floor(6680.0 + ((j2 - 2439870) - 122.1)/365.25);
    j4 = (int)floor(j3 * 365.25);
    j5 = (int)floor((j2 - j4)/30.6001);

    *dd = (int)floor(j2 - j4 - (int)floor(j5 * 30.6001));
    *mm = (int)floor(j5 - 1);
    if (*mm > 12) *mm -= 12;
    *yyyy = (int)floor(j3 - 4715);
    if (*mm > 2) *yyyy -= 1;
    if (*yyyy <= 0) *yyyy -= 1;

    // get time of day from day fraction
    *hh = (int)floor(dayfrac * 24.0);
    *min = (int)floor((dayfrac * 24.0 - *hh) * 60.0);
    f = ((dayfrac * 24.0 - *hh) * 60.0 - *min) * 60.0;
    *sec = (int)floor(f);
    f -= *sec;
    if (f > 0.5) *sec += 1;

    if (*sec == 60) {
        *sec = 0;
        *min += 1;
    }
    if (*min == 60) {
        *min = 0;
        *hh += 1;
    }
}

int Astro::weekDay(double jd)
{
    /* Description    : Ritorna il giorno della settimana.
     *                  0 -> Lunedì
     *                  1 -> Martedì
     *                  ...
     *                  6 -> Domenica
     */
    int s = 0;
        //double t;
        //t = jd - (int)jd;
        //if (t < 0.5) {
        s = (int)(jd + 0.5) % 7;
        //} else {
        //	s = (int)jd % 7;
        //}
    return s;
}

int Astro::leapYear(int yyyy)
{
    /* La regola del calendario gregoriano è la seguente:
       Un anno è bisestile se il suo numero è divisibile per 4,
       con l'eccezione degli anni secolari (quelli divisibili per 100) che sono bisestili solo se divisibili per 400. */
        if ( (((yyyy % 100) != 0) && ((yyyy % 4)   == 0)) ||
         (((yyyy % 100) == 0) && ((yyyy % 400) == 0)) )   //Se anno secolare
        {
        return 1;
    }
    else
    {
        return 0;
    }
}

int Astro::daysInTheMonth(int yyyy, int mm)
{
    int res = 0;
    switch (mm) {
    case	4:
    case	6:
    case	9:
    case	11:
            res = 30;
            break;
    case	2:
            if (leapYear(yyyy)) {
                    res = 29;
            } else {
                    res = 28;
            }
            break;
    default:
            res = 31;
            break;
    }
    return res;
}

int Astro::nthDayOfTheYear (const int dd, const int mm, const int yyyy)
{
    int N;
    if (leapYear(yyyy))
    {
            N = (int)(275*mm/9) - (int)((mm+9)/12) + dd - 30;
    }
    else
    {
            N = (int)(275*mm/9) - 2 * (int)((mm+9)/12) + dd - 30;
    }
    return (N);
}

int Astro::dateFromDayOfTheYear (const int nn, const int yyyy, int *dd, int *mm)
{
    int a, b, c, e;
    //if (yyyy < 1953)
    if (((nn < 1) || (nn > 365)) &&  (!leapYear(yyyy))) {return 0;}
    else if (((nn < 1) || (nn > 366)) &&  (leapYear(yyyy))) {return 0;}

    if (leapYear(yyyy))
    {
            a = 1889;
    }
    else
    {
            a = 1523;
    }
    b = (int)(((double)nn + (double)a - 122.1) / 365.25);
    c = nn + a - (int)(365.25 * (double)b);
    e = (int)((double)c / 30.6001);
    if ((double)e < 13.5)
    {
            *mm = (int)(e - 1);
    }
    else
    {
            *mm = (int)(e - 13);
    }
    *dd = (int)(c - (int)(30.6001 * (double)e));
    return 1;
}

void Astro::getDSTrange (int anno, int *ddStart, int *mmStart, int *ddEnd, int *mmEnd)
{
    *mmStart = 3;
    *mmEnd = 10;
    for (int i=0; i<2; i++) {
        int *dd, *mm;
        int giorno;
        int res = 0;
        if (i == 0) {
            dd = ddStart;
            mm = mmStart;
        } else {
            dd = ddEnd;
            mm = mmEnd;
        }
        giorno = weekDay(dateToJD (1, *mm, anno));
        for (int str=1; str<=daysInTheMonth(anno, *mm); str++) {
            if (giorno == 6) {  //Se è domenica
                res = str;
            }
            giorno = (giorno == 6) ? 0 : (giorno+1);
        }
        *dd = res;
    }
}

int Astro::between_double(double value, double left, double right)
{
    return value >= left && value < right;
}
