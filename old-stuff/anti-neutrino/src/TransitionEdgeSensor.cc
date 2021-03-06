#include "TransitionEdgeSensor.hh"

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

	TESHit* aHit = new TESHit();

	G4StepPoint* post = aStep->GetPostStepPoint();
	G4StepPoint* pre = aStep->GetPreStepPoint();

	aHit->SetParticleName(aStep->GetTrack()->GetDefinition()->GetParticleName());
	aHit->SetPosition(pre->GetPosition());
	aHit->SetDeltaPos(aStep->GetDeltaPosition());
	aHit->SetMomentum(pre->GetMomentum());
	aHit->SetDeltaMomentum(post->GetMomentum()-pre->GetMomentum());
	aHit->SetEDep(aStep->GetTotalEnergyDeposit());
	aHit->SetEnergy(pre->GetKineticEnergy());
	aHit->SetGlobalTime(pre->GetGlobalTime());

	hitsCollection->insert(aHit);
	return true;
}

void TransitionEdgeSensor::EndOfEvent(G4HCofThisEvent* HCE){
//	G4cout<<"Event is finished...\n";
}
