// This  is the messenger class for PrimaryGeneratorAction class
#ifndef GENERATORMESSENGER_HH
#define GENERATORMESSENGER_HH 1

//#include "PrimaryGeneratorAction.hh"
#include "G4UIdirectory.hh"
#include "G4UIcommand.hh"
#include "G4UImessenger.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWith3VectorAndUnit.hh"
#include "G4UIcmdWith3Vector.hh"
#include "PrimaryGeneratorAction.hh"

class PrimaryGeneratorAction;

class GeneratorMessenger:public G4UImessenger{

public:
	GeneratorMessenger(PrimaryGeneratorAction* p);
	~GeneratorMessenger();

	void SetNewValue(G4UIcommand* command, G4String newValue);
	G4String GetCurrentValue(G4UIcommand* command);

private:
	PrimaryGeneratorAction* generator;

	G4UIdirectory* newDir;

	G4UIcmdWithAString* cmdParticleName;
	G4UIcmdWithAString* cmdSpectrum;
	G4UIcmdWithADoubleAndUnit* cmdEnergy;
	G4UIcmdWith3VectorAndUnit* cmdPosition;
	G4UIcmdWith3Vector* cmdDirection;
	G4UIcmdWith3Vector* cmdCell;
	G4UIcmdWith3VectorAndUnit* cmdRange;
};
#endif
