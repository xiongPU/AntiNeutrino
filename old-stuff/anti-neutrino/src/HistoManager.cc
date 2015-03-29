#include <TH1D.h>
#include <TH2D.h>
#include <TFile.h>
#include <TTree.h>
#include <CLHEP/Units/SystemOfUnits.h>

#include "HistoManager.hh"
#include "G4UnitsTable.hh"

HistoManager::HistoManager(G4String name): filename(name), rootFile(0),ntupl(0){

	for (G4int k=0; k<MaxHisto1D; k++) histo1D[k] = 0;
	for (G4int k=0; k<MaxHisto2D; k++) histo2D[k] = 0;
}

HistoManager::~HistoManager(){
	if( rootFile) delete rootFile;
}

void HistoManager::book(){ 
	rootFile = new TFile(filename,"RECREATE");
	if(!rootFile){
		G4cout << " HistoManager::book :" << " problem creating the ROOT TFile "<< G4endl;
		return;
	}
   
	histo1D[0] = new TH1D("source_energy", "Source Energy Distribution", 500, 18.0, 18.6);
	if (!histo1D[0]) G4cout << "\n can't create histo 1" << G4endl;
	histo1D[1] = new TH1D("calorimeter_energy", "Calorimeter Hit Energy Distribution", 500, 13., 15.0);
	if (!histo1D[1]) G4cout << "\n can't create histo 2" << G4endl;
	histo1D[2] = new TH1D("missed_source_energy", "Energy distribution that did not pass MAC-E", 500, 18.0, 18.6);
	if (!histo1D[2]) G4cout << "\n can't create histo 3" << G4endl;
	histo1D[3] = new TH1D("missed_theta", "Polar angle distribution of e- that did not pass MAC-E", 200, 0,1.6);
	if (!histo1D[3]) G4cout << "\n can't create histo 3" << G4endl;

	histo2D[0] = new TH2D("source_position", "Source Position Distribution", 100, -15., 15., 100, -15., 15.);
	if (!histo2D[0]) G4cout << "\n can't create histo 3" << G4endl;
	histo2D[1] = new TH2D("calorimeter_position", "Calorimeter Hit Position Distribution", 100, -30., 30., 100, -30., 30.);
	if (!histo2D[1]) G4cout << "\n can't create histo 4" << G4endl;  
	histo2D[2] = new TH2D("missed_source_position", "Source Position Distribution of Missed e-", 100, -15., 15., 100, -15., 15.);
	if (!histo2D[2]) G4cout << "\n can't create histo 3" << G4endl;
	histo2D[3] = new TH2D("missed_theta_phi", "Angular distribution of missed e-", 100, 0., 1.6, 100, -3.2, 3.2);
	if (!histo2D[3]) G4cout << "\n can't create histo 4" << G4endl;

	ntupl = new TTree("events", "electron phase-space");
	ntupl->Branch("srcx", &(pos_inix), "src_x/D");
	ntupl->Branch("srcy", &(pos_iniy), "src_y/D");
	ntupl->Branch("srcz", &(pos_iniz), "src_z/D");
	ntupl->Branch("src_theta", &theta_ini, "src_theta/D");
	ntupl->Branch("src_phi", &phi_ini, "src_phi/D");
	ntupl->Branch("srcE", &e_ini, "src_E/D");

	ntupl->Branch("calx", &(pos_finx), "cal_x/D");
	ntupl->Branch("caly", &(pos_finy), "cal_y/D");
	ntupl->Branch("calz", &(pos_finz), "cal_z/D");
	ntupl->Branch("cal_theta", &theta_fin, "cal_theta/D");
	ntupl->Branch("cal_phi", &phi_fin, "cal_phi/D");
	ntupl->Branch("calE", &e_fin, "cal_E/D");
 
	G4cout << "\n----> Histogram file is opened in " << filename << G4endl;
}

void HistoManager::save(){ 

	if (rootFile){
		rootFile->Write();
		rootFile->Close();
		G4cout << "\n----> Histogram Tree is saved \n" << G4endl;
	}
}

void HistoManager::FillHisto1D(G4int ih, G4double xbin, G4double weight){

	if (ih >= MaxHisto1D) {
		G4cout << "---> warning from HistoManager::FillHisto() : histo " << ih
			 << " does not exist. (xbin=" << xbin << " weight=" << weight << ")"
			<< G4endl;
		return;
	}
	if  (histo1D[ih]) { histo1D[ih]->Fill(xbin, weight); }
}

void HistoManager::FillHisto2D(G4int ih, G4double xbin, G4double ybin, G4double weight){

	if (ih >= MaxHisto2D) {
		G4cout << "---> warning from HistoManager::FillHisto() : histo " << ih
			<< " does not exist. (xbin=" << xbin << " weight=" << weight << ")"
			<< G4endl;
		return;
	}
	if  (histo2D[ih]) { histo2D[ih]->Fill(xbin, ybin, weight); }
}

void HistoManager::Normalize1D(G4int ih, G4double fac)
{
	if (ih >= MaxHisto1D) {
		G4cout << "---> warning from HistoManager::Normalize() : histo " << ih
			<< " does not exist. (fac=" << fac << ")" << G4endl;
		return;
	}
	if (histo1D[ih]) histo1D[ih]->Scale(fac);
}

void HistoManager::Normalize2D(G4int ih, G4double fac)
{
	if (ih >= MaxHisto2D) {
		G4cout << "---> warning from HistoManager::Normalize() : histo " << ih
			<< " does not exist. (fac=" << fac << ")" << G4endl;
		return;
	}
	if (histo2D[ih]) histo2D[ih]->Scale(fac);
}

void HistoManager::FillNtuple(G4ThreeVector pos1, G4ThreeVector p1, G4double e1, G4ThreeVector pos2, G4ThreeVector p2, G4double e2){

	pos_inix = pos1.x();	pos_finx = pos2.x();
	pos_iniy = pos1.y();	pos_finy = pos2.y();
	pos_iniz = pos1.z();	pos_finz = pos2.z();

	e_ini = e1;	e_fin = e2;
	theta_ini = p1.getTheta();
	theta_fin = p2.getTheta();
	phi_ini = p1.getPhi();
	phi_fin = p2.getPhi();

	if (ntupl) ntupl->Fill();
}
