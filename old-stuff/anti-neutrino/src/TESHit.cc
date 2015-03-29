#include "TESHit.hh"

G4Allocator<TESHit> TESHitAllocator;

TESHit::TESHit(){}

TESHit::~TESHit(){}

TESHit::TESHit( const TESHit& right):G4VHit(){
	Edep = right.Edep;
	time = right.time;
	position = right.position;
	delta_pos = right.delta_pos;
	momentum = right.momentum;
	delta_momentum = right.delta_momentum;
	name = right.name;
}

const TESHit& TESHit::operator =( const TESHit& right){
	Edep = right.Edep;
	time = right.time;
	position = right.position;
	delta_pos = right.delta_pos;
	momentum = right.momentum;
	delta_momentum = right.delta_momentum;
	name = right.name;
	return *this;
}

int TESHit::operator==( const TESHit& right) const{
	return (this==&right) ? 1 : 0;
}

void TESHit::Draw(){

	G4VVisManager* pVVisManager = G4VVisManager::GetConcreteInstance();

	if(pVVisManager){
		G4Circle circle(position);
		circle.SetScreenSize(0.04);
		circle.SetFillStyle(G4Circle::filled);
		G4Colour colour(1.,0.,0.);
		G4VisAttributes attribs(colour);
		circle.SetVisAttributes(attribs);
		pVVisManager->Draw(circle);
	}
}

void TESHit::Print(){}
