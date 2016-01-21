<?php
/**
 * @file
 * Drush commands for the Election OpenSTV Integration module.
 */

/**
 * Implements hook_drush_command().
 */
function election_openstv_drush_command() {
  $items = array();
  $items['election-openstv'] = array(
    'description' => 'Counts the results of an election post using OpenSTV. The report is printed to STDOUT.',
    'arguments' => array(
      'post_id' => 'The ID of the election post (required).',
      'method' => 'The count method (machine name). See election-openstv-methods. Default: ERS97STV.',
      'output_format' => 'The output format. One of CsvReport, HtmlReport, MinimalReport, TextReport, or YamlReport. Default: TextReport.',
    ),
    'bootstrap' => DRUSH_BOOTSTRAP_DRUPAL_FULL,
    'required-arguments' => 1,
    'aliases' => array('eostv'),
    'examples' => array(
      'drush eostv 1 FTSTV CsvReport' => 'Count the results of election post 1, using the Fractional Transfer STV method, and generate a CSV report.',
    ),
  );
  $items['election-openstv-methods'] = array(
    'description' => 'List the available methods for counting with OpenSTV (see election-openstv).',
    'bootstrap' => DRUSH_BOOTSTRAP_DRUPAL_FULL,
    'aliases' => array('eostv-methods'),
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
function drush_election_openstv($post_id, $method = 'ERS97STV', $output_format = 'TextReport') {
  // Load the post.
  $post = election_post_load($post_id);
  if (!$post) {
    return drush_set_error(dt('There are no election posts with the ID @id.', array('@id' => $post_id)));
  }
  $election = $post->election;
  // Check that it is sane to count results for this post.
  if (!election_openstv_check_support($election)) {
    return drush_set_error(dt('This election type ("@type") does not support OpenSTV.', array('@type' => $election->type_info['name'])));
  }
  // Check the method type.
  $allowed_methods = _election_openstv_get_methods();
  if (!isset($allowed_methods[$method])) {
    return drush_set_error(dt('The count method "@method" was not found.', array('@method' => $method)));;
  }
  // Check the output format.
  $allowed_formats = _election_openstv_get_output_formats();
  if (!isset($allowed_formats[$output_format])) {
    return drush_set_error(dt('The output format "@format" was not found.', array('@format' => $output_format)));;
  }
  // Check that OpenSTV is installed.
  $command = _election_openstv_get_command();
  if (!_election_openstv_executable($command)) {
    return drush_set_error(dt('The configured OpenSTV command (@command) cannot be executed. Is OpenSTV installed?', array('@command' => $command)));
  }
  // Run the count.
  echo election_openstv_count_post($post, $method, $output_format);
}

/**
 * List the available counting methods.
 */
function drush_election_openstv_methods() {
  $table = array();
  $table[] = array(dt('Machine name'), dt('Name'));
  foreach (_election_openstv_get_methods() as $machine_name => $description) {
    $table[] = array($machine_name, $description);
  }
  return drush_print_table($table, TRUE);
}
