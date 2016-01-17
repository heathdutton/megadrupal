<?php

/**
 * @file
 * Form callbacks and alter hooks for the Supertext UI form.
 */

/**
 * Add-more submit callback for the per-user accounts.
 */
function tmgmt_supertext_controller_add_more_submit($form, &$form_state) {
  $form_state['values']['settings']['accounts'][] = array(
    'drupal_username' => '',
    'account' => array(
      'api_username' => '',
      'api_token' => '',
    ),
  );
  $form_state['rebuild'] = TRUE;
}

/**
 * Add-more callback for the per-user accounts.
 */
function tmgmt_supertext_controller_add_more_callback($form, $form_state) {
  return $form['plugin_wrapper']['settings']['accounts'];
}

/**
 * Implements hook_form_FORM_ID_alter().
 */
function tmgmt_supertext_form_tmgmt_translator_form_alter(&$form, &$form_state) {
  /** @var TMGMTTranslator $translator */
  $translator = $form_state['tmgmt_translator'];
  if ($translator->plugin == 'supertext') {
    $form['#validate'][] = '_tmgmt_supertext_translator_form_validate';
    // Add our submit callback before the default one.
    array_unshift($form['#submit'], '_tmgmt_supertext_translator_form_submit');
  }
}

/**
 * Validate callback for the Supertext UI form.
 */
function _tmgmt_supertext_translator_form_validate(&$form, &$form_state) {
  /** @var TMGMTTranslator $translator */
  $translator = $form_state['tmgmt_translator'];
  /** @var TMGMTSupertextPluginController $controller */
  $controller = $translator->getController();
  $plugin_settings_form =& $form['plugin_wrapper']['settings'];

  // Trim and validate per-user accounts.
  foreach ($form_state['values']['settings']['accounts'] as $key => &$data) {
    if (!is_int($key)) {
      continue;
    }
    $data['drupal_username'] = trim($data['drupal_username']);
    $data['account']['api_username'] = trim($data['account']['api_username']);
    $data['account']['api_token'] = trim($data['account']['api_token']);
    if ($data['drupal_username'] === '' && $data['account']['api_username'] === '' && $data['account']['api_token'] === '') {
      continue;
    }
    if ($data['drupal_username'] === '') {
      form_error($plugin_settings_form['accounts'][$key]['drupal_username'], t('Please enter user name.'));
    }
    elseif (!user_load_by_name($data['drupal_username'])) {
      form_error($plugin_settings_form['accounts'][$key]['drupal_username'], t('User %user is not found.', array('%user' => $data['drupal_username'])));
    }
    if ($data['account']['api_username'] === '' && $data['account']['api_token'] !== '') {
      form_error($plugin_settings_form['accounts'][$key]['account']['api_username'], t('Please enter Supertext email.'));
    }
    elseif ($data['account']['api_username'] !== '' && $data['account']['api_token'] === '') {
      form_error($plugin_settings_form['accounts'][$key]['account']['api_token'], t('Please enter Supertext API Token.'));
    }
    else {
      $result = $controller->checkCredentials($translator, $data['account']);
      if ($result === FALSE) {
        form_error($plugin_settings_form['accounts'][$key]['account'], t('Email or token is not valid.'));
      }
      elseif ($result === NULL) {
        drupal_set_message('Unable to connect to the Supertext server to check the email and API token. Check the log for more details.', 'warning', FALSE);
      }
    }
  }
  unset($data);

  // Trim and validate the default account.
  $form_state['values']['settings']['api_username'] = trim($form_state['values']['settings']['api_username']);
  $form_state['values']['settings']['api_token'] = trim($form_state['values']['settings']['api_token']);
  if ($form_state['values']['settings']['api_username'] === '' && $form_state['values']['settings']['api_token'] !== '') {
    form_error($plugin_settings_form['api_username'], t('Please enter Supertext email.'));
  }
  elseif ($form_state['values']['settings']['api_username'] !== '' && $form_state['values']['settings']['api_token'] === '') {
    form_error($plugin_settings_form['api_token'], t('Please enter Supertext API Token.'));
  }
  elseif ($form_state['values']['settings']['api_username'] !== '' && $form_state['values']['settings']['api_token'] !== '') {
    $result = $controller->checkCredentials($translator, $form_state['values']['settings']);
    if ($result === FALSE) {
      form_error($plugin_settings_form['api_username'], t('Default email or token is not valid.'));
      form_error($plugin_settings_form['api_token'], t('Default email or token is not valid.'));
    }
    elseif ($result === NULL) {
      drupal_set_message('Unable to connect to the Supertext server to check the email and API token. Check the log for more details.', 'warning', FALSE);
    }
  }
}

/**
 * Submit callback for the Supertext UI form.
 */
function _tmgmt_supertext_translator_form_submit(&$form, &$form_state) {
  // Clean up per-user accounts.
  foreach ($form_state['values']['settings']['accounts'] as $key => $data) {
    if (!is_int($key)) {
      unset($form_state['values']['settings']['accounts'][$key]);
      continue;
    }
    if ($data['drupal_username'] === '' && $data['account']['api_username'] === '' && $data['account']['api_token'] === '') {
      unset($form_state['values']['settings']['accounts'][$key]);
    }
  }
  $form_state['values']['settings']['accounts'] = array_values($form_state['values']['settings']['accounts']);
}
