<?php
/**
 * @file
 * Implements the display extender to add additional setting to views displays.
 */

/**
 * Implements hook_views_plugins().
 */
function views_custom_conditions_views_plugins() {
  $path = drupal_get_path('module', 'views_custom_conditions');
  $plugins = array();
  $plugins['display_extender']['views_custom_conditions'] = array(
    'title' => t('Extender textarea'),
    'help' => t('Add textarea.'),
    'path' => $path,
    'handler' => 'views_custom_conditions_plugin_display_extender_code',
  );
  return $plugins;
}

/**
 * Implements hook_views_query_alter().
 */
function views_custom_conditions_views_query_alter(&$view, &$query) {
 // p($view);
  if (variable_get('views_custom_conditions_' . $view->current_display .'_'. $view->name) || variable_get('views_custom_conditions_default')) {
    // Get View name 
    $name = variable_get('views_custom_conditions_' . $view->current_display .'_'. $view->name);
    if($name) {
    $all_fields =   explode("\n", $name);
    } else {
      $name = variable_get('views_custom_conditions_default');
    }
    
    //Count total no of apply fields in condition.
    $no_of_fields = count($all_fields);
    for ($i = 0; $i < $no_of_fields; $i++) {
      $field = $all_fields[$i];
      // Get the value of table, field name, Value, Operator and Group Operator.
      $field_info =  explode("|", $field);
      $table_name = $field_info[0];
      $field_name = $field_info[1];
      $field_value = $field_info[2];
      $operator = $field_info[3];
      $view->query->add_where(0, $table_name . '.' . $field_name, array($field_value), $operator);
    }
  }
  return $query;
}
