<?php

/**
 * Sends entities for translation.
 *
 * @param array $nids
 *   The array of entity node IDs.
 * @param string $pd4
 *   The project director details.
 * @param string $submission_name
 *   The name of the submission.
 * @param string $due_date
 *   When the translation is due.
 * @param string $project_code
 *   The project's registered code.
 * @param string $source_locale
 *   The locale of the content being translated.
 * @param array $target_locale_arr
 *   Array of desired locales to translate into.
 * @param array $submission_details
 *   Associative array of details about the submission.
 *
 * @return object
 *   GlobalLink object that represents active translation.
 */
function globallink_entity_send_for_translations($nids, $pd4, $submission_name, $due_date, $project_code, $source_locale, $target_locale_arr, $submission_details) {
  module_load_include('inc', 'globallink', 'gl_ws/gl_ws_send_translations');

  $node_check = variable_get('globallink_implementation_type', 0);
  $submitter = $submission_details['submitter'];
  $drupal_locale_code = globallink_get_drupal_locale_code($source_locale);
  $globallink_arr = array();

  foreach ($nids as $nid) {
    list($nid, $vid) = explode('-', $nid, 2);

    $rows = globallink_entity_get_sent_rows_by_nid($nid);

    $target_arr = $target_locale_arr;

    foreach ($rows as $row) {
      if (array_search($row->target, $target_arr)) {
        unset($target_arr[$row->target]);
      }
    }

    if (empty($target_arr)) {
      watchdog('GlobalLink', 'Target languages already sent. Skipping nid - %nid', array('%nid' => $nid), WATCHDOG_WARNING);

      continue;
    }

    $node = node_load($nid, $vid);

    if ($node_check == 1) {
      foreach ($target_arr as $key => $target_locale) {
        if (!globallink_translate_node_for_language($node, globallink_get_drupal_locale_code($target_locale))) {
          unset($target_arr[$key]);
        }
      }
    }

    if (empty($target_arr)) {
      watchdog('GlobalLink', 'No target languages. Skipping nid - %nid', array('%nid' => $nid), WATCHDOG_CRITICAL);

      continue;
    }

    $drupal_target_arr = array();

    foreach ($target_arr as $target_locale) {
      array_push($drupal_target_arr, globallink_get_drupal_locale_code($target_locale));
    }

    $tnid = NULL;
    $tvid = NULL;

    $name = '.xml';

    $xml = globallink_entity_get_xml($node, $drupal_target_arr, $tnid, $tvid, $name, FALSE, $drupal_locale_code);

    if (!$xml) {
      watchdog('GlobalLink', 'Cannot create XML. Skipping nid - %nid', array('%nid' => $nid), WATCHDOG_CRITICAL);

      continue;
    }

    watchdog('GlobalLink', 'XML - %xml', array('%xml' => $xml), WATCHDOG_INFO);

    $globallink = new GlobalLink();
    $globallink->nid = $node->nid;
    $globallink->vid = $node->vid;
    $globallink->title = $node->title;
    $globallink->type = $node->type;
    $globallink->metadata = 'entity';
    $globallink->sourceLocale = $source_locale;
    $globallink->targetLocale = $target_arr;
    $globallink->sourceXML = $xml;
    $globallink->sourceFileName = $name;
    $globallink->submissionName = $submission_name;
    $globallink->dueDate = $due_date;
    $globallink->submissionInstructions = $submission_details['instructions'] . "\nSubmitter: " . $submitter;
    $globallink_arr[] = $globallink;
  }

  if (!empty($globallink_arr)) {
    globallink_send_documents_for_translation_to_pd($globallink_arr, $pd4, $project_code, $submitter);
  }

  return $globallink_arr;
}

/**
 * Updates entity ticket ID.
 *
 * @param array $arr
 *   Array of GlobalLink objects.
 * @param string $project_code
 *   The entity's project code.
 */
function globallink_entity_update_ticket_id($arr, $project_code) {
  foreach ($arr as $globallink) {
    $nid = $globallink->nid;
    $node = node_load($nid);
    $target_locale_arr = $globallink->targetLocale;
    $source_locale_code = globallink_get_drupal_locale_code($globallink->sourceLocale);

    foreach ($target_locale_arr as $target_locale) {
      $row = globallink_entity_get_row_by_nid_and_locale($nid, $globallink->sourceLocale, $target_locale);
      $title = $node->title;

      if ($row) {
        db_update('globallink_core_entity')
        ->fields(array(
          'vid' => $globallink->vid,
          'title' => $title,
          'document_ticket' => $globallink->documentTicket,
          'submission' => $globallink->submissionName,
          'submission_ticket' => $globallink->submissionTicket,
          'status' => 'Sent for Translations',
          'timestamp' => REQUEST_TIME,
          'last_modified' => $node->changed,
          'changed' => 0,
          'project_code' => $project_code,
        ))
        ->condition('rid', $row->rid, '=')
        ->execute();
      }
      else {
        db_insert('globallink_core_entity')
        ->fields(array(
          'nid' => $globallink->nid,
          'vid' => $globallink->vid,
          'type' => $globallink->type,
          'title' => $title,
          'source' => $globallink->sourceLocale,
          'target' => $target_locale,
          'document_ticket' => $globallink->documentTicket,
          'submission' => $globallink->submissionName,
          'submission_ticket' => $globallink->submissionTicket,
          'status' => 'Sent for Translations',
          'timestamp' => REQUEST_TIME,
          'last_modified' => $node->changed,
          'changed' => 0,
          'project_code' => $project_code,
        ))
        ->execute();
      }
    }
  }
}

/**
 * Gets entity row by node ID and locale.
 *
 * @param string $nid
 *   The node ID.
 * @param string $source
 *   The source of the entity.
 * @param string $target
 *   The target of the entity.
 *
 * @return
 *   The entity row if all goes well.  FALSE if it fails.
 */
function globallink_entity_get_row_by_nid_and_locale($nid, $source, $target) {
  $result = db_select('globallink_core_entity', 'tc')
  ->fields('tc')
  ->condition('nid', $nid, '=')
  ->condition('source', $source, '=')
  ->condition('target', $target, '=')
  ->execute();

  foreach ($result as $row) {
    return $row;
  }

  return FALSE;
}

/**
 * Gets sent entity rows by node ID.
 *
 * @param string $nid
 *   The node ID.
 *
 * @return array
 *   Array of entity rows.
 */
function globallink_entity_get_sent_rows_by_nid($nid) {
  $result = db_select('globallink_core_entity', 'tc')
  ->fields('tc')
  ->condition('nid', $nid, '=')
  ->condition('status', array('Sent for Translations', 'Error'), 'IN')
  ->execute();

  $rows = array();

  foreach ($result as $row) {
    $rows[] = $row;
  }

  return $rows;
}

