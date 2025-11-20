#include "Station.h"
#include <iostream>
#include <iomanip>

using namespace std;

void Station::print() const {
    cout << "Station [" << id << "]: " 
         << name << " (" << originalId << ") "
         << "Lat: " << fixed << setprecision(4) << lat 
         << ", Lon: " << fixed << setprecision(4) << lon 
         << endl;
}
