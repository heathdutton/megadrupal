<?php

/**
 * @file
 * Page callbacks for Formatter Field Preset module.
 */

/**
 * Build form that return all existing Formatter Field Presets.
 */
function formatter_field_preset_list_form($form, $form_state) {

  // Store form values as tree.
  $form['#tree'] = TRUE;

  // Load path breadcrumbs.
  $result = formatter_field_preset_load_all();

  foreach ($result as $object) {

    // Display object human-readable name.
    $form[$object->machine_name]['name']['#markup'] = $object->name;

    // Display object type if it is overwritten.
    if ($object->is_overwritten) {
      $form[$object->machine_name]['name']['#markup'] .= '<sup>'. $object->type .'</sup>';
    }

    // Display preset machine name.
    $form[$object->machine_name]['machine_name']['#markup'] = $object->machine_name;

    // Display preset field.
    $form[$object->machine_name]['field']['#markup'] = $object->field;

    // Display preset formatter.
    $form[$object->machine_name]['format']['#markup'] = $object->format;

    // Create some operations for current exportable object.
    $operations = array();

    // If object is disabled we have to display link to enable action.
    if ($object->disabled == TRUE) {
      $operations[] = array(
        'title' => t('Enable'),
        'href' => 'admin/config/content/formatter-field-preset/' . $object->machine_name . '/enable',
      );
    }

    // Link to page with object edit form.
    $operations[] = array(
      'title' => t('Edit'),
      'href' => 'admin/config/content/formatter-field-preset/' . $object->machine_name . '/edit',
    );

    // Link to page with object export code.
    $operations[] = array(
      'title' => t('Export'),
      'href' => 'admin/config/content/formatter-field-preset/' . $object->machine_name . '/export',
    );

    // If object are able to store not only in code we could delete (or revert) it.
    // If object was created with hook_exportable_example_object_settings_info() it could be only reverted.
    if (empty($object->in_code_only)) {
      $operations[] = array(
        'title' => $object->is_overwritten ? t('Revert') : t('Delete'),
        'href' => 'admin/config/content/formatter-field-preset/' . $object->machine_name . '/delete',
      );
    }

    // If object is enabled display link to disable action.
    if ($object->disabled == FALSE) {
      $operations[] = array(
        'title' => t('Disable'),
        'href' => 'admin/config/content/formatter-field-preset/' . $object->machine_name . '/disable',
      );
    }

    // Render operation links as CTools dropdown links. They will be processes with dropdown javasctipt.
    $form[$object->machine_name]['actions']['#markup'] = theme('links__ctools_dropbutton',
      array('links' => $operations, 'attributes' => array('class' => array('links', 'inline'))));

    // Display exportable object status.
    $form[$object->machine_name]['disabled'] = array(
      '#type' => 'value',
      '#value' => $object->disabled,
    );
  }

  // Add styles for exportable objects list page.
  $form['#attached']['css'][] = drupal_get_path('module', 'formatter_field_preset') . '/formatter_field_preset.css';

  // Return array with builded form.
  return $form;
}

/**
 * Preset form.
 */
