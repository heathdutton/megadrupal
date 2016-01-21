<?php
/**
 * @file
 * Webform module limited select component.
 */

/**
 * Implements _webform_defaults_component().
 */
function _webform_defaults_limited_select() {
  return array(
    'name' => '',
    'form_key' => NULL,
    'required' => 0,
    'pid' => 0,
    'weight' => 0,
    'value' => '',
    'extra' => array(
      'items' => '',
      'multiple' => NULL,
      'optrand' => 0,
      'title_display' => 0,
      'description' => '',
      'private' => FALSE,
      'analysis' => TRUE,
    ),
  );
}

/**
 * Implements _webform_theme_component().
 */
function _webform_theme_limited_select() {
  return array(
    'webform_display_limited_select' => array(
      'render element' => 'element',
      'file' => 'webform_limited_select.inc',
      'path' => drupal_get_path('module', 'webform_limited_select'),
    ),
  );
}

/**
 * Implements _webform_edit_component().
 */
function _webform_edit_limited_select($component) {
  $form = array(
    '#attached' => array(
      'js' => array(
        drupal_get_path('module', 'webform') . '/js/select-admin.js' => array('preprocess' => FALSE),
        array('data' => array('webform' => array('selectOptionsUrl' => url('webform/ajax/options/' . $component['nid']))), 'type' => 'setting'),
      ),
    ),
  );

  $form['extra']['items'] = array(
    '#type' => 'textarea',
    '#title' => t('Options'),
    '#default_value' => $component['extra']['items'],
    '#description' => t('<strong>Key-value-limit triplets MUST be specified as "safe_key|Some readable option|submission limit"</strong>. Use of only alphanumeric characters and underscores is recommended in keys. One option per line.') . ' ' . theme('webform_token_help'),
    '#cols' => 60,
    '#rows' => 5,
    '#weight' => 0,
    '#required' => TRUE,
    '#wysiwyg' => FALSE,
    '#element_validate' => array('_webform_edit_validate_limited_select'),
  );

  $form['value'] = array(
    '#type' => 'textfield',
    '#title' => t('Default value'),
    '#default_value' => $component['value'],
    '#description' => t('The default value of the field identified by its key. For multiple selects use commas to separate multiple defaults.') . ' ' . theme('webform_token_help'),
    '#size' => 60,
    '#maxlength' => 1024,
    '#weight' => 0,
  );

  $form['extra']['multiple'] = array(
    '#type' => 'checkbox',
    '#title' => t('Multiple'),
    '#default_value' => $component['extra']['multiple'],
    '#description' => t('Check this option if the user should be allowed to choose multiple values.'),
    '#weight' => 0,
  );

  $form['display']['optrand'] = array(
    '#type' => 'checkbox',
    '#title' => t('Randomize options'),
    '#default_value' => $component['extra']['optrand'],
    '#description' => t('Randomizes the order of the options when they are displayed in the form.'),
    '#parents' => array('extra', 'optrand'),
  );

  return $form;
}

/**
 * Element validation callback. Ensure keys are not duplicated.
 */
