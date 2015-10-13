<?php

/**
 * @file
 * Plugin definition for a key provider using
 * Townsend Security’s Alliance Key Manager service.
 */

$plugin = array(
  'title' => t('Townsend Security AKM'),
  'description' => t('Use Townsend Security’s Alliance Key Manager service for the encryption key.'),
  'key callback' => 'townsec_key_akm_get_key',
  'settings form' => 'townsec_key_akm_key_settings_form',
);

/**
 * Callback method to return the encryption key from the AKM service.
 */
function townsec_key_akm_get_key($settings) {
  // If the encryption method is Townsend Security AES, don't retrieve the key.
  $encryption_method = variable_get('encrypt_encryption_method', NULL);
  if ($encryption_method == 'townsec_aes') {
    return TRUE;
  }

  $primretrieve = $settings['primary_server']['akm_retrieve_port'];
  $bkupretrieve = $settings['backup_server']['akm_retrieve_port'];
  $primserver = 'tls://' . $settings['primary_server']['akm_host_server'] . ':' . $primretrieve;
  $bkupserver = 'tls://' . $settings['backup_server']['akm_backup_server'] . ':' . $bkupretrieve;
  $keyname = $settings['key_name'];
  $errno = NULL;
  $errstr = NULL;
  $encode = $settings['encode'];
  $primlocal = DRUPAL_ROOT . '/' . $settings['primary_server']['client_cert_and_key_file'];
  $primca = DRUPAL_ROOT . '/' . $settings['primary_server']['ca_cert_file'];
  $bkuplocal = DRUPAL_ROOT . '/' . $settings['backup_server']['client_cert_and_key_file'];
  $bkupca = DRUPAL_ROOT . '/' . $settings['backup_server']['ca_cert_file'];
  // Create TLS Connection with provided key locations.
  $primopts = array(
    'ssl' => array(
      'cafile' => $primca,
      'capture_peer_cert' => TRUE,
      'local_cert' => $primlocal,
      'verify_peer' => TRUE,
    ),
  );
  $bkupopts = array(
    'ssl' => array(
      'cafile' => $bkupca,
      'capture_peer_cert' => TRUE,
      'local_cert' => $bkuplocal,
      'verify_peer' => TRUE,
    ),
  );
  // Create TLS context.
  $primctx = stream_context_create($primopts);
  $bkupctx = stream_context_create($bkupopts);
  // Request Key.
  if ($fp = stream_socket_client($primserver, $errno, $errstr, 30, STREAM_CLIENT_CONNECT, $primctx)) {
    $request = sprintf("000712001%-40s%24s" . $encode, $keyname, '');
    fwrite($fp, $request);
    $len = fread($fp, 5);
    if ($len) {
      $rsp = fread($fp, $len + 1);
      if ($rsp) {
        $key = substr($rsp, 95);
      }
    }
    fclose($fp);
  }
  elseif ($fp = stream_socket_client($bkupserver, $errno, $errstr, 30, STREAM_CLIENT_CONNECT, $bkupctx)) {
    $request = sprintf("000712001%-40s%24s" . $encode, $keyname, '');
    fwrite($fp, $request);
    $len = fread($fp, 5);
    if ($len) {
      $rsp = fread($fp, $len + 1);
      if ($rsp) {
        $key = substr($rsp, 95);
        watchdog('Key Connect', 'Primary server not found, successfully used backup server %server.', array('%server' => $bkupserver));
      }
    }
    fclose($fp);
  }
  else {
    $message = t('An error occurred');
    drupal_set_message($message, 'error', TRUE);
    watchdog('Key Connect', 'No Key Server Found');
  }
  return $key;
}

/**
 * Settings form for our key provider.
 */
