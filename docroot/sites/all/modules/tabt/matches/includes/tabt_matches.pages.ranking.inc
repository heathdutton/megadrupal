<?php

/**
 * @file
 * Create and theme the rankingpage.
 */

/**
 * Create the rankingpage for a team.
 *
 * This page can be formatted with theme_tabt_ranking().
 * The rankingarray can be accessed through $ranking.
 *
 * @param team
 *   The team to get the ranking for.
 * @param season
 *   The season being viewed.
 */
function tabt_matches_ranking($team, $season = NULL) {
  // We only show the ranking for a valid team.
  if (!tabt_is_valid_team(drupal_strtoupper($team), $season)) {
    drupal_not_found();
    return;
  }

  // Get the teaminfo so we can get the division.
  $teaminfo = tabt_get_teaminfo($team, $season);

  if (!$teaminfo) {
    // If we get here, the API screwed up and we didn't get a valid
    // TeamEntry-object.
    drupal_not_found();
    return;
  }
  $season = tabt_process_seasonnumber($season);
  // Set the pagetitle and replace the tokens.
  $title = format_string(
    variable_get('tabt_matches_rankingpage_title', 'Ranking @club @team: @division - @season'),
    array(
      '@club' => variable_get('tabt_club_name', 'Team'),
      '@team' => tabt_get_teamname($team, $season),
      '@division' => $teaminfo->DivisionName,
      '@season' => tabt_get_seasonname($season)
    )
  );
  drupal_set_title($title);

  // Get the ranking.
  $ranking = tabt_matches_get_ranking($teaminfo);

  return array(
      '#theme'     => 'tabt_ranking',
      'team'       => $team,
      'ranking'    => $ranking,
      '#teaminfo'  => $teaminfo,
      '#ranking'   => $ranking,
      '#season' => $season
  );
}
