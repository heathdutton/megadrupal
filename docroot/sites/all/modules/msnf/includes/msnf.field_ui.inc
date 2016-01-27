<?php

/**
 * @file
 * msnf.field_ui.inc is a file that contains most functions
 * needed on the Fields UI Manage forms (display and fields).
 */

/**
 * Helper function to get the form parameters to use while
 * building the fields overview form.
 */
function msnf_field_ui_form_params($form) {
  $params = new stdClass();
  $params->entity_type = $form['#entity_type'];
  $params->bundle = $form['#bundle'];
  $params->admin_path = _field_ui_bundle_admin_path($params->entity_type, $params->bundle);

  $params->region_callback = 'msnf_field_overview_row_region';

  $params->steps = msnf_info_steps($params->entity_type, $params->bundle, TRUE);

  // Gather parenting data.
  $params->parents = array();
  foreach ($params->steps as $name => $step) {
    foreach ($step->children as $child) {
      $params->parents[$child] = $name;
    }
  }

  return $params;
}

/**
 * Function to alter the fields overview screen.
 */
function msnf_field_ui_overview_form_alter(&$form, &$form_state) {
  // Only start altering the form if we need to.
  if (!isset($form['#entity_type']) || ($form['#entity_type'] != 'node') || (empty($form['#fields']) && empty($form['#extra']))) {
    return;
  }

  // Get form params.
  $params = msnf_field_ui_form_params($form);

  // Add some things to be able to preserve synced usage of field_ui.
  // This key is used to store the current updated field.
  $form_state += array(
    'formatter_settings_edit' => NULL,
  );
  // Add AJAX wrapper.
  $form['fields']['#prefix'] = '<div id="field-display-overview-wrapper">';
  $form['fields']['#suffix'] = '</div>';
  $form['#steps'] = array_keys($params->steps);

  $table = &$form['fields'];

  // Add a region for 'add_new' rows, but only when fields are available and
  // thus regions.
  if (isset($table['#regions'])) {
    $table['#regions'] += array(
      'add_new' => array('title' => '&nbsp;'),
    );
  }

  // Extend available parenting options.
  foreach ($params->steps as $name => $step) {
    $table['#parent_options'][$name] = $step->label;
  }
  $table['#parent_options']['_add_new_step'] = t('Add new step');

  // Update existing rows accordingly to the parents.
  foreach (element_children($table) as $name) {
    $table[$name]['parent_wrapper']['parent']['#options'] = $table['#parent_options'];
    // Inherit the value of the parent when default value is empty.
    if (empty($table[$name]['parent_wrapper']['parent']['#default_value'])) {
      $table[$name]['parent_wrapper']['parent']['#default_value'] = isset($params->parents[$name]) ? $params->parents[$name] : '';
    }
  }

  // Get formatter options.
  $formatter_options = msnf_field_formatter_options();

  $refresh_rows = isset($form_state['values']['refresh_rows']) ? $form_state['values']['refresh_rows'] : (isset($form_state['input']['refresh_rows']) ? $form_state['input']['refresh_rows'] : NULL);

  // Create the step rows and check actions.
  foreach (array_keys($params->steps) as $name) {

    // Get step either from form state or params.
    if (isset($form_state['msnf_step'][$name])) {
      $step = & $form_state['msnf_step'][$name];
    }
    else {
      $step = & $params->steps[$name];
    }

    // Check the currently selected formatter, and merge persisted values for
    // formatter settings for the step.
    // This needs to be done first, so all fields are updated before creating
    // form elements.
    if (isset($refresh_rows) && $refresh_rows == $name) {
      $settings = isset($form_state['values']['fields'][$name]) ? $form_state['values']['fields'][$name] : (isset($form_state['input']['fields'][$name]) ? $form_state['input']['fields'][$name] : NULL);
      if (array_key_exists('settings_edit', $settings)) {
        $step = $form_state['msnf_step'][$name];
      }
      msnf_formatter_row_update($step, $settings);
    }

    // Save the step when the configuration is submitted.
    if (!empty($form_state['values'][$name . '_formatter_settings_update'])) {
      msnf_formatter_settings_update($step, $form_state['values']['fields'][$name]);
    }
    // After all updates are finished, let the form_state know.
    $form_state['msnf_step'][$name] = $step;

    // Get the settings form.
    $settings = msnf_format_settings_form($step);

    $id = strtr($name, '_', '-');
    $js_rows_data[$id] = array('type' => 'group', 'name' => $name);
    // A group cannot be selected as its own parent.
    $parent_options = $table['#parent_options'];
    unset($parent_options[$name]);
    $table[$name] = array(
      '#attributes' => array('class' => array('draggable', 'msnf-step'), 'id' => $id),
      '#row_type' => 'formstep',
      '#region_callback' => $params->region_callback,
      '#js_settings' => array('rowHandler' => 'group'),
      'human_name' => array(
        '#markup' => check_plain(t($step->label)),
        '#prefix' => '<span class="step-label">',
        '#suffix' => '</span>',
      ),
      'weight' => array(
        '#type' => 'textfield',
        '#default_value' => $step->weight,
        '#size' => 3,
        '#attributes' => array('class' => array('field-weight')),
      ),
      'parent_wrapper' => array(
        'parent' => array(
          '#type' => 'select',
          '#options' => $parent_options,
          '#empty_value' => '',
          '#default_value' => isset($params->parents[$name]) ? $params->parents[$name] : '',
          '#attributes' => array('class' => array('field-parent')),
          '#parents' => array('fields', $name, 'parent'),
        ),
        'hidden_name' => array(
          '#type' => 'hidden',
          '#default_value' => $name,
          '#attributes' => array('class' => array('field-name')),
        ),
      ),
    );

    $table[$name] += array(
      'step_name' => array(
        '#markup' => check_plain($name),
      ),
      'format' => array(
        'type' => array(
          '#type' => 'select',
          '#options' => $formatter_options,
          '#default_value' => $step->format_type,
          '#attributes' => array('class' => array('msnf-step-type')),
        ),
      ),
    );

    $base_button = array(
      '#submit' => array('field_ui_display_overview_multistep_submit'),
      '#ajax' => array(
        'callback' => 'field_ui_display_overview_multistep_js',
        'wrapper' => 'field-display-overview-wrapper',
        'effect' => 'fade',
      ),
      '#field_name' => $name,
    );

    if ($form_state['formatter_settings_edit'] == $name) {
      $table[$name]['format']['#cell_attributes'] = array('colspan' => 3);
      $table[$name]['format']['format_settings'] = array(
        '#type' => 'container',
        '#attributes' => array('class' => array('field-formatter-settings-edit-form')),
        '#parents' => array('fields', $name, 'format_settings'),
        '#weight' => -5,
        'label' => array(
          '#markup' => t('Step format:') . ' <span class="formatter-name">' . $step->format_type . '</span>',
        ),
        // Create a settings form where hooks can pick in.
        'settings' => $settings,
        'actions' => array(
          '#type' => 'actions',
          'save_settings' => $base_button + array(
            '#type' => 'submit',
            '#name' => $name . '_formatter_settings_update',
            '#value' => t('Update'),
            '#op' => 'update',
          ),
          'cancel_settings' => $base_button + array(
            '#type' => 'submit',
            '#name' => $name . '_formatter_settings_cancel',
            '#value' => t('Cancel'),
            '#op' => 'cancel',
            // Do not check errors for the 'Cancel' button.
            '#limit_validation_errors' => array(),
          ),
        ),
      );
      $table[$name]['#attributes']['class'][] = 'field-formatter-settings-editing';
      $table[$name]['format']['type']['#attributes']['class'] = array('element-invisible');
    }
    else {
      // After saving, the settings are updated here aswell. First we create
      // the element for the table cell.
      $table[$name]['settings_summary'] = array('#markup' => '');
      if (!empty($step->format_settings)) {
        $table[$name]['settings_summary'] = msnf_format_settings_summary($name, $step);
      }
      // Add the configure button.
      $table[$name]['settings_edit'] = $base_button + array(
        '#type' => 'image_button',
        '#name' => $name . '_step_settings_edit',
        '#src' => 'misc/configure.png',
        '#attributes' => array('class' => array('field-formatter-settings-edit'), 'alt' => t('Edit')),
        '#op' => 'edit',
        // Do not check errors for the 'Edit' button.
        '#limit_validation_errors' => array(),
        '#prefix' => '<div class="field-formatter-settings-edit-wrapper">',
        '#suffix' => '</div>',
      );
    }
    $table[$name] += array(
      'delete' => array(
        '#markup' => l(t('delete'), $params->admin_path . '/steps/' . $name . '/delete/form'),
      ),
    );
  }

  // Additional row: add new step.
  $parent_options = $table['#parent_options'];
  unset($parent_options['_add_new_step']);
  $table['_add_new_step'] = msnf_add_row('_add_new_step', $parent_options, $params);

  $table['_add_new_step'] += array(
    'format' => array(
      'type' => array(
        '#type' => 'select',
        '#options' => $formatter_options,
        '#default_value' => 'default',
      ),
    ),
    'settings_summary' => array(),
    'settings_edit' => array(),
  );

  // @see field_ui.admin.inc
  $form['refresh_rows'] = array('#type' => 'hidden');
  $form['refresh'] = array(
    '#type' => 'submit',
    '#value' => t('Refresh'),
    '#op' => 'refresh_table',
    '#submit' => array('field_ui_display_overview_multistep_submit'),
    '#ajax' => array(
      'callback' => 'field_ui_display_overview_multistep_js',
      'wrapper' => 'field-display-overview-wrapper',
      'effect' => 'fade',
      // The button stays hidden, so we hide the AJAX spinner too. Ad-hoc
      // spinners will be added manually by the client-side script.
      'progress' => 'none',
    ),
  );

  $form['#attached']['css'][] = drupal_get_path('module', 'msnf') . '/theme/msnf.field_ui.css';
  $form['#attached']['js'][] = drupal_get_path('module', 'msnf') . '/js/msnf.field_ui.js';

  $form['#validate'][] = 'msnf_field_overview_validate';
  $form['#submit'][] = 'msnf_field_overview_submit';
}

