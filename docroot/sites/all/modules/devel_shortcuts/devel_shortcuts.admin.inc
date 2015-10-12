<?php

/**
 * @file
 * This module holds the administratve pages for the Devel Shortcuts module.
 */


/**
 * Administrative settings to enable or disable the tabs.
 */
function devel_shortcuts_admin_settings() {

  $form['devel_shortcuts_edit_entity_type'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show a tab on the entity to quickly access editing the entity type.'),
    '#default_value' => variable_get('devel_shortcuts_edit_entity_type', FALSE),
    '#description' => t('Display a tab above all entities that when clicked, takes the user to the editing page for that entity.'),
  );

  return system_settings_form($form);
}
