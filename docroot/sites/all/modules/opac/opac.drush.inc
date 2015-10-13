<?php
/**
 * @file
 * drush command for harvesting and deleting biblios.
 */

/**
 * Implements hook_drush_help().
 */
function opac_drush_help($section) {
  $return = '';
  switch ($section) {
    case 'drush:opac':
      $return = dt('drush command for OPAC module.');
    case 'drush:opac-harvest':
      $return = dt('Import a set of records into OPAC module.');
  }
  return $return;
}

/**
 * Implements hook_drush_command().
 */
function opac_drush_command() {
  $items = array();

  $items['opac-harvest'] = array(
    'callback'    => 'opac_drush_harvest',
    'description' => dt('Import a set of records into OPAC module.'),
    'aliases'     => array('harvest'),
    'options' => array(
      'servers' => 'A comma-separated list of servers machine names for which to harvest.',
      'batch-size' => 'Number of biblios harvested in each batch process. Default value is harvest_batch_size variable or 1000',
      'create-only' => 'Tells drush to only create nodes. Existing nodes will not be updated.',
      'from' => 'The first (server) biblio number to harvest.',
      'to' => 'The last (server) biblio number to harvest.',
      'file' => 'You must give the absolute path of a file. Allows to specify a file containing biblio ids to harvest.',
      'print-id' => 'Print each biblio id',
    ),
  );
  $items['opac-purge'] = array(
    'callback'    => 'opac_drush_purge_nodes',
    'description' => dt('Delete biblio from the database.'),
    'options' => array(
      'servers' => 'A comma-separated list of servers for which to delete nodes.',
      'ids' => 'A comma-separated list of biblio (server) ids to delete.',
      'confirm' => 'Confirm deletion. If not, a list of node that match condition is displayed',
    ),
  );
  $items['opac-servers-list'] = array(
    'callback'    => 'opac_servers_list',
    'description' => dt('Get the list of availables servers.'),
  );
  $items['opac-update'] = array(
    'callback'    => 'opac_update',
    'description' => dt('Update biblios.'),
    'options' => array(
      'servers' => 'A comma-separated list of servers for which to delete nodes.',
      'days' => 'Updates biblios whose date changed is oldest than number of days.',
      'limit' => 'Limit the number of biblios to be updated.',
      'order-by' => 'Order by itself. Accepted values: created, chenged.',
      'direction' => 'Direction for order-by option. Accepted values: ASC, DESC. default: ASC',
      'confirm' => 'Confirm update. If not, a list of node that match condition is displayed',
    ),
  );
  $items['opac-check'] = array(
    'callback' => 'drush_opac_check',
    'description' => dt('Check consistency between opac_records table and existing nodes'),
    'options' => array(
      'missing-node' => "Only possible value: delete. What to do when a missing node is found. 'delete': Delete the opac_records entry. Default is to do nothing and print a report.",
      'missing-record' => "Only possible value: delete. What to do when a missing opac_records entry is found. 'delete': Delete the node. Default is to do nothing and print a report.",
      'duplicate-node' => "Only possible value: keep-last-id. What to do when duplicate nodes are found. 'keep-last-id': Keep the node with the greatest id, delete the others. Default is to do nothing and print a report.",
      'duplicate-record' => "Only possible value: ask. What to do when duplicate records are found. 'ask': Ask what to do. Default is to do nothing and print a report.",
      'verbose' => "Be verbose. Print one line per inconsistency found.",
    ),
  );

  return $items;
}

/**
 * Prepare opac harvestiong.
 */
function opac_drush_harvest() {
  module_load_include('inc', 'opac', "includes/opac.harvest");

  $servers = array();
  // Get command parameters.
  $servers = drush_get_option_list('servers');
  $verbose = drush_get_option('verbose');
  if (!$verbose) {
    $verbose = drush_get_option('v');
  }
  $batch_size = drush_get_option('batch-size');

  // Check from parameter.
  $from = drush_get_option('from');
  if ($from && !is_numeric($from)) {
    exit("[Error] --from parameter must be a numeric value.\n");
  }

  // Check to parameter.
  $to = drush_get_option('to');
  if ($to && !is_numeric($to)) {
    exit("[Error] --to parameter must be a numeric value.\n");
  }
  $file = drush_get_option('file');

  if ($file && ($from || $to)) {
    exit("[Error] Using file parameter with a range (from/to) is not permitted.\n");
  }
  $print_id = drush_get_option('print-id');
  $create_only = drush_get_option('create-only');
  // Process harvesting.
  return opac_harvest($servers, $verbose, $batch_size, $from, $to, $file, $print_id, $create_only);
}

