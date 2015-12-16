<?php

/**
 * Sends webforms for translation.
 *
 * @param array $lids
 *   The array of webform LIDs.
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
function globallink_webform_send_for_translations($lids, $pd4, $submission_name, $due_date, $project_code, $source_locale, $target_locale_arr, $submission_details) {
  module_load_include('inc', 'globallink', 'gl_ws/gl_ws_send_translations');

  $globallink_arr = array();

  $submitter = $submission_details['submitter'];

  foreach ($lids as $lid) {
    $rows = globallink_webform_get_sent_tpt_rows_by_lid($lid);

    $target_arr = $target_locale_arr;

    foreach ($rows as $row) {
      if (array_search($row->target, $target_locale_arr)) {
        unset($target_arr[$row->target]);
      }
    }

    if (empty($target_arr)) {
      continue;
    }

    $xml = globallink_webform_get_xml($lid);

    $name = 'Webform_' . $lid . '.xml';
    watchdog('GlobalLink', 'XML - %xml', array('%xml' => $xml), WATCHDOG_DEBUG);

    $globallink = new GlobalLink();
    $globallink->type = 'webform';
    $globallink->metadata = 'webform';
    $globallink->sourceLocale = $source_locale;
    $globallink->targetLocale = $target_arr;
    $globallink->sourceXML = $xml;
    $globallink->sourceFileName = $name;
    $globallink->submissionName = $submission_name;
    $globallink->dueDate = $due_date;
    $globallink->otherObjectId = $lid;
    $globallink->submissionInstructions = $submission_details['instructions'] . "\nSubmitter: " . $submitter;
    $globallink_arr[] = $globallink;
  }

  if (!empty($globallink_arr)) {
    globallink_send_documents_for_translation_to_pd($globallink_arr, $pd4, $project_code, $submitter);
  }

  return $globallink_arr;
}

/**
 * Gets XML data from specific webform.
 *
 * @param string $lid
 *   The webform LID.
 *
 * @return array
 *   Associative array of block XML data.
 */
function globallink_webform_get_xml($lid) {
  $property_arr = array();

  $string_result = db_select('i18n_string', 's')
  ->fields('s')
  ->condition('lid', $lid, '=')
  ->execute();

  foreach ($string_result as $row) {
    $property_arr['property'] = $row->property;
  }

  $dom = new DOMDocument('1.0', 'UTF-8');
  $dom->formatOutput = TRUE;

  $root = $dom->createElement('content');
  $dom->appendChild($root);

  $id = $dom->createAttribute('lid');
  $id->value = $lid;
  $root->appendChild($id);

  $result = db_select('locales_source', 'ls')
  ->fields('ls')
  ->condition('lid', $lid, '=')
  ->execute();

  foreach ($result as $row) {
    globallink_insert_child_element($dom, $root, 'webform', $row->source, array(
      'name' => $property_arr['property'],
      'lid' => $lid,
      'location' => $row->location,
      'format' => isset($lid['format']) ? $lid['format'] : '',
    ));
  }

  $xml = $dom->saveXML();

  return $xml;
}

/**
 * Updates webform ticket ID.
 *
 * @param array $arr
 *   Array of GlobalLink objects.
 * @param string $project_code
 *   The webform's project code.
 */
