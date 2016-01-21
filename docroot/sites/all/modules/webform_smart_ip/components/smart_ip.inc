<?php
/**
 * @file
 * Provide Webform component for Brightdoor list signups
 */

/**
 * Implementation of _webform_defaults_component().
 */
function _webform_defaults_smart_ip() {
  return array(
    'name' => '',
    'form_key' => NULL,
    'mandatory' => 0,
    'pid' => 0,
    'weight' => 0,
    'extra' => array(
      'smart_ip_type' => NULL,
      'private' => TRUE,
      'field_prefix' => '',
      'field_suffix' => '',
      'analysis' => TRUE,
    ),
  );
}

/**
 * Implementation of _webform_edit_component().
 */
function _webform_edit_smart_ip($component) {
  // Load current users location information from Smart IP
  $smart_ip_session = smart_ip_session_get('smart_ip');

  // Build list of fields returned by Smart IP
  $smart_ip_fields = array();
  foreach ($smart_ip_session['location'] as $field => $default_value) {
    // Skip fields without values in the database
    // Skip fields not supported by the IP to location service being used.
    // Skip timestamp field since it's already being collected.
    if (!empty($default_value) && strpos($default_value, 'This field is not supported') === FALSE && $field != 'timestamp') {
      $field_name = ucwords(str_replace('_', ' ', $field));
      $field_name = str_replace('Ip', 'IP', $field_name);
      $smart_ip_fields[$field] = $field_name . ' (Ex: ' . $default_value . ')';
    }
  }

  // Build form field for type of Smart IP field.
  $form = array();
  $form['extra']['smart_ip_type'] = array(
    '#type' => 'radios',
    '#title' => t('Type of field'),
    '#options' => $smart_ip_fields,
    '#description' => t('Select the Smart IP field type which you want stored here.'),
    '#required' => TRUE,
    '#default_value' => isset($component['extra']['smart_ip_type']) ? $component['extra']['smart_ip_type'] : 'country_code',
  );

  return $form;
}

/**
 * Implementation of _webform_render_component().
 */
function _webform_render_smart_ip($component, $value = NULL) {
  $form_item = array(
    '#type' => 'hidden',
    '#title' => $component['name'],
    '#required' => $component['mandatory'],
    '#weight' => $component['weight'],
    '#default_value' => isset($value) ? $value[0] : $component['value'],
  );

  return $form_item;
}

/**
 * Implementation of _webform_submit_component().
 */
function _webform_submit_smart_ip($component, $value) {
  return $value;
}


/**
 * Implementation of _webform_display_component().
 */
function _webform_display_smart_ip($component, $value, $format = 'html') {
  return array(
    '#title' => $component['name'],
    '#weight' => $component['weight'],
    '#theme' => 'webform_display_textfield',
    '#theme_wrappers' => $format == 'html' ? array('webform_element') : array('webform_element_text'),
    '#post_render' => array('webform_element_wrapper'),
    '#field_prefix' => $component['extra']['field_prefix'],
    '#field_suffix' => $component['extra']['field_suffix'],
    '#component' => $component,
    '#format' => $format,
    '#value' => isset($value[0]) ? $value[0] : '',
  );
}

/**
 * Implementation of _webform_table_component().
 */
function _webform_table_smart_ip($component, $value) {
  return check_plain(empty($value[0]) ? '' : $value[0]);
}

/**
 * Implementation of _webform_csv_data_component().
 */
function _webform_csv_data_smart_ip($component, $export_options, $value) {
  return !isset($value[0]) ? '' : $value[0];
}


/**
 * Implementation of _webform_csv_headers_component().
 */
function _webform_csv_headers_smart_ip($component, $export_options) {

  $header = array();
  $header[2] = $component['name'];
  return $header;
}

function _webform_analysis_smart_ip($component, $sids = array(), $single = TRUE) {
  $query = db_select('webform_submitted_data', 'wsd')
    ->fields('wsd', array('no', 'data'))
    ->condition('nid', $component['nid'])
    ->condition('cid', $component['cid'])
    ->condition('data', '', '<>')
    ->groupBy('no')
    ->groupBy('data');
  $query->addExpression('COUNT(sid)', 'datacount');

  if (count($sids)) {
    $query->condition('sid', $sids, 'IN');
  }

  $results = $query->execute();

  // Store our results in an array as value => count
  $rows = array();
  foreach ($results as $result) {
    $rows[] = array($result->data, $result->datacount);
  }

  return array(
    'table_rows' => $rows,
  );
}
