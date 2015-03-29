#include "SteppingAction.hh"

#include "RunAction.hh"
#include "EventAction.hh"
#include "G4Step.hh"
#include "globals.hh"

SteppingAction::SteppingAction(RunAction* RA, EventAction* EA):fRunaction(RA), fEventaction(EA){
	histo = HistoManager::GetPointer();
}

SteppingAction::~SteppingAction(){
}

void SteppingAction::UserSteppingAction(const G4Step* fStep){
/*
	G4Track* fTrack = fStep->GetTrack();
	G4ParticleDefinition* thePartDef = fTrack->GetDefinition();
	G4String partType= fTrack->GetDefinition()->GetParticleType();

	if( (partType == "lepton") && (thePartDef->GetPDGStable()) ) {
		if(fStep->GetPostStepPoint()->GetMomentumDirection().z()<0){
			histo->AddReflectionPoint(fStep);
			fTrack->SetTrackStatus( fStopAndKill);
		}
//		if(fTrack->GetLocalTime()/second > 100.0e-6)  fTrack->SetTrackStatus( fStopAndKill );
	}*/
}
