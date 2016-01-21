<?php

/**
 * @file
 * Salesforce Webform module Salesforce Picklist component.
 */

/**
 * Implements _webform_defaults_component().
 */
function _webform_defaults_sf_picklist() {
  return array(
    'name' => '',
    'form_key' => NULL,
    'mandatory' => 0,
    'required' => 0,
    'pid' => 0,
    'weight' => 0,
    'value' => '',
    'extra' => array(
      'list' => '',
      'exclude' => '',
      'optsort' => 0,
      'title_display' => 0,
      'description' => '',
      'custom_keys' => FALSE,
      'options_source' => '',
      'private' => FALSE,
      'size' => '',
    ),
  );
}

/**
 * Implements _webform_theme_component().
 */
function _webform_theme_sf_picklist() {
  return array(
    'webform_display_sf_picklist' => array(
      'render element' => 'element',
      'file' => 'salesforce_webforms.picklist.inc',
      'path' => drupal_get_path('module', 'salesforce_webforms'),
    ),
  );
}

/**
 * Implements _webform_edit_component().
 */
function _webform_edit_sf_picklist($component) {
  $picklists = array();
  $form = array();

  $form['name'] = array(
    '#type' => 'textfield',
    '#default_value' => $component['name'],
    '#title' => t('Label'),
    '#weight' => -10,
  );

  $picklists = salesforce_webforms_get_picklist_options();

  $form['extra']['list'] = array(
    '#type' => 'select',
    '#title' => t('Choose a picklist'),
    '#options' => $picklists,
    '#default_value' => $component['extra']['list'],
    '#size' => 10,
    '#required' => TRUE,
  );

  $form['extra']['exclude'] = array(
    '#type' => 'textarea',
    '#title' => t('Options to exclude'),
    '#default_value' => $component['extra']['exclude'],
    '#description' => t('List options which should be removed from the list provided by Salesforce, one per line. This does not impact the data stored in Salesforce. This option simply allows for a filtering of picklist items displayed on this form.'),
    '#required' => FALSE,
  );

  $form['display']['size'] = array(
    '#type' => 'textfield',
    '#title' => t('Control Size'),
    '#default_value' => $component['extra']['size'],
    '#description' => t('By default, picklists are rendered as a a drop down (size 1) and multi picklists are rendered as a listbox 4 rows high. The default behavior can be changed by setting a height in rows.'),
    '#required' => FALSE,
    '#element_validate' => array('element_validate_integer_positive'),
    '#size' => 2,
    '#parents' => array('extra', 'size'),
  );

  $form['value'] = array(
    '#type' => 'textfield',
    '#title' => t('Default value'),
    '#default_value' => $component['value'],
    '#description' => t('The default value of the field identified by its key.') . ' ' . theme('webform_token_help'),
    '#size' => 60,
    '#maxlength' => 1024,
    '#weight' => 0,
  );

  $form['display']['sort'] = array(
    '#type' => 'radios',
    '#title' => t('Sort Order'),
    '#default_value' => (!empty($component['extra']['sort'])) ? $component['extra']['sort'] : 0,
    '#description' => t('Determines how options are sorted when they are displayed in the form.'),
    '#parents' => array('extra', 'sort'),
    '#options' => array(
      '0' => t('The order defined in Salesforce'),
      '1' => t('Sorted alphabetically'),
      '2' => t('Random'),
    ),
  );

  return $form;
}

/**
 * Implements _webform_render_component().
 */
function _webform_render_sf_picklist($component, $value = NULL, $filter = TRUE) {
  $wrapper_name = salesforce_webforms_map_fieldid($component['form_key']);
  $element = array(
    '#title' => $component['name'],
    '#title_display' => $component['extra']['title_display'] ? $component['extra']['title_display'] : 'before',
    '#required' => ($component['mandatory'] || $component['required']),
    '#weight' => $component['weight'],
    '#description' => $component['extra']['description'],
    '#theme_wrappers' => array('webform_element'),
    '#translatable' => array('title', 'description', 'options'),
    '#element_validate' => array('salesforce_webforms_validate_picklist'),
    '#prefix' => '<div class="sf-picklist-wrapper" id="' . $wrapper_name . '"><input type="hidden" value="1" name="' . $wrapper_name . '-control">',
    '#suffix' => '</div>',
  );

  $default_value = $component['value'];
  list($options, $control, $full, $fieldname, $fieldtype) = _webform_sf_picklist_options($component);

  $optionmap = array(-1);
  // $i = 0;
  foreach ($full as $opt) {
    $optionmap[] = $opt['position'];
  }

  // Add default options if using a select list with no default.
  // This trigger's Drupal 7's adding of the option for us.
  // See @form_process_select().
  $element['#empty_value'] = '';

  // Set the component options.
  $element['#options'] = $options;

  // Set the default value.
  if (isset($value)) {
    // Set the value as a single string.
    // $element['#default_value'] = '';
    // foreach ((array) $value as $option_value) {
    // $element['#default_value'] = $option_value;
    // }
    $element['#default_value'] = $value;
  }
  elseif ($default_value !== '') {
    $element['#default_value'] = $default_value;
  }

  // Set display as a select list.
  $element['#type'] = 'select';

  // Set the size, if specified.
  if (isset($component['extra']['size']) && $component['extra']['size'] > 0) {
    $element['#size'] = $component['extra']['size'];
  }

  // Set multi select as appropriate.
  $element['#multiple'] = ($fieldtype == 'multipicklist');

  $myid = salesforce_webforms_map_fieldid($component['form_key']);
  $sfobj = array(
    "webform-client-form-" . $component['nid'] => array(
      $myid => array(
        'fieldname' => $fieldname,
        'control' => $control,
        'full' => $full,
        'options' => $options,
        'optionmap' => $optionmap,
      ),
    ));

  $element['#attached']['js'][] = array(
    'data' => array(
      'salesforce_webforms' => array(
        'salesforceMap' => $sfobj,
      ),
    ),
    'type' => 'setting',
  );

  return $element;
}

