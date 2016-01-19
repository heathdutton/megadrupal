<?php
/**
 * @file
 * Administration and configuration form for UNI•Login module.
 */

/**
 * Configuration form for UNI•Login parameters.
 *
 * @return array
 *   Drupal form.
 */
function unilogin_configuration_form($form, &$form_state) {
  global $base_url;

  $form['general'] = array(
    '#type' => 'fieldset',
    '#title' => t('General'),
    '#description' => t(
      'Info about UNI•Login authentication: !unilogin_info.',
      array(
        '!unilogin_info' => l(
          t('UNI•Login authentication'),
          'http://www.uni-c.dk/produkter/infrastruktur/uni-login/technicaldescriptionofuni-loginforwebbasedapplications.pdf'
        ),
      )
    ),
  );

  $form['general']['unilogin_active'] = array(
    '#type' => 'radios',
    '#title' => t('Active'),
    '#description' => t('Turn the unilogin functionality on/off.'),
    '#default_value' => variable_get('unilogin_active', 0),
    '#options' => array(t('Off'), t('On')),
  );

  $form['general']['unilogin_id'] = array(
    '#type' => 'textfield',
    '#title' => t('UNI•Login id'),
    '#description' => t('The unilogin id registered at UNI•Login.'),
    '#default_value' => variable_get('unilogin_id', ''),
    '#maxlength' => 127,
    '#required' => TRUE,
  );

  $form['general']['unilogin_secret'] = array(
    '#type' => 'textfield',
    '#title' => t('UNI•Login secret'),
    '#description' => t('Secret used to generate the md5 hash.'),
    '#default_value' => variable_get('unilogin_secret', ''),
    '#maxlength' => 127,
    '#required' => TRUE,
  );

  $form['general']['unilogin_url_login'] = array(
    '#type' => 'textfield',
    '#title' => t('Login URL'),
    '#description' => t('The URL to the external UNI•Login form.'),
    '#default_value' => variable_get(
      'unilogin_url_login',
      UNILOGIN_DEFSET_URL_LOGIN
    ),
    '#maxlength' => 127,
    '#required' => TRUE,
  );

  $form['general']['unilogin_url_return'] = array(
    '#type' => 'textfield',
    '#title' => t('Return URL'),
    '#description' => t('The URL to return to (absolute path).'),
    '#default_value' => variable_get('unilogin_url_return', $base_url),
    '#maxlength' => 127,
    '#required' => TRUE,
  );

  $form['security'] = array(
    '#type' => 'fieldset',
    '#title' => t('Security'),
    '#description' => t('<strong>NB:</strong> Do not change these values unless specifically needed. Default settings should be sufficient.'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  $form['security']['unilogin_ticket_lifespan'] = array(
    '#type' => 'textfield',
    '#title' => t('Ticket lifespan'),
    '#description' => t('How many days should a ticket be stored?'),
    '#default_value' => variable_get(
      'unilogin_ticket_lifespan',
      UNILOGIN_DEFSET_TICKET_LIFESPAN
    ),
    '#size' => 4,
    '#maxlength' => 4,
    '#required' => TRUE,
  );

  $form['security']['unilogin_timestamp_expiration'] = array(
    '#type' => 'textfield',
    '#title' => t('Timestamp expiration'),
    '#description' => t('For how many minutes is the callback timestamp valid?'),
    '#default_value' => variable_get(
      'unilogin_timestamp_expiration',
      UNILOGIN_DEFSET_TIMESTAMP_EXPIRATION
    ),
    '#size' => 4,
    '#maxlength' => 4,
    '#required' => TRUE,
  );

  return system_settings_form($form);
}
