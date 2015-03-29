#include "TargetParam.hh"

class DetectorConstruction;

TargetParam::TargetParam(G4int a, G4int b, G4int c, G4double x, G4double y, G4double z, G4double thickness, G4double spg):Nx(a),Ny(b),Nz(c),dX(x),dY(y),dZ(z),target_thickness(thickness),Gap(spg){
	ftarget_param_ptr = this;
}

TargetParam::~TargetParam(){ ftarget_param_ptr = 0; }

void TargetParam::ComputeTransformation(G4int copyN, G4VPhysicalVolume* physVol) const{
	if(copyN>Nx*Ny*Nz) return;
//----- Set Z translation.

	G4int iz = copyN/(Nx*Ny);
	G4int ix = (copyN%(Nx*Ny)) %Nx;
	G4int iy = (copyN%(Nx*Ny)) /Nx;

//----- Start from lower corner to find the center coordinate of a particular cell.
	G4double x = ix*dX + dX/2 - (Nx*dX)/2;
	G4double y = iy*dY + dY/2 - Ny*dY/2;
	G4double z = 0;
	G4double wz = (target_thickness+(Nz-1)*(dZ+Gap))/2;

	if(iz<Nz/2) z = iz*(Gap+dZ)+dZ/2 - wz;
	else if(iz==Nz/2) z = iz*(Gap+dZ)+target_thickness/2 - wz;
	else z = iz*(Gap+dZ)+dZ/2  - wz -dZ + target_thickness;

	G4ThreeVector r(x,y,z);
	physVol->SetTranslation(r);
	physVol->SetRotation(0);
}

void TargetParam::ComputeDimensions(G4Box& target, const G4int copyN, const G4VPhysicalVolume* physVol)const{
	target.SetXHalfLength(dX/2);
	target.SetYHalfLength(dY/2);
	if(copyN/(Nx*Ny)==Nz/2)
		target.SetZHalfLength(target_thickness/2);
	else	target.SetZHalfLength(dZ/2);
}

G4Material* TargetParam::ComputeMaterial(const G4int copyNo, G4VPhysicalVolume* physVol, const G4VTouchable* touch){
	if(copyNo/(Nx*Ny)==Nz/2) return DetectorConstruction::GetPointer()->GetTargetMaterial();
	else return DetectorConstruction::GetPointer()->GetDetectorMaterial();
}

G4ThreeVector TargetParam::GetCenter(G4int copyN){
	G4int iz = copyN/(Nx*Ny);
	G4int ix = copyN%(Nx*Ny)%Nx;
	G4int iy = copyN%(Nx*Ny)/Nx;

	G4double x = ix*dX + dX/2 - (Nx*dX)/2;
	G4double y = iy*dY + dY/2 - Ny*dY/2;
	G4double z = 0;
	G4double wz = (target_thickness+(Nz-1)*(dZ+Gap))/2;
	if(iz<Nz/2) z = iz*(Gap+dZ)+dZ/2 - wz;
	else if(iz==Nz/2) z = iz*(Gap+dZ)+target_thickness/2 - wz;
	else z = iz*(Gap+dZ)+dZ/2  - wz -dZ + target_thickness;

	G4ThreeVector r(x,y,z);
	return r;
}

TargetParam* TargetParam::ftarget_param_ptr = 0;
TargetParam* TargetParam::GetPointer(){return ftarget_param_ptr;}

