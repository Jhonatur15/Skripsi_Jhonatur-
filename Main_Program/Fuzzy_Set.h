void fuzzySet(){
  
  // FuzzyInput pH
  FuzzyInput *pH = new FuzzyInput(1);
  pH->addFuzzySet(AC);
  pH->addFuzzySet(NT);
  pH->addFuzzySet(AL);
  fuzzy->addFuzzyInput(pH);

  // FuzzyInput TEMP
  FuzzyInput *TempOut = new FuzzyInput(2);
  TempOut->addFuzzySet(CD);
  TempOut->addFuzzySet(NM);
  TempOut->addFuzzySet(HO);
  fuzzy->addFuzzyInput(TempOut);
  
  // FuzzyInput TURB
  FuzzyInput *turb = new FuzzyInput(3);
  turb->addFuzzySet(LT);
  turb->addFuzzySet(MT);
  turb->addFuzzySet(HT);
  fuzzy->addFuzzyInput(turb);

  // FuzzyInput Saturation
  FuzzyInput *sat = new FuzzyInput(4);
  sat->addFuzzySet(LS);
  sat->addFuzzySet(MS);
  sat->addFuzzySet(HS);
  fuzzy->addFuzzyInput(sat);
  
  // FuzzyInput ERROR DO
  FuzzyInput *e_do = new FuzzyInput(5);
  e_do->addFuzzySet(ZD);
  e_do->addFuzzySet(LD);
  e_do->addFuzzySet(MD);
  e_do->addFuzzySet(HD);
  fuzzy->addFuzzyInput(e_do);

  // FuzzyOutput RELAY (Valve)
  FuzzyOutput *sirku = new FuzzyOutput(1);
  sirku->addFuzzySet(OFF_V);
  sirku->addFuzzySet(ON_V);
  fuzzy->addFuzzyOutput(sirku);

  // FuzzyOutput RELAY (Aerator)
  FuzzyOutput *aerat = new FuzzyOutput(2);
  aerat->addFuzzySet(AER_0);
  aerat->addFuzzySet(AER_1);
  aerat->addFuzzySet(AER_2);
  aerat->addFuzzySet(AER_3);
  fuzzy->addFuzzyOutput(aerat);

  FuzzyOutput *heat = new FuzzyOutput(3);
  heat->addFuzzySet(OFF_H);
  heat->addFuzzySet(ON_H);
  fuzzy->addFuzzyOutput(heat);

}