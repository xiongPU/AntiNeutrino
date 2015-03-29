#ifndef AntiNeutrino_h
#define AntiNeutrino_h 1

#include "G4VUserPrimaryGeneratorAction.hh"
#include "DetectorConstruction.hh"

#include "G4SystemOfUnits.hh"
#include "Randomize.hh"
#include "globals.hh"

class AntiNeutrino{
public:
	static AntiNeutrino* GetPointer(){return fantiv;}

	AntiNeutrino(G4ThreeVector);		// initialize with a direction
	AntiNeutrino();
	~AntiNeutrino();

	void LoadData();
	void SetDistribution();
	G4ThreeVector GetNeutronDirection();
	G4ThreeVector GetPositronDirection();

	G4double GetNeutrinoEnergy(){return v_E;}
	G4ThreeVector GetNeutrinoDirection(){return v_dir;}
	void SetNeutrinoEnergy(G4double s){v_E = s; LoadData();}
	void SetNeutrinoDirection(G4ThreeVector s){v_dir = s.unit();}

	G4double GetNeutronEnergy();
	G4double GetPositronEnergy();

private:

	G4ThreeVector v_dir, n_dir, e_dir;
	G4double v_E, n_E, e_E;

	G4double* nEnergy;
	G4double* prob;		// probability distribution of neutron energy
	G4int N;		// total number of data points
	G4double lbin, hbin;
	G4double d;

	G4double* nAngle;	// neutron angular distribution
	G4double* prob_angle;
	G4int angle_N;
	G4double angle_lbin, angle_hbin;
	G4double angle_d;

	static AntiNeutrino* fantiv;
};
#endif