/**
 * Clears "Changed" status for entities.
 *
 * @param array $nids
 *   The array of entity node IDs.
 * @param string $source
 *   The source of the nodes.
 * @param array $tgts
 *   The targets of the nodes.
 */
function globallink_entity_update_change_flag($nids, $source, $tgts) {
  watchdog('GlobalLink', 'CHANGED Status Cleared for Node Ids - [%nids]', array('%nids' => implode(',', $nids)), WATCHDOG_INFO);

  $insert_arr = array();
  $tgt_arr = array_keys($tgts);

  foreach ($tgt_arr as $tgt) {
    foreach ($nids as $nid) {
      $insert_arr[$tgt][$nid] = TRUE;
    }
  }

  foreach ($tgt_arr as $tgt) {
    foreach ($nids as $nid) {
      $result = db_select('globallink_core_entity', 'tc')
      ->fields('tc')
      ->condition('nid', $nid, '=')
      ->condition('target', $tgt, '=')
      ->execute();

      foreach ($result as $item) {
        db_update('globallink_core_entity')
        ->fields(array(
          'timestamp' => REQUEST_TIME,
          'changed' => 2,
        ))
        ->condition('rid', $item->rid, '=')
        ->execute();

        $insert_arr[$tgt][$nid] = FALSE;
      }
    }
  }

  foreach ($tgt_arr as $tgt) {
    foreach ($nids as $nid) {
      if (isset($insert_arr[$tgt]) && isset($insert_arr[$tgt][$nid])) {
        if ($insert_arr[$tgt][$nid]) {
          $node = node_load($nid);

          db_insert('globallink_core_entity')
          ->fields(array(
            'nid' => $nid,
            'vid' => $node->vid,
            'type' => $node->type,
            'title' => $node->title,
            'source' => $source,
            'target' => $tgt,
            'document_ticket' => '',
            'submission' => '',
            'submission_ticket' => '',
            'status' => 'Pending Translations',
            'timestamp' => REQUEST_TIME,
            'last_modified' => $node->changed,
            'changed' => 2,
          ))
          ->execute();
        }
      }
    }
  }
}

/**
 * Cancels entity records.
 *
 * @param array $rowids
 *   Array of entity IDs.
 * @param string $pd4
 *   The project director details.
 */
function globallink_entity_cancel_select_records($rowids, $pd4) {
  $globallink_arr = array();

  foreach ($rowids as $rid) {
    $row = globallink_entity_get_row($rid);

    $globallink = new GlobalLink();
    $globallink->tptRowId = $row->rid;
    $globallink->targetLocale = $row->target;
    $globallink->documentTicket = $row->document_ticket;
    $globallink->submissionTicket = $row->submission_ticket;
    $globallink_arr[$rid] = $globallink;
  }

  globallink_cancel_select_documents($pd4, $globallink_arr);
  globallink_entity_update_row_document($globallink_arr);
}

/**
 * Cancels entity submission.
 *
 * @param string $selected_submission
 *   The name of the submission.
 */
function globallink_entity_cancel_submission($selected_submission) {
  $pd4 = globallink_get_project_director_details();

  $globallink = new GlobalLink();
  $submission_name = globallink_entity_get_submission_name($selected_submission);
  $globallink->submissionName = $submission_name;
  $globallink->submissionTicket = $selected_submission;

  globallink_cancel_pd_submission($pd4, $globallink);
  globallink_entity_update_row_submission($globallink);
}

/**
 * Gets entity submission name.
 *
 * @param string $submission_ticket
 *   The submission ticket.
 *
 * @return string
 *   The entity submission name.
 */
function globallink_entity_get_submission_name($submission_ticket) {
  $query = db_select('globallink_core_entity', 'tc');
  $query->fields('tc');
  $query->condition('submission_ticket', $submission_ticket, '=');

  $results = $query->execute();

  foreach ($results as $row) {
    if ($row->submission != '') {
      return $row->submission;
    }
  }
}

/**
 * Updates entity rows to reflect translation status.
 *
 * @param array $globallink_arr
 *   Array of GlobalLink objects.
 */
function globallink_entity_update_row_document(&$globallink_arr) {
  foreach ($globallink_arr as $globallink) {
    if ($globallink->cancelled) {
      db_update('globallink_core_entity')
      ->fields(array(
        'status' => 'Pending Translations',
        'timestamp' => REQUEST_TIME,
        'changed' => 1,
      ))
      ->condition('rid', $globallink->tptRowId, '=')
      ->execute();
    }
  }
}

/**
 * Updates entity submission with new ticket and name.
 *
 * @param object $globallink
 *   GlobalLink object.
 */
function globallink_entity_update_row_submission(&$globallink) {
  db_update('globallink_core_entity')
  ->fields(array(
    'status' => 'Cancelled',
    'timestamp' => REQUEST_TIME,
    'changed' => 1,
  ))
  ->condition('submission_ticket', $globallink->submissionTicket, '=')
  ->condition('submission', $globallink->submissionName, '=')
  ->execute();
}

/**
 * Gets distinct active entity submission names.
 *
 * @return array
 *   Array of distinct active entity submission names.
 */
function globallink_entity_get_distinct_active_submission_names() {
  $query = db_select('globallink_core_entity', 'tc');
  $query->condition('status', array('Sent for Translations', 'Error'), 'IN');
  $query->distinct();
  $query->fields('tc');

  $results = $query->execute();
  $arr = array('' => '-- Select a Submission --');

  foreach ($results as $row) {
    $arr[$row->submission_ticket] = $row->submission;
  }

  return $arr;
}

/**
 * Gets entity row ID from submission details.
 *
 * @param string $submission_ticket
 *   The submission ticket.
 * @param string $document_ticket
 *   The document ticket.
 * @param string $target_locale
 *   The target locale.
 *
 * @return string
 *   The entity row ID.  FALSE if the row doesn't exist.
 */
function globallink_entity_get_row_id_from_submission($submission_ticket, $document_ticket, $target_locale) {
  $query = db_select('globallink_core_entity', 'tc');
  $query->condition('submission_ticket', $submission_ticket, '=');
  $query->condition('document_ticket', $document_ticket, '=');
  $query->condition('target', $target_locale, '=');
  $query->fields('tc');

  $results = $query->execute();

  foreach ($results as $row) {
    // Return the first one
    return $row->rid;
  }
}

/**
 * Updates status for deleted entities.
 *
 * @param string $pd4
 *   The project director details.
 * @param object $globallink
 *   GlobalLink object.
 *
 * @return bool
 *   TRUE if update was successful, FALSE on error.
 */
