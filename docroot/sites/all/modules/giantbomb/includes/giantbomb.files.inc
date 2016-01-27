<?php

/**
 * @file
 * File management.
 */

function giantbomb_file_exists($url) {
  $result = db_select('giantbomb_files', 'gbf')
    ->fields('gbf', array('fid'))
    ->condition('url', $url)
    ->execute();

  if ($record = $result->fetchObject()) {
    if ($file = file_load($record->fid)) {
      return $file;
    }
  }

  return FALSE;
}

function giantbomb_file_download($url) {
  
}