function _webform_edit_validate_limited_select($element, &$form_state) {
  // Check for duplicate key values to prevent unexpected data loss. Require
  // all options to include a safe_key.
  if (!empty($element['#value'])) {
    $lines = explode("\n", trim($element['#value']));
    $existing_keys = array();
    $duplicate_keys = array();
    $missing_keys = array();
    $long_keys = array();
    $bad_limits = array();
    foreach ($lines as $line) {
      $matches = array();
      $line = trim($line);
      if (preg_match('/^([^|]*)\|(.*)\|(.*)$/', $line, $matches)) {
        $key = $matches[1];
        $limit = $matches[3];
        if ($limit < 1) {
          $bad_limits[] = $key;
        }
      }
      elseif (preg_match('/^([^|]*)\|(.*)$/', $line, $matches)) {
        $key = $matches[1];
      }
      else {
        $missing_keys[] = $line;
      }

      if (isset($key)) {
        if (strlen($key) > 128) {
          $long_keys[] = $key;
        }
        if (isset($existing_keys[$key])) {
          $duplicate_keys[$key] = $key;
        }
        else {
          $existing_keys[$key] = $key;
        }
        unset($key);
      }
    }

    if (!empty($missing_keys)) {
      form_error($element, t('Every option must have a key specified. Specify each option as "safe_key|Some readable option".'));
    }

    if (!empty($long_keys)) {
      form_error($element, t('Option keys must be less than 128 characters. The following keys exceed this limit:') . theme('item_list', $long_keys));
    }

    if (!empty($duplicate_keys)) {
      form_error($element, t('Options within the select list must be unique. The following keys have been used multiple times:') . theme('item_list', array('items' => $duplicate_keys)));
    }

    if (!empty($bad_limits)) {
      form_error($element, t('Submission limits must be positive integers. The following keys have bad submission limits:') . theme('item_list', array('items' => $bad_limits)));
    }

  }

  return TRUE;
}

/**
 * Implements _webform_render_component().
 */
function _webform_render_limited_select($component, $value = NULL, $filter = TRUE) {
  $node = isset($component['nid']) ? node_load($component['nid']) : NULL;

  $element = array(
    '#title' => $filter ? webform_filter_xss($component['name']) : $component['name'],
    '#title_display' => $component['extra']['title_display'] ? $component['extra']['title_display'] : 'before',
    '#required' => $component['required'],
    '#weight' => $component['weight'],
    '#description' => $filter ? webform_filter_descriptions($component['extra']['description'], $node) : $component['extra']['description'],
    '#theme_wrappers' => array('webform_element'),
    '#pre_render' => array(), // Needed to disable double-wrapping of radios and checkboxes.
    '#translatable' => array('title', 'description', 'options'),
  );

  // Convert the user-entered options list into an array.
  $default_value = $filter ? webform_replace_tokens($component['value'], $node) : $component['value'];
  $options = _webform_limited_select_options($component, $filter);

  if ($component['extra']['optrand']) {
    _webform_limited_select_shuffle_options($options);
  }

  $options = _webform_limited_select_process_limits($component, $options);
  foreach ($options['disabled'] as $key) {
    $element[$key] = array('#disabled' => TRUE);
  }
  unset($options['disabled']);

  // Set the component options.
  $element['#options'] = $options;

  // Set the default value.
  if (isset($value)) {
    if ($component['extra']['multiple']) {
      // Set the value as an array.
      $element['#default_value'] = array();
      foreach ((array) $value as $key => $option_value) {
        $element['#default_value'][] = $option_value;
      }
    }
    else {
      // Set the value as a single string.
      $element['#default_value'] = '';
      foreach ((array) $value as $option_value) {
        $element['#default_value'] = $option_value;
      }
    }
  }
  elseif ($default_value !== '') {
    // Convert default value to a list if necessary.
    if ($component['extra']['multiple']) {
      $varray = explode(',', $default_value);
      foreach ($varray as $key => $v) {
        $v = trim($v);
        if ($v !== '') {
          $element['#default_value'][] = $v;
        }
      }
    }
    else {
      $element['#default_value'] = $default_value;
    }
  }
  elseif ($component['extra']['multiple']) {
    $element['#default_value'] = array();
  }

  if ($component['extra']['multiple']) {
    // Set display as a checkbox set.
    $element['#type'] = 'checkboxes';
    $element['#theme_wrappers'] = array_merge(array('checkboxes'), $element['#theme_wrappers']);
    $element['#process'] = array_merge(element_info_property('checkboxes', '#process'), array('webform_expand_limited_select_ids'));
  }
  else {
    // Set display as a radio set.
    $element['#type'] = 'radios';
    $element['#theme_wrappers'] = array_merge(array('radios'), $element['#theme_wrappers']);
    $element['#process'] = array_merge(element_info_property('radios', '#process'), array('webform_expand_limited_select_ids'));
  }

  return $element;
}