function formatter_field_preset_form($form, &$form_state, $formatter_field_preset = NULL) {
  // Create a new empty preset if none was passed.
  $is_new = empty($formatter_field_preset);
  if ($is_new) {
    $formatter_field_preset = formatter_field_preset_create();
  }

  // Save the preset, we'll use lately.
  $form_state['formatter_field_preset'] = $formatter_field_preset;

  // Object human-readable name. Needs for display to user.
  $form['name'] = array(
    '#type' => 'textfield',
    '#title' => t('Preset name'),
    '#description' => t('Human-readable name of preset'),
    '#default_value' => $is_new ? '' : $formatter_field_preset->name,
    '#required' => TRUE,
    '#maxlength' => 255,
  );

  // Object machine name. Needs for many export operations.
  $form['machine_name'] = array(
    '#type' => 'machine_name',
    '#default_value' => $is_new ? '' : $formatter_field_preset->machine_name,
    '#disabled' => !$is_new,
    '#maxlength' => 64,
    '#machine_name' => array(
      'exists' => 'formatter_field_preset_exists',
      'source' => array('name'),
    ),
  );

  $options = array('' => '-' . t('Select') . '-');
  foreach (field_info_fields() as $field_id => $field) {
    if ($field['type'] != 'formatter') {
      // Get the field label.
      $field_info = module_invoke($field['module'], 'field_info');
      $label = $field_info[$field['type']]['label'];
      $options[$field_id] = $field_id . ': ' . $label;
    }
  }

  // Add only field name now.
  $form['field'] = array(
    '#type' => 'select',
    '#title' => t('Field to be formatted'),
    '#required' => TRUE,
    '#options' => $options,
    '#default_value' => $formatter_field_preset->field,
    '#description' => t('The field to be formatted using this preset.'),
    '#ajax' => array(
      'callback' => 'formatter_field_preset_form_js',
      'wrapper' => 'formatter-field-preset-envelope',
      'effect' => 'fade',
    ),
  );

  $settings_form = array();

  // Field to be formatted.
  $field_name = !empty($form_state['values']['field']) ? $form_state['values']['field'] : $formatter_field_preset->field;
  $formatted_field = field_read_field($field_name);

  $form['wrapper'] = array(
    '#type' => 'container',
    '#prefix' => '<div id="formatter-field-preset-envelope">',
    '#suffix' => '</div>',
  );

  if (!empty($formatted_field)) {
    // Fetch formatter options, excluding the "from_field" formatter.
    module_load_include('inc', 'field_ui', 'field_ui.admin');
    $formatter_options = array('' => '-' . t('Select') . '-');
    $formatter_options += field_ui_formatter_options($formatted_field['type']);
    unset($formatter_options['from_field']);

    // Populate $items from $form_state values if available.  This is necessary
    // for the #ajax functionality.
    $item = array();
    if (empty($form_state['values']['field'])) {
      $item['format'] = $formatter_field_preset->format;
      $item['settings'] = $formatter_field_preset->settings;
    }
    else {
      $item['format'] = $form_state['values']['format'];
      $item['settings'] = empty($form_state['values']['settings']) ? array() : $form_state['values']['settings'];
    }

    $form['wrapper']['format'] = array(
      '#type' => 'select',
      '#title' => t('Formatter for %field field', array('%field' => $field_name)),
      '#options' => $formatter_options,
      '#default_value' => isset($item['format']) ? $item['format'] : '',
      '#required' => TRUE,
      '#ajax' => array(
        'callback' => 'formatter_field_preset_form_js',
        'wrapper' => 'formatter-field-preset-settings-envelope',
        'effect' => 'fade',
      ),
    );

    // Retrieve the settings form segment, if $items[$delta] is available.
    if (!empty($item['format'])) {
      $formatter = field_info_formatter_types($item['format']);
      $view_mode = '_custom_display';
      $formatted_instance['display'][$view_mode] = array(
        'type' => $item['format'],
        'settings' => $item['settings'],
        'module' => $formatter['module'],
      );

      $function = $formatter['module'] . '_field_formatter_settings_form';
      if (function_exists($function)) {
        $settings_form = $function($formatted_field, $formatted_instance, $view_mode, $form, $form_state);
      }
    }

    $form['wrapper']['settings'] = $settings_form + array(
      '#type' => 'container',
      '#prefix' => '<div id="formatter-field-preset-settings-envelope">',
      '#suffix' => '</div>',
      '#tree' => TRUE,
    );
  }

  $form['actions'] = array(
    '#type' => 'action',
  );
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => empty($formatter_field_preset->machine_name) ? t('Create') : t('Update'),
  );
  return $form;
}

/**
 * Ajax callback for preset edit form.
 */
function formatter_field_preset_form_js($form, &$form_state) {
  $trigger = $form_state['triggering_element'];
  $path = $trigger['#parents'];
  $source = array_pop($path);
  $path[] = 'wrapper';

  if ($source == 'field') {
    foreach ($form['wrapper']['settings'] as $item_id => $item) {
      if ($item_id{0} != '#') {
        unset($form['wrapper']['settings'][$item_id]);
      }
    }
  }
  elseif ($source == 'format') {
    $path[] = 'settings';
  }
  return  drupal_array_get_nested_value($form, $path);
}

