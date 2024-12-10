void fuzzyRule() {
    // ============================================ //
    /////======= Solenoid Valve (RULE BASE) ======////
    // ============================================ //

  // Build FuzzyRule1 (pH= Asam(AC), Suhu= Cold(CD), Turbidity= Low(LT))
  FuzzyRuleAntecedent *AC_CD_1 = new FuzzyRuleAntecedent();
  AC_CD_1->joinWithAND(AC, CD);
  // 2Rule
  FuzzyRuleAntecedent *LT_1 = new FuzzyRuleAntecedent();
  LT_1->joinSingle(LT);
  // 3Rule
  FuzzyRuleAntecedent *AC_CD_LT = new FuzzyRuleAntecedent();
  AC_CD_LT->joinWithAND(AC_CD_1, LT_1);
    FuzzyRuleConsequent *KOLAM_1 = new FuzzyRuleConsequent();
  KOLAM_1->addOutput(ON_V);
  KOLAM_1->addOutput(ON_H);
  // =====
  FuzzyRule *fuzzyRule1 = new FuzzyRule(1, AC_CD_LT, KOLAM_1);
  fuzzy->addFuzzyRule(fuzzyRule1);

  // Build FuzzyRule2 (pH= Asam(AC), Suhu= Cold(CD), Turbidity= Medium(MT))
  FuzzyRuleAntecedent *AC_CD_2 = new FuzzyRuleAntecedent();
  AC_CD_2->joinWithAND(AC, CD);
  // 2Rule
  FuzzyRuleAntecedent *MT_1 = new FuzzyRuleAntecedent();
  MT_1->joinSingle(MT);
  // 3Rule
  FuzzyRuleAntecedent *AC_CD_MT = new FuzzyRuleAntecedent();
  AC_CD_MT->joinWithAND(AC_CD_2, MT_1);
  FuzzyRuleConsequent *KOLAM_2 = new FuzzyRuleConsequent();
  KOLAM_2->addOutput(ON_V);
  KOLAM_2->addOutput(ON_H);
  // =====
  FuzzyRule *fuzzyRule2 = new FuzzyRule(2, AC_CD_MT, KOLAM_2);
  fuzzy->addFuzzyRule(fuzzyRule2);

  // Build FuzzyRule3 (pH= Asam(AC), Suhu= Cold(CD), Turbidity= High(HT))
  FuzzyRuleAntecedent *AC_CD_3 = new FuzzyRuleAntecedent();
  AC_CD_3->joinWithAND(AC, CD);
  // 2Rule
  FuzzyRuleAntecedent *HT_1 = new FuzzyRuleAntecedent();
  HT_1->joinSingle(HT);
  // 3Rule
  FuzzyRuleAntecedent *AC_CD_HT = new FuzzyRuleAntecedent();
  AC_CD_HT->joinWithAND(AC_CD_3, HT_1);
  FuzzyRuleConsequent *KOLAM_3 = new FuzzyRuleConsequent();
  KOLAM_3->addOutput(ON_V);
  KOLAM_3->addOutput(ON_H);
  // =====
  FuzzyRule *fuzzyRule3 = new FuzzyRule(3, AC_CD_HT, KOLAM_3);
  fuzzy->addFuzzyRule(fuzzyRule3);

  // Build FuzzyRule4 (pH= Asam(AC), Suhu= Normal(NM), Turbidity= Low(LT))
  FuzzyRuleAntecedent *AC_NM_1 = new FuzzyRuleAntecedent();
  AC_NM_1->joinWithAND(AC, NM);
  // 2Rule
  FuzzyRuleAntecedent *LT_2 = new FuzzyRuleAntecedent();
  LT_2->joinSingle(LT);
  // 3Rule
  FuzzyRuleAntecedent *AC_NM_LT = new FuzzyRuleAntecedent();
  AC_NM_LT->joinWithAND(AC_NM_1, LT_2);
    FuzzyRuleConsequent *KOLAM_4 = new FuzzyRuleConsequent();
  KOLAM_4->addOutput(ON_V);
  KOLAM_4->addOutput(OFF_H);
  // =====
  FuzzyRule *fuzzyRule4 = new FuzzyRule(4, AC_NM_LT, KOLAM_4);
  fuzzy->addFuzzyRule(fuzzyRule4);

  // Build FuzzyRule5 (pH= Asam(AC), Suhu= Normal(NM), Turbidity= Medium(MT))
  FuzzyRuleAntecedent *AC_NM_2 = new FuzzyRuleAntecedent();
  AC_NM_2->joinWithAND(AC, NM);
  // 2Rule
  FuzzyRuleAntecedent *MT_2 = new FuzzyRuleAntecedent();
  MT_2->joinSingle(MT);
  // 3Rule
  FuzzyRuleAntecedent *AC_NM_MT = new FuzzyRuleAntecedent();
  AC_NM_MT->joinWithAND(AC_NM_2, MT_2);
  FuzzyRuleConsequent *KOLAM_5 = new FuzzyRuleConsequent();
  KOLAM_5->addOutput(ON_V);
  KOLAM_5->addOutput(OFF_H);
  // =====
  FuzzyRule *fuzzyRule5 = new FuzzyRule(5, AC_NM_MT, KOLAM_5);
  fuzzy->addFuzzyRule(fuzzyRule5);

  // Build FuzzyRule6 (pH= Asam(AC), Suhu= Normal(NM), Turbidity= High(HT))
  FuzzyRuleAntecedent *AC_NM_3 = new FuzzyRuleAntecedent();
  AC_NM_3->joinWithAND(AC, NM);
  // 2Rule
  FuzzyRuleAntecedent *HT_2 = new FuzzyRuleAntecedent();
  HT_2->joinSingle(HT);
  // 3Rule
  FuzzyRuleAntecedent *AC_NM_HT = new FuzzyRuleAntecedent();
  AC_NM_HT->joinWithAND(AC_NM_3, HT_2);
  FuzzyRuleConsequent *KOLAM_6 = new FuzzyRuleConsequent();
  KOLAM_6->addOutput(ON_V);
  KOLAM_6->addOutput(OFF_H);
  // =====
  FuzzyRule *fuzzyRule6 = new FuzzyRule(6, AC_NM_HT, KOLAM_6);
  fuzzy->addFuzzyRule(fuzzyRule6);

  // Build FuzzyRule7 (pH= Netral(NT), Suhu= Cold(CD), Turbidity= Low(LT))
  FuzzyRuleAntecedent *NT_CD_1 = new FuzzyRuleAntecedent();
  NT_CD_1->joinWithAND(NT, CD);
  // 2Rule
  FuzzyRuleAntecedent *LT_3 = new FuzzyRuleAntecedent();
  LT_3->joinSingle(LT);
  // 3Rule
  FuzzyRuleAntecedent *NT_CD_LT = new FuzzyRuleAntecedent();
  NT_CD_LT->joinWithAND(NT_CD_1, LT_3);
    FuzzyRuleConsequent *KOLAM_7 = new FuzzyRuleConsequent();
  KOLAM_7->addOutput(OFF_V);
  KOLAM_7->addOutput(ON_H);
  // =====
  FuzzyRule *fuzzyRule7 = new FuzzyRule(7, NT_CD_LT, KOLAM_7);
  fuzzy->addFuzzyRule(fuzzyRule7);

  // Build FuzzyRule8 (pH= Netral(NT), Suhu= Cold(CD), Turbidity= Medium(MT))
  FuzzyRuleAntecedent *NT_CD_2 = new FuzzyRuleAntecedent();
  NT_CD_2->joinWithAND(NT, CD);
  // 2Rule
  FuzzyRuleAntecedent *MT_3 = new FuzzyRuleAntecedent();
  MT_3->joinSingle(MT);
  // 3Rule
  FuzzyRuleAntecedent *NT_CD_MT = new FuzzyRuleAntecedent();
  NT_CD_MT->joinWithAND(NT_CD_2, MT_3);
  FuzzyRuleConsequent *KOLAM_8 = new FuzzyRuleConsequent();
  KOLAM_8->addOutput(OFF_V);
  KOLAM_8->addOutput(ON_H);
  // =====
  FuzzyRule *fuzzyRule8 = new FuzzyRule(8, NT_CD_MT, KOLAM_8);
  fuzzy->addFuzzyRule(fuzzyRule8);

  // Build FuzzyRule9 (pH= Netral(NT), Suhu= Cold(CD), Turbidity= High(HT))
  FuzzyRuleAntecedent *NT_CD_3 = new FuzzyRuleAntecedent();
  NT_CD_3->joinWithAND(AC, CD);
  // 2Rule
  FuzzyRuleAntecedent *HT_3 = new FuzzyRuleAntecedent();
  HT_3->joinSingle(HT);
  // 3Rule
  FuzzyRuleAntecedent *NT_CD_HT = new FuzzyRuleAntecedent();
  NT_CD_HT->joinWithAND(NT_CD_3, HT_3);
  FuzzyRuleConsequent *KOLAM_9 = new FuzzyRuleConsequent();
  KOLAM_9->addOutput(ON_V);
  KOLAM_9->addOutput(ON_H);
  // =====
  FuzzyRule *fuzzyRule9 = new FuzzyRule(9, NT_CD_HT, KOLAM_9);
  fuzzy->addFuzzyRule(fuzzyRule9);

  // Build FuzzyRule10 (pH= Netral(NT), Suhu= Normal(NM), Turbidity= Low(LT))
  FuzzyRuleAntecedent *NT_NM_1 = new FuzzyRuleAntecedent();
  NT_NM_1->joinWithAND(NT, NM);
  // 2Rule
  FuzzyRuleAntecedent *LT_4 = new FuzzyRuleAntecedent();
  LT_4->joinSingle(LT);
  // 3Rule
  FuzzyRuleAntecedent *NT_NM_LT = new FuzzyRuleAntecedent();
  NT_NM_LT->joinWithAND(NT_NM_1, LT_4);
    FuzzyRuleConsequent *KOLAM_10 = new FuzzyRuleConsequent();
  KOLAM_10->addOutput(OFF_V);
  KOLAM_10->addOutput(OFF_H);
  // =====
  FuzzyRule *fuzzyRule10 = new FuzzyRule(10, NT_NM_LT, KOLAM_10);
  fuzzy->addFuzzyRule(fuzzyRule10);

  // Build FuzzyRule11 (pH= Netral(NT), Suhu= Normal(NM), Turbidity= Medium(MT))
  FuzzyRuleAntecedent *NT_NM_2 = new FuzzyRuleAntecedent();
  NT_NM_2->joinWithAND(NT, NM);
  // 2Rule
  FuzzyRuleAntecedent *MT_4 = new FuzzyRuleAntecedent();
  MT_4->joinSingle(MT);
  // 3Rule
  FuzzyRuleAntecedent *NT_NM_MT = new FuzzyRuleAntecedent();
  NT_NM_MT->joinWithAND(NT_NM_2, MT_4);
    FuzzyRuleConsequent *KOLAM_11 = new FuzzyRuleConsequent();
  KOLAM_11->addOutput(OFF_V);
  KOLAM_11->addOutput(OFF_H);
  // =====
  FuzzyRule *fuzzyRule11 = new FuzzyRule(11, NT_NM_MT, KOLAM_11);
  fuzzy->addFuzzyRule(fuzzyRule11);

  // Build FuzzyRule12 (pH= Netral(NT), Suhu= Normal(NM), Turbidity= High(HT))
  FuzzyRuleAntecedent *NT_NM_3 = new FuzzyRuleAntecedent();
  NT_NM_3->joinWithAND(NT, NM);
  // 2Rule
  FuzzyRuleAntecedent *HT_4 = new FuzzyRuleAntecedent();
  HT_4->joinSingle(HT);
  // 3Rule
  FuzzyRuleAntecedent *NT_NM_HT = new FuzzyRuleAntecedent();
  NT_NM_HT->joinWithAND(NT_NM_3, HT_4);
  FuzzyRuleConsequent *KOLAM_12 = new FuzzyRuleConsequent();
  KOLAM_12->addOutput(ON_V);
  KOLAM_12->addOutput(OFF_H);
  // =====
  FuzzyRule *fuzzyRule12 = new FuzzyRule(12, NT_NM_HT, KOLAM_12);
  fuzzy->addFuzzyRule(fuzzyRule12);

  // Build FuzzyRule13 (pH= Netral(NT), Suhu= Hot(HO), Turbidity= Low(LT))
  FuzzyRuleAntecedent *NT_HO_1 = new FuzzyRuleAntecedent();
  NT_HO_1->joinWithAND(NT, HO);
  // 2Rule
  FuzzyRuleAntecedent *LT_5 = new FuzzyRuleAntecedent();
  LT_5->joinSingle(LT);
  // 3Rule
  FuzzyRuleAntecedent *NT_HO_LT = new FuzzyRuleAntecedent();
  NT_HO_LT->joinWithAND(NT_HO_1, LT_5);
    FuzzyRuleConsequent *KOLAM_13 = new FuzzyRuleConsequent();
  KOLAM_13->addOutput(OFF_V);
  KOLAM_13->addOutput(OFF_H);
  // =====
  FuzzyRule *fuzzyRule13 = new FuzzyRule(13, NT_HO_LT, KOLAM_13);
  fuzzy->addFuzzyRule(fuzzyRule13);

  // Build FuzzyRule14 (pH= Netral(NT), Suhu= Hot(HO), Turbidity= Medium(MT))
  FuzzyRuleAntecedent *NT_HO_2 = new FuzzyRuleAntecedent();
  NT_HO_2->joinWithAND(NT, HO);
  // 2Rule
  FuzzyRuleAntecedent *MT_5 = new FuzzyRuleAntecedent();
  MT_5->joinSingle(MT);
  // 3Rule
  FuzzyRuleAntecedent *NT_HO_MT = new FuzzyRuleAntecedent();
  NT_HO_MT->joinWithAND(NT_HO_2, MT_5);
  FuzzyRuleConsequent *KOLAM_14 = new FuzzyRuleConsequent();
  KOLAM_14->addOutput(OFF_V);
  KOLAM_14->addOutput(OFF_H);
  // =====
  FuzzyRule *fuzzyRule14 = new FuzzyRule(14, NT_HO_MT, KOLAM_14);
  fuzzy->addFuzzyRule(fuzzyRule14);

  // Build FuzzyRule15 (pH= Netral(NT), Suhu= Hot(HO), Turbidity= High(HT))
  FuzzyRuleAntecedent *NT_HO_3 = new FuzzyRuleAntecedent();
  NT_HO_3->joinWithAND(NT, HO);
  // 2Rule
  FuzzyRuleAntecedent *HT_5 = new FuzzyRuleAntecedent();
  HT_5->joinSingle(HT);
  // 3Rule
  FuzzyRuleAntecedent *NT_HO_HT = new FuzzyRuleAntecedent();
  NT_HO_HT->joinWithAND(NT_HO_3, HT_5);
  FuzzyRuleConsequent *KOLAM_15 = new FuzzyRuleConsequent();
  KOLAM_15->addOutput(ON_V);
  KOLAM_15->addOutput(OFF_H);
  // =====
  FuzzyRule *fuzzyRule15 = new FuzzyRule(15, NT_HO_HT, KOLAM_15);
  fuzzy->addFuzzyRule(fuzzyRule15);

  // Build FuzzyRule16 (pH= Basa(AL), Suhu= Cold(CD), Turbidity= Low(LT))
  FuzzyRuleAntecedent *AL_CD_1 = new FuzzyRuleAntecedent();
  AL_CD_1->joinWithAND(AL, CD);
  // 2Rule
  FuzzyRuleAntecedent *LT_6 = new FuzzyRuleAntecedent();
  LT_6->joinSingle(LT);
  // 3Rule
  FuzzyRuleAntecedent *AL_CD_LT = new FuzzyRuleAntecedent();
  AL_CD_LT->joinWithAND(AL_CD_1, LT_6);
  FuzzyRuleConsequent *KOLAM_16 = new FuzzyRuleConsequent();
  KOLAM_16->addOutput(ON_V);
  KOLAM_16->addOutput(ON_H);
  // =====
  FuzzyRule *fuzzyRule16 = new FuzzyRule(16, AL_CD_LT, KOLAM_16);
  fuzzy->addFuzzyRule(fuzzyRule16);

  // Build FuzzyRule17 (pH= Basa(AL), Suhu= Cold(CD), Turbidity= Midium(MT))
  FuzzyRuleAntecedent *AL_CD_3 = new FuzzyRuleAntecedent();
  AL_CD_3->joinWithAND(AL, CD);
  // 2Rule
  FuzzyRuleAntecedent *MT_6 = new FuzzyRuleAntecedent();
  MT_6->joinSingle(MT);
  // 3Rule
  FuzzyRuleAntecedent *AL_CD_MT = new FuzzyRuleAntecedent();
  AL_CD_MT->joinWithAND(AL_CD_3, MT_6);
  FuzzyRuleConsequent *KOLAM_17 = new FuzzyRuleConsequent();
  KOLAM_17->addOutput(ON_V);
  KOLAM_17->addOutput(ON_H);
  // =====
  FuzzyRule *fuzzyRule17 = new FuzzyRule(17, AL_CD_MT, KOLAM_17);
  fuzzy->addFuzzyRule(fuzzyRule17);

  // Build FuzzyRule18 (pH= Basa(AL), Suhu= Cold(CD), Turbidity= High(HT))
  FuzzyRuleAntecedent *AL_CD_2 = new FuzzyRuleAntecedent();
  AL_CD_2->joinWithAND(AL, CD);
  // 2Rule
  FuzzyRuleAntecedent *HT_6 = new FuzzyRuleAntecedent();
  HT_6->joinSingle(HT);
  // 3Rule
  FuzzyRuleAntecedent *AL_CD_HT = new FuzzyRuleAntecedent();
  AL_CD_HT->joinWithAND(AL_CD_2, HT_6);
  FuzzyRuleConsequent *KOLAM_18 = new FuzzyRuleConsequent();
  KOLAM_18->addOutput(ON_V);
  KOLAM_18->addOutput(ON_H);
  // =====
  FuzzyRule *fuzzyRule18 = new FuzzyRule(18, AL_CD_HT, KOLAM_18);
  fuzzy->addFuzzyRule(fuzzyRule18);

  // Build FuzzyRule19 (pH= Basa(AL), Suhu= Normal(NM), Turbidity= Low(LT))
  FuzzyRuleAntecedent *AL_NM_1 = new FuzzyRuleAntecedent();
  AL_NM_1->joinWithAND(AL, NM);
  // 2Rule
  FuzzyRuleAntecedent *LT_7 = new FuzzyRuleAntecedent();
  LT_7->joinSingle(LT);
  // 3Rule
  FuzzyRuleAntecedent *AL_NM_LT = new FuzzyRuleAntecedent();
  AL_NM_LT->joinWithAND(AL_NM_1, LT_7);
  FuzzyRuleConsequent *KOLAM19 = new FuzzyRuleConsequent();
  KOLAM19->addOutput(ON_V);
  KOLAM19->addOutput(OFF_H);
  // =====
  FuzzyRule *fuzzyRule19 = new FuzzyRule(19, AL_NM_LT, KOLAM19);
  fuzzy->addFuzzyRule(fuzzyRule19);

  // Build FuzzyRule20 (pH= Asam(AL), Suhu= Normal(NM), Turbidity= Medium(MT))
  FuzzyRuleAntecedent *AL_NM_2 = new FuzzyRuleAntecedent();
  AL_NM_2->joinWithAND(AL, NM);
  // 2Rule
  FuzzyRuleAntecedent *MT_7 = new FuzzyRuleAntecedent();
  MT_7->joinSingle(MT);
  // 3Rule
  FuzzyRuleAntecedent *AL_NM_MT = new FuzzyRuleAntecedent();
  AL_NM_MT->joinWithAND(AL_NM_2, MT_7);
  FuzzyRuleConsequent *KOLAM_20 = new FuzzyRuleConsequent();
  KOLAM_20->addOutput(ON_V);
  KOLAM_20->addOutput(OFF_H);
  // =====
  FuzzyRule *fuzzyRule20 = new FuzzyRule(20, AL_NM_MT, KOLAM_20);
  fuzzy->addFuzzyRule(fuzzyRule20);

  // Build FuzzyRule21 (pH= Asam(AL), Suhu= Normal(NM), Turbidity= High(HT))
  FuzzyRuleAntecedent *AL_NM_3 = new FuzzyRuleAntecedent();
  AL_NM_3->joinWithAND(AL, NM);
  // 2Rule
  FuzzyRuleAntecedent *HT_7 = new FuzzyRuleAntecedent();
  HT_7->joinSingle(HT);
  // 3Rule
  FuzzyRuleAntecedent *AL_NM_HT = new FuzzyRuleAntecedent();
  AL_NM_HT->joinWithAND(AL_NM_3, HT_7);
  FuzzyRuleConsequent *KOLAM_21 = new FuzzyRuleConsequent();
  KOLAM_21->addOutput(ON_V);
  KOLAM_21->addOutput(OFF_H);
  // =====
  FuzzyRule *fuzzyRule21 = new FuzzyRule(21, AL_NM_HT, KOLAM_21);
  fuzzy->addFuzzyRule(fuzzyRule21);

  // ============================================ //
  /////========== Aerator (RULE BASE) ==========////
  // =========================================== //
  // Build FuzzyRule22 (Saturation= Low(LS), E_Do= Zero(ZD))
  FuzzyRuleAntecedent *LS_ZD = new FuzzyRuleAntecedent();
  LS_ZD->joinWithAND(LS, ZD);
  // 2Rule
    FuzzyRuleConsequent *AER_1_1 = new FuzzyRuleConsequent();
  AER_1_1->addOutput(AER_1);
  // =====
  FuzzyRule *fuzzyRule22 = new FuzzyRule(22, LS_ZD, AER_1_1);
  fuzzy->addFuzzyRule(fuzzyRule22);

  // Build FuzzyRule23 (Saturation= Low(LS), E_Do= Low(LD))
  FuzzyRuleAntecedent *LS_LD = new FuzzyRuleAntecedent();
  LS_LD->joinWithAND(LS, LD);
  // 2Rule
    FuzzyRuleConsequent *AER_1_2 = new FuzzyRuleConsequent();
  AER_1_2->addOutput(AER_1);
  // =====
  FuzzyRule *fuzzyRule23 = new FuzzyRule(23, LS_LD, AER_1_2);
  fuzzy->addFuzzyRule(fuzzyRule23);

  // Build FuzzyRule24 (Saturation= Low(LS), E_Do= Medium(MD))
  FuzzyRuleAntecedent *LS_MD = new FuzzyRuleAntecedent();
  LS_MD->joinWithAND(LS, MD);
  // 2Rule
    FuzzyRuleConsequent *AER_2_1 = new FuzzyRuleConsequent();
  AER_2_1->addOutput(AER_2);
  // =====
  FuzzyRule *fuzzyRule24 = new FuzzyRule(24, LS_MD, AER_2_1);
  fuzzy->addFuzzyRule(fuzzyRule24);

  // Build FuzzyRule25 (Saturation= Low(LS), E_Do= High(HD))
  FuzzyRuleAntecedent *LS_HD = new FuzzyRuleAntecedent();
  LS_HD->joinWithAND(LS, HD);
  // 2Rule
    FuzzyRuleConsequent *AER_3_1 = new FuzzyRuleConsequent();
  AER_3_1->addOutput(AER_3);
  // =====
  FuzzyRule *fuzzyRule25 = new FuzzyRule(25, LS_HD, AER_3_1);
  fuzzy->addFuzzyRule(fuzzyRule25);

  // Build FuzzyRule26 (Saturation= Medium(MS), E_Do= Zero(ZD))
  FuzzyRuleAntecedent *MS_ZD = new FuzzyRuleAntecedent();
  MS_ZD->joinWithAND(MS, ZD);
  // 2Rule
    FuzzyRuleConsequent *AER_1_3 = new FuzzyRuleConsequent();
  AER_1_3->addOutput(AER_1);
  // =====
  FuzzyRule *fuzzyRule26 = new FuzzyRule(26, MS_ZD, AER_1_3);
  fuzzy->addFuzzyRule(fuzzyRule26);

  // Build FuzzyRule27 (Saturation= Medium(MS), E_Do= Low(LD))
  FuzzyRuleAntecedent *MS_LD = new FuzzyRuleAntecedent();
  MS_LD->joinWithAND(MS, LD);
  // 2Rule
    FuzzyRuleConsequent *AER_1_4 = new FuzzyRuleConsequent();
  AER_1_4->addOutput(AER_1);
  // =====
  FuzzyRule *fuzzyRule27 = new FuzzyRule(27, MS_LD, AER_1_4);
  fuzzy->addFuzzyRule(fuzzyRule27);

  // Build FuzzyRule28 (Saturation= Medium(MS), E_Do= Medium(MD))
  FuzzyRuleAntecedent *MS_MD = new FuzzyRuleAntecedent();
  MS_MD->joinWithAND(MS, MD);
  // 2Rule
    FuzzyRuleConsequent *AER_2_2 = new FuzzyRuleConsequent();
  AER_2_2->addOutput(AER_2);
  // =====
  FuzzyRule *fuzzyRule28 = new FuzzyRule(28, MS_MD, AER_2_2);
  fuzzy->addFuzzyRule(fuzzyRule28);

  // Build FuzzyRule29 (Saturation= Medium(MS), E_Do= High(HD))
  FuzzyRuleAntecedent *MS_HD = new FuzzyRuleAntecedent();
  MS_HD->joinWithAND(MS, HD);
  // 2Rule
    FuzzyRuleConsequent *AER_3_2 = new FuzzyRuleConsequent();
  AER_3_2->addOutput(AER_3);
  // =====
  FuzzyRule *fuzzyRule29 = new FuzzyRule(29, MS_HD, AER_3_2);
  fuzzy->addFuzzyRule(fuzzyRule29);

  // Build FuzzyRule30 (Saturation= High(HS), E_Do= Zero(LD))
  FuzzyRuleAntecedent *HS_ZD = new FuzzyRuleAntecedent();
  HS_ZD->joinWithAND(HS, ZD);
  // 2Rule
    FuzzyRuleConsequent *AER_1_5 = new FuzzyRuleConsequent();
  AER_1_5->addOutput(AER_1);
  // =====
  FuzzyRule *fuzzyRule30 = new FuzzyRule(30, HS_ZD, AER_1_5);
  fuzzy->addFuzzyRule(fuzzyRule30);

  // Build FuzzyRule31 (Saturation= High(HS), E_Do= Low(LD))
  FuzzyRuleAntecedent *HS_LD = new FuzzyRuleAntecedent();
  HS_LD->joinWithAND(HS, LD);
  // 2Rule
    FuzzyRuleConsequent *AER_2_3 = new FuzzyRuleConsequent();
  AER_2_3->addOutput(AER_2);
  // =====
  FuzzyRule *fuzzyRule31 = new FuzzyRule(31, HS_LD, AER_2_3);
  fuzzy->addFuzzyRule(fuzzyRule31);

  // Build FuzzyRule32 (Saturation= High(HS), E_Do= Medium(MD))
  FuzzyRuleAntecedent *HS_MD = new FuzzyRuleAntecedent();
  HS_MD->joinWithAND(HS, MD);
  // 2Rule
    FuzzyRuleConsequent *AER_3_3 = new FuzzyRuleConsequent();
  AER_3_3->addOutput(AER_3);
  // =====
  FuzzyRule *fuzzyRule32 = new FuzzyRule(32, HS_MD, AER_3_3);
  fuzzy->addFuzzyRule(fuzzyRule32);

  // Build FuzzyRule33 (Saturation= High(HS), E_Do= High(HD))
  FuzzyRuleAntecedent *HS_HD = new FuzzyRuleAntecedent();
  HS_HD->joinWithAND(HS, HD);
  // 2Rule
    FuzzyRuleConsequent *AER_3_4 = new FuzzyRuleConsequent();
  AER_3_4->addOutput(AER_3);
  // =====
  FuzzyRule *fuzzyRule33 = new FuzzyRule(33, HS_HD, AER_3_4);
  fuzzy->addFuzzyRule(fuzzyRule33);
}