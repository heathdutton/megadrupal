<?php

/**
 * @file
 *
 * Drush commands for administer Shared Content client.
 */

/**
 * Implements hook_drush_command().
 */
function sharedcontent_client_drush_command() {
  $items = array();

  $items['sharedcontent-client-synchronize'] = array(
    'description' => 'Synchronize the index with remote sites',
    'aliases' => array('sc-sync'),
    'examples' => array(
      'drush sc-sync' => dt('Synchronize with all availlable connections.'),
      'drush sc-sync all' => dt('Synchronize with all availlable connections.'),
      'drush sc-sync connection-name' => dt('Synchronize only with the given connection.'),
      'drush sc-sync all "2012-08-27 11:54" 500' => dt('Synchronize the given amount of records newer than the given time with all availlable connections.'),
    ),
    'arguments' => array(
      'connection-name' => dt('The name of the connection or all to synchronize all connections. Blank is equals to all.'),
      'time' => dt('Time in the format Y-m-d H:i. If given only records newer than the given time will be synchronized. If left blank defaults to all.'),
      'count' => dt('Count of records to be synchronized. If left blank defaults to all.'),
    ),
    'drupal dependencies' => array('sharedcontent_client'),
  );

  $items['sharedcontent-client-connection-create'] = array(
    'description' => 'Create new Shared Content connection',
    'aliases' => array('sc-conc'),
    'examples' => array(
      'drush sc-conc http://example.com/sharedcontent' => dt('Crate connection without authentication.'),
      'drush sc-conc http://example.com/sharedcontent foo bar' => dt('Create connection using foo as username and bar as password.'),
      'drush sc-conc http://example.com/sharedcontent foo bar "My Connection"' => dt('Create connection named My Connection.'),
      'drush sc-conc http://example.com/sharedcontent foo bar "My Connection" sc_endpoint' => dt('Create connection using sc_endpoint as machine readable name.'),
    ),
    'arguments' => array(
      'endpoint' => dt('The endpoint url of the service.'),
      'username' => dt('Username to be used for authentication.'),
      'password' => dt('Username to be used for authentication.'),
      'name' => dt('Human readable name to identifie the connection in the ui.'),
      'machine_name' => dt('Unique identifier for the connection.'),
    ),
    'drupal dependencies' => array('sharedcontent_client'),
  );

  $items['sharedcontent-client-manage-linkables'] = array(
    'description' => 'Synchronize the index with remote sites',
    'aliases' => array('sc-link'),
    'examples' => array(
      'drush sc-link add article node' => dt('Add the default sharedcontent_reference field to node article.'),
      'drush sc-link remove article' => dt('Removes the default sharedcontent_reference field from node article.'),
    ),
    'arguments' => array(
      'action' => dt('action to be done. Can be add or remove.'),
      'bundle' => dt('The bundle the action should be applied.'),
      'entity_type' => dt('The entity the action should be applied. If left blank defaults to node'),
    ),
    'drupal dependencies' => array('sharedcontent_client'),
  );

  $items['sharedcontent-create-search-db'] = array(
    'description' => 'Creates a search API server in DB',
    'aliases' => array('sc-csdb'),
    'examples' => array(
      'drush sc-csdb test_server' => dt('Creates a search server in DB with name "test_server"'),
    ),
    'arguments' => array(
      'name' => dt('Server name.'),
    ),
    'drupal dependencies' => array(
      'sharedcontent_client',
      'search_api_db'
    ),
  );

  return $items;
}

/**
 * Creates a search api server in DB.
 */
function drush_sharedcontent_client_sharedcontent_create_search_db($server_name) {
  module_load_include('inc', 'search_api', 'search_api.admin');

  $form_state['values']['name'] = $form_state['values']['machine_name'] = $server_name;
  $form_state['values']['class'] = 'search_api_db_service';
  $form_state['values']['options'] = array('form' => array('min_chars' => 1));

  drupal_form_submit('search_api_admin_add_server', $form_state);

  if ($errors = form_get_errors()) {
    drush_set_error('ERROR', implode(' | ', $errors));
  }
}

/**
 * Drush command to synchronize the index.
 */