/**
 * Return an array of msnf_formatter options.
 */
function msnf_field_formatter_options() {
  $options = &drupal_static(__FUNCTION__);

  if (!isset($options)) {
    $options = array();
    $step_types = msnf_formatter_info();
    foreach ($step_types as $name => $step_type) {
      $options[$name] = $step_type['label'];
    }
  }
  return $options;
}

/**
 * Helper function to add a row in the overview forms.
 */
function msnf_add_row($name, $parent_options, $params) {
  if (!isset($params->mode)) {
    $params->mode = 'form';
  }
  return array(
    '#attributes' => array('class' => array('draggable', 'msnf-step', 'add-new')),
    '#row_type' => 'add_new_step',
    '#js_settings' => array('rowHandler' => 'group'),
    '#region_callback' => $params->region_callback,
    'label' => array(
      '#type' => 'textfield',
      '#size' => 15,
      '#description' => t('Label'),
      '#prefix' => '<div class="label-input"><div class="add-new-placeholder">' . t('Add new step') . '</div>',
      '#suffix' => '</div>',
    ),
    'weight' => array(
      '#type' => 'textfield',
      '#default_value' => field_info_max_weight($params->entity_type, $params->bundle, $params->mode) + 3,
      '#size' => 3,
      '#title_display' => 'invisible',
      '#title' => t('Weight for new step'),
      '#attributes' => array('class' => array('field-weight')),
      '#prefix' => '<div class="add-new-placeholder">&nbsp;</div>',
    ),
    'parent_wrapper' => array(
      'parent' => array(
        '#type' => 'select',
        '#options' => $parent_options,
        '#empty_value' => '',
        '#attributes' => array('class' => array('field-parent')),
        '#prefix' => '<div class="add-new-placeholder">&nbsp;</div>',
        '#parents' => array('fields', $name, 'parent'),
      ),
      'hidden_name' => array(
        '#type' => 'hidden',
        '#default_value' => $name,
        '#attributes' => array('class' => array('field-name')),
      ),
    ),
    'step_name' => array(
      '#type' => 'machine_name',
      '#title' => t('New step name'),
      '#title_display' => 'invisible',
      // This field should stay LTR even for RTL languages.
      '#field_prefix' => '<span dir="ltr">step_',
      '#field_suffix' => '</span>&lrm;',
      '#attributes' => array('dir' => 'ltr'),
      '#size' => 15,
      // Set max length to prevent users from entering names that are to long.
      '#max_length' => 27,
      '#description' => t('Step name (a-z, 0-9, _)'),
      '#prefix' => '<div class="add-new-placeholder">&nbsp;</div>',
      '#machine_name' => array(
        'source' => array('fields', $name, 'label'),
        'exists' => '_msnf_step_name_exists',
        'standalone' => TRUE,
        'label' => '',
      ),
      '#required' => FALSE,
      '#cell_attributes' => array('colspan' => 2),
    ),
  );
}

