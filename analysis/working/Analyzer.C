#define Analyzer_cxx

#include "Analyzer.h"
#include <TH2.h>
#include <TStyle.h>
#include <TH1.h>
#include <TCanvas.h>
#include <TBenchmark.h>
#include <TMath.h>
//#include "../AutoCali/AutoFit.C"
int numRow = 6;
int numCol = 5;
const int numDet = numRow * numCol ;
double rangeEx[3] = { 50, -1, 5}; // resol. [keV], low Ex, high Ex
double rangeCM[3] = {1, 0, 45}; // resol. [deg], low deg, high deg

double ExOffset[30] = { 
 0.021, 0.0105,   1000, 0.0129,  0.0423, 
  1000, 0.0047, 0.0811,   1000, -0.0006, 
 1000, 0.0384, 0.0093, 0.0041, -0.5447,
 0.0011, 0.0249, 0.0229, 0.0535, -0.0004,
 0.0243, 0.0247, 0.0083, -0.0126, -0.1546,
 1000, -0.1075, 0.0211, 0.0551, 0.1028};


//=========================== Global histograms
TH1F * hRun; // runID
TH1F * hMultiHit ; 

TH1F ** hEBISi; // EBIS for each detector

TH1F ** hei; // e
TH2F ** hexi; // e vs x for each detector

TH1F ** hxi; // x for each detector

TH2F * hez;  // e vs z
TH2F * heza;  // e vs z, |x| < 0.9
TH2F ** hezr; // e vs z for each row

TH2F ** hxExi; // x vs Ex for each detector

TH1F * hEx; // Ex
TH1F ** hExi; // Ex for each detector

TH1F ** hExc; // Ex for each column
TH1F ** hExcA; // Ex for each column more detectors summed
TH1F ** hExca; // Ex gated x<0
TH1F ** hExcb; // Ex gated x>0

TH2F ** hExzr; // Ex vs z

TH2F * hExT; // Ex vs thetaCM
TH2F * hExTa; // Ex vs thetaCM, |x| < 0.9

TH1F ** hExr; // Ex for each row
TH2F ** hExTr; // Ex vs thetaCM for each row 

TH1F * hT; // thetaCM
TH1F * hTa; // thetaCM, |x| < 0.9

TCanvas * cAna;
Int_t totnumEntry;
TBenchmark gClock;
Bool_t shown;

