<?php

/**
 * Sends fieldable panels for translation.
 *
 * @param array $fpids
 *   The array of fieldable panels IDs.
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
function globallink_fieldable_panels_pane_send_for_translations($fpids, $pd4, $submission_name, $due_date, $project_code, $source_locale, $target_locale_arr, $submission_details) {
  module_load_include('inc', 'globallink', 'gl_ws/gl_ws_send_translations');

  $submitter = $submission_details['submitter'];
  $globallink_arr = array();

  foreach ($fpids as $fpid) {
    $rows = globallink_fieldable_panels_get_sent_rows_by_fpid($fpid);
    $target_arr = $target_locale_arr;

    foreach ($rows as $row) {
      if (array_search($row->target, $target_locale_arr)) {
        unset($target_arr[$row->target]);
      }
    }

    if (empty($target_arr)) {
      continue;
    }

    $xml = globallink_fieldable_panels_pane_get_xml($fpid);
    $name = 'Fieldable_Panels_Panes_' . $fpid . '.xml';
    watchdog('GlobalLink', 'XML - %xml', array('%xml' => $xml), WATCHDOG_DEBUG);

    $globallink = new GlobalLink();
    $globallink->type = 'fieldable_panels_panes';
    $globallink->metadata = 'fieldable_panels_panes';
    $globallink->sourceLocale = $source_locale;
    $globallink->targetLocale = $target_arr;
    $globallink->sourceXML = $xml;
    $globallink->sourceFileName = $name;
    $globallink->submissionName = $submission_name;
    $globallink->dueDate = $due_date;
    $globallink->otherObjectId = $fpid;
    $globallink->submissionInstructions = $submission_details['instructions'] . "\nSubmitter: " . $submitter;
    $globallink_arr[] = $globallink;
  }

  if (!empty($globallink_arr)) {
    globallink_send_documents_for_translation_to_pd($globallink_arr, $pd4, $project_code, $submitter);
  }

  return $globallink_arr;
}

/**
 * Gets XML data from specific fieldable panel.
 *
 * @param string $fpid
 *   The fieldable panel ID.
 *
 * @return array
 *   Associative array of fieldable panel XML data.
 */
function globallink_fieldable_panels_pane_get_xml($fpid) {
  $dom = new DOMDocument('1.0', 'UTF-8');
  $dom->formatOutput = TRUE;

  $root = $dom->createElement('content');
  $dom->appendChild($root);

  $id = $dom->createAttribute('fpid');
  $id->value = $fpid;
  $root->appendChild($id);

  $fieldable_panels_pane = fieldable_panels_panes_load($fpid);
  $language = $fieldable_panels_pane->language;

  $fields = globallink_fieldable_panels_pane_get_fields($fieldable_panels_pane->bundle);

  foreach ($fields as $field) {
    if (empty($fieldable_panels_pane->$field)) {
      continue;
    }

    $field_values = $fieldable_panels_pane->$field;

    if (is_array($field_values) && !empty($field_values[$language])) {
      foreach ($field_values[$language] as $delta => $values) {
        globallink_insert_child_element($dom, $root, 'field', $values['value'], array('field' => $field, 'delta' => $delta, 'langcode' => $language));
      }
    }
    elseif (!empty($field_values) && is_string($field_values)) { // Test this without field translation
      globallink_insert_child_element($dom, $root, 'field', $field_values, array('field' => $field));
    }
  }

  $xml = $dom->saveXML();

  return $xml;
}

/**
 * Cancels fieldable panels records.
 *
 * @param array $rowids
 *   Array of fieldable panels IDs.
 * @param string $pd4
 *   The project director details.
 */
function globallink_fieldable_panels_cancel_records($rowids, $pd4) {
  $globallink_arr = array();

  foreach ($rowids as $rid) {
    $row = globallink_fieldable_panels_get_row_by_id($rid);
    $globallink = new GlobalLink();
    $globallink->tptRowId = $row->id;
    $globallink->targetLocale = $row->target;
    $globallink->documentTicket = $row->document_ticket;
    $globallink->submissionTicket = $row->submission_ticket;
    $globallink_arr[$rid] = $globallink;
  }

  globallink_cancel_select_documents($pd4, $globallink_arr);
  globallink_fieldable_panels_update_row_document($globallink_arr);
}

