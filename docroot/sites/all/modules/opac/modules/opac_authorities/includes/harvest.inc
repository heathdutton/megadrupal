<?php
/**
 * @file
 * file containing functions related to harvesting.
 */

/**
 * Harvest records. Used with drush command opac-auth-harvest.
 *
 * @param string $serv_ids
 *   A string containing the ILS server machine name.
 *
 * @param bool $verbose
 *   A boolean telling if we use verbose mode.
 *
 * @param int $batch_size
 *   A integer telling how many records to get for each request.
 *
 * @param int $from
 *   A integer containing the first record identifier to request.
 *
 * @param int $to
 *   A integer containing the last record identifier to request.
 *
 * @param bool $print_id
 *   A boolean telling if we print identifiers.
 */
function opac_authorities_harvest($serv_ids = array(), $verbose = FALSE, $batch_size = 0, $from = 0, $to = 0, $file = NULL, $print_id = 0) {
  module_load_include('inc', 'opac_authorities', 'opac_authorities.db');
  module_load_include('inc', 'opac', 'includes/opac.harvest');

  $process_status = array(
    'verbose' => $verbose,
    'print_id' => $print_id,
  );

  // entity_load needs we pass it FALSE
  // to get all entities.
  $serv_ids = count($serv_ids) ? $serv_ids : FALSE;

  // Means that if we pass a serv_id for an non enabled
  // server, it will not be take in account.
  $servers = opac_server_load_multiple($serv_ids, array('serv_enabled' => 1));

  if (!count($servers)) {
    echo "[Warning] No valid server to harvest. Did nothing\n";
  }

  $batch_size = $batch_size ? $batch_size : 1000;
  $process_status['batch_size'] = intval($batch_size);
  $process_status['opac_authorities'] = opac_authorities_get_ids_assoc();
  $from = intval($from);
  $to = intval($to);
  if ($from > $to) {
    exit("[Error] 'from' is greater than 'to'\n");
  }

  foreach ($servers as $server) {
    // Making a kind of pager.
    $process_status['serv_id'] = $server->serv_id;
    $process_status['batch_id'] = 0;

    $connector = opac_get_instance($server);
    $auth_types = opac_authorities_get_server_types($server->serv_id);

    $server_mappings = opac_authorities_get_mappings(array('servers' => $server->serv_id));
    $mappings = array();
    $wanted_fields = array();
    foreach ($server_mappings as $mapping) {
      $wanted_fields[] = $mapping['mapped_with'];
      $mappings[$mapping['authtype']][] = $mapping;
    }
    foreach ($auth_types as $authtype) {
      $wanted_fields[] = $authtype['matching_field'];
    }
    $wanted_fields = array_unique($wanted_fields);

    $batches_id = opac_build_harvest_pager($server, $batch_size, $from, $to, $file);

    foreach ($batches_id as $ids) {
      opac_authorities_process_import($ids, $process_status, $connector, $wanted_fields, $mappings, $auth_types);
    }
  }
}

/**
 * Process one batch of authority harvesting process.
 *
 * @param array $process_status
 *   Associative array that contains some informations about the process.
 *   (server identifier, current id to process, ...)
 *
 * @param object $connector
 *   Opac connector object.
 *
 * @param array $wanted_fields
 *   List of fields that will be asked to connector.
 *
 * @param array $mappings
 *   List of mappings.
 *
 * @param array $auth_types
 *   List of authority types.
 */
function opac_authorities_process_import($ids, &$process_status, $connector, $wanted_fields, $mappings, $auth_types) {
  $transaction = db_transaction();
  $process_status['batch_id']++;
  if ($process_status['verbose']) {
    echo "    ** Harvesting batch " . $process_status['batch_id']
      . " (" . count($ids) . " authorities from " . $ids[0] . " to " . end($ids) . "):\n";
  }

  // Processing authorities request.
  $auths = $connector->getAuthorities($ids, $wanted_fields);
  $count = 0;
  foreach ($auths as $id => $auth) {
    $type = opac_authorities_determines_type($auth_types, $auth);
    if ($type) {
      $auth['authtype'] = $type;
    }
    else {
      $message = "Can't determine the authority type for authority record no. %recordid coming from %servid server.";
      $variables = array(
        '%recordid' => $id,
        '%servid' => $process_status['serv_id'],
      );
      watchdog('OPAC AUTHORITIES HARVEST', $message, $variables, WATCHDOG_WARNING);
      continue;
    }
    $mapping = $mappings[$type];

    // Check if authority identifiers (server id and authority id)
    // already exist in drupal database.
    $authid = isset($process_status['opac_authorities'][$process_status['serv_id']][$id]) ?
    $process_status['opac_authorities'][$process_status['serv_id']][$id] : NULL;

    // Processing authorities import.
    // Making them authority entities.
    $authid = opac_authorities_import_record($process_status['serv_id'], $id,
      $mapping, $auth, $authid);
    if ($authid) {
      $process_status['opac_authorities'][$process_status['serv_id']][$id] = $authid;
      if ($process_status['print_id']) {
        echo "      - record: $id => authority id: $authid (from " . $process_status['serv_id'] . ")\n";
      }
      $count++;
    }
  }
  if ($process_status['verbose']) {
    echo "\t\t=> $count imported/updated\n\n";
  }
}

