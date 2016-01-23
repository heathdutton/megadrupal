<?php
/**
 * @file
 * Create and theme the matchpage.
 */

/**
 * Create a matchpage.
 *
 * This page can be formatted with theme_tabt_matchpage().
 * The TabTmatch-object can be accessed through $match.
 *
 * @param id
 *   The id of the match.
 */
function tabt_matches_matchpage($id, $season = NULL) {
  $season = tabt_process_seasonnumber($season);
  // We need the matchnummer, so only allow numeric id's.
  if (!is_numeric($id)) {
    drupal_not_found();
    return;
  }

  // Get the match
  $match = tabt_matches_get_match($id, TRUE, $season);
  // See if there was something wrong with the match (like invalid id)
  // if so, show the Not Found Page.
  if ($match->err) {
    drupal_set_message($match->errinfo);
    drupal_not_found();
    return;
  }

  // Set the pagetitle and replace the tokens.
  $title = format_string(
    variable_get('tabt_matches_matchpage_title', 'Match: @hometeam - @awayteam'),
    array(
      '@hometeam' => $match->HomeTeam,
      '@awayteam' => $match->AwayTeam,
      '@week' => $match->WeekName,
      '@matchid' => $match->MatchId,
      '@score' => $match->Score
    )
  );
  drupal_set_title($title);

  return array(
    '#theme'    => 'tabt_matchpage',
    '#match'    => $match,
  );
}
