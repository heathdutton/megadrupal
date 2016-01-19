<?php

/**
 * @file
 * Autocomplete component.
 */

// We treat textfield as a sort of "parent class"
module_load_include('inc', 'webform', 'components/textfield');

/**
 * Implements _webform_defaults_component().
 */
function _webform_defaults_autocomplete() {
  $defaults = _webform_defaults_textfield();
  $defaults['extra'] += array(
    'autocomplete_items' => '',
    'autocomplete_existing' => 0,
    'autocomplete_match_rule' => WEBFORM_AUTOCOMPLETE_MATCH_CONTAINS,
    'autocomplete_restrict' => 0,
    'autocomplete_result_count' => 20,
  );
  return $defaults;
}

/**
 * Implements _webform_edit_component().
 */
function _webform_edit_autocomplete($component) {
  $form = array();
  $form['value'] = array(
    '#type' => 'textfield',
    '#title' => t('Default value'),
    '#default_value' => $component['value'],
    '#description' => t('The default value of the field.') . theme('webform_token_help'),
    '#size' => 60,
    '#maxlength' => 1024,
    '#weight' => 0,
  );
  $form['options']['autocomplete_items'] = array(
    '#type' => 'textarea',
    '#title' => t('Options'),
    '#default_value' => $component['extra']['autocomplete_items'],
    '#description' => t('One value per line. Unlike select elements, this field does not use key|value pairs - enter values only.'),
    '#cols' => 60,
    '#rows' => 5,
    '#weight' => 0,
    '#required' => FALSE,
    '#wysiwyg' => FALSE,
    '#parents' => array('extra', 'autocomplete_items'),
  );
  $form['display']['autocomplete_result_count'] = array(
    '#type' => 'textfield',
    '#title' => t('Result Count'),
    '#default_value' => $component['extra']['autocomplete_result_count'],
    '#description' => t('Maximum number of autocomplete results to display.'),
    '#size' => 5,
    '#weight' => -1,
    '#maxlength' => 2,
    '#required' => TRUE,
    '#parents' => array('extra', 'autocomplete_result_count'),
  );
  $form['display']['width'] = array(
    '#type' => 'textfield',
    '#title' => t('Width'),
    '#default_value' => $component['extra']['width'],
    '#description' => t('Width of the textfield.') . ' ' . t('Leaving blank will use the default size.'),
    '#size' => 5,
    '#maxlength' => 10,
    '#weight' => 0,
    '#parents' => array('extra', 'width'),
  );
  $form['display']['placeholder'] = array(
    '#type' => 'textfield',
    '#title' => t('Placeholder'),
    '#default_value' => $component['extra']['placeholder'],
    '#description' => t('The placeholder will be shown in the field until the user starts entering a value.'),
    '#weight' => 1,
    '#parents' => array('extra', 'placeholder'),
  );
  $form['display']['field_prefix'] = array(
    '#type' => 'textfield',
    '#title' => t('Label placed to the left of the textfield'),
    '#default_value' => $component['extra']['field_prefix'],
    '#description' => t('Examples: $, #, -.'),
    '#size' => 20,
    '#maxlength' => 127,
    '#weight' => 1.1,
    '#parents' => array('extra', 'field_prefix'),
  );
  $form['display']['field_suffix'] = array(
    '#type' => 'textfield',
    '#title' => t('Label placed to the right of the textfield'),
    '#default_value' => $component['extra']['field_suffix'],
    '#description' => t('Examples: lb, kg, %.'),
    '#size' => 20,
    '#maxlength' => 127,
    '#weight' => 1.2,
    '#parents' => array('extra', 'field_suffix'),
  );
  $form['options']['autocomplete_existing'] = array(
    '#type' => 'checkbox',
    '#title' => t('Autocomplete from existing submissions'),
    '#return_value' => 1,
    '#description' => t('Autocomplete from previous values entered in this field. If options are entered above, autocomplete will select from both.'),
    '#weight' => 1,
    '#default_value' => $component['extra']['autocomplete_existing'],
    '#parents' => array('extra', 'autocomplete_existing'),
  );
  $form['options']['autocomplete_match_rule'] = array(
    '#type' => 'select',
    '#title' => t('Match Rule'),
    '#options' => array(
      WEBFORM_AUTOCOMPLETE_MATCH_BEGIN => t('starts with'),
      WEBFORM_AUTOCOMPLETE_MATCH_CONTAINS => t('contains'),
      WEBFORM_AUTOCOMPLETE_MATCH_END => t('ends with'),
    ),
    '#parents' => array('extra', 'autocomplete_match_rule'),
    '#default_value' => $component['extra']['autocomplete_match_rule'],
  );
  $form['validation']['unique'] = array(
    '#type' => 'checkbox',
    '#title' => t('Unique'),
    '#return_value' => 1,
    '#description' => t('Require all entered values for this field to be unique. The same value will not autocomplete twice.'),
    '#weight' => 1,
    '#default_value' => $component['extra']['unique'],
    '#parents' => array('extra', 'unique'),
  );
  $form['validation']['autocomplete_restrict'] = array(
    '#type' => 'checkbox',
    '#title' => t('Restrict to listed values'),
    '#return_value' => 1,
    '#description' => t('Only allow options entered here to be chosen. Other input will be rejected.'),
    '#weight' => 1,
    '#default_value' => $component['extra']['autocomplete_restrict'],
    '#parents' => array('extra', 'autocomplete_restrict'),
  );
  $form['#validate'][] = '_webform_autocomplete_admin_validate';
  return $form;
}

