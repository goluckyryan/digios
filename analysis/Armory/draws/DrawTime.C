{

  c1->Clear();
  c1->Divide(2,3);
  c1->cd(1);
  psd_tree->Draw("(e_t[0]-t1_t[0]):(e_t[0]-t2_t[0])>>ht1t2(20,-585,-565,20,-560,-540)","","col");
   c1->cd(2);
  psd_tree->Draw("(e_t[0]-t1_t[0]):(e_t[0]-t3_t[0])>>ht1t3(20,-965,-945,20,-560,-540)","","col");
   c1->cd(3);
  psd_tree->Draw("(e_t[0]-t1_t[0]):(e_t[0]-t4_t[0])>>ht1t4(10,-555,-545,20,-560,-540)","","col");


  //c1->Clear();
 // c1->Divide(2,2);
   c1->cd(4);
  psd_tree->Draw("(e_t[0]-t2_t[0]):(e_t[0]-t3_t[0])>>ht2t3(20,-965,-945,20,-585,-565)","","col");
   c1->cd(5);
  psd_tree->Draw("(e_t[0]-t2_t[0]):(e_t[0]-t4_t[0])>>ht2t4(10,-555,-545,20,-585,-565)","","col");

  
 c1->cd(6);
  psd_tree->Draw("(e_t[0]-t3_t[0]):(e_t[0]-t4_t[0])>>ht3t4(10,-555,-545,20,-965,-945)","","col");


}
