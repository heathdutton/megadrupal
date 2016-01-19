<?php

/**
 * Sends menus for translation.
 *
 * @param array $mlids
 *   The array of menu IDs.
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
function globallink_menu_send_for_translations($mlids, $pd4, $submission_name, $due_date, $project_code, $source_locale, $target_locale_arr, $submission_details) {
  module_load_include('inc', 'globallink', 'gl_ws/gl_ws_send_translations');

  $submitter = $submission_details['submitter'];
  $globallink_arr = array();

  foreach ($mlids as $mlid) {
    $rows = globallink_menu_get_sent_rows_by_mlid($mlid);
    $target_arr = $target_locale_arr;

    foreach ($rows as $row) {
      if (array_search($row->target, $target_locale_arr)) {
        unset($target_arr[$row->target]);
      }
    }

    if (empty($target_arr)) {
      continue;
    }

    $xml = globallink_menu_get_xml($mlid);
    $name = 'Menu_' . $mlid . '.xml';
    watchdog('GlobalLink', 'XML - %xml', array('%xml' => $xml), WATCHDOG_DEBUG);

    $globallink = new GlobalLink();
    $globallink->type = 'menu';
    $globallink->metadata = 'menu';
    $globallink->sourceLocale = $source_locale;
    $globallink->targetLocale = $target_arr;
    $globallink->sourceXML = $xml;
    $globallink->sourceFileName = $name;
    $globallink->submissionName = $submission_name;
    $globallink->dueDate = $due_date;
    $globallink->otherObjectId = $mlid;
    $globallink->submissionInstructions = $submission_details['instructions'] . "\nSubmitter: " . $submitter;
    $globallink_arr[] = $globallink;
  }

  if (!empty($globallink_arr)) {
    globallink_send_documents_for_translation_to_pd($globallink_arr, $pd4, $project_code, $submitter);
  }

  return $globallink_arr;
}

/**
 * Gets XML data from specific menu.
 *
 * @param string $mlid
 *   The menu ID.
 *
 * @return array
 *   Associative array of block XML data.
 */
function globallink_menu_get_xml($mlid) {
  $dom = new DOMDocument('1.0', 'UTF-8');
  $dom->formatOutput = TRUE;

  $root = $dom->createElement('content');
  $dom->appendChild($root);

  $id = $dom->createAttribute('mlid');
  $id->value = $mlid;
  $root->appendChild($id);

  $link = menu_link_load($mlid);
  globallink_insert_child_element($dom, $root, 'menu', $link['link_title'], array('name' => 'link_title'));

  if (isset($link['options']) && isset($link['options']['attributes'])) {
    if (isset($link['options']['attributes']['title'])) {
      if (is_string($link['options']['attributes']['title'])) {
        globallink_insert_child_element($dom, $root, 'menu', $link['options']['attributes']['title'], array('name' => 'title'));
      }
    }
  }

  $xml = $dom->saveXML();

  return $xml;
}

/**
 * Updates menu ticket ID.
 *
 * @param array $arr
 *   Array of GlobalLink objects.
 * @param string $project_code
 *   The menu's project code.
 */
