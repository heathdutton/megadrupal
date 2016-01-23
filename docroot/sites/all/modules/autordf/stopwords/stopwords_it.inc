<?php

// Italian Stopwords
function autordf_stopwords_it(){
  $sp = 'adesso, alla, allo, allora, altre, altri, altro, anche, ancora,' .
      ' avere, aveva, avevano, ben, buono, che, chi, cinque, comprare, con,' .
      ' consecutivi, consecutivo, cosa, cui, del, della, dello, dentro, deve,' .
      ' devo, doppio, due, ecco, fare, fine, fino, fra, gente, giu, hai, hanno,' .
      ' indietro, invece, lavoro, lei, loro, lui, lungo, meglio, molta, molti,' .
      ' molto, nei, nella, noi, nome, nostro, nove, nuovi, nuovo, oltre, ora,' .
      ' otto, peggio, pero, persone, piu, poco, primo, promesso, qua, quarto,' .
      ' quasi, quattro, quello, questo, qui, quindi, quinto, rispetto, sara,' .
      ' secondo, sei, sembra, sembrava, senza, sette, sia, siamo, siete, solo,' .
      ' sono, sopra, soprattutto, sotto, stati, stato, stesso, subito, sul,' .
      ' sulla, tanto, tempo, terzo, tra, tre, triplo, ultimo, una, uno, vai,' .
      ' voi, volte, vostro';
      
  $list = explode(', ', $sp);
  return array('it' => $list);
}