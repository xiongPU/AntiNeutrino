#include "AntiNeutrino.hh"
#define me 0.510995928*MeV
#define mn 939.565378*MeV

AntiNeutrino* AntiNeutrino::fantiv = 0;

AntiNeutrino::AntiNeutrino(){

	G4double polar = pi/6;
	G4double aziml = pi/4;
	v_dir = G4ThreeVector(sin(polar)*cos(aziml),sin(polar)*sin(aziml),cos(polar));
	v_dir = v_dir.unit();

	n_dir = e_dir = v_dir;
	v_E = 4*MeV;	n_E = e_E = 0;

	nEnergy = 0;	prob = 0;	N = 0;
	lbin = hbin = d = 0;
	nAngle = 0;	prob_angle = 0;	angle_N = 0;
	angle_lbin = angle_hbin = angle_d = 0;

	fantiv = this;
	LoadData();
}
AntiNeutrino::AntiNeutrino(G4ThreeVector s){

	v_dir = s.unit();
	n_dir = e_dir = v_dir;
	v_E = 5*MeV;	n_E = e_E = 0;

	nEnergy = 0;	prob = 0;	N = 0;
	lbin = hbin = d = 0;
	nAngle = 0;	prob_angle = 0;	angle_N = 0;
	angle_lbin = angle_hbin = angle_d = 0;

	fantiv = this;
	LoadData();
}
//============================================================================
AntiNeutrino::~AntiNeutrino(){
	delete nEnergy;	nEnergy = 0;
	delete prob;	prob = 0;
	delete nAngle;	nAngle = 0;
	delete prob_angle;	prob_angle = 0;
	fantiv = 0;
}
//============================================================================
void AntiNeutrino::SetDistribution(){
//----------------------------------neutrino momentum----------------------------------------
	G4ThreeVector pv = v_E*(v_dir.unit());
//----------------------------------variables------------------------------------------------	
	G4double pn_mag, pe_mag, theta;
//----------------------------------neutron energy-------------------------------------------

	G4bool Accp = false;
	while(Accp == false){
		while(Accp == false){
			n_E  = G4UniformRand()*(hbin-lbin) + lbin;// a uniform test energy
			G4int n = G4int((n_E-lbin)/d);
			G4double p;	// accept above value with probability p
			if(n<N-1) p = prob[n] + (prob[n+1]-prob[n])*(n_E-nEnergy[n])/d;	// lin interp
			else if(n==N-1) p = prob[N-1];
			else p = 0;
			if(G4UniformRand()<p)	Accp = true;
		}		// neutron kinetic energy
		n_E*=keV;	// kinetic energy
		pn_mag = sqrt(n_E*n_E+2*mn*n_E);	// neutron momentum magnitude

//-----------------------------------positron energy------------------------------------------
		e_E = v_E - n_E - 1.293332*MeV - 0.511*MeV;	//kinetic energy
		pe_mag = sqrt(e_E*e_E+2*me*e_E);
//-----------------------------------angle between n and v------------------------------------
		theta = ((v_E*v_E + pn_mag*pn_mag - pe_mag*pe_mag)/(2*pn_mag*v_E));
		if(theta>1){
			G4cout<<"Neutron energy:\t"<<n_E<<"\t, neutron p-mag:\t"<<pn_mag<<G4endl;
			G4cout<<"Positron energy:\t"<<e_E<<G4endl;
			G4cout<<"Positron momentum:\t"<<pe_mag<<G4endl;
			G4cout<<"Neutron Neutrino p-diff:\t"<<v_E*v_E + pn_mag*pn_mag - pe_mag*pe_mag<<G4endl;
			G4cout<<"Denominator:\t"<<2*pn_mag*v_E<<G4endl;
			Accp = false;}
	}
	theta = acos(theta);		// neutron theta angle
//-----------------------------------neutron direction ---------------------------------------
	G4double uphi = 2*pi*G4UniformRand();
	G4double utheta = acos(2*(G4UniformRand()-0.5));
	n_dir = G4ThreeVector(sin(utheta)*cos(uphi), sin(utheta)*sin(uphi),cos(utheta));
	G4ThreeVector para = cos(theta)*v_dir.unit();// project a random uniform vector to angle theta
	G4ThreeVector perp = sin(theta)*(n_dir.cross(v_dir)).unit();//perpendicular
	n_dir = para + perp;
	G4ThreeVector pn = pn_mag*n_dir;
//-----------------------------------positron direction and momentum--------------------------
	e_dir = (pv - pn).unit();
}

