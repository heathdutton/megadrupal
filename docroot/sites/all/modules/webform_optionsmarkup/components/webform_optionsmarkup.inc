<?php
/**
 * @file
 * Webform component that allows markup in checkbox and radio options.
 */


// Optionsmarkup depends on functions provided by select.
webform_component_include('select');

/**
 * Implements _webform_defaults_component().
 */
function _webform_defaults_optionsmarkup() {
  return array(
    'name' => '',
    'form_key' => NULL,
    'mandatory' => 0,
    'pid' => 0,
    'weight' => 0,
    'value' => '',
    'html' => TRUE,
    'extra' => array(
      'items' => '',
      'multiple' => NULL,
      'title_display' => 0,
      'description' => '',
      'private' => FALSE,
      'aslist' => NULL,
      'optrand' => FALSE,
    ),
  );
}

/**
 * Implements _webform_theme_component().
 */
function _webform_theme_optionsmarkup() {
  return array(
    'webform_display_optionsmarkup' => array(
      'render element' => 'element',
    ),
  );
}

/**
 * Implements _webform_edit_component().
 */
function _webform_edit_optionsmarkup($component) {
  $format_id = variable_get('optionsmarkup_input_format', 'filtered_html');
  if (empty($format_id)) {
    $format_id = filter_fallback_format();
  }
  $format = filter_format_load($format_id);
  if (!filter_access($format)) {
    drupal_set_message(t('You do not have permission to edit this component.'), 'error');
    $nid = arg(1);
    drupal_goto("node/$nid");
    return array();
  }

  $form = array();
  $select_admin_js = drupal_get_path('module', 'webform') . '/js/select-admin.js';
  $form['#attached']['js'][$select_admin_js] = array('preprocess' => FALSE);
  $form['#attached']['js'][] = array(
    'data' => array(
      'webform' => array('selectOptionsUrl' => url('webform/ajax/options/' . $component['nid'])),
    ),
    'type' => 'setting',
  );

  $form['extra']['items'] = array(
    '#type' => 'textarea',
    '#title' => t('Options'),
    '#default_value' => $component['extra']['items'],
    '#description' => t('<strong>Key-value pairs MUST be specified as "safe_key|Some readable title|Some readable description (can contain html markup)"</strong>. Use of only alphanumeric characters and underscores is recommended in keys. One option per line. Option groups may be specified with &lt;Group Name&gt;. &lt;&gt; can be used to insert items at the root of the menu after specifying a group.') . theme('webform_token_help'),
    '#cols' => 60,
    '#rows' => 5,
    '#weight' => 0,
    '#required' => TRUE,
    '#wysiwyg' => FALSE,
    '#element_validate' => array('_webform_edit_validate_optionsmarkup'),
  );

  $form['value'] = array(
    '#type' => 'textfield',
    '#title' => t('Default value'),
    '#default_value' => $component['value'],
    '#description' => t('The default value of the field identified by its key. For multiple selects use commas to separate multiple defaults.') . theme('webform_token_help'),
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
function _webform_edit_validate_optionsmarkup($element, &$form_state) {
  // Check for duplicate key values to prevent unexpected data loss. Require
  // all options to include a safe_key.
  if (!empty($element['#value'])) {
    $lines = explode("\n", trim($element['#value']));
    $existing_keys = array();
    $duplicate_keys = array();
    $missing_keys = array();
    $long_keys = array();
    $group = '';
    foreach ($lines as $line) {
      $matches = array();
      $line = trim($line);
      if (preg_match('/^\<([^>]*)\>$/', $line, $matches)) {
        $group = $matches[1];
        // No need to store group names.
        $key = NULL;
      }
      elseif (preg_match('/^([^|]*)\|(.*)\|(.*)$/', $line, $matches)) {
        $key = $matches[1];
        if (strlen($key) > 128) {
          $long_keys[] = $key;
        }
      }
      elseif (preg_match('/^([^|]*)\|(.*)$/', $line, $matches)) {
        $key = $matches[1];
        if (strlen($key) > 128) {
          $long_keys[] = $key;
        }
      }
      else {
        $missing_keys[] = $line;
      }

      if (isset($key)) {
        if (isset($existing_keys[$group][$key])) {
          $duplicate_keys[$key] = $key;
        }
        else {
          $existing_keys[$group][$key] = $key;
        }
      }
    }

    if (!empty($missing_keys)) {
      form_error($element, t('Every option must have a key specified. Specify each option as "safe_key|Some readable title|Some readable description (can contain html markup)".'));
    }

    if (!empty($long_keys)) {
      form_error($element, t('Option keys must be less than 128 characters. The following keys exceed this limit:') . theme('item_list', $long_keys));
    }

    if (!empty($duplicate_keys)) {
      form_error($element, t('Options within the select list must be unique. The following keys have been used multiple times:') . theme('item_list', array('items' => $duplicate_keys)));
    }

  }

  return TRUE;
}

/**
 * Implements _webform_render_component().
 */
function _webform_render_optionsmarkup($component, $value = NULL, $filter = TRUE) {
  $node = isset($component['nid']) ? node_load($component['nid']) : NULL;

  $element = array(
    '#title' => $filter ? webform_optionsmarkup_filter_content($component['name']) : $component['name'],
    '#title_display' => $component['extra']['title_display'] ? $component['extra']['title_display'] : 'before',
    '#required' => $component['mandatory'],
    '#weight' => $component['weight'],
    '#description' => $filter ? webform_optionsmarkup_filter_content($component['extra']['description'], $node) : $component['extra']['description'],
    '#theme_wrappers' => array('webform_element'),
    // Needed to disable double-wrapping of radios and checkboxes.
    '#pre_render' => array(),
    '#translatable' => array('title', 'description', 'options'),
  );

  // Convert the user-entered options list into an array.
  $default_value = $filter ? _webform_filter_values($component['value'], $node, NULL, NULL, FALSE) : $component['value'];
  $options = _webform_optionsmarkup_options($component, !$component['extra']['aslist'], $filter);

  if (isset($component['extra']['optrand']) && $component['extra']['optrand']) {
    _webform_shuffle_options($options);
  }

  // Set the component options.
  $element['#options'] = $options;

  // Set the default value.
  if (isset($value)) {
    if ($component['extra']['multiple']) {
      // Set the value as an array.
      $element['#default_value'] = array();
      foreach ((array) $value as $option_value) {
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
      foreach ($varray as $v) {
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
    $element['#process'] = array_merge(element_info_property('checkboxes', '#process'), array('webform_optionsmarkup_expand_select_ids'));

    // Entirely replace the normal expand checkboxes with our custom version.
    // This helps render checkboxes in multipage forms.
    $process_key = array_search('form_process_checkboxes', $element['#process']);
    $element['#process'][$process_key] = 'webform_optionsmarkup_expand_checkboxes';
  }
  else {
    // Set display as a radio set.
    $element['#type'] = 'radios';
    $element['#theme_wrappers'] = array_merge(array('radios'), $element['#theme_wrappers']);
    $element['#process'] = array_merge(element_info_property('radios', '#process'), array('webform_optionsmarkup_expand_select_ids'));

    // Entirely replace the normal expand radios with our custom version.
    $process_key = array_search('form_process_radios', $element['#process']);
    $element['#process'][$process_key] = 'webform_optionsmarkup_process_optionsmarkup_radios';
  }

  return $element;
}

/**
 * Drupal 6 hack that properly *renders* checkboxes in multistep forms.
 *
 * This is different than the value hack needed in Drupal 5,
 * which is no longer needed.
 */
function webform_optionsmarkup_expand_checkboxes($element) {
  // Elements that have a value set are already in the form structure cause
  // them not to be written when the expand_checkboxes function is called.
  $default_value = array();
  foreach (element_children($element) as $key) {
    if (isset($element[$key]['#default_value'])) {
      $default_value[$key] = $element[$key]['#default_value'];
      unset($element[$key]);
    }
  }

  $element = webform_optionsmarkup_process_optionsmarkup_checkboxes($element);

  // Escape the values of checkboxes.
  foreach (element_children($element) as $key) {
    $element[$key]['#return_value'] = check_plain($element[$key]['#return_value']);
    $element[$key]['#name'] = $element['#name'] . '[' . $element[$key]['#return_value'] . ']';
  }

  foreach ($default_value as $key => $val) {
    $element[$key]['#default_value'] = $val;
  }
  return $element;
}

/**
 * Implements _webform_display_component().
 */
function _webform_display_optionsmarkup($component, $value, $format = 'html') {
  return array(
    '#title' => $component['name'],
    '#weight' => $component['weight'],
    '#multiple' => $component['extra']['multiple'],
    '#theme' => 'webform_display_optionsmarkup',
    '#theme_wrappers' => $format == 'html' ? array('webform_element') : array('webform_element_text'),
    '#format' => $format,
    '#options' => _webform_optionsmarkup_options($component, !$component['extra']['aslist']),
    '#value' => (array) $value,
    '#translatable' => array('title', 'options'),
  );
}

/**
 * Implements _webform_submit_component().
 *
 * Convert FAPI 0/1 values into something saveable.
 */
function _webform_submit_optionsmarkup($component, $value) {
  // Build a list of all valid keys expected to be submitted.
  $options = _webform_optionsmarkup_options($component, TRUE);

  $return = NULL;
  if (is_array($value)) {
    $return = array();
    foreach ($value as $option_value) {
      // Handle options that are specified options.
      if ($option_value !== '' && isset($options[$option_value])) {
        // Checkboxes submit an integer value of 0 when unchecked. A checkbox
        // with a value of '0' is valid, so we can't use empty() here.
        if ($option_value === 0 && !$component['extra']['aslist'] && $component['extra']['multiple']) {
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
function theme_webform_display_optionsmarkup($variables) {
  $element = $variables['element'];

  // Flatten the list of options so we can get values easily. These options
  // may be translated by hook_webform_display_component_alter().
  $options = array();
  foreach ($element['#options'] as $key => $value) {
    if (is_array($value) AND !$value['title']) {
      foreach ($value as $subkey => $subvalue) {
        $options[$subkey] = $subvalue['title'];
      }
    }
    else {
      $options[$key] = $value['title'];
    }
  }

  $items = array();
  if ($element['#multiple']) {
    foreach ((array) $element['#value'] as $option_value) {
      if ($option_value !== '') {
        // Administer provided values.
        if (isset($options[$option_value])) {
          $items[] = $element['#format'] == 'html' ? webform_optionsmarkup_filter_content($options[$option_value]) : $options[$option_value];
        }
      }
    }
  }
  else {
    if (isset($element['#value'][0]) && $element['#value'][0] !== '') {
      // Administer provided values.
      if (isset($options[$element['#value'][0]])) {
        $items[] = $element['#format'] == 'html' ? webform_optionsmarkup_filter_content($options[$element['#value'][0]]) : $options[$element['#value'][0]];
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
function _webform_analysis_optionsmarkup($component, $sids = array(), $single = FALSE) {
  $options = _webform_optionsmarkup_options($component, TRUE);
  $show_other_results = $single;

  $option_operator = $show_other_results ? 'NOT IN' : 'IN';
  $query = db_select('webform_submitted_data', 'wsd', array('fetch' => PDO::FETCH_ASSOC))
    ->fields('wsd', array('data'))
    ->condition('nid', $component['nid'])
    ->condition('cid', $component['cid'])
    ->condition('data', '', '<>')
    ->condition('data', array_keys($options), $option_operator)
    ->groupBy('data');
  $query->addExpression('COUNT(data)', 'datacount');

  if (count($sids)) {
    $query->condition('sid', $sids, 'IN');
  }

  $count_query = db_select('webform_submitted_data', 'wsd', array('fetch' => PDO::FETCH_ASSOC))
    ->condition('nid', $component['nid'])
    ->condition('cid', $component['cid'])
    ->condition('data', '', '<>');
  $count_query->addExpression('COUNT(*)', 'datacount');
  if (count($sids)) {
    $count_query->condition('sid', $sids, 'IN');
  }

  $result = $query->execute();
  $rows = array();
  $normal_count = 0;
  foreach ($result as $data) {
    $display_option = $single ? $data['data'] : $options[$data['data']]['title'];
    $rows[$data['data']] = array(_webform_filter_xss($display_option), $data['datacount']);
    $normal_count += $data['datacount'];
  }

  if (!$show_other_results) {
    // Order the results according to the normal options array.
    $ordered_rows = array();
    foreach (array_intersect_key($options, $rows) as $key => $label) {
      $ordered_rows[] = $rows[$key];
    }
    $rows = $ordered_rows;
  }

  return $rows;
}

/**
 * Implements _webform_table_component().
 */
function _webform_table_optionsmarkup($component, $value) {
  // Convert submitted 'safe' values to un-edited, original form.
  $options = _webform_optionsmarkup_options($component, TRUE);

  $value = (array) $value;
  $items = array();
  // Set the value as a single string.
  foreach ($value as $option_value) {
    if ($option_value !== '') {
      if (isset($options[$option_value]['title'])) {
        $items[] = _webform_filter_xss($options[$option_value]['title']);
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
function _webform_csv_headers_optionsmarkup($component, $export_options) {
  $headers = array(
    0 => array(),
    1 => array(),
    2 => array(),
  );

  if ($component['extra']['multiple'] && $export_options['select_format'] == 'separate') {
    $headers[0][] = '';
    $headers[1][] = $component['name'];
    $items = _webform_optionsmarkup_options($component, TRUE, FALSE);
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
        $headers[2][] = $item['title'];
      }
      $count++;
    }
  }
  else {
    $headers[0][] = '';
    $headers[1][] = '';
    $headers[2][] = $component['name'];
  }
  return $headers;
}

/**
 * Implements _webform_csv_data_component().
 */
function _webform_csv_data_optionsmarkup($component, $export_options, $value) {
  $options = _webform_optionsmarkup_options($component, TRUE, FALSE);
  $return = array();

  if ($component['extra']['multiple']) {
    foreach ($options as $key => $item) {
      $index = array_search($key, (array) $value);
      if ($index !== FALSE) {
        if ($export_options['select_format'] == 'separate') {
          $return[] = 'X';
        }
        else {
          $return[] = $export_options['select_keys'] ? $key : $item['title'];
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
      $return = isset($options[$key]['title']) ? $options[$key]['title'] : $key;
    }
  }

  if ($component['extra']['multiple'] && $export_options['select_format'] == 'compact') {
    $return = implode(',', (array) $return);
  }

  return $return;
}

/**
 * Generate a list of options for a select list.
 */
function _webform_optionsmarkup_options($component, $flat = FALSE, $filter = TRUE) {
  $options = _webform_optionsmarkup_options_from_text($component['extra']['items'], $flat, $filter);

  return isset($options) ? $options : array();
}

/**
 * Split values from new-line separated text into an array of options.
 *
 * @param string $text
 *   Text to be converted into a select option array.
 * @param bool $flat
 *   Optional. If specified, return the option array and exclude any optgroups.
 * @param bool $filter
 *   Optional. Whether or not to filter returned values.
 */
function _webform_optionsmarkup_options_from_text($text, $flat = FALSE, $filter = TRUE) {
  static $option_cache = array();

  // Keep each processed option block in an array indexed by the MD5 hash of
  // the option text and the value of the $flat variable.
  $md5 = md5($text);

  // Check if this option block has been previously processed.
  if (!isset($option_cache[$flat][$md5])) {
    $options = array();
    $rows = array_filter(explode("\n", trim($text)));
    $group = NULL;
    foreach ($rows as $option) {
      $option = trim($option);
      /*
       * If the Key of the option is within < >, treat as an optgroup
       *
       * <Group 1>
       *   creates an optgroup with the label "Group 1"
       *
       * <>
       *   Unsets the current group, allowing items to be inserted at
       *   the root element.
       */
      if (preg_match('/^\<([^>]*)\>$/', $option, $matches)) {
        if (empty($matches[1])) {
          unset($group);
        }
        elseif (!$flat) {
          $group = $filter ? _webform_filter_values($matches[1], NULL, NULL, NULL, FALSE) : $matches[1];
        }
      }
      elseif (preg_match('/^([^|]+)\|(.*)\|(.*)$/', $option, $matches)) {
        $key = $filter ? _webform_filter_values($matches[1], NULL, NULL, NULL, FALSE) : $matches[1];
        $title = $filter ? _webform_filter_values($matches[2], NULL, NULL, NULL, FALSE) : $matches[2];
        $desc = $filter ? _webform_filter_values($matches[3], NULL, NULL, NULL, FALSE) : $matches[3];
        if (isset($group)) {
          $options[$group][$key] = array('title' => $title, 'desc' => $desc);
        }
        else {
          $options[$key] = array('title' => $title, 'desc' => $desc);
        }
      }
      elseif (preg_match('/^([^|]+)\|(.*)$/', $option, $matches)) {
        $key = $filter ? _webform_filter_values($matches[1], NULL, NULL, NULL, FALSE) : $matches[1];
        $value = $filter ? _webform_filter_values($matches[2], NULL, NULL, NULL, FALSE) : $matches[2];
        if (isset($group)) {
          $options[$group][$key] = array('title' => $value, 'desc' => '');
        }
        else {
          $options[$key] = array('title' => $value, 'desc' => '');
        }
      }
      else {
        $filtered_option = $filter ? _webform_filter_values($option, NULL, NULL, NULL, FALSE) : $option;
        isset($group) ? $options[$group][$filtered_option] = array('title' => $filtered_option) : $options[$filtered_option] = array('title' => $filtered_option);
      }
    }

    $option_cache[$flat][$md5] = $options;
  }

  // Return our options from the option_cache array.
  return $option_cache[$flat][$md5];
}

/**
 * Convert an array of options into text.
 */
function _webform_optionsmarkup_options_to_text($options) {
  $output = '';
  $previous_key = FALSE;

  foreach ($options as $key => $value) {
    // Groups and Options with markup.
    if (is_array($value)) {
      // Groups.
      if (!$value['title']) {
        $output .= '<' . $key . '>' . "\n";
        foreach ($value as $subkey => $subvalue) {
          $output .= $subkey . '|' . $subvalue['title'] . '|' . $subvalue['desc'] . "\n";
        }
      }
      // Options with markup.
      else {
        // Exit out of any groups.
        if (isset($options[$previous_key]) && is_array($options[$previous_key])) {
          $output .= "<>\n";
        }
        // Skip empty rows.
        if ($options[$key] !== '') {
          $output .= $key . '|' . $value['title'] . '|' . $value['desc'] . "\n";
        }
      }
    }
    // Typical key|value pairs.
    else {
      // Exit out of any groups.
      if (isset($options[$previous_key]) && is_array($options[$previous_key])) {
        $output .= "<>\n";
      }
      // Skip empty rows.
      if ($options[$key] !== '') {
        $output .= $key . '|' . $value . "\n";
      }
    }
    $previous_key = $key;
  }

  return $output;
}

/**
 * Processes a checkboxes form element.
 *
 * Split title and description for the checkbox options.
 */
function webform_optionsmarkup_process_optionsmarkup_checkboxes($element) {
  $value = is_array($element['#value']) ? $element['#value'] : array();
  $element['#tree'] = TRUE;
  if (count($element['#options']) > 0) {
    if (!isset($element['#default_value']) || $element['#default_value'] == 0) {
      $element['#default_value'] = array();
    }
    $weight = 0;
    foreach ($element['#options'] as $key => $choice) {
      // Integer 0 is not a valid #return_value, so use '0' instead.
      // @see form_type_checkbox_value().
      // @todo For Drupal 8, cast all integer keys to strings for consistency
      //   with form_process_radios().
      if ($key === 0) {
        $key = '0';
      }
      // Maintain order of options as defined in #options, in case the element
      // defines custom option sub-elements, but does not define all option
      // sub-elements.
      $weight += 0.001;

      $element += array($key => array());
      $element[$key] += array(
        '#type' => 'checkbox',
        '#title' => webform_optionsmarkup_filter_content($choice['title']),
        '#return_value' => $key,
        '#default_value' => isset($value[$key]) ? $key : NULL,
        '#attributes' => $element['#attributes'],
        '#ajax' => isset($element['#ajax']) ? $element['#ajax'] : NULL,
        '#weight' => $weight,
        '#theme_wrappers' => array('webform_element'),
        '#description' => webform_optionsmarkup_filter_content($choice['desc']),
        '#format' => 'html',
      );
    }
  }
  return $element;
}


/**
 * Expands a radios element into individual radio elements.
 *
 * Split title and description for the radio options.
 */
function webform_optionsmarkup_process_optionsmarkup_radios($element) {
  if (isset($element['#options']) && count($element['#options']) > 0) {
    $weight = 0;
    foreach ($element['#options'] as $key => $choice) {
      // Maintain order of options as defined in #options, in case the element
      // defines custom option sub-elements, but does not define all option
      // sub-elements.
      $weight += 0.001;

      $element += array($key => array());
      // Generate the parents as the autogenerator does, so we will have a
      // unique id for each radio button.
      $parents_for_id = array_merge($element['#parents'], array($key));
      $element[$key] += array(
        '#type' => 'radio',
        '#title' => webform_optionsmarkup_filter_content($choice['title']),
        // The key is sanitized in drupal_attributes() during output from the
        // theme function.
        '#return_value' => $key,
        // Use default or FALSE. A value of FALSE means that the radio button is
        // not 'checked'.
        '#default_value' => isset($element['#default_value']) ? $element['#default_value'] : FALSE,
        '#attributes' => $element['#attributes'],
        '#parents' => $element['#parents'],
        '#id' => drupal_html_id('edit-' . implode('-', $parents_for_id)),
        '#ajax' => isset($element['#ajax']) ? $element['#ajax'] : NULL,
        '#weight' => $weight,
        '#theme_wrappers' => array('webform_element'),
        '#description' => webform_optionsmarkup_filter_content($choice['desc']),
      );
    }
  }
  return $element;
}
