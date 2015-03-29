#ifndef PrimaryGeneratorAction_h
#define PrimaryGeneratorAction_h 1

#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4GeneralParticleSource.hh"

#include "DetectorConstruction.hh"

#include "G4Event.hh"

#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4SystemOfUnits.hh"
#include "Randomize.hh"
#include "globals.hh"

class G4Event;
class DetectorConstruction;
class Tritium;


class PrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction{

public:
	PrimaryGeneratorAction();    
	~PrimaryGeneratorAction();

public:
	virtual void GeneratePrimaries(G4Event*);
	G4ParticleGun* GetParticleGun() {return fParticleGun;};
//	G4GeneralParticleSource* GetParticleGun() {return fParticleGun;};

private:
	G4ParticleGun*        fParticleGun;
//	G4GeneralParticleSource*        fParticleGun;
	DetectorConstruction* fDetector;

public:
	void SetParticleNumber(G4int a){particle_number = a;}
	void SetParticleName(G4String s){particle_name = s;}
	void SetParticleEnergy(G4double e){particle_energy = e;}
	void SetParticleMomentumDirection(G4ThreeVector t){particle_momentum_dir = t;}
	void SetParticlePosition(G4ThreeVector t){particle_position = t;}

	G4int GetParticleNumber(){return particle_number;}
	G4String GetParticleName(){return particle_name;}
	G4double GetParticleEnergy(){return particle_energy;}
	G4ThreeVector GetParticlePosition(){return particle_position;}
	G4ThreeVector GetParticleMomentumDir(){return particle_momentum_dir;}

	static PrimaryGeneratorAction* GetPrimGenPointer(){ return fPrimaryGeneratorAction;}

private:
	G4double EnergyDistribution(G4double);//returns probability of acceptance in MC sampling
	G4int particle_number;
	G4String particle_name;
	G4double particle_energy;
	G4ThreeVector particle_position;
	G4ThreeVector particle_momentum_dir;
	Tritium* tritium_src;
private:
	static PrimaryGeneratorAction* fPrimaryGeneratorAction;
};

class Tritium{

public:
	Tritium(G4String);
	~Tritium();
	G4double GetEnergy();

private:
	G4double* T;
	G4double* prob;
	G4int N;
	G4double lbin;
	G4double hbin;
	G4double d;
};

#endif