G4double AntiNeutrino::GetNeutronEnergy(){//produce energy distribution
/*	n_E = 0;
	G4bool Accp = false;
	while(Accp ==false){
		n_E  = G4UniformRand()*(hbin-lbin) + lbin;// a uniform test energy

		G4int n = G4int((n_E-lbin)/d);
		G4double p;	// accept above value with probability p
		if(n<N-1) p = prob[n] + (prob[n+1]-prob[n])*(n_E-nEnergy[n])/d;	// lin interp
		else if(n==N-1) p = prob[N-1];
		else p = 0;
		if(G4UniformRand()<p)	Accp = true;
	}
	n_E*=keV;
	G4cout<<n_E<<G4endl;
*/	return n_E;
}
//==========================================================================
G4ThreeVector AntiNeutrino::GetNeutronDirection(){
/*	G4double theta = 0;
	G4bool Accp = false;
	while(Accp ==false){
		theta  = G4UniformRand()*(angle_hbin-angle_lbin) + angle_lbin;
		G4int n = G4int((theta-angle_lbin)/angle_d);
		G4double p;
		if(n<angle_N-1) p = prob_angle[n] + (prob_angle[n+1]-prob_angle[n])*(theta-nAngle[n])/angle_d;	// linear interpolation
		else if(n==angle_N-1) p = prob_angle[angle_N-1];
		else p = 0;
		if(G4UniformRand()<p)	Accp = true;
	}

	G4double uphi = 2*pi*G4UniformRand();
	G4double utheta = acos(2*(G4UniformRand()-0.5));
	n_dir = G4ThreeVector(sin(utheta)*cos(uphi), sin(utheta)*sin(uphi),cos(utheta));

	G4ThreeVector para = cos(theta)*v_dir.unit();// paralell component
	G4ThreeVector perp = sin(theta)*(n_dir.cross(v_dir)).unit();//perpendicular

	G4cout<<para+perp<<G4endl;
*/	return n_dir;
	
}
//==========================================================================

G4double AntiNeutrino::GetPositronEnergy(){
	return e_E;// = v_E - n_E - 1.293*MeV - 0.511*MeV;
}

G4ThreeVector AntiNeutrino::GetPositronDirection(){
	return e_dir;
}

//==================================================================================
void AntiNeutrino::LoadData(){
	char name_energy[150], name_angle[150];
	sprintf(name_energy, "/home/suerfu/AntiNeutrino/anti-neutrino/src/data_E_Ev_%dMeV.dat",(G4int)(v_E/MeV)); 
	sprintf(name_angle, "/home/suerfu/AntiNeutrino/anti-neutrino/src/data_angle_Ev_%dMeV.dat", (G4int)(v_E/MeV)); 

	std::fstream file(name_energy,std::fstream::in);
	file>>lbin>>hbin>>N;
	G4cout<<"Reading energy distribution from\t"<<name_energy<<G4endl;
	G4cout<<"Energy ranging from\t"<<lbin<<"keV\tto\t"<<hbin<<"keV\n";

	if(nEnergy!=0)	delete nEnergy;
	if(prob!=0)	delete prob;
	nEnergy = new G4double [N];	prob = new G4double [N];
	for(int i=0;i<N;i++){
		nEnergy[i] = 0;
		prob[i] = 0;
	}
	d = (hbin-lbin)/N;
	for(int i=0;i<N;i++) file>>nEnergy[i]>>prob[i];
//------------------------------------------------------------------------------
/*	std::fstream file2(name_angle,std::fstream::in);
	file2>>angle_lbin>>angle_hbin>>angle_N;
	G4cout<<"Angle ranging from "<<angle_lbin<<"deg\t to "<<angle_hbin<<"deg\n";
	angle_lbin *=pi/180;	angle_hbin *= pi/180;

	if(nAngle!=0)	delete nAngle;
	if(prob_angle!=0)	delete prob_angle;
	nAngle = new G4double [angle_N];
	prob_angle = new G4double [angle_N];	// acceptance probability
	for(int i=0;i<angle_N;i++){
		nAngle[i] = 0;
		prob_angle[i] = 0;
	}
	angle_d = (angle_hbin-angle_lbin)/angle_N;
	for(int i=0;i<angle_N;i++) file2>>nAngle[i]>>prob_angle[i];
	for(int i=0;i<angle_N;i++) nAngle[i] *= pi/180.;*/
}
