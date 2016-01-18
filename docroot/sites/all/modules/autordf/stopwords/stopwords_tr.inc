<?php

// Turkish Stopwords
function autordf_stopwords_tr() {
  $sp = 'acaba, altmýþ, altý, ama, bana, bazý, belki, ben, benden, beni,' .
      ' benim, beþ, bin, bir, biri, birkaç, birkez, birþey, birþeyi, biz,' .
      ' bizden, bizi, bizim, buna, bunda, bundan, bunu, bunun, daha, dahi,' .
      ' defa, diye, doksan, dokuz, dört, elli, en  gibi, hem, hep, hepsi, her,' .
      ' hiç, iki, ile, INSERmi, ise, için, katrilyon, kez, kim, kimden, kime,' .
      ' kimi, kýrk, milyar, milyon, nasýl, neden, nerde, nerede, nereye, niye,' .
      ' niçin, ona, ondan, onlar, onlardan, onlari, onlarýn, onu   otuz, sanki,' .
      ' sekiz, seksen, sen, senden, seni, senin, siz, sizden, sizi, sizin,' .
      ' trilyon, tüm, veya, yani, yedi, yetmiþ, yirmi, yüz, çok, çünkü, þey,' .
      ' þeyden, þeyi, þeyler, þuna, þunda, þundan, þunu';
      
  $list = explode(', ', $sp);
  return array('tr' => $list);
}