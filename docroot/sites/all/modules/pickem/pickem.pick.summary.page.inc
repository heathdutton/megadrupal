<?php

/**
 * @file
 * Page callbacks and theme functions for the pick summary page.
 */

/**
 * Menu callback;
 */
function pick_summary_page($league, $week) {
  global $user;
  drupal_set_title(t('Pick Summary - %week_name', array('%week_name' => $week->wk_name)), PASS_THROUGH);

  $breadcrumb = drupal_get_breadcrumb();
  $breadcrumb[] = l($league->name, 'pickem/' . $league->lid);
  drupal_set_breadcrumb($breadcrumb);


  $teams = get_teams(array('gameset_id' => $league->gsid));
  $weeks = get_weeks(array('week_id' => $week->wid));
  $games = get_games(array('week_id' => $week->wid));
  $theme_data = array('games' => array(), 'users' => array());
  foreach ($games as $gid => $g_data) {

    // Figure out the matchup operator.
    if ($g_data->neutral_venue == 1) {
      $matchup = " vs ";
    }
    else {
      $matchup = "<br/>@";
    }
    $theme_data['games'][$gid]['game_started'] = $g_data->game_started;

    // Save who is the home team and who is the visitor.
    $theme_data['games'][$gid]['h_id'] = $g_data->h_id;
    $theme_data['games'][$gid]['v_id'] = $g_data->v_id;

    // Load-up the home team info.
    $theme_data['games'][$gid]['teams'][$g_data->h_id] = array(
      'short_name' => $teams[$g_data->h_id]->short_name,
      'long_name' => $teams[$g_data->h_id]->long_name
    );

    // Load-up the visiting team info.
    $theme_data['games'][$gid]['teams'][$g_data->v_id] = array(
      'short_name' => $teams[$g_data->v_id]->short_name,
      'long_name' => $teams[$g_data->v_id]->long_name
    );

    // Load-up the game titles.
    $theme_data['games'][$gid]['short_game_title'] =
      $theme_data['games'][$gid]['teams'][$g_data->v_id]['short_name'] . $matchup .
        $theme_data['games'][$gid]['teams'][$g_data->h_id]['short_name'];

    $theme_data['games'][$gid]['long_game_title'] =
      $theme_data['games'][$gid]['teams'][$g_data->v_id]['long_name'] . $matchup .
        $theme_data['games'][$gid]['teams'][$g_data->h_id]['long_name'];



    // Figure out who won.
    if ( $g_data->h_score == NULL ) {
      $theme_data['games'][$gid]['winner'] = 0;  // game not played
    }
    elseif ($g_data->h_score == $g_data->v_score) {
      $theme_data['games'][$gid]['winner'] = -1; // game ended in tie
    }
    elseif ($g_data->h_score > $g_data->v_score) {
      $theme_data['games'][$gid]['winner'] = $g_data->h_id;  // home team won.
    }
    elseif ($g_data->v_score > $g_data->h_score) {
      $theme_data['games'][$gid]['winner'] = $g_data->v_id;  // visiting team won.
    }

    // Save the score (May be NULL).
    $theme_data['games'][$gid]['teams'][$g_data->h_id]['score'] = $g_data->h_score;
    $theme_data['games'][$gid]['teams'][$g_data->v_id]['score'] = $g_data->v_score;


  }

  $totals = get_user_totals($league, $week->wid); //sorted by weekly scores
  $picks = get_picks( array('league_id' => $league->lid, 'week_id' => $week->wid) );
  foreach ($totals as $uid => $tot) {
    $user_form = array(
      'uid' => $uid,
      'user_link' => theme('username', array('account' => $tot['user_obj'])),
      'weekly_rank' => $tot['ranking'],
      'weekly_points' => $tot['total'],
      'picks' => array()
    );

    // Loop collect picks and pick points.
    foreach ($theme_data['games'] as $gid => $game_info) {

      // Check to see if user is allowed to see this game for other users.
      if (($league->pick_lock == PICKEM_PICK_LOCK_WEEK && $week->week_started == 1) ||
          ($league->pick_lock == PICKEM_PICK_LOCK_GAME && $game_info['game_started'] == 1)) {

        // User is allowed to see picks now.
        // If they didn't pick at all, use a -1.
        if (empty($picks[$gid][$uid]->winnerpick_id) || $picks[$gid][$uid]->winnerpick_id == 0) {
          $user_form['picks'][$gid]['picked'] = -1;
        }
        else {
          $user_form['picks'][$gid]['picked'] = $picks[$gid][$uid]->winnerpick_id;
        }

        if (isset($picks[$gid][$uid]->pick_h_score) && isset($picks[$gid][$uid]->pick_v_score) && $picks[$gid][$uid]->pick_h_score + $picks[$gid][$uid]->pick_v_score > 1) {
          $user_form['picks'][$gid]['pick_h_score'] = $picks[$gid][$uid]->pick_h_score;
          $user_form['picks'][$gid]['pick_v_score'] = $picks[$gid][$uid]->pick_v_score;
        }

        if ( !isset($tot['weeks'][$week->wid]['games'][$gid]) ) {
          $user_form['picks'][$gid]['points'] = 0;
        }
        else {
          $user_form['picks'][$gid]['points'] = $tot['weeks'][$week->wid]['games'][$gid];
        }

        if ($league->scoring_engine->features['with_confidence'] == 1) {
          $user_form['picks'][$gid]['confidence'] = !isset($picks[$gid][$uid]->confidence)?0:$picks[$gid][$uid]->confidence;
        }
      }

      // User is not allowed to see other user's picks.
      else {
        if (isset($picks[$gid][$uid]->winnerpick_id) && $picks[$gid][$uid]->winnerpick_id != 0) {
          if ($uid == $user->uid) {
            // User is allowed to see their own picks, but not others.
            $user_form['picks'][$gid]['picked'] = $picks[$gid][$uid]->winnerpick_id;
          }
          else {
            // Use 0 to mean "you are not allowed to see this."
            $user_form['picks'][$gid]['picked'] = 0;
          }
        }
        else {
          // Use a -1 to mean they haven't picked yet.
          // It also might mean they are using another pick system which may
          // use another field.
          $user_form['picks'][$gid]['picked'] = -1;
        }
      }
    }

    // Since we are using push they should stay in ranking order.
    array_push($theme_data['users'], $user_form);
  }

  return theme('pick_summary', array('theme_data' => $theme_data));
}


