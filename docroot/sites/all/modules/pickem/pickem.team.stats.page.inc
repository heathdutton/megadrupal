<?php

/**
 * @file
 * Page callbacks and theme functions for the team stats page.
 */


function team_stats_page($league) {
  drupal_set_title(t('Team Stats'), PASS_THROUGH);

  $breadcrumb = drupal_get_breadcrumb();
  $breadcrumb[] = l($league->name, 'pickem/' . $league->lid);
  drupal_set_breadcrumb($breadcrumb);

  return theme('team_stats', array('league' => $league));
}


function theme_team_stats($variables) {
  $league = $variables['league'];
  pickem_add_sport_css($league);
  $teams = get_teams(array('gameset_id' => $league->gsid));
  foreach ($teams as $tid => $team) {
    $team->wins = 0;
    $team->losses = 0;
    $team->ties = 0;
    $team->games_played = 0;
    $team->home_wins = 0;
    $team->home_losses = 0;
    $team->vis_wins = 0;
    $team->vis_losses = 0;
    $team->win_percent = 0.0;
    $team->pts_for = 0;
    $team->pts_against = 0;
  }

  $games = get_games(array('gameset_id' => $league->gsid));
  foreach ($games as $gid => $game) {
    if ( ! isset($game->h_score) || ! isset($game->v_score) ) {
      continue;
    }

    if ( $game->h_score > $game->v_score ) {
      $teams[$game->h_id]->wins++;
      $teams[$game->h_id]->home_wins++;
      $teams[$game->v_id]->losses++;
      $teams[$game->v_id]->vis_losses++;
    }
    elseif ( $game->h_score < $game->v_score ) {
      $teams[$game->v_id]->wins++;
      $teams[$game->v_id]->vis_wins++;
      $teams[$game->h_id]->losses++;
      $teams[$game->h_id]->home_losses++;
    }
    elseif ( $game->h_score == $game->v_score ) {
      $teams[$game->v_id]->ties++;
      $teams[$game->h_id]->ties++;
    }

    $teams[$game->h_id]->games_played++;
    $teams[$game->v_id]->games_played++;

    if ( $teams[$game->h_id]->wins + $teams[$game->h_id]->losses > 0 ) {
      $teams[$game->h_id]->win_percent = $teams[$game->h_id]->wins / ( $teams[$game->h_id]->wins + $teams[$game->h_id]->losses );
    }

    if ( $teams[$game->v_id]->wins + $teams[$game->v_id]->losses > 0 ) {
      $teams[$game->v_id]->win_percent = $teams[$game->v_id]->wins / ( $teams[$game->v_id]->wins + $teams[$game->v_id]->losses );
    }

    $teams[$game->v_id]->pts_for += $game->v_score;
    $teams[$game->v_id]->pts_against += $game->h_score;

    $teams[$game->h_id]->pts_for += $game->h_score;
    $teams[$game->h_id]->pts_against += $game->v_score;

  }

  // setup sort
  $header = array(
    array('data' => t('Team'), 'field' => 'team'),
    array('data' => t('W'), 'class' => array('wins')),
    array('data' => t('L'), 'class' => array('losses')),
    array('data' => t('T'), 'class' => array('ties')),
    array('data' => t('PCT'), 'field' => 'pct', 'sort' => 'desc', 'class' => array('win-pct')),
    array('data' => t('PF'), 'field' => 'pf', 'class' => array('pts-for')),
    array('data' => t('PA'), 'field' => 'pa', 'class' => array('pts-against')),
    array('data' => t('HOME'), 'class' => array('home-record')),
    array('data' => t('ROAD'), 'class' => array('away-record'))
  );

  $ts = tablesort_get_order($header);
  $ts['sort'] = tablesort_get_sort($header);

  if ( $ts['sql'] == 'pct' ) {
    uasort($teams, "sort_team_stats_pct_" . $ts['sort']);
  }
  elseif ( $ts['sql'] == 'team' ) {
    uasort($teams, "sort_team_stats_team_" . $ts['sort']);
  }
  elseif ( $ts['sql'] == 'pf' ) {
    uasort($teams, "sort_team_stats_pf_" . $ts['sort']);
  }
  elseif ( $ts['sql'] == 'pa' ) {
    uasort($teams, "sort_team_stats_pa_" . $ts['sort']);
  }

  // render team data
  $rows = array();
  foreach ($teams as $tid => $team) {
    $team_link = '<span class="logo-team-small logo-team-small-' . $teams[$tid]->assetid . '">&nbsp;</span> ' . 
      $teams[$tid]->long_name;
    $team_link = l($team_link, 'pickem/' . $league->lid . '/chart3/' . $tid, array('html' => TRUE));

    $rows[] = array(
      array('data' => $team_link),
      array('data' => $teams[$tid]->wins),
      array('data' => $teams[$tid]->losses),
      array('data' => $teams[$tid]->ties),
      array('data' => sprintf("%.3f", $teams[$tid]->win_percent)),

      array('data' => $teams[$tid]->pts_for),
      array('data' => $teams[$tid]->pts_against),

      array('data' => $teams[$tid]->home_wins . '-' . $teams[$tid]->home_losses),
      array('data' => $teams[$tid]->vis_wins . '-' . $teams[$tid]->vis_losses)
    );
  }

  $o = theme('table', array('header' => $header, 'rows' => $rows, 'style' => '100%;', 'attributes' => array('class' => array('pickem-team-stats'))));

  return $o;
}


// bunch of sort routines for team_stats
// should really simplify this
function sort_team_stats_pct_desc($a, $b) {
  if ($a->win_percent > $b->win_percent) {
    return -1;
  }
  elseif ($a->win_percent < $b->win_percent) {
    return 1;
  }
  else {
   return 0;
  }
}
function sort_team_stats_pct_asc($a, $b) {
  if ($a->win_percent < $b->win_percent) {
    return -1;
  }
  elseif ($a->win_percent > $b->win_percent) {
    return 1;
  }
  else {
   return 0;
  }
}
function sort_team_stats_team_desc($a, $b) {
  if ($a->long_name > $b->long_name) {
    return -1;
  }
  elseif ($a->long_name < $b->long_name) {
    return 1;
  }
  else {
   return 0;
  }
}
function sort_team_stats_team_asc($a, $b) {
  if ($a->long_name < $b->long_name) {
    return -1;
  }
  elseif ($a->long_name > $b->long_name) {
    return 1;
  }
  else {
   return 0;
  }
}

function sort_team_stats_pf_desc($a, $b) {
  if ($a->pts_for > $b->pts_for) {
    return -1;
  }
  elseif ($a->pts_for < $b->pts_for) {
    return 1;
  }
  else {
   return 0;
  }
}
function sort_team_stats_pf_asc($a, $b) {
  if ($a->pts_for < $b->pts_for) {
    return -1;
  }
  elseif ($a->pts_for > $b->pts_for) {
    return 1;
  }
  else {
   return 0;
  }
}
function sort_team_stats_pa_desc($a, $b) {
  if ($a->pts_against > $b->pts_against) {
    return -1;
  }
  elseif ($a->pts_against < $b->pts_against) {
    return 1;
  }
  else {
   return 0;
  }
}
function sort_team_stats_pa_asc($a, $b) {
  if ($a->pts_against < $b->pts_against) {
    return -1;
  }
  elseif ($a->pts_against > $b->pts_against) {
    return 1;
  }
  else {
   return 0;
  }
}
