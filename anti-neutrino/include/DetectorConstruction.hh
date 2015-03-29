#ifndef DetectorConstruction_h
#define DetectorConstruction_h 1

#include "G4VUserDetectorConstruction.hh"

#include "G4UserLimits.hh"
#include "G4Material.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4Polycone.hh"
#include "G4Tubs.hh"
#include "G4Cons.hh"
#include "TargetParam.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4GeometryManager.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4SolidStore.hh"

#include "G4SimpleHeum.hh"
#include "G4TransportationManager.hh"
#include "G4FieldManager.hh"
#include "G4ChordFinder.hh"

#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"
#include "globals.hh"

#include "EMfield.hh"
#include "G4EqMagElectricField.hh"
#include "G4TransportationManager.hh"
#include "G4ChordFinder.hh"
#include "G4ClassicalRK4.hh"
#include "G4MagIntegratorStepper.hh"
#include "G4MagIntegratorDriver.hh"

#include "TransitionEdgeSensor.hh"
#include "G4VSensitiveDetector.hh"
#include "G4SDManager.hh"
#include "DetectorMessenger.hh"

class DetectorMessenger;

class DetectorConstruction : public G4VUserDetectorConstruction{

public:
	DetectorConstruction();
	~DetectorConstruction();

public:
	virtual	G4VPhysicalVolume* Construct();
	void	InitializeParameterList();

	void	SetLengthX(G4double l){dX = l;}
	G4double GetLengthX(){return dX;}
	void	SetLengthY(G4double l){dY = l;}
	G4double GetLengthY(){return dY;}
	void	SetLengthZ(G4double l){dZ = l;}
	G4double GetLengthZ(){return dZ;}
	void	SetThickness(G4double l){target_thickness = l;}
	G4double GetThickness(){return target_thickness;}
	void	SetDimension(G4ThreeVector l){dX = l.x(); dY = l.y(); dZ = l.z();}
	G4ThreeVector GetDimension(){return G4ThreeVector(dX,dY,dZ);}

	void	SetNx(G4int a){Nx = a;}
	G4int	GetNx(){return Nx;}
	void	SetNy(G4int a){Ny = a;}
	G4int	GetNy(){return Ny;}
	void	SetNz(G4int a){Nz = a;}
	G4int	GetNz(){return Nz;}

	void	SetGap(G4double l){Gap = l;}
	G4double GetGap(){return Gap;}

	G4ThreeVector	GetCenter(G4int nx, G4int ny, G4int nz){
				G4int ix = (nx>Nx)?Nx-1:nx;
				G4int iy = (ny>Ny)?Ny-1:ny;
				G4double x = ix*dX + dX/2 - (Nx*dX)/2;
				G4double y = iy*dY + dY/2 - Ny*dY/2;

				G4int iz = (nz>Nz)?Nz-1:nz;
				G4double wz = (target_thickness+(Nz-1)*(dZ+Gap))/2;
				G4double z;
				if(iz<G4int(Nz/2)) z = iz*(Gap+dZ)+dZ/2 - wz;
				else if(iz==G4int(Nz/2)) z = iz*(Gap+dZ)+target_thickness/2 - wz;
				else z = iz*(Gap+dZ)+dZ/2  - wz -dZ + target_thickness;
				return G4ThreeVector(x,y,z);
			}

	static	DetectorConstruction* GetPointer() {return fInstance;}
	void	UpdateGeometry();
	const	G4VPhysicalVolume* GetWorld(){return world_physical;};           
	void	PrintParameters();

private:
	G4double dX, dY, dZ, target_thickness, Gap;
	G4double Nx, Ny, Nz;
private:
	static DetectorConstruction* fInstance;
	DetectorMessenger*	fmessenger;
	TransitionEdgeSensor* TES_SD;
	void	DefineMaterials();

//----------------World---------------------------------------------------
	G4LogicalVolume*	world_logical;
	G4VPhysicalVolume*	world_physical;

//---------------Material
public:
	G4Material* GetDetectorMaterial();
	G4Material* GetTargetMaterial();
private:
	G4Element* Carbon;
	G4Element* Hydrogen;
	G4Element* Boron;
	G4Element* Gadolinium;
	G4Material* Vacuum;
	G4Material* Plastic;
	G4Material* BPlastic;
	G4Material* HeliumGas;
	G4Material* HeliumLiquid;
};
#endif
