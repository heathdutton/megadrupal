<?php
/**
 * @file
 * Create the teampage.
 */

/**
 * Create a teampage.
 *
 * This page can be formatted with theme_tabt_teams().
 * The teamsarray can be accessed through $teams.
 */
function tabt_teamspage($season = NULL) {
  $output = '';
  $season = tabt_process_seasonnumber($season);

  $teams = tabt_get_teams($season);
  if (isset($teams->TeamEntries)) {
    $result = $teams->TeamEntries;
    usort($result, 'tabt_teams_sort');
  }
  else {
    $result = array();
  }

  return array(
    '#theme'     => 'tabt_teams',
    '#teams'     => $result,
    '#season'    => $season,
  );
}

