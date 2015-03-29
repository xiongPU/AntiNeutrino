#include "DetectorConstruction.hh"
#include "TargetParam.hh"

class TargetParam;

DetectorConstruction* DetectorConstruction::fInstance = 0;

DetectorConstruction::DetectorConstruction():fmessenger(0),TES_SD(0),world_logical(0),world_physical(0){
	DefineMaterials();
	Nx = 1;	Ny = 1; Nz = 5;
	dX = 5*4*m/Nx;	dY = 5*4*m/Ny;	dZ = 6*cm;

	target_thickness = 1*cm;
	Gap = 100*cm;

	fmessenger = new DetectorMessenger(this);
	fInstance = this;
}

G4VPhysicalVolume* DetectorConstruction::Construct(){
	// Cleanup old geometry
	G4GeometryManager::GetInstance()->OpenGeometry();
	G4PhysicalVolumeStore::GetInstance()->Clean();
	G4LogicalVolumeStore::GetInstance()->Clean();
	G4SolidStore::GetInstance()->Clean();

//---------- Define world---------------------------------------------------
	G4double world_z = target_thickness + (Nz-1)*(dZ+Gap);
	G4double world_x = Nx*dX;
	G4double world_y = Ny*dY;

	G4Box* world_solid = new G4Box("world",world_x/2, world_y/2, world_z/2);
	world_logical = new G4LogicalVolume(world_solid,Vacuum,"world");
	world_physical = new G4PVPlacement(0,G4ThreeVector(0,0,0),"world",world_logical,NULL,false,0);
	world_logical->SetVisAttributes(G4VisAttributes::Invisible);

//---------- Define target--------------------------------------------------
	G4Box* target_solid = new G4Box("target",dX/2,dY/2,dZ/2);
	G4LogicalVolume* target_logical = new G4LogicalVolume(target_solid,Plastic,"target");
	TargetParam* target_para = new TargetParam(Nx,Ny,Nz,dX,dY,dZ,target_thickness,Gap);
	G4VPhysicalVolume* target_physical = new G4PVParameterised("target",target_logical, world_logical, kUndefined, Nx*Ny*Nz, target_para);

	G4VisAttributes* target_att = new G4VisAttributes(G4Colour(0.4,0.4,0.6,0.6));
	target_att -> SetForceSolid(true);
	target_logical ->SetVisAttributes(target_att);

//---------- Define sensitive detector--------------------------------------
	if(TES_SD==0){
		G4cout<<"Creating neutron target sensitive detector..."<<G4endl;
		G4String sd_name = "Neutron-Target";
		TES_SD = new TransitionEdgeSensor(sd_name);
		G4SDManager* sdman = G4SDManager::GetSDMpointer();
		sdman->AddNewDetector(TES_SD);
	}
	target_logical->SetSensitiveDetector(TES_SD);
	return world_physical;
}

//-------------------------Material table-----------------------------------

void DetectorConstruction::DefineMaterials(){

	G4double z,a, aC, aH, aB, aGd, density; G4int ncomp;

	Vacuum = new G4Material("Vacuum", z=1., a = 1.00794*g/mole, density= 1.0e-16*g/cm3);
	Carbon = new G4Element("Carbon", "C", z=6, (aC=12.011)*g/mole);
	Hydrogen = new G4Element("Hydrogen", "H", z=1, (aH=1.01)*g/mole);
	Boron = new G4Element("Boron","B", z=5, (aB=10.013)*g/mole);
	Gadolinium = new G4Element("Gadolinium", "Gd", z=3, (aGd=3.0160293)*g/mole);

	G4double mC(4.44*aC), mH(5.18*aH), mB(0.0568*aB), mGd(0.0568*aGd);
	G4double plastic_density = (mC+mH)/(6.022*10)*g/cm3;
	G4double plastic_density_boron = (mC+mH+mB)/(60.22)*g/cm3;

	Plastic = new G4Material("Plastic", plastic_density, ncomp=2);
	Plastic->AddElement(Carbon, mC/(mC+mH));
	Plastic->AddElement(Hydrogen, mH/(mC+mH));

	BPlastic =  new G4Material("Plastic", plastic_density_boron, ncomp=3);
	BPlastic->AddElement(Carbon, mC/(mC+mH+mB));
	BPlastic->AddElement(Hydrogen, mH/(mC+mH+mB));
	BPlastic->AddElement(Boron, mB/(mC+mH+mB));

	HeliumGas = new G4Material("GasHelium", z=2, a = 3.0160293*g/mole, density= 1.48e-4*g/cm3);
	HeliumLiquid = new G4Material("LiquidHelium", z=2, a = 3.0160293*g/mole, density = 0.1412*g/cm3);

	G4cout << *(G4Material::GetMaterialTable()) << G4endl;
}

G4Material* DetectorConstruction::GetTargetMaterial(){return Plastic;}
G4Material* DetectorConstruction::GetDetectorMaterial(){return BPlastic;}

DetectorConstruction::~DetectorConstruction(){
	delete fmessenger;	fmessenger = 0;
	fInstance = 0;
}
#include "G4RunManager.hh"

void DetectorConstruction::UpdateGeometry(){
	G4RunManager::GetRunManager()->DefineWorldVolume(Construct());
	G4cout<<"Geometry updated..."<<G4endl;
}
