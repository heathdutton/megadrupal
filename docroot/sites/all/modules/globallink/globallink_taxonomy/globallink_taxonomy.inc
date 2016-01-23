<?php

/**
 * Sends taxonomies for translation.
 *
 * @param array $lids
 *   The array of taxonomy LIDs.
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
function globallink_taxonomy_send_for_translations($lids, $pd4, $submission_name, $due_date, $project_code, $source_locale, $target_locale_arr, $submission_details) {
  module_load_include('inc', 'globallink', 'gl_ws/gl_ws_send_translations');

  $submitter = $submission_details['submitter'];
  $globallink_arr = array();

  foreach ($lids as $lid) {
    $rows = globallink_taxonomy_get_sent_rows_by_bid($lid);
    $target_arr = $target_locale_arr;

    foreach ($rows as $row) {
      if (array_search($row->target, $target_locale_arr)) {
        unset($target_arr[$row->target]);
      }
    }

    if (empty($target_arr)) {
      continue;
    }

    $term = taxonomy_term_load($lid);
    $path = drupal_lookup_path('alias', 'taxonomy/term/' . $lid, globallink_get_drupal_locale_code($source_locale));

    if (empty($term->name)) {
      continue;
    }

    $xml = globallink_taxonomy_get_xml($term, $path);

    $name = 'Taxonomy_' . $lid . '.xml';

    $globallink = new GlobalLink();
    $globallink->type = 'taxonomy';
    $globallink->metadata = 'taxonomy';
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
 * Gets XML data from specific taxonomy.
 *
 * @param object $term
 *   The taxonomy term.
 * @param string $path
 *   The taxonomy path.
 *
 * @return array
 *   Associative array of block XML data.
 */
function globallink_taxonomy_get_xml($term, $path) {
  $dom = new DOMDocument('1.0', 'UTF-8');
  $dom->formatOutput = TRUE;

  $root = $dom->createElement('content');
  $dom->appendChild($root);

  $id = $dom->createAttribute('bid');
  $id->value = $term->tid;
  $root->appendChild($id);

  if (!empty($term->name)) {
    globallink_insert_child_element($dom, $root, 'taxonomy', $term->name, array('name' => 'name'));
  }

  if (!empty($term->description)) {
    globallink_insert_child_element($dom, $root, 'taxonomy', $term->description, array('name' => 'description'));
  }

  if (!empty($path)) {
    globallink_insert_child_element($dom, $root, 'taxonomy', $path, array('name' => 'path'));
  }

  $fields = field_info_instances('taxonomy_term', $term->vocabulary_machine_name);

  foreach (array_keys($fields) as $field) {
    if (empty($term->{$field})) {
      continue;
    }

    $field_value = $term->{$field};

    if (is_array($field_value) && !empty($field_value[$term->language])) {
      foreach ($field_value[$term->language] as $delta => $values) {
        insert_child_element($dom, $root, 'field', $values['value'], array(
          'field' => $field,
          'delta' => $delta,
          'langcode' => $term->language,
        ));
      }
    }
    elseif (!empty($field_value) && is_string($field_value)) {
      insert_child_element($dom, $root, 'field', $field_value, array(
        'field' => $field,
      ));
    }
  }

  $xml = $dom->saveXML();

  return $xml;
}

/**
 * Gets taxonomy strings.
 *
 * @param string $language
 *   The taxonomy language.  Defaults to NULL.
 * @param string $group
 *   The node group.  Defaults to 'taxonomy.'
 *
 * @return array
 *   Array of taxonomy strings
 */
function globallink_taxonomy_get_strings($language = NULL, $group = 'taxonomy') {
  if (isset($language)) {
    $result = db_query('SELECT s.lid, s.source, s.context, s.location, t.plid, t.plural FROM {locales_source} s LEFT JOIN {locales_target} t ON s.lid = t.lid AND t.language = :language WHERE s.textgroup = :textgroup ORDER BY t.plid, t.plural', array(
      ':language' => $language,
      ':textgroup' => $group,
    ));
  }
  else {
    $result = db_query('SELECT s.lid, s.source, s.context, s.location, t.plid, t.plural FROM {locales_source} s LEFT JOIN {locales_target} t ON s.lid = t.lid WHERE s.textgroup = :textgroup ORDER BY t.plid, t.plural', array(
      ':textgroup' => $group,
    ));
  }

  $strings = array();

  foreach ($result as $child) {
    $string = array(
      'lid' => $child->lid,
      'location' => $child->location,
      'source' => $child->source,
      'context' => $child->context,
    );

    $strings[$child->location] = $string;
  }

  return $strings;
}

