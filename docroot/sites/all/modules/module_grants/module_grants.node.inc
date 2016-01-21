<?php

/**
 * @file
 * This file contains methods copied from node.module and modified to allow
 * ANDing of grants, which is handled a function call to module_grants_apply_node_access_grants_condition
 */


/**
 * Copied from node_access(), with everything before module_invoke_all('node_access')
 * removed (since our override occurs during module_invoke_all('node_access')). Other changes include:
 * 1. Commented out the $rights caching code, we shouldn't need cache here since node_access() is already
 *   doing the caching
 * 2. Replace OR query of module grants with a call to module_grants_get_node_access_result()
 */
function _module_grants_node_access($op, $node, $account = NULL) {
  // We grant access to the node if both of the following conditions are met:
  // - No modules say to deny access.
  // - At least one module says to grant access.
  // If no module specified either allow or deny, we fall back to the
  // node_access table.
  //$access = module_invoke_all('node_access', $node, $op, $account);
  $access = module_grants_invoke_node_access($node, $op, $account);
  if (in_array(NODE_ACCESS_DENY, $access, TRUE)) {
    //$rights[$account->uid][$cid][$op] = FALSE;
    return FALSE;
  }
  elseif (in_array(NODE_ACCESS_ALLOW, $access, TRUE)) {
    //$rights[$account->uid][$cid][$op] = TRUE;
    return TRUE;
  }

  // Check if authors can view their own unpublished nodes.
  if ($op == 'view' && !$node->status && user_access('view own unpublished content', $account) && $account->uid == $node->uid && $account->uid != 0) {
    //$rights[$account->uid][$cid][$op] = TRUE;
    return TRUE;
  }

  // If the module did not override the access rights, use those set in the
  // node_access table.
  if ($op != 'create' && $node->nid) {
    if (module_implements('node_grants')) {
      if (module_grants_is_disabled()) { // if disabled, use the old logic from node.module
        $query = db_select('node_access');
        $query->addExpression('1');
        $query->condition('grant_' . $op, 1, '>=');
        $nids = db_or()->condition('nid', $node->nid);
        if ($node->status) {
          $nids->condition('nid', 0);
        }
        $query->condition($nids);
        $query->range(0, 1);
        $grants = db_or();
        foreach (node_access_grants($op, $account) as $realm => $gids) {
          foreach ($gids as $gid) {
            $grants->condition(db_and()
                ->condition('gid', $gid)
                ->condition('realm', $realm)
            );
          }
        }
        if (count($grants) > 0) {
          $query->condition($grants);
        }
        $result =  (bool) $query
        ->execute()
        ->fetchField();
        //$rights[$account->uid][$cid][$op] = $result;
      }
      else {
        $result = module_grants_get_node_access_result($node, $op, $account);
      }
      return $result;
    }
    elseif (is_object($node) && $op == 'view' && $node->status) {
      // If no modules implement hook_node_grants(), the default behavior is to
      // allow all users to view published nodes, so reflect that here.
      //$rights[$account->uid][$cid][$op] = TRUE;
      return TRUE;
    }
  }

  return FALSE;
}

/**
 * Copied from node_access_view_all_nodes(), changes include:
 * 1. Replace OR query of module grants with a call to module_grants_get_node_access_view_all_nodes_result()
 */
function _module_grants_node_access_view_all_nodes($account = NULL) {
  global $user;
  if (!$account) {
    $account = $user;
  }

  // Statically cache results in an array keyed by $account->uid.
  $access = &drupal_static(__FUNCTION__);
  if (isset($access[$account->uid])) {
    return $access[$account->uid];
  }

  // If no modules implement the node access system, access is always TRUE.
  if (!module_implements('node_grants')) {
    $access[$account->uid] = TRUE;
  }
  else {
    /*
    $query = db_select('node_access');
    $query->addExpression('COUNT(*)');
    $query
      ->condition('nid', 0)
      ->condition('grant_view', 1, '>=');

    $grants = db_or();
    foreach (node_access_grants('view', $account) as $realm => $gids) {
      foreach ($gids as $gid) {
        $grants->condition(db_and()
            ->condition('gid', $gid)
            ->condition('realm', $realm)
        );
      }
    }
    if (count($grants) > 0 ) {
      $query->condition($grants);
    }

    $access[$account->uid] = $query
      ->execute()
      ->fetchField();
    */
    $access[$account->uid] = module_grants_get_node_access_view_all_nodes_result($account);
  }

  return $access[$account->uid];
}

/**
 * Copied from _node_query_node_access_alter(), changes include:
 * 1. Replace calls to node_access_view_all_nodes() with _module_grants_node_access_view_all_nodes()
 * 2. Replace OR query of module grants with a call to module_grants_apply_subquery_for_node_query_node_access_alter()
 */
