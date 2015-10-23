<?php
/**
 * @file
 * Drush DQUI Command File.
 */

/**
 * Implements hook_drush_command().
 */
function qui_drush_command() {
  $items = array();
  $items['qui-view'] = array(
    'description' => "View a DQUI report.",
    'arguments' => array(
      'machine-name' => 'Machine name of the report.',
      'format' => 'Output format. Default format is a drush table.',
    ),
    'options' => array(
      'limit' => 'Limit the results to a max count.',
      'sort-desc' => 'Sort by a column DESC.',
      'sort-asc' => 'Sort by a column ASC.',
      'groupby' => 'Group by a column',
    ),
    'examples' => array(
      'drush qui-view' => 'View available reports, and select which one to view.',
      'drush qui-view user_list' => 'View the user_list_report result in a Drush table.',
      'drush qui-view user_list json > ./output.json' => 'Save the report output to a file.',
      'drush qui-view user_list --limit=5' => 'Display the first 5 rows form the result.',
      'drush qui-view user_list --sort-desc=created' => 'Display user_list results ordered by the created date DESC',
    ),
    'bootstrap' => DRUSH_BOOTSTRAP_DRUSH,
  );
  $items['qui-list'] = array(
    'description' => "List available DQUI reports.",
    'arguments' => array(
      'tag' => 'Filter reports by tags.',
    ),
    'options' => array(),
    'examples' => array(
      'drush qui-list' => 'List all reports.',
      'drush qui-list test_tag' => 'List reports that are tagged with "test_tag"',
    ),
    'bootstrap' => DRUSH_BOOTSTRAP_DRUSH,
  );
  return $items;
}

/**
 * Implements hook_drush_help().
 */
function qui_drush_help($command) {
  switch ($command) {
    case 'drush:qui':
      return dt('View DQUI report results. You can save the output.');
  }
}

/**
 * View a report command.
 *
 * @param string $machine_name
 *   DQUI Report machine name.
 * @param string $format
 *   Report format.
 */
function drush_qui_view($machine_name = '', $format = 'drush') {
  if (!$machine_name) {
    // Display a select list.
    drush_qui_list();
    $id = drush_prompt('Select a report ID to view');
    $report = qui_report_load($id);
    if (!$report) {
      drush_set_error('Invalid report ID.');
      return;
    }
  }
  else {
    $report = qui_report_load_by_machine_name($machine_name);
  }
  _qui_drush_apply_options($report);
  if (!$report) {
    drush_set_error(dt("Could not load report @name.", array('@name' => $machine_name)));
    return;
  }
  if ($format == 'drush') {
    $results = $report->runQuery();
    if (!$results) {
      drush_set_error('No results found.');
      return;
    }
    // Prepend header row.
    $header = array_map('strtoupper', array_keys((array) $results[0]));
    array_unshift($results, $header);
    drush_print_table($results, TRUE);
  }
  else {
    $output = entity_view('qui_report', array($report), $format);
    drush_print($output['qui_report'][$report->identifier()]['formatted']['#markup']);
  }
}

/**
 * Display reports in a table view.
 *
 * @param string $tag
 *   Tag to filter by.
 */
function drush_qui_list($tag = '') {
  $reports = _qui_get_all_reports();
  $rows = array();
  // Prepend header row.
  $header = array('ID', 'NAME', 'MACHINE_NAME', 'TAGS', 'DESCRIPTION');
  foreach ($reports as $report) {
    if ($tag) {
      if (!in_array($tag, $report->tags)) {
        continue;
      }
    }
    $rows[] = array(
      'id' => $report->id,
      'name' => $report->name,
      'machine_name' => $report->machine_name,
      'tags' => implode(', ', $report->tags),
      'description' => $report->description,
    );
  }
  array_unshift($rows, $header);
  drush_print_table($rows, TRUE);
}

/**
 * Apply options to the query.
 *
 * @param QuiReport $report
 *   DQUI Report.
 */
function _qui_drush_apply_options(QuiReport &$report) {
  if ($limit = drush_get_option('limit', FALSE)) {
    $report->query->range['min'] = 0;
    $report->query->range['max'] = $limit;
  }
  if ($sort_asc = drush_get_option('sort-asc', FALSE)) {
    $report->query->order[] = array(
      'column' => $sort_asc,
      'direction' => 'ASC',
    );
  }
  if ($sort_desc = drush_get_option('sort-desc', FALSE)) {
    $report->query->order[] = array(
      'column' => $sort_desc,
      'direction' => 'DESC',
    );
  }
  if ($group = drush_get_option('groupby', FALSE)) {
    $report->query->group[] = array(
      'column' => $group,
    );
  }
}