/**
 * FAPI process function to rename IDs attached to checkboxes and radios.
 */
function webform_expand_limited_select_ids($element) {
  $id = $element['#id'] = str_replace('_', '-', _webform_safe_name(strip_tags($element['#id'])));
  $delta = 0;
  foreach (element_children($element) as $key) {
    $delta++;
    // Convert the #id for each child to a safe name, regardless of key.
    $element[$key]['#id'] = $id . '-' . $delta;

    // Prevent scripts or CSS in the labels for each checkbox or radio.
    $element[$key]['#title'] = webform_filter_xss($element[$key]['#title']);
  }
  return $element;
}

/**
 * Implements _webform_display_component().
 */
function _webform_display_limited_select($component, $value, $format = 'html') {
  return array(
    '#title' => $component['name'],
    '#weight' => $component['weight'],
    '#multiple' => $component['extra']['multiple'],
    '#theme' => 'webform_display_limited_select',
    '#theme_wrappers' => $format == 'html' ? array('webform_element') : array('webform_element_text'),
    '#format' => $format,
    '#options' => _webform_limited_select_options($component),
    '#value' => (array) $value,
    '#translatable' => array('title', 'options'),
  );
}

/**
 * Implements _webform_submit_component().
 *
 * convert FAPI 0/1 values into
 * something saveable.
 */
function _webform_submit_limited_select($component, $value) {

  // Build a list of all valid keys expected to be submitted.
  $options = _webform_limited_select_options($component, TRUE);

  $return = NULL;
  if (is_array($value)) {
    $return = array();
    foreach ($value as $key => $option_value) {
      // Handle options that are specified options.
      if ($option_value !== '' && isset($options[$option_value])) {
        // Checkboxes submit an integer value of 0 when unchecked. A checkbox
        // with a value of '0' is valid, so we can't use empty() here.
        if ($option_value === 0 && $component['extra']['multiple']) {
          unset($value[$option_value]);
        }
        else {
          $return[] = $option_value;
        }
      }
    }
  }
  elseif (is_string($value)) {
    $return = $value;
  }

  return $return;
}

/**
 * Format the text output for this component.
 */
function theme_webform_display_limited_select($variables) {
  $element = $variables['element'];
  $options = $element['#options'];
  $options = _webform_limited_select_process_limits($element, $options);

  $items = array();
  if ($element['#multiple']) {
    foreach ((array) $element['#value'] as $option_value) {
      if ($option_value !== '') {
        // Administer provided values.
        if (isset($options[$option_value])) {
          $items[] = $element['#format'] == 'html' ? webform_filter_xss($options[$option_value]) : $options[$option_value];
        }
        // User-specified in the "other" field.
        else {
          $items[] = $element['#format'] == 'html' ? check_plain($option_value) : $option_value;
        }
      }
    }
  }
  else {
    if (isset($element['#value'][0]) && $element['#value'][0] !== '') {
      // Administer provided values.
      if (isset($options[$element['#value'][0]])) {
        $items[] = $element['#format'] == 'html' ? webform_filter_xss($options[$element['#value'][0]]) : $options[$element['#value'][0]];
      }
      // User-specified in the "other" field.
      else {
        $items[] = $element['#format'] == 'html' ? check_plain($element['#value'][0]) : $element['#value'][0];
      }
    }
  }

  if ($element['#format'] == 'html') {
    $output = count($items) > 1 ? theme('item_list', array('items' => $items)) : (isset($items[0]) ? $items[0] : ' ');
  }
  else {
    if (count($items) > 1) {
      foreach ($items as $key => $item) {
        $items[$key] = ' - ' . $item;
      }
      $output = implode("\n", $items);
    }
    else {
      $output = isset($items[0]) ? $items[0] : ' ';
    }
  }

  return $output;
}

/**
 * Implements _webform_analysis_component().
 */
