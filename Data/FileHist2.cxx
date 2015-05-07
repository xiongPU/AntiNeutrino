#include<fstream>
using namespace std;

int FileHist2(){
   TString str1="/home/suerfu/auger_spec.txt";
   ifstream f(str1);
   Double_t x, xmin, xmax;
   TString str2="/scratch/network/xiong/auger_spect_hist.root";
   Int_t i, n_x, n_bin, n_in1bin=50;
   //TTree *Tr= new TTree("Tr", "Data tree");
   //Tr->Branch("data", &x, "data/D");
   if(f>>x){
       xmin=x;
       xmax=x;
       n_x=1;
       }
   else {
       std::cout<<"no data in the file"<<endl;
       }
     
   while(f>>x){
      n_x++;
      if(x>xmax){
          xmax=x;
	  }
      else if(x<xmin){
          xmin=x;
	  }
   }
   f.close();
   n_bin=n_x/n_in1bin;
   TH1F *h1= new TH1F("h1", "auger Data file read", n_bin, xmin, xmax);
   TFile *f2 = new TFile(str2, "RECREATE");
   f.open(str1);
   //ifstream f2(str1);
   //h1->SetBins(n_bin,h1->GetXaxis()->GetXmin(), h1->GetXaxis()->GetXMax());
   //h1->SetBins(n_bin, h1->GetMinimum(), h1->GetMaximum());
   while(f>>x){
      h1->Fill(x);
   }
   f.close();
   h1->Draw();
   h1->Write();
   f2->Close();
}
 
        
