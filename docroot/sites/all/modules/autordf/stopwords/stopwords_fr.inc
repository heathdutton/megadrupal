<?php

// French Stopwords
function autordf_stopwords_fr() {
  $sp = 'alors, aucuns, aussi, autre, avant, avec, avoir, bon, car, cela, ces,' .
      ' ceux, chaque, comme, comment, dans, des, dedans, dehors, depuis, deux,' .
      ' devrait, doit, donc, dos, droite, début, elle, elles, encore, essai,' .
      ' est, fait, faites, fois, font, force, haut, hors, ici, ils, je  juste,' .
      ' les, leur, maintenant, mais, mes, mine, moins, mon, mot, même, nommés,' .
      ' notre, nous, nouveaux, par, parce, parole, pas, personnes, peut, peu,' .
      ' pièce, plupart, pour, pourquoi, quand, que, quel, quelle, quelles,' .
      ' quels, qui, sans, ses, seulement, sien, son, sont, sous, soyez, sujet,' .
      ' sur, tandis, tellement, tels, tes, ton, tous, tout, trop, très, valeur,' .
      ' voie, voient, vont, votre, vous, étaient, état, étions, été, être';
  
  $list = explode(', ', $sp);
  return array('fr' => $list);
}