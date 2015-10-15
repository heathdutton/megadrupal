<?php
/**
 * @file
 * Provides administrative functions.
 *
 * Copyright 2008-2009 by Jimmy Berry ("boombatower", http://drupal.org/user/214218)
 * Portions copyright 2012-2013 by John Franklin (https://drupal.org/user/683430)
 */

/**
 * Settings form.
 */
function click_heatmap_admin_settings_form($form, &$form_state) {
  $form = array();

  $form['general'] = array(
    '#type' => 'fieldset',
    '#title' => t('General'),
  );
  $form['general']['click_heatmap_scope'] = array(
    '#type' => 'radios',
    '#title' => t('Scope'),
    '#description' => t('The pages that Click heatmaps will be generated for.'),
    '#default_value' => variable_get('click_heatmap_scope', 'home'),
    '#options' => array(
      'all' => t('All pages'),
      'user' => t('Non-admin pages'),
      'admin' => t('Admin pages'),
      'home' => t('Home page'),
    ),
  );

  $form['login'] = array(
    '#type' => 'fieldset',
    '#title' => t('Login'),
    '#description' => t('Login information used for ClickHeat library administration interface.'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['login']['click_heatmap_admin_username'] = array(
    '#type' => 'textfield',
    '#title' => t('Administrator Username'),
    '#default_value' => variable_get('click_heatmap_admin_username', ''),
  );
  $form['login']['click_heatmap_admin_password'] = array(
    '#type' => 'password',
    '#title' => t('Administrator Password'),
  );
  $form['login']['click_heatmap_change_login'] = array(
    '#type' => 'checkbox',
    '#title' => t('Change Login Information'),
    '#description' => t('Check this to override the current login information with the entered values.'),
  );

  $form = system_settings_form($form);
  $form['#submit'][] = 'click_heatmap_admin_settings_form_submit';
  
  return $form;
}

/**
 * Validate settings form.
 */
function click_heatmap_admin_settings_form_validate($form, &$form_state) {
  if ($form_state['values']['click_heatmap_change_login']) {
    // Validate administrator username and password.
    if (!$form_state['values']['click_heatmap_admin_username']) {
      form_set_error('click_heatmap_change_username', t('Administrator username may not be blank.'));
    }
    if (!$form_state['values']['click_heatmap_admin_password']) {
      form_set_error('click_heatmap_change_password', t('Administrator password may not be blank.'));
    }
  }
}

/**
 * Generate ClickHeat configuration if administrative login changed.
 */
function click_heatmap_admin_settings_form_submit($form, &$form_state) {
  if ($form_state['values']['click_heatmap_change_login']) {
    // Login information changes, generate config file.
    click_heatmap_admin_config_generate($form_state['values']['click_heatmap_admin_username'],
                                        $form_state['values']['click_heatmap_admin_password']);
  }
}

/**
 * Generate ClickHeat configuration.
 *
 * @param string $username (Optional) Administrative username.
 * @param string $password (Optional) Administrative password.
 */
function click_heatmap_admin_config_generate($username = '', $password = '') {
  $realpath = realpath(drupal_get_path('module', 'click_heatmap') . '/clickheat/');

  // Default configuration settings.
  $config = array(
    'logPath' => $realpath . '/logs/',
    'cachePath' => $realpath . '/cache/',
    'referers' => FALSE,
    'groups' => FALSE,
    'filesize' => 0,
    'adminLogin' => $username,
    'adminPass' => md5($password),
    'viewerLogin' => '',
    'viewerPass' => '',
    'memory' => (int) str_replace('M', '', ini_get('memory_limit')),
    'step' => 5,
    'dot' => 19,
    'flush' => 40,
    'start' => 's',
    'palette' => FALSE,
    'heatmap' => TRUE,
    'hideIframes' => TRUE,
    'hideFlashes' => TRUE,
    'yesterday' => TRUE,
    'alpha' => 80,
    'version' => click_heatmap_library_version(),
  );
  file_put_contents($realpath . '/config/config.php', '<?php $clickheatConf = ' . var_export($config, TRUE) . ' ?>');

  drupal_set_message(t('ClickHeat configured.'));
}
