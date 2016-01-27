<?php
/**
 * @file
 * Webform module serial component.
 */

/**
 * Implements _webform_defaults_component().
 */
function _webform_defaults_serial() {
  return array(
    'name' => '',
    'form_key' => NULL,
    'pid' => 0,
    'weight' => 0,
    'value' => '',
    'mandatory' => 0,
    'extra' => array(
      'initial' => 1,
      'increment' => 1,
      'private' => FALSE,
    ),
  );
}

/**
 * Implements _webform_theme_component().
 */
function _webform_theme_serial() {
  return array(
    'webform_display_serial' => array(
      'render element' => 'element',
      'file' => 'components/serial.inc',
      'path' => drupal_get_path('module', 'webform_serial'),
    ),
  );
}

/**
 * Implements _webform_edit_component().
 */
function _webform_edit_serial($component) {
  $form = array();
  $form['extra']['initial'] = array(
    '#type' => 'textfield',
    '#title' => t('Initial/next value'),
    '#default_value' => empty($component['extra']['initial']) ? 1 : $component['extra']['initial'],
    '#description' => t('The value of the next serial number. This is usually 1 when you start and will go up with each form submission.'),
    '#size' => 8,
    '#maxlength' => 1024,
    '#weight' => 0,
    '#element_validate' => array('_webform_edit_serial_validate'),
  );
  $form['extra']['increment'] = array(
    '#type' => 'textfield',
    '#title' => t('Increment value'),
    '#default_value' => empty($component['extra']['increment']) ? 1 : $component['extra']['increment'],
    '#description' => t('The increment value of the field. Each time the form is submitted, add this many to the serial field.'),
    '#size' => 8,
    '#maxlength' => 1024,
    '#weight' => 0,
    '#element_validate' => array('_webform_edit_serial_validate'),
  );
  return $form;
}

/**
 * Validation of serial edit form items.
 *
 * Ensure the initial and increment values are positive integers.
 */
function _webform_edit_serial_validate($element, &$form_state) {
  switch ($element['#name']) {
    case 'extra[initial]':
    case 'extra[increment]':
      $original_value = $form_state['values']['extra'][substr($element['#name'], 6, -1)];
      $value = (int) $original_value;
      if ($value < 1 || $value != $original_value) {
        form_error($element, t('The %title needs to be an integer greater than zero.', array('%title' => $element['#title'])));
      }
      break;
  }
}

/**
 * Implements _webform_render_component().
 */
function _webform_render_serial($component, $value = NULL, $filter = TRUE) {
  return array(
    '#title' => $filter ? _webform_filter_xss($component['name']) : $component['name'],
    '#weight' => $component['weight'],
    '#translatable' => array('title'),
  );
}

/**
 * Implements _webform_display_component().
 */
function _webform_display_serial($component, $value, $format = 'html') {
  return array(
    '#title' => $component['name'],
    '#weight' => $component['weight'],
    '#theme' => 'webform_display_serial',
    '#theme_wrappers' => $format == 'html' ? array('webform_element') : array('webform_element_text'),
    '#format' => $format,
    '#value' => isset($value[0]) ? $value[0] : '',
    '#translatable' => array('title'),
    '#field_suffix' => NULL,
    '#field_prefix' => NULL,
  );
}
/**
 * Format the output of data for this component.
 */
function theme_webform_display_serial($variables) {
  $element = $variables['element'];
  $prefix = $element['#format'] == 'html' ? '' : $element['#field_prefix'];
  $suffix = $element['#format'] == 'html' ? '' : $element['#field_suffix'];
  $value = $element['#format'] == 'html' ? check_plain($element['#value']) : $element['#value'];
  return $value !== '' ? ($prefix . $value . $suffix) : ' ';
}

/**
 * Implements _webform_analysis_component().
 */
