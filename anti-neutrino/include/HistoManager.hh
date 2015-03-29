#ifndef HistoManager_h
#define HistoManager_h 1

#include "globals.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"
#include "globals.hh"
#include "G4Step.hh"
#include "PrimaryGeneratorAction.hh"

class TFile;
class TTree;
class TH1D;
class TH2D;

const G4int MaxHisto1D = 2;
const G4int MaxHisto2D = 2;

class HistoManager{

public:
  
	HistoManager(G4String name = "antineutrino.root");
	~HistoManager();
   
	void book();
	void save();

	void FillHisto1D(G4int id, G4double bin, G4double weight = 1.0);
	void FillHisto2D(G4int id, G4double binx, G4double biny, G4double weight = 1.0);
	static HistoManager* GetPointer(){return fhisto;}
//	void FillNtuple(G4int, G4int, G4ThreeVector, G4ThreeVector, G4double, G4int, G4ThreeVector, G4ThreeVector, G4double, G4int, G4ThreeVector, G4double, G4double, G4double, G4double,G4int, G4ThreeVector, G4double, G4double);

	void FillNeutrino(G4int fect, G4ThreeVector, G4double);
	void FillPositron(G4ThreeVector fr_positron_i, G4ThreeVector fp_positron_i, G4double fE_positron_i,G4ThreeVector fpositron_cap_pos, G4double fE_pos_primary, G4double fE_pos_secondary, G4double fE_em_primary, G4double fE_em_secondary);
	void FillNeutron(G4ThreeVector fr_neutron_i, G4ThreeVector fp_neutron_i, G4double fE_neutron_i,G4ThreeVector fneutron_cap_pos, G4double fn_cap_time, G4double felastic_angle);

private:
	static HistoManager* fhisto;

	G4String filename;
	TFile* rootFile;

	TH1D* histo1D[MaxHisto1D];
	TH2D* histo2D[MaxHisto2D];
	TTree* ntuple;

	PrimaryGeneratorAction* primgen;

	G4int evtID;
	G4ThreeVector v_dir;
	G4double vE;
	G4int positron_cellID_i, positron_cellID_f, neutron_cellID_i, neutron_cellID_f;
	G4double r_pos[3], r_neu[3], r_pos_cap[3], r_neu_cap[3];
	G4double p_pos[3], p_neu[3];
	G4double E_positron, E_neutron;
	G4double n_cap_time;
	G4double E_pos_primary, E_pos_secondary;
	G4double E_em_primary, E_em_secondary;

	G4double pos_ctri[3], pos_ctrf[3], n_ctri[3], n_ctrf[3];
	G4double del_angle_true, del_angle_exp, elastic_angle;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

