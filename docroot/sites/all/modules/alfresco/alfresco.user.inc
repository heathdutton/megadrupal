<?php

/**
 * @file
 * User hooks.
 */

/**
 * Implements hook_user_categories().
 */
function alfresco_user_categories() {
  return array(array(
    'name' => 'alfresco',
    'title' => t('Alfresco'),
    'weight' => 1,
  ));
}

/**
 * Implements hook_user_login().
 *
 * Verifica que la cuenta de Alfresco del usuario sea válida.
 *
 * @todo: es posible retrasar la verificación del usuario hasta el primer uso
 *   de acceso al repositorio de alfresco.
 */
function alfresco_user_login(&$edit, $account) {

  // Si el usuario no tiene habilitado acceso al repositorio de Alfresco,
  // utilizará las credenciales genéricas para descargar los elementos.
  if (isset($account->alfresco['status']) && $account->alfresco['status'] == 0) {
    return;
  }

  // Autenticación contra Alfresco para validar la cuenta del usuario
  if (alfresco_auth_is_sso()) {

    // Si el repositorio no está correctamente configurado no intentamos
    // validar la cuenta del usuario contra Alfresco.
    if (variable_get('alfresco_http_request_fails', FALSE)) {
      return;
    }

    // Eliminamos la información de conexión como usuario anónimo, para forzar
    // a realizar la autenticación con las credenciales del usuario.
    alfresco_include('soap');
    alfresco_soap_clear_session();

    // Obtenemos las credenciales del usuario
    // @todo Hacer configurable que credenciales se van a usar
    if (!empty($account->alfresco['username'])
      && !empty($account->alfresco['password'])) {
      $username = $account->alfresco['username'];
      $password = $account->alfresco['password'];
    }
    else {
      $username = $edit['values']['name'];
      $password = $edit['values']['pass'];
    }

    // Guardamos las credenciales para re-autenticar cuando el ticket caduque.
    $_SESSION['alfresco_account']['username'] = $username;
    $_SESSION['alfresco_account']['password'] = base64_encode($password);

    // Comprobamos la cuenta de Alfresco del usuario
    if (alfresco_soap_verify_account($username, $password)) {
      $_SESSION['alfresco_account']['verified'] = TRUE;
    }
    else {
      $_SESSION['alfresco_account']['verified'] = FALSE;
    }
  }
}

/**
 * Implements hook_user_logout().
 */
function alfresco_user_logout($account) {
  // No es necesario limpiar el ticket porque la sesión se destruye cuando un
  // usuario cierra la conexión.
  // @see user_logout()
}

/**
 * Implements hook_user_insert().
 *
 * @todo Crear cuenta de alfresco si no tiene y está configurado.
 */
function alfresco_user_insert(&$edit, $account, $category) {
  alfresco_include('soap');

  // Set default alfresco user space
  $home = '/app:company_home/app:user_homes/cm:'. $account->name;     // Alfresco 3.x
  if (alfresco_soap_node_load($home)) {
    $edit['alfresco']['home'] = $home;
  }
  else {
    $home = '/app:company_home/app:user_homes/sys:'. $account->name;  // Alfresco 2.x
    if (!alfresco_soap_node_load($home)) {
      $edit['alfresco']['home'] = $home;
    }
  }
}

/**
 * Implements hook_form_FORM_ID_alter() for user_profile_form().
 */
function alfresco_form_user_profile_form_alter(&$form, &$form_state) {

  if ($form['#user_category'] == 'alfresco') {
    $account = $form['#user'];

    //drupal_add_js(drupal_get_path('module', 'alfresco') .'/alfresco.js');

    // Account information
    $form['alfresco'] = array(
      '#type' => 'fieldset',
      '#title' => t('Alfresco account information'),
      '#weight' => -10,
      '#tree' => TRUE,
    );

    if (user_access('administer users')) {
      if (alfresco_auth_is_sso()) {
        $form['alfresco']['username'] = array(
          '#type' => 'textfield',
          '#title' => t('Username'),
          '#default_value' => isset($account->data['alfresco']['username']) ? $account->data['alfresco']['username'] : $account->name,
        );
        $form['alfresco']['password'] = array(
          '#type' => 'password_confirm',
          '#size' => 25,
        );
      }
      $form['alfresco']['status'] = array(
        '#type' => 'radios',
        '#title' => t('Repository access'),
        '#default_value' => isset($account->data['alfresco']['status']) ? $account->data['alfresco']['status'] : 1,
        '#options' => array(t('Blocked'), t('Enabled'))
      );
    }

    $form['alfresco']['home'] = array(
      '#type' => 'textfield',
      '#title' => t('User home space'),
      '#default_value' => isset($account->data['alfresco']['home']) ? $account->data['alfresco']['home'] : '',
      '#description' => t('User home path on Alfresco repository.'),
    );

    $form['#validate'][] = 'alfresco_user_validate';
  }
}

/**
 * The user account is about to be modified.
 */
function alfresco_user_validate($form, &$form_state) {
  if (!empty($form_state['values']['alfresco']['home'])) {
    if (!alfresco_valid_path($form_state['values']['alfresco']['home'])) {
      form_set_error('alfresco][home', t('The home path you specified is not valid.'));
    }
    else {
      alfresco_include('soap');
      if (!alfresco_soap_node_load($form_state['values']['alfresco']['home'])) {
        form_set_error('alfresco][home', t('The home path you specified could not be found.'));
      }
    }
  }
}

/**
 * Implements hook_user_presave().
 */
function alfresco_user_presave(&$edit, $account, $category) {
  if (isset($edit['alfresco'])) {
    $edit['data']['alfresco'] = $edit['alfresco'];
  }
}