function _webform_analysis_serial($component, $sids = array(), $single = FALSE) {
  $query = db_select('webform_submitted_data', 'wsd', array('fetch' => PDO::FETCH_ASSOC))
    ->condition('nid', $component['nid'])
    ->condition('cid', $component['cid'])
    // Empty would get CAST() to zero.
    ->condition('data', '', '!=');
  if (count($sids)) {
    $query->condition('sid', $sids, 'IN');
  }
  // Must CAST() to "decimal" because MySQL does not support "integer".
  $query->addExpression('MIN(CAST(data AS decimal))', 'minimum');
  $query->addExpression('MAX(CAST(data AS decimal))', 'maximum');
  $query->addExpression('COUNT(*)', 'count');
  $results = $query->execute()->fetch();

  $rows = array();
  $rows[] = array(t('Lowest serial number'), $results['minimum']);
  $rows[] = array(t('Highest serial number'), $results['maximum']);
  $rows[] = array(t('Number of submissions with a serial number'), $results['count']);

  return array(
    'table_rows' => $rows,
  );
}

/**
 * Implements _webform_table_component().
 */
function _webform_table_serial($component, $value) {
  return check_plain(empty($value[0]) ? '' : $value[0]);
}

/**
 * Implements _webform_csv_headers_component().
 */
function _webform_csv_headers_serial($component, $export_options) {
  $header = array();
  $header[0] = '';
  $header[1] = '';
  $header[2] = $component['name'];
  return $header;
}

/**
 * Implements _webform_csv_data_component().
 */
function _webform_csv_data_serial($component, $export_options, $value) {
  return $value;
}

/**
 * Implements hook_webform_submission_presave().
 */
function webform_serial_webform_submission_presave($node, &$submission) {
  foreach ($node->webform['components'] as $cid => $component) {
    // If component is a serial number with no value, generate a value.
    if ($component['type'] == 'serial' && empty($submission->data[$cid])) {
      // Calculate the increment (default to 1).
      $increment = empty($component['extra']['increment']) ? 1 : (int) $component['extra']['increment'];

      // Get largest serial number in the database to ensure the new one is larger.
      $query = db_select('webform_submitted_data')
        ->condition('nid', $node->nid)
        ->condition('cid', $cid);
      // CAST() is needed because value is stored as text but must be sorted as a number.
      // Must CAST() to "decimal" because MySQL does not support "integer".
      $query->addExpression('MAX(CAST(data AS decimal))');
      $current_max = (int) $query->execute()->fetchField();

      // Use a transaction with SELECT ... FOR UPDATE to lock the row between
      // the SELECT and the UPDATE, ensuring that multiple Webform submissions
      // at the same time do not have duplicate numbers.
      $txn = db_transaction();

      // Get the next serial number as configured in the component.
      $component_extra = db_select('webform_component', 'wc')
        ->forUpdate()
        ->fields('wc', array('extra'))
        ->condition('nid', $node->nid)
        ->condition('cid', $cid)
        ->execute()
        ->fetchField();
      $component_extra = unserialize($component_extra);
      $next_value = empty($component['extra']['initial']) ? 1 : $component['extra']['initial'];

      // If the next value is smaller than the largest value currently in the
      // database, increment the largest value and use that as the next value.
      if ($next_value <= $current_max) {
        $next_value = $current_max + $increment;
      }

      // Increment the next value and save it in the database.
      $component_extra['initial'] = $next_value + $increment;
      db_update('webform_component')
        ->fields(array('extra' => serialize($component_extra)))
        ->condition('nid', $node->nid)
        ->condition('cid', $cid)
        ->execute();

      // Set the serial number in the submission data in a way that is
      // compatible with Webform 3 and Webform 4. If version is not present,
      // such as when working from git, assume Webform 4.
      $value = array($next_value);
      $version = system_get_info('module', 'webform');
      $version = explode('-', $version['version']);
      if (isset($version[1]) && ((int) $version[1] < 4)) {
        $submission->data[$cid]['value'] = $value;
      }
      else {
        $submission->data[$cid] = $value;
      }
    }
  }
}
