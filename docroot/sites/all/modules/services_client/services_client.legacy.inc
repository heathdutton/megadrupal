<?php

/**
 * @file
 * Contains functions required for automated converting old Services Client version 1 hooks
 * to events. This file is included only in drush command 'services-client-migrate-hooks'.
 */

/**
 * Detected legacy field types
 */
define('SERVICES_CLIENT_FIELD_TYPE_UNKNOWN', -1);
define('SERVICES_CLIENT_FIELD_TYPE_PROPERTY', 1);
define('SERVICES_CLIENT_FIELD_TYPE_FIELD', 2);
define('SERVICES_CLIENT_FIELD_TYPE_D6_FIELD', 3);
define('SERVICES_CLIENT_FIELD_TYPE_FIELD_MULTI', 4);

/**
 * Migrate old hook with mapping to new system.
 *
 * @param string $name
 *   Hook machine name.
 *
 * @throws Exception
 *   When migration fails Exception is thrown with description error message.
 */
function services_client_migrate_hook($name) {
  $hook = db_query("SELECT * FROM {services_client_connection_hook} WHERE name = :name", array(':name' => $name))->fetch();
  if (empty($hook)) {
    throw new Exception("Specified hook name wasn't found.");
  }

  if (!empty($hook->config)) {
    $hook->config = unserialize($hook->config);
  }
  elseif (!empty($hook->hook_mappings)) {
    $hook->hook_mappings = unserialize($hook->hook_mappings);
  }

  if (!empty($hook->hook_conditions)) {
    $hook->hook_conditions = unserialize($hook->hook_conditions);
  }

  // Define how to process legacy field map
  $field_mapping = array();
  $field_mapping_processor = function ($item) use (&$field_mapping) {
    list($remote, $local) = explode("|", trim($item));
    $field_mapping[trim($local)] = trim($remote);
  };

  // Define how to process legacy empty map
  $field_empty_mapping = array();
  $empty_mapping_processor = function ($item) use (&$field_empty_mapping) {
    list($source, $destination, $value) = explode("|", trim($item));
    $field_empty_mapping[trim($destination)] = trim($value);
  };

  $filter_remove_empty = function ($item) {
    return !empty($item);
  };

  $node_type_mapping = NULL;
  $conditions = array();

  // Newer exportable format
  if (!empty($hook->config)) {
    array_map($field_mapping_processor, explode("\n", $hook->config['mapping']['config']['field_mapping']));
    array_map($empty_mapping_processor, explode("\n", $hook->config['mapping']['config']['field_mapping_empty']));
    if ($hook->hook == 'node_save' && !empty($hook->config['mapping']['config']['node_type_mapping'])) {
      $node_type_mapping = explode("|", $hook->config['mapping']['config']['node_type_mapping']);
    }
    if (!empty($hook->config['condition']['config'])) {
      $conditions = $hook->config['condition']['config'];
    }
  }
  // Very old first format of hooks
  else {
    array_map($field_mapping_processor, explode("\n", $hook->hook_mappings['field_mapping']));
    array_map($empty_mapping_processor, explode("\n", $hook->hook_mappings['field_mapping_empty']));
    if ($hook->hook == 'node_save' && !empty($hook->hook_mappings['node_type_mapping'])) {
      $node_type_mapping = explode("|", $hook->hook_mappings['node_type_mapping']);
    }
    if (!empty($hook->hook_conditions)) {
      $conditions = $hook->hook_conditions;
    }
  }

  // Remove possible empty lines
  $field_mapping = array_filter($field_mapping, $filter_remove_empty);
  $field_empty_mapping = array_filter($field_empty_mapping, $filter_remove_empty);

  ctools_include('export');
  $event = ctools_export_crud_new('services_client_connection_event');
  $event->connection = $hook->conn_name;
  $event->name = $hook->name;
  $event->title = $hook->title;

  if ($hook->hook == 'node_save') {
    $event->event = 'save';
    $event->entity_type = 'node';
    $event->plugin = 'NodeSaveHandler';
    $handler = $event->getHandler();
    $config = $handler->getConfiguration();
    $config['uuid_resource'] = 'node';
    $config['resource'] = 'node_raw';
    if (!empty($node_type_mapping)) {
      $config['node_type'] = $node_type_mapping[1];
    }
    $handler->setConfiguration($config);

    // Node type plugin
    if (isset($conditions['node_type'])) {
      $uuid = $handler->addPlugin('condition', 'ServicesClientPropertyCondition');
      $config = array(
        'property' => 'type',
        'condition' => 'equals',
        'value' => $conditions['node_type'],
      );
      $handler->setPluginConfig('condition', $uuid, $config);
    }

    services_client_migrate_add_mapping($handler, $field_mapping, $field_empty_mapping);

    $event = $handler->getEvent();
  }
  elseif ($hook->hook == 'node_delete') {
    $event->event = 'delete';
    $event->entity_type = 'node';
    $event->plugin = 'EntityDeleteHandler';
    $handler = $event->getHandler();
    $config = $handler->getConfiguration();
    $config['uuid_resource'] = 'node';
    $config['resource'] = 'node';
    $config['queue'] = 1;
    if (!empty($node_type_mapping)) {
      $config['node_type'] = $node_type_mapping[1];
    }
    $handler->setConfiguration($config);

    // Node type plugin
    if (isset($conditions['node_type'])) {
      $uuid = $handler->addPlugin('condition', 'ServicesClientPropertyCondition');
      $config = array(
        'property' => 'type',
        'condition' => 'equals',
        'value' => $conditions['node_type'],
      );
      $handler->setPluginConfig('condition', $uuid, $config);
    }

    services_client_migrate_add_mapping($handler, $field_mapping, $field_empty_mapping);

    $event = $handler->getEvent();
  }
  elseif ($hook->hook == 'user_save') {
    $event->event = 'save';
    $event->entity_type = 'user';
    $event->plugin = 'UserSaveHandler';
    $handler = $event->getHandler();
    $config = $handler->getConfiguration();
    $config['uuid_resource'] = 'user';
    $config['resource'] = 'user_raw';
    $config['user_sync_by_name'] = TRUE;
    $config['user_map_roles'] = FALSE;
    $config['user_map_roles_map'] = array();
    if (!empty($hook->hook_mappings['user_sync_roles'])) {
      $config['user_map_roles'] = TRUE;
      $config['user_map_roles_map'] = array();
      $roles = array_flip(user_roles(TRUE));
      $roles_mapping = array();
      array_map(function ($item) use (&$roles_mapping) {
        list($local, $remote) = explode("|", trim($item));
        $roles_mapping[trim($local)] = trim($remote);
      }, explode("\n", trim($hook->hook_mappings['user_sync_roles'])));
      foreach ($roles_mapping as $local => $remote) {
        if (isset($roles[$local])) {
          $config['user_map_roles_map'][] = array(
            'local' => $roles[$local],
            'remote' => $remote,
          );
        }
      }
    }
    $handler->setConfiguration($config);

    services_client_migrate_add_mapping($handler, $field_mapping, $field_empty_mapping);

    $event = $handler->getEvent();
  }

  return $event;
}