/**
 * Creates a form for step formatters.
 *
 * @param object $step
 *   The step object.
 */
function msnf_format_settings_form(&$step) {
  $form = array();
  $form['label'] = array(
    '#type' => 'textfield',
    '#title' => t('Step label'),
    '#default_value' => $step->label,
    '#weight' => -5,
  );
  $form += module_invoke_all('msnf_format_settings', $step);
  $form['#validate'] = array('msnf_format_settings_form_validate');
  return $form;
}

/**
 * Update the row so that the step variables are updated.
 *
 * The rendering of the elements needs the updated defaults.
 *
 * @param object $step
 *   The step object.
 * @param array $settings
 *   Configuration settings for the formatter.
 */
function msnf_formatter_row_update(&$step, $settings) {
  // If the row has changed formatter type, update the step object.
  if (!empty($settings['format']['type']) && $settings['format']['type'] != $step->format_type) {
    $step->format_type = $settings['format']['type'];
    msnf_formatter_settings_update($step, $settings);
  }
}

/**
 * Update handler for step configuration settings.
 *
 * @param object $step
 *   The step object.
 * @param array $settings
 *   Configuration settings for the formatter.
 */
function msnf_formatter_settings_update(&$step, $settings) {
  // Fetch the step formatter defaults.
  $step_types = msnf_formatter_info();
  $formatter = $step_types[$step->format_type];

  // For format changes we load the defaults.
  if (empty($settings['format_settings']['settings'])) {
    $step->format_settings = array(
      'instance_settings' => $formatter['instance_settings'],
    );
  }
  else {
    $step->format_type = $settings['format']['type'];
    $step->label = $settings['format_settings']['settings']['label'];
    $step->format_settings = $settings['format_settings']['settings'];
  }

  if (module_exists('i18n_string')) {
    // Update label for translation.
    if (drupal_strlen($step->label) > 0) {
      i18n_string_update(array('msnf_step', $step->identifier, $step->bundle, 'label'), $step->label);
    }
    else {
      i18n_string_remove(array('msnf_step', $step->identifier, $step->bundle, 'label'));
    }
    // Update step description for translation.
    if (drupal_strlen($step->format_settings['instance_settings']['description']) > 0) {
      i18n_string_update(array('msnf_step', $step->identifier, $step->bundle, 'description'), $step->format_settings['instance_settings']['description']);
    }
    else {
      i18n_string_remove(array('msnf_step', $step->identifier, $step->bundle, 'description'));
    }
    // Update step buttons for translation.
    foreach (array('previous', 'next', 'skip') as $button) {
      if (drupal_strlen($step->format_settings['instance_settings']['buttons'][$button]) > 0) {
        i18n_string_update(array('msnf_step', $step->identifier, $step->bundle, "button_{$button}"), $step->format_settings['instance_settings']['buttons'][$button]);
      }
      else {
        i18n_string_remove(array('msnf_step', $step->identifier, $step->bundle, "button_{$button}"));
      }
    }
  }

}

