<?php

/**
 * Save hotspot data in database.
 * @param int $fid File id
 * @param string $language language.
 * @param string $data Information about hotspots (JSON)
 */
function image_hotspots_db_save($fid, $language, $data) {
  if (empty($data)) {
    return;
  }
  $fields = array('fid' => $fid, 'language' => $language, 'coordinates' => $data);
  drupal_write_record('image_hotspot', $fields);
}

/**
 * Update hotspot data in database.
 * @param int $fid File id
 * @param string $language language.
 * @param string $data Information about hotspots (JSON)
 */
function image_hotspots_db_update($fid, $language, $data) {
  if (empty($data)) {
    return;
  }
  $fields = array('fid' => $fid, 'language' => $language, 'coordinates' => $data);
  drupal_write_record('image_hotspot', $fields, array('fid', 'language'));
}

/**
 * Delete hotspot data from database.
 * @param int $fid File id
 * @param string $language language.
 */
function image_hotspots_db_delete($fid, $language) {
  db_delete('image_hotspot')
    ->condition('fid', $fid)
    ->condition('language', $language)
    ->execute();
}

/**
 * Get hotspot data from database.
 * @param array $fids Array of files id
 * @param string $language language.
 */
function image_hotspots_db_get_coordinates($fids, $language) {
  $query = db_select('image_hotspot', 'image_hotspot');
  $query->fields('image_hotspot', array('coordinates', 'fid'));
  $or = db_or();

  foreach ($fids as $fid) {
    $or->condition('image_hotspot.fid', $fid);
  }

  $query->condition($or);
  $query->condition('image_hotspot.language', $language);
  $data = $query->execute()->fetchAll();

  return $data;
}
