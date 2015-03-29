#include <TH1D.h>
#include <TH2D.h>
#include <TFile.h>
#include <TTree.h>
#include <CLHEP/Units/SystemOfUnits.h>
#include "TargetParam.hh"
#include "PrimaryGeneratorAction.hh"
#include "HistoManager.hh"
#include "G4UnitsTable.hh"

class AntiNeutrino;

HistoManager* HistoManager::fhisto = 0;

HistoManager::HistoManager(G4String name): filename(name), rootFile(0),ntuple(0){
	primgen = PrimaryGeneratorAction::GetPointer();
	fhisto = this;

//------------------------------------------------------------------------------
	evtID=0;	positron_cellID_i = positron_cellID_f = -1;
	neutron_cellID_i = neutron_cellID_f = -1;
	vE = NAN;

	for(int i=0;i<3;i++){
		v_dir[i] = NAN;
		r_pos[i] = NAN;	r_neu[i] = NAN;
		p_pos[i] = NAN;	p_neu[i] = NAN;
		r_pos_cap[i] = NAN;	r_neu_cap[i] = NAN;
		pos_ctri[i] = NAN;	pos_ctrf[i] = NAN;
		n_ctri[i] = NAN;	n_ctrf[i] = NAN;
	}

	E_positron = NAN; E_neutron = NAN;
	n_cap_time = NAN;

	E_pos_primary = 0;	E_pos_secondary = 0;
	E_em_primary = 0;	E_em_secondary = 0;

	del_angle_true = NAN;	del_angle_exp = NAN;
	elastic_angle = NAN;
}

HistoManager::~HistoManager(){
	if( rootFile) delete rootFile;
	fhisto = 0;
}

void HistoManager::book(){ 
	rootFile = new TFile(filename,"RECREATE");
	if(!rootFile){
		G4cout << " HistoManager::book :" << " problem creating the ROOT TFile "<< G4endl;
		return;
	}
	ntuple = new TTree("an_det", "Anti-neutrino detector");

	ntuple->Branch("evt", &evtID,"evt/I");
	ntuple->Branch("v_dirx",&v_dir[0],"v_dirx/D");
	ntuple->Branch("v_diry",&v_dir[1],"v_diry/D");
	ntuple->Branch("v_dirz",&v_dir[2],"v_dirz/D");
	ntuple->Branch("v_E",&vE,"v_E/D");

//--------------------------e+ initial/final cell ID and coordinate--------------------------
/*	ntuple->Branch("pos_celli", &positron_cellID_i, "pos_celli/I");
	ntuple->Branch("pos_cellf", &positron_cellID_f, "pos_cellf/I");
	ntuple->Branch("pos_ctrxi", &pos_ctri[0],"pos_ctrxi/D");
	ntuple->Branch("pos_ctryi", &pos_ctri[1],"pos_ctryi/D");
	ntuple->Branch("pos_ctrzi", &pos_ctri[2],"pos_ctrzi/D");
	ntuple->Branch("pos_ctrxf", &pos_ctrf[0],"pos_ctrxf/D");
	ntuple->Branch("pos_ctryf", &pos_ctrf[1],"pos_ctryf/D");
	ntuple->Branch("pos_ctrzf", &pos_ctrf[2],"pos_ctrzf/D");
*/
//---------------------------e+ position, momentum and energy-------------------------------
	ntuple->Branch("pos_rx", &(r_pos[0]), "pos_rx/D");
	ntuple->Branch("pos_ry", &(r_pos[1]), "pos_ry/D");
	ntuple->Branch("pos_rz", &(r_pos[2]), "pos_rz/D");
	ntuple->Branch("pos_theta", &(p_pos[0]), "pos_theta/D");
	ntuple->Branch("pos_phi", &(p_pos[1]), "pos_phi/D");
	ntuple->Branch("pos_E", &(E_positron), "pos_E/D");

//---------------------------neutron cell IDs----------------------------------------------
/*	ntuple->Branch("n_celli", &neutron_cellID_i, "n_celli/I");
	ntuple->Branch("n_cellf", &neutron_cellID_f, "n_cellf/I");
	ntuple->Branch("n_ctrxi", &n_ctri[0],"n_ctrxi/D");
	ntuple->Branch("n_ctryi", &n_ctri[1],"n_ctryi/D");
	ntuple->Branch("n_ctrzi", &n_ctri[2],"n_ctrzi/D");
	ntuple->Branch("n_ctrxf", &n_ctrf[0],"n_ctrxf/D");
	ntuple->Branch("n_ctryf", &n_ctrf[1],"n_ctryf/D");
	ntuple->Branch("n_ctrzf", &n_ctrf[2],"n_ctrzf/D");
*/
//---------------------------neutron position, energy-------------------------------------
	ntuple->Branch("n_rx", &(r_neu[0]), "n_rx/D");
	ntuple->Branch("n_ry", &(r_neu[1]), "n_ry/D");
	ntuple->Branch("n_rz", &(r_neu[2]), "n_rz/D");
	ntuple->Branch("n_theta", &(p_neu[0]), "n_theta/D");
	ntuple->Branch("n_phi", &(p_neu[1]), "n_phi/D");
	ntuple->Branch("n_E", &(E_neutron), "n_E/D");

//	ntuple->Branch("pos_cellf", &positron_cellID_f, "pos_cellf/I");
	ntuple->Branch("pos_cap_rx", &(r_pos_cap[0]), "pos_cap_rx/D");// e+ capture
	ntuple->Branch("pos_cap_ry", &(r_pos_cap[1]), "pos_cap_ry/D");
	ntuple->Branch("pos_cap_rz", &(r_pos_cap[2]), "pos_cap_rz/D");

	ntuple->Branch("pos_E_prim", &E_pos_primary, "pos_E_prim/D");// Edep due to e+ EM process
	ntuple->Branch("pos_E_snd", &E_pos_secondary, "pos_E_snd/D");// in secondary cell
	ntuple->Branch("em_E_prim", &E_em_primary, "em_E_prim/D");// due to secondary e-
	ntuple->Branch("em_E_snd", &E_em_secondary, "em_E_snd/D");

//	ntuple->Branch("n_cellf", &neutron_cellID_f, "n_cellf/I");
	ntuple->Branch("n_cap_rx", &(r_neu_cap[0]), "n_cap_rx/D");// neutron capture
	ntuple->Branch("n_cap_ry", &(r_neu_cap[1]), "n_cap_ry/D");
	ntuple->Branch("n_cap_rz", &(r_neu_cap[2]), "n_cap_rz/D");
	ntuple->Branch("n_cap_time",&n_cap_time,"n_cap_time/D");

	ntuple->Branch("del_angle_n",&del_angle_true,"del_angle_n/D");	// wrt neutron
	ntuple->Branch("del_angle_v",&del_angle_exp,"del_angle_v/D");	// wrt neutrino
	ntuple->Branch("elastic_angle",&elastic_angle,"elastic_angle/D");// wrt n initial angle

	G4cout << "\n----> Histogram file is opened in " << filename << G4endl;
}

