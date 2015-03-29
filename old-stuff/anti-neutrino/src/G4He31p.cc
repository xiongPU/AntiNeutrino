#include "G4He31p.hh"
#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"
#include "G4ParticleTable.hh"

// ######################################################################
// ###                           3He+                                 ###
// ######################################################################

G4He31p* G4He31p::theInstance = 0;

G4He31p* G4He31p::Definition()
{
  if (theInstance !=0) return theInstance;
  const G4String name = "He31p";
  // search in particle table]
  G4ParticleTable* pTable = G4ParticleTable::GetParticleTable();
  G4Ions* anInstance =  reinterpret_cast<G4Ions*>(pTable->FindParticle(name));
  if (anInstance ==0)
  {
  // create particle
  //
  //    Arguments for constructor are as follows
  //               name             mass          width         charge
  //             2*spin           parity  C-conjugation
  //          2*Isospin       2*Isospin3       G-parity
  //               type    lepton number  baryon number   PDG encoding
  //             stable         lifetime    decay table
  //             shortlived      subType    anti_encoding
  //             excitation 
    G4double  year = 31.536000e06 * second;
    anInstance = new G4Ions(
			    name,    2808902399.68*eV,       0.0*MeV,  0.0,
			    1,              +1,             0,
			    0,               0,             0,
			    "nucleus",               0,            +3, 1010020030,
                 true,        1e+31*year,          NULL,
		false,        "static",   -1010020030, 
                  0.0
              );
 
    // Magnetic Moment
    G4double mN = eplus*hbar_Planck/2./(proton_mass_c2 /c_squared);
    anInstance->SetPDGMagneticMoment( 2.97896248 * mN);

   }

  theInstance = reinterpret_cast<G4He31p*>(anInstance);
  return theInstance;
}

G4He31p*  G4He31p::He31pDefinition()
{
  return Definition();
}

G4He31p*  G4He31p::He31p()
{
  return Definition();
}