function globallink_webform_update_ticket_id($arr, $project_code) {
  foreach ($arr as $globallink) {
    $target_locale_arr = $globallink->targetLocale;
    $type = $globallink->type;

    if ($type != 'webform') {
      continue;
    }

    $lid = $globallink->otherObjectId;
    $webform = globallink_load_source_data($lid);

    foreach ($target_locale_arr as $target_locale) {
      $row = globallink_webform_get_row($lid, $type, $globallink->sourceLocale, $target_locale);

      if ($row) {
        db_update('globallink_core_webform')
        ->fields(array(
          'title' => $webform[0]->source,
          'document_ticket' => $globallink->documentTicket,
          'submission' => $globallink->submissionName,
          'submission_ticket' => $globallink->submissionTicket,
          'status' => 'Sent for Translations',
          'timestamp' => REQUEST_TIME,
          'last_modified' => REQUEST_TIME,
          'project_code' => $project_code,
        ))
        ->condition('rid', $row->rid, '=')
        ->execute();
      }
      else {
        db_insert('globallink_core_webform')
        ->fields(array(
          'object_id' => $lid,
          'object_type' => $globallink->type,
          'title' => $webform[0]->source,
          'source' => $globallink->sourceLocale,
          'target' => $target_locale,
          'document_ticket' => $globallink->documentTicket,
          'submission' => $globallink->submissionName,
          'submission_ticket' => $globallink->submissionTicket,
          'status' => 'Sent for Translations',
          'timestamp' => REQUEST_TIME,
          'last_modified' => REQUEST_TIME,
          'project_code' => $project_code,
        ))
        ->execute();
      }
    }
  }
}

/**
 * Updates status for deleted webforms.
 *
 * @param string $pd4
 *   The project director details.
 * @param object $globallink
 *   GlobalLink object.
 *
 * @return bool
 *   TRUE if update was successful, FALSE on error.
 */
function globallink_webform_update_deleted_records($pd4, $globallink) {
  try {
    $globallink->status = 'Source Deleted';

    globallink_send_download_confirmation($globallink->targetTicket, $pd4);
    globallink_webform_update_status($globallink, 'Source Deleted');
  }
  catch (SoapFault $se) {
    watchdog('GlobalLink', 'SOAP Exception - %function - Code[%faultcode], Message[%faultstring]', array('%function' => __FUNCTION__, '%faultcode' => $se->faultcode, '%faultstring' => $se->faultstring), WATCHDOG_ERROR);
    form_set_error('', t('Web Services Error: @faultcode - @faultstring', array('@faultcode' => $se->faultcode, '@faultstring' => $se->faultstring)));

    return FALSE;
  }
  catch (Exception $e) {
    watchdog('GlobalLink', 'Exception - %function - File[%file], Line[%line], Code[%code], Message[%message]', array('%function' => __FUNCTION__, '%file' => $e->getFile(), '%line' => $e->getLine(), '%code' => $e->getCode(), '%message' => $e->getMessage()), WATCHDOG_ERROR);
    form_set_error('', t('Error: @message', array('@message' => $e->getMessage())));

    return FALSE;
  }

  return TRUE;
}

/**
 * Gets number of translated webforms.
 *
 * @param string $pd4
 *   The project director details.
 * @param array $globallink_arr
 *   Array of GlobalLink objects.
 *
 * @return int
 *   The number of translated webforms.
 */
function globallink_webform_get_translated($pd4, &$globallink_arr) {
  $count = 0;

  foreach ($globallink_arr as $globallink) {
    $target_xml = globallink_download_target_resource($pd4, $globallink->targetTicket);

    if ($globallink->sourceDeleted) {
      continue;
    }

    if (!isset($target_xml)) {
      continue;
    }

    $count++;

    $language = globallink_get_drupal_locale_code($globallink->targetLocale);
    $translated_arr = globallink_webform_get_translated_items($target_xml);

    try {
      $lid = $translated_arr['lid'];

      foreach ($translated_arr as $attribute => $translations) {
        if ($attribute == 'lid') {
          continue;
        }

        $webform = '';

        if ($attribute == '#title' || $attribute == '#description') {
          $webform = globallink_load_source_data($translations['lid']);

          if ($webform == '') {
            throw new Exception('Source string not found for webform id ' . $lid . ' and field name ' . $attribute);
          }
        }

        $report = &drupal_static(__FUNCTION__, array(
          'additions' => 0,
          'updates' => 0,
          'deletes' => 0,
          'skips' => 0,
        ));

        _locale_import_one_string_db($report, $language, $webform[0]->context, $webform[0]->source, $translations['translation'], 'webform', $translations['location'], LOCALE_IMPORT_OVERWRITE);
      }

      if ($globallink->status != 'Error') {
        if ($globallink->status != 'Node Deleted') {
          globallink_send_download_confirmation($globallink->targetTicket, $pd4);
        }

        globallink_webform_update_status($globallink);
      }
      else {
        $count--;
      }
    }
    catch (SoapFault $se) {
      $count--;

      globallink_webform_update_status($globallink, 'Error');
      form_set_error('', t('Web Services Error: @faultcode - @faultstring', array('@faultcode' => $se->faultcode, '@faultstring' => $se->faultstring)));
    }
    catch (Exception $ex) {
      $count--;

      globallink_webform_update_status($globallink, 'Error');
      form_set_error('', t('Error: @message', array('@message' => $ex->getMessage())));
    }
  }

  return $count;
}