/**
 * Create mapping plugins by old configurration.
 *
 * @param EventHandler $handler
 *   Event handler.
 *
 * @param array $field
 *   Legacy field mapping configuration.
 *
 * @param array $empty
 *   Empty mapping configuration.
 */
function services_client_migrate_add_mapping($handler, $field, $empty) {
  foreach ($field as $local => $remote) {
    $local_info = services_client_migrate_get_mapping_info($local);
    $remote_info = services_client_migrate_get_mapping_info($remote);

    $reader_config = $formatter_config = array();
    $reader = $formatter = '';

    // Add reader
    if ($local_info['type'] == SERVICES_CLIENT_FIELD_TYPE_PROPERTY) {
      $reader = 'ServicesClientPropertyReader';
      $reader_config = $local_info['data'];
    }
    elseif ($local_info['type'] == SERVICES_CLIENT_FIELD_TYPE_FIELD) {
      $reader = 'ServicesClientFieldReader';
      $reader_config = $local_info['data'] + array('all_values' => TRUE);
    }
    elseif ($local_info['type'] == SERVICES_CLIENT_FIELD_TYPE_FIELD_MULTI) {
      $reader = 'ServicesClientFieldReader';
      $property = drush_prompt("Enter property name for local field '{$local}'", 'value', TRUE);
      $reader_config = $local_info['data'] + array('all_values' => TRUE, 'property' => $property);
    }
    else {
      throw new Exception("Unknown reader type for '{$local}'.");
    }

    // Add formatter info
    if ($remote_info['type'] == SERVICES_CLIENT_FIELD_TYPE_PROPERTY) {
      $formatter = 'ServicesClientPropertyFormatter';
      $formatter_config = $remote_info['data'] + array(
        'multivalue' => 'force_single',
        'empty' => services_client_migrate_get_empty_info($remote_info, $empty),
        'default_value' => '',
      );
    }
    elseif ($remote_info['type'] == SERVICES_CLIENT_FIELD_TYPE_FIELD) {
      $formatter = 'ServicesClientFieldFormatter';
      $formatter_config = $remote_info['data'] + array(
        'multivalue' => 'all_values',
        'empty' => services_client_migrate_get_empty_info($remote_info, $empty),
        'default_value' => '',
      );
    }
    elseif ($remote_info['type'] == SERVICES_CLIENT_FIELD_TYPE_FIELD_MULTI) {
      $property = drush_prompt("Enter property name for remote field '{$remote}'", 'value', TRUE);
      $formatter = 'ServicesClientFieldFormatter';
      $formatter_config = $remote_info['data'] + array(
        'multivalue' => 'all_values',
        'empty' => services_client_migrate_get_empty_info($remote_info, $empty),
        'default_value' => '',
        'property' => $property,
      );
    }
    elseif ($remote_info['type'] == SERVICES_CLIENT_FIELD_TYPE_D6_FIELD) {
      $formatter = 'ServicesClientFieldD6Formatter';
      $formatter_config = $remote_info['data'] + array(
        'multivalue' => 'all_values',
        'empty' => services_client_migrate_get_empty_info($remote_info, $empty),
        'default_value' => '',
      );
    }
    else {
      throw new Exception("Unknown reader type for '{$local}'.");
    }

    $uuid = $handler->addPlugin('mapping', 'ServicesClientMappingPlugin');
    $config = array(
      'reader' => $reader,
      'formatter' => $formatter,
      'reader_config' => $reader_config,
      'formatter_config' => $formatter_config,
    );
    $handler->setPluginConfig('mapping', $uuid, $config);
  }
}

