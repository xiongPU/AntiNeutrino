#ifndef G4Tritium_h
#define G4Tritium_h 1

#include "globals.hh"
#include "G4ios.hh"
#include "G4ParticleDefinition.hh"
#include "G4Ions.hh"

// ######################################################################
// ###                          Tritium                               ###
// ######################################################################

class G4Tritium : public G4Ions
{
 private:
   static G4Tritium* theInstance;
   G4Tritium(){}
   ~G4Tritium(){}

 public:
   static G4Tritium* Definition();
   static G4Tritium* TritiumDefinition();
   static G4Tritium* Tritium();
};

#endif
