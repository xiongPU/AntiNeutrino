#ifndef TrackerParam_HH
#define TrackerParam_HH

#include "G4PVParameterised.hh"
#include "G4VPVParameterisation.hh"
#include "globals.hh"
#include "G4Box.hh"
#include "G4Types.hh"
#include "DetectorConstruction.hh"

class G4Tubs;
class G4Trd;
class GG4Trap;
class G4Sphere;
class G4Orb;
class G4Ellipsoid;
class G4Torus;
class G4Para;
class G4Polycone;
class G4Polyhedra;
class G4Hype;

class TargetParam: public G4VPVParameterisation{
public:
	TargetParam(G4int a, G4int b, G4int c, G4double x, G4double y, G4double z, G4double thickness, G4double spg);
	~TargetParam();

	void ComputeTransformation(G4int copyN, G4VPhysicalVolume* physVol) const;
	void ComputeDimensions(G4Box& target, const G4int copyN, const G4VPhysicalVolume* physVol)const;
	G4ThreeVector GetCenter(G4int copyN);
	G4Material* ComputeMaterial(G4int const, G4VPhysicalVolume*, const G4VTouchable*);
	static TargetParam* GetPointer();
private:
	G4int Nx, Ny, Nz;
	G4double dX, dY, dZ, target_thickness, Gap;
	static TargetParam* ftarget_param_ptr;
public:
	void ComputeDimensions(G4Tubs&,const G4int, const G4VPhysicalVolume*)const{};
	void ComputeDimensions(G4Trd&,const G4int, const G4VPhysicalVolume*)const{};
	void ComputeDimensions(G4Trap&,const G4int, const G4VPhysicalVolume*)const{};
	void ComputeDimensions(G4Cons&,const G4int, const G4VPhysicalVolume*)const{};
	void ComputeDimensions(G4Sphere&,const G4int, const G4VPhysicalVolume*)const{};
	void ComputeDimensions(G4Orb&,const G4int, const G4VPhysicalVolume*)const{};
	void ComputeDimensions(G4Ellipsoid&,const G4int, const G4VPhysicalVolume*)const{};
	void ComputeDimensions(G4Torus&,const G4int, const G4VPhysicalVolume*)const{};
	void ComputeDimensions(G4Para&,const G4int, const G4VPhysicalVolume*)const{};
	void ComputeDimensions(G4Polycone&,const G4int, const G4VPhysicalVolume*)const{};
	void ComputeDimensions(G4Polyhedra&,const G4int, const G4VPhysicalVolume*)const{};
	void ComputeDimensions(G4Hype&,const G4int, const G4VPhysicalVolume*)const{};
};
#endif
