<?php

/**
 * @file
 * Admin forms for warden
 */

/**
 * Page callback: System settings settings.
 *
 * @see warden_menu()
 */
function warden_form($form, &$form_state) {

  $warden_path = variable_get('warden_server_host_path', '');
  $local_token = _warden_get_local_token();

  if (empty($warden_path)) {
    drupal_set_message(t('You are missing some configuration in the sites settings.php file. Please read the README file for more details.'), 'error');
    return $form;
  }

  $form['local_token'] = array(
    '#markup' => '<p>' . t('Site security token: %token', array('%token' => $local_token)) . '</p>',
  );

  $form['message'] = array(
    '#markup' => '<p>' . t('To add this site to your Warden dashboard click the button below. Read the README file which comes with the module for configuration information.') . '</p>',
  );

  $form['add_site'] = array(
    '#type' => 'submit',
    '#value' => t('Add this site to your Warden Dashboard'),
    '#submit' => array('warden_add_site'),
  );

  return $form;
}

/**
 * Form submission callback.
 *
 * @param $form
 * @param $form_state
 * @throws Exception
 */
function warden_add_site($form, &$form_state) {
  global $base_url;
  $site_url = $base_url;
  $site_url .= "|" . _warden_get_local_token();
  $site_url_encrypted = warden_get_api()->encrypt($site_url);
  $site_host_path = variable_get('warden_server_host_path', '');
  $url = $site_host_path . '/sites/add?data=' . $site_url_encrypted;

  if (module_exists('overlay') && overlay_get_mode() == 'child') {
    unset($_GET['destination']);
    overlay_close_dialog($url, array('external' => TRUE));
    $form_state['redirect'] = FALSE;
  }
  else {
    $form_state['redirect'] = $url;
  }
}
