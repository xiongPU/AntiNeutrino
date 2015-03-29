#include "SteppingAction.hh"

#include "RunAction.hh"
#include "EventAction.hh"

#include "G4Step.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SteppingAction::SteppingAction(RunAction* RA, EventAction* EA):fRunaction(RA), fEventaction(EA){
	G4cout<<"Initializing Stepping Action...\n";
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SteppingAction::~SteppingAction(){
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SteppingAction::UserSteppingAction(const G4Step* fStep){

	G4Track* fTrack = fStep->GetTrack();
	G4ParticleDefinition* thePartDef = fTrack->GetDefinition();
	G4String partType= fTrack->GetDefinition()->GetParticleType();

	if( (partType == "lepton") && (thePartDef->GetPDGStable()) ) {
		if(fTrack->GetStep()->GetPostStepPoint()->GetMomentumDirection().getTheta()>pi/2) fTrack->SetTrackStatus( fStopAndKill);
		if(fTrack->GetLocalTime()/second > 100.0e-6)  fTrack->SetTrackStatus( fStopAndKill );
	}
}