/**
 * Gets webform filters.
 *
 * @return array
 *   Associative array of webform filters.
 */
function globallink_webform_get_translate_filter_query() {
  $filter = array();

  if (isset($_SESSION['globallink_webform_filter'])) {
    foreach ($_SESSION['globallink_webform_filter'] as $key => $value) {
      $filter[$key] = $value;
    }
  }

  return $filter;
}

/**
 * Gets webform submission status.
 */
function globallink_webform_get_submission_status($lid, $locale_code) {
  $result = db_select('globallink_core_webform', 'tcw')
  ->fields('tcw', array('status'))
  ->condition('object_id', $lid, '=')
  ->condition('target', $locale_code, '=')
  ->execute();

  while ($record = $result->fetchAssoc()) {
    return $record;
  }
}

/**
 * Gets status for each language to be translated into.
 *
 * @param string $lid
 *   The webform LID.
 *
 * @return string
 *   String of languages and their respective statuses.
 */
function globallink_webform_locale_languages($lid) {
  $mapped_locales = globallink_get_mapped_locales_with_drupal_desc(TRUE);
  $translation_status = '';

  foreach ($mapped_locales as $code => $locale) {
    $result = globallink_webform_get_submission_status($lid, $code);

    if ($result) {
      $translation_status .= $locale . ' - ' . $result['status'] . '<br />';
    }
  }

  return $translation_status;
}

/**
 * Gets sent webform rows by LID.
 *
 * @param string $lid
 *   The webform LID.
 *
 * @return array
 *   Array of webform rows.
 */
function globallink_webform_get_sent_tpt_rows_by_lid($lid) {
  $result = db_select('globallink_core_webform', 'tcw')
  ->fields('tcw')
  ->condition('object_id', $lid, '=')
  ->condition('status', array('Sent for Translations', 'Error'), 'IN')
  ->execute();

  $rows = array();

  foreach ($result as $row) {
    $rows[] = $row;
  }

  return $rows;
}

/**
 * Updates webform status.
 *
 * @param object $globallink
 *   GlobalLink object.
 * @param string $status
 *   The new status.  Defaults to 'Pending Translations.'
 */
function globallink_webform_update_status(&$globallink, $status = 'Pending Translations') {
  db_update('globallink_core_webform')
  ->fields(array('status' => $status, 'timestamp' => REQUEST_TIME))
  ->condition('submission', $globallink->submissionName, '=')
  ->condition('document_ticket', $globallink->documentTicket, '=')
  ->condition('source', $globallink->sourceLocale, '=')
  ->condition('target', $globallink->targetLocale, '=')
  ->execute();
}

/**
 * Retrieves webform's row by ID.
 *
 * @param string $object_id
 *   The object ID.
 * @param string $object_type
 *   The object type.
 * @param string $source
 *   The target source.
 * @param string $target
 *   The target.
 *
 * @return
 *   The row if webform exists.
 */
function globallink_webform_get_row($object_id, $object_type, $source, $target) {
  $result = db_select('globallink_core_webform', 'tcw')
  ->fields('tcw')
  ->condition('object_id', $object_id, '=')
  ->condition('object_type', $object_type, '=')
  ->condition('source', $source, '=')
  ->condition('target', $target, '=')
  ->execute();

  foreach ($result as $row) {
    return $row;
  }

  return FALSE;
}

