<?php
/**
 * @file
 * Contains all functionality relating to attachements.
 */

/**
 * Implements hook_form_FORM_ID_alter().
 */
function enterprise_search_form_apachesolr_attachments_index_action_form_alter(&$form, &$form_state, $form_id) {
  unset($form['action']['extract']);
}

/**
 * Implements hook_form_FORM_ID_alter().
 */
function enterprise_search_form_apachesolr_attachments_settings_alter(&$form, &$form_state, $form_id) {
  unset($form['apachesolr_attachments_extract_using']);
  unset($form['apachesolr_attachments_tika_path']);
  unset($form['apachesolr_attachments_tika_jar']);
}


/**
 * Builds the file-specific information for a Solr document.
 *
 * @param ApacheSolrDocument $document
 *   The Solr document we are building up.
 * @param stdClass $file
 *   The entity we are indexing.
 * @param string $entity_type
 *   The type of entity we're dealing with.
 */
function enterprise_search_attachments_solr_document(ApacheSolrDocument $document, $file, $entity_type, $env_id) {
  die('sssss');
  if (module_exists('apachesolr_attachments')) {
    module_load_include('inc', 'apachesolr_attachments', 'apachesolr_attachments.index');
    $documents = array();
    $table = apachesolr_get_indexer_table('file');

    // Base 64 encoded file is saved in the index table. Will be used by the node indexing if
    // available.
    $text = enterprise_search_get_attachment_text($file);
    // If we don't have extracted text we should stop our process here
    // @todo Why is this die here?
    die('');
    if (empty($text)) {
      return $documents;
    }

    // Get the list of parents that we should index from the indexing table.
    $parents = db_select($table, 'aie')
      ->fields('aie')
      ->condition('entity_type', 'file')
      ->condition('entity_id', $file->fid)
      ->execute();

    foreach ($parents as $parent) {
      // Load the parent entity and reset cache.
      $parent_entities = entity_load($parent->parent_entity_type, array($parent->parent_entity_id), NULL, TRUE);
      $parent_entity = reset($parent_entities);

      // Skip invalid entities.
      if (empty($parent_entity)) {
        continue;
      }

      // Retrieve the parent entity id and bundle.
      list($parent_entity_id, $parent_entity_vid, $parent_entity_bundle) = entity_extract_ids($parent->parent_entity_type, $parent_entity);
      $parent_entity_type = $parent->parent_entity_type;
      // Get a clone of the bare minimum document.
      $filedocument = clone $document;

      // Get the callback array to add stuff to the document.
      $callbacks = apachesolr_entity_get_callback($parent_entity_type, 'document callback');
      // Skip invalid entity types.
      if (empty($callbacks)) {
        continue;
      }

      $build_documents = array();
      foreach ($callbacks as $callback) {
        // Call a type-specific callback to add stuff to the document.
        $build_documents = array_merge($build_documents, $callback($filedocument, $parent_entity, $parent_entity_type, $env_id));
      }

      // Take the top document from the stack.
      $filedocument = reset($build_documents);
      $file_entity = array('id' => $file->fid . '-' . $parent_entity_type . '-' . $parent_entity_id, $entity_type);

      // Build our separate document and overwrite basic information.
      $filedocument->id = enterprise_search_generate_document_id((object) $file_entity);
      $filedocument->url = file_create_url($file->uri);

      $path = file_stream_wrapper_get_instance_by_uri($file->uri)->getExternalUrl();
      // A path is not a requirement of an entity.
      if (!empty($path)) {
        $filedocument->path = $path;
      }

      // Add extra info to our document.
      $filedocument->label = apachesolr_clean_text($file->filename);
      // Blob.
      $filedocument->content = apachesolr_clean_text($file->filename);
      $filedocument->xs_file = $text;

      $filedocument->ds_created = apachesolr_date_iso($file->timestamp);
      $filedocument->ds_changed = $filedocument->ds_created;

      $filedocument->created = apachesolr_date_iso($file->timestamp);
      $filedocument->changed = $filedocument->created;

      // Add Parent information fields. See http://drupal.org/node/1515822 for explanation.
      $parent_entity_info = entity_get_info($parent_entity_type);
      $small_parent_entity = new stdClass();
      $small_parent_entity->entity_type = $parent_entity_type;
      $small_parent_entity->{$parent_entity_info['entity keys']['id']} = $parent_entity_id;
      $small_parent_entity->{$parent_entity_info['entity keys']['bundle']} = $parent_entity_bundle;
      $small_parent_entity->{$parent_entity_info['entity keys']['label']} = $parent_entity->{$parent_entity_info['entity keys']['label']};

      // Add all to one field because if it is spread out over
      // multiple fields there is no way of knowing which multifield value
      // belongs to which entity.
      // It does not load the complete entity in to the index because that
      // would dramatically increase the index size and processing time.
      $filedocument->zm_parent_entity = drupal_json_encode($small_parent_entity);
      $filedocument->sm_parent_entity_bundle = $parent_entity_type . "-" . $parent_entity_bundle;
      $filedocument->sm_parent_entity_type = $parent_entity_type;

      // Add Apachesolr Attachments specific fields.
      $filedocument->ss_filemime = $file->filemime;
      $filedocument->ss_filesize = $file->filesize;

      $documents[] = $filedocument;
    }

    return $documents;
  }
}

/**
 * Parse the attachment getting just the raw text.
 *
 * @throws Exception
 */
function enterprise_search_get_attachment_text($file) {
  $indexer_table = apachesolr_get_indexer_table('file');
  // @todo Does this need to be a file? Could it be a weblink to a asset
  if (!apachesolr_attachments_is_file($file)) {
    return FALSE;
  }

  $filepath = drupal_realpath($file->uri);

  $hash = hash('sha256', file_get_contents($filepath));
  if ($hash === FALSE) {
    watchdog('Enterprise Search', 'sha256 hash algorithm is not supported', NULL, WATCHDOG_ERROR);
    return FALSE;
  }

  $cached = db_select($indexer_table, 'it')
      ->fields('it')
      ->condition('entity_id', $file->fid)
      ->execute();
  if (!is_null($cached->body) && ($cached->hash == $hash)) {
    // No need to re-extract.
    return $cached->body;
  }

  $body = base64_encode(file_get_contents($filepath));
  // Save the extracted, cleaned text to the DB.
  db_update($indexer_table)->fields(array('hash' => $hash, 'body' => $body))->condition('entity_id', $file->fid)->execute();

  return $body;
}