function globallink_entity_update_deleted_records($pd4, $globallink) {
  try {
    $globallink->status = 'Source Deleted';

    globallink_send_download_confirmation($globallink->targetTicket, $pd4);
    globallink_entity_update_status($globallink);
  }
  catch (SoapFault $se) {
    watchdog('GlobalLink', 'SOAP Exception - %function - Code[%faultcode], Message[%faultstring]', array('%function' => __FUNCTION__, '%faultcode' => $se->faultcode, '%faultstring' => $se->faultstring), WATCHDOG_ERROR);
    form_set_error('', t('Web Services Error: @faultcode - @faultstring', array('@faultcode' => $se->faultcode, '@faultstring' => $se->faultstring)));
  }
  catch (Exception $e) {
    watchdog('GlobalLink', 'Exception - %function - File[%file], Line[%line], Code[%code], Message[%message]', array('%function' => __FUNCTION__, '%file' => $e->getFile(), '%line' => $e->getLine(), '%code' => $e->getCode(), '%message' => $e->getMessage()), WATCHDOG_ERROR);
    form_set_error('', t('Error: @message', array('@message' => $e->getMessage())));
  }

  return TRUE;
}

/**
 * Gets number of translated entities.
 *
 * @param string $pd4
 *   The project director details.
 * @param array $globallink_arr
 *   Array of GlobalLink objects.
 *
 * @return int
 *   The number of translated entities.
 */
function globallink_entity_get_translated_enties($pd4, &$globallink_arr) {
  try {
    $count = 0;

    foreach ($globallink_arr as $globallink) {
      if (!$globallink->sourceDeleted) {
        $globallink->targetXML = globallink_download_target_resource($pd4, $globallink->targetTicket);

        if (isset($globallink->targetXML)) {
          $count++;
          globallink_entity_update($globallink);

          if ($globallink->status != 'Error') {
            globallink_send_download_confirmation($globallink->targetTicket, $pd4);
            globallink_entity_update_status($globallink);
          }
          else {
            $count--;
          }
        }
      }
    }
  }
  catch (SoapFault $se) {
    watchdog('GlobalLink', 'SOAP Exception - %function - Code[%faultcode], Message[%faultstring]', array('%function' => __FUNCTION__, '%faultcode' => $se->faultcode, '%faultstring' => $se->faultstring), WATCHDOG_ERROR);
    form_set_error('', t('Web Services Error: @faultcode - @faultstring', array('@faultcode' => $se->faultcode, '@faultstring' => $se->faultstring)));
  }
  catch (Exception $e) {
    watchdog('GlobalLink', 'Exception - %function - File[%file], Line[%line], Code[%code], Message[%message]', array('%function' => __FUNCTION__, '%file' => $e->getFile(), '%line' => $e->getLine(), '%code' => $e->getCode(), '%message' => $e->getMessage()), WATCHDOG_ERROR);
    form_set_error('', t('Error: @message', array('@message' => $e->getMessage())));
  }

  return $count;
}

/**
 * Retrieves entity's row by ID.
 *
 * @param string $row_id
 *   The entity row ID.
 *
 * @return
 *   The row if entity exists.  FALSE if not.
 */
function globallink_entity_get_row($row_id) {
  $result = db_select('globallink_core_entity', 'tc')
  ->fields('tc')
  ->condition('rid', $row_id, '=')
  ->execute();

  foreach ($result as $row) {
    return $row;
  }
}

/**
 * Updates entity status.
 *
 * @param object $globallink
 *   GlobalLink object.
 */
function globallink_entity_update_status(&$globallink) {
  switch ($globallink->status) {
    case 'Source Deleted':
    case 'Error':
      $row = globallink_entity_get_row_by_nid_and_locale($globallink->nid, $globallink->sourceLocale, $globallink->targetLocale);
      $node = node_load($row->nid);

      if ($node->vid != $row->vid) {
        db_update('globallink_core_entity')
        ->fields(array(
          'vid' => $node->vid,
          'title' => $node->title,
          'status' => 'Pending Translations',
          'timestamp' => REQUEST_TIME,
        ))
        ->condition('rid', $row->rid, '=')
        ->execute();
      }
      else {
        db_update('globallink_core_entity')
        ->fields(array(
          'status' => 'Pending Translations',
          'timestamp' => REQUEST_TIME,
        ))
        ->condition('rid', $row->rid, '=')
        ->execute();
      }

      break;
    case 'Source Deleted':
      $row = globallink_entity_get_row_by_nid_and_locale($globallink->nid, $globallink->sourceLocale, $globallink->targetLocale);

      db_update('globallink_core_entity')
      ->fields(array(
        'status' => 'Source Deleted',
        'document_ticket' => '',
        'submission_ticket' => '',
        'timestamp' => REQUEST_TIME,
      ))
      ->condition('rid', $row->rid, '=')
      ->execute();

      break;
    case 'Error':
      $row = globallink_entity_get_row_by_nid_and_locale($globallink->nid, $globallink->sourceLocale, $globallink->targetLocale);
      db_update('globallink_core_entity')
      ->fields(array(
        'status' => 'Error',
        'timestamp' => REQUEST_TIME,
      ))
      ->condition('rid', $row->rid, '=')
      ->execute();

      break;
  }
}

/**
 * Updates entity.
 *
 * @param object $globallink
 *   GlobalLink object.
 * @param array $t_arr
 *   The translated array.  Defaults to NULL.
 */
