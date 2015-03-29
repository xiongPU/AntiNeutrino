#ifndef TRANSITIONEDGESENSOR_hh
#define TRANSITIONEDGESENSOR_hh 1

#include "G4VSensitiveDetector.hh"
#include "G4SDManager.hh"
#include "TESHit.hh"
#include "G4HCofThisEvent.hh"
#include "G4Step.hh"
#include "G4String.hh"

class TransitionEdgeSensor: public G4VSensitiveDetector{
public:
	TransitionEdgeSensor( G4String name);
	virtual ~TransitionEdgeSensor();
	virtual void Initialize( G4HCofThisEvent* HCE);
	virtual G4bool ProcessHits( G4Step* step, G4TouchableHistory* ROhist);
	virtual void EndOfEvent(G4HCofThisEvent* HCE);
private:
	TESHitsCollection* hitsCollection;
	G4int collectionID;
};

#endif
