<?php

/**
 * Sends blocks for translation.
 *
 * @param array $bids
 *   The array of block IDs.
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
function globallink_block_send_blocks_for_translations($bids, $pd4, $submission_name, $due_date, $project_code, $source_locale, $target_locale_arr, $submission_details) {
  module_load_include('inc', 'globallink', 'gl_ws/gl_ws_send_translations');
  $strings = globallink_block_get_strings(globallink_get_drupal_locale_code($source_locale), 'blocks');
  $submitter = $submission_details['submitter'];

  $globallink_arr = array();

  foreach ($bids as $bid) {
    $rows = globallink_block_get_sent_tpt_rows_by_bid($bid);
    $target_arr = $target_locale_arr;

    foreach ($rows as $row) {
      if (array_search($row->target, $target_locale_arr)) {
        unset($target_arr[$row->target]);
      }
    }

    if (empty($target_arr)) {
      continue;
    }

    $title_string_arr = isset($strings['blocks:block:' . $bid . ':title']) ? $strings['blocks:block:' . $bid . ':title'] : FALSE;
    $body_string_arr = isset($strings['blocks:block:' . $bid . ':body']) ? $strings['blocks:block:' . $bid . ':body'] : FALSE;
    if (!$body_string_arr) {
      continue;
    }

    $xml = globallink_block_get_xml($bid, $title_string_arr, $body_string_arr);
    $name = 'Block_' . $bid . '.xml';

    watchdog('GlobalLink', 'XML - %xml', array('%xml' => $xml), WATCHDOG_DEBUG);

    $globallink = new GlobalLink();
    $globallink->type = 'block';
    $globallink->metadata = 'block';
    $globallink->sourceLocale = $source_locale;
    $globallink->targetLocale = $target_arr;
    $globallink->sourceXML = $xml;
    $globallink->sourceFileName = $name;
    $globallink->submissionName = $submission_name;
    $globallink->dueDate = $due_date;
    $globallink->otherObjectId = $bid;
    $globallink->submissionInstructions = $submission_details['instructions'] . "\nSubmitter: " . $submitter;
    $globallink_arr[] = $globallink;
  }

  if (!empty($globallink_arr)) {
    globallink_send_documents_for_translation_to_pd($globallink_arr, $pd4, $project_code, $submitter);
  }

  return $globallink_arr;
}

/**
 * Gets XML data from specific block.
 *
 * @param string $bid
 *   The block ID.
 * @param array $title_string_arr
 *   Array of block's title strings.
 * @param array $body_string_arr
 *   Array of block's body strings.
 *
 * @return array
 *   Associative array of block XML data.
 */
function globallink_block_get_xml($bid, $title_string_arr, $body_string_arr) {
  $dom = new DOMDocument('1.0', 'UTF-8');
  $dom->formatOutput = TRUE;

  $root = $dom->createElement('content');
  $dom->appendChild($root);

  $id = $dom->createAttribute('bid');
  $id->value = $bid;
  $root->appendChild($id);

  $custom_block = block_custom_block_get($bid);

  if ($title_string_arr) {
    globallink_insert_child_element($dom, $root, 'block', $title_string_arr['source'], array(
      'name' => 'title',
      'lid' => $title_string_arr['lid'],
      'location' => $title_string_arr['location'],
      'context' => $title_string_arr['context'],
    ));
  }

  if ($body_string_arr) {
    $format = isset($custom_block['format']) ? $custom_block['format'] : '';
    globallink_insert_child_element($dom, $root, 'block', $body_string_arr['source'], array(
      'name' => 'body',
      'lid' => $body_string_arr['lid'],
      'location' => $body_string_arr['location'],
      'context' => $body_string_arr['context'],
      'format' => $format,
    ));
  }

  $xml = $dom->saveXML();

  return $xml;
}

/**
 * Retrieves strings for all blocks.
 *
 * @param string $language
 *   The blocks' $language. Defaults to NULL.
 * @param string $group
 *   The Drupal content type.  Defaults to 'blocks.'
 *
 * @return array
 *   Array of strings for blocks.
 */
