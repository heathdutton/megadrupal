<?php
function apachesolr_media_index_files($env_id, $limit) {
  $files = apachesolr_media_get_files_to_index($env_id, $limit);
  return apachesolr_media_send_to_solr($env_id, $files); 
}

function apachesolr_media_get_files_to_index($env_id, $limit) {
  $last_index_timestamp = apachesolr_media_get_last_index_position($env_id, 'file');
  $table = 'file_managed';
  $query = db_select($table, 'aie')
    ->fields('aie', array('fid', 'filemime', 'timestamp'))
    ->condition('aie.status', 1)
    ->condition('aie.timestamp', $last_index_timestamp, '>')
    ->orderBy('aie.timestamp', 'ASC');
  $query->range(0, $limit);
  $records = $query->execute();
  if ($records->rowCount() == 0) {
    return FALSE;
  }
  else {
    foreach ($records as $record) {
      $rows[] = file_load($record->fid);
    }
    return $rows;
  }
}


function apachesolr_media_send_to_solr($env_id, $files) {
  if ($files == FALSE) {
    return FALSE;
  }

  try {
    // Get the $solr object
    $solr = apachesolr_get_solr($env_id);
    // If there is no server available, don't continue.
    if (!$solr->ping(variable_get('apachesolr_ping_timeout', 4))) {
      throw new Exception(t('No Solr instance available during indexing.'));
    }
  }
  catch (Exception $e) {
    watchdog('Apache Solr', nl2br(check_plain($e->getMessage())), NULL, WATCHDOG_ERROR);
    return FALSE;
  }

  // Send the file to Solr.
  watchdog('Apache Solr', 'Adding @count files.', array('@count' => count($files)));
  try {
    watchdog('Apache Solr', 'Indexing succeeded on @count files', array(
      '@count' => count($files),
    ), WATCHDOG_INFO);
    global $base_url;
    foreach ($files as $file) {
      $params = array(
        'literal,is_uid' => $file->uid,
        'literal.entity_type' => 'file',
        'literal.label' => $file->filename,
        'literal.entity_id' => $file->fid,
        'literal.bundle' => file_get_type($file),
        'literal.bundle_name' => file_type_get_name($file),
        'literal.hash' => apachesolr_site_hash(),
        'literal.id' => apachesolr_site_hash() . 'file' . $file->fid, 
        'literal.ds_created' => apachesolr_date_iso($file->timestamp),
        'literal.is_uid' => $file->uid,
        'literal.path' => 'file/' . $file->fid,
        'literal.site' => '$base_url',
      );
      $solr->addFile(drupal_realpath($file->uri), $params);
    }
    return count($files);
  }
  catch (Exception $e) {
    /*
    if (!empty($docs)) {
      foreach ($docs as $doc) {
        $eids[] = $doc->entity_type . '/' . $doc->entity_id;
      }
    }
    watchdog('Apache Solr', 'Indexing failed on one of the following entity ids: @eids <br /> !message', array(
      '@eids' => implode(', ', $eids),
      '!message' => nl2br(strip_tags($e->getMessage())),
    ), WATCHDOG_ERROR);
    */
    return FALSE;
  }
}

function apachesolr_media_get_last_index_position($env_id, $entity_type){
  $env = apachesolr_environment_load($env_id);
  $url = $env['url'] . '/select/?q.alt=(*:*)AND(entity_type:' . $entity_type . ')&fl=ds_created&sort=ds_created%20desc&wt=phps&start=0&rows=1';
  $request = drupal_http_request($url);
  $data = unserialize($request->data);
  if (empty($data['response']['docs'][0]['ds_created'])) {
    return FALSE;
  }
  else {
    return strtotime($data['response']['docs'][0]['ds_created']);
  }
}