function _module_grants_node_query_node_access_alter($query, $type) {
  global $user;

  // Read meta-data from query, if provided.
  if (!$account = $query->getMetaData('account')) {
    $account = $user;
  }
  if (!$op = $query->getMetaData('op')) {
    $op = 'view';
  }

  // If $account can bypass node access, or there are no node access modules,
  // or the operation is 'view' and the $account has a global view grant
  // (such as a view grant for node ID 0), we don't need to alter the query.
  if (user_access('bypass node access', $account)) {
    return;
  }
  if (!count(module_implements('node_grants'))) {
    return;
  }
  /*
  if ($op == 'view' && node_access_view_all_nodes($account)) {
    return;
  }
  */
  if ($op == 'view' && _module_grants_node_access_view_all_nodes($account)) {
    return;
  }

  $tables = $query->getTables();
  $base_table = $query->getMetaData('base_table');
  // If no base table is specified explicitly, search for one.
  if (!$base_table) {
    $fallback = '';
    foreach ($tables as $alias => $table_info) {
      if (!($table_info instanceof SelectQueryInterface)) {
        $table = $table_info['table'];
        // If the node table is in the query, it wins immediately.
        if ($table == 'node') {
          $base_table = $table;
          break;
        }
        // Check whether the table has a foreign key to node.nid. If it does,
        // do not run this check again as we found a base table and only node
        // can triumph that.
        if (!$base_table) {
          // The schema is cached.
          $schema = drupal_get_schema($table);
          if (isset($schema['fields']['nid'])) {
            if (isset($schema['foreign keys'])) {
              foreach ($schema['foreign keys'] as $relation) {
                if ($relation['table'] === 'node' && $relation['columns'] === array('nid' => 'nid')) {
                  $base_table = $table;
                }
              }
            }
            else {
              // At least it's a nid. A table with a field called nid is very
              // very likely to be a node.nid in a node access query.
              $fallback = $table;
            }
          }
        }
      }
    }
    // If there is nothing else, use the fallback.
    if (!$base_table) {
      if ($fallback) {
        watchdog('security', 'Your node listing query is using @fallback as a base table in a query tagged for node access. This might not be secure and might not even work. Specify foreign keys in your schema to node.nid ', array('@fallback' => $fallback), WATCHDOG_WARNING);
        $base_table = $fallback;
      }
      else {
        throw new Exception(t('Query tagged for node access but there is no nid. Add foreign keys to node.nid in schema to fix.'));
      }
    }
  }

  // Find all instances of the base table being joined -- could appear
  // more than once in the query, and could be aliased. Join each one to
  // the node_access table.
  /*
  $grants = node_access_grants($op, $account);
  if ($type == 'entity') {
    // The original query looked something like:
    // @code
    //  SELECT nid FROM sometable s
    //  INNER JOIN node_access na ON na.nid = s.nid
    //  WHERE ($node_access_conditions)
    // @endcode
    //
    // Our query will look like:
    // @code
    //  SELECT entity_type, entity_id
    //  FROM field_data_something s
    //  LEFT JOIN node_access na ON s.entity_id = na.nid
    //  WHERE (entity_type = 'node' AND $node_access_conditions) OR (entity_type <> 'node')
    // @endcode
    //
    // So instead of directly adding to the query object, we need to collect
    // all of the node access conditions in a separate db_and() object and
    // then add it to the query at the end.
    $node_conditions = db_and();
  }
  foreach ($tables as $nalias => $tableinfo) {
    $table = $tableinfo['table'];
    if (!($table instanceof SelectQueryInterface) && $table == $base_table) {
      // Set the subquery.
      $subquery = db_select('node_access', 'na')
      ->fields('na', array('nid'));

      $grant_conditions = db_or();
      // If any grant exists for the specified user, then user has access
      // to the node for the specified operation.
      foreach ($grants as $realm => $gids) {
        foreach ($gids as $gid) {
          $grant_conditions->condition(db_and()
              ->condition('na.gid', $gid)
              ->condition('na.realm', $realm)
          );
        }
      }

      // Attach conditions to the subquery for nodes.
      if (count($grant_conditions->conditions())) {
        $subquery->condition($grant_conditions);
      }
      $subquery->condition('na.grant_' . $op, 1, '>=');
      $field = 'nid';
      // Now handle entities.
      if ($type == 'entity') {
        // Set a common alias for entities.
        $base_alias = $nalias;
        $field = 'entity_id';
      }
      $subquery->where("$nalias.$field = na.nid");

      // For an entity query, attach the subquery to entity conditions.
      if ($type == 'entity') {
        $node_conditions->exists($subquery);
      }
      // Otherwise attach it to the node query itself.
      else {
        $query->exists($subquery);
      }
    }
  }

  if ($type == 'entity' && count($subquery->conditions())) {
    // All the node access conditions are only for field values belonging to
    // nodes.
    $node_conditions->condition("$base_alias.entity_type", 'node');
    $or = db_or();
    $or->condition($node_conditions);
    // If the field value belongs to a non-node entity type then this function
    // does not do anything with it.
    $or->condition("$base_alias.entity_type", 'node', '<>');
    // Add the compiled set of rules to the query.
    $query->condition($or);
  }
  */
  module_grants_apply_subquery_for_node_query_node_access_alter($query, $type, $base_table, $op, $account);
}