void HistoManager::save(){ 

	if (rootFile){
		rootFile->Write();
		rootFile->Close();
		G4cout << "\n----> Histogram Tree is saved \n" << G4endl;
	}
}
//===============================================================================================
//	Implementing all kinds of Fill methods.

void HistoManager::FillNeutrino(G4int fevt, G4ThreeVector fv_dir, G4double fvE){
	evtID = fevt;
	v_dir[0] = fv_dir.x();	v_dir[1] = fv_dir.y();	v_dir[2] = fv_dir.z();
	vE = fvE;
}

void HistoManager::FillPositron(G4ThreeVector fr_positron_i, G4ThreeVector fp_positron_i, G4double fE_positron_i,G4ThreeVector fpositron_cap_pos, G4double fE_pos_primary, G4double fE_pos_secondary, G4double fE_em_primary, G4double fE_em_secondary){
	for(int i=0;i<3;i++){
		r_pos[i] = fr_positron_i[i]/cm;
		r_pos_cap[i] = fpositron_cap_pos[i]/cm;
	}
	p_pos[0] = fp_positron_i.getTheta();
	p_pos[1] = fp_positron_i.getPhi();

	E_positron = fE_positron_i/keV;
	E_pos_primary = fE_pos_primary/keV;	E_pos_secondary = fE_pos_secondary/keV;
	E_em_primary = fE_em_primary/keV;	E_em_secondary = fE_em_secondary/keV;
}

void HistoManager::FillNeutron(G4ThreeVector fr_neutron_i, G4ThreeVector fp_neutron_i, G4double fE_neutron_i, G4ThreeVector fneutron_cap_pos, G4double fn_cap_time, G4double felastic_angle){
	for(int i=0;i<3;i++){
		r_neu[i] = fr_neutron_i[i]/cm;
		r_neu_cap[i] = fneutron_cap_pos[i]/cm;
	}

	p_neu[0] = fp_neutron_i.getTheta();
	p_neu[1] = fp_neutron_i.getPhi();
	E_neutron = fE_neutron_i/keV;
	n_cap_time = fn_cap_time/ns;

	elastic_angle = felastic_angle;

	G4ThreeVector v_dir = AntiNeutrino::GetPointer()->GetNeutrinoDirection();
	G4ThreeVector n_vec = fneutron_cap_pos-fr_neutron_i;

	if(isnan(fneutron_cap_pos.x())){
		del_angle_true = NAN;	del_angle_exp = NAN;
	}
	else{
		del_angle_true = acos((fp_neutron_i.unit()).dot(n_vec.unit()));
		del_angle_exp = acos((v_dir.unit()).dot(n_vec.unit()));
	}
	elastic_angle = felastic_angle;

	if(ntuple) ntuple->Fill();
}
