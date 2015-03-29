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

class EventAction : public G4UserEventAction
{
public:
	EventAction( HistoManager*);
	~EventAction();

public:
	virtual void	BeginOfEventAction(const G4Event*);
	virtual void	EndOfEventAction(const G4Event*);

private:
	RunAction* fRunAction;
	HistoManager* histo;

	G4String filename;
	std::fstream file;
	               
};

#endif    
