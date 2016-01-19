<?php
/**
 * @file
 * Exports that help Drupal developers when working with fields.
 */

/**
 * Export individual field and field instance configurations.
 */
function fieldhelp_field_export_list() {
  $instances = field_info_instances();
  $field_types = field_info_field_types();
  $bundles = field_info_bundles();

  $modules = system_rebuild_module_data();

  $header = array(t('Field'), t('Field type'), t('Instances'));
  $rows = array();
  foreach ($instances as $entity_type => $type_bundles) {
    foreach ($type_bundles as $bundle => $bundle_instances) {
      foreach ($bundle_instances as $field_name => $instance) {
        $field = field_info_field($field_name);

        // Initialize the row if we encounter the field for the first time.
        if (!isset($rows[$field_name])) {
          $rows[$field_name]['data'][0] = l($field_name, 'admin/reports/fields/export/field/' . $field_name);
          $module_name = $field_types[$field['type']]['module'];
          $rows[$field_name]['data'][1] = $field_types[$field['type']]['label'] . ' ' . t('(module: !module)', array('!module' => $modules[$module_name]->info['name']));
        }

        // Add the current instance.
        $rows[$field_name]['data'][2][] = l($bundles[$entity_type][$bundle]['label'], 'admin/reports/fields/export/instance/' . $field_name . '/' . $entity_type . '/' . $bundle);
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

/**
 * Page callback for exporting a given field instance.
 */
function fieldhelp_field_export_instance_page($field_name, $entity_type, $bundle) {
  $build = array();
  require_once drupal_get_path('module', 'fieldhelp') . '/fieldhelp.common.inc';
  $instance = fieldhelp_field_export_instance($entity_type, $field_name, $bundle);
  include_once DRUPAL_ROOT . '/includes/utility.inc';
  $build['data'] = array(
    '#prefix' => '<pre>',
    '#markup' => drupal_var_export($instance),
    '#suffix' => '</pre>',
  );
  $build['instructions'] = array(
    '#prefix' => '<p>',
    '#markup' => t('The below command can be used in an install profile or update hook to create a field instance of an existing or <a href="@url">created field</a>.  To place this field on a different instance, simply change the value of the "bundle" key.', array('@url' => url('admin/reports/fields/export/field/' . $field_name))),
    '#suffix' => '</p>',
  );
  $build['command'] = array(
    '#prefix' => '<textarea cols="100" rows="12" class="form-textarea">',
    '#markup' => '$instance = ' . drupal_var_export($instance) . ';' . "\n" . 'field_create_instance($instance);',
    '#suffix' => '</textarea>',
  );
  return $build;
}

/**
 * Page callback for exporting a given field definition.
 */
function fieldhelp_field_export_field_page($field_name) {
  $build = array();
  require_once drupal_get_path('module', 'fieldhelp') . '/fieldhelp.common.inc';
  $field = fieldhelp_field_export_field($field_name);
  include_once DRUPAL_ROOT . '/includes/utility.inc';
  $build['data'] = array(
    '#prefix' => '<pre>',
    '#markup' => drupal_var_export($field),
    '#suffix' => '</pre>',
  );
  $build['instructions'] = array(
    '#prefix' => '<p>',
    '#markup' => t('The below command can be used in an install profile or update hook to create a field %field_name.', array('%field_name' => $field_name)),
    '#suffix' => '</p>',
  );
  $build['command'] = array(
    '#prefix' => '<textarea cols="100" rows="12" class="form-textarea">',
    '#markup' => '$field = ' . drupal_var_export($field) . ';' . "\n" . 'field_create_field($field);',
    '#suffix' => '</textarea>',
  );
  return $build;
}