/**
 * Gets active menu submission names.
 *
 * @return array
 *   Array of active menu submission names.
 */
function globallink_webform_get_active_submission_names() {
  $query = db_select('globallink_core_webform', 'tcw');
  $query->condition('status', array('Sent for Translations', 'Error'), 'IN');
  $query->distinct();
  $query->fields('tcw');
  $results = $query->execute();

  $arr = array('' => '-- Select a Submission --');

  foreach ($results as $row) {
    $arr[$row->submission_ticket] = $row->submission;
  }

  return $arr;
}

/**
 * Gets webform submission name.
 *
 * @param string $submission_ticket
 *   The submission ticket.
 *
 * @return string
 *   The webform submission name.
 */
function globallink_webform_get_submission_name($submission_ticket) {
  $query = db_select('globallink_core_webform', 'tcw');
  $query->fields('tcw');
  $query->condition('submission_ticket', $submission_ticket, '=');
  $results = $query->execute();

  foreach ($results as $row) {
    if ($row->submission != '') {
      return $row->submission;
    }
  }
}

/**
 * Cancels webform submission.
 *
 * @param string $selected_submission
 *   The name of the submission.
 */
function globallink_webform_cancel_submission($selected_submission) {
  $pd4 = globallink_get_project_director_details();

  $globallink = new GlobalLink();
  $submission_name = globallink_webform_get_submission_name($selected_submission);
  $globallink->submissionName = $submission_name;
  $globallink->submissionTicket = $selected_submission;

  globallink_cancel_pd_submission($pd4, $globallink);
  globallink_webform_update_submission($globallink);
}

/**
 * Cancels webform records.
 *
 * @param array $rowids
 *   Array of webform IDs.
 * @param string $pd4
 *   The project director details.
 */
function globallink_webform_cancel_records($rowids, $pd4) {
  $globallink_arr = array();

  foreach ($rowids as $rid) {
    $row = globallink_webform_get_other_row($rid);

    $globallink = new GlobalLink();
    $globallink->tptRowId = $row->rid;
    $globallink->targetLocale = $row->target;
    $globallink->documentTicket = $row->document_ticket;
    $globallink->submissionTicket = $row->submission_ticket;
    $globallink_arr[$rid] = $globallink;
  }

  globallink_cancel_select_documents($pd4, $globallink_arr);
  globallink_webform_update_row_document($globallink_arr);
}

/**
 * Gets other webform row.
 *
 * @param int $row_id
 *   The id of the row.
 *
 * @return object
 *   Other webform row.
 */
function globallink_webform_get_other_row($row_id) {
  $result = db_select('globallink_core_webform', 'tcw')
  ->fields('tcw')
  ->condition('rid', $row_id, '=')
  ->execute();

  foreach ($result as $row) {
    return $row;
  }
}

/**
 * Updates webform submission.
 *
 * @param object $globallink
 *   GlobalLink object.
 */
function globallink_webform_update_submission(&$globallink) {
  db_update('globallink_core_webform')
  ->fields(array(
    'status' => 'Cancelled',
    'timestamp' => REQUEST_TIME,
  ))
  ->condition('submission_ticket', $globallink->submissionTicket, '=')
  ->condition('submission', $globallink->submissionName, '=')
  ->execute();
}

/**
 * Updates webform rows to reflect translation status.
 *
 * @param array $globallink_arr
 *   Array of GlobalLink objects.
 */
function globallink_webform_update_row_document(&$globallink_arr) {
  foreach ($globallink_arr as $globallink) {
    if ($globallink->cancelled) {
      db_update('globallink_core_webform')
      ->fields(array(
        'status' => 'Pending Translations',
        'timestamp' => REQUEST_TIME,
      ))
      ->condition('rid', $globallink->tptRowId, '=')
      ->execute();
    }
  }
}

/**
 * Gets distinct active webform submission names.
 *
 * @return array
 *   Array of distinct active webform submission names.
 */
