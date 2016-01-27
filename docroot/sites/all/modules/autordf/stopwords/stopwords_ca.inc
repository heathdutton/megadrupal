<?php

// Catalan Stopwords
function autordf_stopwords_ca() {
  $sp = 'una, unes, uns, tot, també, altre, algun, alguna, alguns, algunes,' .
      ' ser, soc, ets, som, estic, està, estem, esteu, estan, com, per, perquè,' .
      ' perue, estat, estava, ans, abans, éssent, ambdós, però, per, poder,' .
      ' potser, puc, podem, podeu, poden, vaig, van, fer   faig, fem, feu, fan,' .
      ' cada, inclòs, primer, dese, conseguir, consegueixo, consigueix,' .
      ' consigueixes, conseguim, consigueixen, anar, haver, tenir, tinc, tenim,' .
      ' teniu, tene, les, els, seu, aquí, meu, teu, ells, elles, ens,' .
      ' nosaltres, vosaltres, dins, sols, solament, saber, saps, sap, sabem,' .
      ' sabeu, saben, últim, llarg, bastant, fas, molts, aquells, aquelles,' .
      ' seus, llavors, sota, dalt, molt, era, eres, erem, eren, mode, quant,' .
      ' quan, mentre, qui, amb, entre, sense, aquell';
  
  $list = explode(', ', $sp);
  return array('ca' => $list);
}