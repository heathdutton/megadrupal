<?php
/**
 * @file
 * restmini_client: Admin layer.
 */


/**
 * RESTmini Client settings.
 *
 * @param array $form
 * @param array &$form_state
 *
 * @return array
*/
function restmini_client_admin_form($form, &$form_state) {
  drupal_add_css(
    drupal_get_path('module', 'restmini_client') . '/admin/restmini_client.admin.css',
    array('type' => 'file', 'group' => CSS_DEFAULT, 'preprocess' => FALSE, 'every_page' => FALSE)
  );

  if (!($private = variable_get('file_private_path'))) {
    drupal_set_message(t('The private:// files dir - variable \'file_private_path\' - is not set, check /admin/config/media/file-system.'), 'warning');
  }

  $form['#attributes']['autocomplete'] = 'off';

  $form['general'] = array(
    '#type' => 'fieldset',
    '#title' => t('General settings'),
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
  );

  $name = 'restmini_client_contimeout';
  $form['general'][$name] = array(
    '#type' => 'textfield',
    '#title' => t('Default connection timeout (seconds)'),
    '#description' => t(
      'Also overridable at runtime by option: \'!name\'. Default: !default.',
      array('!name' => 'connect_timeout', '!default' => RestMiniClient::CONNECT_TIMEOUT_DEFAULT)
    ),
    '#required' => TRUE,
    '#size' => 3,
    '#default_value' => variable_get($name, RestMiniClient::CONNECT_TIMEOUT_DEFAULT),
  );

  $name = 'restmini_client_reqtimeout';
  $form['general'][$name] = array(
    '#type' => 'textfield',
    '#title' => t('Default request timeout (seconds)'),
    '#description' => t(
      'Also overridable at runtime by option: \'!name\'. Default: !default.',
      array('!name' => 'request_timeout', '!default' => RestMiniClient::REQUEST_TIMEOUT_DEFAULT)
    ),
    '#required' => TRUE,
    '#size' => 3,
    '#default_value' => variable_get($name, RestMiniClient::REQUEST_TIMEOUT_DEFAULT),
  );

  $form['ssl'] = array(
    '#type' => 'fieldset',
    '#title' => t('SSL/TLS settings'),
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
  );

  $name = 'restmini_client_sslverifydefnot';
  $form['ssl'][$name] = array(
    '#type' => 'checkbox',
    '#title' => t('Default to OMIT SSL verify peer, if no value for the \'ssl_verify\' option provided?'),
    '#description' => t('Verifying SSL certificates may be a real pain. If this site generally doesn\'t need the added security of verifying certificates it may be worthwhile ignoring certificates.'),
    '#default_value' => variable_get($name, FALSE),
  );

  $name = 'restmini_client_cacertssrc';
  $form['ssl'][$name] = array(
    '#type' => 'textfield',
    '#title' => t('SSL CA Root Certificates bundle source URL'),
    '#description' => t('See also !link.', array('!link' => l(t('CA Root Certificates bundle'), RestMiniClient::SSL_CACERTS_URL)))
      . '<br/>' . t('Default: !default', array('!default' => RestMiniClient::SSL_CACERTS_URL)),
    '#required' => TRUE,
    '#default_value' => variable_get($name, RestMiniClient::SSL_CACERTS_URL),
  );

  $name = 'restmini_client_cacertsdir';
  $form['ssl'][$name] = array(
    '#type' => 'textfield',
    '#title' => t('SSL CA Certificates directory'),
    '#description' => t('Absolute path or \'private://dir\'.')
      . '<br/>' . t('Default: !default', array('!default' => 'private://certs/ca')),
    '#required' => TRUE,
    '#default_value' => variable_get($name, 'private://certs/ca'),
  );

  $name = 'restmini_client_cacertscron';
  $form['ssl'][$name] = array(
    '#type' => 'checkbox',
    '#title' => t('Cronjob: Download new SSL CA Root Certificates bundle'),
    '#default_value' => variable_get($name, 0),
    '#description' => t('Downloads to [SSL CA Certificates directory]/cacert.pem'),
  );

  $form['ssl']['download_cacertsfile_now'] = array(
    '#type' => 'checkbox',
    '#title' => t('Download new SSL CA Root Certificates bundle !emphnow!_emph', array('!emph' => '<em>', '!_emph' => '</em>')),
    '#default_value' => 1,
  );

  $form['#submit'][] = 'restmini_client_admin_form_submit';
  return system_settings_form($form);
}

/**
 * @param array $form
 * @param array &$form_state
 */
function restmini_client_admin_form_validate($form, &$form_state) {
  if (($ca = trim($form_state['values']['restmini_client_cacertsdir']))) {
    // Test for private:// the same as done runtime.
    if (!strpos(' ' . $ca, 'private://')
      && !preg_match('/^(\/|[a-zA-Z]:\/)/', $ca)
    ) {
      form_set_error(
        'restmini_client_cacertsdir',
        t(
          '!name doesn\'t comply to: !rule.',
          array(
            '!name' => t('SSL CA Root Certificates directory'),
            '!rule' => t('Absolute path or \'private://dir\''),
          )
        )
      );
    }
    elseif (preg_match('/\/$/', $ca)) {
      form_set_error(
        'restmini_client_cacertsdir',
        t(
          '!name - no trailing slash, please.',
          array(
            '!name' => t('SSL CA Root Certificates directory'),
          )
        )
      );
    }
    elseif (strpos(' ' . $ca, 'private://') && !variable_get('file_private_path')) {
      form_set_error(
        'restmini_client_cacertsdir',
        t(
          '!name cannot use the \'private://\' pattern when the private files directory isn\'t defined.',
          array(
            '!name' => t('SSL CA Root Certificates directory'),
          )
        )
      );
    }
  }
}

/**
 * @param array $form
 * @param array &$form_state
 */
function restmini_client_admin_form_submit($form, &$form_state) {
  $values =& $form_state['values'];

  // Trim.
  $fields = array(
    'restmini_client_contimeout',
    'restmini_client_reqtimeout',
    'restmini_client_cacertssrc',
    'restmini_client_cacertsdir',
  );
  foreach ($fields as $name) {
    $values[$name] = trim($values[$name]);
  }

  // Download new bundle?
  if ($values['download_cacertsfile_now']) {
    RestMiniClient::downloadCaCertBundle($values['restmini_client_cacertsdir'] . '/cacert.pem');
  }
  // Don't save that field as variable.
  unset($values['download_cacertsfile_now']);
}
