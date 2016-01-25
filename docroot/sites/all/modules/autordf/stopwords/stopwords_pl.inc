<?php

// Polish Stopwords
function autordf_stopwords_pl() {
  $sp = 'aby, ach, acz, aczkolwiek, albo, ale, ależ, bardziej, bardzo, bez,' .
      ' bowiem, byli, bynajmniej, być, był, była, było, były, będzie, będą,' .
      ' cali, cała, cały, cię, ciebie, cokolwiek, coś, czasami, czasem, czemu,' .
      ' czy, czyli, daleko, dla, dlaczego, dlatego, dobrze, dokąd, dość, dużo,' .
      ' dwa, dwaj, dwie, dwoje, dziś, dzisiaj, gdy, gdyby, gdyż, gdzie,' .
      ' gdziekolwiek, gdzieś, ich, ile, inna, inne, inny, innych, jak, jakaś,' .
      ' jakby, jaki, jakichś, jakie, jakiś, jakiż, jakkolwiek, jako, jakoś,' .
      ' jeden, jedna, jedno, jednak, jednakże, jego, jej, jemu, jest, jestem,' .
      ' jeszcze, jeśli, jeżeli, już, każdy, kiedy, kilka, kimś, kto, ktokolwiek,' .
      ' ktoś, która, które, którego, której, który, których, którym, którzy, lat,' .
      ' lecz, lub, mają, mam, mimo, między, mną, mnie, mogą, moi, moim, moja,' .
      ' moje, może, możliwe, można, mój, musi, nad, nam, nami, nas, nasi, nasz,' .
      ' nasza, nasze, naszego, naszych, natomiast, natychmiast, nawet, nią, nic,' .
      ' nich, nie, niego, niej, niemu, nigdy, nim, nimi, niż, obok, około, ona,' .
      ' one, oni, ono, oraz, owszem, pan, pana, pani, pod, podczas, pomimo, ponad,' .
      ' ponieważ, powinien, powinna, powinni, powinno, poza, prawie, przecież,' .
      ' przed, przede, przedtem, przez, przy, roku, również, sam, sama, się,' .
      ' skąd, sobie, sobą, sposób, swoje, tak, taka, taki, takie, także, tam,' .
      ' tego, tej, ten, teraz, też, totobą, tobie, toteż, trzeba, tutaj, twoi,' .
      ' twoim, twoja, twoje, twym, twój, tych, tylko, tym, wam, wami, was, wasz,' .
      ' wasza, wasze, według, wiele, wielu, więc, więcej, wszyscy, wszystkich,' .
      ' wszystkie, wszystkim, wszystko, wtedy, właśnie, zapewne, zawsze, zeznowu,' .
      ' znów, został, żaden, żadna, żadne, żadnych, żeby';
  
  $list = explode(', ', $sp);
  return array('pl' => $list);
}