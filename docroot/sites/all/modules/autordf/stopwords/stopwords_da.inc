<?php

// Danish Stopwords
function autordf_stopwords_da() {
  $sp = 'alle, andet, andre, begge, den, denne, der, deres, det, dette, dig,' .
      ' din, dog, eller, end, ene, eneste, enhver, fem, fire, flere, fleste,' .
      ' for, fordi, forrige   fra, før, god, han, hans, har, hendes, her, hun,' .
      ' hvad, hvem, hver, hvilken, hvis, hvor, hvordan, hvorfor, hvornår, ikke,' .
      ' ind, ingen, intet, jeg, jeres, kan, kom, kommer, lav, lidt, lille, man,' .
      ' mand, mange, med, meget, men, mens, mere, mig, ned, nogen, noget, nyt,' .
      ' nær, næste, næsten, otte, over, seks, ses, som, stor, store, syv, til,' .
      ' tre, var';
  $list = explode(', ', $sp);
  return array('da' => $list);
}