function drush_sharedcontent_client_synchronize($connection_name = FALSE, $time = NULL, $count = NULL) {
  if ($connection_name && drupal_strtolower($connection_name) != 'all') {
    _drush_sharedcontent_client_synchronize(services_client_connection_load($connection_name), $time, $count);
  }
  else {
    foreach (sharedcontent_client_get_connections() as $connection) {
      _drush_sharedcontent_client_synchronize($connection, $time, $count);
    }
  }
}

/**
 * Helper function to synchronize index
 *
 * @see drush_sharedcontent_client_synchronize().
 */
function _drush_sharedcontent_client_synchronize($connection, $time, $count) {
  try {
    sharedcontent_client_index_synchronize($connection, $time, 0, $count);
  }
  catch (SharedContentSynchronizeException $e) {
    $message = $e->getMessage() . PHP_EOL . print_r($e->getArguments(), TRUE);
    drush_log($message, 'error');
  }
}

/**
 * Drush command to synchronize the index.
 */
function drush_sharedcontent_client_connection_create($endpoint, $username = '', $password = '', $name = NULL, $machine_name = NULL) {
  $pattern = '/[^a-z0-9_]+/';

  if (is_null($name)) {
    list(, $name) = explode('://', $endpoint);
  }

  if (is_null($machine_name)) {
    $machine_name = preg_replace($pattern, '_', drupal_strtolower($name));
  }
  elseif (preg_match($pattern, $machine_name)) {
    echo dt("Machine_name can only contain lowercase charactes numbers and _.\n");
    return;
  }

  if (!is_null(services_client_connection_load($machine_name))) {
    echo dt("A connection with the machine name %machine_name allready exists.\n", array('%machine_name' => $machine_name));
    return;
  }

  $connection = new stdClass();
  $connection->disabled = FALSE;
  $connection->api_version = 1;
  $connection->name = $machine_name;
  $connection->admin_title = $name;
  $connection->admin_description = '';
  $connection->version = 3;
  $connection->endpoint = $endpoint;
  $connection->config = array(
    'auth' => array(
      'plugin' => '',
    ),
    'server' => array(
      'plugin' => 'ServicesClientConnectionRestServer',
      'config' => array(
        'request_formatter' => 'json',
        'response_parser' => 'json',
      ),
    ),
    'request' => array(
      'plugin' => 'ServicesClientConnectionCurlRequest',
      'config' => array(),
    ),
  );

  if (!empty($username)) {
    $connection->config['auth'] = array(
      'plugin' => 'ServicesClientConnectionBasicAuth',
      'config' => array(
        'username' => $username,
        'password' => $password,
      ),
    );
  }

  services_client_connection_save($connection);

  $endpoint_connections = variable_get('sharedcontent_endpoint_connections', array());
  $endpoint_connections[] = $machine_name;
  variable_set('sharedcontent_endpoint_connections', $endpoint_connections);
}


/**
 * Add or remove the default sharedcontent_reference field from and to entities.
 */
function drush_sharedcontent_client_manage_linkables($action, $bundle, $entity_type = 'node') {

  if (!in_array($action, array('add', 'remove'))) {
    print dt("Invalid action. Use either add or remove.\n");
    return;
  }

  $field_name = sharedcontent_client_get_default_shared_content_field_name();
  $instance = field_info_instance($entity_type, $field_name, $bundle);

  $info = entity_get_info($entity_type);

  if (empty($info)) {
    print dt("Invalid entity type %type given.\n", array('%type' => $entity_type));
    return;
  }

  if (!array_key_exists($bundle, $info['bundles'])) {
    print dt("Invalid bundle %bundle given.\n", array('%bundle' => $bundle));
    return;
  }

  switch ($action) {
    case 'add':
      if (empty($instance)) {
        module_load_include('inc', 'sharedcontent_client', 'includes/sharedcontent_client.admin');
        sharedcontent_client_attach_shared_content_field($field_name, $entity_type, $bundle);
      }
      break;
    case 'remove':
      print dt("This will delete the default sharedcotnent_reference field from %entity %bundle\n", array(
        '%entity' => $entity_type,
        '%bundle' => $bundle
      ));
      print dt("All data will be lost!\n");
      if (drush_confirm(dt('Do whant to continue'))) {
        field_delete_instance($instance);
      }
      break;
  }
}
