#include "TESHit.hh"
#include "PrimaryGeneratorAction.hh"
#include "DetectorConstruction.hh"
#include "TargetParam.hh"

G4Allocator<TESHit> TESHitAllocator;

TESHit::TESHit(){}

TESHit::~TESHit(){}

TESHit::TESHit( const TESHit& right):G4VHit(){
	Edep = right.Edep;
	energy = right.energy;
	time = right.time;
	position = right.position;
	momentum = right.momentum;
	name = right.name;
	cellID = right.cellID;
	status = right.status;
}

const TESHit& TESHit::operator =( const TESHit& right){
	Edep = right.Edep;
	energy = right.energy;
	time = right.time;
	position = right.position;
	momentum = right.momentum;
	name = right.name;
	cellID = right.cellID;
	status = right.status;
	return *this;
}

int TESHit::operator==( const TESHit& right) const{
	return (this==&right) ? 1 : 0;
}

void TESHit::Draw(){
/*
	G4VVisManager* pVVisManager = G4VVisManager::GetConcreteInstance();

	if(pVVisManager){
//		G4Circle circle(position);
//		circle.SetScreenSize(0.04);
//		circle.SetFillStyle(G4Circle::filled);
//		G4Colour colour(1.,0.,0.);
//		G4VisAttributes attribs(colour);
//		circle.SetVisAttributes(attribs);
//		pVVisManager->Draw(circle);

		if(Edep > 0. && (name == "neutron" || name == "e+") ){
			PrimaryGeneratorAction* ptr = PrimaryGeneratorAction::GetPointer();
			G4double scale = (name=="e+"?ptr->GetPositronEnergy():ptr->GetNeutronEnergy());
			DetectorConstruction* dptr = DetectorConstruction::GetPointer();
			G4double dx = dptr->GetLengthX();
			G4double dy = dptr->GetLengthY();
			G4double dz = dptr->GetLengthZ();
//		consider the case hit is in the target layer, which thickness is not dz.
			dz = abs(position.z())<dz?dptr->GetThickness():dz;

//		the for loop is used for plotting, since hit is plotted as a box.
//			for(double i=0.01;i<1;i+=0.01){	
			G4double depth = dptr->GetLengthZ()*Edep/scale;
			G4ThreeVector center_tmp = TargetParam::GetPointer()->GetCenter(cellID);
			G4ThreeVector center = center_tmp.z()>=0?center_tmp+G4ThreeVector(0,0,(depth+dz)/2):center_tmp-G4ThreeVector(0,0,(depth+dz)/2);
			G4Transform3D trans(G4RotationMatrix(),center);

			G4VisAttributes attribs2;
			attribs2.SetColour(name=="e+"?G4Colour(1.,0.,1.,0.8):G4Colour(1.,1.,0.,0.6));
			attribs2.SetForceSolid(true);

			G4Box box("box",dx/2,dy/2,depth/2);
			pVVisManager->Draw(box,attribs2, trans);
//			}
		}
	}*/
}

void TESHit::Print(){}
