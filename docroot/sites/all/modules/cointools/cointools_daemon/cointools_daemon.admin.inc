<?php
/**
 * @file
 * Admin form to change Coin Tools Daemon settings.
 */

use Graze\GuzzleHttp\JsonRpc\Client as JsonRpcClient;
use GuzzleHttp\Url;
use Drupal\cointools_daemon\Client as DaemonClient;

/**
 * Generates admin form.
 */
function cointools_daemon_admin_form(array $form, array &$form_state) {
  $form['scheme'] = array(
    '#type' => 'select',
    '#title' => t("Scheme"),
    '#options' => array(
      'http' => 'HTTP',
      'https' => 'HTTPS',
    ),
    '#default_value' => variable_get('cointools_daemon.scheme'),
  );
  $form['username'] = array(
    '#type' => 'textfield',
    '#title' => t("Username"),
    '#default_value' => variable_get('cointools_daemon.username'),
    '#required' => TRUE,
  );
  $form['password'] = array(
    '#type' => 'password',
    '#title' => t("Password"),
  );
  $form['hostname'] = array(
    '#type' => 'textfield',
    '#title' => t("Hostname"),
    '#default_value' => variable_get('cointools_daemon.hostname'),
  );
  $form['port'] = array(
    '#type' => 'numberfield',
    '#min' => 1,
    '#max' => 65535,
    '#step' => 1,
    '#title' => t("Port"),
    '#default_value' => variable_get('cointools_daemon.port'),
  );
  $form = system_settings_form($form);
  // Make sure our submit handler is invoked.
  $form['#submit'][] = 'cointools_daemon_admin_form_submit';
  return $form;
}

/**
 * Validate handler for admin form.
 */
function cointools_daemon_admin_form_validate(array $form, array &$form_state) {
  $values = $form_state['values'];
  $uri = new Url($values['scheme'], $values['hostname'], $values['username'], $values['password'], $values['port']);

  try {
    $client = new DaemonClient(JsonRpcClient::factory($uri));
    $info = $client->request('getinfo');
  }
  catch (\Exception $exception) {
    form_set_error('', t("Unable to connect."));
  }
}

/**
 * Extra submit handler for admin form.
 */
function cointools_daemon_admin_form_submit(array $form, array &$form_state) {
  $settings = array('scheme', 'username', 'password', 'hostname', 'port');
  foreach ($settings as $setting) {
    variable_set('cointools_daemon.' . $setting, $form_state['values'][$setting]);
  }
}