function globallink_entity_update(&$globallink, $t_arr = NULL) {
  try {
    $target_lang = globallink_get_drupal_locale_code($globallink->targetLocale);

    if ($t_arr != NULL) {
      $translated_arr = $t_arr;
    }
    else {
      $translated_arr = globallink_entity_get_translated_array($globallink->targetXML, $target_lang);
    }

    $target_nid = $translated_arr['nid'];
    $target_vid = $translated_arr['vid'];
    $globallink->nid = $target_nid;
    $globallink->vid = $target_vid;

    $node = node_load($target_nid, $target_vid);

    if (isset($translated_arr['title'])) {
      $node->title = $translated_arr['title'];
    }

    if (isset($node->title_original) && $node->title != $node->title_original) {
      $node->title = $node->title_original;
    }

    if (!$node || is_null($node) || !is_object($node)) {
      $globallink->status = 'Source Deleted';

      return;
    }

    $nodepath = drupal_lookup_path('alias', 'node/' . $target_nid);

    if ($nodepath) {
      if (!empty($translated_arr['path'])) {
        $path = array('alias' => $translated_arr['path'], 'source' => 'node/' . $target_nid, 'language' => $target_lang);
      }
      else {
        $path = array('alias' => $nodepath, 'source' => 'node/' . $target_nid, 'language' => $target_lang);
      }

      if (module_exists('pathauto')) {
        $path['pathauto'] = FALSE;
      }

      path_save($path);
    }

    $node->revision = 1;

    if (module_exists('revisioning')) {
      $node->is_pending = FALSE;
      $node->revision_moderation = FALSE;
    }

    // Node presave hook skip
    $node->tpt_skip = TRUE;

    if (module_exists('metatag')) {
      if (isset($translated_arr['metatag'])) {
        $target_metatag_arr = $translated_arr['metatag'];

        if (!empty($node->metatags[$node->language])) {
          $metatags_names = array_keys($node->metatags[$node->language]);
          $n_metatag = &$node->metatags;

          foreach ($metatags_names as $name) {
            if (isset($target_metatag_arr[$name]) && isset($target_metatag_arr[$name][0])) {
              $gl_obj = $target_metatag_arr[$name]['0'];

              if (is_object($gl_obj)) {
                $translated_content = $gl_obj->translatedContent;
              }
              else {
                $translated_content = $gl_obj;
              }

              $n_metatag[$target_lang][$name] = array('value' => $translated_content);
            }
          }
        }
      }
    }

    $success = globallink_entity_save_translated_entity_with_fields($node, $translated_arr, $target_lang);

    if ($success) {
      // todo Pathauto Module Support
      $globallink->status = 'Published';
    }
    else {
      $globallink->status = 'Error';
    }
  }
  catch (Exception $e) {
    $globallink->status = 'Error';
    watchdog('GlobalLink', 'Exception - %function - File[%file], Line[%line], Code[%code], Message[%message]', array('%function' => __FUNCTION__, '%file' => $e->getFile(), '%line' => $e->getLine(), '%code' => $e->getCode(), '%message' => $e->getMessage()), WATCHDOG_ERROR);
  }
}

/**
 * Gets array of translated entities.
 *
 * @param object $xml
 *   XML representation of entities.
 * @param string $target_lang
 *   The entities' target language.  Defaults to LANGUAGE_NONE.
 *
 * @return array
 *   Associative array of translated entities.
 */
function globallink_entity_get_translated_array($xml, $target_lang = LANGUAGE_NONE) {
  if (is_null($xml) || !is_string($xml) || $xml == '') {
    return array();
  }

  $dom = new DomDocument;
  $dom->preserveWhiteSpace = FALSE;
  $dom->loadXML($xml);

  $arr = array();

  $contents = $dom->getElementsByTagName('content');

  foreach ($contents as $content) {
    if (!is_null($content->attributes)) {
      foreach ($content->attributes as $attr_name => $attr_node) {
        switch ($attr_name) {
          case 'rid':
            $arr['rid'] = $attr_node->value;

            break;
          case 'nid':
            $arr['nid'] = $attr_node->value;

            break;
          case 'vid':
            $arr['vid'] = $attr_node->value;

            break;
        }
      }
    }
  }

  $titles = $dom->getElementsByTagName('title');

  foreach ($titles as $title) {
    $arr['title'] = $title->nodeValue;
  }

  $paths = $dom->getElementsByTagName('path');

  foreach ($paths as $path) {
    $arr['path'] = $path->nodeValue;
  }

  $field_image = $dom->getElementsByTagName('field_image');

  foreach ($field_image as $attr) {
    $field_image_obj = new GLFieldImage();

    if (!is_null($attr->attributes)) {
      foreach ($attr->attributes as $attr_name => $attr_node) {
        switch ($attr_name) {
          case 'type':
            if ($attr_node->value == 'title') {
              $field_image_obj->title = $attr->nodeValue;
            }
            elseif ($attr_node->value == 'alt') {
              $field_image_obj->alt = $attr->nodeValue;
            }

            continue 2;
          case 'delta':
            $field_image_obj->delta = $attr_node->value;

            continue 2;
          case 'field_name':
            $field_image_obj->field_name = $attr_node->value;

            continue 2;
        }
      }

      if (is_null($field_image_obj->delta)) {
        $field_image_obj->delta = '0';
      }

      if (isset($field_image_obj->field_name)) {
        if (isset($field_image_obj->title)) {
          $arr[$field_image_obj->field_name][$target_lang][$field_image_obj->delta]->title = $field_image_obj->title;
        }

        if (isset($field_image_obj->alt)) {
          $arr[$field_image_obj->field_name][$target_lang][$field_image_obj->delta]->alt = $field_image_obj->alt;
        }
      }
      else {
        $arr[$field_image_obj->field_name][$target_lang][$field_image_obj->delta] = $field_image_obj;
      }
    }
  }

  $fields = $dom->getElementsByTagName('field');

  foreach ($fields as $field) {
    $field_obj = new GLField();
    $field_obj->type = 'field';
    $field_obj->translatedContent = $field->nodeValue;

    if (!is_null($field->attributes)) {
      foreach ($field->attributes as $attr_name => $attr_node) {
        switch ($attr_name) {
          case 'entity_type':
            $field_obj->entityType = $attr_node->value;

            continue 2;
          case 'content_type':
            $field_obj->contentType = $attr_node->value;

            continue 2;
          case 'parent_fc':
            $field_obj->parentFCName = $attr_node->value;

            continue 2;
          case 'bundle':
            $field_obj->bundle = $attr_node->value;

            continue 2;
          case 'entity_id':
            $field_obj->entityId = $attr_node->value;

            continue 2;
          case 'field_name':
            $field_obj->fieldName = $attr_node->value;

            continue 2;
          case 'label':
            $field_obj->fieldLabel = $attr_node->value;

            continue 2;
          case 'delta':
            $field_obj->delta = $attr_node->value;

            continue 2;
          case 'format':
            $field_obj->format = $attr_node->value;

            continue 2;
        }
      }

      if (is_null($field_obj->entityId)) {
        $field_obj->entityId = '0';
      }

      if (is_null($field_obj->bundle)) {
        $field_obj->bundle = $field_obj->fieldName;
      }

      if (is_null($field_obj->delta)) {
        $field_obj->delta = '0';
      }

      if ($field_obj->entityType == 'node') {
        $arr[$field_obj->fieldName][$target_lang][$field_obj->delta] = $field_obj;
      }
      else {
        $arr['field_collection'][$field_obj->parentFCName][$field_obj->bundle][$field_obj->entityId][$field_obj->fieldName][$target_lang][$field_obj->delta] = $field_obj;
      }
    }
  }

  $metatags = $dom->getElementsByTagName('metatag');

  foreach ($metatags as $metatag) {
    $metatag_obj = new GLField();
    $metatag_obj->type = 'metatag';
    $metatag_obj->translatedContent = $metatag->nodeValue;

    if (!is_null($metatag->attributes)) {
      foreach ($metatag->attributes as $attr_name => $attr_node) {
        switch ($attr_name) {
          case 'entity_type':
            $metatag_obj->entityType = $attr_node->value;

            continue 2;
          case 'content_type':
            $field_obj->contentType = $attr_node->value;

            continue 2;
          case 'bundle':
            $field_obj->bundle = $attr_node->value;

            continue 2;
          case 'entity_id':
            $metatag_obj->entityId = $attr_node->value;

            continue 2;
          case 'name':
            $metatag_obj->fieldName = $attr_node->value;

            continue 2;
          case 'label':
            $metatag_obj->fieldLabel = $attr_node->value;

            continue 2;
        }
      }

      if (is_null($metatag_obj->entityId)) {
        $metatag_obj->entityId = '0';
      }

      if (is_null($metatag_obj->bundle)) {
        $metatag_obj->bundle = $metatag_obj->fieldName;
      }

      $arr['metatag'][$metatag_obj->bundle][$metatag_obj->entityId] = $metatag_obj;
    }
  }

  return $arr;
}

