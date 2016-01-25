<?php

// Hungarian Stopwords
function autordf_stopwords_hu() {
  $sp = 'egy, fel, meg, ide, oda, szét, össze, vissza, hát, vagy, hogy, van,' .
      ' lesz, volt, csak, nem, igen, mint';
  
  $list = explode(', ', $sp);
  return array('hu' => $list);
}