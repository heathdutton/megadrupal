<?php
/**
 * @file
 * Drush included file for the extra_siteinfo module.
 */

/**
 * Implements hook_drush_command().
 */
function extra_siteinfo_drush_command() {
  $items = array();
  $items['nodetypecount'] = array(
    'callback' => 'extra_siteinfo_node_type_count',
    'description' => 'Drush command to get total number of content types in your site.',
    'aliases' => array('ntc'),
    'examples' => array(
      'Total Number of Content Types' => 'drush nodetypecount',
    ),
  );
  $items['nodecount'] = array(
    'callback' => 'extra_siteinfo_node_count',
    'description' => 'Drush command to get total number of nodes in your site.',
    'aliases' => array('nc'),
    'examples' => array(
      'Total Number of Nodes' => 'drush nodecount',
      'Number of "Article" type nodes' => 'drush nodecount <content-type-machine-name>',
      'Number of "Basic" & "Article" type nodes' => 'drush nodecount page article',
    ),
  );
  $items['rolecount'] = array(
    'callback' => 'extra_siteinfo_role_count',
    'description' => 'Drush command to get number of roles existing in site.',
    'aliases' => array('rc'),
    'examples' => array(
      'Total Number of Roles' => 'drush rolecount',
    ),
  );
  $items['usercount'] = array(
    'callback' => 'extra_siteinfo_user_count',
    'description' => 'Drush command to get user count based on their role.',
    'aliases' => array('uc'),
    'examples' => array(
      'Total Number of Users' => 'drush usercount',
      'Number of user of particular role' => 'drush usercount <role_id>',
    ),
  );
  return $items;
}

/**
 * Callback function for the content type count.
 */
function extra_siteinfo_node_type_count() {
  $query = db_select('node_type', 'nt');
  $query->fields('nt', array('type'));
  $result = $query->execute();
  drush_print('Total Number of Content Types : ' . $result->rowCount());
  while ($record = $result->fetchAssoc()) {
    drush_print($record["type"]);
  }
}

/**
 * Callback function for the node count.
 */
function extra_siteinfo_node_count() {
  $args = func_get_args();
  if ($args) {
    foreach ($args as $type) {
      $query = db_select('node', 'n');
      $query->fields('n', array('nid'));
      $query->condition('type', $type, '=');
      $query->groupBy('n.nid');
      $result = $query->execute();
      drush_print("Number of '" . $type . "' content type nodes : " . $result->rowCount());
    }
  }
  else {
    $query = db_select('node', 'n');
    $query->fields('n', array('nid'));
    $result = $query->execute();
    drush_print('Total Number of Nodes : ' . $result->rowCount());
  }
}

/**
 * Callback function for the role count.
 */
function extra_siteinfo_role_count() {
  $query = db_select('role', 'r');
  $query->fields('r', array('rid', 'name'));
  $result = $query->execute();
  drush_print('Total Number of Roles : ' . $result->rowCount());
  while ($record = $result->fetchAssoc()) {
    drush_print('Role Id : ' . $record["rid"] . ' Role name : ' . $record["name"]);
  }
}

/**
 * Callback function for the user count.
 */
function extra_siteinfo_user_count() {
  $args = func_get_args();
  if ($args) {
    foreach ($args as $role) {
      $query = db_select('users', 'u');
      $query->join('users_roles', 'ur', 'u.uid = ur.uid');
      $query->fields('u', array('uid'));
      $query->condition('ur.rid', $role, "=");
      $result = $query->execute();
      drush_print("Number of users with role id '" . $role . "' are : " . $result->rowCount() . " users.");
    }
  }
  else {
    $query = db_select('users', 'u');
    $query->fields('u', array('uid'));
    $result = $query->execute();
    drush_print('Total Number of Users : ' . $result->rowCount());
  }
}
