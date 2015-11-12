<?php

// Norwegian Stopwords
function autordf_stopwords_nb() {
  $sp = 'alle, andre, arbeid, begge, bort, bra, bruke, denne, der, deres, det,' .
      ' din, disse, eller, ene, eneste, enhver, enn, folk, for, fordi, forsÛke,' .
      ' fra, fÛr, fÛrst, gjorde, gjÛre, god, hadde, han, hans, hennes, her,' .
      ' hva, hvem, hver, hvilken, hvis, hvor, hvordan, hvorfor   i, ikke, inn,' .
      ' innen, kan, kunne, lage, lang, lik, like, makt, mange, med, meg, meget,' .
      ' men, mens, mer, mest, min, mye, mÅte, navn, nei, nÅr, ogsÅ, opp, oss,' .
      ' over, part, punkt, rett, riktig, samme, sant, siden, sist, skulle, slik,' .
      ' slutt, som, start, stille  sÅ, tid, til, tilbake, tilstand, under, uten,' .
      ' var, ved, verdi, vil, ville, vite, vÅr, vÖre, vÖrt';
      
  $list = explode(', ', $sp);
  return array('nb' => $list);
}