/*
 *  apv_raw.h
 *  mmdaqToCMS
 *
 *  Created by S. Colafranceschi on 22/9/21.
 *  Creative Commons (CC BY), aka do whatever you please
 *
 */

#ifndef apv_raw_h
#define apv_raw_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TSelector.h>
#include <TTreeReader.h>
#include <TTreeReaderValue.h>
#include <TTreeReaderArray.h>

// Headers needed by this particular selector
#include <vector>

#include <string>


/**
 Class generated on Wed Sep 15 22:33:34 2021
 ROOT version 6.20/04
 from TTree apv_raw/APVRawData 
 found on file: run12.root provided by M.Maggi
*/

class apv_raw : public TSelector {
public :
   TTreeReader     fReader;  //!the tree reader
   TTree          *fChain = 0;   //!pointer to the analyzed TTree or TChain

   // Readers to access the data (delete the ones you do not need).
   TTreeReaderValue<ULong64_t> evt = {fReader, "evt"};
   TTreeReaderValue<UInt_t> error = {fReader, "error"};
   TTreeReaderValue<Int_t> daqTimeSec = {fReader, "daqTimeSec"};
   TTreeReaderValue<Int_t> daqTimeMicroSec = {fReader, "daqTimeMicroSec"};
   TTreeReaderValue<Int_t> srsTimeStamp = {fReader, "srsTimeStamp"};
   TTreeReaderValue<UInt_t> srsTrigger = {fReader, "srsTrigger"};
   TTreeReaderArray<unsigned int> srsFec = {fReader, "srsFec"};
   TTreeReaderArray<unsigned int> srsChip = {fReader, "srsChip"};
   TTreeReaderArray<unsigned int> srsChan = {fReader, "srsChan"};
   TTreeReaderArray<string> mmChamber = {fReader, "mmChamber"};
   TTreeReaderArray<int> mmLayer = {fReader, "mmLayer"};
   TTreeReaderArray<char> mmReadout = {fReader, "mmReadout"};
   TTreeReaderArray<int> mmStrip = {fReader, "mmStrip"};
   TTreeReaderArray<vector<short>> raw_q = {fReader, "raw_q"};
   TTreeReaderArray<short> max_q = {fReader, "max_q"};
   TTreeReaderArray<int> t_max_q = {fReader, "t_max_q"};

   static const Int_t MAXHITS = 100;
   static const Int_t MAXCLUSTER = 100;

   TFile* recofile;
   TTree* THit;
   TTree* TCluster;
   Int_t           evtID;
   Int_t           nCh;
   Int_t           hitTimebin[MAXHITS];
   Int_t           hitDistance[MAXHITS];
   Int_t           detID[MAXHITS];
   Float_t         planeID[MAXHITS];      
   Int_t           strip[MAXHITS];
   Int_t           srsChanMapped[MAXHITS];   
   Int_t           srsChanTemp[MAXHITS];
   Int_t           Sector[MAXHITS];
   Int_t	   Position[MAXHITS];
   Int_t           Offset[MAXHITS];
   Double_t        StripPitch[MAXHITS];
   Short_t         adc0[MAXHITS];
   Short_t         adc1[MAXHITS];
   Short_t         adc2[MAXHITS];
   Short_t         adc3[MAXHITS];
   Short_t         adc4[MAXHITS];
   Short_t         adc5[MAXHITS];
   Short_t         adc6[MAXHITS];
   Short_t         adc7[MAXHITS];
   Short_t         adc8[MAXHITS];
   Short_t         adc9[MAXHITS];
   Short_t         adc10[MAXHITS];
   Short_t         adc11[MAXHITS];
   Short_t         adc12[MAXHITS];
   Short_t         adc13[MAXHITS];
   Short_t         adc14[MAXHITS];
   Short_t         adc15[MAXHITS];
   Short_t         adc16[MAXHITS];
   Short_t         adc17[MAXHITS];
   Short_t         adc18[MAXHITS];
   Short_t         adc19[MAXHITS];
   Short_t         adc20[MAXHITS];
   Short_t         adc21[MAXHITS];
   Short_t         adc22[MAXHITS];
   Short_t         adc23[MAXHITS];
   Short_t         adc24[MAXHITS];
   Short_t         adc25[MAXHITS];
   Short_t         adc26[MAXHITS];
   Short_t         adc27[MAXHITS];

   Int_t           nclust;
   Float_t         clustPos[MAXCLUSTER];
   Float_t	   clustdetID[MAXCLUSTER];
   Float_t	   clustPlaneID[MAXCLUSTER];
   Int_t           clustSize[MAXCLUSTER];
   Float_t         clustADCs[MAXCLUSTER];
   Int_t           clustTimebin[MAXCLUSTER];

   ReadConf myconfiguration;
   std::string ConfigFile;


   apv_raw(TTree * /*tree*/ =0) { }
   virtual ~apv_raw() { }
   virtual Int_t   Version() const { return 2; }
   virtual void    Begin(TTree *tree);
   virtual void    SlaveBegin(TTree *tree);
   virtual void    Init(TTree *tree);
   virtual Bool_t  Notify();
   virtual Bool_t  Process(Long64_t entry);
   virtual Int_t   GetEntry(Long64_t entry, Int_t getall = 0) { return fChain ? fChain->GetTree()->GetEntry(entry, getall) : 0; }
   virtual void    SetOption(const char *option) { fOption = option; }
   virtual void    SetObject(TObject *obj) { fObject = obj; }
   virtual void    SetInputList(TList *input) { fInput = input; }
   virtual TList  *GetOutputList() const { return fOutput; }
   virtual void    SlaveTerminate();
   virtual void    Terminate();

   ClassDef(apv_raw,0);

};

#endif

#ifdef apv_raw_cxx
void apv_raw::Init(TTree *tree) {
   fReader.SetTree(tree);
}

Bool_t apv_raw::Notify() {
   return kTRUE;
}

#endif // #ifdef apv_raw_cxx