/**
 * Checker for '#machine_name' on preset manchine name field.
 */
function formatter_field_preset_exists($name) {
  // Do not allow 'hidden' as preset machine name.
  if ($name == 'hidden') {
    return TRUE;
  }  
  return formatter_field_preset_load($name);
}

/**
 * Submit callback for preset form.
 */
function formatter_field_preset_form_submit($form, &$form_state) {
  $values =& $form_state['values'];
  $formatter_field_preset = $form_state['formatter_field_preset'];

  $formatter_field_preset->name = $values['name'];
  $formatter_field_preset->machine_name = $values['machine_name'];
  $formatter_field_preset->field = $values['field'];
  $formatter_field_preset->format = $values['format'];
  $formatter_field_preset->settings = empty($values['settings']) ? array() : $values['settings'];

  // Save exportable object in database.
  $save_result = formatter_field_preset_save($formatter_field_preset);

  $message = $save_result ? t('Preset %preset was saved.', array('%preset' => $values['name'])) : t('Failed to save preset.');
  drupal_set_message($message);
  $form_state['redirect'] = 'admin/config/content/formatter-field-preset';
}

/**
 * Form for import code with presets.
 */
function formatter_field_preset_import_form($form, $form_state) {

  // Textarea for pasting import code.
  $form['import'] = array(
    '#type' => 'textarea',
    '#title' => t('Paste code here to import presets.'),
  );

  $form['actions'] = array(
    '#type' => 'action',
  );

  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Import'),
  );

  return $form;
}

/**
 * Validate callback for preset import form. Checks whether presets can be
 * imported.
 */
function formatter_field_preset_import_form_validate($form, &$form_state) {

  // Include CTools library for import/export objects.
  ctools_include('export');

  // Get user import code.
  $code = $form_state['values']['import'];

  // Try to load objects with import code.
  $formatter_field_preset_raw = ctools_export_crud_import('formatter_field_preset', $code);
  if (is_string($formatter_field_preset_raw)) {
    form_set_error('import', t('Unable to get an import from the code. Errors reported: @errors', array('@errors' => $formatter_field_preset_raw)));
  }
  else {
    $form_state['storage']['formatter_field_preset_raw'] = $formatter_field_preset_raw;
  }
}

/**
 * Submit callback for presets import form. Saves imported objects.
 */
function formatter_field_preset_import_form_submit($form, &$form_state) {

  // Include CTools library for import/export objects.
  ctools_include('export');

  // Get parsed preset.
  $formatter_field_preset = $form_state['storage']['formatter_field_preset_raw'];

  // Prepare preset for saving. Emulate full object load.
  $formatter_field_preset = formatter_field_preset_load_prepare($formatter_field_preset);

  // Check if preset with the same machine name exists.
  $formatter_field_preset_original = formatter_field_preset_load($formatter_field_preset->machine_name);

  // If preset with the same machine name exists - delete it.
  if (!empty($formatter_field_preset_original)) {
    formatter_field_preset_delete($formatter_field_preset_original->machine_name);
  }

  // Save imported preset in database.
  if (formatter_field_preset_save($formatter_field_preset)) {
    // Set up a message depends on recent action.
    if (!empty($formatter_field_preset_original)) {
      drupal_set_message(filter_xss(t('Preset "!name" was successfully updated.', array('!name' => $formatter_field_preset->name))));
    }
    else {
      drupal_set_message(filter_xss(t('Preset "!name" was successfully imported.', array('!name' => $formatter_field_preset->name))));
    }
  }

  // Redirect user to list with all presets.
  $form_state['redirect'] = 'admin/config/content/formatter-field-preset';
}

/**
 * Return exportable code for a single preset.
 */
