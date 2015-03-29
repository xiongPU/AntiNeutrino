#ifndef EMfield_h
#define EMfield_h 1

#include "G4ElectroMagneticField.hh"
#include "G4TransportationManager.hh"

class EMfield
#ifndef STANDALONE
 : public G4ElectroMagneticField
#endif

{
public:
	EMfield(G4String);
	~EMfield();

	void GetFieldValue( const G4double point[4], G4double *Bfield) const;
	void LoadField(G4String name);

	G4bool DoesFieldChangeEnergy() const {return true;}

	static EMfield* GetEMFieldPointer();
	static void SetFieldMapName(G4String);

private:
	G4double **Er, **Ez;
	G4double **Br, **Bz;
	G4int Nr,Nz;
	G4double Rmin,Rmax;
	G4double Zmin,Zmax;
	G4double dr, dz;
	G4bool field_is_loaded;

	static EMfield* EMfield_ptr;
	static G4String field_map_name;
};

#endif
