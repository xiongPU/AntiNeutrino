// This  is the messenger class for PrimaryGeneratorAction class
#ifndef DETECTORMESSENGER_HH
#define DETECTORMESSENGER_HH 1

//#include "PrimaryGeneratorAction.hh"
#include "G4UIdirectory.hh"
#include "G4UIcommand.hh"
#include "G4UImessenger.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWith3VectorAndUnit.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "DetectorConstruction.hh"

class DetectorConstruction;

class DetectorMessenger:public G4UImessenger{

public:
	DetectorMessenger(DetectorConstruction* p);
	~DetectorMessenger();

	void SetNewValue(G4UIcommand* command, G4String newValue);
	G4String GetCurrentValue(G4UIcommand* command);

private:
	DetectorConstruction* detector;

	G4UIdirectory* newDir;

	G4UIcmdWithAnInteger* cmdNx;
	G4UIcmdWithAnInteger* cmdNy;
	G4UIcmdWithAnInteger* cmdNz;
	G4UIcmdWithADoubleAndUnit* cmddX;
	G4UIcmdWithADoubleAndUnit* cmddY;
	G4UIcmdWithADoubleAndUnit* cmddZ;
	G4UIcmdWithADoubleAndUnit* cmdGap;
};
#endif
