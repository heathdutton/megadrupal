<?php

/**
 * @file
 * Page callbacks for display of various charts using google chart tools.
 */


function chart_hi_low_page($league, $account = NULL) {
  $breadcrumb = drupal_get_breadcrumb();
  $breadcrumb[] = l($league->name, 'pickem/' . $league->lid);
  drupal_set_breadcrumb($breadcrumb);

  $weeks = get_totals_by_week($league);
  $rows = array();
  foreach ($weeks as $wid => $week) {
    $rows[] = array($week['name'], $week['total'] / $week['count'], $week['min'], $week['max']);
  }


  $g_chart = new stdClass;
  $g_chart->options = array(
    'width' => '100%',
    'height' => 400,
    'chartArea' => array('width' => '85%'),
    'title' => t('Weekly Points High/Low Chart'),
    'legend' => array('position' => 'top'),
    'hAxis' => array(
      'title' => t('Week'),
      'titleTextStyle' => array(
        'color' => '#000000'
      ),
      'maxAlternation' => 3,
      'showTextEvery' => 1,
    ),
    'vAxis' => array(
      'title' => t('Points'),
      'titleTextStyle' => array(
        'color' => '#000000'
      )
    )
  );

  $g_chart->visualization = 'google.visualization.LineChart';
  $g_chart->packages = array('corechart');
  $g_chart->container_id = 'chart_div';
  $g_chart->cols = array(
    array('string', t('Week')),
    array('number', t('League AVG')),
    array(array('type' => 'number', 'role' => 'interval')),
    array(array('type' => 'number', 'role' => 'interval')),
  );
  $g_chart->rows = $rows;

  $gct_obj = new pickem_google_chart_tools_helper();
  $output = $gct_obj->render($g_chart);
  $output .= '<div id="' . $g_chart->container_id . '"></div>';

  return $output;
}


function chart_team_pickability_page($league, $sort_by='win_lose') {
  $breadcrumb = drupal_get_breadcrumb();
  $breadcrumb[] = l($league->name, 'pickem/' . $league->lid);
  drupal_set_breadcrumb($breadcrumb);

  $g_chart = new stdClass;
  $g_chart->options = array(
    'width' => '100%',
    'height' => 400,
    'chartArea' => array('width' => '85%'),
    'title' => t('Team Pickability (Regular Season)'),
    'legend' => array('position' => 'top'),
    'hAxis' => array(
      'title' => 'Percent that a team is correctly picked',
      'titleTextStyle' => array(
        'color' => '#000000'
      ),
      'maxAlternation' => 3,
      'showTextEvery' => 1,
    ),
    'vAxis' => array(
      'title' => 'Percent',
      'titleTextStyle' => array(
        'color' => '#000000'
      )
    )
  );

  $teams = get_team_totals($league, $sort_by);
  $g_chart->rows = array();
  foreach ($teams as $tid => $team_data) {
    $g_chart->rows[] = array($team_data['abbrev'], (int)$team_data['pickability'], (int)$team_data['pickability_w']);
  }

  $g_chart->visualization = 'google.visualization.AreaChart';
  $g_chart->packages = array('corechart');
  $g_chart->container_id = 'chart_div';
  $g_chart->cols = array(
    array('string', 'Opponent'),
    array('number', 'Correctly Picked'),
    array('number', 'Correctly Picked To Win'),
  );

  $gct_obj = new pickem_google_chart_tools_helper();
  $output = $gct_obj->render($g_chart);
  $output .= '<div id="' . $g_chart->container_id . '"></div>';

  if ($sort_by == 'to_win' ) {
    $output .= l(t('Sort By "Correctly Picked"'), 'pickem/' . $league->lid . '/chart2/win_lose', array('html' => TRUE));
  }
  else {
    $output .= l(t('Sort By "Correctly Picked To Win"'), 'pickem/' . $league->lid . '/chart2/to_win', array('html' => TRUE));
  }

  return $output;
}


function chart_team_scoring_page($league, $team_id) {
  $breadcrumb = drupal_get_breadcrumb();
  $breadcrumb[] = l($league->name, 'pickem/' . $league->lid);
  drupal_set_breadcrumb($breadcrumb);

  $teams = get_teams(array('gameset_id' => $league->gsid));

  $g_chart = new stdClass;
  $g_chart->options = array(
    'width' => '100%',
    'height' => 400,
    'chartArea' => array('width' => '85%'),
    'title' => t('Team Scoring - ' . $teams[$team_id]->tm_abbrev),
    'legend' => array('position' => 'top'),
    'hAxis' => array(
      'title' => 'Opponent',
      'titleTextStyle' => array(
        'color' => '#000000'
      ),
      'maxAlternation' => 3,
      'showTextEvery' => 1,
    ),
    'vAxis' => array(
      'title' => 'Points',
      'titleTextStyle' => array(
        'color' => '#000000'
      )
    )
  );

  $g_chart->rows = array();
  $games = get_games(array('gameset_id' => $league->gsid, 'team_id' => $team_id));
  foreach ($games as $gid => $row) {
    if ( is_null($row->h_score) || is_null($row->v_score)  ) {
      continue;
    }

    if ( $team_id == $row->h_id ) {
      $g_chart->rows[] = array($teams[$row->v_id]->tm_abbrev, (int)$row->h_score, (int)$row->v_score);
    }
    else {
      $g_chart->rows[] = array($teams[$row->h_id]->tm_abbrev, (int)$row->v_score, (int)$row->h_score);
    }
  }

  $g_chart->visualization = 'google.visualization.AreaChart';
  $g_chart->packages = array('corechart');
  $g_chart->container_id = 'chart_div';
  $g_chart->cols = array(
    array('string', 'Opponent'),
    array('number', 'Points For ' . $teams[$team_id]->tm_abbrev),
    array('number', 'Points Against ' . $teams[$team_id]->tm_abbrev),
  );

  $gct_obj = new pickem_google_chart_tools_helper();
  $output = $gct_obj->render($g_chart);
  $output .= '<div id="' . $g_chart->container_id . '"></div>';

  return $output;
}
