double rk4 ( double t0, double u0, double dt, double f ( double t, double u ) );

double *rk4vec ( double t0, int n, double u0[], double dt, 
  double *f ( double t, int n, double u[] ) );
void timestamp ( );

void rk4vec_2d_onStack ( double t0,  double u0[], double dt, 
			 void f ( double t, double u0, double u1, double & u2, double & u3 ), 
			 double &x1, double &x2 );



