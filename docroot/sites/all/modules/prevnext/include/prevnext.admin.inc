<?php
/**
 * @file
 * prevnext.admin.inc
 */

/**
 * Previous/Next Settings Form Callback
 */
function prevnext_settings($form, &$form_state) {
  $form['prevnext_enabled_nodetypes'] = array(
    '#title' => t('Enabled Node Types'),
    '#description' => t('Check node types enabled for Previous/Next'),
    '#type' => 'checkboxes',
    '#options' => _prevnext_types_as_options(),
    '#default_value' => variable_get('prevnext_enabled_nodetypes', array())
  );

  $form['#submit'][] = 'prevnext_settings_submit';

  return system_settings_form($form);
}

/**
 * Previous/Next form settings submit
 * Needs to clear cache :)
 *
 * @see prevnext_settings
 */
function prevnext_settings_submit(&$form, $form_state) {
  drupal_flush_all_caches();
}

/**
 * Helper: Retrieve all node types and create an option list with them.
 */
function _prevnext_types_as_options() {
  $node_types = node_type_get_types();

  $options = array();
  foreach ($node_types as $type => $info) {
    $options[$type] = $info->name;
  }

  asort($options);

  return $options;

}