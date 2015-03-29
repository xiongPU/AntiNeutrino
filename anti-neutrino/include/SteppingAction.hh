#ifndef SteppingAction_h
#define SteppingAction_h 1

#include "G4UserSteppingAction.hh"
#include "globals.hh"
#include "RunAction.hh"
#include "EventAction.hh"
#include "HistoManager.hh"

class RunAction;
class EventAction;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class SteppingAction : public G4UserSteppingAction
{
public:
	SteppingAction(RunAction*,EventAction* );
	~SteppingAction();

	virtual void UserSteppingAction(const G4Step*);

private:
	RunAction*	fRunaction;    
	EventAction*	fEventaction;
	HistoManager* histo;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
