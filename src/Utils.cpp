#include "Utils.h"
#include "Station.h"
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

double toRadians(double degree) {
    return degree * (M_PI / 180.0);
}

double haversine(const Station& A, const Station& B) {
    double latA = A.lat, latB = B.lat, lonA = A.lon, lonB = B.lon;
    
    const double R = 6371.0;

    double dLat = toRadians(latB - latA);
    double dLon = toRadians(lonB - lonA);

    latA = toRadians(latA);
    latB = toRadians(latB);

    double a = std::pow(std::sin(dLat / 2.0), 2) + std::cos(latA) * std::cos(latB) * std::pow(std::sin(dLon / 2.0), 2);
    
    double c = 2.0 * std::atan2(std::sqrt(a), std::sqrt(1.0 - a));

    return R * c;

}