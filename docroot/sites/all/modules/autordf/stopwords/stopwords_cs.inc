<?php

// Czech Stopwords
function autordf_stopwords_cs() {
  $sp = 'dnes, timto, budes, budem, byli, jses, muj, svym, tomto, tohle,' .
      ' tuto, tyto, jej, zda, proc, mate, tato, kam, tohoto, kdo, kteri, nam,' .
      ' tom, tomuto, mit, nic, proto, kterou, byla, toho, protoze, asi, nasi,' .
      ' napiste, coz, tim, takze, svych, jeji, svymi, jste, tedy, teto  bylo,' .
      ' kde, prave, nad, nejsou, pod, tema, mezi, pres, pak, vam, ani, kdyz,' .
      ' vsak, jsem, tento, clanku, clanky, aby, jsme, pred, pta, jejich, byl,' .
      ' jeste, bez, take, pouze, prvni, vase, ktera, nas, novy, tipy, pokud,' .
      ' muze, design, strana, jeho, sve, jine, zpravy, nove, neni  vas, jen,' .
      ' podle, zde, clanek, email, byt, vice, bude, jiz, nez, ktery, ktere,' .
      ' nebo, ten, tak, pri, jsou, jak, dalsi, ale, jako, zpet, pro';
  
  $list = explode(', ', $sp);
  return array('cs' => $list);
}