<?php

/**
 * @file
 * Administration callbacks for Revisions RSS.
 */

/**
 * Menu callback; displays the Revisions RSS admin settings page.
 */
function revisions_rss_admin_settings() {
  $types = node_type_get_types();

  foreach($types as $type) {
    $options[$type->type] = $type->name;
  }
  
  $form['revisions_rss_nodetypes'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Generate RSS for these node types'),
    '#options' => $options,
    '#default_value' => variable_get('revisions_rss_nodetypes', array('page')));
  $form['array_filter'] = array('#type' => 'hidden');

  return system_settings_form($form);
}