/**
 * Delete opac nodes.
 */
function opac_drush_purge_nodes() {
  module_load_include('inc', 'opac', "includes/opac.db");
  $content_types = array_keys(opac_get_content_types());
  // Get command parameters.
  $servers = drush_get_option_list('servers');
  $ids = drush_get_option_list('ids');
  $confirm = drush_get_option('confirm');

  if (count($ids) && count($servers) != 1) {
    exit("ERROR: You must specify 1 server when giving record ids to opac-purge command.\n");
  }

  // Build new query.on opac node type.
  $query = new EntityFieldQuery();
  $query->entityCondition('entity_type', 'node', '=')
    ->propertyCondition('type', $content_types, 'IN');

  $entities = $query->execute();
  // If the query matched some node ids
  // we build an array this nodes (to_delete).
  $to_delete = array();
  if (isset($entities['node']) && count($entities['node'])) {
    $to_delete = array_keys($entities['node']);
  }

  // Server and record ids are not stored in the
  // node. So we have to retrieve this information
  // in opac_records table and intersect the arrays.
  if (count($ids)) {
    $nids = array();
    foreach ($ids as $id) {
      $nid = opac_get_node_id($servers[0], $id);
      $nids[] = $nid;
    }
    $to_delete = array_intersect($to_delete, $nids);
  }
  elseif (count($servers)) {
    $records = opac_get_node_ids();
    foreach ($servers as $server) {
      $to_delete = array_intersect($to_delete, array_keys($records[$server]));
    }
  }

  // If --confirm parameters is passed in the command,
  // and there is node to delete: process db_delete.
  // Else, showing the matched node ids.
  if ($confirm && count($to_delete)) {
    $chunked_ids = array_chunk($to_delete, 100);
    foreach ($chunked_ids as $ids) {
      print (date('d-m-y H:i:s') . " >>> Nodes being deleted from " . $ids[0] . " to " . end($ids) . "\n");
      node_delete_multiple($ids);
    }
  }
  else {
    if (count($to_delete)) {
      $string = implode(", ", $to_delete);
      echo "Following nodes match the condition(s):\n\n$string\n\n";
    }
    else {
      echo "No nodes match the condition(s)\n\n";
    }
  }
}

/**
 * Display available ILS servers.
 *
 * Just allows drush users to get the available
 * ILS server and they are enable or not.
 */
function opac_servers_list() {
  module_load_include('inc', 'opac', "includes/opac.db");

  $servers = opac_server_load_multiple(FALSE);

  echo "OPAC SERVERS:\n";
  foreach ($servers as $server) {
    echo $server->serv_id . ": ";
    echo $server->serv_enabled ? "[enabled]\n" : "[disabled] cannot harvest from this server\n";
  }
}

/**
 * Prepare nodes update.
 */
