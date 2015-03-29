#include "PrimaryGeneratorAction.hh"

PrimaryGeneratorAction* PrimaryGeneratorAction::fPrimaryGeneratorAction = 0;

PrimaryGeneratorAction::PrimaryGeneratorAction():particle_number(1),particle_name("e+"),particle_energy(1*MeV),particle_position(G4ThreeVector(0,0,0)),particle_momentum_dir(G4ThreeVector(0,0,1)){

	fParticleGun  = new G4ParticleGun(particle_number);
	fPrimaryGeneratorAction = this;
	fGeneratorMessenger = new GeneratorMessenger(this);
	generator_choice = "uniform";

	neutron_position = G4ThreeVector(0,0,0); neutron_momentum_dir = G4ThreeVector(0,0,1);
	neutron_energy = 0.01*MeV;
	positron_position = G4ThreeVector(0,0,0); positron_momentum_dir = G4ThreeVector(0,1,1);
	positron_energy = 1*MeV;
	time_delay = 100*ns;	// needed to distinguish e+ from n.

	ix = DetectorConstruction::GetPointer()->GetNx()/2;
	iy = DetectorConstruction::GetPointer()->GetNy()/2;
	iz = DetectorConstruction::GetPointer()->GetNz()/2;

	anti_v = new AntiNeutrino();
}

PrimaryGeneratorAction::~PrimaryGeneratorAction(){
	G4cout<<"Deleting particle gun..."<<G4endl;
	delete fParticleGun;	fPrimaryGeneratorAction = 0;
	delete fGeneratorMessenger;	fGeneratorMessenger = 0;
	delete anti_v;	anti_v = 0;
}
//-----------------------------------------------------------------------------------------

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent){

	if(generator_choice=="manual"){
		G4ParticleDefinition* particle;
		particle = G4ParticleTable::GetParticleTable()->FindParticle(particle_name);

		fParticleGun->SetParticleDefinition(particle);
		fParticleGun->SetParticlePosition(particle_position);
		fParticleGun->SetParticleMomentumDirection(particle_momentum_dir);
		fParticleGun->SetParticleEnergy(particle_energy);    
		fParticleGun->GeneratePrimaryVertex(anEvent);
	}
	else{
		DetectorConstruction* detector = DetectorConstruction::GetPointer();//what does it do
		G4ThreeVector center = detector->GetCenter(ix, iy, iz);
		G4double lx = detector->GetLengthX();	lx = lx>10*cm?10*cm:lx;
		G4double ly = detector->GetLengthY();	ly = ly>10*cm?10*cm:ly;
		G4double lz = detector->GetThickness();

//---------------------set information for neutron-----------------------------------------
		G4ParticleDefinition* neutron_particle = G4ParticleTable::GetParticleTable()->FindParticle("neutron");
		fParticleGun->SetParticleDefinition(neutron_particle);

		anti_v->SetDistribution();
		neutron_position = center + G4ThreeVector(2*(G4UniformRand()-0.5)*lx,2*(G4UniformRand()-0.5)*ly,lz*(G4UniformRand()-0.5));	// neutron uniformly in the source cell.
		fParticleGun->SetParticlePosition(neutron_position);

		neutron_momentum_dir = anti_v->GetNeutronDirection();
		fParticleGun->SetParticleMomentumDirection(neutron_momentum_dir);

		neutron_energy = anti_v -> GetNeutronEnergy();
		fParticleGun->SetParticleEnergy(neutron_energy);

		fParticleGun->SetParticleTime(0*ns);
		fParticleGun->GeneratePrimaryVertex(anEvent);

//---------------------set information for positron-----------------------------------------
		G4ParticleDefinition* positron_particle = G4ParticleTable::GetParticleTable()->FindParticle("e+");
		fParticleGun->SetParticleDefinition(positron_particle);

		positron_position = neutron_position;
		fParticleGun->SetParticlePosition(positron_position);

		positron_momentum_dir = anti_v->GetPositronDirection();
//anti_v has a sampling for neutrin
		fParticleGun->SetParticleMomentumDirection(positron_momentum_dir);

		positron_energy = anti_v -> GetPositronEnergy();
		fParticleGun->SetParticleEnergy(positron_energy);

		fParticleGun->SetParticleTime(-time_delay);	// EM processes are fast
		fParticleGun->GeneratePrimaryVertex(anEvent);

#ifdef G4VIS_USE
		fParticleGun->SetParticleDefinition(G4ParticleTable::GetParticleTable()->FindParticle("nu_e"));	
		fParticleGun->SetParticleEnergy(anti_v->GetNeutrinoEnergy());
		fParticleGun->SetParticlePosition(positron_position);
		fParticleGun->SetParticleMomentumDirection(-anti_v->GetNeutrinoDirection());
		fParticleGun->GeneratePrimaryVertex(anEvent);
#endif
	}
}
