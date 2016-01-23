<?php

/**
 * Sends interfaces for translation.
 *
 * @param array $lids
 *   The array of interface LIDs.
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
function globallink_interface_send_for_translations($lids, $pd4, $submission_name, $due_date, $project_code, $source_locale, $target_locale_arr, $submission_details) {
  module_load_include('inc', 'globallink', 'gl_ws/gl_ws_send_translations');

  $source_arr = globallink_get_source('default');
  $submitter = $submission_details['submitter'];
  $globallink_arr = array();

  foreach ($lids as $lid) {
    $rows = globallink_interface_get_sent_rows_by_lid($lid);
    $target_arr = $target_locale_arr;

    foreach ($rows as $row) {
      if (array_search($row->target, $target_locale_arr)) {
        unset($target_arr[$row->target]);
      }
    }

    if (empty($target_arr)) {
      continue;
    }

    $xml = globallink_interface_get_xml($lid, $source_arr[$lid]);
    $name = 'Interface_' . $lid . '.xml';
    watchdog('GlobalLink', 'XML - %xml', array('%xml' => $xml), WATCHDOG_DEBUG);

    $globallink = new GlobalLink();
    $globallink->type = 'interface';
    $globallink->metadata = 'interface';
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
 * Gets XML data from specific interface.
 *
 * @param string $lid
 *   The interface LID.
 * @param array $source_arr
 *   The source array.
 *
 * @return array
 *   Associative array of block XML data.
 */
function globallink_interface_get_xml($lid, $source_arr) {
  $dom = new DOMDocument('1.0', 'UTF-8');
  $dom->formatOutput = TRUE;

  $root = $dom->createElement('content');
  $dom->appendChild($root);

  $id = $dom->createAttribute('lid');
  $id->value = $lid;
  $root->appendChild($id);

  globallink_insert_child_element($dom, $root, 'interface', $source_arr['source'], array(
    'name' => 'source',
    'lid' => $lid,
    'location' => $source_arr['location'],
  ));

  $xml = $dom->saveXML();

  return $xml;
}

/**
 * Updates interface ticket ID.
 *
 * @param array $arr
 *   Array of GlobalLink objects.
 * @param string $project_code
 *   The interface's project code.
 */
