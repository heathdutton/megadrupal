<?php

/**
 * @file
 * Administrative pages for PHP Console module.
 */

/**
 * PHP Console settings form.
 */
function pc_settings_form($form, &$form_state) {

  $ip = ip_address();

  if (!file_exists(PC_LIBRARY_PATH . '/__autoload.php')) {
    drupal_set_message(pc_requirements_message(), 'warning', FALSE);
  }

  if (!$form_state['input']) {
    if (!pc_check_ip($ip)) {
      drupal_set_message(t('Your current IP address %ip is not allowed to access to PHP Console.', array('%ip' => $ip)), 'warning', FALSE);
    }
    elseif (pc_init_pc()) {
      $connector = PhpConsole\Connector::getInstance();
      if (!$connector->isActiveClient()) {
        $extension_link = l(
          t('PHP Console extension'),
          'https://chrome.google.com/webstore/detail/php-console/nfhmhhlpfleoednkpnnnkolmclajemef',
          array('attributes' => array('target' => 'blank'))
        );
        drupal_set_message(t('You need !extension_link to be installed on Google Chrome.', array('!extension_link' => $extension_link)), 'warning', FALSE);
      }
    }
  }

  $form['pc_password_enable'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable password protection'),
    '#description' => t('Remote PHP code execution allowed only in password protected mode.'),
    '#default_value' => variable_get('pc_password_enable'),
  );

  // password_confirm doesn't support form #states.
  // So we make a wrapper around the field.
  // See https://drupal.org/node/1427838.
  $form['pass_wrapper'] = array(
    '#type' => 'container',
    '#states' => array(
      'visible' => array(
        ':input[name="pc_password_enable"]' => array('checked' => TRUE),
      ),
    ),
  );

  $form['pass_wrapper']['pc_password'] = array(
    '#type' => 'password_confirm',
    '#size' => 25,
    '#description' => t('Provide a password for client authorization.'),
  );

  $form['pc_remote_php_execution'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable remote PHP code execution'),
    '#default_value' => variable_get('pc_remote_php_execution'),
    '#description' => t('Note that it is a dangerous security risk in the hands of a malicious or inexperienced user.'),
    '#states' => array(
      'visible' => array(
        ':input[name="pc_password_enable"]' => array('checked' => TRUE),
      ),
    ),
  );

  $form['pc_ips'] = array(
    '#type' => 'textarea',
    '#title' => 'Allowed IP masks',
    '#description' => t('Enter one value per line. Leave empty to disable IP verification.'),
    '#default_value' => variable_get('pc_ips'),
  );
  $form['pc_ips']['#description'] .= ' ' . t('Your IP address is: %ip.', array('%ip' => $ip));

  $form['pc_track_errors'] = array(
    '#type' => 'checkbox',
    '#title' => t('Handle PHP errors'),
    '#default_value' => variable_get('pc_track_errors'),
    '#description' => t("This option doesn't cancel default Drupal error handler."),
  );

  $options = array(
    'server' => t('Server'),
    'session' => t('Session'),
    'cookie' => t('Cookie'),
    'post' => t('Post'),
    'get' => t('Get'),
    'loaded_entities' => t('Loaded entities'),
    'logged_user' => t('Logged user'),
    'menu_item' => t('Current menu item'),
    'variables' => t('Variables'),
    'forms' => t('Forms'),
    'memory_usage' => t('Memory usage'),
    'peak_memory_usage' => t('Peak memory usage'),
    'execution_time' => t('Page execution time'),
    'watchdog' => t('Watchdog messages'),
    'emails' => t('Outgoing emails'),
  );
  $form['pc_debugging_info'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Debugging information'),
    '#options' => $options,
    '#default_value' => variable_get('pc_debugging_info', array()),
    '#description' => t('These data will be sent to browser console on each page request.'),
  );

  $form['#submit'][] = 'pc_settings_form_submit';

  return system_settings_form($form);
}

/**
 * Submit handler for pc_settings_form.
 */
function pc_settings_form_submit($form, &$form_state) {
  if ($form_state['values']['pc_password_enable'] && $form_state['values']['pc_password']) {
    variable_set('pc_password', $form_state['values']['pc_password']);
  }
  unset($form_state['values']['pc_password']);
}
