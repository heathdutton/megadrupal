<?php

/**
 * Implements hook_views_data().
 */
function views_schema_views_data() {
  $data = array();
  $schema = drupal_get_schema();
  $modules = variable_get('views_schema_enabled_modules', array());

  foreach ($schema as $table) {
    if (empty($modules[$table['module']]) || $table['module'] !== $modules[$table['module']]) {
      continue;
    }

    views_schema_create_definition($data, $table);
  }

  return $data;
}
