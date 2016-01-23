<?php
/**
 * @file
 * Services API resource callbacks.
 */

/**
 * Callback to resource action 'ping'.
 */
function _bassets_server_ping() {
  return array('status' => WATCHDOG_NOTICE);
}

/**
 * Callback to resource action 'fetch_entity'.
 */
function _bassets_server_fetch_entity($uuid, $services_client_id) {
  try{
    $files = entity_uuid_load('file', array($uuid));
  }
  catch (Exception $e) {
    watchdog_exception('bassets_server', $e);
    return array(
      'status' => WATCHDOG_ERROR,
      'message' => sprintf('Could not found file with this uuid: %s.', $uuid),
    );
  }
  $file = NULL;
  if (isset($files)) {
    $file = reset($files);
    $client_name = NULL;
    $connections = _bassets_scc_connections_load('client');
    foreach ($connections as $connection) {
      if (property_exists($connection, 'services_client_id') && $connection->services_client_id === $services_client_id) {
        $client_name = $connection->name;
        break;
      }
    }
    // The ID is from another client or the ID was changed on the client.
    if (empty($client_name)) {
      watchdog('bassets_server', 'Could not found connection with id: %id. Ensure to store the services client id also in the connection.', array('%id' => $services_client_id), WATCHDOG_ERROR);
      return array('status' => WATCHDOG_ERROR);
    }
    $hook = 'bassets_fetched';
    $modules = module_implements($hook);
    foreach ($modules as $module) {
      $function = $module . '_' . $hook;
      if (function_exists($function)) {
        $function($file, array($client_name));
      }
    }
    $file->uri = _bassets_server_prepare_uri($file);
    $file->services_client_id = services_client_get_id();
    unset($file->fid);
    unset($file->uid);
  }
  return array('status' => WATCHDOG_NOTICE, 'file' => $file);
}

/**
 * Callback to resource action 'fetch_raw'.
 */
function _bassets_server_fetch_raw($uuid) {
  try{
    $files = entity_uuid_load('file', array($uuid));
  }
  catch (Exception $e) {
    watchdog_exception('bassets_server', $e);
    return array(
      'status' => WATCHDOG_ERROR,
      'message' => sprintf('Could not found file with this uuid: %s.', $uuid),
    );
  }
  $base64_code = NULL;
  $url = NULL;
  $file = reset($files);
  if (is_object($file)) {
    $scheme = file_uri_scheme($file->uri);
    $url = file_create_url($file->uri);
    if ($scheme != 'public') {
      $token = drupal_get_token();
      $url = url('bassetsfile/' . $token, array('absolute' => TRUE));
      cache_set($token, $uuid, 'cache', REQUEST_TIME + 60 * 60);
    }
  }
  else {
    return array(
      'status' => WATCHDOG_ERROR,
      'message' => sprintf('Could not found file with this uuid: %s.', $uuid),
    );
  }
  return array('status' => WATCHDOG_NOTICE, 'rawurl' => $url);
}

/**
 * Callback to resource action 'deleted'.
 */
function _bassets_file_deleted($uuid, $services_client_id) {
  $error_status = array(
    'status' => WATCHDOG_ERROR,
    'message' => sprintf('Could not found file with this uuid: %s.', $uuid),
  );
  try{
    // Rules does not resolve uuids in references entities (taxonomies)
    // or somethings other is broken.
    $ids = entity_get_id_by_uuid('file', array($uuid));
    $id = reset($ids);
  }
  catch (Exception $e) {
    watchdog_exception('bassets_server', $e);
    return $error_status;
  }
  if (!isset($id)) {
    return $error_status;
  }
  $connections = _bassets_scc_connections_load('client');
  $client_machine_name = NULL;
  if (!empty($connections)) {
    foreach ($connections as $connection_name => $conf) {
      if (property_exists($conf, 'services_client_id') && $conf->services_client_id && $conf->services_client_id === $services_client_id) {
        $client_machine_name = $connection_name;
        break;
      }
    }
  }
  if (empty($client_machine_name)) {
    watchdog('bassets_server', 'Could not found connection with id: %id. Ensure to store the services client id also in the connection.', array('%id' => $services_client_id), WATCHDOG_ERROR);
    return array('status' => WATCHDOG_ERROR);
  }

  $file = file_load($id);
  // This prevent the update hook to push the file again.
  _bassets_server_static_context('pushuuid', $uuid);
  $hook = 'bassets_deleted';
  $modules = module_implements($hook);
  foreach ($modules as $module) {
    $function = $module . '_' . $hook;
    if (function_exists($function)) {
      $function($file, $client_machine_name);
    }
  }
  return array('status' => WATCHDOG_NOTICE);
}

/**
 * Callback to resource action 'searchconfig'.
 */
function _bassets_server_search_config() {
  $index_objects = search_api_index_load('files_index')->getFields();
  unset($index_objects['search_api_language']);
  $form_elements = array();
  foreach ($index_objects as $index_object_name => $index_object) {
    if ($index_object['type'] == 'string') {
      $form_element = _bassets_server_search_config_prepare_forminfo($index_object_name, $index_object);
      if (!empty($form_element)) {
        $form_elements[$index_object_name] = $form_element;
      }
    }
  }
  return array('form_elements' => $form_elements);
}

/**
 * Helper function which prepares the form for a field, which used the reused by the basset client.
 *
 * @param string $index_object_name
 *    The fieldname/propery name which is used by search api index.
 * @param array $index_object
 *
 * @return array
 *   The form element for a field or $index_object_name.
 */
function _bassets_server_search_config_prepare_forminfo($index_object_name, $index_object) {
  $fieldname = '';
  $form_element = array();
  if (strpos($index_object_name, ':') === FALSE) {
    $fieldname = $index_object_name;
  }
  else {
    list($fieldname, ) = explode(':', $index_object_name);
  }
  if (!empty($fieldname)) {
    $info = field_info_field($fieldname);
    if (empty($info)) {
      return array();
    }
    $form_element = array('#type' => 'textfield');
    if ($info['type'] == 'list_text') {
      $form_element = array('#type' => 'select', '#empty_value' => '');
      $form_element['#options'] = $info['settings']['allowed_values'];
    }
    else if ($info['type'] == 'taxonomy_term_reference' && $index_object['type'] == 'string') {
      $form_element = array('#type' => 'select', '#empty_value' => '');
      $values = array_values(taxonomy_allowed_values($info));
      $form_element['#options'] = array_combine($values, $values);
    }
    $fields_info_by_bundle = field_info_instances('file');
    foreach ($fields_info_by_bundle as $fields_info) {
      if (isset($fields_info[$fieldname])) {
        $form_element['#title'] = $fields_info[$fieldname]['label'];
        $form_element['#description'] = $fields_info[$fieldname]['description'];
        break;
      }
    }
  }
  return $form_element;
}