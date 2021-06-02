#include <stdint.h>
#include <math.h>


extern float Distance_Calculator(float long1, float lat1, float long2, float lat2);
float Distance_Calculator(float long1, float lat1, float long2, float lat2)
{
	int Reart=6371e3;
float M_PI  =3.142857;
	float phi1 = lat1 * M_PI / 180;
	float phi2 = lat2 * M_PI / 180;
	float dphi = (lat2 - lat1) * M_PI / 180;
	float dlambda = (long2 - long1) * M_PI / 180;
	float a = pow(sin(dphi / 2), 2) + pow(cos(dphi / 2), 2) * pow(sin(dlambda / 2), 2);
	float c = 2 * atan2(sqrt(a), sqrt(1 - a));
	float d = Reart * c;
	return d;
}