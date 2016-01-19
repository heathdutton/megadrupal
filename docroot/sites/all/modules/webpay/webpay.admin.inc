<?php

/**
 * @file
 * This file has the administrative pages for this module.
 */


/**
 * Helper function to storage the currencies supported by Webpay.
 *
 * @return array
 *   List of currencies and they corresponded code.
 */
function webpay_get_currencies() {
  return array_keys(webpay_admin_default_code());
}


/**
 * Helper debug function.
 *
 * @return string
 *   HTML string
 */
function webpay_admin_debug() {
  $output = '<h2>' . t('Logs') . '</h2>';
  $output .= t('Here you can see the sequence of a transaction on close page');
  $output .= webpay_get_log();
  return $output;
}


/**
 * Return a table with all logs related to Webpay's KCC.
 *
 * @return string
 *   HTML table with pager
 */
function webpay_admin_log() {
  $transaction = webpay_get_all_webpay_transaction(TRUE);
  $output = t('No log recorded');
  if (!empty($transaction)) {
    foreach ($transaction[0] as $key => $value) {
      $header[] = $key;
    }
    foreach ($transaction as $value) {
      $rows[] = $value;
    }
    $output = theme('table', array('rows' => $rows, 'header' => $header));
    $output .= theme('pager', array());
  }
  return $output;
}


/**
 * Get a form for all configurations.
 *
 * @param mixed $form
 *   form element
 * @param mixed $form_state
 *   current form state
 *
 * @return array
 *   form element
 */
function webpay_configuration($form, $form_state) {
  $form += webpay_commerce_system_config();
  $form += webpay_basic_config();
  $form += webpay_redirect_pages_config();
  return system_settings_form($form);
}


/**
 * Defines de basic_config form.
 *
 * @return array
 *   form element
 */