function opac_update() {
  module_load_include('inc', 'opac', "includes/opac.db");
  $content_types = array_keys(opac_get_content_types());
  // Get command parameters.
  $servers = drush_get_option_list('servers');
  $confirm = drush_get_option('confirm');
  $days = drush_get_option('days');
  $limit = drush_get_option('limit');
  $order_by = drush_get_option('order-by');
  $direction = drush_get_option('direction');

  // By default, direction of ordering is set to ASC.
  if (!$direction || $direction != 'ASC' || $direction != 'DESC') {
    $direction = 'ASC';
  }
  // Only allow to order by created or changed field.
  if ($order_by && ($order_by != 'created' || $order_by != 'changed')) {
    $order_by = '';
  }

  // Build new query.on opac node type.
  $query = new EntityFieldQuery();
  $query->entityCondition('entity_type', 'node', '=')
    ->propertyCondition('type', $content_types, 'IN');

  // Calculate the max timestamp of nodes to update.
  if ($days) {
    $timestamp = time() - $days * 86400;
    $query->propertyCondition('changed', $timestamp, '<=');
  }

  if ($order_by) {
    $query->propertyOrderBy($order_by, $direction);
  }

  if ($limit) {
    $query->range(0, intval($limit));
  }

  $entities = $query->execute();
  // If the query matched some node ids
  // we build an array this nodes (to_delete).
  $to_delete = array();
  if (isset($entities['node']) && count($entities['node'])) {
    $to_update = array_keys($entities['node']);
  }

  // Node that will be updated can be filtered by ILS server.
  // Server id is not stored in the
  // node. So we have to retrieve this information
  // in opac_records table and intersect the arrays.
  if (count($servers)) {
    $records = opac_get_node_ids();
    foreach ($servers as $server) {
      $to_update = array_intersect($to_update, array_keys($records[$server]));
    }
  }

  // If --confirm parameters is passed in the command,
  // and there is node to delete: process db_delete.
  // Else, showing the matched node ids.
  if ($confirm && count($to_update)) {
    module_load_include('inc', 'opac', "includes/opac.harvest");
    opac_process_update($to_update);
  }
  else {
    if (count($to_update)) {
      $string = implode(", ", $to_update);
      echo "Following nodes match the condition(s):\n\n$string\n\n";
      echo "Number of nodes matching the condition: " . count($to_update) . "\n";
    }
    else {
      echo "No nodes match the condition(s)\n\n";
    }
  }
}

/**
 * Drush command opac-check.
 */
