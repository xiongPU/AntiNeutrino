#include "StepMaxMessenger.hh"

#include "StepMax.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

StepMaxMessenger::StepMaxMessenger(StepMax* stepM)
:fStepMax(stepM){
	fStepMaxCmd = new G4UIcmdWithADoubleAndUnit("/testpt/stepMax",this);
	fStepMaxCmd->SetGuidance("Set max allowed step length");
	fStepMaxCmd->SetParameterName("mxStep",false);
	fStepMaxCmd->SetRange("mxStep>0.");
	fStepMaxCmd->SetUnitCategory("Length");
}

StepMaxMessenger::~StepMaxMessenger(){
	delete fStepMaxCmd;
}

void StepMaxMessenger::SetNewValue(G4UIcommand* command, G4String newValue){ 
	if (command == fStepMaxCmd){
		fStepMax->SetMaxStep(fStepMaxCmd->GetNewDoubleValue(newValue));
	}
}
