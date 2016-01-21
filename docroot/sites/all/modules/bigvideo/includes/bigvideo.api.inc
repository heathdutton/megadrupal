<?php
/**
 * @file
 * Module public functions.
 */

/*
 * Source.
 */

/**
 * Get source object by Source ID.
 *
 * @param int $bsid
 *   Bigvideo Source ID.
 *
 * @return stdClass|null
 *   Source object or NULL.
 */
function bigvideo_source_load($bsid) {
  $result = db_select('bigvideo_source', 'bs')
    ->condition('bs.bsid', $bsid, '=')
    ->fields('bs')
    ->execute()
    ->fetch();
  return $result;
}

/**
 * Get all bigvideo sources.
 *
 * @return array|null
 *   List of source objects or NULL.
 */
function bigvideo_source_load_all() {
  $result = db_select('bigvideo_source', 'bs')
    ->fields('bs')
    ->execute()
    ->fetchAll();
  return $result;
}

/**
 * Create source by object data.
 *
 * @param object $source
 *   Source object for save to database.
 *   Parameters:
 *   $source->name: Administrative name.
 *   $source->type: Type of source (File or Link).
 *   You can use defined constants.
 *   $source->webm: Filed ID or Link for WebM format.
 *   $source->mp4: File ID or Link for MP4 format.
 *
 * @return int|null
 *   Source ID for created entry.
 *
 * @throws \Exception
 *   Database exception if source can't be saved.
 */
function bigvideo_source_create($source) {
  $fields = array();
  $field_names = array('name', 'type', 'webm', 'mp4');
  foreach ($field_names as $field_name) {
    $fields[$field_name] = $source->{$field_name};
  }
  $bsid = db_insert('bigvideo_source')
    ->fields($fields)
    ->execute();
  return $bsid;
}

/**
 * Update source data.
 *
 * @param object $source
 *   Source object for update in database.
 *   Parameters:
 *   $source->bsid: Bigvideo source ID.
 *   $source->name: Administrative name.
 *   $source->type: Type of source (File or Link).
 *   You can use defined constants.
 *   $source->webm: Filed ID or Link for WebM format.
 *   $source->mp4: File ID or Link for MP4 format.
 *
 * @return int|null
 *   Source ID for created entry.
 *
 * @throws \Exception
 *   Database exception if source can't be saved.
 */
function bigvideo_source_update($source) {
  $fields = array();
  $field_names = array('name', 'type', 'webm', 'mp4');
  foreach ($field_names as $field_name) {
    $fields[$field_name] = $source->{$field_name};
  }
  $bsid = db_update('bigvideo_source')
    ->condition('bsid', $source->bsid)
    ->fields($fields)
    ->execute();
  return $bsid;
}

/**
 * Delete source from database.
 *
 * @param int $bsid
 *   Bigvideo Source ID for delete.
 */
function bigvideo_source_delete($bsid) {
  db_delete('bigvideo_source')
    ->condition('bsid', $bsid)
    ->execute();
}

/*
 * Page.
 */

/**
 * Get page object by Page ID.
 *
 * @param int $bpid
 *   Bigvideo Page ID.
 *
 * @return object|null
 *   Page object or NULL.
 */
function bigvideo_page_load($bpid) {
  $result = db_select('bigvideo_page', 'bp')
    ->condition('bp.bpid', $bpid, '=')
    ->fields('bp')
    ->execute()
    ->fetch();
  return $result;
}

/**
 * Get all BigVideo pages.
 *
 * @param bool $status
 *   Page status.
 *
 * @return object|null
 *   List of page objects or NULL.
 */
function bigvideo_page_load_all($status = NULL) {
  $query = db_select('bigvideo_page', 'bp')
    ->fields('bp');
  if (isset($status)) {
    $query->condition('status', $status);
  }
  $result = $query->execute()
    ->fetchAll();
  return $result;
}

/**
 * Create page by object data.
 *
 * @param object $page
 *   Source object for save to database.
 *   Parameters:
 *   $page->name: Administrative name.
 *   $page->source: Bigvideo Source ID used on this page.
 *   $page->path: String contains drupal paths for attaching scripts.
 *   $page->status: Status this page.
 *   You can use defined constants.
 *
 * @return int|null
 *   Page ID for created entry.
 *
 * @throws \Exception
 *   Database exception if page can't be saved.
 */
function bigvideo_page_create($page) {
  $fields = array();
  $field_names = array('name', 'source', 'path', 'status');
  foreach ($field_names as $field_name) {
    $fields[$field_name] = $page->{$field_name};
  }
  $bpid = db_insert('bigvideo_page')
    ->fields($fields)
    ->execute();
  return $bpid;
}

/**
 * Update page by object data.
 *
 * @param object $page
 *   Source object for save to database.
 *   Parameters:
 *   $page->bpid: Bigvideo Page ID for update.
 *   $page->name: Administrative name.
 *   $page->source: Bigvideo Source ID used on this page.
 *   $page->path: String contains drupal paths for attaching scripts.
 *   $page->status: Status this page.
 *   You can use defined constants.
 *
 * @return int|null
 *   Page ID for updated entry.
 *
 * @throws \Exception
 *   Database exception if page can't be saved.
 */
function bigvideo_page_update($page) {
  $fields = array();
  $field_names = array('name', 'source', 'path', 'status');
  foreach ($field_names as $field_name) {
    $fields[$field_name] = $page->{$field_name};
  }
  $bpid = db_update('bigvideo_page')
    ->condition('bpid', $page->bpid)
    ->fields($fields)
    ->execute();
  return $bpid;
}

/**
 * Delete page from database.
 *
 * @param int $bpid
 *   Bigvideo Page ID fro delete.
 */
function bigvideo_page_delete($bpid) {
  db_delete('bigvideo_page')
    ->condition('bpid', $bpid)
    ->execute();
}

/*
 * Common.
 */

/**
 * Create real links to video from Source object.
 *
 * @param object $source
 *   Bigvideo Source.
 *
 * @return array
 *   Video links.
 */
function bigvideo_create_links($source) {
  $links = array();
  if ($source->type == BIGVIDEO_SOURCE_TYPE_FILE) {
    if (intval($source->mp4) && $mp4_file = file_load($source->mp4)) {
      $links['mp4'] = file_create_url($mp4_file->uri);
    }
    if (intval($source->webm) &&  $webm_file = file_load($source->webm)) {
      $links['webm'] = file_create_url($webm_file->uri);
    }
  }
  else {
    $links['mp4'] = $source->mp4;
    $links['webm'] = $source->webm;
  }
  return $links;
}

/**
 * Attach JS scripts and settings to provided content.
 *
 * @param array $content
 *   Some content for attach scripts (page, form).
 *
 * @param array $links
 *   Links to video files.
 */
function bigvideo_attach_scripts(&$content, $links) {
  $content['bigvideo'] = array();
  $content['bigvideo']['#attached']['libraries_load'][] = array('video-js');
  $content['bigvideo']['#attached']['libraries_load'][] = array('imagesloaded');
  $content['bigvideo']['#attached']['libraries_load'][] = array('bigvideojs');
  $content['bigvideo']['#attached']['js'][] = array(
    'data' => array(
      'bigvideo' => array(
        'links' => $links,
      ),
    ),
    'type' => 'setting',
  );
  $content['bigvideo']['#attached']['js'][] = drupal_get_path('module', 'bigvideo') . '/includes/js/bigvideo.js';
}