/**
 * Validation callback for component edit form.
 */
function _webform_autocomplete_admin_validate($form, &$form_state) {
  $extra = $form_state['values']['extra'];
  if ($extra['autocomplete_restrict'] && !$extra['autocomplete_items']) {
    form_error($form['validation']['autocomplete_restrict'], t('You have chosen to restrict submissions to listed options, but have not entered any options.'));
  }
  if ($extra['autocomplete_restrict'] && $extra['autocomplete_existing']) {
    form_error($form['validation']['autocomplete_restrict'], t('You cannot choose both "Restrict to listed values" and "Autocomplete from existing submissions."'));
  }
  if (!is_numeric($extra['autocomplete_result_count']) || $extra['autocomplete_result_count'] < 1) {
    form_error($form['display']['autocomplete_result_count'], t('You must choose to display at least one result'));
  }
}

/**
 * Implements _webform_render_component().
 */
function _webform_render_autocomplete($component, $value = NULL, $filter = TRUE) {
  $node = isset($component['nid']) ? node_load($component['nid']) : NULL;

  $element = array(
    '#type' => 'textfield',
    '#title' => $filter ? webform_filter_xss($component['name']) : $component['name'],
    '#title_display' => $component['extra']['title_display'] ? $component['extra']['title_display'] : 'before',
    '#default_value' => $filter ? webform_replace_tokens($component['value'], $node, NULL, NULL, FALSE) : $component['value'],
    '#required' => $component['required'],
    '#weight' => $component['weight'],
    '#field_prefix' => empty($component['extra']['field_prefix']) ? NULL : ($filter ? webform_filter_xss($component['extra']['field_prefix']) : $component['extra']['field_prefix']),
    '#field_suffix' => empty($component['extra']['field_suffix']) ? NULL : ($filter ? webform_filter_xss($component['extra']['field_suffix']) : $component['extra']['field_suffix']),
    '#description' => $filter ? webform_filter_descriptions($component['extra']['description'], $node) : $component['extra']['description'],
    '#attributes' => $component['extra']['attributes'],
    '#theme_wrappers' => array('webform_element'),
    '#translatable' => array('title', 'description', 'field_prefix', 'field_suffix'),
  );
  // Autocomplete callback
  if ($node && !empty($component['cid'])) {
    $element['#autocomplete_path'] = 'webform_autocomplete/js/' . $node->nid . '/' . $component['cid'];
  }
  
  if ($component['extra']['placeholder']) {
    $element['#attributes']['placeholder'] = $component['extra']['placeholder'];
  }

  // Enforce uniqueness.
  if ($component['extra']['unique']) {
    $element['#element_validate'][] = 'webform_validate_unique';
  }

  // Restrict to listed options.
  if ($component['extra']['autocomplete_restrict']) {
    $element['#element_validate'][] = 'webform_autocomplete_validate_restricted';
  }

  // Change the 'width' option to the correct 'size' option.
  if ($component['extra']['width'] > 0) {
    $element['#size'] = $component['extra']['width'];
  }

  if (isset($value)) {
    $element['#default_value'] = $value[0];
  }

  return $element;
}

/**
 * Implements _webform_display_component().
 */
function _webform_display_autocomplete($component, $value, $format = 'html') {
  return _webform_display_textfield($component, $value, $format);
}

/**
 * Implements _webform_analysis_component().
 */
function _webform_analysis_autocomplete($component, $sids = array()) {
  return _webform_analysis_textfield($component, $sids);
}

/**
 * Implements _webform_table_component().
 */
