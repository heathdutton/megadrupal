<?php

/**
 * Train data from predefined files.
 * If files not available return
 */
class AutordfTraining {
  
  public static function randomize($words, $samplesize) {
    $totalwords = count($words);
    $range = range(0, $totalwords - 1);
    shuffle($range); 
    
    if ($samplesize > $totalwords) {
      $samplesize = $totalwords;
    }
    
    $range = array_slice($range, 0, $samplesize);
    
    $rand = array();
    for ($i = 0; $i < $samplesize; $i++) {
      // Build a array hash for searching
      $word = $words[$range[$i]];
      $rand[$word->entity] = $word->entityType;
    }
    
    return $rand;
  }
  
  public static function getEntities($path, $label, $numwordstoread = 100000) {
    $fh = fopen($path, 'r');
    $nooflines = 0;
    $words = array();
    while($line = fgets($fh)) {
      if ($numwordstoread < $nooflines) {
        break;
      }
      if (strrpos($line, ':') === FALSE && strrpos($line, '(') === FALSE) {
        $words[] = new AutordfNEword($line, $label);
      }
      $nooflines++;
    }
    return $words;
  }
  
  public static function TrainLocalTagger($size = 500000) {
    $path = drupal_get_path('module', 'autordf');
    $organization = self::getEntities($path.'/data/organizations.lst', 'Company');
    $location = self::getEntities($path.'/data/locations.lst', 'City');
    $entities = array_merge($organization, $location);
    $entities = self::randomize($entities, $size);
    return $entities;
  }
}
