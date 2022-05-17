#include "sensor.h"
#include "parameters.h"
#include <gsl/gsl_math.h>
#include <gsl/gsl_poly.h>
#include <cmath>

using namespace std;

double getR(double h, double b, double c)
{
	double R = 0.0;
	double a[5] = { pow(h,4) - b * pow(h,3), 0,
		2 * h * h - b * h + c, 0, 1 };
	double z[8];
	gsl_poly_complex_workspace* w = gsl_poly_complex_workspace_alloc(5);
	gsl_poly_complex_solve(a, 5, w, z);
	gsl_poly_complex_workspace_free(w);
	for (int i = 0; i < 8; i++)
	{
		cout << z[i] << endl;
	}
	R = abs(z[0] - z[2]);
	return R;
}


void initSenseor(sensor* s)
{

}
