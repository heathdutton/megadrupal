<?php

/**
 * @file
 * Menu callbacks and related functions.
 */

/**
 * Provides AJAX Azure login form.
 */
function azure_auth_login() {
  $form = drupal_render(drupal_get_form('azure_auth_login_form'));
  return array(
    '#type' => 'ajax',
    '#commands' => array(
      ajax_command_html('#azure_auth_container', $form),
    ),
  );
}

/**
 * Implements the Azure login form.
 *
 * @param bool $wrap
 *   If true, wrap the identity provider buttons in a fieldset.
 */
function azure_auth_login_form($form, $form_state, $wrap = TRUE) {
  $identity_providers = _azure_auth_get_identity_providers();
  $form = array();
  $form['wrap'] = array(
    '#type' => 'value',
    '#value' => $wrap,
  );
  if (!$identity_providers) {
    return $form;
  }
  $temp = array();
  $temp['identity_providers'] = array(
    '#type' => 'value',
    '#value' => $identity_providers,
  );
  foreach ($identity_providers as $identity_provider) {
    $temp[$identity_provider['Name']] = array(
      '#type' => 'submit',
      '#default_value' => $identity_provider['Name'],
    );
  }
  if ($wrap) {
    $form['azure_auth'] = array(
      '#type' => 'fieldset',
      '#title' => variable_get('azure_auth_link_text', t('Sign in using Windows Azure')),
    );
    $form['azure_auth'] = array_merge($form['azure_auth'], $temp);
  }
  else {
    $form = array_merge($form, $temp);
  }
  return $form;
}

/**
 * Submit callback.
 */
function azure_auth_login_form_submit($form, &$form_state) {
  $fields = $form_state['values']['wrap'] ? $form['azure_auth'] : $form;

  foreach ($fields['identity_providers']['#value'] as $identity_provider) {
    if ($form_state['clicked_button']['#default_value'] == $identity_provider['Name']) {
      $form_state['redirect'] = $identity_provider['LoginUrl'];
    }
  }
}

/**
 * Authentication callback function.
 *
 * Handles an authentication token and, if able, will log the user in or allow
 * the user to register.
 */
function azure_auth_authenticate($error) {
  require_once 'azure_auth.classes.inc';
  require_once 'azure_auth.identity-providers.inc';
  if ($error) {
    $message = t('Unable to log in using Windows Azure.');
    watchdog('azure_auth', $message, array(), WATCHDOG_ERROR);
    drupal_set_message($message, 'error');
  }
  if (isset($_POST['wresult'])) {
    try {
      $identity_providers = module_invoke_all('azure_identity_providers');
      drupal_alter('azure_identity_providers', $identity_providers);
      $token = new AzureAuthenticationSWT(
        $_POST['wresult'],
        variable_get('azure_auth_realm'),
        variable_get('azure_auth_namespace'),
        variable_get('azure_auth_key'),
        variable_get('azure_auth_oldkey'),
        $identity_providers
      );
      if ($token->isValid()) {
        if ($uid = _azure_auth_get_authmap($token->getNameIdentifier())) {
          $account = user_load($uid);
          drupal_set_message(t('Logged in'));
          $form_state['uid'] = $account->uid;
          user_login_submit(array(), $form_state);
        }
        else {
          $_SESSION['azure_auth'] = $token;
          drupal_goto('user/register');
        }
      }
    }
    catch (Exception $e) {
      drupal_set_message($e->getMessage(), 'error');
    }
  }
  drupal_goto('user');
}