/**
 * Creates a summary for the field format configuration summary.
 *
 * @param string $step_name
 *   The name of the form step.
 * @param object $step
 *   The step object.
 * @return array
 *   Renderable array.
 */
function msnf_format_settings_summary($step_name, $step) {
  $summary = implode('<br />', module_invoke_all('msnf_format_summary', $step));
  return array(
    '#markup' => '<div class="field-formatter-summary">' . $summary . '</div>',
    '#cell_attributes' => array('class' => array('field-formatter-summary-cell')),
  );
}

/**
 * Returns the region to which a row in the 'Manage fields' screen belongs.
 *
 * @param array $row
 *   A field or step row
 * @return string
 *   The current region.
 */
function msnf_field_overview_row_region($row) {
  switch ($row['#row_type']) {
    case 'formstep':
      return 'main';
    case 'add_new_step':
      // If no input in 'label', assume the row has not been dragged out of the
      // 'add new' section.
      if (empty($row['label']['#value'])) {
        return 'add_new';
      }
      return 'main';
  }
}

/**
 * Validate handler for the overview screens.
 *
 * @param array $form
 *   The complete form.
 * @param array $form_state
 *   The state of the form.
 */
function msnf_field_overview_validate($form, &$form_state) {
  $form_values = $form_state['values']['fields'];
  $entity_type = $form['#entity_type'];
  $bundle = $form['#bundle'];

  $step = $form_values['_add_new_step'];

  // Validate if any information was provided in the 'add new step' row.
  if (array_filter(array($step['label'], $step['step_name']))) {
    // Missing label.
    if (!$step['label']) {
      form_set_error('fields][_add_new_step][label', t('Add new step: you need to provide a label.'));
    }

    // Missing step name.
    if (!$step['step_name']) {
      form_set_error('fields][_add_new_step][step_name', t('Add new step: you need to provide a step name.'));
    }
    // Step name validation.
    else {
      $step_name = $step['step_name'];

      // Add the 'step_' prefix.
      if (drupal_substr($step_name, 0, 5) != 'step_') {
        $step_name = 'step_' . $step_name;
        form_set_value($form['fields']['_add_new_step']['step_name'], $step_name, $form_state);
      }

      // Invalid step name.
      if (!preg_match('!^step_[a-z0-9_]+$!', $step_name)) {
        form_set_error('fields][_add_new_step][step_name', t('Add new step: the step name %step_name is invalid. The name must include only lowercase unaccentuated letters, numbers, and underscores.', array('%step_name' => $step_name)));
      }
      if (drupal_strlen($step_name) > 32) {
        // This should never happen as we limit the input to 27 characters.
        form_set_error('fields][_add_new_step][step_name', t("Add new step: the step name %step_name is too long. The name is limited to 32 characters, including the 'step_' prefix.", array('%step_name' => $step_name)));
      }
    }
  }
}

