void angle_pos_initdir(){
    TFile *f= new TFile("anti_001_1000000.root");
    TTree *an_det =(TTree*) f.Get("an_det");
    Double_t p_x, p_y, p_z, p_cap_x, p_cap_y, p_cap_z, p_theta, p_phi, r_len, v1_x, v1_y, v1_z, v2_x, v2_y, v2_z, angle;
    TH1F *h_angle = new TH1F("h_angle","Angle between the positron displacement and its initial velocity",100,-15,180);
    an_det->SetBranchAddress("pos_rx", &p_x);
    an_det->SetBranchAddress("pos_ry", &p_y);
    an_det->SetBranchAddress("pos_rz", &p_z);
    an_det->SetBranchAddress("pos_cap_rx", &p_cap_x);
    an_det->SetBranchAddress("pos_cap_ry", &p_cap_y);
    an_det->SetBranchAddress("pos_cap_rz", &p_cap_z);
    an_det->SetBranchAddress("pos_theta", &p_theta);
    an_det->SetBranchAddress("pos_phi", &p_phi);
    Int_t nentries=(Int_t)an_det->GetEntries();
    for (Int_t i = 0; i< nentries; i++){
        an_det->GetEntry(i);
	v1_x = p_cap_x - p_x;
	v1_y = p_cap_y - p_y;
	v1_z = p_cap_z - p_z;
	r_len = sqrt(v1_x**2 + v1_y**2 + v1_z**2);
        v2_x = sin(p_theta)*cos(p_phi);
	v2_y = sin(p_theta)*sin(p_phi);
	v2_z = cos(p_theta);
	angle = acos((v1_x * v2_x + v1_y * v2_y + v1_z * v2_z)/r_len)/3.14159*180;
//	angle = (v1_x * v2_x + v1_y * v2_y + v1_z * v2_z);
	h_angle->Fill(angle);
    }
//    f->Close();
    h_angle->Draw();
    //TFile f1("hist_A001_1E6.root","new");
    //h_angle->Write();
    //f1.Close();
}
