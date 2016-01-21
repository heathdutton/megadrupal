<?php

/**
 * @file
 * Gearman admin settings form, etc.
 */

/**
 *  Settings form callback.
 */
function gearman_settings() {
  $form['gearman_server'] = array(
    '#type' => 'textfield',
    '#title' => t('Gearman Server(s)'),
    '#default_value' => variable_get('gearman_server', '127.0.0.1:4730'),
    '#description' => t('The server(s) you want to connect to. Separate multiple server:port values with commas.'),
    '#required' => TRUE,
  );
  $form['#validate'][] = 'gearman_settings_validate';

  return system_settings_form($form);

}

/**
 * Settings validation. Be sure we can connect!
 */
function gearman_settings_validate($form, $form_state) {
  // Create our worker object.
  $gmworker= new GearmanWorker();
  
  // Connect to our server(s)
  $servers = variable_get('gearman_server', 'localhost:4730');
  $gmworker->addServers('localhost:4730');
  
  $result = $gmworker->echo('test');
 
  if ($result) {
    drupal_set_message(t('The Gearman server is responding properly.'));
  }
  else {
    form_set_error('gearman_server', t('The Gearman server could no be reached!'));
  }
}