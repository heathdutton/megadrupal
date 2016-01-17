<?php

/**
 * Delete a subsite.
 * 
 * @param int $domain_id
 */
function _dominion_delete_subsite($domain_id) {
  $subsite = dominion_get_info($domain_id);
  
  // Recreate the correct structure to delete the domain.
  $domain = db_select('domain', 'd')
    ->fields('d')
    ->condition('d.domain_id', $domain_id)
    ->execute()
    ->fetchAssoc();

  // Use the main function of the domain module in order to delete.
  domain_delete($domain);

  // Delete all editors too of that domain too.
  db_delete('domain_editor')
    ->condition('domain_id', $domain_id)
    ->execute();
  
  db_delete('domain_export')
    ->condition('domain_id', $domain_id)
    ->execute();

  // Clear the cache of this domain_id
  cache_clear_all('domain_id:' . $domain_id, 'cache_dominion');

  // We are going to delete all content including its node access records.
  // There is no reason to rebuild it's node access rights.
  node_access_needs_rebuild(FALSE);
  
  // Start a batch to delete the domain contents.
  $batch = array(
    'operations' => array(),
    'title' => t('Deleting domain contents'),
    'init_message' => t('Starting'),
    'progress_message' => t('Processed @current out of @total.'),
    'error_message' => t('Error'),
    'file' => drupal_get_path('module', 'dominion') . '/dominion.delete.inc',
  );
  $nids = db_select('node_access', 'na')
    ->fields('na', array('nid'))
    ->condition('na.realm', 'domain_id')
    ->condition('na.gid', $domain_id)
    ->execute()
    ->fetchCol();
  foreach ($nids as $nid) {
    $batch['operations'][] = array('_dominion_delete_node', array($domain_id, $nid));
  }
  batch_set($batch);

  // Let other modules act.
  module_invoke_all('dominion_delete', $subsite);
}

/**
 * Delete a node from a domain.
 * This function does only delete nodes which are not assigned to other domains.
 * 
 * @param int $domain_id
 * @param int $nid
 */
function _dominion_delete_node($domain_id, $nid) {
  // Save current messages array to suppress messages from this function.
  $messages = $_SESSION['messages'];
  
  $other = db_select('node_access', 'na')
    ->fields('na', array('gid'))
    ->condition('na.nid', $nid)
    ->condition('na.realm', 'domain_id')
    ->condition('na.gid', $domain_id, '!=')
    ->execute()
    ->fetchCol();
  if ($other) {
    // Other domains are still using this node. Do not delete the node,
    // but delete the node_access record.
    db_delete('node_access')
      ->condition('nid', $nid)
      ->condition('realm', 'domain_id')
      ->condition('gid', $domain_id)
      ->execute();
  }
  else {
    // No other domains are using this node.
    node_delete($nid);
  }
  
  // Suppress messages.
  $_SESSION['messages'] = $messages;
}
