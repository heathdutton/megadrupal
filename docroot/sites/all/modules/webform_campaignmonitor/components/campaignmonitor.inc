<?php

/**
 * @file
 * Webform Campaign Monitor component.
 */

/**
 * Implements _webform_defaults_component().
 */
function _webform_defaults_campaignmonitor() {
  return array(
    'name' => '',
    'form_key' => NULL,
    'pid' => 0,
    'weight' => 0,
    'value' => '',
    'required' => 0,
    'extra' => array(
      'lists' => array(),
      'fields_mappings' => array(),
      'checked' => 0,
      'description' => '',
      'private' => FALSE,
    ),
  );
}

/**
 * Implements _webform_theme_component().
 */
function _webform_theme_campaignmonitor() {
  return array(
    'webform_display_campaignmonitor' => array(
      'render element' => 'element',
    ),
  );
}

/**
 * Implements _webform_edit_component().
 */
function _webform_edit_campaignmonitor($component) {
  $form = array();

  // Connect to the Campaign Monitor API and pull lists info.
  $cm = CampaignMonitor::getConnector();
  $lists_info = $cm->getLists();

  if (!empty($lists_info)) {
    $form['extra']['lists'] = _webform_campaignmonitor_component_list_form($component, $lists_info);
    $form['extra']['fields_mappings'] = _webform_campaignmonitor_component_fields_mappings_form($component, $lists_info);
  }
  else {
    drupal_set_message(t('There are no available lists to subscribe to at the moment.'), 'warning');
  }

  $form['display']['checked'] = array(
    '#type' => 'checkbox',
    '#title' => t('Checked by default'),
    '#default_value' => $component['extra']['checked'],
    '#description' => t('Check this option if this field should be checked by default.'),
    '#weight' => 5,
    '#parents' => array('extra', 'checked'),
  );

  return $form;
}

/**
 * Implements _webform_render_component().
 */
function _webform_render_campaignmonitor($component, $value = NULL, $filter = TRUE, $submission = NULL) {
  $node = isset($component['nid']) ? node_load($component['nid']) : NULL;

  $element = array(
    '#type' => 'checkbox',
    '#title' => $filter ? webform_filter_xss($component['name']) : $component['name'],
    '#default_value' => $value ? reset($value) : $component['extra']['checked'],
    '#required' => $component['required'],
    '#weight' => $component['weight'],
    '#description' => $filter ? webform_filter_descriptions($component['extra']['description'], $node) : $component['extra']['description'],
    '#theme_wrappers' => array('webform_element'),
    '#post_render' => array('webform_element_wrapper'),
    '#translatable' => array('title', 'description'),
  );

  return $element;
}

/**
 * Implements _webform_display_component().
 */
function _webform_display_campaignmonitor($component, $value, $format = 'html', $submission = array()) {
  return array(
    '#title' => $component['name'],
    '#weight' => $component['weight'],
    '#theme' => 'webform_display_campaignmonitor',
    '#theme_wrappers' => $format == 'html' ? array('webform_element') : array('webform_element_text'),
    '#format' => $format,
    '#value' => isset($value[0]) ? $value[0] : '',
    '#translatable' => array('title', 'description'),
  );
}

/**
 * Format the output of data for this component.
 */
function theme_webform_display_campaignmonitor($variables) {
  $element = $variables['element'];
  return $element['#value'] ? t('Yes') : t('No');
}

/**
 * Implements _webform_analysis_component().
 */
function _webform_analysis_campaignmonitor($component, $sids = array(), $single = FALSE, $join = NULL) {
  $query = db_select('webform_submitted_data', 'wsd', array('fetch' => PDO::FETCH_ASSOC))
    ->fields('wsd', array('data'))
    ->condition('wsd.nid', $component['nid'])
    ->condition('wsd.cid', $component['cid']);

  if (count($sids)) {
    $query->condition('wsd.sid', $sids, 'IN');
  }

  if ($join) {
    $query->innerJoin($join, 'ws2_', 'wsd.sid = ws2_.sid');
  }

  $nonblanks = 0;
  $submissions = 0;

  $result = $query->execute();
  foreach ($result as $data) {
    if ($data['data']) {
      $nonblanks++;
    }
    $submissions++;
  }

  $rows = array();
  $rows[0] = array(t("User didn't check the box"), ($submissions - $nonblanks));
  $rows[1] = array(t('User did check the box'), $nonblanks);

  return $rows;
}

/**
 * Implements _webform_table_component().
 */
function _webform_table_campaignmonitor($component, $value) {
  return $value[0] ? t('Yes') : t('No');
}

/**
 * Implements _webform_csv_headers_component().
 */
function _webform_csv_headers_campaignmonitor($component, $export_options) {
  $header = array();

  $header[0] = '';
  $header[1] = '';
  $header[2] = $export_options['header_keys'] ? $component['form_key'] : $component['name'];

  return $header;
}

/**
 * Implements _webform_csv_data_component().
 */
function _webform_csv_data_campaignmonitor($component, $export_options, $value) {
  return $value[0] ? t('Yes') : t('No');
}
