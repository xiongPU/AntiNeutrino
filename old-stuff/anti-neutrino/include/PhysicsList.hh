#ifndef PhysicsList_h
#define PhysicsList_h 1

#include "G4VModularPhysicsList.hh"
#include "globals.hh"

class G4VPhysicsConstructor;
class PhysicsListMessenger;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class PhysicsList: public G4VModularPhysicsList
{
  public:
    PhysicsList();
   ~PhysicsList();

    virtual void ConstructParticle();
    virtual void ConstructProcess(); 
               
    void AddPhysicsList(const G4String& name);
    void AddStepMax();       
    
    virtual void SetCuts();
    void SetCutForGamma(G4double);
    void SetCutForElectron(G4double);
    void SetCutForPositron(G4double);
      
  private:
  
    PhysicsListMessenger*  fMessenger; 

    G4VPhysicsConstructor* fParticleList;
    G4VPhysicsConstructor* fRaddecayList;

    G4String               fEmName;
    G4VPhysicsConstructor* fEmPhysicsList;

    G4double fCutForGamma;
    G4double fCutForElectron;
};

#endif
