#include "EventAction.hh"
#include <cmath>

EventAction::EventAction( HistoManager* h):histo(h){
//	fEventMessenger = new EventMessenger(this);
}

EventAction::~EventAction(){
//	delete fEventMessenger;
}

void EventAction::BeginOfEventAction(const G4Event* evt){

	G4int evtID = evt->GetEventID();
	G4cout<<"---------- Beginning Event "<<evtID<<" ----------"<<G4endl;
}


void EventAction::EndOfEventAction(const G4Event* evt){

	G4int evtID = evt->GetEventID();
	G4cout<<"---------- Processing Event "<<evtID<<" ----------"<<G4endl;

	PrimaryGeneratorAction* fPrimGen = PrimaryGeneratorAction::GetPrimGenPointer();
	G4ThreeVector pos_ini = fPrimGen->GetParticlePosition();
	G4ThreeVector p_ini = (fPrimGen->GetParticleMomentumDir()).unit();
	G4double E_ini = fPrimGen->GetParticleEnergy();

	histo->FillHisto2D(0, pos_ini.x()/mm, pos_ini.y()/mm);

	G4String colName = "TES_hit_collection";

	G4SDManager* sdman = G4SDManager::GetSDMpointer();
	G4int collectionID = sdman->GetCollectionID(colName);

	G4int NofHits = 0;
	G4ThreeVector hit_pos(NAN,NAN,NAN), hit_p(NAN,NAN,NAN);
	G4double hit_energy(NAN);
	TESHit* hit;

	if(evt->GetHCofThisEvent()){
		TESHitsCollection* TES_HC = (TESHitsCollection*)evt->GetHCofThisEvent()->GetHC(collectionID);
		NofHits = TES_HC->GetSize();

		if(TES_HC && NofHits>0){
			for( int i=0; i<NofHits; i++){
					hit = (*TES_HC)[i];
					hit_pos = hit -> GetPosition();
					hit_p = (hit -> GetMomentum()).unit();
					hit_energy = hit ->GetEnergy();

					histo->FillHisto1D(0, E_ini/keV);
					histo->FillHisto1D(1, hit_energy/keV);
					histo->FillHisto2D(1,hit_pos.x()/mm, hit_pos.y()/mm);
			}
		}
	}

	if(NofHits==0){
		histo->FillHisto1D(2, E_ini/keV);
		histo->FillHisto1D(3, p_ini.getTheta());
		histo->FillHisto2D(2, pos_ini.x(),pos_ini.y());
		histo->FillHisto2D(3, p_ini.getTheta(), p_ini.getPhi());
	}

	histo->FillNtuple(pos_ini/mm, p_ini, E_ini/keV, hit_pos/mm, hit_p, hit_energy/keV);
}