function globallink_webform_get_distinct_active_submission_names() {
  $query = db_select('globallink_core_webform', 'tcw');
  $query->condition('status', array('Sent for Translations', 'Error'), 'IN');
  $query->distinct();
  $query->fields('tcw');
  $results = $query->execute();

  $arr = array('[any]' => t('Any'));

  foreach ($results as $row) {
    $arr[$row->submission] = $row->submission;
  }

  return $arr;
}

/**
 * Gets translated webform items from XML data.
 *
 * @param object $xml
 *   XML representation of webform items.
 *
 * @return array
 *   Array of webform items.
 */
function globallink_webform_get_translated_items($xml) {
  if (is_null($xml) || !is_string($xml) || $xml == '') {
    return array();
  }

  $dom = new DomDocument;
  $dom->preserveWhiteSpace = FALSE;
  $dom->loadXML($xml);

  $contents = $dom->getElementsByTagName('content');

  $lid = '';

  foreach ($contents as $content) {
    if (!is_null($content->attributes)) {
      foreach ($content->attributes as $attr_name => $attr_node) {
        if ($attr_name == 'lid') {
          $lid = $attr_node->value;
        }
      }
    }
  }

  if ($lid == '') {
    return array();
  }

  $webform_arr = array();
  $webform_arr['lid'] = $lid;

  $webforms = $dom->getElementsByTagName('webform');

  foreach ($webforms as $webform) {
    if (!is_null($webform->attributes)) {
      $w_arr = array();

      foreach ($webform->attributes as $attr_name => $attr_node) {
        $w_arr[$attr_name] = $attr_node->value;
      }

      $w_arr['translation'] = $webform->nodeValue;
      $webform_arr[$w_arr['name']] = $w_arr;
    }
  }

  return $webform_arr;
}

/**
 * Gets webform translation status.
 *
 * @param string $lid
 *   The webform LID.
 * @param string $tgt_locale
 *   The target locale.
 * @param string $title
 *   The translation title.
 *
 * @return string
 *   Status message.
 */
function globallink_webform_get_translation_status($lid, $tgt_locale, &$title) {
  $query = db_select('globallink_core_webform', 'tcw');
  $query->condition('object_id', $lid, '=');
  $query->condition('target', $tgt_locale, '=');
  $query->distinct();
  $query->fields('tcw');
  $results = $query->execute();

  foreach ($results as $row) {
    $title = $row->title;
    $webform = globallink_load_source_data($lid);

    if (!$webform || is_null($webform)) {
      return 'Source Deleted';
    }
    elseif ($row->status == 'Sent for Translations') {
      return 'Translation Completed';
    }
    elseif ($row->status == 'Error') {
      return 'Error';
    }
  }

  return 'Sent for Translations';
}

/**
 * Gets sent webform rows by LID target.
 *
 * @param string $lid
 *   The webform LID.
 * @param string $target
 *   The LID target.
 *
 * @return array
 *   Array of sent webform rows.
 */
function globallink_webform_get_sent_tpt_rows_by_lid_target($lid, $target) {
  $result = db_select('globallink_core_webform', 'tcw')
  ->fields('tcw')
  ->condition('object_id', $lid, '=')
  ->condition('target', $target, '=')
  ->condition('status', array('Sent for Translations', 'Error'), 'IN')
  ->execute();

  $rows = array();

  foreach ($result as $row) {
    $rows[] = $row;
  }

  return $rows;
}

/**
 * Gets webform row ID from submission details.
 *
 * @param string $submission_ticket
 *   The submission ticket.
 * @param string $document_ticket
 *   The document ticket.
 * @param string $target_locale
 *   The target locale.
 *
 * @return string
 *   The webform row ID.  FALSE if the row doesn't exist.
 */
