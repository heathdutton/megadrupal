<?php

/**
 * @file
 * Create and theme the rankingpage.
 */


/**
 * Theme the rankingpage.
 */
function theme_tabt_ranking($variables) {
  $teaminfo = $variables['teaminfo'];
  $ranking = $variables['ranking'];
  $output = '';

  if (isset($teaminfo->Alias)) {
    $team_seasons = tabt_get_seasons_teams($teaminfo->Alias);
    $output .= '<center>' . tabt_get_seasons_form($variables['season'], variable_get('tabt_matches_rankingpage', 'championship/ranking') . '/' . $teaminfo->Alias, $team_seasons) . '</center>';
  }

  $header = array(
    '',
    '',
    array(
        'data' => t('Matches', array(), array('context' => 'game')),
        'colspan' => 4,
        'style' => 'text-align: center'),
    '',
  );
  $rows = array();
  $rows[] = array(
    array('header' => '', 'data' => t('Rank', array(), array('context' => 'game'))),
    array('header' => '', 'data' => t('Team')),
    array('header' => '', 'data' => t('Played')),
    array('header' => '', 'data' => t('Won')),
    array('header' => '', 'data' => t('Lost')),
    array('header' => '', 'data' => t('Draw', array(), array('context' => 'game'))),
    array('header' => '', 'data' => t('Points'))
  );
  foreach ($ranking as $club) {
    $row = array(
      check_plain($club->Position),
      check_plain($club->Team),
      check_plain($club->GamesPlayed),
      check_plain($club->GamesWon),
      check_plain($club->GamesLost),
      check_plain($club->GamesDraw),
      check_plain($club->Points)
    );
    if ($club->TeamClub == variable_get('tabt_club_id', 'Vl-B000') && drupal_strtoupper(drupal_substr($club->Team, -1)) == drupal_strtoupper($teaminfo->Team)) {
      $row = array('data' => $row, 'style' => 'font-weight:bold;');
    }
    $rows[] = $row;
  }
  $output .= theme('table', array('header' => $header, 'rows' => $rows));
  $output .= _tabt_get_footer();

  return $output;
}
