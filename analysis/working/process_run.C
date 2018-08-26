void
process_run(Int_t RUNNUM=5, Int_t SORTNUM=0)
{
  if (SORTNUM==0) {
    TString name("/Users/heliosdigios/experiments/infl001_19O/root_data/run");
    name+=RUNNUM;
    name+=".root";
    //TString name("/Users/heliosdigios/experiments/h058_p35/root_data/run");
    //name+=RUNNUM;
    //name+=".root";
    TFile f(name);
    TTree *t1 = (TTree*)f.Get("tree");
    //  TTree *t1 = tree;
    t1->Process("/Users/heliosdigios/experiments/infl001_19O/sort_codes/GeneralSort.C++");
    f.Close();
  }

  else if (SORTNUM==1) {
    TString name("gen.root");
    TFile ff(name);
    TTree *t2 = (TTree*)ff.Get("gen_tree");
    t2->Process("../codes/Monitors.C++");
    //ff.Close();

  }
}