<?php

// Spanish Stopwords
function autordf_stopwords_es() {
  $sp = 'una, unas, unos, uno, sobre, todo, también, tras, otro, algún,' .
      ' alguno, alguna, algunos, algunas, ser, soy, eres, somos, sois, estoy,' .
      ' esta, estamos, estais, estan, como, para, atras, porque, porué, estado,' .
      ' estaba, ante, antes, siendo, ambos, pero, por, poder, puede, puedo,' .
      ' podemos, podeis, pueden, fui, fue, fuimos, fueron, hacer, hago, hace,' .
      ' hacemos, haceis, hacen, cada, fin, incluso, primero, desde, conseguir,' .
      ' consigo, consigue, consigues, conseguimos, consiguen, voy, vamos, vais,' .
      ' van, vaya, gueno, tener, tengo, tiene, tenemos, teneis, tienen, las,' .
      ' los, aqui, mio, tuyo, ellos, ellas, nos, nosotros, vosotros, vosotras,' .
      ' dentro, solo, solamente, saber, sabes, sabe, sabemos, sabeis, saben,' .
      ' ultimo, largo, bastante, haces, muchos, aquellos, aquellas, sus,' .
      ' entonces, tiempo, verdad, verdadero, verdadera, cierto, ciertos, cierta,' .
      ' ciertas, intentar, intento, intenta, intentas, intentamos, intentais,' .
      ' intentan, dos, bajo, arriba, encima, usar, uso, usas, usa, usamos,' .
      ' usais, usan, emplear, empleo, empleas, emplean, ampleamos, empleais,' .
      ' valor, muy, era, eras, eramos, eran, modo, bien, cual, cuando, donde,' .
      ' mientras, quien, con, entre, sin, trabajo, trabajar, trabajas, trabaja,' .
      ' trabajamos, trabajais, trabajan, podria, podrias, podriamos, podrian,' .
      ' podriais, aquel';
  $list = explode(', ', $sp);
  return array('es' => $list);
}