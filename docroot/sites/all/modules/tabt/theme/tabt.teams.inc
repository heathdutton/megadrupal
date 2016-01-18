<?php

/**
 * @file
 * Theme the teampage.
 */

/**
 * Theme the teamspage.
 */
function theme_tabt_teams($variables) {
  $teams = $variables['teams'];
  $output = '';

  $output .= '<center>' . tabt_get_seasons_form($variables['season'], variable_get('tabt_teamspage', 'teams')) . '</center>';

  if (module_exists('tabt_matches')) {
    $header = array(
      t('Team'),
      t('Division'),
      t('Links')
    );
  }
  else {
    $header = array(
      t('Team'),
      t('Division')
    );
  }

  $rows = array();

  foreach ($teams as $team) {
    $row = array();
    $row[] = tabt_get_teamname($team->TeamId, $variables['season']);
    $row[] = $team->DivisionName;

    if (module_exists('tabt_matches')) {
      $links = array();

      $team_url = tabt_get_teamurl($team->TeamId);

      if ($variables['season'] == tabt_get_current_season()) {
        $links['calendar'] = array(
          'title' => t('calendar'),
          'href' => variable_get('tabt_matches_matchespage', 'championship/calendar') . '/' . $team_url,
        );
        $links['ranking'] = array(
          'title' => t('ranking'),
          'href' => variable_get('tabt_matches_rankingpage', 'championship/ranking') . '/' . $team_url
        );
      }
      else {
        $season_name = tabt_get_seasonname($variables['season']);
        $links['calendar'] = array(
          'title' => t('calendar'),
          'href' => variable_get('tabt_matches_matchespage', 'championship/calendar') . '/' . $team_url . '/' . $season_name,
        );
        $links['ranking'] = array(
          'title' => t('ranking'),
          'href' => variable_get('tabt_matches_rankingpage', 'championship/ranking') . '/' . $team_url . '/' . $season_name
        );
      }

      $row[] = array('data' => array(
        '#theme' => 'links__tabt_teams',
        '#links' => $links,
        '#attributes' => array('class' => array('links', 'inline')),
      ));
    }
    $rows[] = $row;

  }

  $output .= theme('table', array('header' => $header, 'rows' => $rows));
  $output .= _tabt_get_footer();

  return $output;
}