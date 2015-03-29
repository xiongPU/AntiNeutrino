#ifndef TESHIT_hh
#define TESHIT_hh 1

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"

#include "G4VVisManager.hh"
#include "G4Circle.hh"
#include "G4Color.hh"
#include "G4VisAttributes.hh"

class TESHit: public G4VHit{

public:
	TESHit();
	~TESHit();
	TESHit( const TESHit& right);
	const TESHit& operator =( const TESHit& right);
	int operator==( const TESHit& right) const;

	inline void * operator new( size_t);
	inline void operator delete( void* aHit);

	void Draw();
	void Print();
	
private:
	G4double Edep;
	G4double energy;
	G4double time;
	G4ThreeVector position;
	G4ThreeVector delta_pos;
	G4ThreeVector momentum;
	G4ThreeVector delta_momentum;
	G4String name;

public:
	void SetParticleName( G4String n){name = n;}
	G4String GetParticleName(){return name;}

	void SetEDep( G4double E){Edep = E;}
	G4double GetEDep(){return Edep;}

	void SetEnergy( G4double E){energy = E;}
	G4double GetEnergy(){return energy;}

	void SetGlobalTime( G4double t){time = t;}
	G4double GetGlobalTime(){return time;}

	void SetPosition( G4ThreeVector a){position = a;}
	G4ThreeVector GetPosition(){return position;}

	void SetDeltaPos( G4ThreeVector a){delta_pos = a;}
	G4ThreeVector GetDeltaPos(){return delta_pos;}

	void SetMomentum( G4ThreeVector a){momentum = a;}
	G4ThreeVector GetMomentum(){return momentum;}

	void SetDeltaMomentum( G4ThreeVector a){delta_momentum = a;}
	G4ThreeVector GetDeltaMomentum(){return delta_momentum;}
};

typedef G4THitsCollection<TESHit> TESHitsCollection;

extern G4Allocator<TESHit> TESHitAllocator;

inline void* TESHit::operator new(size_t){
	void *ahit;
	ahit = (void*) TESHitAllocator.MallocSingle();
	return ahit;
}

inline void TESHit::operator delete( void *aHit){
	TESHitAllocator.FreeSingle( (TESHit*) aHit);
}

#endif