/**
 * Import authority record.
 *
 * @param string $serv_id
 *   Opac server identifier.
 *
 * @param string $record_id
 *   Record ILS identifier.
 *
 * @param array $mappings
 *   List of mappings.
 *
 * @param array $record
 *   Associative array that represents the authority.
 *
 * @param int $authid
 *   Authority Drupal identifier, if authority already exists.
 *
 * @return int
 *   Authority Drupal identifier.
 */
function opac_authorities_import_record($serv_id, $record_id, $mappings, $record = array(), $authid = NULL) {
  // No record means it has been deleted from ILS server.
  // So deleting corresponding entity from drupal.
  if (!$record && $authid) {
    opac_authority_delete_multiple(array($authid));
    return;
  }

  // Make a record an entity.
  // Third argument pass an existing entity or NULL value for a new one.
  $old_authority = $authid ? opac_authority_load($authid) : NULL;
  $authority = opac_authority_transform_to_entity(
    $record, $mappings, $old_authority);

  // Save the authority.
  $authority->server_id = $serv_id;
  $authority->server_authid = $record_id;
  opac_authority_save($authority);
  return entity_id('opac_authority', $authority);
}

/**
 * Transform authority record into an OpacAuthority object.
 *
 * @param array $record
 *   Associative array that represents the authority.
 *
 * @param array $mappings
 *   List of mappings.
 *
 * @param object $entity
 *   OpacAuthority object, if it already exists.
 *
 * @return object
 *   OpacAuthority object.
 */
function opac_authority_transform_to_entity($record, $mappings, $entity = NULL) {
  if (!$entity) {
    $entity = entity_create('opac_authority', array('authtype' => $record['authtype']));
  }

  if ($entity->authtype != $record['authtype']) {
    opac_authority_delete_multiple(array(entity_id('opac_authority', $entity)));
    $entity = entity_create('opac_authority', array('authtype' => $record['authtype']));
  }
  unset($record['authtype']);

  opac_authority_record_to_fields($record, $entity, $mappings);
  return $entity;
}

/**
 * Fill authority fields.
 *
 * @param array $record
 *   Associative array that represents the authority.
 *
 * @param object $entity
 *   OpacAuthority object.
 *
 * @param array $mappings
 *   List of mappings used for building authority fields.
 */
function opac_authority_record_to_fields($record, $entity, $mappings) {
  module_load_include('inc', 'opac', 'includes/helper.taxonomy');

  // Loop through all mapped fields.
  // All values in remotefield fields will be copied in field.
  foreach ($mappings as $mapping) {
    $field = $mapping['authority_field_name'];
    $remotefield = $mapping['mapped_with'];

    if ($field == 'title') {
      // Special case for title: it is not an array.
      if (isset($record[$remotefield])) {
        $entity->title = $record[$remotefield][0];
      }
      else {
        $entity->title = t("No title");
      }
      // Authority title must not exceed 255 characters.
      if (drupal_strlen($entity->title) > 255) {
        $entity->title = drupal_substr($entity->title, 0, 255);
      }
    }

    else {
      $entity->{$field}['und'] = array();
      if (isset($mapping['vocabulary_machine_name'])) {
        // A taxonomy mapping rule exists.
        // That means the authority field is related to a vocabulary.
        if (isset($record[$remotefield])) {
          foreach ($record[$remotefield] as $value) {
            // Retrieves the term id depending on the field value.
            $tid = _opac_taxonomy_get_term($mapping['vid'], $mapping['term_field'], $value);
            if ($tid) {
              $entity->{$field}['und'][] = array('tid' => $tid);

            }
            // No term match the field value. So if nomatch_rule
            // is 'create', we add a new term: The remotefield value.
            elseif ($mapping['nomatch_rule'] == 'create') {
              $tid = _opac_taxonomy_new_term($mapping['vid'], $value, $mapping['term_field']);
              $entity->{$field}['und'][] = array('tid' => $tid);
            }

            // If we did not find a taxo id, and we have a default value,
            // we use it.
            else {

              // Getting fields info, in order to check for a default value.
              $field_infos = field_info_instance('opac_authority', $field, $entity->authtype);

              // If we have a default value, we use it.
              $default_tid = $field_infos['default_value'][0]['tid'];
              if ($default_tid) {
                $entity->{$field}['und'][] = array('tid' => $default_tid);
              }
            }
          }
        }
      }
      else {
        if (isset($record[$remotefield])) {
          foreach ($record[$remotefield] as $value) {
            $entity->{$field}['und'][] = array('value' => $value);
          }
        }
      }
    }
  }
}

/**
 * Determines the type of an authority.
 *
 * The type guess depends on authority types matching rules.
 *
 * @param array $authority_types
 *   List of authority types, represented as associative arrays.
 *
 * @param array $authority
 *   Associative array that represent the authority.
 *
 * @return string
 *   Machine name of corresponding authority type.
 *   If no authority type corresponds, it returns NULL.
 */
function opac_authorities_determines_type($authority_types, $authority) {
  foreach ($authority_types as $machine_name => $authority_type) {
    $field = $authority_type['matching_field'];
    if (array_key_exists($field, $authority)) {
      $subject = $authority[$authority_type['matching_field']][0];
      $pattern = $authority_type['value'];
      preg_match($pattern, $subject, $matches);
      if (count($matches)) {
        return $machine_name;
      }
    }
  }
  return NULL;
}