/**
 * Gets active entity submission by node ID.
 *
 * @param string $nid
 *   The entity node ID.
 *
 * @return array
 *   Associative array representing the active entity submission.
 */
function globallink_entity_get_active_submission_by_nid($nid) {
  $query = db_select('globallink_core_entity', 'tc');
  $query->condition('status', array('Sent for Translations', 'Error'), 'IN');
  $query->condition('nid', $nid, '=');
  $query->fields('tc');

  $results = $query->execute();
  $arr = array();

  foreach ($results as $row) {
    if (array_key_exists($row->submission, $arr)) {
      $t_arr = $arr[$row->submission];
      $t_arr[$row->target] = $row->vid;
      $arr[$row->submission] = $t_arr;
    }
    else {
      $_arr = array($row->target => $row->vid);
      $arr[$row->submission] = $_arr;
    }
  }

  return $arr;
}

/**
 * Gets active entity submission rows by node ID.
 *
 * @param string $nids
 *   The array of entity node IDs.
 *
 * @return
 *   Associative array of entity active submission rows.  FALSE if the array is empty.
 */
function globallink_entity_get_active_submission_rows_by_nid($nids) {
  $query = db_select('globallink_core_entity', 'tc');
  $query->condition('status', array('Sent for Translations', 'Error'), 'IN');
  $query->condition('nid', $nids, 'IN');
  $query->fields('tc');

  $results = $query->execute();
  $arr = array();

  foreach ($results as $row) {
    if (array_key_exists($row->nid, $arr)) {
      array_push($arr[$row->nid], $row);
    }
    else {
      $arr[$row->nid] = array($row);
    }
  }

  $final_arr = array();

  foreach ($arr as $nid => $nid_arr) {
    $sub_arr = array();

    foreach ($nid_arr as $r) {
      if (array_key_exists($r->submission, $sub_arr)) {
        array_push($sub_arr[$r->submission], $r->target);
      }
      else {
        $sub_arr[$r->submission] = array($r->vid => $r->target);
      }
    }

    if (count($sub_arr) > 0) {
      $final_arr[$nid] = $sub_arr;
    }
  }

  if (count($final_arr) > 0) {
    return $final_arr;
  }

  return FALSE;
}

/**
 * Saves entity.
 *
 * @param object $node
 *   The entity node.
 *
 * @return bool
 *   TRUE if successful.  FALSE on failure.
 */
function globallink_entity_save(&$node) {
  try {
    node_save($node);

    return TRUE;
  }
  catch (Exception $e) {
    watchdog('GlobalLink', 'Exception - %function - File[%file], Line[%line], Code[%code], Message[%message]', array('%function' => __FUNCTION__, '%file' => $e->getFile(), '%line' => $e->getLine(), '%code' => $e->getCode(), '%message' => $e->getMessage()), WATCHDOG_ERROR);

    return FALSE;
  }
}

/**
 * Saves translated entity with fields.
 *
 * @param object $node
 *   The entity node.
 * @param array $translation_arr
 *   The translation array.
 * @param string $target_lang
 *   The target language of the entity.
 *
 * @return bool
 *   TRUE if save was successful.  FALSE on failure.
 */
function globallink_entity_save_translated_entity_with_fields(&$node, $translation_arr, $target_lang) {
  $config_fields = globallink_get_config_fields($node->type);

  foreach ($config_fields as $field_name => $field_info) {
    if ($field_info->field_type == 'image' && $field_info->translatable == 1) {
      $source_lang = $node->language;
      if (isset($node->{$field_name}[$source_lang])) {
        $node->{$field_name}[$target_lang] = $node->{$field_name}[$source_lang];
      }
    }
  }

  foreach ($translation_arr as $field => $values) {
    if (!is_array($values)) {
      continue;
    }

    foreach ($values as $target => $arr_value) {
      foreach ($arr_value as $obj) {
        if (!isset($obj->translatedContent)) {
          continue;
        }

        $format = '';

        if (!empty($obj->format)) {
          $format = $obj->format;
        }

        if (isset($node->{$field}[$target])) {
          $node->{$field}[$target][$obj->delta] = array(
            'value' => $obj->translatedContent,
            'format' => $format,
          );
        }
        else {
          $node->{$field}[$target] = array(
            $obj->delta => array(
              'value' => $obj->translatedContent,
              'format' => $format,
            ),
          );
        }
      }
    }
  }

  $is_hook_enabled = variable_get('globallink_implementation_type', 0);

  if ($is_hook_enabled == 1) {
    globallink_update_node_hook($node, $node);
  }

  // Set new translation data
  $node->translations->data[$target_lang] = array(
    'entity_type' => 'node',
    'entity_id' => $node->nid,
    'language' => $target_lang, // Target language
    'source' => $node->language, // Source language
    'uid' => $node->uid,
    'status' => variable_get('globallink_publish_node', 0), // publish translation
    'translate' => 0, // Translation out of date
    'created' => $node->created,
    'changed' => $node->changed, // Date of the first node save
  );

  // Save new translation data
  $success = globallink_entity_save($node);

  return $success;
}

/**
 * Saves entity field collections.
 *
 * @param object $node
 *   The entity node.
 * @param array $t_arr
 *   The translated array.
 * @param string $target_lang
 *   The node's target language.
 */
function globallink_entity_save_field_collections($node, $t_arr, $target_lang) {
  $field_arr = field_info_instances('node', $node->type);
  $keys = array_keys($field_arr);

  foreach ($keys as $field) {
    $items = field_get_items('node', $node, $field);

    if ($items) {
      $field_def = field_read_field($field);

      if ($field_def['type'] == 'field_collection') {
        if (isset($t_arr['field_collection'][$field])) {
          foreach ($items as $entity_id_arr) {
            if (isset($entity_id_arr['value'])) {
              $fc_entity_id = $entity_id_arr['value'];
              globallink_entity_save_field_collections_recursively('node', $node, $fc_entity_id, $t_arr['field_collection'][$field], $target_lang);
            }
          }
        }
      }
    }
  }
}

