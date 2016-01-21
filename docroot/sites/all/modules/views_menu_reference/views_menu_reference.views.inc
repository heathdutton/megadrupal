<?php

/**
 * @file
 * Declares the Views plugin to add filter and argument handlers for
 * field_views_menu_reference - fields to enable processing in views.
 */

/**
 * Implements hook_views_plugins().
 */
function views_menu_reference_views_plugins() {
  return array(
    // Add the current path as default argument in views.
    'argument default' => array(
      'views_menu_reference_plugin_argument_current_path' => array(
        'title' => t('Views Menu Reference Path Matching'),
        'handler' => 'views_menu_reference_plugin_argument_current_path',
      ),
    ),
  );
}

/**
 * Implements hook_views_data_alter().
 */
function views_menu_reference_views_data_alter(&$data) {
  foreach (field_info_fields() as $field) {
    // Only handle field_views_menu_reference fields.
    if ($field['type'] == 'field_views_menu_reference') {
      // Get a quick alias for the field name, for readability.
      $field = &$field['field_name'];

      // Add the mlid field to views.
      $data['field_data_' . $field][$field . '_mlid'] = array(
        'group' => t('Views Menu Reference'),
        'title' => t('@field', array('@field' => $field)),
        'field' => $field . '_mlid',
        // Add the filter handler for fields of this type.
        'filter' => array(
          'field' => $field . '_mlid',
          'table' => 'field_data_' . $field,
          'help' => t('A filter matching the path and rules from views_menu_reference_fields against an URL.'),
          'handler' => 'views_menu_reference_handler_filter_path',
        ),
        // Add the argument handler for fields of this type.
        'argument' => array(
          'handler' => 'views_menu_reference_handler_argument_current_path',
          'table' => 'field_data_' . $field,
          'help' => t('An argument handler matching the path and rules from views_menu_reference_fields against an URL.'),
        ),
      );
    }
  }
}
