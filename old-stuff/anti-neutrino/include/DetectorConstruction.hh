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

class DetectorConstruction : public G4VUserDetectorConstruction{

public:
	DetectorConstruction();
	~DetectorConstruction();

public:
	virtual	G4VPhysicalVolume* Construct();
	void	ConstructWorld();
	void	ConstructTarget();
	void	ConstructGadolinium();
	void	ConstructNeutronAbsorber();
	void	ConstructDetector();

	void	SetTargetPosition(G4ThreeVector t){target_position = t;}
	G4ThreeVector	GetTargetPosition(){return target_position;}
	void	SetTargetRadius(G4double a){target_radius = a;}
	G4double	GetTargetRadius(){return target_radius;}
	void	SetTargetThickness(G4double t){target_thickness = t;}
	G4double	GetTargetThickness(){return target_thickness;}
	G4double	GetTargetLength(){return 2*target_hf_length;}
/*
	void	SetCalPosition(G4ThreeVector t){cal_position = t;}
	G4ThreeVector	GetCalPosition(){return cal_position;}
	void	SetCalRadius(G4double a){cal_radius = a;}
	G4double	GetCalRadius(){return cal_radius;}
	void	SetCalThickness(G4double t){cal_hf_length = t/2;}
	G4double	GetCalThickness(){return 2*cal_hf_length;}
*/
	void	UpdateGeometry();
  
	const	G4VPhysicalVolume* GetWorld(){return world_physical;};           

	void	PrintParameters();

//	void SetFieldMapName(G4String s){field_map_name = s;}

	static DetectorConstruction* GetPointer() {return fInstance;}

private:
/*
	EMfield*	field;
	G4String	field_map_name;
*/
	void	DefineMaterials();

	static DetectorConstruction* fInstance;

//----------------World---------------------------------------------------

	G4Material*	world_material;
	G4Tubs*	world_solid;
	G4LogicalVolume*	world_logical;
	G4VPhysicalVolume*	world_physical;

	G4double world_radius;
	G4double world_hf_length;

//----------------Target-----------------------------------------

	G4Material*	target_material;
	G4Tubs*		target_solid;
	G4LogicalVolume*	target_logical;
	G4VPhysicalVolume*	target_physical;

	G4ThreeVector	target_position;
	G4double	target_radius;
	G4double	target_thickness;
	G4double	target_hf_length;

//----------------Detector----------------------------------------

	G4Material*	detector_material;
	G4Tubs*		detector_solid;
	G4LogicalVolume*	detector_logical;
	G4VPhysicalVolume*	detector_physical;

	G4ThreeVector	detector_position;
	G4double	detector_radius;
	G4double	detector_thickness;
	G4double	detector_hf_length;

//----------------Gadolinium--------------------------------------

	G4Material*	gadolinium_material;
	G4Tubs*		gadolinium1_solid;
	G4LogicalVolume*	gadolinium1_logical;
	G4VPhysicalVolume*	gadolinium1_physical;

	G4double	gadolinium_thickness;

	G4double	gadolinium1_radius;
	G4double	gadolinium1_hf_length;
	G4ThreeVector	gadolinium1_position;

	G4Tubs*		gadolinium2_solid;
	G4LogicalVolume*	gadolinium2_logical;
	G4VPhysicalVolume*	gadolinium2_physical;

	G4double	gadolinium2_radius;
	G4double	gadolinium2_hf_length;
	G4ThreeVector	gadolinium2_position;

//----------------Neutron Absorber--------------------------------

	G4Material*	neutron_abs_material;
	G4Tubs*		neutron_abs_solid;
	G4LogicalVolume*	neutron_abs_logical;
	G4VPhysicalVolume*	neutron_abs_physical;

	G4double	neutron_abs_radius;
	G4double	neutron_abs_hf_length;
	G4double	neutron_abs_thickness;
	G4ThreeVector	neutron_abs_position;
};

#endif
