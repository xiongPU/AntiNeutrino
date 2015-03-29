#include "EventAction.hh"

#ifdef G4VIS_USE
#include "G4VVisManager.hh"
#include "G4Color.hh"
#include "G4VisAttributes.hh"
#endif

#include <cmath>

EventAction::EventAction( HistoManager* h):histo(h){
//	fEventMessenger = new EventMessenger(this);
//	DetectorConstruction* dptr = DetectorConstruction::GetPointer();
//	G4int N = (dptr->GetNx())*(dptr->GetNy())*(dptr->GetNz());
//	plotted = new G4int[N];
//	for(int i=0;i<N;i++)
//		{plotted[i] = -1;}
}

EventAction::~EventAction(){
//	delete fEventMessenger;
//	if(plotted) delete [] plotted;
//	plotted = NULL;
}

void EventAction::BeginOfEventAction(const G4Event* evt){
	G4int evtID = evt->GetEventID();
	G4cout<<"-----> Beginning Event "<<evtID<<" ----------"<<G4endl;
}

void EventAction::EndOfEventAction(const G4Event* evt){

	G4int evtID = evt->GetEventID();
	G4cout<<"-----> Processing Event "<<evtID<<" ----------"<<G4endl;

//	time-delay is used to seperate n events from e+ events (which is fast).
	DetectorConstruction* dptr = DetectorConstruction::GetPointer();
	PrimaryGeneratorAction* fPrimGen = PrimaryGeneratorAction::GetPointer();
	G4double time_delay = -fPrimGen->GetTimeDelay();

//============================Initial information===================================
//	Event is produced in pairs, so both e+ and n energy should be well defined.

	G4ThreeVector r_positron_i = fPrimGen->GetPositronPosition();
	G4ThreeVector p_positron_i = fPrimGen->GetPositronMomentumDirection().unit();
	G4double E_positron_i = fPrimGen->GetPositronEnergy();

	G4ThreeVector r_neutron_i = fPrimGen->GetNeutronPosition();
	G4ThreeVector p_neutron_i = fPrimGen->GetNeutronMomentumDirection().unit();
	G4double E_neutron_i = fPrimGen->GetNeutronEnergy();

//==================================================================================
	G4String colName = "TES_hit_collection";
	G4SDManager* sdman = 0;	sdman = G4SDManager::GetSDMpointer();
	G4int collectionID = -1;	collectionID = sdman->GetCollectionID(colName);
	if(collectionID ==-1){
		G4cout<<"Collection name wrong/sensitive detector is disabled...\n"<<G4endl;
		return;
	}
//==================================================================================
	
	G4ThreeVector positron_cap_pos(NAN,NAN,NAN);
	G4double E_pos_primary(0);
	G4double E_pos_secondary(0);
	G4double E_em_primary = 0;
	G4double E_em_secondary = 0;
	
	G4ThreeVector neutron_cap_pos(NAN,NAN,NAN);
	G4double n_cap_time = NAN;

	G4ThreeVector elastic_dir = p_neutron_i.unit();		// angle n comes out from target
	G4double elastic_time = (-10*s), first_contact(10000*s);

//	G4int positron_cellID_i(-1), positron_cellID_f(-1);
//	G4int neutron_cellID_i(-1), neutron_cellID_f(-1);

	G4int NofHits = 0;
	TESHit* hit;

	if(evt->GetHCofThisEvent()){
		TESHitsCollection* TES_HC = NULL;
		TES_HC = (TESHitsCollection*)evt->GetHCofThisEvent()->GetHC(collectionID);
		if(TES_HC==NULL) return;

		if(TES_HC) NofHits = TES_HC->GetSize();
		if(NofHits>0){
			G4double p_time_i(0*s), p_time_f(time_delay), n_time(-1*s);
			for( int i=0;i<NofHits;i++){
				hit = (*TES_HC)[i];
				if(hit->GetParticleName()=="e+"){
					if(hit->GetGlobalTime()<p_time_i){
						p_time_i = hit->GetGlobalTime();
//						positron_cellID_i = hit->GetCellID();
					}
					if(hit->GetGlobalTime()>p_time_f){
						p_time_f = hit->GetGlobalTime();
//						positron_cellID_f = hit->GetCellID();
						G4int status = hit->GetTrackStatus();
						if(status==1 || status==2){
							positron_cap_pos = hit->GetPosition();
						}
					}
				}
				else if(hit->GetParticleName()=="neutron"){
					if(hit->GetGlobalTime()>n_time){
						n_time = hit->GetGlobalTime();
//						neutron_cellID_f = hit->GetCellID();
						G4int status = hit->GetTrackStatus();
						if(status==1 || status==2){
							neutron_cap_pos = hit->GetPosition();
							n_cap_time = n_time;
						}
					}
					if(hit->GetGlobalTime()<first_contact && abs(hit->GetPosition().z())>dptr->GetThickness())
						first_contact = hit->GetGlobalTime();
				}
//				neutron_cellID_i = positron_cellID_i;
			}

//=============================================neutron leaving target=========================================
			for( int i=0; i<NofHits; i++){
				hit = (*TES_HC)[i];

				if(hit->GetParticleName()=="neutron")
				if(hit->GetGlobalTime()>elastic_time)
				if(abs(hit->GetPosition().z())<dptr->GetThickness())
				if(hit->GetGlobalTime()<first_contact){
					elastic_dir = hit->GetMomentum();
					elastic_time = hit->GetGlobalTime();
				}
//==============================================EM process===================================================
				if(hit->GetGlobalTime()<0){

					if(hit->GetParticleName()=="e-"){
//||hit->GetParticleName()=="gamma"){
//	exclude gamma energy deposition.
//if(hit->GetCellID()==positron_cellID_i)
						if(abs(hit->GetPosition().z())<dptr->GetThickness()/2)
							E_em_primary += hit->GetEDep();
						else
							E_em_secondary +=hit->GetEDep();
					}
					else if(hit->GetParticleName()=="e+")
						abs(hit->GetPosition().z())<dptr->GetThickness()/2?
							E_pos_primary += hit->GetEDep():E_pos_secondary+=hit->GetEDep();
					else {};
				}
			}
		}
	}
	G4double elastic_angle = acos((elastic_dir.unit().dot(p_neutron_i.unit())));

//	histo->FillNtuple(evtID, positron_cellID_i, r_positron_i, p_positron_i, E_positron_i, neutron_cellID_i, r_neutron_i, p_neutron_i, E_neutron_i, positron_cellID_f, positron_cap_pos, E_pos_primary, E_pos_secondary, E_em_primary, E_em_secondary, neutron_cellID_f, neutron_cap_pos, n_cap_time, elastic_dir.getTheta());

	histo->FillNeutrino(evtID, fPrimGen->GetNeutrinoDirection(), fPrimGen->GetNeutrinoEnergy());
	histo->FillPositron(r_positron_i, p_positron_i, E_positron_i, positron_cap_pos, E_pos_primary, E_pos_secondary, E_em_primary, E_em_secondary);
	histo->FillNeutron(r_neutron_i, p_neutron_i, E_neutron_i, neutron_cap_pos, n_cap_time, elastic_angle);

//==============================================Draw hits====================================================
#ifdef G4VIS_USE
	TESHitsCollection* TES_HC = NULL;
	TES_HC = (TESHitsCollection*)evt->GetHCofThisEvent()->GetHC(collectionID);
	if(TES_HC==NULL) return;
	if(TES_HC) NofHits = TES_HC->GetSize();
	if(NofHits>0){
		G4int N = (dptr->GetNx())*(dptr->GetNy())*(dptr->GetNz());
		G4double* cell_p = new G4double[N];
		G4double* cell_n = new G4double[N];
		for(int i=0;i<N;i++)
			{cell_p[i] = 0;	cell_n[i] = 0;}
		for( int i=0; i<NofHits; i++){
			hit = (*TES_HC)[i];
			if(hit->GetParticleName()=="e+") cell_p[hit->GetCellID()] += hit->GetEDep();
			else if(hit->GetParticleName()=="neutron") cell_n[hit->GetCellID()] += hit->GetEDep();
		}
		G4VVisManager* pVVisManager = G4VVisManager::GetConcreteInstance();
		if(pVVisManager){
			G4double e_scale = E_positron_i;
			G4double n_scale = E_neutron_i;
			G4double dx(0.8*dptr->GetLengthX()), dy(0.8*dptr->GetLengthY());
			G4double dz(dptr->GetLengthZ());
			for(int i=0;i<N;i++){
				G4double e_depth(dptr->GetLengthZ()*cell_p[i]/e_scale),n_depth(dptr->GetLengthZ()*cell_n[i]/n_scale);
				G4ThreeVector center_tmp = TargetParam::GetPointer()->GetCenter(i);
				dz = abs(center_tmp.z())<dz?dptr->GetThickness():dz;

				if(e_depth/e_scale >0.1){
					G4ThreeVector e_center = center_tmp.z()>=0?
					center_tmp+G4ThreeVector(0,0,(e_depth+dz)/2):center_tmp+G4ThreeVector(0,0,(e_depth+dz)/2);
					G4Transform3D trans1(G4RotationMatrix(),e_center);
					G4VisAttributes attribs;
					attribs.SetColour(G4Colour(1.,0.,1.,0.9));
					attribs.SetForceSolid(true);
					G4Box box1("box",dx/2,dy/2,e_depth/2);
					pVVisManager->Draw( box1, attribs, trans1);
//					plotted[i] = 1;G4cout<<"E+ Plotting\t"<<i<<G4endl;
				}
				if(n_depth/n_scale > 0.01){
					G4ThreeVector n_center = center_tmp.z()>=0?
					center_tmp+G4ThreeVector(0,0,(n_depth+dz)/2):center_tmp-G4ThreeVector(0,0,(n_depth+dz)/2);
					G4Transform3D trans2(G4RotationMatrix(),n_center);
					G4VisAttributes attribs2;
					attribs2.SetColour(G4Colour(1.,1.,0.,0.9));
					attribs2.SetForceSolid(true);
					G4Box box2("box",dx/2,dy/2,n_depth/2);
					pVVisManager->Draw( box2, attribs2, trans2);
//					plotted[i] = 1;G4cout<<"Neutron Plotting\t"<<i<<G4endl;
				}
			}
		}
	}
#endif
}
