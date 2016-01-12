#include <stdio.h>
#include <math.h>

/*
double erfc(double x)
{
    // constants
    double a1 =  0.254829592;
    double a2 = -0.284496736;
    double a3 =  1.421413741;
    double a4 = -1.453152027;
    double a5 =  1.061405429;
    double p  =  0.3275911;

    // Save the sign of x
    int sign = 1;
    if (x < 0)
        sign = -1;
    x = fabs(x);

    // A&S formula 7.1.26
    double t = 1.0/(1.0 + p*x);
    double y = 1.0 - (((((a5*t + a4)*t) + a3)*t + a2)*t + a1)*t*exp(-x*x);

    return 1 - sign*y;
}
*/


static double Q(double x)
{
	return 0.5 * erfc(x / sqrt(2.0));
}



double GetBerPsk(double Snr, int constellation_scheme)
{
	int ModulationIndex;
	int Bps;

	switch (constellation_scheme)
	{
		case 0: // QPSK
			ModulationIndex = 0; Bps = 2; break;
		case 1: // 8PSK
			ModulationIndex = 1; Bps = 3; break;		
	}
	
	double tab_d[2] =
	{
		2.0, // QPSK
		0.585786, // 8PSK
	};
	
	double N0, Ps;
	N0 = pow(10.0, -Snr / 10.0);
	// Ps = erfc((sqrt(tab_d[ModulationIndex] / (2.0 * N0))) / sqrt(2.0));
	// Ps = erfc(sqrt(tab_d[ModulationIndex] / (4.0 * N0)));
	Ps = 2 * Q(sqrt(tab_d[ModulationIndex] / (2.0 * N0)));
	
	return Ps / Bps;
}

int main()
{
	double Input = 11.4;
	int Modulation = 1;
	
	printf("VBER = %.1E\n", GetBerPsk(Input, Modulation));
}

