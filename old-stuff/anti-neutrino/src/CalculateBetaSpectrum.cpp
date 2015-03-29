// This program calculates tritium beta decay within a certain energy range near end-point.
// The calculation is based on Fermi golden rule, see wiki/Beta_decay

#include <cmath>
#include <fstream>
#include <iostream>
#include <cstdlib>
#include <cstring>
using namespace std;

#define pi 3.141592653

const double alpha = 0.007297352;	// fine structure constant
const double me = 510.99;	// electron mass in keV
const double Q = 18.6;


double F(double T){
	double eta = -alpha*2*(T+me)/sqrt(T*T+2*T*me);
	return 2*pi*eta/(1-exp(-2*pi*eta));
}

int main(int argc, char** argv){

	cout<<"Initializing...\n";

	double lbin = 18.2;
	if(argc>1){
		for(int i=1;i<argc;i++){
			if(strcmp(argv[i-1],"-low")==0)	lbin = atof(argv[i]);
		}
	}

	double hbin = Q;

	int N = 3000;

	double* hist = new double [N];
	double* x = new double [N];
	for(int i=0;i<N;i++){
		hist[i] = 0;
	}
	x[0] = lbin;

	double d = (Q-lbin)/(N-1);

	cout<<"Calculating beta decay spectrum...\n";

	double max = 0;	// later will be used to normalize the spectrum

	for(int i=0;i<N;i++) x[i] = x[0] + i*d;

	for(int i=0;i<N;i++){
		double p = sqrt(x[i]*x[i]+2*me*x[i]);
		double E = x[i]+me;
		hist[i] = F(x[i])*p*E*(Q-x[i])*(Q-x[i]);
		max = (hist[i]>max)?hist[i]:max;
	}

	for(int i=0;i<N;i++){
		hist[i] /= max;
	}
	
	cout<<"Exporting the spectrum..."<<endl;
	fstream file("Tritium_full_beta_spectrum).dat",fstream::out);
	if(!file.is_open()){
		cout<<"ERROR opening the file...\nExiting...\n";
		return 0;
	}

	file<<lbin<<'\t'<<Q<<'\t'<<N<<endl;

	for(int i=0;i<N;i++)
		file<<x[i]<<'\t'<<hist[i]<<endl;

	cout<<"Export finished...\nExiting...\n";

	delete hist;	hist = 0;
	delete x;	x = 0;

	return 0;

}
