<?php

/**
 * @file
 * API documentation for Bassets server.
 */

/**
 * Allows you to remove a Bassets client from the stack.
 *
 * Before the Bassets server pushs the file to the clients.
 *
 * @see _bassets_server_push_file()
 *
 * @param object $file
 *   The file entity.
 * @param array $clients
 *   An array of bassets client.
 */
function hook_bassets_pre_push_alter($file, &$clients) {
  if ($file->filename == 'myfile') {
    $clients = array();
  }
  if (in_array('bassets_client_1', $clients) && $file->fid == 5) {
    unset($clients['bassets_client_1']);
  }
}

/**
 * A file was pushed to one or more Bassets clients.
 *
 * @see _bassets_server_push_file()
 *
 * @param object $file
 *   The file entity.
 * @param array $client_names
 *   An array of bassets client.
 */
function hook_bassets_pushed($file, $client_names = array()) {
  if (!empty($client_names)) {
    _mymodule_save($client_names, $file);
  }
}

/**
 * A file was deleted on this Bassets client.
 *
 * @see _bassets_file_deleted()
 *
 * @param object $file
 *   The file entity.
 * @param string $client_name
 *   The machine name of the connection.
 */
function hook_bassets_deleted($file, $client_name) {
  _mymodule_send_email($file, $client_name);
}

/**
 * A file was fetched by a Bassets client.
 *
 * @see _bassets_server_fetch_entity()
 *
 * @param object $file
 *   The file entity.
 * @param array $client_names
 *   An array of bassets client.
 */
function hook_bassets_fetched($file, $client_names = array()) {
  if (!empty($client_names)) {
    _mymodule_save($client_names, $file);
  }
}