function webpay_basic_config() {
  $form['webpay_basic_config'] = array(
    '#type' => 'fieldset',
    '#title' => t('Basic Settings'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['webpay_basic_config']['webpay_devolucion_url'] = array(
    '#type' => 'textfield',
    '#title' => t('Refund URL'),
    '#description' => t('Example: "node/1" .Transbank requires a url that gives information about refunds.'),
    '#default_value' => variable_get('webpay_devolucion_url', ''),
    '#size' => 19,
    '#maxlength' => 255,
  );

  $form['webpay_basic_config']['webpay_debug_webpay_close'] = array(
    '#type' => 'checkbox',
    '#title' => t('Debug the webpay close page'),
    '#description' => t('Registers $_SERVER values on POST action to close page. Just for development purpose'),
    '#default_value' => variable_get('webpay_debug_webpay_close', FALSE),
  );

  $form['webpay_basic_config']['webpay_active_access'] = array(
    '#type' => 'checkbox',
    '#title' => t('Active Access Restriction'),
    '#description' => t('Activates access restriction for IPs on dev or prod enviroment'),
    '#default_value' => variable_get('webpay_active_access', FALSE),
  );

  return $form;
}


/**
 * Form to define de commerce system for the site.
 *
 * @return array
 *   form element
 */
function webpay_commerce_system_config() {
  $form['webpay_commerce_system'] = array(
    '#type' => 'fieldset',
    '#title' => t('Activated Electronic Commerce'),
    '#collapsible' => TRUE,
  );
  $infos = module_invoke_all('webpay_commerce_system');
  $rows = array();
  if (!empty($infos)) {
    foreach ($infos as $key => $info) {
      $rows[] = array(
        $info['title'] . " ($key)",
        isset($info['description']) ? $info['description'] : '',
      );
    }
  }

  $form['webpay_commerce_system']['systems'] = array(
    '#theme' => 'table',
    '#rows' => $rows,
    '#empty' => t('You need Active a Commerce System'),
    '#header' => array(t('System'), t('Description')),
  );

  return $form;
}


/**
 * Form to retrieve redirections on different cases.
 *
 * @return array
 *   form element
 */
function webpay_redirect_pages_config() {
  $form['redirect_pages_no_order'] = array(
    '#type' => 'fieldset',
    '#title' => t('Redirect pages if no order on POST'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  $form['redirect_pages_no_order']['webpay_redirect_no_order_failure'] = array(
    '#type' => 'textfield',
    '#title' => t('Redirect page from Failure'),
    '#default_value' => variable_get('webpay_redirect_no_order_failure', ''),
  );

  $form['redirect_pages_no_order']['webpay_redirect_no_order_success'] = array(
    '#type' => 'textfield',
    '#title' => t('Redirect page from Success'),
    '#default_value' => variable_get('webpay_redirect_no_order_success', ''),
  );

  return $form;
}


/**
 * Form for Webapay's KCC's values.
 *
 * @param array $form
 *   form element
 * @param array $form_state
 *   current form state
 *
 * @return array
 *   form element
 */
function webpay_configuration_kcc($form, $form_state) {
  $form['webpay_kcc_general'] = array(
    '#type' => 'fieldset',
    '#title' => t('General Data'),
    '#description' => t('This parameters will be written on tbk_config.dat file. Apache must have write permission'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  $form['webpay_kcc_general']['webpay_tbk_check_mac_extension'] = array(
    '#type' => 'select',
    '#title' => t('Extension MAC'),
    '#options' => array(
      'cgi' => 'CGI for Linux',
      'exe' => 'EXE for Window',
    ),
    '#default_value' => variable_get('webpay_tbk_check_mac_extension', 'cgi'),
  );

  $form['webpay_kcc_general']['webpay_MEDCOM'] = array(
    '#type' => 'select',
    '#title' => t('MEDCOM'),
    '#options' => array(
      1 => 'Secure conection (SSL)',
      2 => 'Connecting through redirection using Javascript.',
    ),
    '#description' => t('KCC 6 only has "Secure conection (SSL)" option. But it is possible that in certification mode you must set the second option.'),
    '#default_value' => variable_get('webpay_MEDCOM', 1),
    '#required' => TRUE,
  );

  $form['webpay_kcc_general']['webpay_SERVERCOM'] = array(
    '#type' => 'textfield',
    '#title' => t('SERVERCOM'),
    '#description' => t('IP of the server in which there is tbk_bp_resultado file'),
    '#default_value' => variable_get('webpay_SERVERCOM', '0.0.0.0'),
    '#size' => 19,
    '#maxlength' => 19,
    '#required' => TRUE,
  );

  $form['webpay_kcc_general']['webpay_PORTCOM'] = array(
    '#type' => 'textfield',
    '#title' => t('PORTCOM'),
    '#description' => t('Port assigned to the server in which there is tbk_bp_resultado'),
    '#default_value' => variable_get('webpay_PORTCOM', '80'),
    '#size' => 12,
    '#maxlength' => 12,
    '#required' => TRUE,
  );

  $form['webpay_kcc_general']['webpay_WHITELISTCOM'] = array(
    '#type' => 'textfield',
    '#title' => t('WHITELISTCOM'),
    '#description' => t('List of characters allowed by POST or GET'),
    '#default_value' => variable_get('webpay_WHITELISTCOM', 'ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz 0123456789./:=&?_'),
    '#size' => 60,
    '#maxlength' => 90,
    '#required' => TRUE,
  );

  $form['webpay_kcc_general']['webpay_HOST'] = array(
    '#type' => 'textfield',
    '#title' => t('Host'),
    '#description' => t("This server's IP"),
    '#default_value' => variable_get('webpay_HOST', '0.0.0.0'),
    '#size' => 19,
    '#maxlength' => 19,
    '#required' => TRUE,
  );

  $form['webpay_kcc_general']['webpay_WPORT'] = array(
    '#type' => 'textfield',
    '#title' => t('WPORT'),
    '#description' => t('Server HTTP port'),
    '#default_value' => variable_get('webpay_WPORT', '80'),
    '#size' => 19,
    '#maxlength' => 19,
    '#required' => TRUE,
  );

  $form['webpay_kcc_general']['webpay_URLCGITRA'] = array(
    '#type' => 'textfield',
    '#title' => t('URLCGITRA'),
    '#description' => t('Path for cgi bp_revision'),
    '#default_value' => variable_get('webpay_URLCGITRA', '/filtroUnificado/bp_revision.cgi'),
    '#size' => 30,
    '#maxlength' => 60,
    '#required' => TRUE,
  );

  $form['webpay_kcc_general']['webpay_URLCGIMEDTRA'] = array(
    '#type' => 'textfield',
    '#title' => t('URLCGIMEDTRA'),
    '#description' => t('Path for bp_validacion'),
    '#default_value' => variable_get('webpay_URLCGIMEDTRA', '/filtroUnificado/bp_validacion.cgi'),
    '#size' => 30,
    '#maxlength' => 60,
    '#required' => TRUE,

  );

  $form['webpay_kcc_general']['webpay_PREFIJO_CONF_TR'] = array(
    '#type' => 'textfield',
    '#title' => t('PREFIJO_CONF_TR'),
    '#description' => t('Prefix for tha page that confirms the transaction'),
    '#default_value' => variable_get('webpay_PREFIJO_CONF_TR', 'HTML_'),
    '#size' => 30,
    '#maxlength' => 60,
    '#required' => TRUE,
  );

  $currencies = webpay_get_currencies();
  foreach ($currencies as $currency) {
    $form += webpay_kcc_config($currency);
  }

  $form['#submit'][] = 'webpay_edit_files';

  return system_settings_form($form);
}


/**
 * Form for KCC's basic configuration.
 *
 * @param string $currency
 *   currency code
 *
 * @return array
 *   form element
 */
function webpay_kcc_config($currency) {
  global $base_url;

  $environments  = webpay_get_environment();
  $code_defaults = webpay_admin_default_code();

  foreach ($environments as $key => $environment) {
    $options[$key] = $environment['name'];
  }

  $options_translate = array(
    '!currency' => $currency,
  );
  $form['webpay_kcc_' . $currency] = array(
    '#type' => 'fieldset',
    '#title' => t('Data KCC for !currency', $options_translate),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  $form['webpay_kcc_' . $currency]['webpay_ROOT_CGI_' . $currency] = array(
    '#type' => 'textfield',
    '#title' => t('PATH CGI for !currency', $options_translate),
    '#description' => t('If no path is defined then the currency is not enabled'),
    '#default_value' => variable_get('webpay_ROOT_CGI_' . $currency, ''),
  );

  $form['webpay_kcc_' . $currency]['webpay_IDCOMERCIO_' . $currency] = array(
    '#type' => 'textfield',
    '#title' => t('ID COMERCIO for !currency', $options_translate),
    '#description' => t('Twelve digit code depending on the mode choosen'),
    '#default_value' => variable_get('webpay_IDCOMERCIO_' . $currency, $code_defaults[$currency]),
    '#size' => 19,
    '#maxlength' => 19,
    '#required' => TRUE,
  );

  $form['webpay_kcc_' . $currency]['webpay_TBK_KEY_ID_' . $currency] = array(
    '#type' => 'textfield',
    '#title' => t('TBK KEY ID for !currency', $options_translate),
    '#description' => t('Public key for the commerce'),
    '#default_value' => variable_get('webpay_TBK_KEY_ID_' . $currency, '100'),
    '#size' => 5,
    '#maxlength' => 5,
    '#required' => TRUE,
  );

  $form['webpay_kcc_' . $currency]['webpay_PARAMVERIFCOM_' . $currency] = array(
    '#type' => 'checkbox',
    '#title' => t('PARAMVERIFCOM for !currency', $options_translate),
    '#description' => t('Check if you want filter and parameter revision'),
    '#default_value' => variable_get('webpay_PARAMVERIFCOM_' . $currency, 1),
  );

  $form['webpay_kcc_' . $currency]['webpay_environment_' . $currency] = array(
    '#type' => 'select',
    '#title' => t('Environment for !currency', $options_translate),
    '#description' => t('Select the Environment.'),
    '#options' => $options,
    '#default_value' => variable_get('webpay_environment_' . $currency, 0),
    '#required' => TRUE,
  );

  $form['#validate'][] = 'webpay_admin_kcc_config_validate';

  return $form;
}


/**
 * Validation for webpay_kcc_config().
 *
 * @param array $form
 *   form element
 * @param array $form_state
 *   current form state
 */
function webpay_admin_kcc_config_validate($form, &$form_state) {
  $values    = $form_state['values'];
  $path_root = webpay_document_root() . '/';

  $values['webpay_ROOT_CGI_CLP'] = trim($values['webpay_ROOT_CGI_CLP']);
  $values['webpay_ROOT_CGI_USD'] = trim($values['webpay_ROOT_CGI_USD']);

  $no_empty_clp  = ($values['webpay_ROOT_CGI_CLP'] != '');
  $no_empty_usd  = ($values['webpay_ROOT_CGI_USD'] != '');
  $equal_clp_usd = ($values['webpay_ROOT_CGI_CLP'] == $values['webpay_ROOT_CGI_USD']);

  if ($no_empty_clp && $no_empty_usd && $equal_clp_usd) {
    $msg = t('If you want both currencies then you will need to install 2 KCCs');
    form_set_error('webpay_ROOT_CGI_USD', $msg);
    return;
  }

  $currs = array();
  if ($no_empty_clp) {
    $currs[] = 'CLP';
  }
  elseif ($no_empty_usd) {
    $currs[] = 'USD';
  }

  if ($no_empty_clp || $no_empty_usd) {
    foreach ($currs as $curr) {
      $tbk_config  = $path_root . $values['webpay_ROOT_CGI_' . $curr];
      $tbk_config .= '/datos/tbk_config.dat';

      if (!file_exists($tbk_config)) {
        $msg = t('Invalid path or tbk_config.dat not found');
        form_set_error('webpay_ROOT_CGI_' . $curr, $msg);
      }
    }
  }
}


/**
 * Writes the file tbk_config.dat.
 *
 * @param array $form
 *   form element
 * @param array $form_state
 *   current form state
 */
function webpay_edit_files($form, $form_state) {
  global $base_url;
  $environments = webpay_get_environment();
  $settings = $form_state['values'];

  foreach (array('CLP', 'USD') as $currency) {
    if ($settings['webpay_ROOT_CGI_' . $currency] == '') {
      continue;
    }

    $filename = 'tbk_config.dat.' . $currency;
    $uri = 'public://' . $filename;
    $file = drupal_realpath($uri);
    $fp = fopen($file, "w");

    $environment = $environments[$settings['webpay_environment_' . $currency]];
    $data_tbk_config = array(
      'IDCOMERCIO  = ' . $settings['webpay_IDCOMERCIO_' . $currency],
      'MEDCOM  = ' . $settings['webpay_MEDCOM'],
      'PARAMVERIFCOM = ' . $settings['webpay_PARAMVERIFCOM_' . $currency],
      'TBK_KEY_ID  = ' . $settings['webpay_TBK_KEY_ID_' . $currency],
      'URLCGICOM = ' . $base_url . '/' . $settings['webpay_ROOT_CGI_' . $currency] . '/tbk_bp_resultado.cgi',
      'SERVERCOM = ' . $settings['webpay_SERVERCOM'],
      'PORTCOM = ' . $settings['webpay_PORTCOM'],
      'WHITELISTCOM = ' . $settings['webpay_WHITELISTCOM'],
      'HOST = ' . $settings['webpay_HOST'],
      'WPORT = ' . $settings['webpay_WPORT'],
      'URLCGITRA = ' . $settings['webpay_URLCGITRA'],
      'URLCGIMEDTRA = ' . $settings['webpay_URLCGIMEDTRA'],
      'SERVERTRA = ' . $environment['servertra'],
      'PORTTRA = ' . $environment['porttra'],
      'PREFIJO_CONF_TR = ' . $settings['webpay_PREFIJO_CONF_TR'],
      'HTML_TR_NORMAL = ' . $base_url . '/?q=webpay/close/' . $currency,
    );
    foreach ($data_tbk_config as $data) {
      fwrite($fp, $data . "\n");
    }
    fclose($fp);

    $tbk_config = webpay_document_root() . '/' . $settings['webpay_ROOT_CGI_' . $currency] . '/datos/tbk_config.dat';

    if (!is_writable($tbk_config)) {
      $url = file_create_url($uri);

      $args = array(
        '@currency'   => $currency,
        '%filename'   => $filename,
        '@tbk_config' => $tbk_config,
        '@url'        => $url,
      );
      drupal_set_message(t('It seems Apache does not have permission to copy the file tbk_config.dat on KCC for the currency @currency. You can download the file from <a href="@url">here</a> and replace it for @tbk_config', $args), 'warning');
    }
    else {
      if (copy($file, $tbk_config)) {
        $args = array(
          '@currency'   => $currency,
          '@tbk_config' => $tbk_config,
        );
        drupal_set_message(t('tbk_config.dat for @currency has been created and has been copied in @tbk_config', $args));
      }
    }
  }
}


/**
 * Helper function to storage default currency codes.
 *
 * @return array
 *   an array with the default codes
 */
function webpay_admin_default_code() {
  return array(
    'CLP' => '597026007976',
    'USD' => '597026007984',
  );
}