/**
 * Render API callback: Checks if a step machine name is taken.
 *
 * @param <string> $value
 *   The machine name, not prefixed with 'step_'.
 *
 * @return <boolean>
 *   Whether or not the step machine name is taken.
 */
function _msnf_step_name_exists($value) {
  // Prefix with 'step_'.
  $step_name = 'step_' . $value;

  // We need to check inactive steps as well, so we can't use
  // field_info_fields().
  return (bool) msnf_read_steps(array('step_name' => $step_name), array('include_inactive' => TRUE));
}

/**
 * Submit handler for the overview screens.
 *
 * @param array $form
 *   The complete form.
 * @param array $form_state
 *   The state of the form.
 */
function msnf_field_overview_submit($form, &$form_state) {
  $form_values = $form_state['values']['fields'];
  $entity_type = $form['#entity_type'];
  $bundle = $form['#bundle'];

  // Collect children.
  $children = array_fill_keys($form['#steps'], array());
  foreach ($form_values as $name => $value) {
    if (!empty($value['parent'])) {
      // Substitute newly added fields, in case they were dragged directly in a
      // step.
      if ($name == '_add_new_field' && isset($form_state['fields_added']['_add_new_field'])) {
        $name = $form_state['fields_added']['_add_new_field'];
      }
      elseif ($name == '_add_existing_field' && isset($form_state['fields_added']['_add_existing_field'])) {
        $name = $form_state['fields_added']['_add_existing_field'];
      }
      $children[$value['parent']][$name] = $name;
    }
  }

  // Prepare storage with ctools.
  ctools_include('export');

  // Create new step.
  if (!empty($form_values['_add_new_step']['step_name'])) {
    $values = $form_values['_add_new_step'];

    $step_types = msnf_formatter_info();
    $formatter = $step_types[$values['format']['type']];

    $new_step = (object) array(
      'identifier' => $values['step_name'] . '|' . $entity_type . '|' . $bundle,
      'step_name' => $values['step_name'],
      'entity_type' => $entity_type,
      'bundle' => $bundle,
      'children' => isset($children['_add_new_step']) ? array_keys($children['_add_new_step']) : array(),
      'parent_name' => $values['parent'],
      'weight' => $values['weight'],
      'label' => $values['label'],
      'format_type' => $values['format']['type'],
      'disabled' => FALSE,
    );
    $new_step->format_settings = array();
    if (isset($formatter['instance_settings'])) {
      $new_step->format_settings['instance_settings'] = $formatter['instance_settings'];
    }

    // Save and enable it in ctools.
    ctools_export_crud_save('msnf_step', $new_step);
    ctools_export_crud_enable('msnf_step', $new_step->identifier);

    // Store new step information for any additional submit handlers.
    $form_state['steps_added']['_add_new_step'] = $new_step->step_name;
    drupal_set_message(t('New step %label successfully created.', array('%label' => $new_step->label)));

    // Replace the newly created step in the $children array, in case it was
    // dragged directly in an existing field.
    foreach (array_keys($children) as $parent) {
      if (isset($children[$parent]['_add_new_step'])) {
        unset($children[$parent]['_add_new_step']);
        $children[$parent][$new_step->step_name] = $new_step->step_name;
      }
    }
  }

  // Update existing steps.
  $steps = msnf_info_steps($entity_type, $bundle, TRUE);
  foreach ($form['#steps'] as $step_name) {
    $step = $steps[$step_name];
    $step->label = $form_state['msnf_step'][$step_name]->label;
    $step->children = array_keys($children[$step_name]);
    $step->parent_name = $form_values[$step_name]['parent'];
    $step->weight = $form_values[$step_name]['weight'];
    $step->format_type = isset($form_values[$step_name]['format']['type']) ? $form_values[$step_name]['format']['type'] : 'visible';
    if (isset($form_state['msnf_step'][$step_name]->format_settings)) {
      $step->format_settings = $form_state['msnf_step'][$step_name]->format_settings;
    }
    ctools_export_crud_save('msnf_step', $step);
  }

  cache_clear_all('msnf_step', 'cache_field');
}

