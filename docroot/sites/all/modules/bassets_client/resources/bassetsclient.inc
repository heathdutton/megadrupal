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
 * Callback to resource action 'pushuuid'.
 */
function _bassets_file_pushuuid($uuid, $uri, $services_client_id) {
  $bassets_file = bassets_client_load_bassets_file($uri);
  if (empty($bassets_file)) {
    $saved = bassets_client_save_bassets_file($uuid, $services_client_id);
    if (!$saved) {
      $msg = sprintf('Did not found a connection with this ID: %id', $services_client_id);
      return array('status' => WATCHDOG_ERROR, 'message' => $msg);
    }
    $bassets_file = bassets_client_load_bassets_file($uri);
  }
  else {
    $conf = services_client_connection_load($bassets_file->bassets_server);
    // The uuid is from another host or the id was changed on the host.
    if (property_exists($conf, 'services_client_id') && $conf->services_client_id && $conf->services_client_id !== $services_client_id) {
      $msg = sprintf('Your pushed UUID: %s and your services_client_id: %s do not match with the stored data.', $uuid, $services_client_id);
      return array('status' => WATCHDOG_ERROR, 'message' => $msg);
    }
  }
  try{
    $files = entity_uuid_load('file', array($uuid));
  }
  catch (Exception $e) {
    watchdog_exception('bassets_client', $e);
    return array('status' => WATCHDOG_ERROR, 'message' => $e->getMessage());
  }
  // Build a new object.
  if (empty($files)) {
    $settings = bassets_scc_get_settings($uri);
    $fileuri = _bassets_client_replace_uri_scheme($uri);
    // We do not use file_uri_to_object() because this calls filesize(),
    // which also called in file_save().
    $file = new stdClass();
    $file->uid = empty($GLOBALS['user']->uid) ? $settings->fileowner : $GLOBALS['user']->uid;
    $file->filename = basename($fileuri);
    $file->status = FILE_STATUS_PERMANENT;
    $file->uri = $fileuri;
    $op = 'create';
  }
  else {
    $op = 'update';
    $file = reset($files);
  }
  if (!isset($file) || empty($file)) {
    return array('status' => WATCHDOG_ERROR, 'message' => 'A unknown error.');
  }
  $response_data = array();
  try{
    $client = services_client_connection_get($bassets_file->bassets_server);
    $client->action(BASSETS_SERVER_RESOURCE, 'fetch_entity',
      array(
        'uuid' => $uuid,
        'services_client_id' => services_client_get_id(),
      )
    );
    $response = $client->getResponse();
    if (!empty($response->data) && $response->data['status'] == WATCHDOG_NOTICE) {
      $response->data['file']['uri'] = _bassets_client_replace_uri_scheme($response->data['file']['uri']);
      $response_data = $response->data['file'];
    }
    elseif (isset($response->data['message']) && isset($response->data['status'])) {
      watchdog('bassets_client', print_r($response->data['message'], TRUE), array(), $response->data['status']);
    }
  }
  catch (Exception $e) {
    watchdog_exception('bassets_client', $e);
    return array('status' => WATCHDOG_ERROR, 'message' => $e->getMessage());
  }
  if (empty($response_data)) {
    return;
  }
  $updated_file = array_merge((array) $file, $response_data);
  $updated_file = (object) $updated_file;

  try{
    entity_uuid_save('file', $updated_file);
  }
  catch (Exception $e) {
    watchdog_exception('bassets_client', $e);
    return array('status' => WATCHDOG_ERROR, 'message' => $e->getMessage());
  }
  return array(
    'status' => WATCHDOG_NOTICE,
    'op' => $op,
    'file' => $updated_file,
  );
}

/**
 * Callback to resource action 'fileusage'.
 *
 * Reuse the code from file_entity_usage_page() and change the url to absolute.
 */
function _bassets_file_fileusage($uuid) {
  $files = array();
  try{
    $files = entity_uuid_load('file', array($uuid));
  }
  catch (Exception $e) {
    watchdog_exception('bassets_client', $e);
    return array('status' => WATCHDOG_ERROR, 'message' => $e->getMessage());
  }
  $usage = array();
  if (count($files) > 0) {
    $file = reset($files);
    $rows = array();
    $occured_entities = array();

    foreach (file_usage_list($file) as $module => $usage) {
      $info = system_get_info('module', $module);

      // There are cases, where actual entitiy doesen't exist.
      // We have to handle this.
      foreach ($usage as $entity_type => $entity_ids) {
        $entity_info = entity_get_info($entity_type);
        $entities = empty($entity_info) ? NULL : entity_load($entity_type, array_keys($entity_ids));

        foreach ($entity_ids as $entity_id => $count) {
          // If some other module already added this entity just sum all counts.
          if (isset($occured_entities[$entity_type][$entity_id])) {
            $rows[$occured_entities[$entity_type][$entity_id]][2] += $count;
            continue;
          }

          $label = empty($entities[$entity_id]) ? $module : entity_label($entity_type, $entities[$entity_id]);
          $entity_uri = empty($entities[$entity_id]) ? NULL : entity_uri($entity_type, $entities[$entity_id]);

          // Some entities do not have URL.
          if (empty($entity_uri)) {
            $rows[] = array($entity_type, check_plain($label), $count);
          }
          else {
            $uri = $entity_uri['path'];
            $rows[] = array(
              $entity_type,
              l($label, $uri, array('absolute' => TRUE)),
              $count,
            );
          }
          $occured_entities[$entity_type][$entity_id] = count($rows) - 1;
        }
      }
    }
    $header[] = array(
      'data' => t('Type'),
    );
    $header[] = array(
      'data' => t('Title'),
    );
    $header[] = array(
      'data' => t('Count'),
    );
    $build['usage_table'] = array(
      '#theme' => 'table',
      '#header' => $header,
      '#rows' => $rows,
      '#caption' => t('This table lists all of the places where @filename is used.',
      array('@filename' => $file->filename)),
      '#empty' => t('This file is not currently used.'),
    );
  }
  return array('status' => WATCHDOG_NOTICE, 'usage' => drupal_render($build));
}

/**
 * Callback to resource operation 'delete'.
 */
function _bassets_file_delete($uuid) {
  $files = array();
  try{
    $files = entity_uuid_load('file', array($uuid));
  }
  catch (Exception $e) {
    watchdog_exception('bassets_client', $e);
    return array('status' => WATCHDOG_ERROR, 'message' => $e->getMessage());
  }
  $file = reset($files);
  try{
    if (!empty($file)) {
      _bassets_client_static_context('delete', $uuid);
      entity_uuid_delete('file', $uuid);
    }
  }
  catch (Exception $e) {
    watchdog_exception('bassets_client', $e);
    return array('status' => WATCHDOG_ERROR, 'message' => $e->getMessage());
  }
  return array(
    'status' => WATCHDOG_NOTICE,
    'message' => sprintf('The file with the UUID: %s was deleted', $uuid),
  );
}