function globallink_interface_update_ticket_id($arr, $project_code) {
  foreach ($arr as $globallink) {
    $target_locale_arr = $globallink->targetLocale;
    $type = $globallink->type;

    if ($type != 'interface') {
      continue;
    }

    $lid = $globallink->otherObjectId;
    $interface = globallink_load_source_data($lid);

    foreach ($target_locale_arr as $target_locale) {
      $row = globallink_interface_get_row($lid, $type, $globallink->sourceLocale, $target_locale);

      if ($row) {
        db_update('globallink_core_interface')
        ->fields(array(
          'title' => $interface[0]->source,
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
        db_insert('globallink_core_interface')
        ->fields(array(
          'object_id' => $lid,
          'object_type' => $globallink->type,
          'title' => $interface[0]->source,
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
 * Gets status for each language to be translated into.
 *
 * @param string $lid
 *   The interface LID.
 *
 * @return string
 *   String of languages and their respective statuses.
 */
function globallink_interface_locale_languages($lid) {
  $mapped_locales = globallink_get_mapped_locales_with_drupal_desc(TRUE);
  $translation_status = '';

  foreach ($mapped_locales as $code => $locale) {
    $result = globallink_interface_get_submission_status($lid, $code);

    if ($result) {
      $translation_status .= $locale . ' - ' . $result['status'] . '<br />';
    }
  }

  return $translation_status;
}

/**
 * Gets interface submission status.
 */
function globallink_interface_get_submission_status($lid, $locale_code) {
  $result = db_select('globallink_core_interface', 'tci')
  ->fields('tci', array('status'))
  ->condition('object_id', $lid, '=')
  ->condition('target', $locale_code, '=')
  ->execute();

  while ($record = $result->fetchAssoc()) {
    return $record;
  }
}

/**
 * Gets interface filters.
 *
 * @return array
 *   Associative array of interface filters.
 */
function globallink_interface_get_translate_filter_query() {
  $filter = array();

  if (isset($_SESSION['globallink_interface_filter'])) {
    foreach ($_SESSION['globallink_interface_filter'] as $key => $value) {
      $filter[$key] = $value;
    }
  }

  return $filter;
}

/**
 * Gets sent interface rows by LID.
 *
 * @param string $lid
 *   The interface LID.
 *
 * @return array
 *   Array of interface rows.
 */
function globallink_interface_get_sent_rows_by_lid($lid) {
  $result = db_select('globallink_core_interface', 'tci')
  ->fields('tci')
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
 * Gets interface translation status.
 *
 * @param string $lid
 *   The interface LID.
 * @param string $tgt_locale
 *   The target locale.
 * @param string $title
 *   The translation title.
 *
 * @return string
 *   Status message.
 */
function globallink_interface_get_translation_status($lid, $tgt_locale, &$title) {
  $query = db_select('globallink_core_interface', 'tco');
  $query->condition('object_id', $lid, '=');
  $query->condition('target', $tgt_locale, '=');
  $query->distinct();
  $query->fields('tco');

  $results = $query->execute();

  foreach ($results as $row) {
    $title = $row->title;
    $interface = globallink_load_source_data($lid);

    if (!$interface || is_null($interface)) {
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
 * Updates status for deleted interfaces.
 *
 * @param string $pd4
 *   The project director details.
 * @param object $globallink
 *   GlobalLink object.
 *
 * @return bool
 *   TRUE if update was successful, FALSE on error.
 */
function globallink_interface_update_deleted_records($pd4, $globallink) {
  try {
    $globallink->status = 'Source Deleted';

    globallink_send_download_confirmation($globallink->targetTicket, $pd4);
    globallink_interface_update_status($globallink, 'Source Deleted');
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
 * Gets number of translated interfaces.
 *
 * @param string $pd4
 *   The project director details.
 * @param array $globallink_arr
 *   Array of GlobalLink objects.
 *
 * @return int
 *   The number of translated interfaces.
 */
function globallink_interface_get_translated($pd4, &$globallink_arr) {
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
    $translated_arr = globallink_interface_get_translated_items($target_xml);

    try {
      $lid = $translated_arr['lid'];

      foreach ($translated_arr as $attribute => $translations) {
        if ($attribute == 'lid') {
          continue;
        }

        $interface = '';

        if ($attribute == 'source') {
          $interface = globallink_load_source_data($translations['lid']);

          if ($interface == '') {
            throw new Exception('Source string not found for interface id ' . $lid . ' and field name ' . $attribute);
          }
        }

        $report = &drupal_static(__FUNCTION__, array(
          'additions' => 0,
          'updates' => 0,
          'deletes' => 0,
          'skips' => 0,
        ));
        _locale_import_one_string_db($report, $language, '', $interface[0]->source, $translations['translation'], 'default', $translations['location'], LOCALE_IMPORT_OVERWRITE);
      }

      if ($globallink->status != 'Error') {
        if ($globallink->status != 'Node Deleted') {
          globallink_send_download_confirmation($globallink->targetTicket, $pd4);
        }

        globallink_interface_update_status($globallink);
      }
      else {
        $count--;
      }
    }
    catch (SoapFault $se) {
      $count--;

      globallink_interface_update_status($globallink, 'Error');
      form_set_error('', t('Web Services Error: @faultcode - @faultstring', array('@faultcode' => $se->faultcode, '@faultstring' => $se->faultstring)));
    }
    catch (Exception $ex) {
      $count--;

      globallink_interface_update_status($globallink, 'Error');
      form_set_error('', t('Error: @message', array('@message' => $ex->getMessage())));
    }
  }

  return $count;
}

/**
 * Updates interface status.
 *
 * @param object $globallink
 *   GlobalLink object.
 * @param string $status
 *   The new status.  Defaults to 'Pending Translations.'
 */
function globallink_interface_update_status(&$globallink, $status = 'Pending Translations') {
  db_update('globallink_core_interface')
  ->fields(array('status' => $status, 'timestamp' => REQUEST_TIME))
  ->condition('submission', $globallink->submissionName, '=')
  ->condition('document_ticket', $globallink->documentTicket, '=')
  ->condition('source', $globallink->sourceLocale, '=')
  ->condition('target', $globallink->targetLocale, '=')
  ->execute();
}

/**
 * Retrieves interface's row.
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
 *   The row if interface exists.  FALSE if not.
 */
function globallink_interface_get_row($object_id, $object_type, $source, $target) {
  $result = db_select('globallink_core_interface', 'tci')
  ->fields('tci')
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
 * Gets active interface submission names.
 *
 * @return array
 *   Array of active interface submission names.
 */
function globallink_interface_get_active_submission_names() {
  $query = db_select('globallink_core_interface', 'tco');
  $query->condition('status', array('Sent for Translations', 'Error'), 'IN');
  $query->distinct();
  $query->fields('tco');

  $results = $query->execute();

  $arr = array('' => '-- Select a Submission --');

  foreach ($results as $row) {
    $arr[$row->submission_ticket] = $row->submission;
  }

  return $arr;
}

/**
 * Cancels interface records.
 *
 * @param array $rowids
 *   Array of interface IDs.
 * @param string $pd4
 *   The project director details.
 */
function globallink_interface_cancel_records($rowids, $pd4) {
  $globallink_arr = array();

  foreach ($rowids as $rid) {
    $row = globallink_get_other_row($rid, 'interface');

    $globallink = new GlobalLink();
    $globallink->tptRowId = $row->rid;
    $globallink->targetLocale = $row->target;
    $globallink->documentTicket = $row->document_ticket;
    $globallink->submissionTicket = $row->submission_ticket;
    $globallink_arr[$rid] = $globallink;
  }

  globallink_cancel_select_documents($pd4, $globallink_arr);
  globallink_interface_update_row_document($globallink_arr);
}

/**
 * Updates interface rows to reflect translation status.
 *
 * @param array $globallink_arr
 *   Array of GlobalLink objects.
 */
function globallink_interface_update_row_document(&$globallink_arr) {
  foreach ($globallink_arr as $globallink) {
    if (!$globallink->cancelled) {
      continue;
    }

    db_update('globallink_core_interface')
    ->fields(array(
      'status' => 'Pending Translations',
      'timestamp' => REQUEST_TIME,
    ))
    ->condition('rid', $globallink->tptRowId, '=')
    ->execute();
  }
}

/**
 * Cancels interface submission.
 *
 * @param string $selected_submission
 *   The name of the submission.
 */
function globallink_interface_cancel_submission($selected_submission) {
  $pd4 = globallink_get_project_director_details();

  $globallink = new GlobalLink();
  $submission_name = globallink_interface_get_submission_name($selected_submission);
  $globallink->submissionName = $submission_name;
  $globallink->submissionTicket = $selected_submission;

  globallink_cancel_pd_submission($pd4, $globallink);
  globallink_interface_update_submission($globallink);
}

/**
 * Gets interface submission name.
 *
 * @param string $submission_ticket
 *   The submission ticket.
 *
 * @return string
 *   The interface submission name.
 */
function globallink_interface_get_submission_name($submission_ticket) {
  $query = db_select('globallink_core_interface', 'tc');
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
 * Updates interface submission.
 *
 * @param object $globallink
 *   GlobalLink object.
 */
function globallink_interface_update_submission(&$globallink) {
  db_update('globallink_core_interface')
  ->fields(array(
    'status' => 'Cancelled',
    'timestamp' => REQUEST_TIME,
  ))
  ->condition('submission_ticket', $globallink->submissionTicket, '=')
  ->condition('submission', $globallink->submissionName, '=')
  ->execute();
}

/**
 * Gets translated interfaces from XML data.
 *
 * @param object $xml
 *   XML representation of interfaces.
 *
 * @return array
 *   Array of interfaces.
 */
function globallink_interface_get_translated_items($xml) {
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

  $interface_arr = array();
  $interface_arr['lid'] = $lid;

  $interfaces = $dom->getElementsByTagName('interface');

  foreach ($interfaces as $interface) {
    if (!is_null($interface->attributes)) {
      $i_arr = array();

      foreach ($interface->attributes as $attr_name => $attr_node) {
        $i_arr[$attr_name] = $attr_node->value;
      }

      $i_arr['translation'] = $interface->nodeValue;
      $interface_arr[$i_arr['name']] = $i_arr;
    }
  }

  return $interface_arr;
}

/**
 * Gets interface row ID from submission details.
 *
 * @param string $submission_ticket
 *   The submission ticket.
 * @param string $document_ticket
 *   The document ticket.
 * @param string $target_locale
 *   The target locale.
 *
 * @return string
 *   The interface row ID.
 */
function globallink_interface_get_row_id_from_submission($submission_ticket, $document_ticket, $target_locale) {
  $result = db_select('globallink_core_interface', 'tc')
  ->condition('submission_ticket', $submission_ticket, '=')
  ->condition('document_ticket', $document_ticket, '=')
  ->condition('target', $target_locale, '=')
  ->fields('tc')
  ->execute();

  foreach ($result as $row) {
    return $row->rid;
  }
}

/**
 * Gets active interface submission rows by LID.
 *
 * @param string $nids
 *   The array of interface LIDs.
 *
 * @return
 *   Associative array of interface active submission rows.  FALSE if the array is empty.
 */
function globallink_interface_get_active_submission_rows_by_lid($lids) {
  if (is_array($lids) && empty($lids)) {
    return FALSE;
  }

  $query = db_select('globallink_core_interface', 'tci');
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
 * Gets distinct active interface submission names.
 *
 * @return array
 *   Array of distinct active interface submission names.
 */
function globallink_interface_get_distinct_active_submission_names() {
  $query = db_select('globallink_core_interface', 'tc');
  $query->condition('status', array('Sent for Translations', 'Error'), 'IN');
  $query->distinct();
  $query->fields('tc');

  $results = $query->execute();

  $arr = array('[any]' => t('Any'));

  foreach ($results as $row) {
    $arr[$row->submission] = $row->submission;
  }

  return $arr;
}

/**
 * Updates interface submission status.
 *
 * @param string $submission_ticket
 *   The submission ticket.
 * @param string $status
 *   The submission status.  Defaults to 'Cancelled.'
 */
function globallink_interface_update_submission_status($submission_ticket, $status = 'Cancelled') {
  db_update('globallink_core_interface')
  ->fields(array(
    'status' => $status,
    'timestamp' => REQUEST_TIME,
  ))
  ->condition('submission_ticket', $submission_ticket, '=')
  ->execute();
}

/**
 * Gets interface project director submission status.
 */
function globallink_interface_get_pd_submission_status() {
  module_load_include('inc', 'globallink', 'globallink_settings');
  module_load_include('inc', 'globallink', 'gl_ws/gl_ws_common');

  $query = db_select('globallink_core_interface', 'tc');
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
          globallink_interface_update_submission_status($row->submission_ticket);
        }
      }
      catch (SoapFault $se) {
        globallink_interface_update_submission_status($row->submission_ticket);
      }
      catch (Exception $ex) {
        globallink_interface_update_submission_status($row->submission_ticket);
      }
    }
  }
}

/**
 * Checks interface status based on row IDs.
 *
 * @param array $rids_arr
 *   Array of row IDs.
 *
 * @return array
 *   Array of row IDs that have been sent for translation or threw an error.
 */
function globallink_interface_check_status($rids_arr) {
  $status = TRUE;

  $query = db_select('globallink_core_interface', 'tci')
  ->fields('tci', array('rid'))
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
 * Clears cancelled interface documents.
 *
 * @return int
 *   Number of cancelled interface documents.
 */
function globallink_interface_clear_cancelled_documents() {
  $count = 0;

  $query = db_select('globallink_core_interface', 'tci')
  ->fields('tci', array('submission_ticket'))
  ->distinct()
  ->condition('status', 'Cancelled', '=');

  $results = $query->execute();

  foreach ($results as $item) {
    globallink_interface_update_submission_status($item->submission_ticket, 'Pending Translations');

    $count++;
  }

  return $count;
}
