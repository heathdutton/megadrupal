<?php

/**
 * @file
 * Page callbacks for display league standings.
 */


/**
 * Menu callback; show user standings for regular season and playoffs.
 */
function standings_page($league) {
  return theme('standings', array('league' => $league));
}


/**
 * Theme function to display the league standings page.
 */
function theme_standings($variables) {
  $league = $variables['league'];

  $breadcrumb = drupal_get_breadcrumb();
  $breadcrumb[] = l($league->name, 'pickem/' . $league->lid);
  drupal_set_breadcrumb($breadcrumb);
  drupal_set_title(t("Standings"), PASS_THROUGH);
  drupal_add_library('system', 'drupal.collapse');

  $totals = get_user_totals($league);
  $week_totals = get_totals_by_week($league);

  // Setup output variable.
  $output = '<div class="pickem">' . t('Click on week numbers to see weekly pick summary.');

  // Regular season table.
  $reg_weeks = get_weeks(array('gameset_id' => $league->gsid, 'playoffs' => 0));
  if (count($reg_weeks) > 0) {
    $reg_season_table = get_standings_table($league, $totals, $week_totals, $reg_weeks);
    $output .= '<div><fieldset class="collapsible">' .
      '<legend><span class="fieldset-legend"><a href="#">' . t("Regular Season") .
      '</a></span></legend>';
    $output .= '<div class="fieldset-wrapper">';
    $output .= theme('table', array(
      'header' => $reg_season_table['header'],
      'rows' => $reg_season_table['rows'],
      'attributes' => array('class' => array('standings')))
    );
    $output .= "</div></fieldset></div>";
  }

  // Playoff table.
  $po_weeks = get_weeks(array('gameset_id' => $league->gsid, 'playoffs' => 1));
  if (count($po_weeks) > 0) {
    $po_season_table = get_standings_table($league, $totals, $week_totals, $po_weeks, TRUE);
    $output .= '<div><fieldset class="collapsible">' .
      '<legend><span class="fieldset-legend"><a href="#">' . t("Playoffs") .
      '</a></span></legend>';
    $output .= '<div class="fieldset-wrapper">';
    $output .= theme('table', array(
      'header' => $po_season_table['header'],
      'rows' => $po_season_table['rows'],
      'attributes' => array('class' => array('standings')))
    );
    $output .= "</div></fieldset></div>";
  }
  $output .= "</div>";
  return $output;
}


/*
 * Helper function to create the header and rows arrays for the standings table theme function.
 */
function get_standings_table($league, $totals, $week_totals, $weeks, $use_week_names=FALSE) {
  $table = array();
  $header = array();
  $header[] = array('data' => t("Overall Rank"));
  $header[] = array('data' => t("TOT"), 'title' => t("Total points"));
  $header[] = array('data' => '&nbsp;', 'title' => t("Points back"));
  foreach ($weeks as $wid => $week) {
    if ($use_week_names) {
      $header[] = array('data' => l(t('Week&nbsp;%week_number<br/>%week_name', array('%week_number' => $week->wk_number, '%week_name' => $week->wk_name)), 'pickem/' . $league->lid . "/summary/$wid", array('html' => TRUE)), 'class' => 'week-numbers');
    }
    else {
      $header[] = array('data' => l($week->wk_number, 'pickem/' . $league->lid . "/summary/$wid"), 'class' => 'week-numbers');
    }
  }
  $rows = array();
  foreach ($totals as $uid => $tot) {
    $row = array();
    // Username
    $row[] = array('data' => $tot['ranking'] . '.&nbsp;&nbsp;' . theme('username', array('account' => $tot['user_obj'])), 'class' => "users");
    // Total points
    $row[] = array('data' => $tot['total'], 'class' => "totals");
    // Points back
    if ($tot['pts_back'] == 0) {
      $row[] = array('data' => "&nbsp;", 'class' => "gamesback", 'title' => t("Points back"));
      $high = $tot['total'];
    }
    else {
      $row[] = array('data' => "+" . $tot['pts_back'], 'class' => "gamesback", 'title' => t("Points back"));
    }

    // Weeks.
    foreach ($weeks as $wid => $week) {
      $class = "weekly";
      if (!isset($tot['weeks'][$wid])) {
        // Do no calcs, week does not yet have any scores.
      }
      else {
        if ($tot['weeks'][$wid]['picked_games'] < $tot['weeks'][$wid]['weeks_games']) {
          $class .= " skipped-pick";
        }
        if ($tot['weeks'][$wid]['total'] == $week_totals[$wid]["min"] && $week_totals[$wid]["min"] != $week_totals[$wid]["max"]) {
          $class .= " weekly-low";
        }
        if ($tot['weeks'][$wid]['total'] == $week_totals[$wid]["max"] && $week_totals[$wid]["min"] != $week_totals[$wid]["max"]) {
          $class .= " weekly-high";
        }
      }

      // Week does not yet have any scores.
      if (!isset($tot['weeks'][$wid])) {
        $row[] = array('data' => "&nbsp;");
      }
      else {
        $row[] = array('data' => $tot['weeks'][$wid]['total'], 'class' => $class);
      }
    }
    $rows[] = $row;
  }
  $table['header'] = $header;
  $table['rows'] = $rows;
  return $table;
}
