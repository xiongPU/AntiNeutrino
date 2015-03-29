#ifndef RunAction_h
#define RunAction_h 1

#include "G4UserRunAction.hh"
#include "globals.hh"
#include "g4root.hh"

#include "DetectorConstruction.hh"
#include "PrimaryGeneratorAction.hh"
#include "HistoManager.hh"

#include "G4Run.hh"
#include "G4RunManager.hh"
#include "G4UnitsTable.hh"
#include "G4EmCalculator.hh"

#include "Randomize.hh"
#include <iomanip>

class G4Run;
class G4ParticleDefinition;
class G4Material;

class RunAction : public G4UserRunAction
{
public:
	RunAction(DetectorConstruction*, HistoManager*);
	~RunAction();
public:
	virtual void BeginOfRunAction(const G4Run*);
	virtual void   EndOfRunAction(const G4Run*);
public:

//	G4AnalysisManager* GetAnalysisManager(void) {return G4AnalysisManager::Instance();}

private:
	DetectorConstruction*   fDetector;
	PrimaryGeneratorAction* fPrimary;
	HistoManager* histo;
//	G4AnalysisManager* analysisManager;
};

#endif
