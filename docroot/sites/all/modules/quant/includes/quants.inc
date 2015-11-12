<?php

/**
 * Wrapper for hook_quants()
 * 
 * Provide quant objects to be rendered as charts
 * 
 * Quant object items:
 *  id          A unique ID (Ex: 'comment_creation')
 *  label       The title of the chart
 *  labelsum    Whether or not to display the sum of items in the title (Boolean)
 *  table       Which database to query from
 *  field       The database field that stores the timestamp
 *  query       Specify a complete DB query to use to fetch the items 
 *                 (Optional; Use :period to indicate timestamp. This overrides field and table)
 *  queryCustom If query is set, you must provide the same query here altered to take a 'from'
 *                and 'to' date. (Use :period0 and :period1 to specify both, with 'from' coming before 'to')
 *  dataType    Specify the type of data we're dealing with (Options: single, multiple, count)
 *  group       The database field to group items on. (Optional; Only needed if dataType= multiple)
 *  count       Which database field to count (Optional; Only needed if dataType= count)
 *  chartType   Which type of chart to use (Options: line, bar, pie)
 * 
 * @return
 *   An array of quant objects
 */
function _quant_quants() {
  $quants = array();
  
  // User Creation
  $quant = new Quant;
  $quant->id = 'user_creation';
  $quant->label = t('User creation');
  $quant->labelsum = TRUE;
  $quant->table = 'users';
  $quant->field = 'created';
  $quant->dataType = 'single';
  $quant->chartType = 'line';
  $quants[$quant->id] = $quant;
  
  // Content Creation
  $quant = new Quant;
  $quant->id = 'content_creation';
  $quant->label = t('Content creation');
  $quant->labelsum = TRUE;
  $quant->table = 'node';
  $quant->field = 'created';
  $quant->query = quant_node_query();
  $quant->queryCustom = quant_node_query(array(), TRUE);
  $quant->dataType = 'single';
  $quant->chartType = 'line';
  $quants[$quant->id] = $quant;
  
  // Comment Creation
  if (module_exists('comment')) {
    $quant = new Quant;
    $quant->id = 'comment_creation';
    $quant->label = t('Comment creation');
    $quant->labelsum = TRUE;
    $quant->table = 'comment';
    $quant->field = 'created';
    $quant->dataType = 'single';
    $quant->chartType = 'line';
    $quants[$quant->id] = $quant;
  }
  
  // Content Creation by Type
  $quant = new Quant;
  $quant->id = 'node_creation_by_type';
  $quant->label = t('Content creation by type');
  $quant->labelsum = TRUE;
  $quant->table = 'node';
  $quant->field = 'created';
  $quant->group = 'type'; // Which db field to group items by
  $quant->query = quant_node_query(array($quant->group));
  $quant->queryCustom = quant_node_query(array($quant->group), TRUE);
  $quant->dataType = 'multiple'; // See $quant->group
  $quant->chartType = 'line';
  $quants[$quant->id] = $quant;
  
  // Aggregate Content Creation by Type
  $quant = new Quant;
  $quant->id = 'node_creation_aggregate_by_type';
  $quant->label = t('Aggregate content creation');
  $quant->labelsum = TRUE;
  $quant->table = 'node';
  $quant->field = 'created';
  $quant->count = 'type'; // Which db field to we want to count
  $quant->dataType = 'count';
  $quant->chartType = 'pie';
  $quants[$quant->id] = $quant;
  
  // User Points - Uses transactions only, not point amount
  if (module_exists('userpoints')) {
    $quant = new Quant;
    $quant->id = 'user_points';
    $quant->label = t('User point transactions');
    $quant->labelsum = TRUE;
    $quant->table = 'userpoints_txn';
    $quant->field = 'time_stamp';
    $quant->dataType = 'single';
    $quant->chartType = 'line';
    $quants[$quant->id] = $quant;
  }
  
  // Organic Groups
  if (module_exists('og')) {
    // Group Joins
    $quant = new Quant;
    $quant->id = 'group_joins';
    $quant->label = t('Group joins');
    $quant->labelsum = TRUE;
    $quant->query = "
      SELECT created 
      FROM {og_membership}
      WHERE created >= :period
      AND entity_type = 'user'
      AND state = " . OG_STATE_ACTIVE . "
      ORDER BY created DESC
    "; // We can provide a custom query instead
    $quant->queryCustom = "
      SELECT created 
      FROM {og_membership}
      WHERE created >= :period0
      AND created <= :period1
      AND entity_type = 'user'
      AND state = " . OG_STATE_ACTIVE . "
      ORDER BY created DESC
    "; // We can provide a custom query instead
    $quant->table = 'og_membership';
    $quant->field = 'created';
    $quant->dataType = 'single';
    $quant->chartType = 'line';
    $quants[$quant->id] = $quant;  
    
    // Group Creation
    if (db_table_exists('og')) {
      $quant = new Quant;
      $quant->id = 'group_creation';
      $quant->label = t('Group creation');
      $quant->labelsum = TRUE; // Show the total amount of items in the chart title
      $quant->query = "
        SELECT created 
        FROM {og}
        WHERE created >= :period
        ORDER BY created DESC
      "; // We can provide a custom query instead
      $quant->queryCustom = "
        SELECT created 
        FROM {og}
        WHERE created >= :period0
        AND created <= :period1
        ORDER BY created DESC"; // We need to provide the query altered for a custom interval
      $quant->table = 'og';
      $quant->field = 'created';
      $quant->dataType = 'single';
      $quant->chartType = 'line';
      $quants[$quant->id] = $quant;
    }
  }
  
  // User Shouts
  if (module_exists('shoutbox')) {
    $quant = new Quant;
    $quant->id = 'user_shouts';
    $quant->label = t('User shouts');
    $quant->labelsum = TRUE;
    $quant->table = 'shoutbox';
    $quant->field = 'created';
    $quant->dataType = 'single';
    $quant->chartType = 'line';
    $quants[$quant->id] = $quant;
  }
  
  // Invites
  if (module_exists('invite')) {
    // Invites sent
    $quant = new Quant;
    $quant->id = 'user_invites_sent';
    $quant->label = t('Invites sent');
    $quant->labelsum = TRUE;
    $quant->table = 'invite';
    $quant->field = 'created';
    $quant->dataType = 'single';
    $quant->chartType = 'line';
    $quants[$quant->id] = $quant;
    
    // Invites accepted
    $quant = new Quant;
    $quant->id = 'user_invites_accepted';
    $quant->label = t('Invites accepted');
    $quant->labelsum = TRUE;
    $quant->query = "
      SELECT created 
      FROM {invite} 
      WHERE joined = 1 
      AND created >= :period 
      ORDER BY created DESC";
    $quant->queryCustom = "
      SELECT created 
      FROM {invite} 
      WHERE joined = 1 
      AND created >= :period0 
      AND created <= :period1 
      ORDER BY created DESC";
    $quant->table = 'invite';
    $quant->field = 'created';
    $quant->dataType = 'single';
    $quant->chartType = 'line';
    $quants[$quant->id] = $quant;
  }
  
  // Status shares (Statuses)
  if (module_exists('statuses')) {
    $quant = new Quant;
    $quant->id = 'statuses';
    $quant->label = t('Statuses shared');
    $quant->labelsum = TRUE;
    $quant->table = 'statuses';
    $quant->field = 'created';
    $quant->dataType = 'single';
    $quant->chartType = 'line';
    $quants[$quant->id] = $quant;
  }
  
  return $quants;
}

/**
 * Build a node database query
 * Avoid grabbing nodes that act as organic groups
 * 
 * @param $additional_fields
 *   An array of additional database fields to fetch other than 'created'
 * @param $custom
 *   Boolean indication whether we should return a format for query
 *   or queryCustom. (See hook_quants() documentation)
 * @return
 *   A database query statement
 */
function quant_node_query($additional_fields = array(), $custom = FALSE) {
  $query = '';
  $omit = array();
  
  // Add required field to array
  $additional_fields[] = 'created';
  
  // Omit group nodes, if any
  if (module_exists('og')) {
    foreach (node_type_get_types() as $type => $node) {
      if (og_is_group_type('node', $type)) {
        $omit[] = $type;
      }
    }
  }
  
  $query = "SELECT " . implode(', ', $additional_fields) . " FROM {node}";
  if ($custom) {
    $query .= " WHERE created >= :period0 AND created <= :period1";
  }
  else {
    $query .= " WHERE created >= :period";
  }
  if (count($omit)) {
    $query .= " AND type NOT IN ('" . implode('\', \'', $omit) . "')";
  }
  $query .= " ORDER BY created DESC";
  
  return $query;
}
