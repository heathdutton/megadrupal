<?php
/**
 * @file
 * Internal nodes admin page
 */

/**
 * Form builder; creates admin settings.
 *
 */
function internal_nodes_admin_form($form, &$form_state) {
  $form['internal_nodes_outbound_alter'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable outbound URL alter'),
    '#description' => t('Enable the hook_url_outbound_alter() implementation and add enable option to content type edit forms. When enabled for a node, if access to view that node is denied and the action is <em>redirect</em>, the node\'s URL will be rewritten to the redirect URL. <em>Warning: hook_url_outbound_alter() is called many times per page; only enable if the functionality required.</em>'),
    '#default_value' => variable_get('internal_nodes_outbound_alter', 0),
  );

  return system_settings_form($form);
}
