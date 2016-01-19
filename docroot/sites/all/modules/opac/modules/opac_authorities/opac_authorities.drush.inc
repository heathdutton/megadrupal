<?php
/**
 * @file
 * drush command for harvesting authorities.
 */

/**
 * Implements hook_drush_command().
 */
function opac_authorities_drush_command() {
  $items = array();
  $items['opac-auth-harvest'] = array(
    'callback' => 'opac_authorities_drush_harvest',
    'description' => dt('Import a set of authority records into Drupal'),
    'aliases' => array('auth-harvest'),
    'options' => array(
      'servers' => 'A comma-separated list of server machine names. Only those servers will be harvested.',
      'batch-size' => 'Number of authorities to harvest before committing to database.',
      'from' => 'The first authority (server) id to harvest.',
      'to' => 'The last authority (server) id to harvest.',
      'file' => 'You must give the absolute path of a file. Allows to specify a file containing auth ids to harvest.',
      'print-id' => 'Print each harvested authority id',
    ),
  );
  $items['opac-auth-purge'] = array(
    'callback'    => 'opac_authorities_drush_purge_nodes',
    'description' => dt('Delete authorities from the database.'),
    'options' => array(
      'servers' => 'A comma-separated list of servers machine names for which to delete nodes.',
      'ids' => 'A comma-separated list of authorities (server) ids to delete.',
      'confirm' => 'Confirm deletion. If not, a list of node that match condition is displayed',
    ),
  );

  return $items;
}

/**
 * Prepare opac authorities harvesting.
 */
function opac_authorities_drush_harvest() {
  module_load_include('inc', 'opac_authorities', "includes/harvest");

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

  $to = drush_get_option('to');
  $print_id = drush_get_option('print-id');
  opac_authorities_harvest($servers, $verbose, $batch_size, $from, $to, $file, $print_id);
}

/**
 * Delete authority nodes.
 */
function opac_authorities_drush_purge_nodes() {
  module_load_include('inc', 'opac', "includes/opac.db");
  $authority_types = array_keys(opac_authority_types());
  // Get command parameters.
  $servers = drush_get_option_list('servers');
  $ids = drush_get_option_list('ids');
  $confirm = drush_get_option('confirm');

  if (count($ids) && count($servers) != 1) {
    exit("ERROR: You must specify 1 server when giving record ids to opac-purge command.\n");
  }
  // Build new query.on opac node type.
  $query = new EntityFieldQuery();
  $query->entityCondition('entity_type', 'opac_authority', '=');

  $entities = $query->execute();

  // If the query matched some entities ids
  // we build an array with this entities (to_delete).
  $to_delete = array();
  if (isset($entities['opac_authority']) && count($entities['opac_authority'])) {
    $to_delete = array_keys($entities['opac_authority']);
  }

  // Server and record ids are not stored in the
  // node. So we have to retrieve this information
  // in opac_authority table and intersect the arrays.
  if (count($ids)) {
    $nids = array();
    foreach ($ids as $id) {
      $nid = opac_authorities_get_node_id($servers[0], $id);
      $nids[] = $nid;
    }
    $to_delete = array_intersect($to_delete, $nids);
  }
  elseif (count($servers)) {
    $records = opac_authorities_get_node_ids();
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
      entity_delete_multiple('opac_authority', $ids);
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