/**
 * Retrieve field mapping info. Detect weather type of mapping is
 * field or simple property.
 *
 * @param string $name
 *   Mapping name.
 *
 * @return array
 *   Field description.
 */
function services_client_migrate_get_mapping_info($name) {
  $match = array();
  // field_name#>und#>0#>value
  if (preg_match('~^(?P<field_name>[\w_]+)#>(?P<language>\w+)#>\d?#>(?P<property>[\w_]+)$~i', $name, $match)) {
    return array(
      'type' => SERVICES_CLIENT_FIELD_TYPE_FIELD,
      'data' => array(
        'field' => $match['field_name'],
        'language' => $match['language'],
        'property' => $match['property'],
      ),
    );
  }
  // property
  elseif (preg_match('~^(?P<property>[\w_]+)$~i', $name, $match)) {
    return array(
      'type' => SERVICES_CLIENT_FIELD_TYPE_PROPERTY,
      'data' => array(
        'property' => $match['property'],
      ),
    );
  }
  elseif (preg_match('~^(?P<field_name>[\w_]+)#>\d?#>(?P<property>[\w_]+)$~i', $name, $match)) {
    return array(
      'type' => SERVICES_CLIENT_FIELD_TYPE_D6_FIELD,
      'data' => array(
        'field' => $match['field_name'],
        'language' => $match['language'],
        'property' => $match['property'],
      ),
    );
  }
  elseif (preg_match('~^(?P<field_name>[\w_]+)#>(?P<language>\w+)$~i', $name, $match)) {
    return array(
      'type' => SERVICES_CLIENT_FIELD_TYPE_FIELD_MULTI,
      'data' => array(
        'field' => $match['field_name'],
        'language' => $match['language'],
      ),
    );
  }
  else {
    return array('type' => SERVICES_CLIENT_FIELD_TYPE_UNKNOWN);
  }
}

/**
 * Retrieve info about legacy empty mapping configuration.
 *
 * @param array $info
 *   Destination field info retrieved by services_client_migrate_get_mapping_info
 *
 * @param array $empty
 *   Legacy empty mapping configuration.
 *
 * @return string
 *   Type of empty mapping for formatter plugin.
 */
function services_client_migrate_get_empty_info($info, $empty) {
  // Remote is field type
  if ($info['type'] == SERVICES_CLIENT_FIELD_TYPE_FIELD) {
    $data = services_client_migrate_get_empty_info_config($info['data']['field'], $empty);
    // Empty value is defined in fomat "field_name#>langcode"
    if (!empty($data) && preg_match('~^(?P<field>[\w_]+)#>\w+$~i', trim($data[0]))) {
      return $data[1] == "NULL" ? 'null_field' : 'default_value';
    }
    elseif (!empty($data) && preg_match('~^(?P<field>[\w_]+)$~i', trim($data[0]))) {
      return $data[1] == "NULL" ? 'null_field' : 'default_value';
    }
    else {
      return 'no_field';
    }
  }
}

/**
 * Retrieve mapping configuration for given field from old settings.
 *
 * @param string $field
 *   Destination field name.
 *
 * @param array $empty
 *   Legacy empty mapping.
 *
 * @return array
 *   Legacy configuration if found in format array(0 => 'legacy_field', 1 => 'legacy_value')
 */
function services_client_migrate_get_empty_info_config($field, $empty) {
  foreach ($empty as $key => $value) {
    if (strpos($key, $field) === 0) {
      return array($key, $value);
    }
  }
}

/**
 * Determine whether user can be synced.
 *
 * @param $account
 *   User account
 */
function services_client_user_exclude($account) {
  $exclude = explode(',', trim(variable_get('services_client_exclude_users', '1')));
  return in_array($account->uid, $exclude);
}
