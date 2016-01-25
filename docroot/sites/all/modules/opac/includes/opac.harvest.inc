<?php
/**
 * @file
 * file containing functions related to harvesting.
 */

/**
 * Harvest records. Used with drush command opac-harvest.
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
 * @param string $file
 *   A file path containing biblio ids to request.
 *
 * @param bool $print_id
 *   A boolean telling if we print identifiers.
 */
function opac_harvest($serv_ids = array(), $verbose = FALSE, $batch_size = 0, $from = 0, $to = 0, $file = NULL, $print_id = 0, $create_only = 0) {
  module_load_include('inc', 'opac', "includes/opac.db");
  module_load_include('inc', 'opac', "includes/opac.node");

  $process_status = array(
    'verbose' => $verbose,
    'print_id' => $print_id,
    'create_only' => $create_only,
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

  $batch_size = $batch_size ? $batch_size : variable_get('harvest_batch_size', 1000);
  $process_status['opac_nodes'] = opac_get_node_ids();

  foreach ($servers as $server) {
    $process_status['serv_id'] = $server->serv_id;
    $process_status['batch_id'] = 0;

    // Make a pager depending on batch_size.
    $batches_id = opac_build_harvest_pager($server, $batch_size, $from, $to, $file);

    $connector = opac_get_instance($server);
    $wanted_fields = opac_build_fields_list($server->serv_id);

    $content_types = opac_get_content_types($server->serv_id);
    $mappings = array();
    foreach ($content_types as $machine_name => $content_type) {
      if (!opac_content_types_is_valid($machine_name)) {
        unset($content_types[$machine_name]);
      }
      // Create an instance of the connector.
      // Get mapping and fields list to request.
      $mapping = opac_build_fields_mapping($server->serv_id, $machine_name);
      $mappings[$machine_name] = $mapping;
    }

    foreach ($batches_id as $ids) {
      opac_process_import($ids, $process_status, $connector, $wanted_fields, $mappings, $content_types);
    }
    unset($connector);
  }
  return 0;
}

/**
 * Process biblios import.
 *
 * @param array $ids
 *   Biblio ids to request.
 *
 * @param array $process_status
 *   An array with all information about import state.
 *
 * @param object $connector
 *   An instance of the connector.
 *
 * @param array $wanted_fields
 *   Fields to demand to the connector.
 *
 * @param array $mappings
 *   Fields mapping.
 *
 * @param array $content_types
 *   Available content types.
 */
function opac_process_import($ids, &$process_status, $connector, $wanted_fields, $mappings, $content_types) {
  $transaction = db_transaction();
  $process_status['batch_id']++;
  if ($process_status['verbose']) {
    echo "    ** Harvesting batch " . $process_status['batch_id']
      . " (" . count($ids) . " record(s) from " . $ids[0] . " to " . end($ids) . "):\n";
  }

  // Processing biblios request.
  $bibs = $connector->getBiblios($ids, $wanted_fields);

  // If GetBiblios returned a negative value,
  // it had troubles retrieving this batch.
  if ($bibs < 0) {
    if ($bibs == -2) {
      error_log("It seems that the ILS webservice is disabled (or you don't have the permissions to access it)");
    }
    error_log("Unable to get biblios for this batch, aborting.");
    return;
  }

  $count = 0;
  foreach ($bibs as $id => $bib) {
    // Check if biblio identifiers (server id and record id)
    // already exist in drupal database (nodes).
    $nid = isset($process_status['opac_nodes'][$process_status['serv_id']][$id]) ?
    $process_status['opac_nodes'][$process_status['serv_id']][$id] : NULL;

    // Skip node update if create_only is TRUE.
    if ($nid && $process_status['create_only']) {
      echo "      - SKIPPED: record: $id => node id: $nid\n";
      continue;
    }

    // The record has been delete from ILS server.
    // So deleting corresponding node from drupal.
    if ($nid && !$bib) {
      node_delete($nid);
      echo "      - record: $id (empty record) => delete node: $nid (from " . $process_status['serv_id'] . ")\n";
      continue;
    }

    // Record is empty and does not exist in Drupal.
    if (!$bib) {
      continue;
    }

    $type = opac_determines_type($content_types, $bib);
    if ($type) {
      $bib['opac_content_type'] = $type;
    }
    elseif (count($bib)) {
      $message = "Can't determine the content type for record n° %recordid coming from %servid server.";
      $message .= " Title: %title. Switch this record";
      $variables = array(
        '%recordid' => $id,
        '%servid' => $process_status['serv_id'],
        '%title' => $bib['title'],
      );
      watchdog('OPAC HARVEST', $message, $variables, WATCHDOG_WARNING);
      continue;
    }
    $mapping = $mappings[$type];

    // Processing biblios import.
    // Making them nodes.
    $nid = opac_import_record($process_status['serv_id'], $id, $mapping, $bib, $nid);
    if ($nid) {
      $process_status['opac_nodes'][$process_status['serv_id']][$id] = $nid;
      if ($process_status['print_id']) {
        echo "      - record: $id => node id: $nid (from " . $process_status['serv_id'] . ")\n";
      }
      $count++;
    }
  }
  if ($process_status['verbose']) {
    echo "        => $count imported/updated\n\n";
  }
}

/**
 * Update nodes.
 *
 * @param array $nodes
 *   An array containing node ids.
 */
function opac_process_update($nodes) {
  module_load_include('inc', 'opac', "includes/opac.db");
  module_load_include('inc', 'opac', "includes/opac.node");

  $opac_nodes = opac_get_node_ids();
  echo "Prepare to update nodes:\n";
  $to_process = array();
  foreach ($nodes as $nid) {
    $node = node_load($nid);
    list($id, $serv_id) = opac_get_record($nid);
    $to_process[$serv_id][] = $id;
  }

  foreach ($to_process as $serv_id => $ids) {
    // Get server informations.
    // Create an instance of the connector.
    // Get mapping and fields list to request.
    $server = opac_server_load($serv_id);
    $connector = opac_get_instance($server);
    $mapping = opac_build_fields_mapping($server->serv_id);
    $wanted_fields = opac_build_fields_list($server->serv_id);
    $content_types = opac_get_content_types($server->serv_id);

    // Processing biblios request.
    $bibs = $connector->getBiblios($ids, $wanted_fields);
    foreach ($bibs as $id => $bib) {
      $type = opac_determines_type($content_types, $bib);
      if ($type) {
        $bib['opac_content_type'] = $type;
      }
      $nid = isset($opac_nodes[$serv_id][$id]) ? $opac_nodes[$serv_id][$id] : NULL;
      // Processing biblios import.
      // Making them nodes.
      $nid = opac_import_record($server->serv_id, $id, $mapping, $bib, $nid);
      if ($nid) {
        echo "node $nid updated\n";
      }
    }
  }
}

/**
 * Import record as node.
 *
 * @param string $serv_id
 *   A string containing the ILS server machine name.
 *
 * @param int $record_id
 *   An integer containing record identifier.
 *
 * @param array $mapping
 *   An array containing a mapping between record fields and node fields.
 *
 * @param array $record
 *   An array containing record fields.
 */
function opac_import_record($serv_id, $record_id, $mapping, $record = array(), $nid = NULL) {
  module_load_include('inc', 'opac', "includes/opac.node");
  module_load_include('inc', 'opac', "includes/opac.db");

  $record['#record_id'] = $record_id;
  $record['#server_id'] = $serv_id;

  // Make a record a node.
  // Third argument pass an existing node or
  // NULL value for a new one.
  $old_node = $nid ? node_load($nid) : NULL;
  $node = opac_transform_to_node(
    $record,
    $mapping,
    $old_node
  );

  if (!$node) {
    return 0;
  }

  // Save the node.
  $node = node_submit($node);
  node_save($node);

  // If it's a new node or if nid has changed, we must store
  // record and server ids for this node id.
  if (!$nid || ($nid != $node->nid)) {
    opac_add_record($node->nid, $record_id, $serv_id);
  }
  return $node->nid;
}

/**
 * Build a fields mapping for a given ILS server.
 *
 * Arrange fields in mapped or unmapped list.
 *
 * @param string $serv_id
 *   ILS server identifier.
 *
 * @param string $fieldname
 *   Field name for which we want the mapping.
 *
 * @return array
 *   Fields mapping.
 */
function opac_build_fields_mapping($serv_id, $content_type = NULL, $fieldname = NULL, $reset = FALSE) {

  $cache_name = 'opac_fields_mapping_' . $serv_id . '_' . $content_type . '_' . $fieldname;
  $sorted_fields;

  if (!$reset && ($cache = cache_get($cache_name))) {
    $sorted_fields = $cache->data;
  }
  else {
    module_load_include('inc', 'opac', "includes/opac.db");

    $content_types = opac_get_content_types($serv_id);

    $sorted_fields = array('unmapped' => array(), 'mapped' => array());
    foreach ($content_types as $ct) {
      if ($content_type && $content_type != $ct['content_type']) {
        continue;
      }
      // Get the fields attached to this to the content_type.
      $record_fields = field_info_instances('node', $ct['content_type']);

      // Create a 'mapped' and 'unmapped' list.
      // Mapped and unmapped field will not be displayed at the same place.
      foreach ($record_fields as $name => $field) {
        // Only get mapping for fieldname if this argument is not null.
        if ($fieldname && $name != $fieldname) {
          continue;
        }
        // Retrieves the ILS/connector fields mapped with $name.
        // The $name var is the node field name.
        // The $mapped_field var is the ILS/connector field name.
        $mapped_field = opac_get_mapping($serv_id, $ct['content_type'], $name);
        // The field is already mapped.
        // Adds operations link for this mapping.
        if ($mapped_field) {
          $mapped_field['operations'] = l(t('edit'),
            "admin/config/opac/server/$serv_id/mapping/" . $ct['content_type']
            . "/$name/edit")
            . "&nbsp;" . l(t('delete'),
            "admin/config/opac/server/$serv_id/mapping/" . $ct['content_type']
            . "/$name/delete");
          $sorted_fields['mapped'][] = $mapped_field;
        }
        // Remaining unmapped fields.
        else {
          $field['content_type'] = $ct['content_type'];
          $sorted_fields['unmapped'][] = $field;
        }
      }
    }
    cache_set($cache_name, $sorted_fields, 'cache');
  }
  return $sorted_fields;
}

/**
 * Build a content-types mapping for a given ILS server.
 *
 * Arrange opac_node_type fields in mapped or
 * unmapped list.
 *
 * @param string $serv_id
 *   ILS server identifier.
 *
 * @param string $fieldname
 *   Field name for which we want the mapping.
 *
 * @return array
 *   Fields mapping.
 */
function opac_build_content_types_mapping($serv_id, $fieldname = NULL) {
  module_load_include('inc', 'opac', "includes/opac.db");
  $types = node_type_get_types();
  $sorted_fields = array('unmapped' => array());
  foreach ($types as $name => $descs) {
    $sorted_fields['unmapped'][$name] = $descs->{'type'};
  }

  return $sorted_fields;
}

/**
 * Build a mapped fields list for a given ILS server.
 *
 * When calling a connector method such as getBiblios,
 * only mapped fields have to returned by the connector.
 * Others are useless.
 *
 * @param string $serv_id
 *   ILS server identifier.
 *
 * @return array
 *   Fields list.
 */
function opac_build_fields_list($serv_id) {
  module_load_include('inc', 'opac', 'includes/opac.db');
  // Get all fields.
  // They are ranged by mapped or unmmaped.
  $mappings = opac_build_fields_mapping($serv_id);
  $fields = array();
  // Keep only mapped fields.
  foreach ($mappings['mapped'] as $mapping) {
    $fields[$mapping['mapped_with']] = 1;
  }
  $content_types = opac_get_content_types($serv_id);
  foreach ($content_types as $ct) {
    $fields[$ct['matching_field']] = 1;
  }
  return array_keys($fields);
}

/**
 * Dertermine which content type use for a given record.
 *
 * @param array $content_types
 *   All opac content types with rules to match.
 *
 * @param array $bib
 *   The ILS record to test.
 *
 * @return string
 *   The content type machine name or NULL.
 */
function opac_determines_type($content_types, &$bib) {
  foreach ($content_types as $machine_name => $content_type) {
    $field = $content_type['matching_field'];
    if (array_key_exists($field, $bib)) {
      $subject = $bib[$content_type['matching_field']][0];
      $pattern = $content_type['value'];
      preg_match($pattern, $subject, $matches);
      if (count($matches)) {
        return $machine_name;
      }
    }
  }
  return NULL;
}

/**
 * Dertermine if an opac content type is valid.
 *
 * Meaning if it exists and has serverid and reocrdid fields.
 *
 * @param array $type
 *   Content type machine name.
 *
 * @return bool
 *   TRUE or FALSE.
 */
function opac_content_types_is_valid($type) {
  // Get all existing content types.
  $types = node_type_get_types();

  // Checks if content type exists.
  if (!isset($types[$type])) {
    return FALSE;
  }

  // Retrieves fields instance for $type.
  $fields = field_info_instances('node', $type);

  return TRUE;
}

/**
 * Build an sorted array containing biblio ids.
 *
 * @param object $server
 *   Opac server entity.
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
 * @param string $file
 *   A file path containing biblio ids to request.
 */
function opac_build_harvest_pager($server, $batch_size, $from, $to, $file = NULL) {
  $ids = array();
  if ($file) {
    $ids = opac_split_biblio_ids(opac_read_harvest_file($file), $batch_size);
  }
  else {
    // If $from and $to exist, then
    // they have priority. So first_bib_num and
    // last_bib_num server properties are overridden.
    if ($from) {
      $server->first_bib_num = intval($from);
    }
    if ($to) {
      $server->last_bib_num = intval($to);
    }

    // first_bib_num can't be above last_bib_num.
    if ($server->first_bib_num > $server->last_bib_num) {
      exit("[Error] first_bib_num is superior to last_bib_num\n");
    }
    $ids = opac_split_biblio_ids(range($server->first_bib_num, $server->last_bib_num, 1), $batch_size);
  }
  return $ids;
}

/**
 * Read biblio ids file to request.
 *
 * @param string $file
 *   File name.
 *
 * @return array
 *   Sorted biblio ids.
 */
function opac_read_harvest_file($file) {
  if (!file_exists($file)) {
    exit("[Error] File $file doesn't exist.\n");
  }

  $content = file($file, FILE_USE_INCLUDE_PATH);
  $ids = array();
  foreach ($content as $id) {
    $id = trim($id);
    if (!is_numeric($id)) {
      echo "[Error] \"$id\" is not a numeric value in $file. This value has been removed from the batch\n";
      continue;
    }
    $ids[] = $id;
  }
  sort($ids);
  return $ids;
}

/**
 * Split an array of biblio ids.
 *
 * @param array $ids
 *   Biblio ids.
 *
 * @param int $batch_size
 *   Size of batches.
 *
 * @return array
 *   Splitted biblio ids.
 */
function opac_split_biblio_ids($ids, $batch_size) {
  $splitted = array();
  $key = 0;
  $i = 0;
  foreach ($ids as $id) {
    $splitted[$key][$i++] = $id;
    if ($i == $batch_size) {
      $i = 0;
      $key++;
    }
  }
  return $splitted;
}
