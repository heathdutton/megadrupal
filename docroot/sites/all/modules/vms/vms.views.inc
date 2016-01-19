<?php
/**
 * @file
 * Declares the Views plugin for inheriting argument value.
 */

/**
 * Implements hook_views_plugins().
 */
function vms_views_plugins() {
  return array(
    'argument default' => array(
      'vms_current_mlid' => array(
        'title' => t('Currently active menu item link ID'),
        'handler' => 'vms_plugin_argument_default_mlid',
      ),
    ),
  );
}

/**
 * Implements hook_views_data_alter().
 */
function vms_views_data_alter(&$data) {
  // Get a list of all integer fields on the site – only these should be used
  // for MLID filtering.
  foreach (field_info_fields() as $field) {
    if ($field['type'] == 'number_integer') {
      // Get a quick alias for the field name, for readability.
      $field = &$field['field_name'];

      $data['field_data_' . $field]['vms_' . $field] = array(
        'group' => t('MLID filtering'),
        'title' => t('@field', array('@field' => $field)),
        'field' => $field . '_value',
        'filter' => array(
          'field' => $field . '_value',
          'table' => 'field_data_' . $field,
          'help' => t('A filter treating the field as a MLID value, matching it
            against active menu link IDs.'),
          'handler'  => 'vms_handler_filter_mlid',
        ),
      );
    }
  }
}
