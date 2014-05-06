#include <iostream>
#include <algorithm>
#include <map>

#include <TFile.h>
#include <TH1F.h>

using namespace std;

int ana()
{
    TFile f("check_useful_triggers_forest.root");

    TH1F* h = (TH1F*)f.Get("h");

    map<int, string> m;

    for(int i = 0; i<h->GetNbinsX(); i++) {
        string label(h->GetXaxis()->GetBinLabel(i+1));
        int count = h->GetBinContent(i+1);

        m.insert(pair<int, string>(count, label));
    }

    //
    for (std::map<int, string>::iterator it=m.begin(); it!=m.end(); ++it) {
        cout<<it->second<<"                         "<<it->first<<endl;
    }
}
