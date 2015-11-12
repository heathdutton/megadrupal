<?php
/**
 * @file
 * Administrative functions.
 */

/**
 * Builds a settings form for Varnish.
 */
function hosting_varnish_settings_form() {
  $form = array();

  if (hosting_feature('server')) {
    $web_servers = hosting_get_servers('http');

    $form['hosting_varnish_web_servers'] = array(
      '#type' => 'checkboxes',
      '#title' => t('Cached web servers'),
      '#options' => $web_servers,
      '#default_value' => variable_get('hosting_varnish_web_servers', array()),
      '#description' => t('Specify which servers are served by Varnish. If none is selected, all are assumed to be.'),
    );
  }
  $form['varnish_status'] = array(
    '#type' => 'item',
    '#title' => t('Status'),
    '#value' => theme('varnish_status', varnish_get_status(), floatval(variable_get('varnish_version', 2.1))),
    '#description' => t('Configure !settings_link.', array('!settings_link' => l(t('Varnish settings'), 'admin/settings/varnish'))),
  );

  return system_settings_form($form);
}