/**
 * Updates fieldable panels rows to reflect translation status.
 *
 * @param array $globallink_arr
 *   Array of GlobalLink objects.
 */
function globallink_fieldable_panels_update_row_document(&$globallink_arr) {
  foreach ($globallink_arr as $globallink) {
    if ($globallink->cancelled) {
      db_update('globallink_core_fieldable_panels')
      ->fields(array(
        'status' => 'Pending Translations',
        'timestamp' => REQUEST_TIME,
      ))
      ->condition('id', $globallink->tptRowId, '=')
      ->execute();
    }
  }
}

/**
 * Cancels fieldable panels submission.
 *
 * @param string $selected_submission
 *   The name of the submission.
 */
function globallink_fieldable_panels_cancel_submission($selected_submission) {
  $pd4 = globallink_get_project_director_details();
  $globallink = new GlobalLink();
  $submission_name = globallink_fieldable_panels_get_submission_name($selected_submission);
  $globallink->submissionName = $submission_name;
  $globallink->submissionTicket = $selected_submission;

  globallink_cancel_pd_submission($pd4, $globallink);
  globallink_fieldable_panels_update_submission($globallink);
}

/**
 * Checks fieldable panel status based on row IDs.
 *
 * @param array $rids_arr
 *   Array of row IDs.
 *
 * @return array
 *   Array of row IDs that have been sent for translation or threw an error.
 */
function globallink_fieldable_panels_check_status($ids_arr) {
  $status = TRUE;

  $query = db_select('globallink_core_fieldable_panels', 'tcfp')
  ->fields('tcfp', array('id'))
  ->condition('status', array('Sent for Translations', 'Error'), 'IN');

  $results = $query->execute();
  $rows = array();

  foreach ($results as $item) {
    $rows[$item->id] = $item->id;
  }

  foreach ($ids_arr as $val) {
    if (!in_array($val, $rows)) {
      unset($ids_arr[$val]);
      $status = FALSE;
    }
  }

  if (!$status) {
    drupal_set_message(t('Cannot cancel documents that have been cancelled in Globallink.'), 'warning', NULL);
  }

  return $ids_arr;
}

/**
 * Clears cancelled fieldable panels documents.
 *
 * @return int
 *   Number of cancelled fieldable panels documents.
 */
function globallink_fieldable_panels_clear_cancelled_documents() {
  $count = 0;

  $query = db_select('globallink_core_fieldable_panels', 'tcfp')
  ->fields('tcfp', array('submission_ticket'))
  ->distinct()
  ->condition('status', 'Cancelled', '=');

  $results = $query->execute();

  foreach ($results as $item) {
    globallink_fieldable_panels_update_submission_status($item->submission_ticket, 'Pending Translations');

    $count++;
  }

  return $count;
}

/**
 * Gets fieldable panels submission name.
 *
 * @param string $submission_ticket
 *   The submission ticket.
 *
 * @return string
 *   The fieldable panels submission name.
 */
function globallink_fieldable_panels_get_submission_name($submission_ticket) {
  $query = db_select('globallink_core_fieldable_panels', 'tcfp');
  $query->fields('tcfp');
  $query->condition('submission_ticket', $submission_ticket, '=');
  $results = $query->execute();

  foreach ($results as $row) {
    if ($row->submission != '') {
      return $row->submission;
    }
  }
}

/**
 * Updates fieldable panels submission.
 *
 * @param object $globallink
 *   GlobalLink object.
 */
function globallink_fieldable_panels_update_submission(&$globallink) {
  db_update('globallink_core_fieldable_panels')
  ->fields(array(
    'status' => 'Cancelled',
    'timestamp' => REQUEST_TIME,
  ))
  ->condition('submission_ticket', $globallink->submissionTicket, '=')
  ->condition('submission', $globallink->submissionName, '=')
  ->execute();
}

/**
 * Gets fieldable panels fields based on bundle.
 *
 * @param string $bundle
 *   The bundle.
 *
 * @return array
 *   Array of fields.
 */
function globallink_fieldable_panels_pane_get_fields($bundle) {
  $fields = array();

  foreach (field_info_instances('fieldable_panels_pane', $bundle) as $field) {
    $fields[] = $field['field_name'];
  }

  return $fields;
}

/**
 * Gets active fieldable panels submission names.
 *
 * @return array
 *   Array of active fieldable panels submission names.
 */