/**
 * Updates taxonomy ticket ID on GlobalLink.
 *
 * @param array $arr
 *   Array of GlobalLink objects.
 * @param string $project_code
 *   The taxonomy's project code.
 */
function globallink_taxonomy_update_ticket_id($arr, $project_code) {
  foreach ($arr as $globallink) {
    $target_locale_arr = $globallink->targetLocale;
    $type = $globallink->type;

    if ($type != 'taxonomy') {
      continue;
    }

    $bid = $globallink->otherObjectId;
    $taxonomy = taxonomy_term_load($bid);

    foreach ($target_locale_arr as $target_locale) {
      $row = globallink_taxonomy_get_row($bid, $type, $globallink->sourceLocale, $target_locale);

      if ($row) {
        db_update('globallink_core_taxonomy')
        ->fields(array(
          'title' => $taxonomy->name,
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
        db_insert('globallink_core_taxonomy')
        ->fields(array(
          'object_id' => $bid,
          'object_type' => $globallink->type,
          'title' => $taxonomy->name,
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
 * Retrieves taxonomy's row by ID.
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
 *   The row if taxonomy exists.
 */
function globallink_taxonomy_get_row($object_id, $object_type, $source, $target) {
  $result = db_select('globallink_core_taxonomy', 'tco')
  ->fields('tco')
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
 * Updates status for deleted taxonomies.
 *
 * @param string $pd4
 *   The project director details.
 * @param object $globallink
 *   GlobalLink object.
 *
 * @return bool
 *   TRUE if update was successful, FALSE on error.
 */
function globallink_taxonomy_update_deleted_records($pd4, $globallink) {
  try {
    $globallink->status = 'Source Deleted';

    globallink_send_download_confirmation($globallink->targetTicket, $pd4);
    globallink_taxonomy_update_status($globallink, 'Source Deleted');
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
 * Gets taxonomy translation status.
 *
 * @param string $bid
 *   The taxonomy object ID.
 * @param string $tgt_locale
 *   The target locale.
 * @param string $title
 *   The translation title.
 *
 * @return string
 *   Status message.
 */
function globallink_taxonomy_get_translation_status($bid, $tgt_locale, &$title) {
  $query = db_select('globallink_core_taxonomy', 'tco');
  $query->condition('object_id', $bid, '=');
  $query->condition('target', $tgt_locale, '=');
  $query->distinct();
  $query->fields('tco');
  $results = $query->execute();

  foreach ($results as $row) {
    $title = $row->title;
    $s_taxonomy = taxonomy_term_load($bid);

    if (!$s_taxonomy || is_null($s_taxonomy)) {
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
 * Gets active taxonomy submission names.
 *
 * @return array
 *   Array of active taxonomy submission names.
 */
function globallink_taxonomy_get_active_submission_names() {
  $query = db_select('globallink_core_taxonomy', 'tco');
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
 * Cancels taxonomy records.
 *
 * @param array $rowids
 *   Array of taxonomy IDs.
 * @param string $pd4
 *   The project director details.
 */
function globallink_taxonomy_cancel_records($rowids, $pd4) {
  $globallink_arr = array();

  foreach ($rowids as $rid) {
    $row = globallink_get_other_row($rid, 'taxonomy');

    $globallink = new GlobalLink();
    $globallink->tptRowId = $row->rid;
    $globallink->targetLocale = $row->target;
    $globallink->documentTicket = $row->document_ticket;
    $globallink->submissionTicket = $row->submission_ticket;
    $globallink_arr[$rid] = $globallink;
  }

  globallink_cancel_select_documents($pd4, $globallink_arr);
  globallink_taxonomy_update_row_document($globallink_arr);
}

/**
 * Updates taxonomy rows to reflect translation status.
 *
 * @param array $globallink_arr
 *   Array of GlobalLink objects.
 */
function globallink_taxonomy_update_row_document(&$globallink_arr) {
  foreach ($globallink_arr as $globallink) {
    if ($globallink->cancelled) {
      db_update('globallink_core_taxonomy')
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
 * Cancels taxonomy submission.
 *
 * @param string $selected_submission
 *   The name of the submission.
 */
function globallink_taxonomy_cancel_submission($selected_submission) {
  $pd4 = globallink_get_project_director_details();

  $globallink = new GlobalLink();

  $submission_name = globallink_taxonomy_get_submission_name($selected_submission);
  $globallink->submissionName = $submission_name;
  $globallink->submissionTicket = $selected_submission;

  globallink_cancel_pd_submission($pd4, $globallink);
  globallink_taxonomy_update_submission($globallink);
}

/**
 * Gets taxonomy submission name.
 *
 * @param string $submission_ticket
 *   The submission ticket.
 *
 * @return string
 *   The taxonomy submission name.
 */
function globallink_taxonomy_get_submission_name($submission_ticket) {
  $query = db_select('globallink_core_taxonomy', 'tc');
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
 * Updates taxonomy submission.
 *
 * @param object $globallink
 *   GlobalLink object.
 */
function globallink_taxonomy_update_submission(&$globallink) {
  db_update('globallink_core_taxonomy')
  ->fields(array(
    'status' => 'Cancelled',
    'timestamp' => REQUEST_TIME,
  ))
  ->condition('submission_ticket', $globallink->submissionTicket, '=')
  ->condition('submission', $globallink->submissionName, '=')
  ->execute();
}

/**
 * Gets number of translated taxonomies.
 *
 * @param string $pd4
 *   The project director details.
 * @param array $globallink_arr
 *   Array of GlobalLink objects.
 *
 * @return int
 *   The number of translated taxonomies.
 */
function globallink_taxonomy_get_translated($pd4, &$globallink_arr) {
  $count = 0;

  foreach ($globallink_arr as $globallink) {
    if ($globallink->sourceDeleted) {
      continue;
    }

    $target_xml = globallink_download_target_resource($pd4, $globallink->targetTicket);

    if (!isset($target_xml)) {
      continue;
    }

    $count++;

    $language = globallink_get_drupal_locale_code($globallink->targetLocale);
    $translated_arr = globallink_taxonomy_get_translated_items($target_xml);

    try {
      $bid = $translated_arr['bid'];
      unset($translated_arr['bid']);

      $term = taxonomy_term_load($bid);

      if (empty($term)) {
        continue;
      }

      unset($term->tid);
      $term->language = $language;

      $new_term = taxonomy_term_save($term);

      if ($new_term != 1) {
        form_set_error('', t('Unable to save new term for @bid where language is @language', array('@bid' => $bid, '@language' => $language)));

        continue;
      }

      foreach ($translated_arr as $attribute => $translations) {
        switch ($attribute) {
          case 'name':
            $term->name = $translations['translation'];

            break;
          case 'description':
            $term->description = $translations['translation'];

            break;
          case 'path':
            $term->path['pathauto'] = FALSE;

            $uri = taxonomy_term_uri($term);
            $new_path = path_load($uri['path']);
            $new_path['alias'] = $translations['translation'];
            path_save($new_path);

            break;
        }
      }

      taxonomy_term_save($term);
      globallink_taxonomy_update_status($globallink);
      globallink_send_download_confirmation($globallink->targetTicket, $pd4);
    }
    catch (SoapFault $se) {
      $count--;

      globallink_taxonomy_update_status($globallink, 'Error');
      watchdog('GlobalLink', 'SOAP Exception - %function - Code[%faultcode], Message[%faultstring]', array('%function' => __FUNCTION__, '%faultcode' => $se->faultcode, '%faultstring' => $se->faultstring), WATCHDOG_ERROR);
      form_set_error('', t('Web Services Error: @faultcode - @faultstring', array('@faultcode' => $se->faultcode, '@faultstring' => $se->faultstring)));
    }
    catch (Exception $e) {
      $count--;

      globallink_taxonomy_update_status($globallink, 'Error');
      watchdog('GlobalLink', 'Exception - %function - File[%file], Line[%line], Code[%code], Message[%message]', array('%function' => __FUNCTION__, '%file' => $e->getFile(), '%line' => $e->getLine(), '%code' => $e->getCode(), '%message' => $e->getMessage()), WATCHDOG_ERROR);
      form_set_error('', t('Error: @message', array('@message' => $e->getMessage())));
    }
  }

  return $count;
}

/**
 * Gets translated taxonomies from XML data.
 *
 * @param object $xml
 *   XML representation of taxonomies.
 *
 * @return array
 *   Array of taxonomies.
 */
function globallink_taxonomy_get_translated_items($xml) {
  if (is_null($xml) || !is_string($xml) || $xml == '') {
    return array();
  }

  $dom = new DomDocument;
  $dom->preserveWhiteSpace = FALSE;
  $dom->loadXML($xml);

  $contents = $dom->getElementsByTagName('content');

  $bid = '';

  foreach ($contents as $content) {
    if (!is_null($content->attributes)) {
      foreach ($content->attributes as $attr_name => $attr_node) {
        if ($attr_name == 'bid') {
          $bid = $attr_node->value;
        }
      }
    }
  }

  if ($bid == '') {
    return array();
  }

  $taxonomy_arr = array();
  $taxonomy_arr['bid'] = $bid;

  $taxonomies = $dom->getElementsByTagName('taxonomy');

  foreach ($taxonomies as $taxonomy) {
    if (!is_null($taxonomy->attributes)) {
      $b_arr = array();

      foreach ($taxonomy->attributes as $attr_name => $attr_node) {
        $b_arr[$attr_name] = $attr_node->value;
      }

      $b_arr['translation'] = $taxonomy->nodeValue;
      $taxonomy_arr[$b_arr['name']] = $b_arr;
    }
  }

  return $taxonomy_arr;
}

/**
 * Updates taxonomy status.
 *
 * @param object $globallink
 *   GlobalLink object.
 * @param string $status
 *   The new status.  Defaults to 'Pending Translations.'
 */
function globallink_taxonomy_update_status(&$globallink, $status = 'Pending Translations') {
  db_update('globallink_core_taxonomy')
  ->fields(array('status' => $status, 'timestamp' => REQUEST_TIME))
  ->condition('submission', $globallink->submissionName, '=')
  ->condition('document_ticket', $globallink->documentTicket, '=')
  ->condition('source', $globallink->sourceLocale, '=')
  ->condition('target', $globallink->targetLocale, '=')
  ->execute();
}

/**
 * Gets active taxonomy submission rows by object ID.
 *
 * @param string $object_id
 *   The object ID.
 *
 * @return
 *   Associative array of taxonomy active submission rows.  FALSE if the array is empty.
 */
function globallink_taxonomy_get_active_submission_rows($object_id) {
  $query = db_select('globallink_core_taxonomy', 'tc');
  $query->condition('status', 'Sent for Translations', '=');
  $query->condition('object_id', $object_id, '=');
  $query->fields('tc');
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

  foreach ($arr as $bid => $bid_arr) {
    $sub_arr = array();

    foreach ($bid_arr as $r) {
      if (array_key_exists($r->submission, $sub_arr)) {
        array_push($sub_arr[$r->submission], $r->target);
      }
      else {
        $sub_arr[$r->submission] = array($r->target);
      }
    }

    if (count($sub_arr) > 0) {
      $final_arr[$bid] = $sub_arr;
    }
  }

  if (count($final_arr) > 0) {
    return $final_arr;
  }

  return FALSE;
}

/**
 * Gets active taxonomy submission rows by object ID.
 *
 * @param string $bid
 *   The taxonomy object ID.
 *
 * @return
 *   Associative array of taxonomy active submission rows.  FALSE if the array is empty.
 */
function globallink_taxonomy_get_sent_rows_by_bid($bid) {
  $result = db_select('globallink_core_taxonomy', 'tc')
  ->fields('tc')
  ->condition('object_id', $bid, '=')
  ->condition('status', 'Sent for Translations', '=')
  ->execute();

  $rows = array();

  foreach ($result as $row) {
    $rows[] = $row;
  }

  return $rows;
}

/**
 * Gets taxonomy row ID from submission details.
 *
 * @param string $submission_ticket
 *   The submission ticket.
 * @param string $document_ticket
 *   The document ticket.
 * @param string $target_locale
 *   The target locale.
 *
 * @return string
 *   The taxonomy row ID.  FALSE if the row doesn't exist.
 */
function globallink_taxonomy_get_row_id_from_submission($submission_ticket, $document_ticket, $target_locale) {
  $result = db_select('globallink_core_taxonomy', 'tc')
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
 * Gets array of taxonomies from XML data.
 *
 * @param object $xml
 *   The XML data.
 *
 * @return array
 *   Array of taxonomies.
 */
function globallink_taxonomy_get_arr_items($xml) {
  if (is_null($xml) || !is_string($xml) || $xml == '') {
    return array();
  }

  $dom = new DomDocument;
  $dom->preserveWhiteSpace = FALSE;
  $dom->loadXML($xml);

  $contents = $dom->getElementsByTagName('content');
  $bid = '';

  foreach ($contents as $content) {
    if (!is_null($content->attributes)) {
      foreach ($content->attributes as $attr_name => $attr_node) {
        if ($attr_name == 'bid') {
          $bid = $attr_node->value;
        }
      }
    }
  }

  if ($bid == '') {
    return array();
  }

  $block_arr = array();
  $block_arr['bid'] = $bid;
  $blocks = $dom->getElementsByTagName('taxonomy');

  foreach ($blocks as $block) {
    if (!is_null($block->attributes)) {
      $b_arr = array();

      foreach ($block->attributes as $attr_name => $attr_node) {
        $b_arr[$attr_name] = $attr_node->value;
      }

      $block_arr[$b_arr['name']] = $block->nodeValue;
    }
  }

  return $block_arr;
}

/**
 * Gets taxonomy vocabulary list.
 *
 * @return array
 *   Array of taxonomy vocabulary.
 */
function globallink_taxonomy_get_vocabulary_list() {
  $v_arr = array('[any]' => 'Any');
  $arr = array();

  $vocabs = taxonomy_vocabulary_load_multiple(FALSE);

  foreach ($vocabs as $vocab) {
    if ($vocab->i18n_mode == 1) {
      $arr[$vocab->vid] = $vocab->name;
    }
  }

  asort($arr);

  return $v_arr + $arr;
}

/**
 * Gets distinct active taxonomy submission names.
 *
 * @return array
 *   Array of distinct active taxonomy submission names.
 */
function globallink_taxonomy_get_distinct_active_submission_names() {
  $query = db_select('globallink_core_taxonomy', 'tc');
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
 * Updates taxonomy submission status.
 *
 * @param string $submission_ticket
 *   The submission ticket.
 * @param string $status
 *   The submission status.  Defaults to 'Cancelled.'
 */
function globallink_taxonomy_update_submission_status($submission_ticket, $status = 'Cancelled') {
  db_update('globallink_core_taxonomy')
  ->fields(array(
    'status' => $status,
    'timestamp' => REQUEST_TIME,
  ))
  ->condition('submission_ticket', $submission_ticket, '=')
  ->execute();
}

/**
 * Gets taxonomy submission status.
 */
function globallink_taxonomy_get_submission_status() {
  module_load_include('inc', 'globallink', 'globallink_settings');
  module_load_include('inc', 'globallink', 'gl_ws/gl_ws_common');

  $query = db_select('globallink_core_taxonomy', 'tc');
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
          globallink_taxonomy_update_submission_status($row->submission_ticket);
        }
      }
      catch (SoapFault $se) {
        globallink_taxonomy_update_submission_status($row->submission_ticket);
      }
      catch (Exception $ex) {
        globallink_taxonomy_update_submission_status($row->submission_ticket);
      }
    }
  }
}

/**
 * Checks taxonomy status based on row IDs.
 *
 * @param array $rids_arr
 *   Array of row IDs.
 *
 * @return array
 *   Array of row IDs that have been sent for translation or threw an error.
 */
function globallink_taxonomy_check_status($rids_arr) {
  $status = TRUE;

  $query = db_select('globallink_core_taxonomy', 'tc')
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
 * Clears cancelled taxonomy documents.
 *
 * @return int
 *   Number of cancelled taxonomy documents.
 */
function globallink_taxonomy_clear_cancelled_documents() {
  $count = 0;

  $query = db_select('globallink_core_taxonomy', 'tc')
  ->fields('tc', array('submission_ticket'))
  ->distinct()
  ->condition('status', 'Cancelled', '=');

  $results = $query->execute();

  foreach ($results as $item) {
    globallink_taxonomy_update_submission_status($item->submission_ticket, 'Pending Translations');

    $count++;
  }

  return $count;
}
