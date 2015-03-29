#include "PhysicsList.hh"
#include "PhysicsListMessenger.hh"

#include "PhysListEmStandard.hh"
#include "PhysListEmLivermore.hh"
#include "PhysListEmPenelope.hh"

#include "G4LossTableManager.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"

#include "G4ParticleTypes.hh"
#include "G4IonConstructor.hh"
#include "G4PhysicsListHelper.hh"
#include "G4RadioactiveDecay.hh"
#include "G4UAtomicDeexcitation.hh"

#include "G4RadioactiveDecayPhysics.hh"
#include "G4DecayPhysics.hh"
#include "G4PhaseSpaceDecayChannel.hh"
#include "G4ProcessManager.hh"

#include "G4UserSpecialCuts.hh"

PhysicsList::PhysicsList() : G4VModularPhysicsList(){

	G4LossTableManager::Instance();
	fMessenger = new PhysicsListMessenger(this); 

  //add new units for radioActive decays
  // 
	const G4double minute = 60*second;
	const G4double hour   = 60*minute;
	const G4double day    = 24*hour;
	const G4double year   = 365*day;
	new G4UnitDefinition("minute", "min", "Time", minute);
	new G4UnitDefinition("hour",   "h",   "Time", hour);
	new G4UnitDefinition("day",    "d",   "Time", day);
	new G4UnitDefinition("year",   "y",   "Time", year);        

  //default physics
	fParticleList = new G4DecayPhysics();

  //radioactive decay physics
	fRaddecayList = new G4RadioactiveDecayPhysics();

  // EM physics
//  fEmName = G4String("standard");
	fEmName = G4String("penelope");
	fEmPhysicsList = new PhysListEmStandard(fEmName);
    
	defaultCutValue = 0.1*mm;
	fCutForGamma     = defaultCutValue;
	fCutForElectron  = defaultCutValue;

	SetVerboseLevel(1);
}

PhysicsList::~PhysicsList(){

  delete fParticleList;
  delete fRaddecayList;
  delete fEmPhysicsList;
  delete fMessenger;  
}


void PhysicsList::AddPhysicsList(const G4String& name){

	if (verboseLevel>1) {
		G4cout << "PhysicsList::AddPhysicsList: <" << name << ">" << G4endl;
	}

	if (name == fEmName) return;

	if (name == "standard") {

		fEmName = name;
		delete fEmPhysicsList;
		fEmPhysicsList = new PhysListEmStandard(name);
	}
	else if (name == "livermore") {

	fEmName = name;
	delete fEmPhysicsList;
	fEmPhysicsList = new PhysListEmLivermore(name);

	}
	else if (name == "penelope") {

	fEmName = name;
	delete fEmPhysicsList;
	fEmPhysicsList = new PhysListEmPenelope(name);
	}
	else {
		G4cout << "PhysicsList::AddPhysicsList: <" << name << ">"
			<< " is not defined"<< G4endl;
  }
}

// Bosons
#include "G4ChargedGeantino.hh"
#include "G4Geantino.hh"
#include "G4Gamma.hh"

// leptons
#include "G4Electron.hh"
#include "G4Positron.hh"

#include "G4MuonPlus.hh"
#include "G4MuonMinus.hh"

// Mesons
#include "G4PionPlus.hh"
#include "G4PionMinus.hh"

#include "G4KaonPlus.hh"
#include "G4KaonMinus.hh"

// Baryons
#include "G4Proton.hh"
#include "G4AntiProton.hh"
#include "G4Neutron.hh"
#include "G4AntiNeutron.hh"

// Nuclei
#include "G4Deuteron.hh"
#include "G4Triton.hh"
#include "G4Alpha.hh"
#include "G4GenericIon.hh"


