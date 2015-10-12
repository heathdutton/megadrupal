<?php
/**
 * Implements hook_drush_help().
 */
function epub_drush_help($command) {
  switch ($command) {
    case 'drush:epub-flush':
      return dt('Set the mime of uploaded epub files from "application/octet-stream" to "application/epub+zip" and unzip the epub files on the server.');
  }
}

/**
 * Implements hook_drush_command().
 */
function epub_drush_command() {
  $items = array();
  $items['epub-flush'] = array(
    'description' => dt('Unzip all the epub files.'),
    'arguments'   => array(
      'fid1'    => dt('start with this fid'),
      'fid2'    => dt('stop with this fid'),
    ),
    'examples' => array(
      'Flush all files' => 'drush epub-flush',
      'Flush file with fid from 5' => 'drush epub-flush 5',
      'Flush file with fid between 5-10' => 'drush epub-flush 5 10',
    ),
    //'aliases' => array('myc'),
  );
  return $items;
}

function drush_epub_flush($fid1 = NULL, $fid2 = NULL) {
  $query = db_select('file_managed', 'fid')->fields('fid', array('fid'));
  if (is_numeric($fid1)) {
    $query->condition('fid', $fid1, '>=');
    if (is_numeric($fid2)) {
      if ($fid2 > $fid1) {
        $query->condition('fid', $fid2, '<=');
      }
      else {
        drush_set_error('Make sure fid1 < fid2.');
        return False;
      }
    }
  }
  $enabled_types = file_type_get_enabled_types();
  foreach ($query->execute()->fetchCol() as $fid){
    $file = file_load($fid);
    if (epub_reset_mimetype($file)) {
      drush_log(dt('File @fid: mimetype was reset.', array('@fid' => $file->fid)), 'ok');
    }
    /*
    if (epub_file_update($file)) {
      drush_log(dt('File @fid was unzipped.', array('@fid' => $file->fid)), 'ok');
    }
    else {
      drush_log(dt('File @fid can\'t be unzipped.', array('@fid' => $file->fid)), 'warning');
    }
    */
  }
}