function formatter_field_preset_export_form($form, $form_state, $formatter_field_preset) {

  // Set up actual page title.
  drupal_set_title(filter_xss(t('Export preset "!name"', array('!name' => $formatter_field_preset->name))));

  // Include CTools library for import/export objects.
  ctools_include('export');

  // Parse loaded preset into import code.
  $code = ctools_export_crud_export('formatter_field_preset', $formatter_field_preset);

  // Display parsed code in the textarea.
  $lines = substr_count($code, "\n");
  $form['export'] = array(
    '#title' => t('Export data'),
    '#type' => 'textarea',
    '#value' => $code,
    '#rows' => $lines,
    '#description' => t('Copy the export text and paste it into import area.'),
  );

  return $form;
}

/**
 * Form for that allows user to delete preset from database.
 */
function formatter_field_preset_delete_form($form, $form_state, $formatter_field_preset) {

  $form['name'] = array(
    '#type' => 'value',
    '#value' => $formatter_field_preset->name,
  );
  $form['machine_name'] = array(
    '#type' => 'value',
    '#value' => $formatter_field_preset->machine_name,
  );

  if ($formatter_field_preset->is_overwritten) {
    $form['message'] = array(
      '#markup' => t('Are you sure that you want to revert preset %name from database?', array('%name' => $formatter_field_preset->name))
    );
  }
  else {
    $form['message'] = array(
      '#markup' => t('Are you sure that you want to delete preset %name from database?', array('%name' => $formatter_field_preset->name))
    );
  }

  $form['actions'] = array(
    '#type' => 'actions'
  );

  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => $formatter_field_preset->is_overwritten ? t('Revert') : t('Delete'),
  );

  $form['actions']['cancel'] = array(
    '#type' => 'submit',
    '#value' => t('Cancel'),
  );

  return $form;
}

/**
 * Submit callback for preset delete form. Remove object from database.
 */
function formatter_field_preset_delete_form_submit($form, &$form_state) {
  $values =& $form_state['values'];

  // Delete preset from database.
  if ($values['op'] == $values['submit']) {
    formatter_field_preset_delete($values['machine_name']);
    drupal_set_message(t('Preset %name was successfully deleted.', array('%name' => $values['name'])));
  }

  // Redirect user to list with all presets.
  $form_state['redirect'] = 'admin/config/content/formatter-field-preset';
}

/**
 * Page callback. Enables or disables selected preset.
 *
 * @param object $preset
 *   Preset loaded from database.
 * @param string $operation
 *   Operations: 'enable' or 'disable'.
 */
function formatter_field_preset_toggle_page($formatter_field_preset, $operation = 'enable') {
  $operations = array(
    'enable' => t('Preset %name was enabled.', array('%name' => $formatter_field_preset->name)),
    'disable' => t('Preset %name was disabled.', array('%name' => $formatter_field_preset->name)),
  );

  if (!in_array($operation, array_keys($operations))) {
    return;
  }

  // Include CTools library for import/export objects.
  ctools_include('export');

  // Toggle preset using CTools CRUD operations.
  $function = 'ctools_export_crud_' . $operation;
  $function('formatter_field_preset', $formatter_field_preset);

  // Set up a message about successfull enabling or disabling.
  drupal_set_message($operations[$operation]);

  // Redirect user to list with all presets.
  drupal_goto('admin/config/content/formatter-field-preset');
}

/**
 * Return code that presents preset.
 *
 * @param $formatter_field_preset
 *   Preset loaded from database.
 * @param $indent
 *   Variable shows how much code lines should be intended for hierarchical structure.
 * @return string
 *   Exportable code.
 */
function formatter_field_preset_export($formatter_field_preset, $indent) {
  // Simply call CTools export function. He will do rest of work for you.
  return ctools_export_object('formatter_field_preset', $formatter_field_preset, $indent);
}

/**
 * Provide list of presets for exportable interface.
 * Example: Features of CTools bulk export.
 *
 * @return array
 *   Array with available presets.
 */
function formatter_field_preset_export_list() {

  // Load all available presets from database.
  $result = formatter_field_preset_load_all();

  // Build array with presets. This list will be displayed in exportable UI.
  $list = array();
  foreach ($result as $formatter_field_preset) {
    $string = $formatter_field_preset->name . " (" . $formatter_field_preset->machine_name . ")";
    $list[$formatter_field_preset->machine_name] = check_plain($string);
  }

  // Return array with presets.
  return $list;
}
