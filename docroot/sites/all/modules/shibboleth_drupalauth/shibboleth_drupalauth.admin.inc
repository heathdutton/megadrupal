<?php
/**
 * @file
 * Allows Drupal to be an authentication source for Shibboleth
 */

/**
 * Administration form (admin/config/people/shibboleth_drupalauth)
 */
function shibboleth_drupalauth_admin_settings($form, &$form_state) {
  $settings = shibboleth_drupalauth_settings();

  $form['shibboleth_drupalauth_servers'] = array(
    '#title' => t('Configured Shibboleth Identity Providers'),
    '#type' => 'fieldset',
  );


  $form['shibboleth_drupalauth_servers']['list'] = array(
    '#type' => 'markup', 
    '#prefix' => '<ul>',
    '#suffix' => '</ul>',
  );

  foreach ($settings['servers'] as $ip => $set) {
    $form['shibboleth_drupalauth_servers']['list'][$ip] = array(
      '#markup' => l(t('Shibboleth IdP at @s', array('@s' => $ip)), "admin/config/people/shibboleth_drupalauth/$ip"),
      '#prefix' => '<li>',
      '#suffix' => '</li>',
    );
  }

  $form['shibboleth_drupalauth_servers']['new_server'] = array(
      '#markup' => l(t('Configure a new identity provider'), 'admin/config/people/shibboleth_drupalauth/new'),
  );

  $settingsdesc = array(
    'cookiename' => array(
      'name' => t('Cookie Name'),
      'description' => t('The named of the authentication cookie.'),
    ),
    'cookiedomain' => array(
      'name' => t('Cookie Domain'),
      'description' => t('The domain the authentication cookie is set on (i.e. if the drupal site is drupal.example.com and the Shibboleth IdP is idp.example.com, the cookie domain should be example.com).'),
    ),
    'cookieexpire' => array(
      'name' => t('Cookie Expire'),
      'description' => t('The expiry on the authentication cookie.'),
    ),
    'cookiepath' => array(
      'name' => t('Cookie Path'),
      'description' => t('The cookie path (Should be either / or /idp).'),
    ),
  );

  $form['shibboleth_drupalauth'] = array(
    '#title' => t('Global Settings'),
    '#type' => 'fieldset',
  );

  foreach ($settings as $name => $defaultvalue) {
    if ($name == 'servers') {
      continue;
    }
    $form['shibboleth_drupalauth'][$name] = array(
      '#type' => 'textfield',
      '#title' => $settingsdesc[$name]['name'],
      '#description' => $settingsdesc[$name]['description'],
      '#size' => 80,
      '#maxlength' => 255,
      '#default_value' => isset($form_state['values'][$name]) ? $form_state['value'][$name] : $defaultvalue,
    );
  }

  $form['shibboleth_drupalauth']['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Save',
  );

  return $form;
}

/**
 * Form validation for the administrative form
 *   Empty now, but should probably do something eventually
 */
function shibboleth_drupalauth_admin_settings_validate(&$form, &$form_state) {

}

/**
 * Form submit for the administrative form
 */
function shibboleth_drupalauth_admin_settings_submit(&$form, &$form_state) {
  $settings = shibboleth_drupalauth_settings();
  foreach ($settings as $name => $value) {
    if (isset($form_state['values'][$name]) and !empty($form_state['values'][$name])) {
      $settings[$name] = $form_state['values'][$name];
    }
  }
  shibboleth_drupalauth_save_settings($settings);
}

/**
 * Configuration form (admin/config/people/shibboleth_drupalauth/%ipaddress)
 */
function shibboleth_drupalauth_admin_settings_server($form, &$form_state, $ip) {
  $settings = shibboleth_drupalauth_settings();

  $form['shibboleth_drupalauth']['ip'] = array(
    '#type' => 'textfield',
    '#title' => t('IP Address of the IdP'),
    '#description' => t('IP Address to provide data to (\'any\' will allow any IP address to test authentication.)'),
    '#size' => 80,
    '#maxlength' => 255,
    '#default_value' => isset($form_state['values']['ip']) ? $form_state['value']['ip'] :
      isset($settings['servers'][$ip]['ip']) ? $settings['servers'][$ip]['ip'] : '',
  );

  $form['shibboleth_drupalauth']['idpurl'] = array(
    '#type' => 'textfield',
    '#title' => t('URL to the IdP DrupalAuth Servelet'),
    '#description' => t('The full URL to continue the shibboleth authentication process.'),
    '#size' => 80,
    '#maxlength' => 255,
    '#default_value' => isset($form_state['values']['idpurl']) ? $form_state['value']['idpurl'] :
      isset($settings['servers'][$ip]['idpurl']) ? $settings['servers'][$ip]['idpurl'] : '',
  );

  $form['shibboleth_drupalauth']['principal'] = array(
    '#type' => 'select',
    '#title' => t('Data to provide'),
    '#description' => t('Data to provide as the Shibboleth principal'),
    '#options' => _shibboleth_drupalauth_data_options(),
    '#default_value' => isset($form_state['values']['principal']) ? $form_state['value']['principal'] :
      isset($settings['servers'][$ip]['principal']) ? $settings['servers'][$ip]['principal'] : '',
  );

  $form['shibboleth_drupalauth']['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Save',
  );

  if (isset($settings['servers'][$ip]['ip'])) {
    $form['shibboleth_drupalauth']['delete'] = array(
      '#markup' => l(t('delete'), 'admin/config/people/shibboleth_drupalauth/' . $settings['servers'][$ip]['ip'] . '/delete'),
    );
  }

  return $form;
}

/**
 * Form validation for the administrative form
 *   Empty now, but should probably do something eventually
 */
function shibboleth_drupalauth_admin_settings_server_validate(&$form, &$form_state) {
  if (!filter_var($form_state['values']['ip'], FILTER_VALIDATE_IP) and $form_state['values']['ip'] != 'any') {
    form_set_error('ip', t('IP not a valid IP address.'));
  }
}

/**
 * Form submit for the administrative form
 */
function shibboleth_drupalauth_admin_settings_server_submit(&$form, &$form_state) {
  $settings = shibboleth_drupalauth_settings();
  $settings['servers'][$form_state['values']['ip']] = array(
    'ip' => $form_state['values']['ip'],
    'idpurl' => $form_state['values']['idpurl'],
    'principal' => $form_state['values']['principal'],
  );
  shibboleth_drupalauth_save_settings($settings);
  drupal_goto('admin/config/people/shibboleth_drupalauth');
}

/**
 * Delete Server
 */
function shibboleth_drupalauth_admin_settings_server_delete($form, &$form_state, $ip) {
  $settings = shibboleth_drupalauth_settings();
  unset($settings['servers'][$ip]);
  shibboleth_drupalauth_save_settings($settings);
  drupal_goto('admin/config/people/shibboleth_drupalauth');
}

