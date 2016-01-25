<?php

/**
 * @file
 * User authenticate API admin pages.
 */

/**
 * Form callback for settings page.
 */
function user_auth_api_settings_page() {
  $form = array();

  $auth = new UserAuthAPI();
  $plugins = $auth->getPluginInfo();
  $auth->sortPluginsByWeight($plugins);

  $header = array(t('plugin name'), t('Class'), t('External'), t('Weight'));
  $rows = array();

  foreach ($plugins as $name => $plugin_info) {
    $row = array(
      '<strong>' . check_plain($name) . '</strong>',
      check_plain($plugin_info['class']),
      $plugin_info['external'] ? t('Yes') : t('No'),
      (int) $plugin_info['weight'],
    );
    $rows[] = $row;
  }

  $table = array(
    'header' => $header,
    'rows' => $rows,
    'empty' => t('No plugins were detected. Please try clearing the caches if a plugin isn\'t registered here yet.'),
  );

  $form['user_auth_api_plugins'] = array(
    '#type' => 'fieldset',
    '#title' => t('Available plugins'),
  );
  $form['user_auth_api_plugins']['table'] = array(
    '#markup' => theme('table', $table),
  );

  $form['disable_fieldset'] = array(
    '#type' => 'fieldset',
  );

  $form['disable_fieldset']['user_auth_api_disable'] = array(
    '#type' => 'checkbox',
    '#title' => t('Disable User authentication validation'),
    '#description' => t('Disable Any User authentication API plugins being called.'),
    '#default_value' => variable_get('user_auth_api_disable', FALSE),
  );

  return system_settings_form($form);
}
