#include "rk4.hpp"

void rk4_API (int id, double t0, double u[], double tmax, double dt);
void rk4_API (int id, double t0, double x0, double v0, double tmax, double dt,
	      double & x1, double & v1);
double *rk4_f ( double t, int n, double u[] );


void rk4_API_onStack (int id, double t0, double x0, double v0, double tmax, double dt,
		      double & x1, double & v1);
void rk4_f_2d_onStack ( double t, double u0, double u1, double & uprime0, double & uprime1);

