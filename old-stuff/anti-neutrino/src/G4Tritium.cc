#include "G4Tritium.hh"
#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"
#include "G4ParticleTable.hh"

// ######################################################################
// ###                           Tritium                              ###
// ######################################################################

G4Tritium* G4Tritium::theInstance = 0;

G4Tritium* G4Tritium::Definition()
{
  if (theInstance !=0) return theInstance;
  const G4String name = "Tritium";
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
			    name,    2809431989.86*eV,       0.0*MeV,  0.0,
			    1,              +1,             0,
			    0,               0,             0,
			    "nucleus",               0,            +3, 1010010030,
                 false,      12.32*year,          NULL,
		false,        "static",   -1010010030, 
                  0.0
              );
 
    // Magnetic Moment
    G4double mN = eplus*hbar_Planck/2./(proton_mass_c2 /c_squared);
    anInstance->SetPDGMagneticMoment( 2.97896248 * mN);

   }

  theInstance = reinterpret_cast<G4Tritium*>(anInstance);
  return theInstance;
}

G4Tritium*  G4Tritium::TritiumDefinition()
{
  return Definition();
}

G4Tritium*  G4Tritium::Tritium()
{
  return Definition();
}


