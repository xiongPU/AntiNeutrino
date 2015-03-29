#include <time.h>
#include <math>
using namespace std;
#define PI 3.14159265357
#define Me 0.510998928
#define Mn 939.565378
#define Delta 939.565378-938.272046


int SetNeutrinoDirection(TTree* tree, Double_t* v_direction){
	tree->SetBranchAddress( "v_dirx", &v_direction[0]);
	tree->SetBranchAddress( "v_diry", &v_direction[1]);
	tree->SetBranchAddress( "v_dirz", &v_direction[2]);
	return 0;
}
int SetPositron(TTree* tree, Double_t* e_position_i, Double_t* e_moment_i){
	tree->SetBranchAddress( "pos_rx", &e_position_i[0]);
	tree->SetBranchAddress( "pos_ry", &e_position_i[1]);
	tree->SetBranchAddress( "pos_rz", &e_position_i[2]);
	tree->SetBranchAddress( "pos_theta", &e_moment_i[0]);
	tree->SetBranchAddress( "pos_phi", &e_moment_i[1]);
	tree->SetBranchAddress( "pos_E", &e_moment_i[2]);
	return 0;
}
int SetNeutron(TTree* tree, Double_t* n_position_i, Double_t* n_moment_i){
	tree->SetBranchAddress( "n_rx", &n_position_i[0]);
	tree->SetBranchAddress( "n_ry", &n_position_i[1]);
	tree->SetBranchAddress( "n_rz", &n_position_i[2]);
	tree->SetBranchAddress( "n_theta", &n_moment_i[0]);
	tree->SetBranchAddress( "n_phi", &n_moment_i[1]);
	tree->SetBranchAddress( "n_E", &n_moment_i[2]);
	return 0;
}
int SetCapture(TTree* tree, Double_t* e_capture, Double_t* n_capture){
	tree->SetBranchAddress("pos_cap_rx",&e_capture[0]);
	tree->SetBranchAddress("pos_cap_ry",&e_capture[1]);
	tree->SetBranchAddress("pos_cap_rz",&e_capture[2]);
	tree->SetBranchAddress( "n_cap_rx", &n_capture[0]);
	tree->SetBranchAddress( "n_cap_ry", &n_capture[1]);
	tree->SetBranchAddress( "n_cap_rz", &n_capture[2]);
	return 0;
}
int SetEnergyDeposit(TTree* tree, Double_t* positron_Edep){
	tree->SetBranchAddress( "pos_E_prim", &positron_Edep[0]);
	tree->SetBranchAddress( "pos_E_snd", &positron_Edep[1]);
	tree->SetBranchAddress( "em_E_prim", &positron_Edep[2]);
	tree->SetBranchAddress( "em_E_snd", &positron_Edep[3]);
	return 0;
}
int SetCaptureTime(TTree* tree, Double_t* n_cap_time){
	tree->SetBranchAddress( "n_cap_time", n_cap_time);
	return 0;
}
int SetAngles(TTree* tree, Double_t* del_angle){
	tree->SetBranchAddress( "del_angle_n", &del_angle[0]);
	tree->SetBranchAddress( "del_angle_v", &del_angle[1]);
	tree->SetBranchAddress( "elastic_angle", &del_angle[2]);
	return 0;
}
//================================================================================

