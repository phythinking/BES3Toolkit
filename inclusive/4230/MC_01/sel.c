void sel()
{
    double n1[14];
    TChain t1("signal");
    for (int i = 1; i <= 22; i++)
    {
        t1.Add(Form("mc%d/A*.root", i));
    }
    n1[0] = t1.GetEntries();
    TChain t2("signal");
    t2.Add("all/A*.root");
    n1[1] = t2.GetEntries();

    TChain t9("truth");
    for (int i = 1; i <= 22; i++)
    {
        t9.Add(Form("mc%d/A*.root", i));
    }
    n1[8] = t9.GetEntries();
    TChain t10("truth");
    t10.Add("all/A*.root");
    n1[9] = t10.GetEntries();

    TChain t11("tag");
    for (int i = 1; i <= 22; i++)
    {
        t11.Add(Form("mc%d/A*.root", i));
    }
    n1[10] = t11.GetEntries();
    TChain t12("tag");
    t12.Add("all/A*.root");
    n1[11] = t12.GetEntries();

    TChain t13("taga");
    for (int i = 1; i <= 22; i++)
    {
        t13.Add(Form("mc%d/A*.root", i));
    }
    n1[12] = t13.GetEntries();
    TChain t14("taga");
    t14.Add("all/A*.root");
    n1[13] = t14.GetEntries();

    cout << "signal: " << n1[0] << "\t" << n1[1] << "\t" << n1[0] - n1[1] << endl;
    cout << "truth: " << n1[8] << "\t" << n1[9] << "\t" << n1[8] - n1[9] << endl;
    cout << "tag: " << n1[10] << "\t" << n1[11] << "\t" << n1[10] - n1[11] << endl;
    cout << "taga: " << n1[12] << "\t" << n1[13] << "\t" << n1[12] - n1[13] << endl;
}
