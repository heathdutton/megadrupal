<?php

/**
 * @file
 * Drush Toolkit module.
 */

/**
 * Implements hook_drush_help().
 */
function toolkit_drush_help($section) {

  switch ($section) {
    case 'meta:toolkit:title':
      return dt('Toolkit commands');

    case 'meta:toolkit:summary':
      return dt('Lifesaver Drush commands for developers.');
  }

}

/**
 * Implements hook_drush_command().
 */
function toolkit_drush_command() {

  $items = array();

  $items['toolkit-load-node'] = array(
    'description' => 'Dumps node object to screen.',
    'callback' => 'drush_toolkit_load_node',
    'aliases' => array('load-node', 'tln', 'tnl'),
    'arguments' => array(
      'nid' => 'Node ID to load',
    ),
    'required-arguments' => 1,
    'examples' => array(
      'drush load-node 1' => 'Loads node 1 and dumps the contents of the loaded node object.',
    ),
  );

  $items['toolkit-load-node-by-ct'] = array(
    'description' => 'Count characters in the body field of all nodes for the chosen content type.',
    'callback' => 'drush_toolkit_load_node_by_ct',
    'aliases' => array('load-node-by-ct', 'tln-ct', 'tnl-ct'),
    'arguments' => array(
      'ct' => 'Content type to load (Note: Use a machine name)',
      'lang' => '(optional) Language to be counted. Default: und. Possible values: tr, en, fr',
    ),
    'required-arguments' => 1,
    'examples' => array(
      'drush load-node-by-ct article en' => 'Counts all characters of the body field in English "article" contents.',
    ),
  );

  $items['toolkit-load-user'] = array(
    'description' => 'Dumps user object to screen.',
    'callback' => 'drush_toolkit_load_user',
    'aliases' => array('load-user', 'tlu', 'tul'),
    'arguments' => array(
      'uid' => 'User ID or user name to load',
    ),
    'required-arguments' => 1,
    'examples' => array(
      'drush load-user 1' => 'Loads user ID 1 and dumps the contents of the loaded user object.',
      'drush load-user admin' => 'Loads user "admin" and dumps the contents of the loaded user object.',
    ),
  );

  return $items;

}

/**
 * Implements drush_hook_COMMAND().
 *
 * Get field details of a node entity.
 */
function drush_toolkit_load_node($nid = NULL) {

  if (empty($nid) || !ctype_digit($nid)) {
    // drush_set_error always returns false.
    return drush_set_error('error', dt('@nid is not a valid node ID', array('@nid' => $nid)));
  }

  // Load node by node ID value.
  $node = node_load($nid);

  if (!empty($node)) {
    // Print the output.
    drush_print_r($node);
  }
  else {
    return drush_set_error('error', dt('Node ID @nid is not available', array('@nid' => $nid)));
  }

}

/**
 * Implements drush_hook_COMMAND().
 *
 * Get character count of the body field for selected content types.
 */
function drush_toolkit_load_node_by_ct($ct = NULL, $lang = 'und') {

  if (empty($ct)) {
    // drush_set_error always returns false.
    return drush_set_error('error', dt('No content type was selected.'));
  }

  // Initialize counters.
  $count_published = 0;
  $count_unpublished = 0;
  $length_published = 0;
  $length_published_notags = 0;
  $length_unpublished = 0;
  $length_unpublished_notags = 0;

  // Load all nodes by content type.
  $nodes = node_load_multiple(array(), array('type' => $ct));

  // If there are no nodes, put a warning and exit.
  if (empty($nodes)) {
    // drush_set_error always returns false.
    return drush_set_error('warning', dt('No nodes are present in @content_type', array('@content_type' => $ct)));
  }

  // Print some informative messages about language.
  drush_print(dt('Processing language "@lang"', array('@lang' => $lang)));

  // Loop through all loaded nodes.
  foreach ($nodes as $n) {

    // Choose published nodes first and unpublished ones last.
    if ($n->status == 1) {
      // Increase published nodes counter.
      $count_published++;
      // Measure the length of body.
      $length_published += strlen($n->body[$lang][0]['value']);
      // It is good to measure with no html tags.
      $notags_body = strip_tags($n->body[$lang][0]['value']);
      // Measure the length of body with no html tags.
      $length_published_notags += strlen($notags_body);
    }
    else {
      // Increase unpublished nodes counter.
      $count_unpublished++;
      // Measure the length of body.
      $length_unpublished = $length_unpublished + strlen($n->body[$lang][0]['value']);
      // It is good to measure with no html tags.
      $notags_body = strip_tags($n->body[$lang][0]['value']);
      // Measure the length of body with no html tags.
      $length_unpublished_notags += strlen($notags_body);
    }
  }

  // Print the findings.
  drush_print(dt('Total @content_type published: @count', array(
    '@content_type' => $ct,
    '@count' => (int) $count_published,
  )));
  drush_print(dt('Total @content_type unpublished: @count', array(
    '@content_type' => $ct,
    '@count' => (int) $count_unpublished,
  )));
  drush_print(dt('Total characters (published with tags): @length', array('@length' => $length_published)));
  drush_print(dt('Total characters (published with no tags): @length', array('@length' => $length_published_notags)));
  drush_print(dt('Total characters (unpublished with tags): @length', array('@length' => $length_unpublished)));
  drush_print(dt('Total characters (unpublished with no tags): @length', array('@length' => $length_unpublished_notags)));

}

/**
 * Implements drush_hook_COMMAND().
 *
 * Get field details of a user entity.
 */
function drush_toolkit_load_user($uid = NULL) {

  if (empty($uid)) {
    // drush_set_error always returns false.
    return drush_set_error('error', dt('Specify User ID or user name to load'));
  }

  if (ctype_digit($uid) && $uid > 0) {
    // Load user by user ID value.
    $user = user_load($uid);
  }
  else {
    // Load user by user name.
    $user = user_load_by_name($uid);
  }

  if (!empty($user)) {
    // Print the output.
    drush_print_r($user);
  }
  else {
    return drush_set_error('error', dt('User ID @uid is not available', array('@uid' => $uid)));
  }

}