int CreateHistoAngle(TH1F* del_ang_n[], TH1F* del_ang_v[], TH1F* angle_v_recon[], Int_t Nbin, Double_t* range_angle, Double_t time_begin, Double_t time_end){
	Double_t a = range_angle[0];	Double_t b = range_angle[1];
	cout<<"Creating normal distribution"<<endl;
	char time_cut[200];	sprintf(time_cut,"n_t_cut_%d-%d",time_begin,time_end);
	del_ang_n[0] = new TH1F("n_angle","n_angle",Nbin,a,b);
	del_ang_n[1] = new TH1F("n_z_cut","n_z_cut",Nbin,a,b);	
	del_ang_n[2] = new TH1F("n_t_cut",time_cut, Nbin,a,b);	
	del_ang_n[3] = new TH1F("n_cut","n_cut",Nbin,a,b);
	del_ang_n[4] = new TH1F("n_intg","n_intg",Nbin,a,b);
	del_ang_n[5] = new TH1F("n_intg_z_cut","n_intg_z_cut",Nbin,a,b);	
	del_ang_n[6] = new TH1F("n_intg_t_cut",time_cut,Nbin,a,b);	
	del_ang_n[7] = new TH1F("n_intg_cut","n_intg_cut",Nbin,a,b);

	cout<<"Creating integrated distribution..."<<endl;
	char time_cut2[200]	;sprintf(time_cut2,"v_t_cut_%d-%d",time_begin,time_end);
	del_ang_v[0] = new TH1F("v_angle","v_angle",Nbin,a,b);
	del_ang_v[1] = new TH1F("v_z_cut","v_z_cut",Nbin,a,b);	
	del_ang_v[2] = new TH1F("v_t_cut",time_cut2, Nbin,a,b);	
	del_ang_v[3] = new TH1F("v_cut","v_cut",Nbin,a,b);
	del_ang_v[4] = new TH1F("v_intg","v_intg",Nbin,a,b);
	del_ang_v[5] = new TH1F("v_intg_z_cut","v_intg_z_cut",Nbin,a,b);	
	del_ang_v[6] = new TH1F("v_intg_t_cut",time_cut2, Nbin,a,b);	
	del_ang_v[7] = new TH1F("v_intg_cut","v_intg_cut",Nbin,a,b);

	cout<<"Creating reconstructed distribution..."<<endl;
	angle_v_recon[0] = new TH1F("v_recon","v_recon",Nbin,a,b);
	angle_v_recon[1] = new TH1F("v_recon_cut","v_recon_cut",Nbin,a,b);
	angle_v_recon[2] = new TH1F("v_intg_recon","v_intg_recon",Nbin,a,b);
	angle_v_recon[3] = new TH1F("v_intg_recon_cut","n_intg_recon_cut",Nbin,a,b);
	return 0;
}

template<typename T>
T Max(T a, T b){return a>b?a:b;}
template<typename T>
T Min(T a, T b){return a>b?b:a;}

// use angle and energy vector to set momentum.
void SetMomentum(Double_t* src, Double_t Mass, Double_t* target){// everything in MeV
	Double_t p = sqrt(src[2]*src[2]+2*src[2]*Mass);
	target[0] = p*sin(src[0])*cos(src[1]);
	target[1] = p*sin(src[0])*sin(src[1]);
	target[2] = p*cos(src[0]);
}
// use angle+energy vector to set momentum direction
void SetMomentumDir(Double_t* src, Double_t* target){
	target[0] = sin(src[0])*cos(src[1]);
	target[1] = sin(src[0])*sin(src[1]);
	target[2] = cos(src[0]);
}

// use position difference and kinetic energy to set momentum.
void SetMomentum(Double_t* src2, Double_t* src1,Double_t K, Double_t mass, Double_t* target){
	Double_t p = sqrt(K*K+2*K*mass);
	target[0] = src2[0]-src1[0];
	target[1] = src2[1]-src1[1];
	target[2] = src2[2]-src1[2];
}

void Unit(Double_t a[]);	// forward declaration
// use position difference to set momentum direction.
void SetMomentumDir(Double_t* src2, Double_t* src1, Double_t* target){
	target[0] = src2[0]-src1[0];
	target[1] = src2[1]-src1[1];
	target[2] = src2[2]-src1[2];
	Unit(target);
}

void SetVector(Double_t* src, Double_t* target){
	for(int i=0;i<3;i++) target[i] = src[i];
}