function _webform_analysis_limited_select($component, $sids = array(), $single = FALSE) {
  $options = _webform_limited_select_options($component, TRUE);
  $options = _webform_limited_select_procesS_limits($component, $options);
  $show_other_results = $single;

  // Gather the normal results first (not "other" options).
  $query = db_select('webform_submitted_data', 'wsd', array('fetch' => PDO::FETCH_ASSOC))
    ->fields('wsd', array('data'))
    ->condition('nid', $component['nid'])
    ->condition('cid', $component['cid'])
    ->condition('data', '', '<>')
    ->groupBy('data');
  $query->addExpression('COUNT(data)', 'datacount');

  if (count($sids)) {
    $query->condition('sid', $sids, 'IN');
  }

  $query->condition('data', array_keys($options), 'IN');

  $result = $query->execute();
  $rows = array();
  $normal_count = 0;
  foreach ($result as $data) {
    $display_option = isset($options[$data['data']]) ? $options[$data['data']] : $data['data'];
    $rows[$data['data']] = array(webform_filter_xss($display_option), $data['datacount']);
    $normal_count += $data['datacount'];
  }

  // Order the results according to the normal options array.
  $ordered_rows = array();
  foreach (array_intersect_key($options, $rows) as $key => $label) {
    $ordered_rows[] = $rows[$key];
  }
  $rows = $ordered_rows;

  return array(
    'table_rows' => $rows,
    'other_data' => $other,
  );
}

/**
 * Implements _webform_table_component().
 */
function _webform_table_limited_select($component, $value) {
  // Convert submitted 'safe' values to un-edited, original form.
  $options = _webform_limited_select_options($component, TRUE);
  $options = _webform_limited_select_procesS_limits($component, $options);

  $value = (array) $value;
  $items = array();
  // Set the value as a single string.
  foreach ($value as $option_value) {
    if ($option_value !== '') {
      if (isset($options[$option_value])) {
        $items[] = webform_filter_xss($options[$option_value]);
      }
      else {
        $items[] = check_plain($option_value);
      }
    }
  }

  return implode('<br />', $items);
}

/**
 * Implements _webform_csv_headers_component().
 */
function _webform_csv_headers_limited_select($component, $export_options) {
  $headers = array(
    0 => array(),
    1 => array(),
    2 => array(),
  );

  if ($component['extra']['multiple'] && $export_options['select_format'] == 'separate') {
    $headers[0][] = '';
    $headers[1][] = $export_options['header_keys'] ? $component['form_key'] : $component['name'];
    $items = _webform_limited_select_options($component, TRUE, FALSE);
    $count = 0;
    foreach ($items as $key => $item) {
      // Empty column per sub-field in main header.
      if ($count != 0) {
        $headers[0][] = '';
        $headers[1][] = '';
      }
      if ($export_options['select_keys']) {
        $headers[2][] = $key;
      }
      else {
        $headers[2][] = $item[0];
      }
      $count++;
    }
  }
  else {
    $headers[0][] = '';
    $headers[1][] = '';
    $headers[2][] = $export_options['header_keys'] ? $component['form_key'] : $component['name'];
  }
  return $headers;
}

/**
 * Implements _webform_csv_data_component().
 */
function _webform_csv_data_limited_select($component, $export_options, $value) {
  $options = _webform_limited_select_options($component, TRUE, FALSE);
  $return = array();

  if ($component['extra']['multiple']) {
    foreach ($options as $key => $item) {
      $index = array_search($key, (array) $value);
      if ($index !== FALSE) {
        if ($export_options['select_format'] == 'separate') {
          $return[] = 'X';
        }
        else {
          $return[] = $export_options['select_keys'] ? $key : $item;
        }
        unset($value[$index]);
      }
      elseif ($export_options['select_format'] == 'separate') {
        $return[] = '';
      }
    }
  }
  else {
    $key = $value[0];
    if ($export_options['select_keys']) {
      $return = $key;
    }
    else {
      $return = isset($options[$key]) ? $options[$key] : $key;
    }
  }

  if ($component['extra']['multiple'] && $export_options['select_format'] == 'compact') {
    $return = implode(',', (array) $return);
  }

  return $return;
}


