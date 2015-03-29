#ifndef G4He31p_h
#define G4He31p_h 1

#include "globals.hh"
#include "G4ios.hh"
#include "G4ParticleDefinition.hh"
#include "G4Ions.hh"

// ######################################################################
// ###                         3He+                                   ###
// ######################################################################

class G4He31p : public G4Ions
{
 private:
   static G4He31p* theInstance;
   G4He31p(){}
   ~G4He31p(){}

 public:
   static G4He31p* Definition();
   static G4He31p* He31pDefinition();
   static G4He31p* He31p();
};

#endif
