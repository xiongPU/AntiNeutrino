#include "DetectorConstruction.hh"
#define MAG 1          // Magnetic field grid

DetectorConstruction* DetectorConstruction::fInstance = 0;

DetectorConstruction::DetectorConstruction(){
	DefineMaterials();

	world_radius = 2*m;
	world_hf_length = 2*m;

	target_radius = 50*cm;
	target_thickness = 20*mm;
	target_hf_length = world_hf_length*0.8;;
	target_position = G4ThreeVector(0*cm,0*cm,0*cm);

	G4double D = 50*cm;

	gadolinium_thickness = 25*um;
	gadolinium1_radius = target_radius+target_thickness+D;
	gadolinium1_hf_length = target_hf_length;
	gadolinium1_position = G4ThreeVector(0,0,0);

	neutron_abs_radius = gadolinium1_radius+gadolinium_thickness;
	neutron_abs_thickness = 20*cm;
	neutron_abs_hf_length = target_hf_length;
	neutron_abs_position = G4ThreeVector(0,0,0);

	gadolinium2_radius = neutron_abs_radius+neutron_abs_thickness;
	gadolinium2_hf_length = gadolinium1_hf_length;
	gadolinium2_position = G4ThreeVector(0,0,0);

	detector_radius = gadolinium2_radius*1.5;
	detector_hf_length = target_hf_length;
	detector_thickness = 1*cm;
	detector_position = G4ThreeVector(0,0,0);

	fInstance = this;
}

G4VPhysicalVolume* DetectorConstruction::Construct(){
	// Cleanup old geometry
	G4GeometryManager::GetInstance()->OpenGeometry();
	G4PhysicalVolumeStore::GetInstance()->Clean();
	G4LogicalVolumeStore::GetInstance()->Clean();
	G4SolidStore::GetInstance()->Clean();

	ConstructWorld();	
	ConstructTarget();
	ConstructGadolinium();
	ConstructNeutronAbsorber();
	ConstructDetector();

	return world_physical;
}

//-------------------------world---------------------------------------------------------------------------------

void DetectorConstruction::ConstructWorld(){

	world_solid  = new G4Tubs("world_solid",0*m,world_radius,world_hf_length,0,2*pi);
	world_logical  = new G4LogicalVolume(world_solid, world_material, "world_logical");
	world_logical->SetUserLimits(new G4UserLimits(DBL_MAX,DBL_MAX,DBL_MAX,0.1*eV));
	world_physical = new G4PVPlacement(0,G4ThreeVector(0,0,0),"world_physical",world_logical,NULL,false,0);
}

//-------------------------Target---------------------------------------------------------------------------------

void DetectorConstruction::ConstructTarget(){

	target_solid = new G4Tubs( "target_solid", target_radius, target_radius+target_thickness, target_hf_length,0,2*pi);
	target_logical = new G4LogicalVolume( target_solid, target_material, "target_logical");
	target_physical = new G4PVPlacement( 0, target_position, target_logical, "target_physical", world_logical, false, 0);
}

//-------------------------Gadolinium-----------------------------------------------------------------------------

void DetectorConstruction::ConstructGadolinium(){

	gadolinium1_solid = new G4Tubs( "gadolinium1_solid", gadolinium1_radius, gadolinium1_radius+gadolinium_thickness, gadolinium1_hf_length,0,2*pi);
	gadolinium1_logical = new G4LogicalVolume( gadolinium1_solid, gadolinium_material, "gadolinium1_logical");
	gadolinium1_physical = new G4PVPlacement( 0, gadolinium1_position, gadolinium1_logical, "gadolinium1_physical", world_logical, false, 0);

	gadolinium2_solid = new G4Tubs( "gadolinium2_solid", gadolinium2_radius, gadolinium2_radius+gadolinium_thickness, gadolinium2_hf_length,0,2*pi);
	gadolinium2_logical = new G4LogicalVolume( gadolinium2_solid, gadolinium_material, "gadolinium2_logical");
	gadolinium2_physical = new G4PVPlacement( 0, gadolinium2_position, gadolinium2_logical, "gadolinium2_physical", world_logical, false, 0);
}

//-------------------------neutron absorber----------------------------------------------------------------------

void DetectorConstruction::ConstructNeutronAbsorber(){

	neutron_abs_solid = new G4Tubs( "neutron_abs_solid",neutron_abs_radius, neutron_abs_radius+neutron_abs_thickness, neutron_abs_hf_length,0,2*pi);
	neutron_abs_logical = new G4LogicalVolume( neutron_abs_solid, neutron_abs_material, "neutron_abs_logical");
	neutron_abs_physical = new G4PVPlacement( 0, neutron_abs_position, neutron_abs_logical, "neutron_abs", world_logical, false, 0);
}

//-------------------------Detector---------------------------------------------------------------------------

void DetectorConstruction::ConstructDetector(){

	detector_solid = new G4Tubs( "detector_solid", detector_radius, detector_radius + detector_thickness, detector_hf_length,0,2*pi);
	detector_logical = new G4LogicalVolume( detector_solid, detector_material, "detector_logical");
	detector_physical = new G4PVPlacement( 0, detector_position, detector_logical, "detector_physical", world_logical, false, 0);

	G4String sd_name = "Gamma-Detector";
	TransitionEdgeSensor* sd = new TransitionEdgeSensor( "gamma_detector");
	G4SDManager* sdman = G4SDManager::GetSDMpointer();
	sdman->AddNewDetector(sd);
	detector_logical->SetSensitiveDetector(sd);
}

DetectorConstruction::~DetectorConstruction(){
//	if (field) delete field;
	fInstance = 0;
}

//-------------------------Material table------------------------------------------------------------------------

void DetectorConstruction::DefineMaterials(){

	G4double z, a, density;
	world_material = new G4Material("Vacuum", z=1., a = 1.00794*g/mole, density= 1.0e-16*g/cm3);
	detector_material = new G4Material("Steel", z=26., a = 55.845*g/mole, density = 7.874*g/cm3);
	gadolinium_material = new G4Material("Gadolinium", z=64., a = 157.25*g/mole, density= 7.9*g/cm3);
	target_material = new G4Material("Graphene", z=6., a = 12.01*g/mole, density= 2.267*g/cm3);
	neutron_abs_material = new G4Material("Plastic", z=6., a = 12.01*g/mole, density = 1.267*g/cm3);
//	target_material = gadolinium_material;
//	neutron_abs_material = gadolinium_material;
	G4cout << *(G4Material::GetMaterialTable()) << G4endl;
}

/*
void DetectorConstruction::ConstructField(){

	field = new EMfield(field_map_name);
	G4FieldManager* fmgr = G4TransportationManager::GetTransportationManager()->GetFieldManager();	
	fmgr->SetDetectorField( field);

	G4int nvar = 6;		//integrate over position, momentum, energy and time
	G4EqMagElectricField* equation = new G4EqMagElectricField( field);
	G4MagIntegratorStepper* stepper = new G4ClassicalRK4( equation, nvar);

	G4double minstep = 1*um;
	G4MagInt_Driver* driver = new G4MagInt_Driver( minstep, stepper, stepper->GetNumberOfVariables());

	G4ChordFinder* chordfinder = new G4ChordFinder( driver);
	fmgr->SetChordFinder( chordfinder);
}
*/
#include "G4RunManager.hh"

void DetectorConstruction::UpdateGeometry()
{
  G4RunManager::GetRunManager()->GeometryHasBeenModified();
}