/**
 * Saves entity field collections recursively.
 *
 * @param string $entity_type
 *   The entity type.
 * @param string $host_entity
 *   The host entity.
 * @param string $fc_entity_id
 *   The field collection entity ID.
 * @param array $translated_fc_arr
 *   The translated field collection array.
 * @param string $target_lang
 *   The entities' target language.
 */
function globallink_entity_save_field_collections_recursively($entity_type, $host_entity, $fc_entity_id, $translated_fc_arr, $target_lang) {
  $field_collection_item_entity = entity_load_unchanged('field_collection_item', $fc_entity_id);

  if (!$field_collection_item_entity) {
    return;
  }

  $field_collection_name = $field_collection_item_entity->field_name;
  $field_collection_item_array = get_object_vars($field_collection_item_entity);
  $arr = array_keys($field_collection_item_array);

  $new_field_collection_arr = array('field_name' => $field_collection_name);

  foreach ($arr as $key) {
    // Check if this key exists; If true then read this.
    $fc_field_def = field_read_field($key);

    if (!empty($fc_field_def) && isset($fc_field_def['type'])) {
      switch ($fc_field_def['type']) {
        case 'list_boolean':
        case 'file':
        case 'taxonomy_term_reference':
          continue 2;

          break;
        case 'field_collection':
          if (isset($field_collection_item_array[$key]) && isset($field_collection_item_array[$key][$target_lang])) {
            $field_data_arr = $field_collection_item_array[$key][$target_lang];
            $new_field_collection_arr[$key][$target_lang] = $field_data_arr;
          }

          break;
        default:
          if (!isset($field_collection_item_array[$key])) {
            break;
          }

          if (!isset($field_collection_item_array[$key][LANGUAGE_NONE])) {
            break;
          }

          $field_data_arr = $field_collection_item_array[$key][LANGUAGE_NONE];

          foreach ($field_data_arr as $delta => $field_data) {
            if (isset($translated_fc_arr[$field_collection_name][$fc_entity_id][$key][$target_lang][$delta])) {
              $gl_obj = $translated_fc_arr[$field_collection_name][$fc_entity_id][$key][$target_lang][$delta];

              if (is_object($gl_obj)) {
                $translated_content = $gl_obj->translatedContent;
              }
              else {
                $translated_content = $gl_obj;
              }

              $new_field_collection_arr[$key][$target_lang][$delta] = array('value' => $translated_content);

              if (isset($field_data['format'])) {
                $new_field_collection_arr[$key][$target_lang][$delta]['format'] = $field_data['format'];
              }
            }
          }
      }
    }
  }

  if ($entity_type != 'node') {
    $host_entity->tpt_skip = TRUE;
  }

  $new_entity = entity_create('field_collection_item', $new_field_collection_arr); // Create new field collection item.
  $new_entity->setHostEntity($entity_type, $host_entity); // Attach it to the node.
  $new_entity->save(TRUE); // Save field-collection item

  field_attach_presave($entity_type, $host_entity);
  field_attach_update($entity_type, $host_entity);

  // Now set the child FC if any
  foreach ($arr as $key) {
    // Check if this key exists; If true then read this.
    $fc_field_def = field_read_field($key);

    if ($fc_field_def && !empty($fc_field_def) && isset($fc_field_def['type'])) {
      if ($fc_field_def['type'] == 'field_collection') {
        $items = field_get_items('field_collection_item', $field_collection_item_entity, $key);

        if ($items) {
          foreach ($items as $entity_id_arr) {
            if (isset($entity_id_arr['value'])) {
              $fc_entity_id = $entity_id_arr['value'];
              globallink_entity_save_field_collections_recursively('field_collection_item', $new_entity, $fc_entity_id, $translated_fc_arr, $target_lang);
            }
          }
        }
      }
    }
  }
}

/**
 * Gets XML data from specific entity.
 *
 * @param object $node
 *   The entity node.
 * @param array $target_arr
 *   The target array
 * @param string $tnid
 *   The translated entity node ID.  Defaults to NULL.
 * @param string $tvid
 *   The translated entity VID.  Defaults to NULL.
 * @param string $name
 *   The node name.  Defaults to empty.
 * @param bool $for_display
 *   Whether or not the XML will be displayed.  Defaults to FALSE.
 * @param string $source_lang
 *   The source language of the entity.  Defaults to empty.
 *
 * @return
 *   XML representation of the entity.  'Source Deleted' if the entity no longer exists.
 */
function globallink_entity_get_xml($node, $target_arr, $tnid = NULL, $tvid = NULL, &$name = '', $for_display = FALSE, $source_lang = '') {
  if (is_null($node)) {
    return 'Source Deleted';
  }
  elseif (!$node) {
    return 'Source Deleted';
  }

  if ($node && is_object($node)) {
    if ($node->language != 'en') {
      $name = 'Node_' . $node->nid . '_Non_English' . $name;
    }
    else {
      $name = globallink_format_file_name($node->title) . $name;
    }

    $xml = globallink_entity_generate_xml_document($node, $target_arr, $tnid, $tvid, $for_display, $source_lang);

    return $xml;
  }

  return 'Source Deleted';
}

/**
 * Generates XML document for entity.
 *
 * @param object $node
 *   The entity node.
 * @param array $target_arr
 *   The target array
 * @param string $tnid
 *   The translated entity node ID.  Defaults to NULL.
 * @param string $tvid
 *   The translated entity VID.  Defaults to NULL.
 * @param bool $for_display
 *   Whether or not the XML will be displayed.  Defaults to FALSE.
 * @param string $source_lang
 *   The source language of the entity.  Defaults to empty.
 *
 * @return
 *   XML representation of the entity.  FALSE on failure.
 */
