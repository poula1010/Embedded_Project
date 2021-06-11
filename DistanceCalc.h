#include <stdint.h>
#include <math.h>


extern double Distance_Calculator(double long1, double lat1, double long2, double lat2);
double Distance_Calculator(double long1, double lat1, double long2, double lat2)
{
	int Reart=6371e3;
double M_PI  =3.142857;
	double phi1 = lat1 * M_PI / 180;
	double phi2 = lat2 * M_PI / 180;
	double dphi = (phi2 - phi1);
	double dlambda = (long2 - long1) * M_PI / 180;
	double a = pow(sin(dphi / 2), 2) + pow(cos(dphi / 2), 2) * pow(sin(dlambda / 2), 2);
	double c = 2 * atan2(sqrt(a), sqrt(1 - a));
	double d = Reart * c;
	return d;
}