function townsec_key_akm_key_settings_form($defaults) {
  $form = array();
  $form['primary_server'] = array(
    '#type' => 'fieldset',
    '#title' => t('Primary Server Settings'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['backup_server'] = array(
    '#type' => 'fieldset',
    '#title' => t('Backup Server Settings'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['primary_server']['client_cert_and_key_file'] = array(
    '#type' => 'textfield',
    '#title' => t('Location of Client X509 Certificate and Private Key File'),
    '#description' => t('The file location relative to the root Drupal directory (e.g. %rel).', array(
      '%rel' => '../private/cert/local.pem',
    )),
    '#default_value' => isset($defaults['primary_server']['client_cert_and_key_file']) ? $defaults['primary_server']['client_cert_and_key_file'] : '',
    '#required' => TRUE,
  );
  $form['primary_server']['ca_cert_file'] = array(
    '#type' => 'textfield',
    '#title' => t('Location of Certificate Authority Certificate File'),
    '#description' => t('The file location relative to the root Drupal directory (e.g. %rel).', array(
      '%rel' => '../private/cert/ca.pem',
    )),
    '#default_value' => isset($defaults['primary_server']['ca_cert_file']) ? $defaults['primary_server']['ca_cert_file'] : '',
    '#required' => TRUE,
  );
  $form['backup_server']['client_cert_and_key_file'] = array(
    '#type' => 'textfield',
    '#title' => t('Location of Client X509 Certificate and Private Key File'),
    '#description' => t('The file location relative to the root Drupal directory (e.g. %rel).', array(
      '%rel' => '../private/cert/local.pem',
    )),
    '#default_value' => isset($defaults['backup_server']['client_cert_and_key_file']) ? $defaults['backup_server']['client_cert_and_key_file'] : '',
    '#required' => FALSE,
  );
  $form['backup_server']['ca_cert_file'] = array(
    '#type' => 'textfield',
    '#title' => t('Location of Certificate Authority Certificate File'),
    '#description' => t('The file location relative to the root Drupal directory (e.g. %rel).', array(
      '%rel' => '../private/cert/ca.pem',
    )),
    '#default_value' => isset($defaults['backup_server']['ca_cert_file']) ? $defaults['backup_server']['ca_cert_file'] : '',
    '#required' => FALSE,
  );
  $form['primary_server']['akm_host_server'] = array(
    '#type' => 'textfield',
    '#title' => t('AKM Host Server'),
    '#description' => t('Enter the AKM host server name or IP number.'),
    '#default_value' => isset($defaults['primary_server']['akm_host_server']) ? $defaults['primary_server']['akm_host_server'] : '',
    '#required' => TRUE,
  );
  $form['backup_server']['akm_backup_server'] = array(
    '#type' => 'textfield',
    '#title' => t('AKM Backup Server'),
    '#description' => t('Enter the AKM Backup server name or IP number.'),
    '#default_value' => isset($defaults['backup_server']['akm_backup_server']) ? $defaults['backup_server']['akm_backup_server'] : '',
    '#required' => FALSE,
  );
  $form['primary_server']['akm_retrieve_port'] = array(
    '#type' => 'textfield',
    '#title' => t('Key Retrieval Port'),
    '#description' => t('Enter the number of the port for key retrieval.'),
    '#default_value' => isset($defaults['primary_server']['akm_retrieve_port']) ? $defaults['primary_server']['akm_retrieve_port'] : '',
    '#required' => TRUE,
  );
  $form['backup_server']['akm_retrieve_port'] = array(
    '#type' => 'textfield',
    '#title' => t('Key Retrieval Port'),
    '#description' => t('Enter the number of the port for key retrieval.'),
    '#default_value' => isset($defaults['backup_server']['akm_retrieve_port']) ? $defaults['backup_server']['akm_retrieve_port'] : '',
    '#required' => FALSE,
  );
  $form['primary_server']['akm_encrypt_port'] = array(
    '#type' => 'textfield',
    '#title' => t('Remote Encryption Port'),
    '#description' => t('Enter the number of the port for remote encryption.'),
    '#default_value' => isset($defaults['primary_server']['akm_encrypt_port']) ? $defaults['primary_server']['akm_encrypt_port'] : '',
    '#required' => TRUE,
  );
  $form['backup_server']['akm_encrypt_port'] = array(
    '#type' => 'textfield',
    '#title' => t('Remote Encryption Port'),
    '#description' => t('Enter the number of the port for remote encryption.'),
    '#default_value' => isset($defaults['backup_server']['akm_encrypt_port']) ? $defaults['backup_server']['akm_encrypt_port'] : '',
    '#required' => FALSE,
  );
  $form['key_name'] = array(
    '#type' => 'textfield',
    '#title' => t('Key Name'),
    '#description' => t('Enter the name of the key.'),
    '#default_value' => isset($defaults['key_name']) ? $defaults['key_name'] : '',
    '#required' => TRUE,
  );
  $form['key_type'] = array(
    '#type' => 'select',
    '#title' => t('Key Type'),
    '#description' => t('Choose the type of key.'),
    '#options' => array(
      'AES-256' => t('AES-256'),
    ),
    '#default_value' => isset($defaults['key_type']) ? $defaults['key_type'] : 'AES-256',
  );
  $form['key_size'] = array(
    '#type' => 'textfield',
    '#title' => t('Key Size'),
    '#description' => t('Enter the size of the key.'),
    '#default_value' => isset($defaults['key_size']) ? $defaults['key_size'] : '',
    '#required' => TRUE,
  );
  $form['encode'] = array(
    '#type' => 'select',
    '#title' => t('Encoding Method'),
    '#description' => t('Choose the encoding method.'),
    '#options' => array(
      'BIN' => t('Binary'),
      'B64' => t('Base64 Encoded'),
      'B16' => t('Base16 Encoded'),
    ),
    '#default_value' => isset($defaults['key_type']) ? $defaults['encode'] : 'B64',
  );
  return $form;
}