function globallink_fieldable_panels_get_active_submission_names() {
  $query = db_select('globallink_core_fieldable_panels', 'tcfp');
  $query->condition('status', array('Sent for Translations', 'Error'), 'IN');
  $query->distinct();
  $query->fields('tcfp');

  $results = $query->execute();
  $arr = array('' => '-- Select a Submission --');

  foreach ($results as $row) {
    $arr[$row->submission_ticket] = $row->submission;
  }

  return $arr;
}

/**
 * Gets active fieldable panels submission rows by node ID.
 *
 * @param string $fpids
 *   The array of fieldable panels IDs.
 *
 * @return
 *   Associative array of fieldable panels active submission rows.  FALSE if the array is empty.
 */
function globallink_fieldable_panels_get_active_submission_rows($fpid) {
  $query = db_select('globallink_core_fieldable_panels', 'tcfp');
  $query->condition('status', 'Sent for Translations', '=');
  $query->condition('fpid', $fpid, '=');
  $query->fields('tcfp');

  $results = $query->execute();
  $arr = array();

  foreach ($results as $row) {
    if (array_key_exists($row->fpid, $arr)) {
      array_push($arr[$row->fpid], $row);
    }
    else {
      $arr[$row->fpid] = array($row);
    }
  }

  $final_arr = array();

  foreach ($arr as $fpid => $mlid_arr) {
    $sub_arr = array();

    foreach ($mlid_arr as $r) {
      if (array_key_exists($r->submission, $sub_arr)) {
        array_push($sub_arr[$r->submission], $r->target);
      }
      else {
        $sub_arr[$r->submission] = array($r->target);
      }
    }

    if (count($sub_arr) > 0) {
      $final_arr[$fpid] = $sub_arr;
    }
  }

  if (count($final_arr) > 0) {
    return $final_arr;
  }

  return FALSE;
}

/**
 * Gets distinct active fieldable panels submission names.
 *
 * @return array
 *   Array of distinct active fieldable panels submission names.
 */
function globallink_fieldable_panels_get_distinct_active_submission_names() {
  $query = db_select('globallink_core_fieldable_panels', 'tcfp');
  $query->condition('status', array('Sent for Translations', 'Error'), 'IN');
  $query->distinct();
  $query->fields('tcfp');

  $results = $query->execute();
  $arr = array('[any]' => t('Any'));

  foreach ($results as $row) {
    $arr[$row->submission] = $row->submission;
  }

  return $arr;
}

/**
 * Gets fieldable panels submission status.
 */
function globallink_fieldable_panels_get_submission_status() {
  module_load_include('inc', 'globallink', 'globallink_settings');
  module_load_include('inc', 'globallink', 'gl_ws/gl_ws_common');

  $query = db_select('globallink_core_fieldable_panels', 'tcfp');
  $query->fields('tcfp', array('submission_ticket'));
  $query->distinct();
  $query->condition('status', 'Sent for Translations', '=');

  $results = $query->execute();

  foreach ($results as $row) {
    if ($row->submission_ticket) {
      try {
        $pd4 = globallink_get_project_director_details();

        $status = globallink_get_status($pd4, $row->submission_ticket);

        if (!$status || $status == 'CANCELLED') {
          globallink_fieldable_panels_update_submission_status($row->submission_ticket);
        }
      }
      catch (SoapFault $se) {
        globallink_fieldable_panels_update_submission_status($row->submission_ticket);
      }
      catch (Exception $ex) {
        globallink_fieldable_panels_update_submission_status($row->submission_ticket);
      }
    }
  }
}

/**
 * Gets sent fieldable panels row that matches criteria.
 *
 * @param string $submission_ticket
 *   The submission ticket.
 * @param string $document_ticket
 *   The document ticket.
 * @param string $target_locale
 *   The target locale.
 *
 * @return
 *   Fieldable panels object that matches criteria.  FALSE if none exists.
 */
function globallink_fieldable_panels_get_sent_tpt_rows($submission_ticket, $document_ticket, $target_locale) {
  $query = db_select('globallink_core_fieldable_panels', 'tcfp');
  $query->condition('submission_ticket', $submission_ticket, '=');
  $query->condition('document_ticket', $document_ticket, '=');
  $query->condition('target', $target_locale, '=');
  $query->fields('tcfp');

  $results = $query->execute();

  foreach ($results as $row) {
    return $row;
  }

  return FALSE;
}

