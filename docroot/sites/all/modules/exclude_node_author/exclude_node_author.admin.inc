<?php
/**
 * @file
 * Exclude Node author Administrative Interface
 */

/**
 * Admin configuration form
 */
function exclude_node_author_admin_settings() {
  $form['exclude_node_author_administrator'] = array(
    '#type' => 'checkbox',
    '#title' => t('Do not hide author from users with "Administer exclude node author" permission.'),
    '#description' => t('Allows administrators to always see author information.'),
    '#default_value' => variable_get('exclude_node_author_administrator', 0),
    '#disabled' => FALSE,
  );

  $form['keywords'] = array(
    '#title' => t('Nodes with author excluded'),
    '#type' => 'textarea',
    '#description' => t('list of node IDs that will not have the author information shown.'),
    '#default_value' => implode(",",variable_get('exclude_node_author_nid_list', array())),
    '#disabled' => TRUE,
  );
 
  return system_settings_form($form);
}