#ifndef PrimaryGeneratorAction_h
#define PrimaryGeneratorAction_h 1
#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4GeneralParticleSource.hh"
#include "DetectorConstruction.hh"
#include "GeneratorMessenger.hh"
#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4SystemOfUnits.hh"
#include "Randomize.hh"
#include "globals.hh"
#include "AntiNeutrino.hh"

class G4Event;
class DetectorConstruction;
class GeneratorMessenger;
class HistoManager;

class PrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction{

public:
	PrimaryGeneratorAction();    
	~PrimaryGeneratorAction();

	virtual void GeneratePrimaries(G4Event*);
	void SetSpectrum(G4String s)
		{generator_choice = s;}
	G4String GetSpectrum()
		{return generator_choice;}
	G4ParticleGun* GetParticleGun() 
		{return fParticleGun;};
	static PrimaryGeneratorAction* GetPointer()
		{ return fPrimaryGeneratorAction;}

//---------------------------Set method for manual mode----------------------------------
	void SetParticleNumber(G4int a)
		{particle_number = a;}
	G4int GetParticleNumber()
		{return particle_number;}
	void SetParticleName(G4String s)
		{particle_name = s;}
	G4String GetParticleName()
		{return particle_name;}
	void SetParticlePosition(G4ThreeVector t)
		{particle_position = t;}
	G4ThreeVector GetParticlePosition()
		{return particle_position;}
	void SetParticleMomentumDirection(G4ThreeVector t)
		{particle_momentum_dir = t;}
	G4ThreeVector GetParticleMomentumDirection()
		{return particle_momentum_dir;}
	void SetParticleEnergy(G4double e)
		{particle_energy = e;}
	G4double GetParticleEnergy()
		{return particle_energy;}

//---------------------------Get Method, default is e+ ----------------------------------
/*	G4double GetEnergyMin(G4String a="e+"){		// provided for Messenger
		if (a!="neutron") return lbin_ep;
		else return lbin_n;
	}
	void SetEnergyMin(G4double e, G4String a="e+"){
		if (a!="neutron") lbin_ep = e;
		else lbin_n = e;
	}
	G4double GetEnergyMax(G4String a="e+"){
		if (a!="neutron") return hbin_ep;
		else return hbin_n;
	}
	void SetEnergyMax(G4double e, G4String a="e+"){
		if (a!="neutron") hbin_ep = e;
		else hbin_n = e;
	}
*/	void SetCellIndex(G4int a, G4int b, G4int c){
		ix = a; iy = b; iz = c;
	}
	G4ThreeVector GetCellIndex(){
		return G4ThreeVector(ix, iy, iz);
	}

private:
	G4ParticleGun* fParticleGun;
	DetectorConstruction* fDetector;
	GeneratorMessenger* fGeneratorMessenger;
	static PrimaryGeneratorAction* fPrimaryGeneratorAction;

	G4String generator_choice;
	G4int ix, iy, iz;
//-------------------------------manual mode-----------------------------------
	G4int particle_number;
	G4String particle_name;
	G4double particle_energy;
	G4ThreeVector particle_position;
	G4ThreeVector particle_momentum_dir;
//-------------------------------spectrum mode---------------------------------
	AntiNeutrino* anti_v;
	G4double neutron_energy;
	G4ThreeVector neutron_position;
	G4ThreeVector neutron_momentum_dir;
	G4double time_delay;
	G4double positron_energy;
	G4ThreeVector positron_position;
	G4ThreeVector positron_momentum_dir;
//-------------------------------set method for spectrum mode----------------------
public:
	void SetTimeDelay(G4double a){time_delay = a;}	// time delay is used for distinguishing
	G4double GetTimeDelay(){return time_delay;}
//-------------------------------neutron-------------------------------------------

	G4double GetNeutrinoEnergy(){
		if (anti_v)return anti_v->GetNeutrinoEnergy();
		else return 0;
	}
	G4ThreeVector GetNeutrinoDirection(){
		if (anti_v)return anti_v->GetNeutrinoDirection();
		else return G4ThreeVector(0,0,1);
	}

	void SetNeutronPosition(G4ThreeVector t)
		{neutron_position = t;}
	G4ThreeVector GetNeutronPosition()
		{return neutron_position;}
	void SetNeutronEnergy(G4double e)
		{neutron_energy = e;}
	G4double GetNeutronEnergy()
		{return neutron_energy;}
	void SetNeutronMomentumDirection(G4ThreeVector t)
		{neutron_momentum_dir = t;}
	G4ThreeVector GetNeutronMomentumDirection()
		{return neutron_momentum_dir;}
//--------------------------------positron------------------------------------------
	void SetPositronPosition(G4ThreeVector t)
		{positron_position = t;}
	G4ThreeVector GetPositronPosition()
		{return positron_position;}
	void SetPositronEnergy(G4double e)
		{positron_energy = e;}
	G4double GetPositronEnergy()
		{return positron_energy;}
	void SetPositronMomentumDirection(G4ThreeVector t)
		{positron_momentum_dir = t;}
	G4ThreeVector GetPositronMomentumDirection()
		{return positron_momentum_dir;}
};

#endif