/**
 * Implements _webform_options_component().
 *
 * This function is confusingly an alias of _webform_limited_select_options(). However
 * this version is intended to be accessed publicly via
 * webform_component_invoke(), since it is a Webform "hook", rather than an
 * internal, private function. To get the list of select list options for
 * a component, use:
 *
 * @code
 * $options = webform_component_invoke($component['type'], 'options', $component);
 * @endcode
 */
function _webform_options_limited_select($component, $filter = TRUE) {
  return _webform_limited_select_options($component, $filter);
}

/**
 * Generate a list of options for a select list.
 */
function _webform_limited_select_options($component, $filter = TRUE) {
  $options = _webform_limited_select_options_from_text($component['extra']['items'], $filter);
  return isset($options) ? $options : array();
}

/**
 * Utility function to split user-entered values from new-line separated
 * text into an array of options.
 *
 * @param $text
 *   Text to be converted into a select option array.
 * @param $filter
 *   Optional. Whether or not to filter returned values.
 */
function _webform_limited_select_options_from_text($text, $filter = TRUE) {
  static $option_cache = array();

  // Keep each processed option block in an array indexed by the MD5 hash of
  // the option text
  $md5 = md5($text);

  // Check if this option block has been previously processed.
  if (!isset($option_cache[$md5])) {
    $options = array();
    $rows = array_filter(explode("\n", trim($text)));
    foreach ($rows as $option) {
      $option = trim($option);
      if (preg_match('/^([^|]+)\|(.*)\|(.*)$/', $option, $matches)) {
        $key = $filter ? webform_replace_tokens($matches[1]) : $matches[1];
        $value = $filter ? webform_replace_tokens($matches[2]) : $matches[2];
        $limit = $matches[3];
        $options[$key] = array($value, $limit);
      }
      elseif (preg_match('/^([^|]+)\|(.*)$/', $option, $matches)) {
        $key = $filter ? webform_replace_tokens($matches[1]) : $matches[1];
        $value = $filter ? webform_replace_tokens($matches[2]) : $matches[2];
        $options[$key] = array($value, 0);
      }
    }

    $option_cache[$md5] = $options;
  }

  // Return our options from the option_cache array.
  return $option_cache[$md5];
}

/**
 * Convert an array of options into text.
 */
function _webform_limited_select_options_to_text($options) {
  $output = '';

  foreach ($options as $key => $value) {
    if ($options[$key] !== '') {
      $output .= $key . '|' . $value . "\n";
    }
  }

  return $output;
}

/**
 * Utility function to shuffle an array while preserving key-value pairs.
 */
function _webform_limited_select_shuffle_options(&$array) {
  // First shuffle the array keys, then use them as the basis for ordering
  // the options.
  $aux = array();
  $keys = array_keys($array);
  shuffle($keys);
  foreach ($keys as $key) {
    $aux[$key] = $array[$key];
  }
  $array = $aux;
}

/**
 * Process submission limits
 */
function _webform_limited_select_process_limits($component, $options) {
  $return = array();
  $return['disabled'] = array();
  foreach ($options as $key => $option) {
    list($value, $limit) = $option;
    if ($limit > 0) {
      $count = _webform_limited_select_count_submissions($component, $key);
      if ($count >= $limit) {
        $return['disabled'][] = $key;
      }
    }
    $return[$key] = $value;
  }
  return $return;
}

/**
 * Count the submissions for a given key
 */
function _webform_limited_select_count_submissions($component, $key) {
  $q = db_query('
    SELECT count(*)
    FROM   webform_submitted_data
    WHERE  nid = :n AND cid = :c AND data = :d',
    array(':n' => $component['nid'], ':c' => $component['cid'], ':d' => $key)
  );
  $count = $q->fetchCol();
  return $count[0];
}