function globallink_webform_get_row_id_from_submission($submission_ticket, $document_ticket, $target_locale) {
  $query = db_select('globallink_core_webform', 'tc');
  $query->condition('submission_ticket', $submission_ticket, '=');
  $query->condition('document_ticket', $document_ticket, '=');
  $query->condition('target', $target_locale, '=');
  $query->fields('tc');
  $results = $query->execute();

  foreach ($results as $row) {
    return $row->rid;
  }
}

/**
 * Gets active webform submission rows by LID.
 *
 * @param string $lids
 *   The webform LID.
 *
 * @return
 *   Associative array of webform active submission rows.  FALSE if the array is empty.
 */
function globallink_webform_get_active_submission_rows_by_lid($lids) {
  $query = db_select('globallink_core_webform', 'tci');
  $query->condition('status', 'Sent for Translations', '=');
  $query->condition('object_id', $lids, 'IN');
  $query->fields('tci');
  $results = $query->execute();

  $arr = array();

  foreach ($results as $row) {
    if (array_key_exists($row->object_id, $arr)) {
      array_push($arr[$row->object_id], $row);
    }
    else {
      $arr[$row->object_id] = array($row);
    }
  }

  $final_arr = array();

  foreach ($arr as $lid => $lid_arr) {
    $sub_arr = array();

    foreach ($lid_arr as $r) {
      if (array_key_exists($r->submission, $sub_arr)) {
        array_push($sub_arr[$r->submission], $r->target);
      }
      else {
        $sub_arr[$r->submission] = array($r->target);
      }
    }

    if (count($sub_arr) > 0) {
      $final_arr[$lid] = $sub_arr;
    }
  }

  if (count($final_arr) > 0) {
    return $final_arr;
  }

  return FALSE;
}

/**
 * Updates webform status.
 *
 * @param object $globallink
 *   GlobalLink object.
 * @param string $status
 *   The new status.  Defaults to 'Cancelled.'
 */
function globallink_webform_update_submission_status($submission_ticket, $status = 'Cancelled') {
  db_update('globallink_core_webform')
  ->fields(array(
    'status' => $status,
    'timestamp' => REQUEST_TIME,
  ))
  ->condition('submission_ticket', $submission_ticket, '=')
  ->execute();
}

/**
 * Gets webform project director submission status.
 */
function globallink_webform_get_pd_submission_status() {
  module_load_include('inc', 'globallink', 'globallink_settings');
  module_load_include('inc', 'globallink', 'gl_ws/gl_ws_common');

  $query = db_select('globallink_core_webform', 'tc');
  $query->fields('tc', array('submission_ticket'));
  $query->distinct();
  $query->condition('status', 'Sent for Translations', '=');
  $results = $query->execute();

  foreach ($results as $row) {
    if ($row->submission_ticket) {
      try {
        $pd4 = globallink_get_project_director_details();

        $doc_status = globallink_get_status($pd4, $row->submission_ticket);

        if (!$doc_status || $doc_status == 'CANCELLED') {
          globallink_webform_update_submission_status($row->submission_ticket);
        }
      }
      catch (SoapFault $se) {
        globallink_webform_update_submission_status($row->submission_ticket);
      }
      catch (Exception $ex) {
        globallink_webform_update_submission_status($row->submission_ticket);
      }
    }
  }
}

/**
 * Checks webform status based on row IDs.
 *
 * @param array $rids_arr
 *   Array of row IDs.
 *
 * @return array
 *   Array of row IDs that have been sent for translation or threw an error.
 */
function globallink_webform_check_status($rids_arr) {
  $status = TRUE;
  $query = db_select('globallink_core_webform', 'tc')
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
 * Clears cancelled webform documents.
 *
 * @return int
 *   Number of cancelled webform documents.
 */
function globallink_webform_clear_cancelled_documents() {
  $count = 0;

  $query = db_select('globallink_core_webform', 'tc')
  ->fields('tc', array('submission_ticket'))
  ->distinct()
  ->condition('status', 'Cancelled', '=');

  $results = $query->execute();

  foreach ($results as $item) {
    globallink_webform_update_submission_status($item->submission_ticket, 'Pending Translations');

    $count++;
  }

  return $count;
}
