//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Sun Nov  7 01:26:45 2021 by ROOT version 6.24/04
// from TTree apv_raw_ped/APVRawPedestals
// found on file: GE21-X-M2-CERN-0001_Run0083_Physics_590uA_XRay_40k_50uA_250Evt_mmdaq.root
//////////////////////////////////////////////////////////

#ifndef apv_raw_ped_h
#define apv_raw_ped_h

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



class apv_raw_ped : public TSelector {
public :
   TTreeReader     fpedReader;  //!the tree reader
   TTree          *fpedChain = 0;   //!pointer to the analyzed TTree or TChain

   // Readers to access the data (delete the ones you do not need).
   TTreeReaderValue<ULong64_t> evt = {fpedReader, "evt"};
   TTreeReaderValue<UInt_t> error = {fpedReader, "error"};
   TTreeReaderValue<Int_t> daqTimeSec = {fpedReader, "daqTimeSec"};
   TTreeReaderValue<Int_t> daqTimeMicroSec = {fpedReader, "daqTimeMicroSec"};
   TTreeReaderValue<Int_t> srsTimeStamp = {fpedReader, "srsTimeStamp"};
   TTreeReaderValue<UInt_t> srsTrigger = {fpedReader, "srsTrigger"};
   TTreeReaderArray<unsigned int> srsFec = {fpedReader, "srsFec"};
   TTreeReaderArray<unsigned int> srsChip = {fpedReader, "srsChip"};
   TTreeReaderArray<unsigned int> srsChan = {fpedReader, "srsChan"};
   TTreeReaderArray<string> mmChamber = {fpedReader, "mmChamber"};
   TTreeReaderArray<int> mmLayer = {fpedReader, "mmLayer"};
   TTreeReaderArray<char> mmReadout = {fpedReader, "mmReadout"};
   TTreeReaderArray<int> mmStrip = {fpedReader, "mmStrip"};
   TTreeReaderArray<double> ped_mean = {fpedReader, "ped_mean"};
   TTreeReaderArray<double> ped_stdev = {fpedReader, "ped_stdev"};
   TTreeReaderArray<double> ped_sigma = {fpedReader, "ped_sigma"};


   apv_raw_ped(TTree * /*tree*/ =0) { }
   virtual ~apv_raw_ped() { }
   virtual Int_t   Version() const { return 2; }
   virtual void    Begin(TTree *tree);
   virtual void    SlaveBegin(TTree *tree);
   virtual void    Init(TTree *tree);
   virtual Bool_t  Notify();
   virtual Bool_t  Process(Long64_t entry);
   virtual Int_t   GetEntry(Long64_t entry, Int_t getall = 0) { return fpedChain ? fpedChain->GetTree()->GetEntry(entry, getall) : 0; }
   virtual void    SetOption(const char *option) { fOption = option; }
   virtual void    SetObject(TObject *obj) { fObject = obj; }
   virtual void    SetInputList(TList *input) { fInput = input; }
   virtual TList  *GetOutputList() const { return fOutput; }
   virtual void    SlaveTerminate();
   virtual void    Terminate();

   ClassDef(apv_raw_ped,0);

};

#endif

#ifdef apv_raw_ped_cxx
void apv_raw_ped::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the reader is initialized.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   fpedReader.SetTree(tree);
}

Bool_t apv_raw_ped::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}


#endif // #ifdef apv_raw_ped_cxx
