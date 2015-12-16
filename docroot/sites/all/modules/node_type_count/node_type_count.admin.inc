<?php

/**
 * @file
 * Administrative page callbacks for Node Type Count module.
 */

/**
 * This code (noted in the callback above) creates the contents of the.
 *
 * Page for Node count.
 */
function node_type_count_page_node() {
  // Configure the table header columns.
  $header  = array(
    t('Content Type'),
    t('Machine Name'),
    t('Published'),
    t('UnPublished'),
  );
  // Fetch all the result.
  $results = node_type_count_status();
  // Configure the table rows.
  $rows = array();
  // Check if variable $results is clean for foreach.
  if (is_array($results) || is_object($results)) {
    // Loop through $results.
    foreach ($results as $content) {
      $rows[] = array(
        check_plain($content['title']),
        $content['machine_name'],
        $content['published'],
        $content['unpublished'],
      );
    }
  }
  // Render table.
  $render_array['maintable'] = array(
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => $rows,
  );
  return $render_array;
}

/**
 * This code (noted in the callback above) creates the.
 *
 * Contents of the page for User count.
 */
function node_type_count_page_user() {
  $query = db_select('users_roles', 'ur')
    ->fields('r', array('name'))
    ->groupBy('r.name');
  // Add the COUNT expression.
  $query->addExpression('COUNT(ur.uid)', 'ncount');
  // Add the JOIN.
  $query->join('role', 'r', 'r.rid = ur.rid');
  $results = $query->execute()->fetchAllKeyed();
  // Configure the table header columns.
  $header = array(
    t('Role Name'),
    t('Number of Users'),
  );
  // Configure the table rows.
  $rows = array();
  // Check if variable $results is clean for foreach.
  if (is_array($results) || is_object($results)) {
    // Loop through $results.
    foreach ($results as $user_role_name => $user_role_count) {
      $rows[] = array(
        check_plain($user_role_name),
        $user_role_count,
      );
    }
  }
  // Render table.
  $render_array['maintable'] = array(
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => $rows,
  );
  return $render_array;
}

/**
 * This is the helper function for node_type_count_status() to get the count.
 *
 * Of the published or unpublished content of particular content type.
 *
 * @param string $content_type
 *   Machine name of the content type.
 * @param bool $status
 *   Node status.
 *
 * @return numeric
 *   Returns the number of node published or unpublished according to the TYPE
 */
function node_type_count_published($content_type, $status = NODE_PUBLISHED) {
  $query = new EntityFieldQuery();
  // Grab nodes.
  $query->entityCondition('entity_type', 'node')
     // Filter by type.
    ->entityCondition('bundle', $content_type);

  if ($status) {
    // Filter by published.
    $query->propertyCondition('status', NODE_PUBLISHED);
  }
  else {
    // Filter by unpublished.
    $query->propertyCondition('status', NODE_NOT_PUBLISHED);
  }
  // Count.
  $query->count();
  $result = $query->execute();
  return $result;
}

/**
 * This function is used to query all content type with its title and get.
 *
 * The published and unpublishhed count for each content type.
 *
 * @return array
 *   Returns array of all content type with the machine name and title.
 *   Number of published node and number of unpublishhed node.
 */
function node_type_count_status() {
  $result_final = array();
  // Select node_type table.
  $result = db_select('node_type', 'n')
    ->fields('n', array(
      'type',
      'name',
  ))
  // Select type and name column.
    ->execute()->fetchAllKeyed();
  // Fetch result as keyed array.
  // Check if variable $results is clean for foreach.
  if (is_array($result) || is_object($result)) {
    foreach ($result as $node_type_machine_name => $content_type_title) {
      // Get the value as key and value pair.
      $result_arr['title'] = $content_type_title;
      $result_arr['machine_name'] = $node_type_machine_name;
      $result_arr['published'] = node_type_count_published(check_plain($node_type_machine_name), NODE_PUBLISHED);
      $result_arr['unpublished'] = node_type_count_published(check_plain($node_type_machine_name), NODE_NOT_PUBLISHED);
      $result_final[$node_type_machine_name] = $result_arr;
    }
  }
  return $result_final;
}
