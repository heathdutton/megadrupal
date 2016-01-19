<?php

function insert_file($item) {
  // exclude folders
  if ($item['mimeType'] == 'application/vnd.google-apps.folder' || $item['labels']['trashed'] == 1) {
    return false;
  }
  $dbitem = array();
  $dbitem['gfid'] = $item['id'];
  $dbitem['title'] = $item['title'];
  $dbitem['url'] = $item['alternateLink'];
  if (!gdrive_file_exist($item['id'])) {
    db_insert('gdrive_files')->fields($dbitem)->execute();
    return true;
  }
  return false;
}

function gdrive_files_list() {
  return db_select('gdrive_files', 'g')->fields('g', array('fid', 'title'))->execute()->fetchAllKeyed();
}

function gdrive_file_exist($id) {
  $query = db_select('gdrive_files');
  $query->addExpression('COUNT(*)');
  $query->condition('gfid', $id);
  $count = $query->execute()->fetchField();
  return $count;
}

function gdrive_get_file_info($fid) {
  return db_select('gdrive_files', 'g')->fields('g', array('fid', 'title', 'url'))->condition('fid', $fid)->execute()->fetchAllAssoc('fid');
}