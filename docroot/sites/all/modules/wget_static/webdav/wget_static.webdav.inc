<?php
/**
 * @file
 * Consists basic functions for webdav.
 */

/**
 * To create new directory.
 */
function _wget_static_webdav_delete_req_cmd($form_state, $folder) {
  return "-X DELETE '" . $form_state['values']['protocol'] . "://" . $form_state['values']['host'] . "/" . $folder . "/' ";
}

/**
 * Creates certificates cmd.
 */
function _wget_static_webdav_cert_cmd($form_state) {
  $pem = drupal_realpath(file_load($form_state['values']['pem'])->uri);
  if (preg_match("/.pem\z/", $pem)) {
    $crt = drupal_realpath(file_load($form_state['values']['cert'])->uri);
    return "--cert " . $pem . ":'" . $form_state['values']['passph'] . "' --cacert " . $crt . " ";
  }
  elseif (preg_match("/.p12\z/", $pem)) {
    return "--cert " . $pem . ":'" . $form_state['values']['passph'] . "' ";
  }
}

/**
 * Returns reponse code.
 */
function _wget_static_parse_reponse($response) {
  $status = preg_match("/[0-9]{3}\z/", $response, $code);
  return $code['0'];
}

/**
 * Rename index.html Request.
 */
function _wget_static_webdav_rename_cmd($form_state, $folder) {
  return "-X MOVE --header 'Destination: " . $form_state['values']['protocol'] . "://" . $form_state['values']['host'] . "/" . $folder . "/indexrenamed.html' '" . $form_state['values']['protocol'] . "://" . $form_state['values']['host'] . "/" . $folder . "/index.html' ";
}

/**
 * Create Directory cmd.
 */
function _wget_static_webdav_create_dir_cmd($form_state, $folder) {
  return "-X MKCOL '" . $form_state['values']['protocol'] . "://" . $form_state['values']['host'] . "/" . $folder . "/' ";
}

/**
 * Upload Request cmd.
 */
function _wget_static_webdav_upload_content_cmd($src, $des, $file) {
  return "-T '" . $src . "/" . $file . "' '" . $des . "/' ";
}

/**
 * Upload Request cmd.
 */
function _wget_static_webdav_upload_file_cmd($src_file, $des, $file) {
  return "-T '" . $src_file . "' '" . $des . "/" . $file . "' ";
}
