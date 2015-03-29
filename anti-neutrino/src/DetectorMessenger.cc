#include "DetectorMessenger.hh"

DetectorMessenger::DetectorMessenger(DetectorConstruction* p): detector(p){

	newDir = new G4UIdirectory("/det/");
	newDir->SetGuidance("Configure detector geometry.");

	cmdNx = new G4UIcmdWithAnInteger("/det/nx", this);
	cmdNx -> SetGuidance("Set number of detector array in x-direction.");
	cmdNx -> SetParameterName("Nx",false);
	cmdNx -> SetRange("Nx > 0");

	cmdNy = new G4UIcmdWithAnInteger("/det/ny", this);
	cmdNy -> SetGuidance("Set number of detector array in y-direction.");
	cmdNy -> SetParameterName("Ny",false);
	cmdNy -> SetRange("Ny > 0");

	cmdNz = new G4UIcmdWithAnInteger("/det/nz", this);
	cmdNz -> SetGuidance("Set number of detector array in z-direction.");
	cmdNz -> SetParameterName("Nz",false);
	cmdNz -> SetRange("Nz > 0");

	cmddX = new G4UIcmdWithADoubleAndUnit("/det/dx",this);
	cmddX -> SetGuidance("Set x-dimension of detector cell.");
	cmddX -> SetParameterName("dx",false);
	cmddX -> SetRange("dx>0.");
	cmddX -> SetDefaultUnit("cm");

	cmddY = new G4UIcmdWithADoubleAndUnit("/det/dy",this);
	cmddY -> SetGuidance("Set y-dimension of detector cell.");
	cmddY -> SetParameterName("dy",false);
	cmddY -> SetRange("dy>0.");
	cmddY -> SetDefaultUnit("cm");

	cmddZ = new G4UIcmdWithADoubleAndUnit("/det/dz",this);
	cmddZ -> SetGuidance("Set z-dimension of detector cell.");
	cmddZ -> SetParameterName("dz",false);
	cmddZ -> SetRange("dz>0.");
	cmddZ -> SetDefaultUnit("cm");

	cmdGap = new G4UIcmdWithADoubleAndUnit("/det/gap",this);
	cmdGap -> SetGuidance("Set spacing between detector arrays.");
	cmdGap -> SetParameterName("gap",false);
	cmdGap -> SetRange("gap>0.");
	cmdGap -> SetDefaultUnit("cm");
}

DetectorMessenger::~DetectorMessenger(){
	delete cmdNx; delete cmdNy; delete cmdNz;
	delete cmddX; delete cmddY; delete cmddZ;
	delete cmdGap;
}

void DetectorMessenger::SetNewValue(G4UIcommand* command, G4String newValue){
	G4cout<<"Reconfigurating geometry...\n";
	if(command==cmdNx){
		detector->SetNx(cmdNx->GetNewIntValue(newValue));
	}
	else if(command==cmdNy){
		detector->SetNy(cmdNy->GetNewIntValue(newValue));
	}
	else if(command==cmdNz){
		detector->SetNz(cmdNz->GetNewIntValue(newValue));
	}
	else if(command==cmddX){
		detector->SetLengthX(cmddX->GetNewDoubleValue(newValue));
	}
	else if(command==cmddY){
		detector->SetLengthY(cmddY->GetNewDoubleValue(newValue));
	}
	else if(command==cmddZ){
		detector->SetLengthZ(cmddZ->GetNewDoubleValue(newValue));
	}
	else if(command==cmdGap){
		detector->SetGap(cmdGap->GetNewDoubleValue(newValue));
	}
	else {};
	G4cout<<"Updating geometry..."<<G4endl;
	detector->UpdateGeometry();
}

G4String DetectorMessenger::GetCurrentValue(G4UIcommand* command){
	if(command==cmdNx){
		return cmdNx->ConvertToString(detector->GetNx());
	}
	else if(command==cmdNy){
		return cmdNy->ConvertToString(detector->GetNy());
	}
	else if(command==cmdNz){
		return cmdNz->ConvertToString(detector->GetNz());
	}
	else if(command==cmddX){
		return cmddX->ConvertToString(detector->GetLengthX(),"cm");
	}
	else if(command==cmddY){
		return cmddY->ConvertToString(detector->GetLengthY(),"cm");
	}
	else if(command==cmddZ){
		return cmddZ->ConvertToString(detector->GetLengthZ(),"cm");
	}
	else if(command==cmdGap){
		return cmdGap->ConvertToString(detector->GetGap(),"cm");
	}
	else return "NULL";
}