/**
 * Gets fieldable panel translation status.
 *
 * @param string $fpid
 *   The fieldable panel ID.
 * @param string $tgt_locale
 *   The target locale.
 * @param string $title
 *   The translation title.
 *
 * @return string
 *   Status message.
 */
function globallink_fieldable_panels_get_translation_status($fpid, $tgt_locale, &$title) {
  $query = db_select('globallink_core_fieldable_panels', 'tcfp');
  $query->condition('fpid', $fpid, '=');
  $query->condition('target', $tgt_locale, '=');
  $query->distinct();
  $query->fields('tcfp');

  $results = $query->execute();

  foreach ($results as $row) {
    $title = $row->title;
    $entity = fieldable_panels_panes_load($fpid);

    if (empty($entity)) {
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
 * Gets fieldable panels row ID from submission details.
 *
 * @param string $submission_ticket
 *   The submission ticket.
 * @param string $document_ticket
 *   The document ticket.
 * @param string $target_locale
 *   The target locale.
 *
 * @return
 *   The fieldable panels row ID.  FALSE if the row doesn't exist.
 */
function globallink_fieldable_panels_get_row_id_from_submission($submission_ticket, $document_ticket, $target_locale) {
  $query = db_select('globallink_core_fieldable_panels', 'tcfp');
  $query->condition('submission_ticket', $submission_ticket, '=');
  $query->condition('document_ticket', $document_ticket, '=');
  $query->condition('target', $target_locale, '=');
  $query->fields('tcfp');

  $results = $query->execute();

  foreach ($results as $row) {
    return $row->id;
  }

  return FALSE;
}

/**
 * Gets sent fieldable panel rows by node ID.
 *
 * @param string $fpid
 *   The fieldable panel ID.
 *
 * @return array
 *   Array of fieldable panels rows.
 */
function globallink_fieldable_panels_get_sent_rows_by_fpid($fpid) {
  $result = db_select('globallink_core_fieldable_panels', 'tcfp')
  ->fields('tcfp')
  ->condition('fpid', $fpid, '=')
  ->condition('status', array('Sent for Translations', 'Error'), 'IN')
  ->execute();

  $rows = array();

  foreach ($result as $row) {
    $rows[] = $row;
  }

  return $rows;
}

/**
 * Retrieves fieldable panel's row by ID.
 *
 * @param string $fpid
 *   The fieldable panel row ID.
 * @param string $source
 *   The fieldable panel source.
 * @param string $target
 *   The fieldable panel target.
 *
 * @return
 *   The row if fieldable panel exists.  FALSE if not.
 */
function globallink_fieldable_panels_get_row($fpid, $source, $target) {
  $result = db_select('globallink_core_fieldable_panels', 'tcfp')
  ->fields('tcfp')
  ->condition('fpid', $fpid, '=')
  ->condition('source', $source, '=')
  ->condition('target', $target, '=')
  ->execute();

  foreach ($result as $row) {
    return $row;
  }

  return FALSE;
}

/**
 * Gets fieldable panel row by id.
 *
 * @param string $id
 *   The fieldable panel row ID.
 *
 * @return
 *   The fieldable panel row.  FALSE if row doesn't exist.
 */
function globallink_fieldable_panels_get_row_by_id($id) {
  $result = db_select('globallink_core_fieldable_panels', 'tcfp')
  ->fields('tcfp')
  ->condition('id', $id, '=')
  ->execute();

  foreach ($result as $row) {
    return $row;
  }

  return FALSE;
}

/**
 * Gets array of translated fieldable panels.
 *
 * @param object $xml
 *   XML representation of fieldable panels.
 *
 * @return array
 *   Associative array of translated fieldable panels.
 */
function globallink_fieldable_panels_get_translated_array($xml) {
  if (is_null($xml) || !is_string($xml) || $xml == '') {
    return array();
  }

  $dom = new DomDocument;
  $dom->preserveWhiteSpace = FALSE;
  $dom->loadXML($xml);

  $arr = array();

  $titles = $dom->getElementsByTagName('title');

  foreach ($titles as $title) {
    $arr['title'] = $title->nodeValue;
  }

  $contents = $dom->getElementsByTagName('content');

  $fpid = '';

  foreach ($contents as $content) {
    if (!is_null($content->attributes)) {
      foreach ($content->attributes as $attr_name => $attr_node) {
        if ($attr_name == 'fpid') {
          $fpid = $attr_node->value;
        }
      }
    }
  }

  if ($fpid == '') {
    return array();
  }

  $arr['fpid'] = $fpid;

  $fields = $dom->getElementsByTagName('field');

  foreach ($fields as $field) {
    $field_obj = new GLField();
    $field_obj->type = 'field';
    $field_obj->translatedContent = $field->nodeValue;

    if (is_null($field->attributes)) {
      continue;
    }

    foreach ($field->attributes as $attr_name => $attr_node) {
      switch ($attr_name) {
        case 'field':
          $field_obj->fieldName = $attr_node->value;
          continue 2;
        case 'delta':
          $field_obj->delta = $attr_node->value;
          continue 2;
        case 'langcode':
          $field_obj->langcode = $attr_node->value;
          continue 2;
      }
    }

    if (is_null($field_obj->langcode)) {
      $field_obj->langcode = LANGUAGE_NONE;
    }

    if (is_null($field_obj->delta)) {
      $field_obj->delta = '0';
    }

    $arr[$field_obj->fieldName][$field_obj->langcode][$field_obj->delta] = $field_obj;
  }

  return $arr;
}

/**
 * Updates fieldable panels submission status.
 *
 * @param string $submission_ticket
 *   The submission ticket.
 * @param string $status
 *   The submission status.  Defaults to 'Cancelled.'
 */
function globallink_fieldable_panels_update_submission_status($submission_ticket, $status = 'Cancelled') {
  db_update('globallink_core_fieldable_panels')
  ->fields(array(
    'status' => $status,
    'timestamp' => REQUEST_TIME,
  ))
  ->condition('submission_ticket', $submission_ticket, '=')
  ->execute();
}

/**
 * Gets number of translated fieldable panels.
 *
 * @param string $pd4
 *   The project director details.
 * @param array $globallink_arr
 *   Array of GlobalLink objects.
 *
 * @return int
 *   The number of translated fieldable panels.
 */
function globallink_fieldable_panels_get_translated($pd4, &$globallink_arr) {
  $count = 0;

  foreach ($globallink_arr as $globallink) {
    $target_xml = globallink_download_target_resource($pd4, $globallink->targetTicket);

    if ($globallink->sourceDeleted || empty($target_xml)) {
      continue;
    }

    $count++;
    $target_locale = globallink_get_drupal_locale_code($globallink->targetLocale);
    $source_locale = globallink_get_drupal_locale_code($globallink->sourceLocale);

    $translated_arr = globallink_fieldable_panels_get_translated_array($target_xml);

    try {
      $fpid = $translated_arr['fpid'];
      unset($translated_arr['fpid']);
      $entity = fieldable_panels_panes_load($fpid);

      if (empty($entity)) {
        globallink_fieldable_panels_update_status($globallink, 'Source Deleted');

        continue;
      }

      $fields = globallink_fieldable_panels_pane_get_fields($entity->bundle);

      foreach ($fields as $field) {
        $t_field_lang = LANGUAGE_NONE;

        if (empty($translated_arr[$field])) {
          continue;
        }

        if (key($translated_arr[$field]) !== LANGUAGE_NONE) {
          $t_field_lang = key($translated_arr[$field]);
        }

        if (empty($entity->{$field}[$target_locale]) && $target_locale != LANGUAGE_NONE) {
            $entity->{$field}[$target_locale] = $entity->{$field}[$t_field_lang];
        }

        $t_field_arr = $translated_arr[$field][$source_locale];

        foreach ($entity->{$field}[$target_locale] as $delta => $fp_field) {
          if (empty($t_field_arr[$delta])) {
            continue;
          }

          $translation = $t_field_arr[$delta]->translatedContent;
          $entity->{$field}[$target_locale][$delta]['value'] = $translation;
        }
      }

      $entity->revision = 1;
      $entity->translations->data[$target_locale] = array(
        'language' => $target_locale, // Target language
        'source' => $source_locale, // Source language
        'uid' => $entity->uid,
        'status' => variable_get('globallink_publish_node', 0), // publish translation
        'translate' => 0, // Translation out of date
        'created' => $entity->created,
      );

      $entity->translations->hook[$target_locale] = array(
        'hook' => 'insert',
        'date' => NULL,
      );

      fieldable_panels_panes_save($entity);
      globallink_fieldable_panels_update_status($globallink);
      globallink_send_download_confirmation($globallink->targetTicket, $pd4);
    }
    catch (SoapFault $se) {
      $count--;

      globallink_fieldable_panels_update_status($globallink, 'Error');
      watchdog('GlobalLink', 'SOAP Exception - %function - Code[%faultcode], Message[%faultstring]', array('%function' => __FUNCTION__, '%faultcode' => $se->faultcode, '%faultstring' => $se->faultstring), WATCHDOG_ERROR);
      form_set_error('', t('Web Services Error: @faultcode - @faultstring', array('@faultcode' => $se->faultcode, '@faultstring' => $se->faultstring)));
    }
    catch (Exception $e) {
      $count--;

      globallink_fieldable_panels_update_status($globallink, 'Error');
      watchdog('GlobalLink', 'Exception - %function - File[%file], Line[%line], Code[%code], Message[%message]', array('%function' => __FUNCTION__, '%file' => $e->getFile(), '%line' => $e->getLine(), '%code' => $e->getCode(), '%message' => $e->getMessage()), WATCHDOG_ERROR);
      form_set_error('', t('Error: @message', array('@message' => $e->getMessage())));
    }
  }

  return $count;
}

/**
 * Updates fieldable panels status.
 *
 * @param object $globallink
 *   GlobalLink object.
 * @param string $status
 *   The new status.  Defaults to 'Pending Translations.'
 */
function globallink_fieldable_panels_update_status(&$globallink, $status = 'Pending Translations') {
  db_update('globallink_core_fieldable_panels')
  ->fields(array(
    'status' => $status,
    'timestamp' => REQUEST_TIME,
  ))
  ->condition('submission', $globallink->submissionName, '=')
  ->condition('document_ticket', $globallink->documentTicket, '=')
  ->condition('source', $globallink->sourceLocale, '=')
  ->condition('target', $globallink->targetLocale, '=')
  ->execute();
}

/**
 * Updates status for deleted fieldable panels.
 *
 * @param string $pd4
 *   The project director details.
 * @param object $globallink
 *   GlobalLink object.
 *
 * @return bool
 *   TRUE if update was successful, FALSE on error.
 */
function globallink_fieldable_panels_update_deleted_records($pd4, $globallink) {
  try {
    $globallink->status = 'Source Deleted';

    globallink_send_download_confirmation($globallink->targetTicket, $pd4);
    globallink_fieldable_panels_update_status($globallink, 'Source Deleted');
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
 * Updates fieldable panel's ticket ID.
 *
 * @param array $arr
 *   Array of GlobalLink objects.
 * @param string $project_code
 *   The fieldable panel's project code.
 */
function globallink_fieldable_panels_update_ticket_id($arr, $project_code) {
  foreach ($arr as $globallink) {
    $target_locale_arr = $globallink->targetLocale;
    $type = $globallink->type;

    if ($type != 'fieldable_panels_panes') {
      continue;
    }

    $fpid = $globallink->otherObjectId;
    $fieldable_panels = fieldable_panels_panes_load($fpid);

    foreach ($target_locale_arr as $target_locale) {
      $row = globallink_fieldable_panels_get_row($fpid, $globallink->sourceLocale, $target_locale);
      $title = 'Title Not Found';

      if (!empty($fieldable_panels->title)) {
        $title = $fieldable_panels->title;
      }
      else {
        if (!empty($fieldable_panels->admin_title)) {
          $title = $fieldable_panels->admin_title;
        }
      }

      if ($row) {
        db_update('globallink_core_fieldable_panels')
        ->fields(array(
          'title' => $title,
          'document_ticket' => $globallink->documentTicket,
          'submission' => $globallink->submissionName,
          'submission_ticket' => $globallink->submissionTicket,
          'status' => 'Sent for Translations',
          'timestamp' => REQUEST_TIME,
          'last_modified' => REQUEST_TIME,
          'project_code' => $project_code,
        ))
        ->condition('id', $row->id, '=')
        ->execute();
      }
      else {
        db_insert('globallink_core_fieldable_panels')
        ->fields(array(
          'fpid' => $fpid,
          'title' => $title,
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
