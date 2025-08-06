#include "astro.h"
#include <iostream>
#include <cassert>
#include <chrono>

using namespace std;

void test_round_trip(int dd, int mm, int yyyy, int hh=0, int min=0, int sec=0) {
    double jd = Astro::dateTimeToJD(dd, mm, yyyy, hh, min, sec);
    int dd2, mm2, yyyy2, hh2, min2, sec2;
    Astro::jdToDateTime(jd, &dd2, &mm2, &yyyy2, &hh2, &min2, &sec2);
    assert(dd == dd2 && mm == mm2 && yyyy == yyyy2);
    assert(hh == hh2 && min == min2 && sec == sec2);
}

void test_leap_years() {
    assert(Astro::leapYear(2000));
    assert(!Astro::leapYear(1900));
    assert(Astro::leapYear(2024));
    assert(!Astro::leapYear(2100));
}

void test_days_in_month() {
    assert(Astro::daysInTheMonth(2024, 2) == 29);
    assert(Astro::daysInTheMonth(2023, 2) == 28);
    assert(Astro::daysInTheMonth(2023, 4) == 30);
    assert(Astro::daysInTheMonth(2023, 1) == 31);
}

void test_2100_bug_fix() {
    double jd = Astro::dateToJD(29, 2, 2100);
    int dd, mm, yyyy, hh, min, sec;
    Astro::jdToDateTime(jd, &dd, &mm, &yyyy, &hh, &min, &sec);
    assert(dd == 1 && mm == 3 && yyyy == 2100);
}

void test_with_chrono() {
    using namespace std::chrono;
    int year = 2024;
    int month = 12;
    int day = 31;

    // calcolo giorno dell'anno con chrono
    sys_days base = sys_days{January/1/year};
    sys_days date = sys_days{month/day/year};
    int doy = (date - base).count() + 1;

    // confronto con Astro
    int our_doy = Astro::nthDayOfTheYear(day, month, year);
    assert(doy == our_doy);
}

int main() {
    cout << "Running tests...\n";
    test_round_trip(1, 1, 2000);
    test_round_trip(31, 12, 2023, 23, 59, 59);
    test_round_trip(29, 2, 2024); // bisestile
    test_leap_years();
    test_days_in_month();
    test_2100_bug_fix();
    test_with_chrono();
    cout << "All tests passed!\n";
    return 0;
}
