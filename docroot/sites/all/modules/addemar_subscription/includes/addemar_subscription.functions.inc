<?php
/**
 * @file
 * Helper functions for the Addemar Subscription module.
 */

/**
 * Gets a clean version of the fields as an array.
 *
 * @return array
 *   The relevant fields.
 */
function _addemar_subscription_addemar_fields() {
  $fields = variable_get_value('addemar_subscription_addemar_fields');

  if (!empty($fields)) {
    $fields = explode("\n", $fields);
    $fields = array_map('trim', $fields);
  }

  return $fields;
}

/**
 * Ajax callback for the admin form.
 *
 * @return array
 *   The form element.
 */
function _addemar_subscription_add_more_newsletter($form, $form_state) {
  return $form['addemar_subscription_settings']['groups_wrapper']['addemar_subscription_groups'];
}

/**
 * Builds part of the Add more Newsletters form.
 *
 * @return array
 *    A group id, group description part of a form.
 */
function _addemar_subscription_add_more_newsletter_form($key, $weight, $group = array(), $description = '') {
  $form = array(
    '#tree' => TRUE,
    '#weight' => $weight++,
  );

  $form['open_newsletter_wrapper'] = array(
    '#markup' => '<div class="newsletter_wrapper">',
    '#weight' => $weight++,
  );

  $soap_wsdl = variable_get('addemar_subscription_soap_wsdl', ADDEMAR_SUBSCRIPTION_SOAP_WSDL);
  $group_options = array();
  if ($s_client = _addemar_subscription_get_soap_client($soap_wsdl)) {
    $mail_groups = $s_client->getMailgroupsByType(0);
    foreach ($mail_groups as $mail_group) {
      $group_data = $s_client->getMailgroupData($mail_group);
      // Only show static groups:
      if ($group_data->type == '1') {
        $special = ' (' . t('warning: special group!') .')';
      }
      else {
        $special = '';
      }
      $group_options[$mail_group] = check_plain('#' . $mail_group . ': ' . $group_data->name) . $special;
    }
  }

  $form['description'] = array(
    '#type'             => 'textfield',
    '#title'            => t('Newsletter description'),
    '#default_value'    => $description,
    '#required'         => FALSE,
    '#description'      => t('Enter the text that should appear as a description for this newsletter in the form where users can select the newsletters they want to subscribe to.'),
    '#weight' => $weight++,
    '#parents' => array('addemar_subscription_groups', $key, 'description'),
  );

  $form['group_id'] = array(
    '#type'             => 'checkboxes',
    '#title'            => t('Contact Groups'),
    '#default_value'    => $group,
    '#description'      => t("Select the contact groups to which people who select this newsletter should be subscribed (only <strong>static</strong> groups are shown).<br/> If you do not check any box in this option, users selecting this option will just be added to the contacts list. If you do check a group in this option, the user will be added to the general contacts list but also to the checked contact groups."),
    '#required'         => FALSE,
    '#options'          => $group_options,
    '#weight' => $weight++,
    '#parents' => array('addemar_subscription_groups', $key, 'group_id'),
  );

  $form['close_newsletter_wrapper'] = array(
    '#markup' => '</div>',
    '#weight' => $weight++,
  );

  return $form;
}

/**
 * Return proxy options (if a proxy is to be used
 *
 * @param string $url
 *   URL of the WSDL file
 *
 * @return array
 *   Array with proxy options
 */
function _addemar_subscription_proxyconfig($url) {
  $options = array();
  $server = variable_get('proxy_server', '');

  if (!$server) {
    return $options;
  }

  // Check if WSDL-host is in the proxy exception list
  $host = parse_url($url, PHP_URL_HOST);
  if ($host) {
    $localhost = array('localhost', '127.0.0.1', '::1');
    $exceptions = variable_get('proxy_exceptions', $localhost);
    $is_exception = in_array(strtolower($host), $exceptions, TRUE);
    if ($is_exception) {
      return $options;
    }
  }

  $options['proxy_host'] = $server;
  $options['proxy_port'] = variable_get('proxy_port', '');

  // Check if authentication is required
  $login = variable_get('proxy_username', '');
  if ($login) {
    $options['proxy_login'] = $login;
    $options['proxy_password'] = variable_get('proxy_password', '');
  }
  return $options;
}

/**
 * Gets a Soap Client based on a WSDL URI. Used to catch SoapFault.
 *
 * @param string|NULL
 *   URI of the WSDL file or NULL if we need to get the WSDL file from the
 *   addemar_subscription_soap_wsdl variable.
 *
 * @return bool|object
 *   The Soap client.
 */
function _addemar_subscription_get_soap_client($wsdl = NULL) {
  if (!isset($wsdl)) {
    $wsdl = variable_get('addemar_subscription_soap_wsdl', ADDEMAR_SUBSCRIPTION_SOAP_WSDL);
  }
  $s_client = FALSE;

  if (extension_loaded('soap')) {
    try {
      // Xdebug and SOAP exception handling interfere with each other here
      // so disable Xdebug if it is on, just for this call.
      if (function_exists('xdebug_disable')) {
        xdebug_disable();
      }
      $options = array('exceptions' => TRUE);
      $options = $options + _addemar_subscription_proxyconfig($wsdl);
      // Make use of @ to avoid the display of the fatal error.
      $s_client = @new SoapClient($wsdl, $options);
    }
    catch (SoapFault $e) {
      if (function_exists('xdebug_enable')) {
        xdebug_enable();
      }
      $msg = t('The Addemar subscription WSDL is not properly configured. If you are using https, check that php_openssl is enabled.');
      drupal_set_message(check_plain($msg), 'error');
      watchdog('error', $msg);
      $s_client = FALSE;
    }
    if (function_exists('xdebug_enable')) {
      xdebug_enable();
    }
  }

  return $s_client;
}