void PhysicsList::ConstructParticle(){

// pseudo-particles
	G4Geantino::GeantinoDefinition();
	G4ChargedGeantino::ChargedGeantinoDefinition();
  
// gamma
	G4Gamma::GammaDefinition();

// leptons
	G4Electron::ElectronDefinition();
	G4Positron::PositronDefinition();
	G4NeutrinoE::NeutrinoEDefinition();
	G4AntiNeutrinoE::AntiNeutrinoEDefinition();

	G4MuonPlus::MuonPlusDefinition();
	G4MuonMinus::MuonMinusDefinition();
  
// mesons
	G4PionPlus::PionPlusDefinition();
	G4PionMinus::PionMinusDefinition();
	G4KaonPlus::KaonPlusDefinition();
	G4KaonMinus::KaonMinusDefinition();
  
// baryons
	G4Proton::ProtonDefinition();
	G4AntiProton::AntiProtonDefinition();
	G4Neutron::NeutronDefinition();
	G4AntiNeutron::AntiNeutronDefinition();
  
// ions
	G4Deuteron::DeuteronDefinition();
	G4Triton::TritonDefinition();
	G4Alpha::AlphaDefinition();
	G4GenericIon::GenericIonDefinition();

	G4IonConstructor iConstructor;
	iConstructor.ConstructParticle();  
}

#include "G4EmProcessOptions.hh"

void PhysicsList::ConstructProcess(){

	AddTransportation();
  // em
	fEmPhysicsList->ConstructProcess();
  // decays
	fParticleList->ConstructProcess();
	fRaddecayList->ConstructProcess();

	AddStepMax();
  
  // Em options
  //
	G4EmProcessOptions emOptions;
  
  //physics tables
  //
	emOptions.SetMinEnergy(10*eV);        //default 100 eV    
	emOptions.SetMaxEnergy(10*TeV);        //default 100 TeV 
	emOptions.SetDEDXBinning(12*20);        //default=12*7  
	emOptions.SetLambdaBinning(12*20);        //default=12*7
          
  //build CSDA range
  //
	emOptions.SetBuildCSDARange(true);
	emOptions.SetMaxEnergyForCSDARange(10*TeV);  
	emOptions.SetDEDXBinningForCSDARange(12*20);
   
  //ionization
  //
	emOptions.SetSubCutoff(false);        //default  

  // G4RadioactiveDecay* radioactiveDecay = new G4RadioactiveDecay();
  // radioactiveDecay->SetHLThreshold(-1.*s);
  // radioactiveDecay->SetICM(true);                //Internal Conversion
  // radioactiveDecay->SetARM(false);               //Atomic Rearangement
  
  // G4PhysicsListHelper* ph = G4PhysicsListHelper::GetPhysicsListHelper();  
  // ph->RegisterProcess(radioactiveDecay, G4GenericIon::GenericIon());
      
  // Deexcitation (in case of Atomic Rearangement)
  //
	G4UAtomicDeexcitation* de = new G4UAtomicDeexcitation();
	de->SetFluo(true);
	de->SetAuger(true);   
	de->SetPIXE(false);  
	G4LossTableManager::Instance()->SetAtomDeexcitation(de);  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "G4ProcessManager.hh"
#include "StepMax.hh"

void PhysicsList::AddStepMax(){

  // Step limitation seen as a process
	StepMax* stepMaxProcess = new StepMax();

	theParticleIterator->reset();
	while ((*theParticleIterator)()){
		G4ParticleDefinition* particle = theParticleIterator->value();
		G4ProcessManager* pmanager = particle->GetProcessManager();

		if (stepMaxProcess->IsApplicable(*particle) && !particle->IsShortLived()){
			pmanager ->AddDiscreteProcess(stepMaxProcess);
		}
		G4String particleName = particle->GetParticleName();
		if (particleName == "e-"){
			pmanager->AddDiscreteProcess(new G4UserSpecialCuts());
		}
	}
}

void PhysicsList::SetCuts(){
	if (verboseLevel >0){
		G4cout << "PhysicsList::SetCuts:";
		G4cout << "CutLength : " << G4BestUnit(defaultCutValue,"Length") << G4endl;
	}
  
  // set cut values for gamma at first and for e- second and next for e+,
  // because some processes for e+/e- need cut values for gamma
	SetCutValue(fCutForGamma, "gamma");
	SetCutValue(fCutForElectron, "e-");
	SetCutValue(fCutForElectron, "e+");

	if (verboseLevel>0) DumpCutValuesTable();
}

void PhysicsList::SetCutForGamma(G4double cut){
	fCutForGamma = cut;
	SetParticleCuts(fCutForGamma, G4Gamma::Gamma());
}

void PhysicsList::SetCutForElectron(G4double cut){
	fCutForElectron = cut;
	SetParticleCuts(fCutForElectron, G4Electron::Electron());
	SetParticleCuts(fCutForElectron, G4Positron::Positron());
}
