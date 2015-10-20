<?php

/**
 * @file
 * The administration interface.
 */

/**
 * XLIFF export API for given nodes.
 */
function xliff_bulk_export($nids, $bilingual = FALSE) {
  $error_count = 0;
  $tar = array();
  foreach ($nids as $nid) {
    if ($bilingual) {
      $output = xliff_bilingual_export($nid, $message);
    }
    else {
      $output = xliff_export($nid);
    }
    if (empty($output)) {
      watchdog('xliff_bulkexport', 'Failed to export <a href="/node/@node">node @node</a>.', array('@node' => $nid), WATCHDOG_ERROR);
      $error_count++;
    }
    else {
      $node_language = db_query("SELECT n.language FROM {node} n WHERE n.nid = :nid", array(':nid' => $nid))->fetchField();
      $langcode = (!empty($node_language)) ? $node_language : language_default('language');
      $tar["node-$nid-$langcode.xlf"] = $output;
    }
  }

  $success_count = count($nids) - $error_count;
  if ($success_count) {
    drupal_set_message(t('%node_count exported.', array('%node_count' => format_plural($success_count, '1 node', '@count nodes'))));
  }
  if ($error_count) {
    $dblog_link = '';
    if (user_access('access site reports')) {
      $dblog_link = ' ' . l(t('View report'), 'admin/reports/dblog');
    }
    drupal_set_message(t('Failed to export %node_count.', array('%node_count' => format_plural($error_count, '1 node', '@count nodes'))) . $dblog_link, 'error');
  }

  if (empty($tar)) {
    return;
  }

  // Clear out output buffer to remove any garbage from tar output.
  if (ob_get_level()) {
    ob_end_clean();
  }

  $filename = 'xliff-bulk-export.tar';
  drupal_add_http_header('Content-type', 'application/x-tar');
  drupal_add_http_header('Content-Disposition', 'attachment; filename="' . $filename . '"');

  foreach ($tar as $filename => $content) {
    print xliff_bulk_tar_create($filename, $content);
  }
  print pack("a1024", "");
  exit;
}

/**
 * Menu callback; Admin form for xliff bulk export.
 */
function xliff_bulk_export_page() {
  $langcode = isset($_SESSION['xliff_bulk']['langcode']) ? $_SESSION['xliff_bulk']['langcode'] : language_default('language');
  $node_type = isset($_SESSION['xliff_bulk']['node_type']) ? $_SESSION['xliff_bulk']['node_type'] : 'page'; //@todo: fix default.
  $translation_status = isset($_SESSION['xliff_bulk']['translation_status']) ? $_SESSION['xliff_bulk']['translation_status'] : 0;
  $node_status = isset($_SESSION['xliff_bulk']['node_status']) ? $_SESSION['xliff_bulk']['node_status'] : -1;

  $build['filter'] = drupal_get_form('xliff_bulk_export_filter', $langcode, $node_type, $translation_status, $node_status);
  $build['admin'] = drupal_get_form('xliff_bulk_export_filtered_form', $langcode, $node_type, $translation_status, $node_status);
  return $build;
}

/**
 * Submit handler for the filter form.
 */
function xliff_bulk_export_filter_submit($form, &$form_state) {
  $_SESSION['xliff_bulk']['node_type'] = $form_state['values']['node_type'];
  $_SESSION['xliff_bulk']['translation_status'] = $form_state['values']['translation_status'];
  $_SESSION['xliff_bulk']['node_status'] = $form_state['values']['node_status'];
  $_SESSION['xliff_bulk']['langcode'] = $form_state['values']['langcode'];
}

/**
 * Submit handler for the admin form.
 */
function xliff_bulk_export_filtered_form_submit($form, &$form_state) {
  $bilingual = $form_state['values']['bilingual'];
  switch ($form_state['clicked_button']['#id']) {
    case 'edit-submit':
      $nids = array_filter($form_state['values']['nodes']);
      xliff_bulk_export($nids, $bilingual);
      break;

    case 'edit-submit-all':
      $nids = array();
      $node_type = $form_state['values']['node_type'];
      $langcode = $form_state['values']['langcode'];
      $status = $form_state['values']['node_status'];

      $query = db_select('node', 'n')
        ->fields('n', array('nid', 'nid'))
        ->condition('n.type', $node_type)
        ->condition('n.language', $langcode);

      if (in_array($status, array(0, 1))) {
        $query->condition('n.status', $status);
      }

      $nids = $query->execute()->fetchAllKeyed();
      xliff_bulk_export($nids, $bilingual);
      break;
  }
}

/**
 * Node type filter.
 */
