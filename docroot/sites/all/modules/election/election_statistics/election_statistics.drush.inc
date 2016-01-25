<?php
/**
 * @file
 * Drush commands for the Election Statistics module.
 */

/**
 * Implements hook_drush_command().
 */
function election_statistics_drush_command() {
  $items = array();
  $items['election-statistics'] = array(
    'description' => 'Fetch some election statistics. The report is printed to STDOUT.',
    'arguments' => array(
      'election_id' => 'The ID of the election (required).',
    ),
    'options' => array(
      'posts' => 'Display results for the posts in this election too.',
    ),
    'required-arguments' => 1,
    'aliases' => array('estats'),
    'examples' => array(
      'drush estats 123' => 'Get statistics for the election with ID 123.',
    ),
  );
  return $items;
}

/**
 * Get and print voting statistics for an election, via Drush.
 *
 * @param int $election_id
 *   The ID of the election.
 */
function drush_election_statistics($election_id) {
  // Load the election.
  $election = election_load($election_id);
  if (!$election) {
    return drush_set_error(dt('There are no elections with the ID @id.', array('@id' => $election_id)));
  }
  // Get the statistics.
  $totals = election_statistics_get_totals($election);
  // Display statistics for the whole election.
  _election_statistics_drush_print($totals, $election->title);
  // Display statistics for individual posts if the --posts option is set.
  if (drush_get_option('posts', FALSE)) {
    $posts = election_post_load_by_election($election);
    $post_totals = election_statistics_get_post_totals($election, $posts);
    foreach ($posts as $post_id => $post) {
      _election_statistics_drush_print((array) $post_totals[$post_id], $post->title);
    }
  }
}

/**
 * Print election voting statistics to drush output.
 *
 * @param array $totals
 *   An associative array of election statistics.
 * @param string $heading
 *   An optional heading for the statistics.
 */
function _election_statistics_drush_print(array $totals, $heading = NULL) {
  $table = array();
  $table[] = array(dt('Voters'), $totals['voters']);
  $table[] = array(dt('Ballots'), $totals['ballots']);
  $table[] = array(dt('Votes'), $totals['votes']);
  $table[] = array(dt('Abstentions'), $totals['abstentions']);
  if ($heading !== NULL) {
    drush_print($heading);
  }
  drush_print_table($table);
}
