<?php
/**
 * @file
 * Create the calendarpage.
 */

/**
 * Create the calendarpage for a team.
 *
 * This page can be formatted with theme_tabt_matchespage().
 * The rankingarray can be accessed through $matches.
 *
 * @param team
 *   The team to get the calendar for.
 * @param season
 *   The season currently being viewed.
 */
function tabt_matches_matchespage($team, $season = NULL) {
  // We only show the matches for a valid team.
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
    variable_get('tabt_matches_matchespage_title', 'Calendar @club @team: @division - @season'),
    array(
      '@club' => variable_get('tabt_club_name', 'Team'),
      '@team' => tabt_get_teamname($team, $season),
      '@division' => $teaminfo->DivisionName,
      '@season' => tabt_get_seasonname($season)
    )
  );
  drupal_set_title($title);

  // Get all matches
  $matches = tabt_matches_get_matches($team, $season);

  return array(
    '#theme'     => 'tabt_calendar',
    '#matches'   => $matches,
    '#team'      => $team,
    '#teaminfo'  => $teaminfo,
    '#season' => $season
  );
}