function globallink_entity_generate_xml_document($node, $target_arr, $tnid = NULL, $tvid = NULL, $for_display = FALSE, $source_lang = '') {
  $is_hook_enabled = variable_get('globallink_implementation_type', 0);

  try {
    $dom = new DOMDocument('1.0', 'UTF-8');
    $dom->formatOutput = TRUE;

    $root = $dom->createElement('content');
    $nid = $dom->createAttribute('nid');

    if ($tnid != NULL) {
      $nid->value = $tnid;
    }
    else {
      $nid->value = $node->nid;
    }

    $root->appendChild($nid);

    $vid = $dom->createAttribute('vid');

    if ($tvid != NULL) {
      $vid->value = $tvid;
    }
    else {
      $vid->value = $node->vid;
    }

    $root->appendChild($vid);

    $url = $dom->createAttribute('pageUrl');
    $path = path_load(array('source' => 'node/' . $node->nid, 'language' => $source_lang));

    if (isset($path['alias'])) {
      $url->value = url($path['alias'], array('absolute' => TRUE));
    }
    else {
      $url->value = url('node/' . $node->nid, array('absolute' => TRUE));
    }

    if (!empty($path)) {
      globallink_insert_child_element($dom, $root, 'path', $path['alias']);
    }

    $root->appendChild($url);
    $dom->appendChild($root);
    $field_arr = field_info_instances('node', $node->type);
    $keys = array_keys($field_arr);

    foreach ($keys as $field) {
      $field_def = field_read_field($field);
      $items = field_get_items('node', $node, $field, $source_lang);

      if ($items) {
        $parent_fc = '';

        if ($field_def['type'] == 'field_collection') {
          $parent_fc = $field;

          if (isset($items[0]['revision_id'])) {
            entity_load('field_collection_item', array($items[0]['value']), array('revision_id' => $items[0]['revision_id']));
          }
        }

        globallink_entity_traverse_fields_and_field_collections('node', $node->type, $parent_fc, $node->type, $node->nid, $items, $field, $field_def, $dom, $root, $node, $target_arr, $is_hook_enabled);
      }
    }

    if (module_exists('metatag')) {
      if (isset($node->metatags)) {
        if ($for_display) {
          if (!empty($node->metatags[$node->language])) {
            $metatags = $node->metatags[$node->language];

            foreach ($metatags as $name => $value) {
              if (isset($value['value'])) {
                globallink_insert_child_element($dom, $root, 'metatag', $value['value'], array('entity_type' => 'node', 'name' => $name, 'label' => 'Metatag - ' . ucwords($name)));
              }
            }
          }
        }
        elseif (globallink_is_field_configured_for_translation('node', $node->type, 'metatags', $node->type)) {
          if (!empty($node->metatags[$node->language])) {
            $metatags = $node->metatags[$node->language];

            foreach ($metatags as $name => $value) {
              if (isset($value['value'])) {
                globallink_insert_child_element($dom, $root, 'metatag', $value['value'], array('entity_type' => 'node', 'name' => $name, 'label' => 'Metatag - ' . ucwords($name)));
              }
            }
          }
        }
      }
    }
  }
  catch (Exception $e) {
    watchdog('GlobalLink', 'Exception - %function - File[%file], Line[%line], Code[%code], Message[%message]', array('%function' => __FUNCTION__, '%file' => $e->getFile(), '%line' => $e->getLine(), '%code' => $e->getCode(), '%message' => $e->getMessage()), WATCHDOG_ERROR);
    throw $e;
  }

  $root_element = $dom->getElementsByTagName('content')->item(0);

  if (!$root_element->hasChildNodes()) {
    return FALSE;
  }

  return $dom->saveXML();
}

/**
 * Traverses entity fields and field collections.
 *
 * @param string $entity_type
 *   The entity type.
 * @param string $content_type
 *   The content type.
 * @param string $parent_fc
 *   The parent field collection.
 * @param string $bundle
 *   The bundle.
 * @param string $entity_id
 *   The entity ID.
 * @param array $items
 *   The array of items.
 * @param object $field
 *   The entity field.
 * @param object $field_def
 *   The field definition object.
 * @param object $dom
 *   Object representation of the DOM.
 * @param object $root
 *   The DOM root.
 * @param object $source_node
 *   The source node.
 * @param array $target_arr
 *   The target array.
 * @param bool $is_hook_enabled
 *   Determines whether or not the hook is enabled.  Defaults to 0.
 */
function globallink_entity_traverse_fields_and_field_collections($entity_type, $content_type, $parent_fc, $bundle, $entity_id, $items, $field, $field_def, $dom, $root, $source_node, $target_arr, $is_hook_enabled = 0) {
  if (!$items) {
    return;
  }

  switch ($field_def['type']) {
    case 'list_boolean':
    case 'file':
    case 'taxonomy_term_reference':
      return;
  }

  if ($field_def['type'] != 'field_collection') {
    if ($is_hook_enabled == 0) {
      if (!globallink_is_field_configured_for_translation($entity_type, $bundle, $field, $content_type)) {
        return;
      }

      // Regular Text Field, get the content directly from items array
      foreach ($items as $delta => $item) {
        if (isset($item['value']) && is_string($item['value'])) {
          $f_label = field_info_instance($entity_type, $field, $bundle);
          $f_value = $item['value'];
          $f_format = (isset($item['format']) && !is_null($item['format'])) ? $item['format'] : '';

          globallink_insert_child_element($dom, $root, 'field', $f_value, array('entity_type' => $entity_type, 'content_type' => $content_type, 'parent_fc' => $parent_fc, 'bundle' => $bundle, 'entity_id' => $entity_id, 'field_name' => $field, 'label' => $f_label['label'], 'delta' => $delta, 'format' => $f_format));
        }
        elseif ($field_def['type'] == 'link_field' && isset($item['title']) && is_string($item['title'])) {
          $f_label = field_info_instance($entity_type, $field, $bundle);
          $f_value = $item['title'];
          $f_format = (isset($item['format']) && !is_null($item['format'])) ? $item['format'] : '';

          globallink_insert_child_element($dom, $root, 'field', $f_value, array('entity_type' => $entity_type, 'content_type' => $content_type, 'parent_fc' => $parent_fc, 'bundle' => $bundle, 'entity_id' => $entity_id, 'field_name' => $field, 'label' => $f_label['label'], 'delta' => $delta, 'format' => $f_format));
        }
        elseif ($field_def['type'] == 'image') {
          $f_label = field_info_instance($entity_type, $field, $bundle);

          if (isset($item['alt'])) {
            $f_value = $item['alt'];

            globallink_insert_child_element($dom, $root, 'field_image', $f_value, array('field_name' => $field, 'type' => 'alt', 'delta' => $delta));
          }

          if (isset($item['title'])) {
            $f_value = $item['title'];

            globallink_insert_child_element($dom, $root, 'field_image', $f_value, array('field_name' => $field, 'type' => 'title', 'delta' => $delta));
          }
        }
      }
    }
    else {
      if (!globallink_is_field_translatable($source_node, $field, $target_arr)) {
        return;
      }

      // Regular Text Field, get the content directly from items array
      foreach ($items as $delta => $item) {
        if (isset($item['value']) && is_string($item['value'])) {
          $f_label = field_info_instance($entity_type, $field, $bundle);
          $f_value = $item['value'];
          $f_format = (isset($item['format']) && !is_null($item['format'])) ? $item['format'] : '';
          globallink_insert_child_element($dom, $root, 'field', $f_value, array('entity_type' => $entity_type, 'content_type' => $content_type, 'parent_fc' => $parent_fc, 'bundle' => $bundle, 'entity_id' => $entity_id, 'field_name' => $field, 'label' => $f_label['label'], 'delta' => $delta, 'format' => $f_format));
        }
        elseif ($field_def['type'] == 'link_field' && isset($item['title']) && is_string($item['title'])) {
          $f_label = field_info_instance($entity_type, $field, $bundle);
          $f_value = $item['title'];
          $f_format = (isset($item['format']) && !is_null($item['format'])) ? $item['format'] : '';
          globallink_insert_child_element($dom, $root, 'field', $f_value, array('entity_type' => $entity_type, 'content_type' => $content_type, 'parent_fc' => $parent_fc, 'bundle' => $bundle, 'entity_id' => $entity_id, 'field_name' => $field, 'label' => $f_label['label'], 'delta' => $delta, 'format' => $f_format));
        }
      }
    }
  }
  else {
    if (!module_exists('field_collection')) {
      return;
    }

    // Field Collection field, read the entity id from item and load
    // Entity object and then do recursion for nested field collections.
    foreach ($items as $entity_id_arr) {
      if (!isset($entity_id_arr['value'])) {
        continue;
      }

      $fc_entity_id = $entity_id_arr['value'];
      $field_collection_item_entity_arr = array();

      if (isset($entity_id_arr['revision_id'])) {
        $field_collection_item_entity_arr = entity_load('field_collection_item', array($fc_entity_id), array('revision_id' => $entity_id_arr['revision_id']));
      }
      else {
        $field_collection_item_entity_arr = entity_load('field_collection_item', array($fc_entity_id));
      }

      if (!$field_collection_item_entity_arr || !is_array($field_collection_item_entity_arr) || sizeof($field_collection_item_entity_arr) < 1) {
        continue;
      }

      $field_collection_item_entity = $field_collection_item_entity_arr[$fc_entity_id];
      $field_collection_name = $field_collection_item_entity->field_name;
      $field_collection_item_array = get_object_vars($field_collection_item_entity);
      $arr = array_keys($field_collection_item_array);

      foreach ($arr as $key) {
        // Check if this key exists; If true then read this.
        $fc_field_def = field_read_field($key);

        if (!$fc_field_def || empty($fc_field_def) || !isset($fc_field_def['type'])) {
          continue;
        }

        switch ($fc_field_def['type']) {
          case 'list_boolean':
          case 'file':
          case 'taxonomy_term_reference':
            continue 2;
        }

        $fc_item = field_get_items('field_collection_item', $field_collection_item_entity, $key);

        if ($fc_item) {
          globallink_entity_traverse_fields_and_field_collections('field_collection_item', $content_type, $parent_fc, $field_collection_name, $fc_entity_id, $fc_item, $key, $fc_field_def, $dom, $root, $source_node, $target_arr, $is_hook_enabled);
        }
      }
    }
  }
}