void Analyzer::Begin(TTree *tree)
{
   TString option = GetOption();

   totnumEntry = tree->GetEntries();
   hRun = new TH1F("hRun", "runID with gate; runID; count / run", 50, 20, 70);
   hMultiHit = new TH1F("hMultiHit", "MultiHit; hit; count / hit ", 10, 0, 10);
   
   //ex spectrum
   hEx   = new TH1F("hEx"  ,  Form("Ex; Ex[MeV]; count / %2.0f keV", rangeEx[0]) , (rangeEx[2]-rangeEx[1])/rangeEx[0]*1000., rangeEx[1], rangeEx[2]);

   // thetaCM spectrum
   hT  = new TH1F("hT",  Form("thetaCM ; thetaCM [deg]; count / %2.0f deg", rangeCM[0])     , (rangeCM[2]-rangeCM[1])/rangeCM[0], rangeCM[1], rangeCM[2]);
   hTa = new TH1F("hTa", Form("thetaCM (|x|<0.9); thetaCM [deg]; count / %2.0f deg", rangeCM[0]), (rangeCM[2]-rangeCM[1])/rangeCM[0], rangeCM[1], rangeCM[2]);
   
   //E vs Z
   hez  = new TH2F("hez" , "e - z; z [mm]; e [MeV]", 450, -450, 0, 400, 0, 10);
   heza = new TH2F("heza", "e - z (|x|<0.9); z [mm]; e [MeV]", 450, -450, 0, 400, 0, 10);
   
   //Ex vs thetaCM
   hExT  = new TH2F("hExT" , "Ex - thetaCM; thetaCM [deg]; Ex [MeV]", 400, 0, 50, 400, -1, 5);
   hExTa = new TH2F("hExTa", "Ex - thetaCM (|x|<0.9); thetaCM [deg]; Ex [MeV]", 400, 0, 50, 400, -1, 5);

   hEBISi = new TH1F *[numDet];
   hei = new TH1F *[numDet];
   hexi = new  TH2F *[numDet];
   hxi = new  TH1F *[numDet];
   hxExi = new TH2F *[numDet];
   hExi = new TH1F *[numDet];

   for( int i = 0; i < numDet; i ++){
      hei[i]     = new TH1F(Form("hei%d", i) , Form("e (det-%d); e [MeV]; count/ 10 keV", i), 1000, 0, 10);
      hxi[i]     = new TH1F(Form("hxi%d", i) , Form("x (det-%d); x [a.u.]; count ", i), 400, -1.2, 1.2);
      hEBISi[i]  = new TH1F(Form("hEBISi%d", i) , Form("e_t - EBIS_t (det-%d); time [us]; count/ 1 us", i), 1500, 0, 1500);
      hExi[i]    = new TH1F(Form("hExi%d", i) , Form("Ex (det-%d); Ex[MeV]; count/ %2.0f keV", i,  rangeEx[0]), (rangeEx[2]-rangeEx[1])/rangeEx[0]*1000., rangeEx[1], rangeEx[2]);
      hexi[i]    = new TH2F(Form("hexi%d", i) , Form("e - x (det-%d); x [a.u]; e [MeV]", i), 450, -1.2, 1.2, 400, 0, 10);
      hxExi[i]   = new TH2F(Form("hxExi%d", i), Form("x - Ex (det-%d); Ex [MeV); x [a.u.]", i), 400, -1, 5, 400, -1.2, 1.2);         
   }

   hezr = new TH2F * [numRow];
   hExTr = new TH2F * [numRow];
   hExzr = new TH2F * [numRow];
   hExr = new TH1F * [numRow];
   
   for( int i = 0; i < numRow; i++){
      hExTr[i] = new TH2F(Form("hExTr%d", i), Form("Ex - thetaCM (row-%d); thetaCM [deg]; Ex [MeV]", i) , 400, 0, 50, 400, -1, 5);
      hExzr[i] = new TH2F(Form("hExzr%d", i), Form("Ex - z (row-%d); z [mm]; Ex [MeV]", i) , 400, -450, 0, 400, -1, 5);
      hezr[i]  = new TH2F(Form("hezr%d", i) , Form("e - z (row-%d); z [mm]; e [MeV]", i) , 400, -450, 0, 400, 0, 13);
      hExr[i]  = new TH1F(Form("hExr%d", i) , Form("Ex (row-%d); e [MeV] ; count / %2.0f keV", i, rangeEx[0]) , (rangeEx[2]-rangeEx[1])/rangeEx[0]*1000., rangeEx[1], rangeEx[2]);
   }

   hExc = new TH1F * [numCol];
   hExcA = new TH1F * [numCol];
   hExca = new TH1F * [numCol];
   hExcb = new TH1F * [numCol];

   for( int i = 0; i < numCol; i++){
      hExc[i] = new TH1F(Form("hExc%d", i), Form("Ex (col-%d); Ex [MeV]; count / %2.0f keV", i, rangeEx[0]) , (rangeEx[2]-rangeEx[1])/rangeEx[0]*1000., rangeEx[1], rangeEx[2]);
      hExcA[i] = new TH1F(Form("hExcA%d", i), Form("Ex (col-%d) All; Ex [MeV]; count / %2.0f keV", i, rangeEx[0]) , (rangeEx[2]-rangeEx[1])/rangeEx[0]*1000., rangeEx[1], rangeEx[2]);
      hExca[i]  = new TH1F(Form("hExca%d",i) ,  Form("Ex (x<0, col-%d); Ex[MeV]; count / %2.0f keV",i, rangeEx[0]), (rangeEx[2]-rangeEx[1])/rangeEx[0]*1000., rangeEx[1], rangeEx[2]);
      hExcb[i]  = new TH1F(Form("hExcb%d",i) ,  Form("Ex (x>0, col-%d); Ex[MeV]; count / %2.0f keV",i, rangeEx[0]), (rangeEx[2]-rangeEx[1])/rangeEx[0]*1000., rangeEx[1], rangeEx[2]);   
   }
   
   
   gClock.Reset();
   gClock.Start("timer");
   shown = 0;
}

void Analyzer::SlaveBegin(TTree * /*tree*/)
{
   TString option = GetOption();
}

