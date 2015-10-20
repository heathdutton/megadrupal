<?php

/**
 * @file
 * Helper API functions for AUL(Access User Lists) module.
 *
 * You can use the functions below to haldle grants in quickly way.
 * Using of the functions in this file is not required. 
 * You can use only functions from main module file. But have a look on 
 * functions below as on example.
 */

define('AUL_DEFAULT_SOURCE', 'aul');
define('AUL_CANCEL_ACTION', 'cancel');
define('AUL_DEFAULT_KEY_NAME', 'uid');

/**
 * Save AUL with permissions.
 * Provides also creation of AUL if one doesn't exist.
 * If all grants are not set. AUL access will be removed.
 * 
 * @param int $key_id
 *   Id of key entity to grant permissions for. E.g. user id, role id etc.
 * @param $nid
 *   Node id.
 * @param array $grants
 *   Array of permissions. can contain view, update, delete keys.
 * @param string $source
 *   (optional) Source of AUL data. Module name or field name. Works as namespace for AUL
 *   grants.
 * @param string $key_name
 *   (optional) Helps to formulate realm name. Could be 'uid', 'rid' etc.
 */
function aul_add_aul($key_id, $nid, $grants, $source = AUL_DEFAULT_SOURCE, $key_name = AUL_DEFAULT_KEY_NAME) {
  
  // Check access of parent entity.
  foreach ($grants as $op => $value) {

    if (!$value) {
      continue;
    }

    // Allow other modules prevent AUL grants assignment.
    $context = array(
      'key_id' => $key_id,
      'nid' => $nid,
      'grants' => $grants,
      'source' => $source,
      'op' => $op,
      'key_name' => $key_name,
      'action' => 'add',
    );
    
    // @todo: hook.
    // Let other modules react before adding grants for a user to a node.
    $cancel = module_invoke_all('aul_pre_add', $context);

    // Add user access.
    if (!in_array(AUL_CANCEL_ACTION, $cancel, TRUE)) {
      // Get AUL grant id of create new.
      if ($gid = aul_get_id_or_create($key_id, $key_name, $nid, $op, $source)) {
        aul_value_set($gid, 1);
        
        $context['gid'] = $gid;
        // Let other modules react after adding grants for a user to a node.
        module_invoke_all('aul_add', $context);
      }
    }
  }
}

/**
 * Remove AUL for some operations.
 * 
 * @param int $key_id
 *   Id of key entity to grant permissions for. E.g. user id, role id etc.
 * @param $nid
 *   Node id.
 * @param array $grants
 *   Array of permissions. can contain view, update, delete keys.
 * @param string $source
 *   Source of AUL data. Module name or field name. Works as namespace for AUL
 *   grants.
 * @param string $key_name
 *   Helps to formulate realm name. Could be 'uid', 'rid' etc.
 */
function aul_remove_aul($key_id, $nid, $grants, $source, $key_name = AUL_DEFAULT_KEY_NAME) {
  
  // Check access of parent entity.
  foreach ($grants as $op => $value) {

    if (!$value) {
      continue;
    }
    
    $conditions = array(
      'key_id' => $key_id,
      'key_name' => $key_name,
      'nid' => $nid,
      'op' => $op,
      'source' => $source,
    );

    // Get AUL grant id if exists.
    if ($gid = aul_get_id($conditions)) {
      
      // Allow other modules prevent AUL grants assignment.
      $context = array(
        'gid' => $gid,  
        'key_id' => $key_id,
        'nid' => $nid,
        'grants' => $grants,
        'source' => $source,
        'op' => $op,
        'key_name' => $key_name,
        'action' => 'remove',
      );
      // @todo: hook
      // Let other modules react before removing grants for a user to a node.
      $cancel = module_invoke_all('aul_pre_remove', $context);
      
      if (!in_array(AUL_CANCEL_ACTION, $cancel, TRUE)) {
        // Remove user access to node.
        aul_delete_aul_by_id($gid);
        // Let other modules react after removing grants for a user to a node.
        module_invoke_all('aul_remove', $context);
      }
    }
  }
}

/**
 * Returns the UID and grants of a user that are given to the user by an AUL.
 *
 * @param integer $nid
 *   Node id.
 * @param string $source
 *   (optional) Source of AUL data. Module name or field name. Works as
 *   namespace for AUL grants.
 *
 * @return array
 *   array if UIDs as key and grants as values of each array element
 */
function aul_get_uids_with_grants($nid, $source = AUL_DEFAULT_SOURCE) {
  $users = db_select('aul', 'a')
   ->fields('a', array('key_id', 'op', 'value'))
   ->condition('a.nid', $nid)
   ->condition('a.key_name', 'uid')
   ->condition('a.source', $source)
   ->execute()->fetchAll(PDO::FETCH_GROUP|PDO::FETCH_ASSOC);

  $result = array();
  if (empty($users)) {
    return $result;
  }

  foreach ($users as $uid => $user) {
    foreach ($user as $record) {
      $result[$uid][$record['op']] = $record['value'];
    }
  }

  return $result;
}

/**
 * @todo: refactore it.
 * Get collected node grants.
 * 
 * @param int $nid
 *   Node id.
 * @param string $module
 *   Module name string. User for separating grants realms into different groups
 *   and namespaces.
 * @param string $prefix
 *   Optional custom realm prefix.
 * 
 * @return array
 *   Node grants for roles.
 */
/*function aul_collect_node_grants($nid, $module = NULL, $prefix = 'uid') {
  
  $query = db_select('aul', 'a');
  $query->addField('a', 'aul_id', 'aul_id');
  $query->addField('a', 'name', 'realm');
  $query->innerJoin('aul_node', 'n', 'n.aul_id = a.aul_id');
  $query->innerJoin('aul_user', 'u', 'u.aul_id = a.aul_id');
  $query->addField('u', 'uid', $prefix);
  $query->condition('n.nid', $nid);
  $query->condition('a.name', "%_{$prefix}_%", 'LIKE');
  if($module) {
    $query->condition('a.module', $module);
  }

  // Add grant SUMs to query.
  $query->addExpression('SUM(u.grant_view)', 'grant_view');
  $query->addExpression('SUM(u.grant_update)', 'grant_update');
  $query->addExpression('SUM(u.grant_delete)', 'grant_delete');

  // GROUP BY rid.
  $query->groupBy('u.uid');
  $query->distinct();

  return $query->execute()->fetchAll();
}*/