function _webform_table_autocomplete($component, $value) {
  return _webform_table_textfield($component, $value);
}

/**
 * Implements _webform_csv_headers_component().
 */
function _webform_csv_headers_autocomplete($component, $export_options) {
  return _webform_csv_headers_textfield($component, $export_options);
}

/**
 * Implements _webform_csv_data_component().
 */
function _webform_csv_data_autocomplete($component, $export_options, $value) {
  return _webform_csv_data_textfield($component, $export_options, $value);
}

/**
 * Autocomplete callback.
 */
function webform_autocomplete_js($node, $cid, $str = '') {
  if ($str !== '') {
    $component = $node->webform['components'][$cid];
    $result_count = (int) $component['extra']['autocomplete_result_count'];
    $match_rule = $component['extra']['autocomplete_match_rule'];
    $results = array();
    $count = 0;
    // Search from prepulated options
    if (!empty($component['extra']['autocomplete_items'])) {
      $options = preg_split('/\r\n|\r|\n/', $component['extra']['autocomplete_items']);
      foreach ($options as $val) {
        $is_match = FALSE;
        if ($match_rule == WEBFORM_AUTOCOMPLETE_MATCH_BEGIN) {
          if (strpos($val, $str, 0) === 0) {
            $is_match = TRUE;
          }
        }
        elseif ($match_rule == WEBFORM_AUTOCOMPLETE_MATCH_CONTAINS) {
          if (stripos($val, $str) !== FALSE) {
            $is_match = TRUE;
          }
        }
        else {
          if (strpos($val, $str) + strlen($str) == strlen(trim($val))) {
            $is_match = TRUE;
          }
        }
        
        if ($is_match) {
          $results[$val] = $val;
          // Limit to 20 results
          if (++$count >= $result_count) {
            break;
          }
        }
      }
    }
    // Search from existing submissions
    // Only fire the query if we have fewer than $result_count results already
    if (!empty($component['extra']['autocomplete_existing']) && $count < $result_count) {
      $str_like = db_like($str);
      if ($match_rule == WEBFORM_AUTOCOMPLETE_MATCH_CONTAINS || $match_rule == WEBFORM_AUTOCOMPLETE_MATCH_END) {
        $str_like = '%' . $str_like;
      }
      if ($match_rule == WEBFORM_AUTOCOMPLETE_MATCH_BEGIN || $match_rule == WEBFORM_AUTOCOMPLETE_MATCH_CONTAINS) {
        $str_like = $str_like . '%';
      }
      
      $db = db_query("SELECT DISTINCT data
        FROM {webform_submitted_data}
        WHERE nid = :nid AND cid = :cid AND data LIKE :str LIMIT " . ($result_count - $count),
        array(':nid' => $node->nid, ':cid' => $cid, ':str' => $str_like)
      );
      foreach ($db as $row) {
        $results[$row->data] = $row->data;
      }
    }
    // Sort php and sql results together
    natcasesort($results);
    drupal_alter('webform_autocomplete_options', $results, $node, $cid, $str);
    drupal_json_output($results);
  }
}

/**
 * Implements _webform_form_builder_map_component().
 */
function _webform_form_builder_map_autocomplete() {
  return array(
    'form_builder_type' => 'autocomplete',
    'properties' => array(
      'size' => array(
        'form_parents' => array('display', 'width'),
        'storage_parents' => array('extra', 'width'),
      ),
      'maxlength' => array(
        'form_parents' => array('display', 'maxlength'),
        'storage_parents' => array('extra', 'maxlength'),
      ),
      'field_prefix' => array(
        'form_parents' => array('display', 'field_prefix'),
        'storage_parents' => array('extra', 'field_prefix'),
      ),
      'field_suffix' => array(
        'form_parents' => array('display', 'field_suffix'),
        'storage_parents' => array('extra', 'field_suffix'),
      ),
      'autocomplete_items' => array(
        'form_parents' => array('options', 'autocomplete_items'),
        'storage_parents' => array('extra', 'autocomplete_items'),
      ),
      'autocomplete_existing' => array(
        'form_parents' => array('options', 'autocomplete_existing'),
        'storage_parents' => array('extra', 'autocomplete_existing'),
      ),
      'autocomplete_match_rule' => array(
        'form_parents' => array('options', 'autocomplete_match_rule'),
        'storage_parents' => array('extra', 'autocomplete_match_rule'),
      ),
      'autocomplete_restrict' => array(
        'form_parents' => array('validation', 'autocomplete_restrict'),
        'storage_parents' => array('extra', 'autocomplete_restrict'),
      ),
    ),
  );
}
