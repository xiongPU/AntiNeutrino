#ifndef TESHIT_hh
#define TESHIT_hh 1

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"
#include "G4Track.hh"

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
private:
	G4String name;
	G4ThreeVector position;
	G4ThreeVector momentum;	
	G4double energy;
	G4double Edep;
	G4double time;
	G4int cellID;
	G4TrackStatus status;
public:
	void Draw();
	void Print();	
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

	void SetMomentum( G4ThreeVector a){momentum = a;}
	G4ThreeVector GetMomentum(){return momentum;}

	void SetCellID(G4int a){cellID = a;}
	G4int GetCellID(){return cellID;}

	void SetTrackStatus(G4TrackStatus st){ status = st;}
	G4TrackStatus GetTrackStatus(){return status;}
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