void Normalize(TH1F* histo){
	Int_t N = histo->GetEntries();
	if(N==0) return;
	Int_t Nbin = histo->GetSize();	//hist with 100 bins, Size returns 102 while NbinsX returns 100
	for(int i=0;i<Nbin;i++)
		histo->SetBinContent(i,1.0*histo->GetBinContent(i)/N);
}

void IntegralDistribution(TH1F* tgt, TH1F* src){//probability of angle smaller than a particular angle
	Int_t Ntgt = tgt->GetSize();	Int_t Nsrc = src->GetSize();
	if(Ntgt!=Nsrc){cout<<"Target and source have different bin numbers...\n"; return;}
	for(int i=0;i<Nsrc;i++){
		Double_t tmp = 0;
		for(int j=i;j<Nsrc;j++)
			tmp += src->GetBinContent(j);
		tgt->SetBinContent(i,tmp);
	}
}

void CumulativeDistribution(TH1F* tgt, TH1F* src){//probability of angle smaller than a particular angle
	Int_t Ntgt = tgt->GetSize();	Int_t Nsrc = src->GetSize();
	if(Ntgt!=Nsrc){cout<<"Target and source have different bin numbers...\n"; return;}
	for(int i=0;i<Nsrc;i++){
		Double_t tmp = 0;
		for(int j=0;j<i;j++)
			tmp += src->GetBinContent(j);
		tgt->SetBinContent(i,tmp);
	}
}

void DivideDistribution(TH1F* tgt, Double_t a){//probability of angle smaller than a particular angle
	Int_t Ntgt = tgt->GetSize();
	for(int i=0;i<Ntgt;i++)
		tgt->SetBinContent(i,(tgt->GetBinContent(i))*1.0/a);
}

Double_t GetAngle(Double_t a[], Double_t b[]){
	Double_t angle(0), mag1(0), mag2(0);
	for(int i=0;i<3;i++){
		angle += a[i]*b[i];	mag1 += a[i]*a[i];	mag2+=b[i]*b[i];
	}
	return 180*acos(angle/sqrt(mag1*mag2))/PI;
}
Double_t GetAngleInRadian(Double_t a[], Double_t b[]){
	Double_t angle(0), mag1(0), mag2(0);
	for(int i=0;i<3;i++){
		angle += a[i]*b[i];	mag1 += a[i]*a[i];	mag2+=b[i]*b[i];
	}
	return acos(angle/sqrt(mag1*mag2));
}
Double_t Dot(Double_t a[], Double_t b[]){
	Double_t mag(0);
	for(int i=0;i<3;i++){
		mag += a[i]*b[i];
	}
	return mag;
}
void Unit(Double_t a[]){
	Double_t mag = 0;
	for(int i=0;i<3;i++) mag+=a[i]*a[i];
	for(int i=0;i<3;i++) a[i]/=sqrt(mag);
	return;
}


//iteratively solve for neutrino momentum.
void Add(const Double_t a[], const Double_t b[], Double_t c[]){
	for(int i=0;i<3;i++) c[i]=a[i]+b[i];
}
void Multiply(const Double_t a, const Double_t b[], Double_t c[]){
	for(int i=0;i<3;i++) c[i] = a*b[i];
}