/**
 * Validate the entered css class from the submitted format settings.
 *
 * @param array $element
 *   The validated element.
 * @param array $form_state
 *   The current state of the form.
 */
function msnf_validate_css_class($element, &$form_state) {
  if (!empty($form_state['values']['fields'][$form_state['formatter_settings_edit']]['format_settings']['settings']['instance_settings']['classes']) && !preg_match('!^[A-Za-z0-9-_ ]+$!', $form_state['values']['fields'][$form_state['formatter_settings_edit']]['format_settings']['settings']['instance_settings']['classes'])) {
    form_error($element, t('The css class must include only letters, numbers, underscores and dashes.'));
  }
}

/**
 * Implements hook_field_info_max_weight().
 */
function msnf_field_info_max_weight($entity_type, $bundle, $context) {
  $weights = array();
  foreach (msnf_info_steps($entity_type, $bundle, $context) as $step) {
    $weights[] = $step->weight;
  }
  return $weights ? max($weights) : NULL;
}

/**
 * Menu callback; present a form for removing a form step.
 */
function msnf_delete_form($form, &$form_state, $step) {
  $form['#step'] = $step;
  $admin_path = _field_ui_bundle_admin_path($step->entity_type, $step->bundle) . '/fields';
  $form['#redirect'] = array($admin_path);
  $output = confirm_form($form,
    t('Are you sure you want to delete the step %step?', array('%step' => t($step->label))),
    $admin_path,
    t('This action cannot be undone.'),
    t('Delete'),
    t('Cancel'),
    'confirm'
  );
  return $output;
}

/**
 * Remove step from bundle.
 */
function msnf_delete_form_submit($form, &$form_state) {
  $step = $form['#step'];
  $bundle = $step->bundle;
  $entity_type = $step->entity_type;

  $bundles = field_info_bundles();
  $bundle_label = $bundles[$entity_type][$bundle]['label'];

  ctools_include('export');
  msnf_step_export_delete($step, FALSE);

  drupal_set_message(t('The step %step has been deleted from the %type content type.', array('%step' => t($step->label), '%type' => $bundle_label)));

  // Redirect.
  $form_state['redirect'] = $form['#redirect'];
}
