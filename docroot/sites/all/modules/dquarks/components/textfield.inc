<?php

/**
 * @file
 * Quiz module email component.
 */

/**
 * Create a default textfield component.
 */
function _dquarks_defaults_textfield() {
  return array(
    'name' => '',
    'form_key' => NULL,
    'pid' => 0,
    'weight' => 0,
    'value' => '',
    'mandatory' => 0,
    'email' => 1,
    'extra' => array(
      'width' => '',
      'maxlength' => '',
      'field_prefix' => '',
      'field_suffix' => '',
      'disabled' => 0,
      'description' => '',
      'attributes' => array(),
    ),
  );
}

/**
 * Create a set of form items to be displayed on the form.
 *
 * @return array
 *   An array of form items to be displayed on the edit component page.
 */
function _dquarks_edit_textfield($currfield) {
  $edit_fields = array();
  $edit_fields['value'] = array(
    '#type' => 'textfield',
    '#title' => t('Default value'),
    '#default_value' => $currfield['value'],
    '#description' => t('The default value of the field.') . theme('dquarks_token_help'),
    '#size' => 60,
    '#maxlength' => 127,
    '#weight' => 0,
  );
  $edit_fields['extra']['width'] = array(
    '#type' => 'textfield',
    '#title' => t('Width'),
    '#default_value' => $currfield['extra']['width'],
    '#description' => t('Width of the textfield.') . ' ' . t('Leaving blank will use the default size.'),
    '#size' => 5,
    '#maxlength' => 10,
    '#weight' => 0,
  );
  $edit_fields['extra']['maxlength'] = array(
    '#type' => 'textfield',
    '#title' => t('Maxlength'),
    '#default_value' => $currfield['extra']['maxlength'],
    '#description' => t('Maxlength of the textfield.'),
    '#size' => 5,
    '#maxlength' => 10,
    '#weight' => 0,
  );
  $edit_fields['extra']['field_prefix'] = array(
    '#type' => 'textfield',
    '#title' => t('Label placed to the left of the textfield'),
    '#default_value' => $currfield['extra']['field_prefix'],
    '#description' => t('Examples: $, #, -.'),
    '#size' => 20,
    '#maxlength' => 127,
    '#weight' => 1.1,
  );
  $edit_fields['extra']['field_suffix'] = array(
    '#type' => 'textfield',
    '#title' => t('Label placed to the right of the textfield'),
    '#default_value' => $currfield['extra']['field_suffix'],
    '#description' => t('Examples: lb, kg, %.'),
    '#size' => 20,
    '#maxlength' => 127,
    '#weight' => 1.2,
  );
  $edit_fields['extra']['disabled'] = array(
    '#type' => 'checkbox',
    '#title' => t('Disabled'),
    '#return_value' => 1,
    '#description' => t('Make this field non-editable. Useful for setting an unchangeable default value.'),
    '#weight' => 3,
    '#default_value' => $currfield['extra']['disabled'],
  );
  return $edit_fields;
}

/**
 * Build a form item array containing all the properties of this component.
 *
 * @param array $component
 *   An array of information describing the component, directly correlating to
 *   the dquarks_component database schema.
 *
 * @return array
 *   An array of a form item to be displayed on the client-side dquarks.
 */
function _dquarks_render_textfield($component) {
  $form_item = array(
    '#type' => $component['type'],
    '#title' => $component['name'],
    '#default_value' => _dquarks_filter_values($component['value'], NULL, NULL, FALSE),
    '#required' => $component['mandatory'],
    '#weight' => $component['weight'],
    '#field_prefix' => empty($component['extra']['field_prefix']) ? NULL : check_plain($component['extra']['field_prefix']),
    '#field_suffix' => empty($component['extra']['field_suffix']) ? NULL : check_plain($component['extra']['field_suffix']),
    '#description' => _dquarks_filter_descriptions($component['extra']['description']),
    '#attributes' => $component['extra']['attributes'],
    '#prefix' => '<div class="dquarks-component-' . $component['type'] . '" id="dquarks-component-' . $component['form_key'] . '">',
    '#suffix' => '</div>',
  );

  if ($component['extra']['disabled']) {
    $form_item['#attributes']['readonly'] = 'readonly';
  }

  // Change the 'width' option to the correct 'size' option.
  if ($component['extra']['width'] > 0) {
    $form_item['#size'] = $component['extra']['width'];
  }
  if ($component['extra']['maxlength'] > 0) {
    $form_item['#maxlength'] = $component['extra']['maxlength'];
  }

  return $form_item;
}

