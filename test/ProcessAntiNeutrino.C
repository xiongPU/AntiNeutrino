#include <time.h>
#include <math>
#include "ProcessAntiNeutrino.h"
using namespace std;
#define PI 3.14159265357
#define Me 0.510998928
#define Mn 939.565378
#define Delta 939.565378-938.272046

void ProcessAntiNeutrino(char* name="test.root", char* treename="an_det"){
	Double_t time_begin(0), time_end(1000);	// time range in the cut
	Double_t time_accp(20000);		// time cut used in the acceptance ratio distribution plot
	Double_t neutron_range(5);

	const int Nrec = 10;			// keep track of how many neutrons pass cuts
	Double_t Record[Nrec];
	for(int i=0;i<Nrec;i++)	Record[i] = 0;

	TH1::AddDirectory(kFALSE);
	printf("This code will process file %s\n",name);
	printf("Opening ROOT file %s\n",name);
	TFile* rootfile = new TFile(name,"READ");	// original root file that contains data

	char newname[100];
	strcpy(newname,"processed-");	strcat(newname,name);
	printf("New file to write:\t%s\n",newname);

	TFile* newfile = new TFile(newname,"UPDATE");	// newfile to contain plots
	if(rootfile==0 || newfile==0){cout<<"Cannot open the file "<<name<<endl;return;}
	else cout<<"ROOT file opened...\n";

	TTree* tree;	rootfile->GetObject(treename,tree);
	if(tree==0){cout<<"Cannot retrieve the event tree...\n"<<endl;return;}
	else cout<<"TTree object created...\n";

//========================== Read from the first file===============================
	Int_t Nevt = tree->GetEntries();		// number of events
	Int_t Nbin = Nevt/200;
	if(Nbin>200) Nbin = 200;

//========================== Variables to hold the values in the tree==============
	Double_t v_direction[3];			// neutrino direction
	Double_t e_position_i[3], e_moment_i[3];	// e+ initial position and momentum
	Double_t n_position_i[3], n_moment_i[3];	// neutron initial
	Double_t e_capture[3], n_capture[3];		// e+ and n capture position
	Double_t n_cap_time;				// neutron capture time
	Double_t positron_Edep[4];			// energy deposite of positrons
	Double_t del_angle[3];				// error in reconstructing angles

	cout<<"Setting branch addresses...\n";
	SetNeutrinoDirection(tree, v_direction);
	SetPositron(tree, e_position_i, e_moment_i);
	SetNeutron(tree, n_position_i, n_moment_i);
	SetCapture(tree, e_capture, n_capture);
	SetEnergyDeposit(tree, positron_Edep);
	SetCaptureTime(tree, &n_cap_time);
	SetAngles(tree, del_angle);

//===========================Determine the range of variables=====================
	Double_t range_n_cap[2],range_angle[2];
	range_angle[0] = 0;	range_angle[1] = 180;
	Double_t E_prim[2], E_snd[2];

	cout<<"Determining the range...\n";
	tree->GetEntry(0);
	range_n_cap[0] = range_n_cap[1] = 0;
	E_prim[0] = E_prim[1] = positron_Edep[0];	E_snd[0] = E_snd[1] = positron_Edep[1];

	Bool_t continuous(false);
	for(int evt=0;evt<Nevt;evt++){
		tree->GetEntry(evt);
		range_n_cap[0] = Min(range_n_cap[0], n_capture[2]);
		range_n_cap[1] = Max(range_n_cap[1], n_capture[2]);
		E_prim[0] = Min(positron_Edep[0],E_prim[0]);	E_prim[1] = Max(positron_Edep[0],E_prim[1]);
		E_snd[0] = Min(positron_Edep[1],E_snd[0]);	E_snd[1] = Max(positron_Edep[1],E_snd[1]);
		if(n_capture[2]>10&&n_capture[2]<60) continuous = true;		// if detector is spaced or not
	}

	range_n_cap[0] *=1.1;	range_n_cap[1] *=1.1;	E_prim[1] *= 1.1;	E_snd[1] *= 1.1;
	if(continuous){neutron_range = 0;cout<<"Monolithic detector!\n";}
	cout<<"neutron capture location:\t"<<range_n_cap[0]<<'\t'<<range_n_cap[1]<<endl;
	cout<<"positron e-dep in primary:\t"<<E_prim[0]<<'\t'<<E_prim[1]<<endl;
	cout<<"positron e-dep in secondary:\t"<<E_snd[0]<<'\t'<<E_snd[1]<<endl;

//===========================creating histograms==================================

	cout<<"Allocating memories for histograms...\n";
        Double_t a = range_angle[0];    Double_t b = range_angle[1];

	TH1F* n_capture_pos;	// neutron capture z-position
	TH1F* neutron_theta;	// polar angle of neutron momentum?

	TH1F* angle_positron[2];	// angular error in e+, exact vs position difference
	TH1F* del_ang_n[8];	TH1F* del_ang_v[8];
	TH1F* angle_v_recon[4];		// error to v angle using momentum conservation

	TH2F* error, *error_to_n, *error_to_e;
	TH1F* accp_ratio, *accp_ratio_int;
	const int dAngle = 10;	const int N_theta=180/dAngle;	TH2F* theta_del_t[N_theta];

	n_capture_pos = new TH1F("n_cap_pos","Neutron Capture Z-Position",Nbin,range_n_cap[0], range_n_cap[1]);
	neutron_theta = new TH1F("n_theta","Neutron Polar Angle",Nbin,range_angle[0],range_angle[1]);
	angle_positron[0] = new TH1F("ep_angle","Error in e+ Angle",Nbin,range_angle[0],range_angle[1]);
	angle_positron[1] = new TH1F("ep_z_cut","Error in e+ Angle w. Cut",Nbin,range_angle[0],range_angle[1]);


        char time_cut[200];     sprintf(time_cut,"Error in Neutron Angle, T-cut:%d-%d",time_begin,time_end);
        del_ang_n[0] = new TH1F("n_angle","Error in Neutron Angle",Nbin,a,b);
        del_ang_n[1] = new TH1F("n_z_cut","Error in Neutron Angle",Nbin,a,b);
        del_ang_n[2] = new TH1F("n_t_cut",time_cut, Nbin,a,b);
        del_ang_n[3] = new TH1F("n_cut","Error in Neutron Angle",Nbin,a,b);
        del_ang_n[4] = new TH1F("n_intg","Neutron Angle Integrated Error Distribution",Nbin,a,b);
        del_ang_n[5] = new TH1F("n_intg_z_cut","Neutron Angle Integrated Error Distribution",Nbin,a,b);
        del_ang_n[6] = new TH1F("n_intg_t_cut",time_cut,Nbin,a,b);
        del_ang_n[7] = new TH1F("n_intg_cut","Neutron Angle Integrated Error Distribution",Nbin,a,b);

	char time_cut2[200];	sprintf(time_cut2,"Neutron Angle w.r.t Anti-v, time-cut:%d-%d",time_begin,time_end);
        del_ang_v[0] = new TH1F("v_angle","Neutron Angle w.r.t. Anti-v",Nbin,a,b);
        del_ang_v[1] = new TH1F("v_z_cut","Neutron Angle w.r.t. Anti-v",Nbin,a,b);  
        del_ang_v[2] = new TH1F("v_t_cut",time_cut2, Nbin,a,b); 
        del_ang_v[3] = new TH1F("v_cut","Neutron Angle w.r.t. Anti-v",Nbin,a,b);
        del_ang_v[4] = new TH1F("v_intg","Integrated Neutron Angle Distribution w.r.t. Anti-v",Nbin,a,b);
        del_ang_v[5] = new TH1F("v_intg_z_cut","Integrated Neutron Angle Distribution w.r.t. Anti-v",Nbin,a,b);        
        del_ang_v[6] = new TH1F("v_intg_t_cut",time_cut2, Nbin,a,b);    
        del_ang_v[7] = new TH1F("v_intg_cut","Integrated Neutron Angle Distribution w.r.t. Anti-v",Nbin,a,b);

	angle_v_recon[0] = new TH1F("v_recon","Error in Reconstructed Anti-Neutrino Angle",Nbin,a,b);
	angle_v_recon[1] = new TH1F("v_recon_cut","Integrated Distribution of Anti-Neutrino Angle",Nbin,a,b);
	angle_v_recon[2] = new TH1F("v_intg_recon","Error in Reconstructed Anti-Neutrino Angle with Cut",Nbin,a,b);
	angle_v_recon[3] = new TH1F("v_intg_recon_cut","Integrated Distribution of Anti-Neutrino Angle with Cut",Nbin,a,b);

	error = new TH2F("error","Error vs Cap. Time",Nbin,a,b,Nbin,time_begin, 4*time_end);
	error_to_n = new TH2F("error_to_n","Error to Neutron",Nbin,a,b,Nbin,a,b);
	error_to_e = new TH2F("error_to_e","Error to Positron",Nbin,a,b,Nbin,a,b);

	accp_ratio = new TH1F("accp_ratio_raw","Neutron Acceptance vs Time", Nbin, 0, time_accp);
	accp_ratio_int = new TH1F("accp_ratio","Neutron Acceptance vs Time", Nbin, 0, time_accp);

	for(int i=0;i<N_theta;i++){
		char tmp[100]; sprintf(tmp,"theta_t_%d_%d",i*dAngle,(i+1)*dAngle);
		theta_del_t[i] = new TH2F(tmp,tmp,Nbin,a,b,Nbin,0,time_accp);
	}
//========================Begin to loop over the tree=============================
	printf("Looping over the ROOT event tree with %d events.\n",Nevt);

	for(int evt=0;evt<Nevt;evt++){
		Double_t e_moment_appx[3], n_moment_appx[3], v_moment_appx[3];
		Double_t e_moment_true[3], n_moment_true[3];

		tree->GetEntry(evt);
		del_angle[0] *=180/PI;	del_angle[1] *=180/PI;	del_angle[2] *=180/PI;
			// convert from radian to degree
		n_moment_i[2] /=1000.0;	e_moment_i[2] /=1000.0;
			// convert to MeV from keV
		Record[0]++;	if(evt%10000==0)cout<<"Processing event: "<<evt<<endl;

		n_capture_pos->Fill(n_capture[2]);	neutron_theta->Fill(180*n_moment_i[0]/PI);
		del_ang_n[0]->Fill(del_angle[0]);	del_ang_v[0]->Fill(del_angle[1]);

		SetMomentum(e_moment_i, Me, e_moment_true);	// momentum with magnitude
		SetMomentum(n_moment_i, Mn, n_moment_true);	// with magnitude
//		SetMomentum(e_capture, e_position_i, e_moment_i[2], Me, e_moment_appx);	// with mag
		SetVector(e_moment_true, e_moment_appx);	// equal
//		SetVector(n_moment_true, n_moment_appx);

		angle_positron[0]->Fill(GetAngle(e_moment_true,e_moment_appx));
//		neutron_theta->Fill(GetAngle(n_moment_true, n_moment_appx));	// theoretical analysis
//==================================================================================================

		// pass z-cut
		bool fill_both = true;
		if(fabs(n_capture[2])>neutron_range){
			Record[1]++;
			del_ang_n[1]->Fill(del_angle[0]);
			del_ang_v[1]->Fill(del_angle[1]);
			accp_ratio->Fill(n_cap_time);
		}
		else fill_both = false;

		// pass timing cut
		if(n_cap_time>time_begin && n_cap_time < time_end){
			Record[2]++;
			del_ang_n[2]->Fill(del_angle[0]);
			del_ang_v[2]->Fill(del_angle[1]);
		}
		else fill_both = false;

//================================================================================================
		SetMomentumDir(n_capture, n_position_i, n_moment_appx);	// n direction set
		FindMomentum(v_moment_appx, n_moment_appx, e_moment_appx);
		Double_t pn_mag(0); for(int i=0;i<3;i++) pn_mag += n_moment_appx[i]*n_moment_appx[i];
							// neutron momentum magnitude
		Double_t kn = sqrt(pn_mag + Mn*Mn)-Mn;
							// neutron Kinetic Energy
		pn_mag = sqrt(pn_mag);
							// n momentum magnitude
		Double_t time_travel = 1.0/fabs(n_moment_appx[2]/pn_mag)*sqrt(Mn/(2.0*kn))/(2.98*10);
							// neutron travel time
		Double_t v_angle = GetAngle(v_moment_appx, v_direction);

		if(Dot(e_moment_appx, n_moment_appx)<0 && fill_both){		// angular cut
			Record[3]++;
			del_ang_n[3]->Fill(del_angle[0]);
			del_ang_v[3]->Fill(del_angle[1]);

			if(n_cap_time>time_travel){
				Record[4]++;
				angle_v_recon[0]->Fill(v_angle);

				error->Fill(v_angle, n_cap_time);
				error_to_n->Fill(del_angle[0], v_angle);
				error_to_e->Fill(GetAngle(e_moment_true,e_moment_appx),v_angle);
				error->Fill(del_angle[0],n_cap_time);
			}
		}

		if(v_angle>0&&v_angle<180)
			theta_del_t[(int(v_angle))/dAngle]->Fill(GetAngle(e_moment_appx, n_moment_appx),n_cap_time);

		if(positron_Edep[0]>200 && positron_Edep[1]>0)Record[5]++;
		if(positron_Edep[0]>200) Record[6]++;
		if(positron_Edep[1]>0) Record[7]++;
		if(fabs(e_capture[2])>neutron_range ){
			Record[8]++;
			if(fill_both){
				angle_positron[1]->Fill(GetAngle(e_moment_true, e_moment_appx));
				if(positron_Edep[0]>200 && positron_Edep[1]>0){
					angle_v_recon[1]->Fill(GetAngle(v_moment_appx, v_direction));
					Record[9]++;
				}
			}
		}
	}

//==================================Normalize histogram=========================================
	cout<<"Normalizing histograms...\n";
	Normalize(n_capture_pos);	Normalize(neutron_theta);
	for(int i=0;i<4;i++)
		{Normalize(del_ang_n[i]);	Normalize(del_ang_v[i]);}

	for(int i=0;i<2;i++)
		{Normalize(angle_v_recon[i]);	Normalize(angle_positron[i]);}

//==================================Integral distribution======================================
	for(int i=0;i<4;i++){
		IntegralDistribution(del_ang_n[i+4], del_ang_n[i]);
		IntegralDistribution(del_ang_v[i+4], del_ang_v[i]);
	}
	for(int i=0;i<2;i++)
		IntegralDistribution(angle_v_recon[i+2],angle_v_recon[i]);

	CumulativeDistribution(accp_ratio_int,accp_ratio);
	DivideDistribution(accp_ratio_int, Nevt);

//========================New directory=======================================================
	cout<<"Creating new directories...\n";
	newfile->cd();

	printf("Retrieving local time...\n");
	time_t timer;	time(&timer);
	struct tm* now = localtime(&timer);
	char dirname[200];
	sprintf(dirname,"run-%d-%d-%d-%d",now->tm_mon+1,now->tm_mday,now->tm_hour,now->tm_min);
	printf("New directory name:\t%s",dirname);	cout<<endl;
	TDirectory* newdir = newfile->mkdir(dirname);
	newfile->cd(dirname);

	TDirectory* pdf_dir = newdir->mkdir("pdf");
	cout<<"Writing histograms to file...\n";
	pdf_dir->cd();
	error->Write();	error_to_n->Write();	error_to_e->Write();
	n_capture_pos->Write();		delete n_capture_pos;
	neutron_theta->Write();		delete neutron_theta;
	for(int i=0;i<4;i++){
		del_ang_n[i]->Write();
		del_ang_v[i]->Write();
		delete del_ang_n[i];	delete del_ang_v[i];
	}
	for(int i=0;i<2;i++){
		angle_v_recon[i]->Write();	delete angle_v_recon[i];
		angle_positron[i]->Write();	delete angle_positron[i];
	}

//===========================Sub-directory for integral distribution=====================================
	TDirectory* subdir = newdir->mkdir("intg_dist");
	subdir->cd();
	for(int i=4;i<8;i++){
		del_ang_n[i]->Write();	del_ang_v[i]->Write();
		delete del_ang_n[i];	delete del_ang_v[i];
	}
	for(int i=2;i<4;i++){angle_v_recon[i]->Write();	delete angle_v_recon[i];}
	accp_ratio_int->Write();	delete accp_ratio; delete accp_ratio_int;

	TDirectory* subdir2 = newdir->mkdir("en_angle");
	subdir2->cd();
	for(int i=0;i<N_theta;i++){
		theta_del_t[i]->Write();	delete theta_del_t[i];
	}

	cout<<"Program successful...\nSaving histograms to a new directory...\n";
	newfile->Write();	newfile->Close();
	delete newfile;
	PrintStatistics(Record);
	cout<<"Exiting...\n";
	rootfile->Close();
	return;
}
