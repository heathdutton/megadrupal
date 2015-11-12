<?php

/**
 * @file
 * Contains administrative forms for the current module.
 */

/**
 * FAPI definition for the move components from defaults hooks to database form.
 */
function unlink_defaults_admin_form($form) {

  // Get list of table schemas which support export.
  ctools_include('export');
  $schemas = ctools_export_get_schemas(TRUE);

  // Prepare array of all configurations that implements ctools exportable.
  $components = array();
  foreach ($schemas as $table => $schema) {

    // Get list of all configs with human readable names.
    if (!empty($schema['export']['list callback']) && function_exists($schema['export']['list callback'])) {
      $names = $schema['export']['list callback']();
    }
    else {
      $names = ctools_export_default_list($table, $schema);
    }
    $configs = ctools_export_crud_load_all($table);
    foreach ($configs as $config_name => $config) {
      if (!empty($names[$config_name])) {
        $components[$table][$config_name] = array(
          'title' => $names[$config_name],
          'module' => !empty($config->export_module) ? $config->export_module : t('None'),
          'storage' => $config->export_type == EXPORT_IN_CODE ? t('In code') : t('In database'),
        );
      }
    }

    if (!empty($components[$table])) {
      ksort($components[$table]);
    }
  }

  // Add custom components which defined by hook_unlink_defaults_components(),
  // which do not use ctools export or which use entity export.
  // @see unlink_defaults_unlink_defaults_components().
  $custom_components = module_invoke_all('unlink_defaults_components');
  foreach ($custom_components as $name => $custom_component) {
    $components[$name] = $custom_component['list'];
    $schemas[$name]['module'] = $custom_component['module'];
  }

  $form['settings'] = array(
    '#type' => 'vertical_tabs',
  );

  $options = array();
  $tables = array();
  foreach ($components as $table => $configs) {
    if (empty($configs)) {
      continue;
    }

    foreach ($configs as $config_name => $config) {
      $options[$table][$config_name] = array();
      $options[$table][$config_name][] = $config['title'];
      $options[$table][$config_name][] = $config['storage'];
      $options[$table][$config_name][] = $config['module'];
      $options[$table][$config_name]['#attributes'] = array('class' => array('bulk-selection'));
    }

    $module = $schemas[$table]['module'];
    $header = array($table, t('Storage'), t('Module'));

    $files = system_rebuild_module_data();
    $module_name = $files[$module]->info['name'];

    $tables[] = $table;

    if (!isset($form[$module_name])) {
      $form[$module_name] = array(
        '#type' => 'fieldset',
        '#group' => 'settings',
        '#title' => $module_name,
      );
    }

    $values = variable_get('unlink_defaults', array());
    $form[$module_name]['tables'][$table] = array(
      '#prefix' => '<div class="export-container">',
      '#suffix' => '</div>',
      '#type' => 'tableselect',
      '#header' => $header,
      '#default_value' => !empty($values[$table]) ? drupal_map_assoc($values[$table]) : array(),
      '#options' => $options[$table],
    );
  }

  $form['tables'] = array(
    '#type' => 'value',
    '#value' => $tables,
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Move selected configs to the database'),
  );

  $form['#attached']['js'][] = ctools_attach_js('admin', 'unlink_defaults');
  return $form;
}

/**
 * Process unlink_defaults_form submit form.
 */
function unlink_defaults_admin_form_submit($form, &$form_state) {

  $tables = $form_state['values']['tables'];
  $configs_to_unlink = array();
  foreach ($tables as $table) {
    $names = array_keys(array_filter($form_state['values'][$table]));
    if (!empty($names)) {
      natcasesort($names);
      $configs_to_unlink[$table] = $names;
    }
  }

  // Save list of configs that should be moved to the database.
  // It is important that save of this variable happens before execution of
  // ctools_export_crud_load_all() below.
  variable_set('unlink_defaults', $configs_to_unlink);

  // Flush ctools export cache.
  ctools_include('export');
  $schemas = ctools_export_get_schemas(TRUE);
  foreach ($schemas as $table => $schema) {
    cache_clear_all('ctools_export_index:' . $table, $schema['export']['default cache bin']);

    // Invoke configuration reload, because otherwise the first form load will
    // miss recently checked configs due to complicated ctools API.
    ctools_export_crud_load_all($table, TRUE);
  }

  // Rebuild the default entities provided in code, if module entity exists.
  if (module_exists('entity')) {
    entity_defaults_rebuild();
  }

  // Show message to a user.
  if (!empty($configs_to_unlink)) {
    drupal_set_message(t('All checked configurations has been moved to the database.'));
  }
  else {
    drupal_set_message(t('No configurations selected.'), 'warning');
  }
}