/**
 * Helper function to generate field IDs based on the component.
 */
function salesforce_webforms_map_fieldid($formkey) {
  return 'sf-picklist-' . str_replace('_', '-', $formkey);
}

/**
 * Implements _webform_display_component().
 */
function _webform_display_sf_picklist($component, $value, $format = 'html') {
  list($options, $control, $full, $fieldpath, $fieldtype) = _webform_sf_picklist_options($component);
  return array(
    '#title' => $component['name'],
    '#title_display' => $component['extra']['title_display'] ? $component['extra']['title_display'] : 'before',
    '#weight' => $component['weight'],
    '#theme' => 'webform_display_sf_picklist',
    '#theme_wrappers' => $format == 'html' ? array('webform_element') : array('webform_element_text'),
    '#format' => $format,
    '#options' => $options,
    '#sf' => array(
      'keyfield' => $control,
      'picklist' => $full,
    ),
    '#value' => (array) $value,
    '#translatable' => array('title', 'options'),
    '#multiple' => ($fieldtype == 'multipicklist'),
    '#size' => $component['extra']['size'],
  );
}

/**
 * Returns HTML for a picklist.
 *
 * @param array $variables
 *   An associative array containing:
 *   - element: The element to render.
 *
 * @ingroup themeable
 */
function theme_webform_display_sf_picklist(array $variables) {
  $element = $variables['element'];

  // Flatten the list of options so we can get values easily.
  // These options may be translated by hook_webform_display_component_alter().
  $options = array();
  foreach ($element['#options'] as $key => $value) {
    if (is_array($value)) {
      foreach ($value as $subkey => $subvalue) {
        $options[$subkey] = $subvalue;
      }
    }
    else {
      $options[$key] = $value;
    }
  }

  $items = array();
  if ($element['#multiple']) {
    return implode(';', $element['#value']);
  }
  elseif (isset($element['#value'][0]) && $element['#value'][0] !== '') {
    // Administer provided values.
    if (isset($options[$element['#value'][0]])) {
      $items[] = $options[$element['#value'][0]];
    }
    // User-specified in the "other" field.
    else {
      $items[] = $element['#format'] == 'html' ? check_plain($element['#value'][0]) : $element['#value'][0];
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
function _webform_analysis_sf_picklist($component, $sids = array(), $single = FALSE) {
  list($options) = _webform_sf_picklist_options($component, TRUE);

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
    $rows[$data['data']] = array($display_option, $data['datacount']);
    $normal_count += $data['datacount'];
  }

  // Order the results according to the normal options array.
  $ordered_rows = array();
  foreach (array_intersect_key($options, $rows) as $key => $label) {
    $ordered_rows[] = $rows[$key];
  }
  $rows = $ordered_rows;

  return $rows;
}

/**
 * Implements _webform_table_component().
 */
function _webform_table_sf_picklist($component, $value) {
  // Convert submitted 'safe' values to un-edited, original form.
  list($options) = _webform_sf_picklist_options($component, TRUE);

  $value = (array) $value;
  $items = array();
  // Set the value as a single string.
  foreach ($value as $option_value) {
    if ($option_value !== '') {
      if (isset($options[$option_value])) {
        $items[] = check_plain($options[$option_value]);
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
function _webform_csv_headers_sf_picklist($component, $export_options) {
  $headers = array(
    0 => array(),
    1 => array(),
    2 => array(),
  );

  $headers[0][] = '';
  $headers[1][] = '';
  $headers[2][] = $export_options['header_keys'] ? $component['form_key'] : $component['name'];
  return $headers;
}

/**
 * Implements _webform_csv_data_component().
 */
function _webform_csv_data_sf_picklist($component, $export_options, $value) {
  list($options) = _webform_sf_picklist_options($component, TRUE, FALSE);
  $return = array();

  $key = $value[0];
  if ($export_options['select_keys']) {
    $return = $key;
  }
  else {
    $return = isset($options[$key]) ? $options[$key] : $key;
  }

  return $return;
}


/**
 * Implements _webform_options_component().
 *
 * This function is an alias of _webform_sf_picklist_options(). However
 * this version is intended to be accessed publicly via
 * webform_component_invoke(), since it is a Webform "hook", rather than an
 * internal, private function. To get the list of select list options for
 * a component, use:
 *
 * @code
 * $options = webform_component_invoke($component['type'],'options',$component);
 * @endcode
 */
function _webform_options_sf_picklist($component) {
  list($opt) = _webform_sf_picklist_options($component);
  return $opt;
}

/**
 * Generate a list of options for a select list.
 *
 * @param array $component
 *   A webform component array.
 *
 * @return array
 *   Returns an array of 5 elements:
 *   - Array of options suitable for use in an 'options' element.
 *   - String name of a field which controls the validity of values within
 *     this field, if this is a dependent picklist.
 *   - The full list of values, possibly sorted, with extra control data to
 *     allow for control between dependent fields.
 *   - String name of this field.
 *   - String field type (picklist or multipicklist)
 */
function _webform_sf_picklist_options(array $component) {
  $options = array();
  $control = NULL;

  $picklist_key = $component['extra']['list'];
  $exclusions = $component['extra']['exclude'];
  $exclusion_rows = array_filter(explode("\n", trim($exclusions)));
  $exclude = array();
  foreach ($exclusion_rows as $row) {
    $row = trim($row);
    $exclude[$row] = $row;
  }

  // Break the key up into the object and field.
  list($obj, $field) = explode(".", $picklist_key);

  // Load that object.
  $info = salesforce_webforms_get_object_fields($obj);

  if ($info[$field]['controllerName']) {
    $control = $obj . "." . $info[$field]['controllerName'];
  }

  $picklist = $info[$field]['picklistValues'];
  $fieldtype = $info[$field]['type'];

  $position = 1;
  foreach ($picklist as &$pick) {
    if ($control) {
      $pick['map'] = _salesforce_webforms_valid_map($pick['validFor']);
    }

    $pick['position'] = $position++;
  }

  $sortorder = $component['extra']['sort'];
  // See if any sorting needs to be done would go here.
  switch ($sortorder) {
    case 1:
      _salesforce_webforms_alpha_options($picklist);
      break;

    case 2:
      _salesforce_webforms_shuffle_options($picklist);
      break;
  }

  // Build the option list, and throw away any ignored values.
  $newlist = array();
  foreach ($picklist as &$pick) {
    if ($pick['active'] && !isset($exclude[$pick['value']])) {
      $options[$pick['value']] = $pick['label'];
      $newlist[] = $pick;
    }
  }

  return array($options, $control, $newlist, $picklist_key, $fieldtype);
}

/**
 * Determines the list of parent items for which this item is valid.
 *
 * @param string $valid
 *   The encoded value from Salesforce's 'validFor' attribute.
 *
 * @return int[]
 *   The array of positions in the parent field where this value is allowed.
 */
function _salesforce_webforms_valid_map($valid) {
  $bitmap = base64_decode($valid);
  $len = strlen($bitmap);

  $map = array();
  for ($i = 0; $i < $len * 8; $i++) {
    $map[$i] = _salesforce_webforms_checkbit($i, $bitmap) ? 1 : 0;
  }

  return $map;
}

/**
 * Sees if a given bit is set in the bitmap.
 */
function _salesforce_webforms_checkbit($idx, $bitmap) {
  $bytes = unpack("c*", $bitmap);
  $byte = $bytes[($idx>>3) + 1];

  return (($byte & (0x80 >> ($idx % 8))) != 0);
}

/**
 * Utility function to shuffle an array while preserving key-value pairs.
 */
function _salesforce_webforms_shuffle_options(&$array) {
  // First shuffle the array keys.
  // Then use them as the basis for ordering the options.
  $aux = array();
  $keys = array_keys($array);
  shuffle($keys);
  foreach ($keys as $key) {
    $aux[$key] = $array[$key];
  }
  $array = $aux;
}

/**
 * Utility function to sort an array while preserving key-value pairs.
 */
function _salesforce_webforms_alpha_options(&$array) {
  usort($array, "_salesforce_webforms_alpha_cmp");
}

/**
 * Utility function to compare two full option arrays by the label.
 *
 * Callback for usort().
 */
function _salesforce_webforms_alpha_cmp($a, $b) {
  return strnatcmp($a['label'], $b['label']);
}

/**
 * Element validation handler for picklists in _webforms_render_sf_picklist().
 *
 * @see _webform_render_sf_picklist()
 */
function salesforce_webforms_validate_picklist(&$el, &$state, $form) {
  $form_key = $el['#webform_component']['form_key'];
  $control = salesforce_webforms_map_fieldid($form_key) . "-control";
  if (!$state['input'][$control]) {
    // We are disabled - flag as OK to be empty.
    $errors = form_get_errors();
    if (!$errors) {
      $errors = array();
    }

    // Clear out error log.
    drupal_get_messages('error');
    form_clear_error();

    // Add back in the errors, except for this one.
    foreach ($errors as $field => $msg) {
      if ($field != "submitted][$form_key") {
        form_set_error($field, $msg);
      }
    }
  }
}
