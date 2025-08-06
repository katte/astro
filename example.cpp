#include "astro.h"
#include <iostream>
using namespace std;

int main() {
    int d, m, y, hh, mi, ss;
    cout << "Inserisci data (gg mm aaaa hh mm ss): ";
    cin >> d >> m >> y >> hh >> mi >> ss;

    try {
        double jd = Astro::dateTimeToJD(d, m, y, hh, mi, ss);
        cout << "Julian Day: " << jd << endl;

        int d2, m2, y2, hh2, mi2, ss2;
        Astro::jdToDateTime(jd, &d2, &m2, &y2, &hh2, &mi2, &ss2);
        cout << "Riconversione: " << d2 << "/" << m2 << "/" << y2 << " "
             << hh2 << ":" << mi2 << ":" << ss2 << endl;

        auto wd = Astro::weekDay(jd);
        cout << "Giorno della settimana: " << static_cast<int>(wd) 
             << " (0=Lun, 6=Dom)" << endl;

    } catch (const std::exception &e) {
        cerr << "Errore: " << e.what() << endl;
    }
    return 0;
}
