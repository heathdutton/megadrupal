<?php

/**
 * @file
 * Plugin definition for the Townsend Security AES encryption method.
 */

$plugin = array(
  'title' => t('Townsend Security AES'),
  'description' => t('This encryption method uses the Townsend Security Alliance Key Manager onboard AES encryption to provide remote NIST certified encryption of sensitive data.'),
  'encrypt callback' => 'townsec_key_encryption_method_aes',
);

/**
 * Callback for Encrypt implementation: default.
 *
 * This method uses a simple encryption method of character
 * replacement.
 */
function townsec_key_encryption_method_aes($op = 'encrypt', $text = '', $key = '', $options = array()) {
  $provider = encrypt_get_key_provider('townsec_akm');
  $settings = $provider['settings'];
  $primencrypt = $settings['primary_server']['akm_encrypt_port'];
  $bkupencrypt = $settings['backup_server']['akm_encrypt_port'];
  $primserver = 'tls://' . $settings['primary_server']['akm_host_server'] . ':' . $primencrypt;
  $bkupserver = 'tls://' . $settings['backup_server']['akm_backup_server'] . ':' . $bkupencrypt;
  $keyname = $settings['key_name'];
  $errno = NULL;
  $errstr = NULL;
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
  if ($fp = stream_socket_client($primserver, $errno, $errstr, 30, STREAM_CLIENT_CONNECT, $primctx)) {
    // Initiate the primary connection.
  }
  elseif ($fp = stream_socket_client($bkupserver, $errno, $errstr, 30, STREAM_CLIENT_CONNECT, $bkupctx)) {
    // Create backup connection.
  }
  if ($fp == FALSE) {
    return '';
  }
  // Key Length = 40 (left justify pad on right).
  // Instance = 24 (leave blank or instance got back).
  if ($op == 'encrypt') {
    // Generate a random IV to use w/ the encryption.
    $iv = user_password(16);
    $textcount = sprintf('%05d', strlen($text));
    if(floor($textcount/16) != $textcount/16){
      $padlen = 16 * ceil($textcount/16);
      $text = sprintf('% -' . $padlen . 's', $text);
      $textcount = sprintf('%05d', strlen($text));
    }
    $key = sprintf('% -64s', $keyname);
    $request = sprintf("000982019YNB16" . $textcount . "YNYY" . $iv . "" . $key . "" . "" . $text . "");
    fwrite($fp, $request);
    $len = fread($fp, 5);
    if ($len) {
      //Be sure to read all the way to the end of the returned values
      $return = fread($fp, $len + (3*$textcount));
      if ($return) {
        $inst = substr($return, 15, 24);
        $coded = substr($return, 39);
        $value = $iv . $inst . $coded;
      }
    }
    else {
      return '';
    }
    fclose($fp);
    return $value;
  }
  else {
    $iv = substr($text, 0, 16);
    $inst = substr($text, 16, 24);
    $coded = substr($text, 40);
    $textcount = sprintf('%05d', strlen($coded));
    $keypad = sprintf('% -40s', $keyname);
    $key = $keypad . $inst;
    //Make sure request is mod16
    if(floor($textcount/16) != $textcount/16){
      $padlen = 16 * ceil($textcount/16);
      $coded = sprintf('% -' . $padlen . 's', $coded);
      $textcount = sprintf('%05d', strlen($coded));
    }
    $decrypt_header = "001012021YNB16" . $textcount . "BINYNYY" . $iv . $key;
    $decrypt = sprintf($decrypt_header . $coded);
    fwrite($fp, $decrypt);
    $len = fread($fp, 5);
    if ($len) {
      $rsp = fread($fp, $len + $textcount);
      if ($rsp) {
        $value = substr($rsp, 39);
        $value = rtrim($value);
      }
    }
    else {
      return '';
    }
    fclose($fp);
    return $value;
  }
}
