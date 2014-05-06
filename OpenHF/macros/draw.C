void draw(char* filename, int ich, bool allTrg)
{
    gSystem->Load("../../../../lib/slc5_amd64_gcc462/libUserCodeOpenHF.so");

    FindUsefulTrigger *a = new FindUsefulTrigger;

    TCanvas* cc = new TCanvas("cc", "", 1000, 1000);
    a->draw(filename, ich, cc, allTrg);

}