function xliff_bulk_export_filter($form, &$form_state, $langcode, $node_type, $translation_status, $node_status) {
  $form['langcode'] = array(
    '#type' => 'select',
    '#title' => t('Language'),
    '#description' => t('Select the language to display.'),
    '#options' => array(LANGUAGE_NONE => t('Language neutral')) + locale_language_list('name', TRUE),
    '#default_value' => $langcode,
    '#prefix' => '<div id="language-filter">',
    '#suffix' => '</div>',
  );

  $form['node_type'] = array(
    '#type' => 'select',
    '#title' => t('Content type'),
    '#description' => t('Select the content types to display.'),
    '#options' => node_type_get_names(),
    '#default_value' => $node_type,
    '#prefix' => '<div id="node-type-filter">',
    '#suffix' => '</div>',
  );

  $status_options = array(
    -1 => '<' . t('Any') . '>',
    1 => t('published'),
    0 => t('not published'),
  );
  $form['node_status'] = array(
    '#type' => 'select',
    '#title' => t('Status'),
    '#description' => t('Select the content status.'),
    '#options' => $status_options,
    '#default_value' => $node_status,
    '#prefix' => '<div id="node-status-filter">',
    '#suffix' => '</div>',
  );

  // Include translation states if we have this module enabled
  /*if (module_exists('translation')) {
    $filters['status']['options'] += array(
      'translate-0' => t('Up to date translation'),
      'translate-1' => t('Outdated translation'),
    );
  }*/

  $form['translation_status'] = array(
    '#type' => 'hidden',
    '#title' => t('Translation status'),
    '#description' => t('Select the translation status.'),
    '#default_value' => $translation_status,
    '#prefix' => '<div id="translation-status-filter">',
    '#suffix' => '</div>',
  );

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['filter'] = array(
    '#type' => 'submit',
    '#value' => t('Filter'),
  );

  $form['#attached']['css'][] = drupal_get_path('module', 'xliff_bulk') . '/css/xliff-bulk-export.css';
  return $form;
}

/**
 * Form for bulk_export.
 *
 * @param $langcode
 *   filter by language code
 * @param $node_type
 *   filter by node type
 * @param $translation_status
 *   ?
 */
function xliff_bulk_export_filtered_form($form, &$form_state, $langcode, $node_type, $translation_status, $node_status) {
  $header = array(
    'title' => array('data' => t('Title'), 'field' => 'n.title'),
    'type' => array('data' => t('Type'), 'field' => 'n.type'),
    'language' => array('data' => t('Language'), 'field' => 'n.language'),
    'status' => array('data' => t('Status'), 'field' => 'n.status'),
    'changed' => array('data' => t('Updated'), 'field' => 'n.changed', 'sort' => 'desc'),
  );

  $query = db_select('node', 'n');
  $query
    ->condition('n.type', $node_type)
    ->condition('n.language', $langcode);

  if (in_array($node_status, array(0, 1))) {
    $query->condition('n.status', $node_status);
  }

  $count_query = clone $query;
  $count_query->addExpression('COUNT(n.nid)');

  $query = $query->extend('PagerDefault')->extend('TableSort');
  $query
    ->fields('n', array('nid', 'title', 'type', 'language', 'status', 'changed'))
    ->limit(50)
    ->orderByHeader($header)
    ->setCountQuery($count_query);

  $result = $query->execute();

  $options = array();
  $node_types = node_type_get_names();
  $languages = language_list();
  $status_options = array(0 => t('not published'), 1 => t('published'));
  foreach ($result as $node) {
    $options[$node->nid] = array(
      'title' => array('data' => l($node->title, 'node/' . $node->nid), 'class' => 'node-title'),
      'type' => array('data' => check_plain($node_types[$node->type]), 'class' => 'node-type'),
      'language' => array('data' => ($node->language == LANGUAGE_NONE) ? t('Language neutral') : t($languages[$node->language]->name), 'class' => 'node-language'),
      'status' => array('data' => $status_options[$node->status], 'class' => 'node-status'),
      'changed' => array('data' => format_date($node->changed, 'short'), 'class' => 'node-changed'),
    );
  }

  $form['nodes'] = array(
    '#type' => 'tableselect',
    '#header' => $header,
    '#options' => $options,
    '#attributes' => array('id' => 'xliff-bulk-export-node-list'),
    '#empty' => t('No content available.'),
  );
  $form['pager'] = array('#markup' => theme('pager'));

  $form['bilingual'] = array(
    '#type' => 'checkbox',
    '#title' => t('Bilingual mode'),
    '#default_value' => 0,
    '#description' => t('The XLIFF Export will be done in bilingual mode. Can be used on translations only. There is no guarantee that this export will work as node translations may have different html format, therefore the source and target strings could not be paired.'),
  );

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Export selected'),
  );

  $form['actions']['submit_all'] = array(
    '#type' => 'submit',
    '#value' => t('Export all'),
  );
  // @todo: This should be automatic somehow.
  $form['help'] = array(
    '#markup' => '<br />' . t('After file save please reload the page to see the result count and potential errors.') . '<br />',
  );
  return $form;
}

/**
 * Tar creation function. Written by dmitrig01.
 *
 * @param $name
 *   Filename of the file to be tarred.
 * @param $contents
 *   String contents of the file.
 *
 * @return
 *   A string of the tar file contents.
 */
function xliff_bulk_tar_create($name, $contents) {
  $tar = '';
  $binary_data_first = pack("a100a8a8a8a12A12",
    $name,
    '100644 ', // File permissions
    '   765 ', // UID,
    '   765 ', // GID,
    sprintf("%11s ", decoct(strlen($contents))), // Filesize,
    sprintf("%11s", decoct(REQUEST_TIME)) // Creation time
  );
  $binary_data_last = pack("a1a100a6a2a32a32a8a8a155a12", '', '', '', '', '', '', '', '', '', '');

  $checksum = 0;
  for ($i = 0; $i < 148; $i++) {
    $checksum += ord(substr($binary_data_first, $i, 1));
  }
  for ($i = 148; $i < 156; $i++) {
    $checksum += ord(' ');
  }
  for ($i = 156, $j = 0; $i < 512; $i++, $j++) {
    $checksum += ord(substr($binary_data_last, $j, 1));
  }

  $tar .= $binary_data_first;
  $tar .= pack("a8", sprintf("%6s ", decoct($checksum)));
  $tar .= $binary_data_last;

  $buffer = str_split($contents, 512);
  foreach ($buffer as $item) {
    $tar .= pack("a512", $item);
  }
  return $tar;
}
