<?php
/**
 * @file
 * Drush commands for the Election Export module.
 */

/**
 * Implements hook_drush_command().
 */
function election_export_drush_command() {
  $items = array();
  $items['election-export'] = array(
    'description' => 'Exports results for an election as a ZIP archive (.zip) containing ballot (.blt) files.',
    'arguments' => array(
      'election_id' => 'The ID of the election (required).',
      'destination' => 'A writeable destination filename, relative to private://election-export/. Defaults to STDOUT.',
    ),
    'bootstrap' => DRUSH_BOOTSTRAP_DRUPAL_FULL,
    'required-arguments' => 1,
    'options' => array(
      'include-ballot-ids' => 'Ballot IDs could help to identify ballots uniquely.',
    ),
    'aliases' => array('eex'),
    'examples' => array(
      'drush eex 1 > ~/election1.zip' => 'Export all the results of election #1 to a .zip file in your home directory.',
    ),
  );
  $items['election-export-post'] = array(
    'description' => 'Exports results for an election post as a ballot file.',
    'arguments' => array(
      'post_id' => 'The ID of the election post (required).',
    ),
    'bootstrap' => DRUSH_BOOTSTRAP_DRUPAL_FULL,
    'required-arguments' => 1,
    'options' => array(
      'include-ballot-ids' => 'Ballot IDs could help to identify ballots uniquely.',
    ),
    'aliases' => array('eexp'),
    'examples' => array(
      'drush eexp 1 > ~/electionpost1.blt' => 'Export the results of election post #1 to a .blt file in your home directory.',
    ),
  );
  return $items;
}

/**
 * Export the results for an election.
 *
 * @param int $election_id
 *   The ID of the election.
 * @param string $destination
 *   The destination filename to write to (optional). If unspecified, output
 *   will be printed to stdout. This filename should have the extension '.zip'
 *   and be in a writeable directory. Relative filenames will relate to the
 *   directory 'private://election-export'.
 */
function drush_election_export($election_id, $destination = NULL) {
  $election = election_load($election_id);
  if (!$election) {
    return drush_set_error(dt('No election found with the ID "@eid".', array('@eid' => $election_id)));
  }
  if (!election_export_check_support($election)) {
    return drush_set_error(dt("This election type doesn't support exporting."));
  }
  if (!empty($destination)) {
    $dirname = dirname($destination);
    if ($dirname == '.') {
      $dirname = 'private://election-export';
      if (!file_exists($dirname)) {
        mkdir($dirname);
      }
      $destination = $dirname . '/' . $destination;
    }
    if ($dirname == $destination) {
      $destination .= '/election-export-' . $election_id . '.zip';
    }
    elseif (file_exists($destination)) {
      $destination = drupal_realpath($destination);
      return drush_set_error(dt('The file @destination already exists.', array('@destination' => $destination)));
    }
    if (!is_writable($dirname)) {
      return drush_set_error(dt('Directory not writeable: "@dir".', array('@dir' => $dirname)));
    }
    $extension = pathinfo($destination, PATHINFO_EXTENSION);
    if ($extension && $extension !== 'zip') {
      return drush_set_error(dt('The destination extension must be ".zip".'));
    }
  }
  $options = array(
    'include ballot ids' => drush_get_option('include-ballot-ids'),
  );
  // Output to STDOUT.
  if (empty($destination)) {
    $tmp_filename = election_export_election($election, NULL, $options);
    $success = readfile($tmp_filename);
    drupal_unlink($tmp_filename);
    if ($success) {
      drush_log(dt('Election @eid ("@title") successfully exported.', array(
        '@eid' => $election_id,
        '@title' => $election->title,
      )), 'ok');
    }
  }
  else {
    // Output to specified filename.
    $success = election_export_election($election, $destination, $options);
    if ($success) {
      drush_log(dt('Election @eid ("@title") successfully exported to: @destination', array(
        '@eid' => $election_id,
        '@title' => $election->title,
        '@destination' => drupal_realpath($destination),
      )), 'ok');
    }
  }
  // Log failure.
  if (!$success) {
    drush_set_error(dt('Export failed.'));
  }
}

/**
 * Export the results for an election post.
 *
 * @param int $post_id
 *   The ID of the election post.
 */
function drush_election_export_post($post_id) {
  $post = election_post_load($post_id);
  if (!$post) {
    return drush_set_error(dt('No election post found with the ID "@id".', array('@id' => $post_id)));
  }
  $election = $post->election;
  if (!election_export_check_support($election)) {
    return drush_set_error(dt("This election type doesn't support exporting."));
  }
  $options = array(
    'include ballot ids' => drush_get_option('include-ballot-ids'),
  );
  // Output directly to STDOUT.
  $success = election_export_post($post, 'php://stdout', $options);
  // Log success or failure.
  if ($success) {
    drush_log(dt('Results for @post @id ("@title") successfully exported.', array(
      '@post' => election_get_post_name($election->type),
      '@id' => $post_id,
      '@title' => $post->title,
    )), 'ok');
  }
  else {
    drush_set_error(dt('Export failed.'));
  }
}
