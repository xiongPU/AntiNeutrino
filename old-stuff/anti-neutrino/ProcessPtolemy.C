// this file will process NTuple in the ROOT file created by PTOLEMY
// and plots various quantities of interest.

using namespace std;

void ProcessPtolemy(char* name, char* treename="events"){

	cout<<"This code will process file "<<name<<endl;
	cout<<"Opening ROOT file "<<name<<endl;

	TFile* rootfile = new TFile(name,"UPDATE");
	if(rootfile==0){
		std::cout<<"Cannot open the file...\n"<<name<<std::endl;
		return;
	}
	cout<<"ROOT file opened...\n";

	TTree* tree;
	rootfile->GetObject(treename,tree);
	if(tree==0){
		std::cout<<"Cannot retrieve the event tree...\n"<<std::endl;
		return;
	}
	cout<<"TTree object created...\n";

	Int_t Nevt = tree->GetEntries();
	Int_t Nbin = Nevt/100;
	if(Nbin>200) Nbin = 250;
//-------------------------- Variables to hold the values in the tree

	Double_t srcx,srcy,srcTheta,srcPhi,srcE;
	Double_t calx,caly,calTheta,calPhi,calE;

	cout<<"Setting branch addresses...\n";
	tree->SetBranchAddress( "srcx", &srcx);
	tree->SetBranchAddress( "srcy", &srcy);
	tree->SetBranchAddress( "src_theta", &srcTheta);
	tree->SetBranchAddress( "src_phi", &srcPhi);
	tree->SetBranchAddress( "srcE", &srcE);
	tree->SetBranchAddress( "calx", &calx);
	tree->SetBranchAddress( "caly", &caly);
	tree->SetBranchAddress( "cal_theta", &calTheta);
	tree->SetBranchAddress( "cal_phi", &calPhi);
	tree->SetBranchAddress( "calE", &calE);

	Double_t range_srcx[2],range_srcy[2],range_srctheta[2],range_srcphi[2],range_srcE[2];
	Double_t range_calx[2],range_caly[2],range_caltheta[2],range_calphi[2],range_calE[2];

	cout<<"Initializing range variables...\n";
	range_srcx[0] = -2.2;	range_srcx[1] = 2.2;
	range_srcy[0] = -2.2;	range_srcy[1] = 2.2;
	range_srctheta[0] = 0;	range_srctheta[1] = 1.6;
	range_srcphi[0] = 0;	range_srcphi[1] = 3.2;
	range_srcE[0] = 18.2;	range_srcE[1] = 18.6;

	range_calx[0] = -3.2;	range_calx[1] = 3.2;
	range_caly[0] = -3.2;	range_caly[1] = 3.2;
	range_caltheta[0] = 0;	range_caltheta[1] = 1.6;
	range_calphi[0] = 0;	range_calphi[1] = 3.2;
	range_calE[0] = 14.2;	range_calE[1] = 14.6;

//---------------------------creating histograms

	cout<<"Allocating memories for histograms...\n";

	TH1F* raw1D[10];	TH2F* raw2D[4];
	TH1F* pass1D[10];	TH2F* pass2D[4];
	TH1F* miss1D[5];	TH2F* miss2D[2];

	TH2F* pass_comp[3];	TH2F* miss_comp[3];

	TH1F* transmission_E;
	TH2F* transmission[2];

//--------------------------initialize histograms
//--------------------------RAW

	cout<<"Initializing histograms---RAW...\n";
	raw1D[0] = new TH1F("raw_srcx","raw_srcx",Nbin,range_srcx[0],range_srcx[1]);
	raw1D[1] = new TH1F("raw_srcy","raw_srcy",Nbin,range_srcy[0],range_srcy[1]);
	raw1D[2] = new TH1F("raw_srcTheta","raw_srcTheta",Nbin,range_srctheta[0],range_srctheta[1]);
	raw1D[3] = new TH1F("raw_srcPhi","raw_srcPhi",Nbin,range_srcphi[0],range_srcphi[1]);
	raw1D[4] = new TH1F("raw_srcE","raw_srcE",Nbin,range_srcE[0],range_srcE[1]);
	raw1D[5] = new TH1F("raw_calx","raw_calx",Nbin,range_calx[0],range_calx[1]);
	raw1D[6] = new TH1F("raw_caly","raw_caly",Nbin,range_caly[0],range_caly[1]);
	raw1D[7] = new TH1F("raw_calTheta","raw_calTheta",Nbin,range_caltheta[0],range_caltheta[1]);
	raw1D[8] = new TH1F("raw_calPhi","raw_calPhi",Nbin,range_calphi[0],range_calphi[1]);
	raw1D[9] = new TH1F("raw_calE","raw_calE",Nbin,range_calE[0],range_calE[1]);

	raw2D[0] = new TH2F("src_xy","src_xy",Nbin,range_srcx[0],range_srcx[1],Nbin,range_srcy[0],range_srcy[1]);
	raw2D[1] = new TH2F("src_ThetaPhi","src_ThetaPhi",Nbin,range_srctheta[0],range_srctheta[1],Nbin,range_srcphi[0],range_srcphi[1]);
	raw2D[2] = new TH2F("cal_xy","cal_xy",Nbin,range_calx[0],range_calx[1],Nbin,range_caly[0],range_caly[1]);
	raw2D[3] = new TH2F("cal_ThetaPhi","cal_ThetaPhi",Nbin,range_caltheta[0],range_caltheta[1],Nbin,range_calphi[0],range_calphi[1]);

//-------------------------PASSED
	cout<<"Initializing histograms---PASSED...\n";
	pass1D[0] = new TH1F("pass_srcx","pass_srcx",Nbin,range_srcx[0],range_srcx[1]);
	pass1D[1] = new TH1F("pass_srcy","pass_srcy",Nbin,range_srcy[0],range_srcy[1]);
	pass1D[2] = new TH1F("pass_srcTheta","pass_srcTheta",Nbin,range_srctheta[0],range_srctheta[1]);
	pass1D[3] = new TH1F("pass_srcPhi","pass_srcPhi",Nbin,range_srcphi[0],range_srcphi[1]);
	pass1D[4] = new TH1F("pass_srcE","pass_srcE",Nbin,range_srcE[0],range_srcE[1]);
	pass1D[5] = new TH1F("pass_calx","pass_calx",Nbin,range_calx[0],range_calx[1]);
	pass1D[6] = new TH1F("pass_caly","pass_caly",Nbin,range_caly[0],range_caly[1]);
	pass1D[7] = new TH1F("pass_calTheta","pass_calTheta",Nbin,range_caltheta[0],range_caltheta[1]);
	pass1D[8] = new TH1F("pass_calPhi","pass_calPhi",Nbin,range_calphi[0],range_calphi[1]);
	pass1D[9] = new TH1F("pass_calE","pass_calE",Nbin,range_calE[0],range_calE[1]);

	pass2D[0] = new TH2F("pass_srcxy","pass_srcxy",Nbin,range_srcx[0],range_srcx[1],Nbin,range_srcy[0],range_srcy[1]);
	pass2D[1] = new TH2F("pass_srcThetaPhi","pass_srcThetaPhi",Nbin,range_srctheta[0],range_srctheta[1],Nbin,range_srcphi[0],range_srcphi[1]);
	pass2D[2] = new TH2F("pass_calxy","pass_calxy",Nbin,range_calx[0],range_calx[1],Nbin,range_caly[0],range_caly[1]);
	pass2D[3] = new TH2F("pass_calThetaPhi","pass_calThetaPhi",Nbin,range_caltheta[0],range_caltheta[1],Nbin,range_calphi[0],range_calphi[1]);

//--------------------------MISSED
	cout<<"Initializing histograms---MISSED...\n";
	miss1D[0] = new TH1F("miss_srcx","miss_srcx",Nbin,range_srcx[0],range_srcx[1]);
	miss1D[1] = new TH1F("miss_srcy","miss_srcy",Nbin,range_srcy[0],range_srcy[1]);
	miss1D[2] = new TH1F("miss_srcTheta","miss_srcTheta",Nbin,range_srctheta[0],range_srctheta[1]);
	miss1D[3] = new TH1F("miss_srcPhi","miss_srcPhi",Nbin,range_srcphi[0],range_srcphi[1]);
	miss1D[4] = new TH1F("miss_srcE","miss_srcE",Nbin,range_srcE[0],range_srcE[1]);

	miss2D[0] = new TH2F("miss_srcxy","miss_srcxy",Nbin,range_srcx[0],range_srcx[1],Nbin,range_srcy[0],range_srcy[1]);
	miss2D[1] = new TH2F("miss_srcThetaPhi","miss_srcThetaPhi",Nbin,range_srctheta[0],range_srctheta[1],Nbin,range_srcphi[0],range_srcphi[1]);

//-------------------------COMPLEX
	cout<<"Initializing histograms---COMPLEX...\n";
	pass_comp[0] = new TH2F("pass_rPhi","pass_rPhi",Nbin,range_srcx[0],range_srcx[1],Nbin,range_srcphi[0],range_srcphi[1]);
	pass_comp[1] = new TH2F("pass_ThetaPhi","pass_ThetaPhi",Nbin, range_srctheta[0],range_srctheta[1],Nbin,range_srcphi[0],range_srcphi[1]);
	pass_comp[2] = new TH2F("pass_ThetaE","pass_ThetaE",Nbin,range_srctheta[0],range_srctheta[1],Nbin,range_srcE[0],range_srcE[1]);

	miss_comp[0] = new TH2F("miss_rPhi","miss_rPhi",Nbin,range_srcx[0],range_srcx[1],Nbin,range_srcphi[0],range_srcphi[1]);
	miss_comp[1] = new TH2F("miss_ThetaPhi","miss_ThetaPhi",Nbin, range_srctheta[0],range_srctheta[1],Nbin,range_srcphi[0],range_srcphi[1]);
	miss_comp[2] = new TH2F("miss_ThetaE","miss_ThetaE",Nbin,range_srctheta[0],range_srctheta[1],Nbin,range_srcE[0],range_srcE[1]);

//------------------------TRANSMISSION
	cout<<"Initializing histograms---TRANSMISSION...\n";
	transmission_E = new TH1F("trans_E","Transmission as function of energy",Nbin,range_srcE[0],range_srcE[1]);
	transmission[0] = new TH2F("trans_rE","Transmission as function of radius and energy",Nbin,range_srcx[0],range_srcx[1],Nbin,range_srcE[0],range_srcE[1]);
	transmission[1] = new TH2F("trans_ThetaE","Transmission as function of polar angle and energy",Nbin,range_srcx[0],range_srcx[1],Nbin,range_srcE[0],range_srcE[1]);
	transmission[2] = new TH2F("trans_rTheta","Transmission as function of radius and polar angle",Nbin,range_srcx[0],range_srcx[1],Nbin,range_srctheta[0],range_srctheta[1]);

	TH2F* total[3];
	total[0] = new TH2F("total_rE","temp",Nbin,range_srcx[0],range_srcx[1],Nbin,range_srcE[0],range_srcE[1]);
	total[1] = new TH2F("total_ThetaE","temp",Nbin,range_srcx[0],range_srcx[1],Nbin,range_srcE[0],range_srcE[1]);
	total[2] = new TH2F("total_rTheta","temp",Nbin,range_srcx[0],range_srcx[1],Nbin,range_srctheta[0],range_srctheta[1]);

//------------------------Begin to loop over the tree

	cout<<"Looping over the ROOT event tree...\n";
	for(int evt=0;evt<Nevt;evt++){
		tree->GetEntry(evt);

		Double_t r = sqrt(srcx*srcx+srcy*srcy);
		Double_t r_phi = atan2(srcy,srcx);
		Double_t delPhi = acos(cos(r_phi-srcPhi));

//-------------------------Fill RAW & TOTAL
		raw1D[0]->Fill(srcx);		raw1D[1]->Fill(srcy);
		raw1D[2]->Fill(srcTheta);	raw1D[3]->Fill(srcPhi);		raw1D[4]->Fill(srcE);
		raw1D[5]->Fill(calx);		raw1D[6]->Fill(caly);
		raw1D[7]->Fill(calTheta);	raw1D[8]->Fill(calPhi);		raw1D[9]->Fill(calE);

		raw2D[0]->Fill(srcx,srcy);
		raw2D[1]->Fill(srcTheta,srcPhi);
		raw2D[2]->Fill(calx,caly);
		raw2D[3]->Fill(calTheta,calPhi);

		total[0]->Fill(r,srcE);
		total[1]->Fill(srcTheta,srcE);
		total[2]->Fill(r,srcTheta);
//-------------------------Fill PASS
		if(calE>0){
			pass1D[0]->Fill(srcx);		pass1D[1]->Fill(srcy);
			pass1D[2]->Fill(srcTheta);	pass1D[3]->Fill(srcPhi);	pass1D[4]->Fill(srcE);
			pass1D[5]->Fill(calx);		pass1D[6]->Fill(caly);
			pass1D[7]->Fill(calTheta);	pass1D[8]->Fill(calPhi);	pass1D[9]->Fill(calE);

			pass2D[0]->Fill(srcx,srcy);
			pass2D[1]->Fill(srcTheta,srcPhi);
			pass2D[2]->Fill(calx,caly);
			pass2D[3]->Fill(calTheta,calPhi);	

			pass_comp[0]->Fill(r,delPhi);
			pass_comp[1]->Fill(srcTheta,delPhi);
			pass_comp[2]->Fill(srcTheta,srcE);

			transmission_E->Fill(srcE);
			transmission[0]->Fill(r,srcE);
			transmission[1]->Fill(srcTheta,srcE);
			transmission[2]->Fill(r,srcTheta);
		}
//------------------------Fill MISS
		else{
			miss1D[0]->Fill(srcx);		miss1D[1]->Fill(srcy);
			miss1D[2]->Fill(srcTheta);	miss1D[3]->Fill(srcPhi);	miss1D[4]->Fill(srcE);
			miss2D[0]->Fill(srcx,srcy);
			miss2D[1]->Fill(srcTheta,srcPhi);

			miss_comp[0]->Fill(r,delPhi);
			miss_comp[1]->Fill(srcTheta,delPhi);
			miss_comp[2]->Fill(srcTheta,srcE);
		}
	}

//-----------------------Process various transmission functions
	cout<<"Processing transmission functions...\n";
	Int_t NbinsX = transmission_E->GetNbinsX();
	Int_t NbinsY = 0;
	Double_t ratio = 0;

	for(int i=0;i<NbinsX;i++){
		Double_t a(0),b(0);
		a = raw1D[4]->GetBinContent(i);
		b = transmission_E->GetBinContent(i);
		if(a==0)
			ratio = 0;
		else	
			ratio = b/a;
		transmission_E->SetBinContent(i,ratio);
	}

	for(int i=0;i<3;i++){
		NbinsX = transmission[i]->GetNbinsX();
		NbinsY = transmission[i]->GetNbinsY();

		for(int j=0;j<NbinsX;j++)
			for(int k=0;k<NbinsY;k++){
				Double_t a(0),b(0);
				a = total[i]->GetBinContent(j,k);
				b = transmission[i]->GetBinContent(j,k);
				if(a==0)
					ratio = 0;
				else
					ratio = b/a;
				transmission[i]->SetBinContent(j,k,ratio);
			}
	}
	
	cout<<"Program successful...\nSaving histograms to a new directory...\n";

//	TFile* newfile = new TFile("ptolemy_processed.root");
	TDirectory* nd = rootfile->mkdir("Processed");

	if(nd==0){
		cout<<"The directory already exists...\n";
	}
	rootfile->cd("Processed");

	for(int i=0;i<10;i++)	raw1D[i]->Write();
	for(int i=0;i<4;i++)	raw2D[i]->Write();
	for(int i=0;i<10;i++)	pass1D[i]->Write();
	for(int i=0;i<4;i++)	pass2D[i]->Write();
	for(int i=0;i<5;i++)	miss1D[i]->Write();
	for(int i=0;i<2;i++)	miss2D[i]->Write();
	for(int i=0;i<3;i++)	pass_comp[i]->Write();
	for(int i=0;i<3;i++)	miss_comp[i]->Write();
	transmission_E->Write();
	for(int i=0;i<3;i++)	transmission[i]->Write();

	rootfile->Write();

	cout<<"Exiting...\n";
}