Bool_t Analyzer::Process(Long64_t entry)
{
   b_run->GetEntry(entry, 0);
   b_e->GetEntry(entry,0);
   //b_xf->GetEntry(entry,0);
   //b_xn->GetEntry(entry,0);
   b_x->GetEntry(entry,0);
   b_z->GetEntry(entry,0);
   b_Ex->GetEntry(entry, 0);
   b_thetaCM->GetEntry(entry, 0);
   b_e_t->GetEntry(entry, 0);
   //b_EBIS_t->GetEntry(entry, 0);
   //b_hitID->GetEntry(entry, 0);
   
   hRun->Fill(run);
   
   int multiHit = 0;

   
   for( int i = 0 ; i < numDet ; i++){
      
     //if( e_t[i] == 0 ) continue;
      
            
      //hEBISi[i]->Fill( (e_t[i]-ebis_t)/1e2 );
      
      //======== cut-EBIS
      //if( !(100 < (e_t[i]-ebis_t)/1e2 && (e_t[i]-ebis_t)/1e2 < 900 )) continue;
      //if( 11000 < (e_t-EBIS_t)/1e2 && (e_t-EBIS_t)/1e2 < 19000 ) 
      
     //======== cut-e
     if( TMath::IsNaN(e[i]) ) continue;
     if( e[i] < 1.2 ) continue;
     
      //======== cut-x
      if( !(TMath::Abs(x[i]) < 0.95) ) continue;
      
      //======== cut-hitID
      //if( hitID[i] != 0 ) continue;
      
      //======== cut-thetaCM
      if( thetaCM < 10 ) continue;
      
      //======== cut-det
      if( i == 2 || i == 5 || i == 8 || i == 10 || i == 14 || i == 24 || i == 29) continue;  // for ground state
      double scaling = 1;
      if( i%numCol == 0 ) scaling = numRow/3.;
      if( i%numCol == 1 ) scaling = numRow/6.;
      if( i%numCol == 2 ) scaling = numRow/5.;
      if( i%numCol == 3 ) scaling = numRow/5.;
      if( i%numCol == 4 ) scaling = numRow/3.;

      //======== special gate;
      //if( i == 16 && e[i] < 6 ) continue;
      
      Ex = Ex - ExOffset[i];
      
      multiHit ++;
      
      hei[i]->Fill( e[i] );
      hxi[i]->Fill( x[i] );
      hexi[i]->Fill(x[i], e[i]);
      
      hExi[i]->Fill(Ex);
      hxExi[i]->Fill(Ex, x[i]);

      hez->Fill(z[i],e[i]);
      hExT->Fill(thetaCM,Ex, scaling);
      hT->Fill(thetaCM, scaling);      
      
      heza->Fill(z[i],e[i]);
      hExTa->Fill(thetaCM,Ex, scaling);
      hTa->Fill(thetaCM, scaling);
      
      int row = -1;
      for( int ri = 0; ri < numRow; ri++){
        if( ri*numCol <= i && i < (ri+1)*numCol) row = ri;
      }
      hezr[row]->Fill(z[i], e[i]);
      hExTr[row]->Fill(thetaCM, Ex, scaling);
      hExzr[row]->Fill(z[i], Ex);
      
      hExcA[i%numCol]->Fill(Ex, scaling);
      hExr[row]->Fill(Ex, scaling);
      
      //======== special gate;
      //if( i == 1 || i == 5 || i == 17 || i == 6 || i == 7 || i == 8 || i == 12 || i == 18 || i == 19) continue;  // for beyound ground state
      
      hExc[i%numCol]->Fill(Ex, scaling);
      hEx->Fill(Ex, scaling);
      
     
      if( x[i] < 0) {
         hExca[i%numCol]->Fill(Ex);
      }else{
         hExcb[i%numCol]->Fill(Ex);
      }
      
   }
   
   hMultiHit->Fill(multiHit);
   
   //#################################################################### Timer  
   gClock.Stop("timer");
   Double_t time = gClock.GetRealTime("timer");
   gClock.Start("timer");

   if ( !shown ) {
      if (fmod(time, 10) < 1 ){
         printf( "%10lld[%3d%%]|%3.0f min %5.2f sec | expect:%5.2f min\n", 
               entry, 
               TMath::Nint((entry)*100./totnumEntry),
               TMath::Floor(time/60.), time - TMath::Floor(time/60.)*60.,
               totnumEntry*time/(entry+1.)/60.);
               shown = 1;
      }
   }else{
      if (fmod(time, 10) > 9 ){
         shown = 0;
      }
   }

   return kTRUE;
}

void Analyzer::SlaveTerminate()
{
}