/**
 * Gets entity title
 *
 * @param object $globallink
 *   GlobalLink object.
 *
 * @return
 *   The entity title.  FALSE if it cannot be found.
 */
function globallink_entity_get_title(&$globallink) {
  $result = db_select('globallink_core_entity', 'tc')
  ->fields('tc')
  ->condition('document_ticket', $globallink->documentTicket, '=')
  ->condition('submission_ticket', $globallink->submissionTicket, '=')
  ->execute();

  foreach ($result as $item) {
    $nid = $item->nid;
    $globallink->nid = $nid;
    $globallink->vid = $item->vid;
    $globallink->tptRowId = $item->rid;
    $globallink->title = $item->title;

    switch ($item->status) {
      case 'Sent for Translations':
        $globallink->status = 'Translation Completed';

        break;
      case 'Error':
        $globallink->status = 'Error';

        break;
    }

    return l(globallink_format_display_string($globallink->title), 'node/' . $item->nid);
  }

  return FALSE;
}

/**
 * Updates entity submission status.
 *
 * @param string $submission_ticket
 *   The submission ticket.
 * @param string $status
 *   The submission status.  Defaults to 'Cancelled.'
 */
function globallink_entity_update_submission_status($submission_ticket, $status = 'Cancelled') {
  db_update('globallink_core_entity')
  ->fields(array(
    'status' => $status,
    'timestamp' => REQUEST_TIME,
  ))
  ->condition('submission_ticket', $submission_ticket, '=')
  ->execute();
}

/**
 * Gets entity submission status.
 */
function globallink_entity_get_submission_status() {
  module_load_include('inc', 'globallink', 'globallink');
  module_load_include('inc', 'globallink', 'globallink_settings');
  module_load_include('inc', 'globallink', 'gl_ws/gl_ws_common');

  $query = db_select('globallink_core_entity', 'tc');
  $query->fields('tc', array('submission_ticket'));
  $query->distinct();
  $query->condition('status', 'Sent for Translations', '=');

  $results = $query->execute();

  foreach ($results as $row) {
    if ($row->submission_ticket) {
      try {
        $pd4 = globallink_get_project_director_details();
        $status = globallink_get_status($pd4, $row->submission_ticket);

        if (!$status || $status == 'CANCELLED') {
          globallink_entity_update_submission_status($row->submission_ticket);
        }
      }
      catch (SoapFault $se) {
        globallink_entity_update_submission_status($row->submission_ticket);
      }
      catch (Exception $ex) {
        globallink_entity_update_submission_status($row->submission_ticket);
      }
    }
  }
}

/**
 * Checks entity status based on row IDs.
 *
 * @param array $rids_arr
 *   Array of row IDs.
 *
 * @return array
 *   Array of row IDs that have been sent for translation or threw an error.
 */
function globallink_entity_check_status($rids_arr) {
  $status = TRUE;
  $query = db_select('globallink_core_entity', 'tc')
  ->fields('tc', array('rid'))
  ->condition('status', array('Sent for Translations', 'Error'), 'IN');

  $results = $query->execute();
  $rows = array();

  foreach ($results as $item) {
    $rows[$item->rid] = $item->rid;
  }

  foreach ($rids_arr as $val) {
    if (!in_array($val, $rows)) {
      unset($rids_arr[$val]);
      $status = FALSE;
    }
  }

  if (!$status) {
    drupal_set_message(t('Cannot cancel documents that have been cancelled in Globallink.'), 'warning', NULL);
  }

  return $rids_arr;
}

/**
 * Clears cancelled entity documents.
 *
 * @return int
 *   Number of cancelled entity documents.
 */
function globallink_entity_clear_cancelled_documents() {
  $count = 0;
  $query = db_select('globallink_core_entity', 'tc')
  ->fields('tc', array('submission_ticket'))
  ->distinct()
  ->condition('status', 'Cancelled', '=');

  $results = $query->execute();

  foreach ($results as $item) {
    globallink_entity_update_submission_status($item->submission_ticket, 'Pending Translations');

    $count++;
  }

  return $count;
}
