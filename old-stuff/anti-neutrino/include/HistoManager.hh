#ifndef HistoManager_h
#define HistoManager_h 1

#include "globals.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"
#include "globals.hh"

class TFile;
class TTree;
class TH1D;
class TH2D;

const G4int MaxHisto1D = 4;
const G4int MaxHisto2D = 4;


class HistoManager{

public:
  
	HistoManager(G4String name = "PTOLEMY_phasespace.root");
	~HistoManager();
   
	void book();
	void save();

	void FillHisto1D(G4int id, G4double bin, G4double weight = 1.0);
	void FillHisto2D(G4int id, G4double binx, G4double biny, G4double weight = 1.0);
	void Normalize1D(G4int id, G4double fac);
	void Normalize2D(G4int id, G4double fac);

	void FillNtuple(G4ThreeVector, G4ThreeVector, G4double, G4ThreeVector, G4ThreeVector, G4double);
    	       
private:
	G4String filename;  
	TFile* rootFile;
	TH1D* histo1D[MaxHisto1D];
	TH2D* histo2D[MaxHisto2D];
	TTree* ntupl;

	G4double pos_inix, pos_iniy, pos_iniz;
	G4double theta_ini;
	G4double phi_ini;
	G4double e_ini;

	G4double pos_finx, pos_finy, pos_finz;
	G4double theta_fin;
	G4double phi_fin;
	G4double e_fin;
};

#endif
