<?php

/**
 * @file
 * Admin settings form for the XML-RPC server we want to test.
 */

/**
 * Form definition for configuring the module.
 *
 * Allow the user to set the remote server address.
 */
function xmlrpctester_settings() {
  $form['xmlrpc_server_address'] = array(
    '#title' => t('Server address'),
    '#type' => 'textfield',
    '#height' => 1,
    '#default_value' => _xmlrpctester_server(),
    '#description' => t('The fully qualified address of the XML-RPC server. Click reset to defaults to switch back to this site.'),
  );
  return system_settings_form($form);
}
