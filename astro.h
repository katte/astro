#ifndef ASTRO_H
#define ASTRO_H

#include <stdexcept>

class Astro {
public:
    enum class WeekDay { Monday = 0, Tuesday, Wednesday, Thursday, Friday, Saturday, Sunday };

    explicit Astro();

    static double dateToJD (int dd, int mm, int yyyy);
    static double dateTimeToJD (int dd, int mm, int yyyy, int hh, int min, int sec);
    static void jdToDateTime (double jd, int *dd, int *mm, int *yyyy, int *hh, int *min, int *sec);
    static WeekDay weekDay(double jd);

    static constexpr bool leapYear(int yyyy) {
        return ( (yyyy % 4 == 0 && yyyy % 100 != 0) || (yyyy % 400 == 0) );
    }

    static int daysInTheMonth(int yyyy, int mm);
    static int nthDayOfTheYear (int dd, int mm, int yyyy);
    static int dateFromDayOfTheYear (int nn, int yyyy, int *dd, int *mm);
    static void getDSTrange (int anno, int *ddStart, int *mmStart, int *ddEnd, int *mmEnd);

    static constexpr bool between_double(double value, double left, double right) {
        return value >= left && value < right;
    }

private:
    static void validateDate(int dd, int mm, int yyyy);
};

#endif
