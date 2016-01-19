<?php

/**
 * @file
 * Page callbacks and theme functions for the team power page.
 */


function team_power_page($league) {
  drupal_set_title(t('Team Regular Season vPower Rankings'), PASS_THROUGH);

  $breadcrumb = drupal_get_breadcrumb();
  $breadcrumb[] = l($league->name, 'pickem/' . $league->lid);
  drupal_set_breadcrumb($breadcrumb);

  // Calculate the power array.
  $team_stats = get_team_stats($league);

  $games = get_games(array('gameset_id' => $league->gsid), array('order_by' => 'pw.first_gamestart,g.gamestart,g.gid'));
  if (count($games) == 0) {
    return t('No games.');
  }
  foreach ($games as $gid => $game) {
    // skip unplayed games
    if ( !isset($game->h_score) || !isset($game->v_score) ) {
      continue;
    }

    // Opponents overall win percentage, not weekly win percentage.
    // This will devalue scoring against a team that was good and turned bad.
    $team_stats[0][$game->v_id]['opp_win_sum'] += $team_stats[0][$game->h_id]['win_percent'];
    $team_stats[0][$game->h_id]['opp_win_sum'] += $team_stats[0][$game->v_id]['win_percent'];

    $team_stats[0][$game->v_id]['opp_win_percentage'] = $team_stats[0][$game->v_id]['opp_win_sum'] / $team_stats[0][$game->v_id]['games_played'];
    $team_stats[0][$game->h_id]['opp_win_percentage'] = $team_stats[0][$game->h_id]['opp_win_sum'] / $team_stats[0][$game->h_id]['games_played'];

    // Scoring Factor
    if ( ($game->v_score + $game->h_score) > 0 ) {
      $team_stats[0][$game->v_id]['scoring_sum'] += $team_stats[0][$game->h_id]['win_percent'] * ($game->v_score / ($game->v_score + $game->h_score));
      $team_stats[0][$game->h_id]['scoring_sum'] += $team_stats[0][$game->v_id]['win_percent'] * ($game->h_score / ($game->h_score + $game->v_score));
    }
    $team_stats[0][$game->v_id]['scoring_factor'] = $team_stats[0][$game->v_id]['scoring_sum'] / $team_stats[0][$game->v_id]['games_played'];
    $team_stats[0][$game->h_id]['scoring_factor'] = $team_stats[0][$game->h_id]['scoring_sum'] / $team_stats[0][$game->h_id]['games_played'];

    // Streak Factor
    $team_stats[0][$game->v_id]['streak_factor'] = pickem_get_streak_factor($team_stats[0][$game->v_id]['streak']);
    $team_stats[0][$game->h_id]['streak_factor'] = pickem_get_streak_factor($team_stats[0][$game->h_id]['streak']);


    // Power Ranking

    // overall visitor team power ranking
    $team_stats[0][$game->v_id]['power'] = $team_stats[0][$game->v_id]['win_percent'] + $team_stats[0][$game->v_id]['scoring_factor'] + $team_stats[0][$game->v_id]['streak_factor'];
    // weekly visitor team power ranking
    $team_stats[$game->wid][$game->v_id]['power'] = $team_stats[0][$game->v_id]['power'];

    // overall home team power ranking
    $team_stats[0][$game->h_id]['power'] = $team_stats[0][$game->h_id]['win_percent'] + $team_stats[0][$game->h_id]['scoring_factor'] + $team_stats[0][$game->h_id]['streak_factor'];
    // weekly home team power ranking
    $team_stats[$game->wid][$game->h_id]['power'] = $team_stats[0][$game->h_id]['power'];

  }

  // Figure out power ranking for every week.
  // We'll use this later to plot power over the season.
  foreach ($team_stats as $week_number => $week_data) {
    // Sort in power order so high-low normalization works.
    uasort($team_stats[$week_number], 'pickem_sort_team_power');

    // collect high and low power ranking
    list($tid, $data) = each($team_stats[$week_number]);
    if (!isset($data['power'])) {
      $high = 0;
    }
    else {
      $high = $data['power'];
    }
    $low = NULL;
    while (list($tid, $data) = each($team_stats[$week_number])) {
      if (!isset($data['power'])) {
        $low = 0;
      }
      else {
        $low = $data['power'];
      }
    }
    $high = $high - $low;

    // subtract smallest power from all powers to zero base
    // multiply all powers by a factor of 100 / highest power
    reset($team_stats[$week_number]);
    $ord = 1;
    foreach ($team_stats[$week_number] as $tid => $data) {

      // Adjust only if power is set.
      if (isset($team_stats[$week_number][$tid]['power'])) {
        $team_stats[$week_number][$tid]['power'] = $team_stats[$week_number][$tid]['power'] - $low;

        // Adjusting won't work if you have a divide by zero error!
        if ( $high != 0 ) {
          $team_stats[$week_number][$tid]['power'] = round($team_stats[$week_number][$tid]['power'] * 100 / $high, 1);
        }
      }

      $team_stats[$week_number][$tid]['ord'] = $ord++;
    }
  }

  return theme('team_power', array('league' => $league, 'team_stats' => $team_stats));
}


