#include "astro.h"
#include <stdint.h>
#include <stdlib.h>
#include <math.h>

Astro::Astro() {}

void Astro::validateDate(int dd, int mm, int yyyy) {
    if (yyyy == 0) throw std::invalid_argument("Year 0 does not exist in Gregorian/Julian calendars");
    if (mm < 1 || mm > 12) throw std::invalid_argument("Month must be between 1 and 12");
    int dim = daysInTheMonth(yyyy, mm);
    if (dd < 1 || dd > dim) throw std::invalid_argument("Day is out of range for the given month/year");
}

double Astro::dateToJD (int dd, int mm, int yyyy) {
    return dateTimeToJD(dd, mm, yyyy, 0, 0, 0);
}

double Astro::dateTimeToJD (int dd, int mm, int yyyy, int hh, int min, int sec) {
    validateDate(dd, mm, yyyy);

    int jy, ja, jm;
    int intgr, gregcal;
    double dayfrac, frac, jd0, jd;

    if (mm > 2) {
        jy = yyyy;
        jm = mm + 1;
    } else {
        jy = yyyy - 1;
        jm = mm + 13;
    }

    intgr = (int)floor(floor(365.25 * jy) + floor(30.6001 * jm) + dd + 1720995);
    gregcal = 15 + 31 * (10 + 12 * 1582);

    if ((dd + 31 * (mm + 12 * yyyy)) >= gregcal) {
        ja = (int)floor(0.01 * jy);
        intgr += 2 - ja + (int)floor(0.25 * ja);
    }

    dayfrac = hh / 24.0 - 0.5;
    if (dayfrac < 0.0) {
        dayfrac += 1.0;
        intgr--;
    }

    frac = dayfrac + (min + sec/60.0) / 60.0 / 24.0;

    jd0 = ((double)intgr + frac) * 100000;
    jd = floor(jd0);
    if ((jd0 - jd) > 0.5) {
        jd += 1.0;
    }
    jd = jd / 100000.0;
    return jd;
}

void Astro::jdToDateTime (double jd, int *dd, int *mm, int *yyyy, int *hh, int *min, int *sec) {
    if (!dd || !mm || !yyyy || !hh || !min || !sec) throw std::invalid_argument("Null pointer in jdToDateTime");

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

    // Correzione per i casi limite (es. 2100)
    if (*mm == 2 && *dd == 29 && !leapYear(*yyyy)) {
        *dd = 1;
        *mm = 3;
    }

    *hh = (int)floor(dayfrac * 24.0);
    *min = (int)floor((dayfrac * 24.0 - *hh) * 60.0);
    f = ((dayfrac * 24.0 - *hh) * 60.0 - *min) * 60.0;
    *sec = (int)floor(f);
    f -= *sec;
    if (f > 0.5) *sec += 1;

    if (*sec == 60) { *sec = 0; (*min)++; }
    if (*min == 60) { *min = 0; (*hh)++; }
}

Astro::WeekDay Astro::weekDay(double jd) {
    int s = (int)(jd + 0.5) % 7;
    return static_cast<WeekDay>(s);
}

int Astro::daysInTheMonth(int yyyy, int mm) {
    switch (mm) {
        case 4: case 6: case 9: case 11: return 30;
        case 2: return leapYear(yyyy) ? 29 : 28;
        default: return 31;
    }
}

int Astro::nthDayOfTheYear (int dd, int mm, int yyyy) {
    validateDate(dd, mm, yyyy);
    if (leapYear(yyyy))
        return (int)(275*mm/9) - (int)((mm+9)/12) + dd - 30;
    else
        return (int)(275*mm/9) - 2 * (int)((mm+9)/12) + dd - 30;
}

int Astro::dateFromDayOfTheYear (int nn, int yyyy, int *dd, int *mm) {
    if (((nn < 1) || (nn > 365)) && (!leapYear(yyyy))) return 0;
    if (((nn < 1) || (nn > 366)) && (leapYear(yyyy))) return 0;

    int a = leapYear(yyyy) ? 1889 : 1523;
    int b = (int)(((double)nn + (double)a - 122.1) / 365.25);
    int c = nn + a - (int)(365.25 * (double)b);
    int e = (int)((double)c / 30.6001);

    *mm = (e < 13.5) ? (int)(e - 1) : (int)(e - 13);
    *dd = (int)(c - (int)(30.6001 * (double)e));
    return 1;
}

void Astro::getDSTrange (int anno, int *ddStart, int *mmStart, int *ddEnd, int *mmEnd) {
    *mmStart = 3;
    *mmEnd = 10;
    for (int i=0; i<2; i++) {
        int *dd, *mm;
        int giorno;
        int res = 0;
        if (i == 0) { dd = ddStart; mm = mmStart; } 
        else { dd = ddEnd; mm = mmEnd; }
        giorno = static_cast<int>(weekDay(dateToJD (1, *mm, anno)));
        for (int str=1; str<=daysInTheMonth(anno, *mm); str++) {
            if (giorno == 6) { res = str; }
            giorno = (giorno == 6) ? 0 : (giorno+1);
        }
        *dd = res;
    }
}
