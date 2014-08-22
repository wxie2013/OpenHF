{
    nt = new TNtuple("nt", "", "x:y");
    for(int i = 0; i<100000; i++) {
        float a = gRandom->Gaus(10, 2);
        float b = gRandom->Gaus(6, 3);
        float c = gRandom->Gaus(3, 1);
        float x = a+b;
        float y = c+b;
        nt->Fill(x, y);
    }
}
