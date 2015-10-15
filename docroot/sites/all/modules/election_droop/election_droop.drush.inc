<?php
/**
 * @file
 * Drush commands for the Election Droop Integration module.
 */

/**
 * Implements hook_drush_command().
 */
function election_droop_drush_command() {
  $items = array();
  $items['election-droop'] = array(
    'description' => 'Counts the results of an election post using Droop. The report is printed to STDOUT.',
    'arguments' => array(
      'post_id' => 'The ID of the election post (required).',
      'method' => 'The count method (default: ERS97STV).',
      'output_format' => 'The output format (default: TextReport).',
    ),
    'bootstrap' => DRUSH_BOOTSTRAP_DRUPAL_FULL,
    'required-arguments' => 1,
    'aliases' => array('edroop'),
  );
  return $items;
}

/**
 * Count the results for an election post.
 *
 * @param int $post_id
 *   The ID of the election post.
 * @param string $method
 *   The count method to use.
 */
function drush_election_droop($post_id, $method = 'meek') {
  // Load the post.
  $post = election_post_load($post_id);
  if (!$post) {
    return drush_set_error(dt('There are no election posts with the ID @id.', array('@id' => $post_id)));
  }
  $election = $post->election;
  // Check that it is sane to count results for this post.
  if (!election_droop_check_support($election)) {
    return drush_set_error(dt('This election type ("@type") does not support Droop.', array('@type' => $election->type_info['name'])));
  }
  // Check the method type.
  $allowed_methods = _election_droop_get_methods();
  if (!in_array($method, $allowed_methods)) {
    return drush_set_error(dt('The count method "@method" was not found.', array('@method' => $method)));;
  }
  // Run the count.
  echo election_droop_count($post, $method);
}