double findXMax(TH1F ** hist){
  
  double max = 0;
  int maxDetID = -1;
  for( int i = 0; i < numDet; i++){
    if( i == 11 ) continue;
    double temp = hist[i]->GetMaximum();
    if( temp > max) {
      max = temp;
      maxDetID = i;
    }
  }
  printf("====== detID %d is max : %f \n", maxDetID, max);
  
  return max;
}

void Analyzer::Terminate()
{
   gClock.Stop("timer");
   Double_t time = gClock.GetRealTime("timer");
   printf("%10d[%3d%%]|%3.0f min %5.2f sec | finished.\n", 
               totnumEntry, 
               100,
               TMath::Floor(time/60.), time - TMath::Floor(time/60.)*60.);
  
   gStyle->SetOptStat("");
   
   //=========== load transfer.root
   TFile * ft = new TFile("transfer.root");
   TObjArray * fxList = (TObjArray *) ft->FindObjectAny("fxList");
   TObjArray * gList = (TObjArray *) ft->FindObjectAny("gList");
   
   //=========== load AutoFit.C
   gROOT->ProcessLine(".L ../Armory/AutoFit.C");
  
   //=========== Plot
   int nX = 5, nY = 6;
   int sizeX = 250, sizeY = 150;
   cAna = new TCanvas("cAna", "Analyzer", nX * sizeX, nY * sizeY);
   cAna->Divide(nX,nY);
   if( cAna->GetShowEditor() ) cAna->ToggleEditor(); 
   
   //double max = findXMax(hei)
   double max = findXMax(hExi);
   
   //hEx->Draw();
   
   for( int i = 0; i < numDet; i++){
     cAna->cd(i+1);
     cAna->cd(i+1)->SetGrid();
     //cAna->cd(i+1)->SetLogy();
     //hei[i]->GetYaxis()->SetRangeUser(0.5, max * 1.1);
     //hei[i]->Draw();
     
     //hxi[i]->Draw();
     
     //hEBISi[i]->Draw();
     //hExi[i]->GetYaxis()->SetRangeUser(0, max*1.2);
     //hExi[i]->Draw();
   
     //fit2GaussP1(hExi[i], 0.0, 0.05, 1.2, 0.05, -1, 1.5, 0);
   
     //hExPi[i]->SetLineColor(2);
     //hExPi[i]->Draw("same");
   
     //2-D 
     //hexi[i]->Draw(); 
     //hxExi[i]->SetMarkerStyle(7);
     //hxExi[i]->SetMarkerColor(4);
     //hxExi[i]->Draw("scat");
     hxExi[i]->Draw("colz");
   } 
   
   //for( int i = 0; i < numDet; i++){
   //   //hei[i]->GetYaxis()->SetRangeUser(0, max);
   //   hExi[i]->GetYaxis()->SetRangeUser(0, max);
   //   if( i < 6 ) {
   //      cAna->cd(i+1);
   //      hExi[i]->Draw();
   //   }
   //   if( 6 <= i && i < 12) {
   //      cAna->cd(i-6+1);
   //      hExi[i]->SetLineColor(2); 
   //      hExi[i]->Draw("same");
   //   }
   //   if(12 <= i && i < 18) {
   //      cAna->cd(i-12+1);
   //      hExi[i]->SetLineColor(kGreen+3); 
   //      hExi[i]->Draw("same");
   //   }
   //   if(18 <= i && i < numDet) {
   //      cAna->cd(i-18+1);
   //      hExi[i]->SetLineColor(6); 
   //      hExi[i]->Draw("same");
   //   }
   //} 
   
   //for( int i = 0; i < 4; i++){
   //   cAna->cd(i+1);
   //   //cAna->cd(i+1)->SetGrid();
   //   //hezr[i]->Draw("colz");
   //   //fxList->At(0)->Draw("same");
   //   //fxList->At(2)->Draw("same");
   //   //fxList->At(5)->Draw("same");
   //   //fxList->At(7)->Draw("same");
   //   //gList->At(0)->Draw("same");
   //   //gList->At(10)->Draw("same");
   //   //hezr[i]->Draw("colz same");
   //   
   //   hExTr[i]->Draw("colz");
   //   //hExzr[i]->Draw("colz");
   //   //hExPzr[i]->Draw("colz");
   //}
   
   //for( int i = 0; i < 6; i++){
   //   cAna->cd(i+1);
   //   cAna->cd(i+1)->SetGrid();
   //   hExc[i]->Draw();
   //}
   
   /**/
}