function globallink_menu_update_ticket_id($arr, $project_code) {
  foreach ($arr as $globallink) {
    $target_locale_arr = $globallink->targetLocale;
    $type = $globallink->type;

    if ($type != 'menu') {
      continue;
    }

    $mlid = $globallink->otherObjectId;
    $link = menu_link_load($mlid);

    foreach ($target_locale_arr as $target_locale) {
      $row = globallink_menu_get_row($mlid, $type, $globallink->sourceLocale, $target_locale);

      if ($row) {
        db_update('globallink_core_menu')
        ->fields(array(
          'title' => $link['link_title'],
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
        db_insert('globallink_core_menu')
        ->fields(array(
          'object_id' => $mlid,
          'object_type' => $globallink->type,
          'title' => $link['link_title'],
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
 * Retrieves menu's row by ID.
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
 *   The row if menu exists.
 */
function globallink_menu_get_row($object_id, $object_type, $source, $target) {
  $result = db_select('globallink_core_menu', 'tco')
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
 * Updates status for deleted menus.
 *
 * @param string $pd4
 *   The project director details.
 * @param object $globallink
 *   GlobalLink object.
 *
 * @return bool
 *   TRUE if update was successful, FALSE on error.
 */
function globallink_menu_update_deleted_records($pd4, $globallink) {
  try {
    $globallink->status = 'Source Deleted';

    globallink_send_download_confirmation($globallink->targetTicket, $pd4);
    globallink_menu_update_status($globallink, 'Source Deleted');
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
 * Gets menu translation status.
 *
 * @param string $mlid
 *   The menu ID.
 * @param string $tgt_locale
 *   The target locale.
 * @param string $title
 *   The translation title.
 *
 * @return string
 *   Status message.
 */
function globallink_menu_get_translation_status($mlid, $tgt_locale, &$title) {
  $query = db_select('globallink_core_menu', 'tco');
  $query->condition('object_id', $mlid, '=');
  $query->condition('target', $tgt_locale, '=');
  $query->distinct();
  $query->fields('tco');

  $results = $query->execute();

  foreach ($results as $row) {
    $title = $row->title;
    $s_link = menu_link_load($mlid);

    if (!$s_link) {
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
 * Gets active menu submission names.
 *
 * @return array
 *   Array of active menu submission names.
 */
function globallink_menu_get_active_submission_names() {
  $query = db_select('globallink_core_menu', 'tco');
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
 * Cancels menu records.
 *
 * @param array $rowids
 *   Array of menu IDs.
 * @param string $pd4
 *   The project director details.
 */
function globallink_menu_cancel_records($rowids, $pd4) {
  $globallink_arr = array();

  foreach ($rowids as $rid) {
    $row = globallink_get_other_row($rid, 'menu');

    $globallink = new GlobalLink();
    $globallink->tptRowId = $row->rid;
    $globallink->targetLocale = $row->target;
    $globallink->documentTicket = $row->document_ticket;
    $globallink->submissionTicket = $row->submission_ticket;
    $globallink_arr[$rid] = $globallink;
  }

  globallink_cancel_select_documents($pd4, $globallink_arr);
  globallink_menu_update_row_document($globallink_arr);
}

/**
 * Updates menu rows to reflect translation status.
 *
 * @param array $globallink_arr
 *   Array of GlobalLink objects.
 */
function globallink_menu_update_row_document(&$globallink_arr) {
  foreach ($globallink_arr as $globallink) {
    if ($globallink->cancelled) {
      db_update('globallink_core_menu')
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
 * Cancels menu submission.
 *
 * @param string $selected_submission
 *   The name of the submission.
 */
function globallink_menu_cancel_submission($selected_submission) {
  $pd4 = globallink_get_project_director_details();

  $globallink = new GlobalLink();

  $submission_name = globallink_menu_get_submission_name($selected_submission);
  $globallink->submissionName = $submission_name;
  $globallink->submissionTicket = $selected_submission;

  globallink_cancel_pd_submission($pd4, $globallink);
  globallink_menu_update_submission($globallink);
}

/**
 * Gets menu submission name.
 *
 * @param string $submission_ticket
 *   The submission ticket.
 *
 * @return string
 *   The menu submission name.
 */
function globallink_menu_get_submission_name($submission_ticket) {
  $query = db_select('globallink_core_menu', 'tc');
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
 * Updates menu submission.
 *
 * @param object $globallink
 *   GlobalLink object.
 */
function globallink_menu_update_submission(&$globallink) {
  db_update('globallink_core_menu')
  ->fields(array(
    'status' => 'Cancelled',
    'timestamp' => REQUEST_TIME,
  ))
  ->condition('submission_ticket', $globallink->submissionTicket, '=')
  ->condition('submission', $globallink->submissionName, '=')
  ->execute();
}

/**
 * Gets internationalization menu set.
 *
 * @param string $tsid
 *   The TSID.
 *
 * @return array
 *   Array representing the internationalization menu set.
 */
function globallink_menu_get_i18n_menu_set($tsid) {
  $arr = array();

  $query = db_select('menu_links', 'ml');
  $query->fields('ml');
  $query->condition('i18n_tsid', $tsid, '=');

  $results = $query->execute();

  foreach ($results as $row) {
    $arr[$row->language] = $row;
  }

  return $arr;
}

/**
 * Sorts GlobalLink objects by menu order.
 *
 * @param string $gl_1
 *   The first GlobalLink object.
 * @param string $gl_2
 *   The second GlobalLink object.
 *
 * @return int
 *   -1 if the first object has fewer parents than the second object.  1 otherwise.
 */
function globallink_menu_sort_gl_objects_by_menu_order($gl_1, $gl_2) {
  $link_1 = menu_link_load($gl_1->otherObjectId);
  $link_2 = menu_link_load($gl_2->otherObjectId);

  return (globallink_menu_count_parents($link_1) < globallink_menu_count_parents($link_2)) ? -1 : 1;
}

/**
 * Counts parents of menu link.
 *
 * @param string $menu_link
 *   The menu link.
 *
 * @return int
 *   The number of parents of the menu link.
 */
function globallink_menu_count_parents($menu_link) {
  $count = 0;

  if (isset($menu_link)) {
    if (isset($menu_link['p1']) && $menu_link['p1'] != 0) {
      $count++;
    }

    if (isset($menu_link['p2']) && $menu_link['p2'] != 0) {
      $count++;
    }

    if (isset($menu_link['p3']) && $menu_link['p3'] != 0) {
      $count++;
    }

    if (isset($menu_link['p4']) && $menu_link['p4'] != 0) {
      $count++;
    }

    if (isset($menu_link['p5']) && $menu_link['p5'] != 0) {
      $count++;
    }

    if (isset($menu_link['p6']) && $menu_link['p6'] != 0) {
      $count++;
    }

    if (isset($menu_link['p7']) && $menu_link['p7'] != 0) {
      $count++;
    }

    if (isset($menu_link['p8']) && $menu_link['p8'] != 0) {
      $count++;
    }

    if (isset($menu_link['p9']) && $menu_link['p9'] != 0) {
      $count++;
    }
  }

  return $count;
}

/**
 * Gets number of translated menus.
 *
 * @param string $pd4
 *   The project director details.
 * @param array $globallink_arr
 *   Array of GlobalLink objects.
 *
 * @return int
 *   The number of translated menus.
 */
function globallink_menu_get_translated($pd4, &$globallink_arr) {
  module_load_include('inc', 'i18n_menu', 'i18n_menu.admin');

  $count = 0;
  $source_link = array('link_title' => 'No Title');

  usort($globallink_arr, 'globallink_menu_sort_gl_objects_by_menu_order');

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
    $arr = globallink_menu_get_translated_items($target_xml);

    try {
      $mlid = $arr['mlid'];
      $t_link_title = $arr['link_title'];
      $source_link = menu_link_load($mlid);

      if ($source_link) {
        $t_title = FALSE;

        if (isset($source_link['options'])) {
          if (isset($source_link['options']['attributes'])) {
            if (isset($source_link['options']['attributes']['title'])) {
              if ($source_link['options']['attributes']['title'] != '') {
                $t_title = isset($arr['title']) ? $arr['title'] : FALSE;
              }
            }
          }
        }

        $t_plid = 0;

        if (isset($source_link['plid']) && $source_link['plid'] != 0) {
          $plid = $source_link['plid'];
          $p_source_link = menu_link_load($plid);
          $p_path_arr = translation_path_get_translations($p_source_link['link_path']);

          if (isset($p_path_arr[$language])) {
            $p_target_link = i18n_menu_link_load($p_path_arr[$language], $language);
            $t_plid = $p_target_link['mlid'];
          }
          elseif (isset($p_source_link['i18n_tsid']) && $p_source_link['i18n_tsid'] != 0) {
            $i18n_arr = globallink_menu_get_i18n_menu_set($p_source_link['i18n_tsid']);

            if (isset($i18n_arr[$language])) {
              $menu_link_row = $i18n_arr[$language];
              $t_plid = $menu_link_row->mlid;
            }
          }
        }

        $path_arr = translation_path_get_translations($source_link['link_path']);
        $link_path = '';

        if (!isset($path_arr[$language])) {
          $link_path = $source_link['link_path'];
        }
        else {
          $link_path = $path_arr[$language];
        }

        $t_link = NULL;

        if (isset($source_link['i18n_tsid']) && $source_link['i18n_tsid'] != 0) {
          $i18n_arr = globallink_menu_get_i18n_menu_set($source_link['i18n_tsid']);

          if (isset($i18n_arr[$language])) {
            $menu_link_row = $i18n_arr[$language];
            $t_link = menu_link_load($menu_link_row->mlid);
          }
        }

        if (is_array($t_link)) {
          $t_link['link_title'] = $t_link_title;
          $t_link['menu_name'] = $source_link['menu_name'];
          $t_link['customized'] = $source_link['customized'];
          $t_link['has_children'] = $source_link['has_children'];
          $t_link['expanded'] = $source_link['expanded'];
          $t_link['weight'] = $source_link['weight'];
          $t_link['hidden'] = $source_link['hidden'];

          if ($t_title && $t_link != '') {
            if (isset($t_link['options'])) {
              if (isset($t_link['options']['attributes'])) {
                $t_link['options']['attributes']['title'] = $t_title;
              }
              else {
                $t_link['options']['attributes'] = array('title' => $t_title);
              }
            }
          }
        }
        else {
          $t_link = array();

          $t_link['link_title'] = $t_link_title;
          $t_link['language'] = $language;
          $t_link['link_path'] = $link_path;
          $t_link['menu_name'] = $source_link['menu_name'];
          $t_link['customized'] = $source_link['customized'];
          $t_link['has_children'] = $source_link['has_children'];
          $t_link['expanded'] = $source_link['expanded'];
          $t_link['weight'] = $source_link['weight'];
          $t_link['hidden'] = $source_link['hidden'];

          if ($t_plid != 0) {
            $t_link['plid'] = $t_plid;
          }

          if ($t_title) {
            $t_link['options'] = array(
              'attributes' => array('title' => $t_title),
              'langcode' => $language,
            );
          }
        }

        menu_link_save($t_link);

        $translation_set = ($source_link['i18n_tsid']) ? i18n_translation_set_load($source_link['i18n_tsid']) : i18n_translation_set_create('menu_link');
        $translation_set->add_item($source_link, $source_link['language']);
        $translation_set->add_item($t_link, $language);
        $translation_set->save(TRUE);

        globallink_menu_update_status($globallink);
      }
      else {
        // Source Link is deleted
        globallink_menu_update_status($globallink, 'Source Deleted');
      }

      globallink_send_download_confirmation($globallink->targetTicket, $pd4);
    }
    catch (SoapFault $se) {
      $count--;

      globallink_menu_update_status($globallink, 'Error');
      watchdog('GlobalLink', 'SOAP Exception - %function - Code[%faultcode], Message[%faultstring]', array('%function' => __FUNCTION__, '%faultcode' => $se->faultcode, '%faultstring' => $se->faultstring), WATCHDOG_ERROR);
      form_set_error('', t('Web Services Error: @faultcode - @faultstring', array('@faultcode' => $se->faultcode, '@faultstring' => $se->faultstring)));
    }
    catch (Exception $e) {
      $count--;

      globallink_menu_update_status($globallink, 'Error');
      watchdog('GlobalLink', 'Exception - %function - File[%file], Line[%line], Code[%code], Message[%message]', array('%function' => __FUNCTION__, '%file' => $e->getFile(), '%line' => $e->getLine(), '%code' => $e->getCode(), '%message' => $e->getMessage()), WATCHDOG_ERROR);
      form_set_error('', t('Error: @message', array('@message' => $e->getMessage())));
    }
  }

  return $count;
}

/**
 * Gets translated menu items from XML data.
 *
 * @param object $xml
 *   XML representation of menu items.
 *
 * @return array
 *   Array of menu items.
 */
function globallink_menu_get_translated_items($xml) {
  if (is_null($xml) || !is_string($xml) || $xml == '') {
    return array();
  }

  $dom = new DomDocument;
  $dom->preserveWhiteSpace = FALSE;
  $dom->loadXML($xml);

  $arr = array();

  $contents = $dom->getElementsByTagName('content');

  $mlid = '';

  foreach ($contents as $content) {
    if (is_null($content->attributes)) {
      continue;
    }

    foreach ($content->attributes as $attr_name => $attr_node) {
      if ($attr_name == 'mlid') {
        $mlid = $attr_node->value;
      }
    }
  }

  if ($mlid == '') {
    return array();
  }

  $arr['mlid'] = $mlid;

  $menus = $dom->getElementsByTagName('menu');

  foreach ($menus as $menu) {
    $type = '';

    foreach ($menu->attributes as $attr_name => $attr_node) {
      if ($attr_name == 'name') {
        $type = $attr_node->value;
      }
    }

    $arr[$type] = $menu->nodeValue;
  }

  return $arr;
}

/**
 * Updates menu status.
 *
 * @param object $globallink
 *   GlobalLink object.
 * @param string $status
 *   The new status.  Defaults to 'Pending Translations.'
 */
function globallink_menu_update_status(&$globallink, $status = 'Pending Translations') {
  db_update('globallink_core_menu')
  ->fields(array('status' => $status, 'timestamp' => REQUEST_TIME))
  ->condition('submission', $globallink->submissionName, '=')
  ->condition('document_ticket', $globallink->documentTicket, '=')
  ->condition('source', $globallink->sourceLocale, '=')
  ->condition('target', $globallink->targetLocale, '=')
  ->execute();
}

/**
 * Gets active menu submission rows by object ID.
 *
 * @param string $object_id
 *   The object ID.
 *
 * @return
 *   Associative array of menu active submission rows.  FALSE if the array is empty.
 */
function globallink_menu_get_active_submission_rows($object_id) {
  $query = db_select('globallink_core_menu', 'tc');
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

  foreach ($arr as $mlid => $mlid_arr) {
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
      $final_arr[$mlid] = $sub_arr;
    }
  }

  if (count($final_arr) > 0) {
    return $final_arr;
  }

  return FALSE;
}

/**
 * Gets active menu submission rows by menu ID.
 *
 * @param string $mlid
 *   The menu ID.
 *
 * @return
 *   Associative array of menu active submission rows.  FALSE if the array is empty.
 */
function globallink_menu_get_sent_rows_by_mlid($mlid) {
  $result = db_select('globallink_core_menu', 'tc')
  ->fields('tc')
  ->condition('object_id', $mlid, '=')
  ->condition('status', array('Sent for Translations', 'Error'), 'IN')
  ->execute();

  $rows = array();

  foreach ($result as $row) {
    $rows[] = $row;
  }

  return $rows;
}

/**
 * Gets sent menu rows from submission details.
 *
 * @param string $submission_ticket
 *   The submission ticket.
 * @param string $document_ticket
 *   The document ticket.
 * @param string $target_locale
 *   The target locale.
 *
 * @return
 *   Array of sent menu rows.  FALSE if the array is empty.
 */
function globallink_menu_get_sent_rows($submission_ticket, $document_ticket, $target_locale) {
  $query = db_select('globallink_core_menu', 'tc');
  $query->condition('submission_ticket', $submission_ticket, '=');
  $query->condition('document_ticket', $document_ticket, '=');
  $query->condition('target', $target_locale, '=');
  $query->fields('tc');

  $results = $query->execute();

  foreach ($results as $row) {
    return $row;
  }

  return FALSE;
}

/**
 * Gets menu row ID from submission details.
 *
 * @param string $submission_ticket
 *   The submission ticket.
 * @param string $document_ticket
 *   The document ticket.
 * @param string $target_locale
 *   The target locale.
 *
 * @return string
 *   The menu row ID.  FALSE if the row doesn't exist.
 */
function globallink_menu_get_row_id_from_submission($submission_ticket, $document_ticket, $target_locale) {
  $query = db_select('globallink_core_menu', 'tc');
  $query->condition('submission_ticket', $submission_ticket, '=');
  $query->condition('document_ticket', $document_ticket, '=');
  $query->condition('target', $target_locale, '=');
  $query->fields('tc');

  $results = $query->execute();

  foreach ($results as $row) {
    return $row->rid;
  }

  return FALSE;
}

/**
 * Loads menu data from menu ID.
 *
 * @param string $mlid
 *   The menu ID.
 *
 * @return array
 *   Array of menu data.
 */
function globallink_menu_load_data($mlid) {
  $query = db_select('menu_links', 'ml')
  ->fields('ml')
  ->condition('mlid', $mlid, '=');

  $results = $query->execute();

  $rows = array();

  foreach ($results as $item) {
    $rows[] = $item;
  }

  return $rows;
}

/**
 * Gets distinct active menu submission names.
 *
 * @return array
 *   Array of distinct active menu submission names.
 */
function globallink_menu_get_distinct_active_submission_names() {
  $query = db_select('globallink_core_menu', 'tc');
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
 * Updates menu submission status.
 *
 * @param string $submission_ticket
 *   The submission ticket.
 * @param string $status
 *   The submission status.  Defaults to 'Cancelled.'
 */
function globallink_menu_update_submission_status($submission_ticket, $status = 'Cancelled') {
  db_update('globallink_core_menu')
  ->fields(array('status' => $status, 'timestamp' => REQUEST_TIME))
  ->condition('submission_ticket', $submission_ticket, '=')
  ->execute();
}

/**
 * Gets menu submission status.
 */
function globallink_menu_get_submission_status() {
  module_load_include('inc', 'globallink', 'globallink_settings');
  module_load_include('inc', 'globallink', 'gl_ws/gl_ws_common');

  $query = db_select('globallink_core_menu', 'tc');
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
          globallink_menu_update_submission_status($row->submission_ticket);
        }
      }
      catch (SoapFault $se) {
        globallink_menu_update_submission_status($row->submission_ticket);
      }
      catch (Exception $ex) {
        globallink_menu_update_submission_status($row->submission_ticket);
      }
    }
  }
}

/**
 * Checks menu status based on row IDs.
 *
 * @param array $rids_arr
 *   Array of row IDs.
 *
 * @return array
 *   Array of row IDs that have been sent for translation or threw an error.
 */
function globallink_menu_check_status($rids_arr) {
  $status = TRUE;

  $query = db_select('globallink_core_menu', 'tc')
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
 * Clears cancelled menu documents.
 *
 * @return int
 *   Number of cancelled menu documents.
 */
function globallink_menu_clear_cancelled_documents() {
  $count = 0;

  $query = db_select('globallink_core_menu', 'tc')
  ->fields('tc', array('submission_ticket'))
  ->distinct()
  ->condition('status', 'Cancelled', '=');

  $results = $query->execute();

  foreach ($results as $item) {
    globallink_menu_update_submission_status($item->submission_ticket, 'Pending Translations');

    $count++;
  }

  return $count;
}