function drush_opac_check() {
  $missing_node = drush_get_option('missing-node');
  $missing_record = drush_get_option('missing-record');
  $duplicate_node = drush_get_option('duplicate-node');
  $duplicate_record = drush_get_option('duplicate-record');
  $verbose = drush_get_option('verbose');

  if (isset($missing_node) && $missing_node != 'delete') {
    drush_log("Illegal value for --missing-node option. Must be 'delete'.", 'error');
    return;
  }

  if (isset($missing_record) && $missing_record != 'delete') {
    drush_log("Illegal value for --missing-record option. Must be 'delete'.", 'error');
    return;
  }

  if (isset($duplicate_node) && $duplicate_node != 'keep-last-id') {
    drush_log("Illegal value for --duplicate-node option. Must be 'keep-last-id'.", 'error');
    return;
  }

  if (isset($duplicate_record) && $duplicate_record != 'ask') {
    drush_log("Illegal value for --duplicate-record option. Must be 'ask'.", 'error');
    return;
  }

  module_load_include('inc', 'opac', 'includes/opac.check');

  $missing_nodes = opac_check_missing_nodes();
  $missing_records = opac_check_missing_records();
  $duplicate_nodes = opac_check_duplicate_nodes();
  $duplicate_records = opac_check_duplicate_records();

  // Calculate counts for printing report.
  $number_of_missing_nodes_start = count($missing_nodes);
  $number_of_missing_records_start = count($missing_records);
  $number_of_duplicate_nodes_start = 0;
  foreach ($duplicate_nodes as $serv_id => $serv_records) {
    foreach ($serv_records as $recordid => $nids) {
      $number_of_duplicate_nodes_start += count($nids);
    }
  }
  $number_of_duplicate_records_start = 0;
  foreach ($duplicate_records as $nid => $records) {
    $number_of_duplicate_records_start += count($records);
  }

  if ($verbose || isset($missing_node)) {
    foreach ($missing_nodes as $nid) {
      if ($verbose) {
        print "Node $nid is missing.\n";
      }
      if (isset($missing_node) && $missing_node == 'delete') {
        module_load_include('inc', 'opac', 'includes/opac.db');
        $num_deleted = opac_delete_record($nid);
        if ($verbose) {
          print "Deleted $num_deleted record(s) for node $nid\n";
        }
      }
    }
  }

  if ($verbose || isset($missing_record)) {
    foreach ($missing_records as $nid) {
      if ($verbose) {
        print "Node $nid have no record in opac_records table.\n";
      }
      if (isset($missing_record) && $missing_record == 'delete') {
        node_delete($nid);
        if ($verbose) {
          print "Deleted node $nid\n";
        }
      }
    }
  }

  // Check again, as removals above may have reduced the number of duplicates.
  $duplicate_nodes = opac_check_duplicate_nodes();

  if ($verbose || isset($duplicate_node)) {
    foreach ($duplicate_nodes as $serv_id => $serv_records) {
      foreach ($serv_records as $recordid => $nids) {
        if ($verbose) {
          print "The following nodes point to the same server/record pair: ";
          print implode(', ', $nids) . "\n";
        }
        if (isset($duplicate_node) && $duplicate_node == 'keep-last-id') {
          sort($nids, SORT_NUMERIC);
          $nid_to_keep = array_pop($nids);
          node_delete_multiple($nids);
          if ($verbose) {
            print "Node kept: $nid_to_keep. Node(s) deleted: ";
            print implode(', ', $nids) . "\n";
          }
        }
      }
    }
  }

  // Check again, as removals above may have reduced the number of duplicates.
  $duplicate_records = opac_check_duplicate_records();

  if ($verbose || isset($duplicate_record)) {
    foreach ($duplicate_records as $nid => $records) {
      if ($verbose && !isset($duplicate_records)) {
        print "Node $nid has the following entries in opac_records: ";
        print implode(', ', array_map(function($record) {
          return $record['serv_id'] . '/' . $record['recordid'];
        }, $records)) . "\n";
      }

      if (isset($duplicate_record) && $duplicate_record == 'ask') {
        print "Node $nid has the following entries in opac_records: ";
        $i = 0;
        print implode(', ', array_map(function($record) use (&$i) {
          return "[" . $i++ . "] " . $record['serv_id'] . '/' . $record['recordid'];
        }, $records)) . "\n";

        // Ask the user which record to keep.
        do {
          print "Choose which record to keep [0-" . ($i - 1) . "] or just hit Return to do nothing.\n";
          print "> ";
          $choice = fscanf(STDIN, "%d");
          $choice = isset($choice) ? $choice[0] : NULL;
        } while (isset($choice) && ($choice < 0 || $choice > $i - 1));

        if (isset($choice)) {
          $record_to_keep = $records[$choice];
          array_splice($records, $choice, 1);

          // Delete all entries for $nid, then recreate the entry.
          opac_delete_record($nid);
          opac_add_record($nid, $record_to_keep['recordid'], $record_to_keep['serv_id']);

          if ($verbose) {
            print "Record kept: " . $record_to_keep['serv_id'] . "/" . $record_to_keep['recordid'] . ". ";
            print "Removed records: ";
            print implode(', ', array_map(function($record) {
              return $record['serv_id'] . '/' . $record['recordid'];
            }, $records)) . "\n";
          }
        }
      }
    }
  }

  // Recalculate counts after the process.
  $missing_nodes = opac_check_missing_nodes();
  $missing_records = opac_check_missing_records();
  $duplicate_nodes = opac_check_duplicate_nodes();
  $duplicate_records = opac_check_duplicate_records();

  $number_of_missing_nodes_end = count($missing_nodes);
  $number_of_missing_records_end = count($missing_records);
  $number_of_duplicate_nodes_end = 0;
  foreach ($duplicate_nodes as $serv_id => $serv_records) {
    foreach ($serv_records as $recordid => $nids) {
      $number_of_duplicate_nodes_end += count($nids);
    }
  }
  $number_of_duplicate_records_end = 0;
  foreach ($duplicate_records as $nid => $records) {
    $number_of_duplicate_records_end += count($records);
  }

  print "\n";
  print "\n";
  print "Report:\n";
  print "=======\n";
  print "\n";
  print "Before script runs:\n";
  print "-------------------\n";
  print "Number of missing nodes: $number_of_missing_nodes_start\n";
  print "Number of missing records: $number_of_missing_records_start\n";
  print "Number of duplicate nodes: $number_of_duplicate_nodes_start\n";
  print "Number of duplicate records: $number_of_duplicate_records_start\n";
  print "\n";
  print "After script runs:\n";
  print "------------------\n";
  print "Number of missing nodes: $number_of_missing_nodes_end\n";
  print "Number of missing records: $number_of_missing_records_end\n";
  print "Number of duplicate nodes: $number_of_duplicate_nodes_end\n";
  print "Number of duplicate records: $number_of_duplicate_records_end\n";
}
