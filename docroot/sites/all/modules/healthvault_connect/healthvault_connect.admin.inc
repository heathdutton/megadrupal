<?php

/**
 * @file
 * Adminstration forms for HealthVault® Connect.
 */

/**
 * General configuration form for controlling the behaviour.
 */
function healthvault_connect_admin_settings($form, &$form_state) {
  $form = variable_module_form($form, $form_state, 'healthvault_connect');

  if (drupal_multilingual()) {
    $form['healthvault_connect_info'] = array(
      '#type' => 'item',
      '#markup' => module_exists('i18n_variable') ?
        t('You can turn on localization for some of these settings using multilingual variables. Go to !link and open the %group group.',
          array(
            '!link' => l(t('multilingual variable settings'), 'admin/config/regional/i18n/variable'),
            '%group' => t('HealthVault® Connect'),
          )) :
        t('You can turn on localization for some of these settings using !link.',
          array('!link' => l(t('Variable translation'), 'http://drupal.org/node/1113374'))),
    );
  }

  return $form;
}

/**
 * Extended validation of healthvault_connect_admin_settings().
 * If the form contains no other errors a first anonymous connection to
 * HealthVault® will be established to validate the credentials.
 */
function healthvault_connect_admin_settings_validate($form, &$form_state) {
  $errors = form_get_errors();
  if (empty($errors)) {
    // check HealthVault® connection if all form values are set

    // Fake an authenticated user
    $session['healthVault']['userAuthToken'] = 'dummy';

    composer_manager_register_autoloader();

    try {
      $logger = new Psr3Watchdog();
      $logger->setType('healthvault_connect');

      $hv = new \biologis\HV\HVClient(
        $form_state['values']['healthvault_connect_app_id'],
        $session
      );

      $hv->setLogger($logger);

      $hv->connect(
        $form_state['values']['healthvault_connect_thumb_print'],
        $form_state['values']['healthvault_connect_private_key']
      );

      drupal_set_message(t('HealthVault® connection established!'));
    }
    catch (Exception $e) {
      form_set_error('', t('HealthVault® connection could not be established using these settings. Error message: %msg', array('%msg' => $e->getMessage())));
    }
  }
}