/**
 * Display the result of a textfield submission.
 *
 * @param array $data
 *   An array of information containing the submission result, directly
 *   correlating to the dquarks_submitted_data database schema.
 * @param array $component
 *   An array of information describing the component, directly correlating to
 *   the dquarks_component database schema.
 * @param boolean $enabled
 *   If enabled, the value may be changed. Otherwise it will set to readonly.
 *
 * @return array
 *   Textual output formatted for human reading.
 */
function _dquarks_submission_display_textfield($data, $component, $enabled = FALSE) {
  $form_item = _dquarks_render_textfield($component);
  $form_item['#default_value'] = $data['value']['0'];
  if (!$enabled) {
    $form_item['#attributes']['readonly'] = 'readonly';
  }
  return $form_item;
}

/**
 * Format the output of emailed data for this component.
 *
 * @param array $variables
 *   An array of information describing the component,
 *   directly correlating to the dquarks_component database schema.
 *
 * @return string
 *   Textual output to be included in the email.
 */
function theme_dquarks_mail_textfield($variables) {
  $data = $variables['data'];
  $component = $variables['component'];
  $value = $data == '' ? '' : ' ' . $component['extra']['field_prefix'] . $data . $component['extra']['field_suffix'];
  return $component['name'] . ':' . $value;
}

/**
 * Module specific instance of hook_theme().
 */
function _dquarks_theme_textfield() {
  return array(
    'dquarks_mail_textfield' => array(
      'arguments' => array('data' => NULL, 'component' => NULL),
    ),
  );
}

/**
 * Module specific instance of hook_help().
 */
function _dquarks_help_textfield($section) {
  switch ($section) {
    case 'admin/settings/dquarks#textfield_description':
      return t('Basic textfield type.');
  }
}

/**
 * Calculate and returns statistics about results for this component.
 *
 * @param array $component
 *   An array of information describing the component, directly correlating to
 *   the dquarks_component database schema
 * @param array $sids
 *   An optional array of submission IDs (sid). If supplied, the analysis will
 *   be limited to these sids.
 *
 * @return array
 *   An array of data rows, each containing a statistic for this component's
 *   submissions.
 */
function _dquarks_analysis_rows_textfield($component, $sids = array()) {
  $sidfilter = count($sids) ? " AND sid IN :sids " : "";

  $result = db_query('SELECT data FROM {dquarks_submitted_data} WHERE nid = :nid AND cid = :cid' . $sidfilter, array(
    ':nid' => $component['nid'],
    ':cid' => $component['cid'],
    ':sids' => $sids)
  );


  $nonblanks = 0;
  $submissions = 0;
  $wordcount = 0;
  $rows = array();
  foreach ($result as $data) {
    if (drupal_strlen(trim($data->data)) > 0) {
      $nonblanks++;
      $wordcount += str_word_count(trim($data->data));
    }
    $submissions++;
  }

  $rows[0] = array(t('Left Blank'), ($submissions - $nonblanks));
  $rows[1] = array(t('User entered value'), $nonblanks);
  $rows[2] = array(
    t('Average submission length in words (ex blanks)'),
    ($nonblanks != 0 ? number_format($wordcount / $nonblanks, 2) : '0'),
  );
  return $rows;
}

/**
 * Return the result of this component's submission for display in a table.
 *
 * @param array $data
 *   An array of information containing the submission result, directly
 *   correlating to the dquarks_submitted_data database schema
 *
 * @return string
 *   Textual output formatted for human reading.
 */
function _dquarks_table_data_textfield($data) {
  return check_plain(empty($data['value']['0']) ? '' : $data['value']['0']);
}

/**
 * Return the header information for this component.
 *
 * @param array $component
 *   An array of information describing the component, directly correlating to
 *   the dquarks_component database schema.
 *
 * @return array
 *   An array of data to be displayed in the first three rows of a Excel file,
 *   not including either prefixed or trailing commas.
 */
function _dquarks_xlsx_headers_textfield($component) {
  $header = array(
    array(array('colspan' => 0, 'value' => '')),
    array(array('colspan' => 0, 'value' => '')),
    array(array('colspan' => 0, 'value' => $component['name'])),
  );
  return $header;
}

/**
 * Return the result of a textfield submission.
 *
 * @param array $data
 *   An array of information containing the submission result, directly
 *   correlating to the dquarks_submitted_data database schema.
 *
 * @return array
 *   Textual output formatted for Excel, not including either prefixed or
 *   trailing commas.
 */
function _dquarks_xlsx_data_textfield($data) {
  // The output of this function will be displayed under the "results" tab then
  // "submissions".
  return !isset($data['value']['0']) ? array(
    array(
      'colspan' => 0,
      'value' => ''),
  ) : array(
    array(
      'colspan' => 0,
      'value' => $data['value']['0'],
    ),
  );
}
