#include "PrimaryGeneratorAction.hh"

PrimaryGeneratorAction* PrimaryGeneratorAction::fPrimaryGeneratorAction = 0;

PrimaryGeneratorAction::PrimaryGeneratorAction():particle_number(1),particle_energy(18.6*keV),particle_position(G4ThreeVector(0,0,0)),particle_momentum_dir(G4ThreeVector(0,0,1)){
	particle_name = "neutron";
	tritium_src = new Tritium("/home/suerfu/PTOLEMY/ptolemy/src/Tritium_beta_spectrum.dat");
	fParticleGun  = new G4ParticleGun(particle_number);
	fPrimaryGeneratorAction = this;
}

PrimaryGeneratorAction::~PrimaryGeneratorAction()
{
	G4cout<<"Deleting particle gun..."<<G4endl;
	delete fParticleGun;
	fPrimaryGeneratorAction = 0;
}

G4double PrimaryGeneratorAction::EnergyDistribution(G4double e){
	G4double x = e/keV;
	G4double sigma = 4.0;
	G4double mean = 18;
	return 1.0/(2*pi*sigma)*exp((x-mean)*(x-mean)/(2*sigma*sigma));
}

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
	G4ParticleDefinition* particle = G4ParticleTable::GetParticleTable()->FindParticle(particle_name);
	fParticleGun->SetParticleDefinition(particle);

	DetectorConstruction* detector = DetectorConstruction::GetPointer();
	G4double r = detector->GetTargetThickness();
	G4double l = detector->GetTargetLength();
	G4double R = detector->GetTargetRadius();

	G4double theta = 2*pi*G4UniformRand();
	G4double rad = R+r*0.5;//*G4UniformRand();	//needs an improved sampling algorithm

	particle_position = detector->GetTargetPosition() + G4ThreeVector(rad*cos(theta),rad*sin(theta),l*(G4UniformRand()-0.5));
	fParticleGun->SetParticlePosition(particle_position);	

	theta = acos(G4UniformRand());
	G4double phi = particle_position.getPhi();//2*pi*G4UniformRand();
	particle_momentum_dir = G4ThreeVector(sin(theta)*cos(phi),sin(theta)*sin(phi),cos(theta));
	fParticleGun->SetParticleMomentumDirection(particle_momentum_dir);

	particle_energy = 0.01*MeV;// + (3*MeV-0.5*MeV)*G4UniformRand();
	if(particle_name=="e+") particle_energy = 4*MeV;
//	particle_energy = tritium_src->GetEnergy();
	fParticleGun->SetParticleEnergy(particle_energy);    

	fParticleGun->GeneratePrimaryVertex(anEvent);
}

Tritium::Tritium(G4String name):T(0),prob(0),N(0),lbin(0),hbin(0),d(0){
	std::fstream file(name,std::fstream::in);
	file>>lbin>>hbin>>N;

	T = new G4double [N];
	prob = new G4double [N];
	for(int i=0;i<N;i++){
		T[i] = 0;
		prob[i] = 0;
	}

	d = (hbin-lbin)/N;

	for(int i=0;i<N;i++) file>>T[i]>>prob[i];
}

Tritium::~Tritium(){
	delete T;	T = 0;
	delete prob;	prob = 0;
}

G4double Tritium::GetEnergy(){	// use Monte-Carlo sampling/metropolis algorithm to produce energy distribution

	G4double E = 0;
	G4bool Accp = false;
	while(Accp ==false){
		E  = G4UniformRand()*(hbin-lbin) + lbin;
		G4int n = G4int((E-lbin)/d);

		G4double p;
		if(n<N-1) p = prob[n] + (prob[n+1]-prob[n])*(E-T[n])/d;	// linear interpolation
		else if(n==N-1) p = prob[N-1];
		else{
			G4cout<<"Error in Tritium::GetEnergy, n value is larger than "<<N<<G4endl;
			p = 0;
		}

		if(G4UniformRand()<p)	Accp = true;
	}
	return E*keV;
}
