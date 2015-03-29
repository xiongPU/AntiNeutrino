#include "TransitionEdgeSensor.hh"
#include "G4VProcess.hh"

TransitionEdgeSensor::TransitionEdgeSensor(G4String sd_name):G4VSensitiveDetector(sd_name),collectionID(-1){
	collectionName.insert("TES_hit_collection");
}

TransitionEdgeSensor::~TransitionEdgeSensor(){}

void TransitionEdgeSensor::Initialize(G4HCofThisEvent* HCE){
	if(collectionID<0) collectionID = GetCollectionID(0);	// safe because using only one hit collection
	hitsCollection = new TESHitsCollection("calorimeter",collectionName[0]);
	HCE->AddHitsCollection(collectionID,hitsCollection);
}

G4bool TransitionEdgeSensor::ProcessHits( G4Step* aStep, G4TouchableHistory* ROhist){

	G4StepPoint* post = aStep->GetPostStepPoint();
	G4StepPoint* pre = aStep->GetPreStepPoint();
	G4String particle_name = aStep->GetTrack()->GetDefinition()->GetParticleName();

//	G4cout<<pre->GetPosition()<<'\t'<<post->GetPosition()<<G4endl;
//	G4cout<<"Last Step?\t"<<aStep->IsLastStepInVolume()<<G4endl;
//	if(particle_name=="neutron"&& aStep->IsLastStepInVolume())
//		return true;

	TESHit* aHit = new TESHit();
	aHit->SetParticleName(particle_name);
	aHit->SetCellID(pre->GetTouchable()->GetReplicaNumber(0));
	aHit->SetPosition(pre->GetPosition());
	aHit->SetMomentum(pre->GetMomentum());
	aHit->SetGlobalTime(pre->GetGlobalTime());
	aHit->SetEDep(aStep->GetTotalEnergyDeposit());
	aHit->SetEnergy(pre->GetKineticEnergy());
	aHit->SetTrackStatus(aStep->GetTrack()->GetTrackStatus());
//	G4cout<<"track status is "<<aStep->GetTrack()->GetTrackStatus()<<G4endl;

	hitsCollection->insert(aHit);
	return true;
}

void TransitionEdgeSensor::EndOfEvent(G4HCofThisEvent* HCE){
//	G4cout<<"Event is finished...\n";
}