function globallink_block_get_strings($language = NULL, $group = 'blocks') {
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
 * Updates block ticket ID.
 *
 * @param array $arr
 *   Array of GlobalLink objects.
 * @param string $project_code
 *   The block's project code.
 */
function globallink_block_update_ticket_id($arr, $project_code) {
  foreach ($arr as $globallink) {
    $target_locale_arr = $globallink->targetLocale;
    $type = $globallink->type;

    if ($type == 'block') {
      $bid = $globallink->otherObjectId;
      $block = globallink_block_load_data($bid);

      foreach ($target_locale_arr as $target_locale) {
        $row = globallink_block_get_row($bid, $type, $globallink->sourceLocale, $target_locale);

        if ($row) {
          db_update('globallink_core_block')
          ->fields(array(
            'title' => $block[0]->info,
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
          db_insert('globallink_core_block')
          ->fields(array(
            'object_id' => $bid,
            'object_type' => $globallink->type,
            'title' => $block[0]->info,
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
}

/**
 * Retrieves block's row.
 *
 * @param string $object_id
 *   The block's ID.
 * @param string $object_type
 *   The block's type.
 * @param string $source
 *   The source of the block.
 * @param string $target
 *   The target of the block.
 *
 * @return
 *   The row if block exists.  FALSE if the block doesn't exist.
 */
function globallink_block_get_row($object_id, $object_type, $source, $target) {
  $result = db_select('globallink_core_block', 'tco')
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
 * Updates status for deleted blocks.
 *
 * @param string $pd4
 *   The project director details.
 * @param object $globallink
 *   GlobalLink object.
 *
 * @return bool
 *   TRUE if update was successful, FALSE on error.
 */
function globallink_block_update_deleted_records($pd4, $globallink) {
  try {
    $globallink->status = 'Source Deleted';

    globallink_send_download_confirmation($globallink->targetTicket, $pd4);
    globallink_block_update_status($globallink, 'Source Deleted');
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
 * Gets block translation status.
 *
 * @param string $bid
 *   The block ID.
 * @param string $tgt_locale
 *   The target locale.
 * @param string $title
 *   The translation title.
 *
 * @return string
 *   Status message.
 */
function globallink_block_get_translation_status($bid, $tgt_locale, &$title) {
  $query = db_select('globallink_core_block', 'tco');
  $query->condition('object_id', $bid, '=');
  $query->condition('target', $tgt_locale, '=');
  $query->distinct();
  $query->fields('tco');

  $results = $query->execute();

  foreach ($results as $row) {
    $title = $row->title;
    $s_block = block_custom_block_get($bid);

    if (!$s_block || is_null($s_block)) {
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
 * Gets block active submission names.
 *
 * @return array
 *   Array of block submission names.
 */
function globallink_block_get_active_submission_names() {
  $query = db_select('globallink_core_block', 'tco');
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
 * Cancels block records.
 *
 * @param array $rowids
 *   Array of block IDs.
 * @param string $pd4
 *   The project director details.
 */
function globallink_block_cancel_records($rowids, $pd4) {
  $globallink_arr = array();

  foreach ($rowids as $rid) {
    $row = globallink_get_other_row($rid, 'block');

    $globallink = new GlobalLink();
    $globallink->tptRowId = $row->rid;
    $globallink->targetLocale = $row->target;
    $globallink->documentTicket = $row->document_ticket;
    $globallink->submissionTicket = $row->submission_ticket;
    $globallink_arr[$rid] = $globallink;
  }

  globallink_cancel_select_documents($pd4, $globallink_arr);
  globallink_block_update_row_document($globallink_arr);
}

/**
 * Updates block rows to reflect translation status.
 *
 * @param array $globallink_arr
 *   Array of GlobalLink objects.
 */
function globallink_block_update_row_document(&$globallink_arr) {
  foreach ($globallink_arr as $globallink) {
    if ($globallink->cancelled) {
      db_update('globallink_core_block')
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
 * Cancels block submission.
 *
 * @param string $selected_submission
 *   The name of the submission.
 */
function globallink_block_cancel_submission($selected_submission) {
  $pd4 = globallink_get_project_director_details();

  $globallink = new GlobalLink();
  $submission_name = globallink_block_get_submission_name($selected_submission);
  $globallink->submissionName = $submission_name;
  $globallink->submissionTicket = $selected_submission;

  globallink_cancel_pd_submission($pd4, $globallink);
  globallink_block_update_submission($globallink);
}

/**
 * Gets block submission name.
 *
 * @param string $submission_ticket
 *   The submission ticket.
 *
 * @return string
 *   The block submission name.
 */
function globallink_block_get_submission_name($submission_ticket) {
  $query = db_select('globallink_core_block', 'tc');
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
 * Updates block submission with new ticket and name.
 *
 * @param object $globallink
 *   GlobalLink object.
 */
function globallink_block_update_submission(&$globallink) {
  db_update('globallink_core_block')
  ->fields(array(
    'status' => 'Cancelled',
    'timestamp' => REQUEST_TIME,
  ))
  ->condition('submission_ticket', $globallink->submissionTicket, '=')
  ->condition('submission', $globallink->submissionName, '=')
  ->execute();
}

/**
 * Gets number of translated blocks.
 *
 * @param string $pd4
 *   The project director details.
 * @param array $globallink_arr
 *   Array of GlobalLink objects.
 *
 * @return int
 *   The number of translated blocks.
 */
function globallink_block_get_translated_blocks($pd4, &$globallink_arr) {
  $count = 0;

  $source_link = array('link_title' => 'No Title');
  $strings = globallink_block_get_strings(NULL, 'blocks');

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
    $translated_arr = globallink_block_get_translated_items($target_xml);

    try {
      $bid = $translated_arr['bid'];

      foreach ($translated_arr as $attribute => $translations) {
        if ($attribute == 'bid') {
          continue;
        }

        $source = '';

        switch ($attribute) {
          case 'title':
            $title_string_arr = isset($strings['blocks:block:$bid:title']) ? $strings['blocks:block:$bid:title'] : FALSE;

            if (!$title_string_arr) {
              continue;
            }

            $source = $title_string_arr['source'];

            break;
          case 'body':
            $body_string_arr = isset($strings['blocks:block:$bid:body']) ? $strings['blocks:block:$bid:body'] : FALSE;

            if (!$body_string_arr) {
              throw new Exception('Source string not found for block id ' . $bid . ' and field name ' . $attribute);
            }

            $source = $body_string_arr['source'];

            break;
        }

        $report = &drupal_static(__FUNCTION__, array(
          'additions' => 0,
          'updates' => 0,
          'deletes' => 0,
          'skips' => 0,
        ));

        _locale_import_one_string_db($report, $language, $translations['context'], $source, $translations['translation'], 'blocks', $translations['location'], LOCALE_IMPORT_OVERWRITE);
      }

      globallink_block_update_status($globallink);
      globallink_send_download_confirmation($globallink->targetTicket, $pd4);
    }
    catch (SoapFault $se) {
      $count--;

      globallink_block_update_status($globallink, 'Error');

      watchdog('GlobalLink', 'SOAP Exception - %function - Code[%faultcode], Message[%faultstring]', array('%function' => __FUNCTION__, '%faultcode' => $se->faultcode, '%faultstring' => $se->faultstring), WATCHDOG_ERROR);
      form_set_error('', t('Web Services Error: @faultcode - @faultstring', array('@faultcode' => $se->faultcode, '@faultstring' => $se->faultstring)));
    }
    catch (Exception $e) {
      $count--;

      globallink_block_update_status($globallink, 'Error');

      watchdog('GlobalLink', 'Exception - %function - File[%file], Line[%line], Code[%code], Message[%message]', array('%function' => __FUNCTION__, '%file' => $e->getFile(), '%line' => $e->getLine(), '%code' => $e->getCode(), '%message' => $e->getMessage()), WATCHDOG_ERROR);
      form_set_error('', t('Error: @message', array('@message' => $e->getMessage())));
    }
  }

  return $count;
}

/**
 * Gets translated blocks.
 *
 * @param object $xml
 *   XML representation of blocks.
 *
 * @return array
 *   Array of blocks.
 */
function globallink_block_get_translated_items($xml) {
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

  $blocks = $dom->getElementsByTagName('block');

  foreach ($blocks as $block) {
    if (!is_null($block->attributes)) {
      $b_arr = array();

      foreach ($block->attributes as $attr_name => $attr_node) {
        $b_arr[$attr_name] = $attr_node->value;
      }

      $b_arr['translation'] = $block->nodeValue;
      $block_arr[$b_arr['name']] = $b_arr;
    }
  }

  return $block_arr;
}

/**
 * Updates block status.
 *
 * @param object $globallink
 *   GlobalLink object.
 * @param string $status
 *   The translation status.  Defaults to 'Pending Translations.'
 */
function globallink_block_update_status(&$globallink, $status = 'Pending Translations') {
  db_update('globallink_core_block')
  ->fields(array('status' => $status, 'timestamp' => REQUEST_TIME))
  ->condition('submission', $globallink->submissionName, '=')
  ->condition('document_ticket', $globallink->documentTicket, '=')
  ->condition('source', $globallink->sourceLocale, '=')
  ->condition('target', $globallink->targetLocale, '=')
  ->execute();
}

/**
 * Gets active block submission rows.
 *
 * @param string $object_id
 *   The GlobalLink object ID.
 *
 * @return
 *   Array of active block submission rows if there are any.  FALSE if not.
 */
function globallink_block_get_active_submission_rows($object_id) {
  $query = db_select('globallink_core_block', 'tc');
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
 * Gets sent block rows by ID.
 *
 * @param string $bid
 *   The block ID.
 *
 * @return array
 *   Array of block rows.
 */
function globallink_block_get_sent_tpt_rows_by_bid($bid) {
  $result = db_select('globallink_core_block', 'tc')
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
 * Gets block row ID from submission details.
 *
 * @param string $submission_ticket
 *   The submission ticket.
 * @param string $document_ticket
 *   The document ticket.
 * @param string $target_locale
 *   The target locale.
 *
 * @return string
 *   The block row ID.
 */
function globallink_block_get_row_id_from_submission($submission_ticket, $document_ticket, $target_locale) {
  $result = db_select('globallink_core_block', 'tc')
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
 * Loads block data.
 *
 * @param string $bid
 *   The block ID.
 *
 * @return array
 *   Array of block data.
 */
function globallink_block_load_data($bid) {
  $query = db_select('block_custom', 'bc')
  ->fields('bc')
  ->condition('bid', $bid, '=');

  $results = $query->execute();

  $rows = array();

  foreach ($results as $item) {
    $rows[] = $item;
  }

  return $rows;
}

/**
 * Gets array of blocks from XML data.
 *
 * @param object $xml
 *   The XML data.
 *
 * @return array
 *   Array of blocks.
 */
function globallink_block_get_arr_items($xml) {
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
  $blocks = $dom->getElementsByTagName('block');

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
 * Gets distinct active block submission names.
 *
 * @return array
 *   Array of distinct active block submission names.
 */
function globallink_block_get_distinct_active_submission_names() {
  $query = db_select('globallink_core_block', 'tc');
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
 * Updates block submission status.
 *
 * @param string $submission_ticket
 *   The submission ticket.
 * @param string $status
 *   The submission status.  Defaults to 'Cancelled.'
 */
function globallink_block_update_submission_status($submission_ticket, $status = 'Cancelled') {
  db_update('globallink_core_block')
  ->fields(array(
    'status' => $status,
    'timestamp' => REQUEST_TIME,
  ))
  ->condition('submission_ticket', $submission_ticket, '=')
  ->execute();
}

/**
 * Gets block submission status.
 */
function globallink_block_get_submission_status() {
  module_load_include('inc', 'globallink', 'globallink_settings');
  module_load_include('inc', 'globallink', 'gl_ws/gl_ws_common');

  $query = db_select('globallink_core_block', 'tcb');
  $query->fields('tcb', array('submission_ticket'));
  $query->distinct();
  $query->condition('status', 'Sent for Translations', '=');

  $results = $query->execute();

  foreach ($results as $row) {
    if ($row->submission_ticket) {
      try {
        $pd4 = globallink_get_project_director_details();
        $status = globallink_get_status($pd4, $row->submission_ticket);

        if (!$status || $status == 'CANCELLED') {
          globallink_block_update_submission_status($row->submission_ticket);
        }
      }
      catch (SoapFault $se) {
        globallink_block_update_submission_status($row->submission_ticket);
      }
      catch (Exception $ex) {
        globallink_block_update_submission_status($row->submission_ticket);
      }
    }
  }
}

/**
 * Checks block status based on row IDs.
 *
 * @param array $rids_arr
 *   Array of row IDs.
 *
 * @return array
 *   Array of row IDs that have been sent for translation or threw an error.
 */
function globallink_block_check_status($rids_arr) {
  $status = TRUE;

  $query = db_select('globallink_core_block', 'tcb')
  ->fields('tcb', array('rid'))
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
 * Clears cancelled block documents.
 *
 * @return int
 *   Number of cancelled block documents.
 */
function globallink_block_clear_cancelled_documents() {
  $count = 0;

  $query = db_select('globallink_core_block', 'tcb')
  ->fields('tcb', array('submission_ticket'))
  ->distinct()
  ->condition('status', 'Cancelled', '=');

  $results = $query->execute();

  foreach ($results as $item) {
    globallink_block_update_submission_status($item->submission_ticket, 'Pending Translations');

    $count++;
  }

  return $count;
}
