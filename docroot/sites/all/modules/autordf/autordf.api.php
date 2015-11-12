<?php

/**
 * @file
 * Hooks provided by the Autordf module.
 */

/**
 * 
 * Define stopwords for different languages
 * 
 * @param $op
 *  - list
 *  - process
 * 
 */ 
function hook_autordf_stopwords($op) {
  switch ($op) {
    case 'list':
      return array('en', 'es');
      
    case 'prepare':
      $return['en'] = array('stop', 'words', 'list');
      $return['es'] = array('gueno', 'tener');
     return $return;
  }
}

