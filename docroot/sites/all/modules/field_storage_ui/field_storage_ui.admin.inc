<?php

/**
 * @file
 * Field Storage admin file.
 */

/**
 * Settings form callback allowing to set the 'field_storage_default' variable.
 */
function field_storage_ui_field_storage_default_form($form, $form_state) {
  $help = t('This form allows you to set the default field storage.') . '<br />';
  $help .= t('Important: this has no effect on already created fields!');
  $form['help'] = array(
    '#markup' => $help,
  );

  $form['field_storage_default'] = _field_storage_ui_storage_options('radios');

  return system_settings_form($form);
}

/**
 * Alter the field_ui_field_overview_form allowing to choose the field storage.
 */
function _field_storage_ui_form_field_ui_field_overview_form_alter(&$form) {
  $field_storages = field_storage_ui_get_field_storage_types();
  // Add field storage options only when we have at least 2 storage options.
  if (count($field_storages) < 2) {
    return;
  }
  // Make some room for the storage element.
  $form['fields']['_add_new_field']['widget_type']['#cell_attributes']['colspan'] = 2;
  // Add the storage select input.
  $form['fields']['_add_new_field']['storage'] = _field_storage_ui_storage_options('select', $field_storages);

  // Add a pre-submit function, called before the field is created.
  array_unshift($form['#submit'], 'field_storage_ui_form_field_ui_field_overview_form_presubmit');

  // Add a post-submit function, called after field UI creates the field.
  $form['#submit'][] = 'field_storage_ui_form_field_ui_field_overview_form_postsubmit';
}

/**
 * Return a form element for field_storage_default variable.
 *
 * @param string $type
 *   The form #type element.
 * @param array $field_storages
 *   Optional array list of field storages.
 *
 * @return array
 *   The array form element.
 */
function _field_storage_ui_storage_options($type, $field_storages = NULL) {
  $element = array(
    '#type' => $type,
    '#multiple' => FALSE,
    '#title' => t('Storage'),
    '#required' => TRUE,
    '#options' => array(),
    '#default_value' => variable_get('field_storage_default', 'field_sql_storage'),
  );

  if (!$field_storages) {
    $field_storages = field_storage_ui_get_field_storage_types();
  }
  foreach ($field_storages as $name => $info) {
    $element['#options'][$name] = $info['label'];
  }

  return $element;
}

/**
 * Custom submit function before field_ui_field_overview_form_submit() executes.
 *
 * @see field_ui_field_overview_form_submit()
 * @see field_storage_ui_form_field_ui_field_overview_form_postsubmit()
 */
function field_storage_ui_form_field_ui_field_overview_form_presubmit($form, &$form_state) {
  // Check if a new field has to be saved.
  if (!empty($form_state['values']['fields']['_add_new_field']['field_name'])) {

    $selected_field_storage = $form_state['values']['fields']['_add_new_field']['storage'];
    $default_field_storage = variable_get('field_storage_default', 'field_sql_storage');
    $field_storage_types = field_storage_ui_get_field_storage_types();

    // Check if user selected a storage other then the default field storage and
    // make sure that other form alters did not altered the storage adding an
    // invalid storage type.
    if ($default_field_storage != $selected_field_storage && isset($field_storage_types[$selected_field_storage])) {

      // The field_create_field() uses the 'field_storage_default' variable when
      // creating a new field. Set this variable to selected user storage.
      // @see field_ui_field_overview_form_submit()
      // @see field_create_field()
      variable_set('field_storage_default', $selected_field_storage);

      // Save the original field_storage_default value so it can be reverted in
      // the post-submit function.
      $form_state['values']['field_storage_default'] = $default_field_storage;
    }
  }
}

/**
 * Custom submit function after field_ui_field_overview_form_submit() executes.
 *
 * @see field_ui_field_overview_form_submit()
 * @see field_storage_ui_form_field_ui_field_overview_form_presubmit()
 */
function field_storage_ui_form_field_ui_field_overview_form_postsubmit($form, $form_state) {
  if (isset($form_state['values']['field_storage_default'])) {
    variable_set('field_storage_default', $form_state['values']['field_storage_default']);
  }
}

/**
 * Returns a list of field storage types.
 *
 * @return array
 *   A list of field storage types.
 *
 * @see _field_info_collate_types()
 */
function field_storage_ui_get_field_storage_types() {
  $info = array();
  // Populate storage types.
  foreach (module_implements('field_storage_info') as $module) {
    $storage_types = (array) module_invoke($module, 'field_storage_info');
    foreach ($storage_types as $name => $storage_info) {
      // Provide defaults.
      $storage_info += array(
        'settings' => array(),
      );
      $info[$name] = $storage_info;
      $info[$name]['module'] = $module;
    }
  }

  return $info;
}

/**
 * Lists all defined fields including storage information.
 */
function _field_storage_ui_fields_list() {
  $instances = field_info_instances();
  $field_types = field_info_field_types();
  $bundles = field_info_bundles();
  $field_storages = field_storage_ui_get_field_storage_types();

  $modules = system_rebuild_module_data();

  $header = array(
    t('Field name'),
    t('Field type'),
    t('Used in'),
    t('Field storage'),
  );
  $rows = array();
  foreach ($instances as $entity_type => $type_bundles) {
    foreach ($type_bundles as $bundle => $bundle_instances) {
      foreach ($bundle_instances as $field_name => $instance) {
        $field = field_info_field($field_name);

        // Initialize the row if we encounter the field for the first time.
        if (!isset($rows[$field_name])) {
          $rows[$field_name]['class'] = $field['locked'] ? array('menu-disabled') : array('');
          $rows[$field_name]['data'][0] = $field['locked'] ? t('@field_name (Locked)', array('@field_name' => $field_name)) : $field_name;
          $module_name = $field_types[$field['type']]['module'];
          $rows[$field_name]['data'][1] = $field_types[$field['type']]['label'] . ' ' . t('(module: !module)', array('!module' => $modules[$module_name]->info['name']));
        }

        // Add the current instance.
        $admin_path = _field_ui_bundle_admin_path($entity_type, $bundle);
        $rows[$field_name]['data'][2][] = $admin_path ? l($bundles[$entity_type][$bundle]['label'], $admin_path . '/fields') : $bundles[$entity_type][$bundle]['label'];

        // Add the field storage.
        if (isset($field_storages[$field['storage']['type']])) {
          $args = array(
            '@label' => $field_storages[$field['storage']['type']]['label'],
            '@storage' => $field['storage']['type'],
          );
          $rows[$field_name]['data'][3] = format_string('@label (@storage)', $args);
        }
        else {
          $rows[$field_name]['data'][3] = $field['storage']['type'];
        }
      }
    }
  }
  foreach ($rows as $field_name => $cell) {
    $rows[$field_name]['data'][2] = implode(', ', $cell['data'][2]);
  }
  if (empty($rows)) {
    $output = t('No fields have been defined yet.');
  }
  else {
    // Sort rows by field name.
    ksort($rows);
    $output = theme('table', array('header' => $header, 'rows' => $rows));
  }

  return $output;
}
