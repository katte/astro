#ifndef ASTRO_H
#define ASTRO_H

class Astro {
public:
	explicit Astro();
        static double dateToJD (const int dd, const int mm, const int yyyy);
        static double dateTimeToJD (const int dd, const int mm, const int yyyy, const int hh, const int min, const int sec);
        static void jdToDateTime (const double jd, int *dd, int *mm, int *yyyy, int *hh, int *min, int *sec);
        static int weekDay(double jd);
        static int leapYear(int yyyy);
        static int daysInTheMonth(int yyyy, int mm);
        static int nthDayOfTheYear (const int dd, const int mm, const int yyyy);
        static int dateFromDayOfTheYear (const int nn, const int yyyy, int *dd, int *mm);
        static void getDSTrange (int anno, int *ddStart, int *mmStart, int *ddEnd, int *mmEnd);
        static int between_double(double value, double left, double right);
};

#endif