function theme_pick_summary($variables) {
  global $pager_page_array, $pager_total;

  $theme_data = $variables['theme_data'];

  $page = isset($_GET['page']) ? $_GET['page'] : 0;
  $pager_page_array = explode(',', $page);
  // how many pages.
  $items_per_page = variable_get('pickem_summary_items_per_page', 1000);
  $pager_total[0] = ceil(count($theme_data['games']) / $items_per_page);

  $header = array();
  $header[] = t('Weekly Rank');
  $header[] = t('Points');
  $i = 0;
  foreach ($theme_data['games'] as $gid => $game_info) {
    if ( $i >= $page * $items_per_page && $i < ($page+1) * $items_per_page ) {
      $long_game_title = str_replace("<br/>", " ", $theme_data['games'][$gid]['long_game_title']);
      $header[] = array('data' => $theme_data['games'][$gid]['short_game_title'], 'title' => $long_game_title);
    }
    $i++;
  }

  $rows = array();
  foreach (element_children($theme_data['users']) as $key) {
    $cols = array();
    $cols[] = array('data' => $theme_data['users'][$key]['weekly_rank'] . '.&nbsp;' . $theme_data['users'][$key]['user_link'], 'class' => 'users');
    $cols[] = $theme_data['users'][$key]['weekly_points'];

    $i = 0;
    foreach (element_children($theme_data['games']) as $gid) {
      if ( $i >= $page * $items_per_page && $i < ($page+1) * $items_per_page ) {
        $class = '';

        // Turn the boxes red or green.  Green if you made a pick and picked correctly or the game tied.
        // Red if you didn't make a pick or got it wrong.
        if ( $theme_data['games'][$gid]['winner'] == 0 ) {
          $class = 'not-played';
        }
        // Picked, points > 0.
        elseif ( $theme_data['users'][$key]['picks'][$gid]['picked'] != -1 &&
          ($theme_data['users'][$key]['picks'][$gid]['points'] > 0 ||
           $theme_data['games'][$gid]['winner'] == -1 ) ) {
          $class = 'pickem-win';
        }
        else {
          $class = 'pickem-loss';
        }

        $col = array('data' => '');
        if ( $theme_data['users'][$key]['picks'][$gid]['picked'] == 0 ) {
          $col['data'] .= 'X';
        }
        elseif ( $theme_data['users'][$key]['picks'][$gid]['picked'] == -1 ) {
          $col['data'] .= '-';
        }
        else {
          $col['data'] .= $theme_data['games'][$gid]['teams'][$theme_data['users'][$key]['picks'][$gid]['picked']]['short_name'];
        }
        $col['class'] = $class;

        // Only reveal picked score and confidence points after game is final?
        // Is this a feature?
        if ($class != 'not-played') {
          if (isset($theme_data['users'][$key]['picks'][$gid]['pick_v_score'])) {
            $game = $theme_data['games'][$gid];
            $pick = $theme_data['users'][$key]['picks'][$gid];
            // Visting team short name
            $col['data'] = $game['teams'][$game['v_id']]['short_name'] . ' ';
            // Visting team user's picked score
            $col['data'] .= $pick['pick_v_score'];
            $col['data'] .= '|';
            // Home team short name
            $col['data'] .= $game['teams'][$game['h_id']]['short_name'] . ' ';
            // Home team user's picked score
            $col['data'] .= $pick['pick_h_score'];
          }
          elseif (isset($theme_data['users'][$key]['picks'][$gid]['confidence'])) {
            $col['data'] .= "(" . $theme_data['users'][$key]['picks'][$gid]['confidence'] . ")";
          }
        }
        $cols[] = $col;
      }
      $i++;
    }
    array_push($rows, $cols);
  }
  $output = '';
  $output .= theme('table', array('header' => $header, 'rows' => $rows, 'attributes' => array('class' => array('pickem-pick-summary'))));
  $pager_output = theme('pager', array(), 20);
  if ( strlen($pager_output) > 0 ) {
    $output .= $pager_output;
    $output .= '<div style="text-align:center;">' . t('More games on other pages') . '</div>';
  }

  return $output;
}
