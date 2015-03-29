#include "GeneratorMessenger.hh"


GeneratorMessenger::GeneratorMessenger(PrimaryGeneratorAction* p): generator(p){

	newDir = new G4UIdirectory("/generator/");
	newDir->SetGuidance("Configure particle generator.");

	cmdParticleName = new G4UIcmdWithAString("/generator/particle", this);
	cmdParticleName -> SetGuidance("Set particle type.");
	cmdParticleName -> SetParameterName("particle_name",false);

	cmdSpectrum = new G4UIcmdWithAString("/generator/spectrum", this);
	cmdSpectrum -> SetGuidance("Set energy spectrum.\n uniform and Tritium is currently available.");
	cmdSpectrum -> SetParameterName("energy_spectrum",false);

	cmdEnergy = new G4UIcmdWithADoubleAndUnit("/generator/energy",this);
	cmdEnergy -> SetGuidance("Set particle energy.");
	cmdEnergy -> SetParameterName("energy",false);
	cmdEnergy -> SetRange("energy>0.");
	cmdEnergy ->SetDefaultUnit("keV");

	cmdPosition = new G4UIcmdWith3VectorAndUnit("/generator/position",this);
	cmdPosition -> SetGuidance("Set particle position.");
	cmdPosition -> SetParameterName("x","y","z",false,false);
	cmdPosition ->SetDefaultUnit("mm");

	cmdDirection = new G4UIcmdWith3Vector("/generator/direction",this);
	cmdDirection -> SetGuidance("Set particle momentum direction.");
	cmdDirection -> SetParameterName("x","y","z", false,false);

	cmdCell = new G4UIcmdWith3Vector("/generator/cell",this);
	cmdCell -> SetGuidance("Set by index the cell particle momentum direction.");
	cmdCell -> SetParameterName("ix","iy","iz", false,false);
	cmdCell -> SetRange("ix>0 && iy>0 && iz>0");

//	cmdRange = new G4UIcmdWith3VectorAndUnit("/generator/range",this);
//	cmdRange -> SetGuidance("Set particle energy range. \nThird argument >0 for e+, <0 for n.");
//	cmdRange -> SetParameterName("x","y","z",false,false);
//	cmdRange ->SetDefaultUnit("keV");
}

GeneratorMessenger::~GeneratorMessenger(){
	delete cmdEnergy;	cmdEnergy = 0;
	delete cmdParticleName;	cmdParticleName = 0;
	delete cmdSpectrum;	cmdSpectrum = 0;
	delete cmdPosition;	cmdPosition = 0;
	delete cmdDirection;	cmdDirection = 0;
	delete cmdCell;	cmdCell = 0;
//	delete cmdRange;	cmdRange = 0;
}

void GeneratorMessenger::SetNewValue(G4UIcommand* command, G4String newValue){
	if(command==cmdParticleName){
		generator->SetParticleName(newValue);
	}
	else if(command==cmdSpectrum){
		generator->SetSpectrum(newValue);
	}
	else if(command==cmdEnergy){
		generator->SetParticleEnergy(cmdEnergy->GetNewDoubleValue(newValue));
	}
	else if(command==cmdPosition){
		generator->SetParticlePosition(cmdPosition->GetNew3VectorValue(newValue));
	}
	else if(command==cmdDirection){
		generator->SetParticleMomentumDirection(cmdDirection->GetNew3VectorValue(newValue));
	}
	else if(command==cmdCell){
		G4ThreeVector t = cmdDirection->GetNew3VectorValue(newValue);
		generator->SetCellIndex(G4int(round(t.x())), G4int(round(t.y())), G4int(round(t.z())));
	}
/*	else if(command==cmdRange){
		G4ThreeVector t = cmdRange->GetNew3VectorValue(newValue);
		G4String s = "null";
		if(t.z()<0) s = "neutron";	else s = "e+";
		generator->SetEnergyMin(t.x(),s);
		generator->SetEnergyMax(t.y(),s);
	}
*/	else {};
}

G4String GeneratorMessenger::GetCurrentValue(G4UIcommand* command){
	if(command==cmdParticleName){
		return generator->GetParticleName();
	}
	else if(command==cmdSpectrum){
		return generator->GetSpectrum();
	}
	else if(command==cmdEnergy){
		return cmdEnergy->ConvertToString(generator->GetParticleEnergy(),"keV");
	}
	else if(command==cmdPosition){
		return cmdPosition->ConvertToString(generator->GetParticlePosition(),"mm");
	}
	else if(command==cmdDirection){
		return cmdDirection->ConvertToString(generator->GetParticleMomentumDirection());
	}
	else if(command==cmdCell){
		return cmdCell->ConvertToString(generator->GetCellIndex());
	}
/*	else if(command==cmdRange){
		G4ThreeVector r(generator->GetEnergyMin(), generator->GetEnergyMax(),0);
		return cmdRange->ConvertToString(r);
	}
*/	else return "NULL";
}