function theme_team_power($variables) {
  $league = $variables['league'];
  pickem_add_sport_css($league);
  $team_stats = $variables['team_stats'];

  // Table headers.
  $header = array(
    array('data' => t('Rank')),
    array('data' => t('Team')),
    array('data' => t('WIN PCT')),
    array('data' => t('SCORING')),
    array('data' => t('STREAK')),
    array('data' => t('vPOWER')),
  );

  // Row data.
  $rows = array();
  foreach ($team_stats[0] as $tid => $data) {
    $team_link = '<span class="logo-team-small logo-team-small-' . $team_stats[0][$tid]['team']->assetid . '">&nbsp;</span>' .
      ' ' . $team_stats[0][$tid]['team']->long_name;
    $team_link = l($team_link, 'pickem/' . $league->lid . '/chart3/' . $tid, array('html' => TRUE));

    // Format columns.
    if (!isset($team_stats[0][$tid]['win_percent'])) {
      $team_stats[0][$tid]['win_percent'] = 'N/A';
    }
    else {
      $team_stats[0][$tid]['win_percent'] = sprintf("%.3f", $team_stats[0][$tid]['win_percent']);
    }
    if (!isset($team_stats[0][$tid]['scoring_factor'])) {
      $team_stats[0][$tid]['scoring_factor'] = 'N/A';
    }
    else {
      $team_stats[0][$tid]['scoring_factor'] = sprintf("%.4f", $team_stats[0][$tid]['scoring_factor']);
    }
    if (!isset($team_stats[0][$tid]['streak_factor'])) {
      $team_stats[0][$tid]['streak_factor'] = 'N/A';
    }
    else {
      $team_stats[0][$tid]['streak_factor'] = sprintf("%.2f", $team_stats[0][$tid]['streak_factor']);
    }
    if (!isset($team_stats[0][$tid]['power'])) {
      $team_stats[0][$tid]['power'] = 'N/A';
    }

    $rows[] = array(
      array('data' => $team_stats[0][$tid]['ord'], 'class' => 'rank'),
      array('data' => $team_link),
      array('data' => $team_stats[0][$tid]['win_percent']),
      array('data' => $team_stats[0][$tid]['scoring_factor']),
      array('data' => $team_stats[0][$tid]['streak_factor']),
      array('data' => $team_stats[0][$tid]['power'], 'style' => 'width:20px;'),
    );
  }


  return theme('table', array('header' => $header, 'rows' => $rows, 'attributes' => array('class' => array('pickem-power-ranking'))));
}


function pickem_sort_team_power($a, $b) {
  // Handle edge cases where power is NULL.
  if (!isset($a['power'])) {
    return 1;
  }
  if (!isset($b['power'])) {
    return -1;
  }

  // Normal ranking where both powers are set.
  if ($a['power'] > $b['power']) {
    return -1;
  }
  elseif ($a['power'] < $b['power']) {
    return 1;
  }
  else {
   return 0;
  }
}


function pickem_get_streak_factor($streak) {
  $streak_factor = 0;
  if ($streak >= 3) {
    $streak_factor = 0.09;
  }
  elseif ($streak >= 2) {
    $streak_factor = 0.07;
  }
  elseif ($streak >= 1) {
    $streak_factor = 0.05;
  }
  return $streak_factor;
}
