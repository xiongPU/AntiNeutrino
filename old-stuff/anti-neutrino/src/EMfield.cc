// this file implements the algorithm for calculating fields
// GetFieldValue should modify field value by Bfield pointer
// the evaluation point is point[]

#include "EMfield.hh"
#include "G4SystemOfUnits.hh"
#include <fstream>

G4String EMfield::field_map_name = "NULL";
EMfield* EMfield::EMfield_ptr = NULL;
EMfield* EMfield::GetEMFieldPointer(){
	return EMfield_ptr;
}

void EMfield::SetFieldMapName(G4String s){
	field_map_name = s;
}

EMfield::EMfield(G4String s){
	LoadField(s);
	EMfield_ptr = this;
}

EMfield::~EMfield(){

	G4cout<<"Deleting fields..."<<G4endl;
	if(field_is_loaded)
		for( int i=0; i<Nr; i++){
			delete [] Er[i];
			delete [] Ez[i];
			delete [] Br[i];
			delete [] Bz[i];
		}
	delete [] Er;
	delete [] Ez;
	delete [] Br;
	delete [] Bz;

	EMfield_ptr = NULL;
}

void EMfield::LoadField(G4String name){
	if(field_map_name=="NULL") field_map_name = name;
	
	std::ifstream file( field_map_name);

	G4int Nr,Nz;
	file>>Rmin>>Rmax>>Nr>>Zmin>>Zmax>>Nz;
	Rmin*=mm;	Rmax*=mm;
	Zmin*=mm;	Zmax*=mm;
	dr = (Rmax-Rmin)/(Nr-1);
	dz = (Zmax-Zmin)/(Nz-1);

	Er = new G4double* [Nr];
	Br = new G4double* [Nr];
	Ez = new G4double* [Nr];
	Bz = new G4double* [Nr];

	for( int i=0; i<Nr; i++){
		Er[i] = new G4double[Nz];
		Br[i] = new G4double[Nz];
		Ez[i] = new G4double[Nz];
		Bz[i] = new G4double[Nz];	
	}

	G4cout<<"Loading "<<field_map_name<<", PTOLEMY field-map generated from COMSOL multiphysics...\n";

	for( int i=0; i<Nz; i++){
		for( int j=0; j<Nr; j++){
			G4double r,z;
			file>>r>>z>>Er[j][i]>>Ez[j][i]>>Br[j][i]>>Bz[j][i];
		}
	}
	if(Er){
		field_is_loaded = true;
		G4cout<< "Field map successfully loaded..."<<G4endl;
		G4cout<<"From "<<field_map_name<<":\t"<<Nr<<"\t radial points, and "<<Nz<<"\t axial points..."<<G4endl;
	}
	else{
		G4cout<<" Problem loading field map...\n";
		field_is_loaded = false;
	}
}

void EMfield::GetFieldValue(const G4double point[], G4double* Bfield) const{

	G4double r = sqrt(point[0]*point[0]+point[1]*point[1]);
	G4double z = point[2];
	r /= mm;
	z /= mm;

//	G4cout<<"Evaluating at r="<<r<<G4endl;
//	G4cout<<"Input is\t"<<r<<'\t'<<z<<G4endl;
//	G4cout<<r<<"\t["<<Rmin<<","<<Rmax<<"], "<<z<<"\t["<<Zmin<<","<<Zmax<<"]"<<G4endl;
	if( r<Rmin|| r>Rmax-dr || z<Zmin+dz || z>Zmax-dr){
		for(int i=0;i<6;i++) Bfield[i] = 0;
//		G4cout<<"**********Field Evaluation Out of range**********\n";
		return;
	}
//	G4cout<<"Before: "<<(r-Rmin)/dr+1<<'\t'<<(z-Zmin)/dz+1<<G4endl;
	G4int Nr_floor = G4int((r-Rmin)/dr+1);
	G4int Nz_floor = G4int((z-Zmin)/dz+1);
//	G4cout<<"Two floors: "<<Nr_floor<<'\t'<<Nz_floor<<G4endl;

	G4double r1 = Rmin + dr*(Nr_floor-1);
	G4double r2 = r1 + dr;
	G4double z1 = Zmin + dz*(Nz_floor-1);
	G4double z2 = z1 + dz;

	if( r<r1 || r>r2 || z<z1 ||z>z2 ) G4cout<<"Error! Point not in range...\n";

//----------------------Interpolation-Bilinear-----------------------------

	G4double Erad(0), Brad(0);
//	G4cout<<Nr_floor<<'\t'<<Nz_floor<<'\t'<<Er[Nr_floor][Nz_floor]<<'\t'<<Er[Nr_floor+1][Nz_floor+1]<<G4endl;
	Erad = Er[Nr_floor][Nz_floor]*(r2-r)*(z2-z) + Er[Nr_floor+1][Nz_floor]*(r-r1)*(z2-z) + Er[Nr_floor][Nz_floor+1]*(r2-r)*(z-z1) + Er[Nr_floor+1][Nz_floor+1]*(r-r1)*(z-z1);
//	G4cout<<Erad<<G4endl;
	Brad = Br[Nr_floor][Nz_floor]*(r2-r)*(z2-z) + Br[Nr_floor+1][Nz_floor]*(r-r1)*(z2-z) + Br[Nr_floor][Nz_floor+1]*(r2-r)*(z-z1) + Br[Nr_floor+1][Nz_floor+1]*(r-r1)*(z-z1);

	Erad /= (dr*dz);
	Erad*= 1*volt/m;
	Bfield[3] = Erad * point[0]/r;
	Bfield[4] = Erad * point[1]/r;

	Brad /= (dr*dz);
	Brad*= tesla;
	Bfield[0] = Brad * point[0]/r;
	Bfield[1] = Brad * point[1]/r;


	Bfield[5] = Ez[Nr_floor][Nz_floor]*(r2-r)*(z2-z) + Ez[Nr_floor+1][Nz_floor]*(r-r1)*(z2-z) + Ez[Nr_floor][Nz_floor+1]*(r2-r)*(z-z1) + Ez[Nr_floor+1][Nz_floor+1]*(r-r1)*(z-z1);
	Bfield[5] /= (dr*dz);
	Bfield[5] *= 1*volt/m;

	Bfield[2] = Bz[Nr_floor][Nz_floor]*(r2-r)*(z2-z) + Bz[Nr_floor+1][Nz_floor]*(r-r1)*(z2-z) + Bz[Nr_floor][Nz_floor+1]*(r2-r)*(z-z1) + Bz[Nr_floor+1][Nz_floor+1]*(r-r1)*(z-z1);
	Bfield[2] /= (dr*dz);
	Bfield[2] *= tesla;
}