void FindMomentum(Double_t pv[], Double_t pn[], const Double_t pe[], Double_t En = -1){
	Double_t Ee(0);
	Double_t pe_dir[3];
	for(int i=0;i<3;i++){
		Ee += pe[i]*pe[i];
		pe_dir[i] = pe[i];
	}
	Ee = sqrt(Ee + Me*Me);			// positron total energy

	const Double_t a = (Delta*Delta-Me*Me)/(2.0*Mn);
	Double_t theta = GetAngleInRadian(pe,pn);
//---------------------------------------initial step----------------------------------

	Unit(pn);
	if(En>0){Multiply(sqrt(En*En+2*En*Mn),pn,pn);
		 Add(pn,pe,pv);
		 return;}

	Double_t Ev = Ee+Delta;
	Double_t Kn = cos(2*theta) - 2*cos(theta)*sqrt(Ev*Ev/(Ee*Ee-Me*Me)-1+cos(theta)*cos(theta));
	Kn *= (Ee*Ee-Me*Me)/(2*Mn);
	Kn += Ev*Ev/(2*Mn);
	Double_t p_mag = sqrt(2*Kn*Mn);
	Unit(pn);	Multiply(p_mag,pn,pn);


//	Multiply(sqrt(2*Ee*(Ee+Delta)),pn,pn);	// neutron momentum initial guess
//	Double_t ve = sqrt(Dot(pe,pe))/Ee;	// positron velocity

//	Kn = Ee*(Ee+Delta)/Mn+a;
//	p_mag = sqrt(2*K*Mn);
//	Unit(pn);	Multiply(p_mag,pn,pn);
	Add(pn,pe,pv);
	return;

//	Unit(pv);	Unit(pe_dir);	// unit vector for dot product
//	Double_t K = Ee*(Ee+Delta)/Mn*(1-ve*Dot(pv,pe_dir))+a;

//	Double_t p_mag = sqrt(K*K+2*K*Mn);
//	Unit(pn);	Multiply(p_mag,pn,pn);
	Add(pn,pe,pv);

//---------------------------------------iteration-------------------------------------
	for(int i=0;i<4;i++){
		Unit(pv);
		K = Ee*(Ee+Delta)/Mn*(1-ve*Dot(pv,pe_dir))+a;
		p_mag = sqrt(K*K+2*K*Mn);
		Unit(pn);	Multiply(p_mag,pn,pn);	Add(pn,pe,pv);
	}
}

void PrintStatistics(Double_t record[10]){
	cout<<"Printing statistics..."<<'\n'<<endl;
	for(int i=0;i<50;i++) cout<<"*";
	cout<<endl;

	cout<<"\n\tTotal event analyzed:\t"<<record[0]<<endl<<endl;

	cout<<"\tNeutron escaped:\t"<<record[1]<<endl;
	cout<<"\t\tPercentage:\t"<<100.0*record[1]/record[0]<<'%'<<endl<<endl;
	cout<<"\tNeutron before 2us:\t"<<record[2]<<endl;
	cout<<"\t\tPercentage:\t"<<100.0*record[2]/record[0]<<'%'<<endl<<endl;
	cout<<"\tAbove combined:\t\t"<<record[3]<<endl;
	cout<<"\t\tPercentage:\t"<<100.0*record[3]/record[0]<<'%'<<endl<<endl;
	cout<<"\tNeutron after delta T:\t"<<record[4]<<endl;
	cout<<"\t\tPercentage:\t"<<100.0*record[4]/record[0]<<'%'<<endl<<endl;

	cout<<"\te+ 200keV prim & snd:\t"<<record[5]<<endl;
	cout<<"\t\tPercentage:\t"<<100.0*record[5]/record[0]<<'%'<<endl<<endl;
	cout<<"\te+ 200keV prim:\t\t"<<record[6]<<endl;
	cout<<"\t\tPercentage:\t"<<100.0*record[6]/record[0]<<'%'<<endl<<endl;
	cout<<"\te+ 0keV snd:\t\t"<<record[7]<<endl;
	cout<<"\t\tPercentage:\t"<<100.0*record[7]/record[0]<<'%'<<endl<<endl;

	cout<<"\te+ z > 10cm:\t\t"<<record[8]<<endl;
	cout<<"\t\tPercentage:\t"<<100.0*record[8]/record[0]<<'%'<<endl<<endl;

	cout<<"\te+ z>10cm, prim snd > 200 keV:\t"<<record[9]<<endl;
	cout<<"\t\tPercentage:\t"<<100.0*record[9]/record[0]<<'%'<<endl<<endl;

	for(int i=0;i<50;i++) cout<<"*";cout<<endl;
}
