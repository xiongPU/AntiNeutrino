#ifndef EventAction_h
#define EventAction_h 1

#include "G4UserEventAction.hh"
#include "globals.hh"

#include "RunAction.hh"
#include "HistoManager.hh"
//#include "EventMessenger.hh"
#include "PrimaryGeneratorAction.hh"

#include "G4Event.hh"
#include "G4SDManager.hh"
#include "TESHit.hh"
#include "TransitionEdgeSensor.hh"
#include <fstream>
#include <vector>

class EventAction : public G4UserEventAction
{
public:
	EventAction( HistoManager*);
	~EventAction();

public:
	virtual void	BeginOfEventAction(const G4Event*);
	virtual void	EndOfEventAction(const G4Event*);
	void AddReflectionPoint(const G4Step* step){
		pos_ref = (step->GetPreStepPoint()->GetPosition());
		momentum_ref = (step->GetPreStepPoint()->GetMomentumDirection());
		e_ref = (step->GetPreStepPoint()->GetKineticEnergy());
		G4cout<<"Adding: "<<e_ref/keV<<G4endl;
	}

private:
	RunAction* fRunAction;
	HistoManager* histo;
	G4bool check_reflection;

	G4String filename;
	std::fstream file;
	G4ThreeVector pos_ref;
	G4ThreeVector momentum_ref;
	G4double e_ref;

	G4int* plotted;
//	vector<TESHit> positron_hit;
//	vector<TESHit> neutron_hit;
};

#endif    
