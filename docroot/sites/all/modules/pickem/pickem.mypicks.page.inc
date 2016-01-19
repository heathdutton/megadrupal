<?php

/**
 * @file
 * Page callbacks for adding, editing, and deleting user picks.
 */


/**
 * Menu callback; presents the list of all weeks and games.
 */
function mypicks_all_page($league) {
  global $user;
  if (!is_my_league($league)) {
    return drupal_not_found();
  }
  drupal_set_title(t('My Picks - %user_name', array('%user_name' => $user->name)), PASS_THROUGH);

  $breadcrumb = drupal_get_breadcrumb();
  $breadcrumb[] = l($league->name, 'pickem/' . $league->lid);
  drupal_set_breadcrumb($breadcrumb);

  $teams = get_teams(array('gameset_id' => $league->gsid));
  $games = get_games(array('league_id' => $league->lid), array('by_week' => 1, 'order_by' => 'week_ended, g.gamestart, g.gid'));
  
  return theme('mypicks_all', array('league' => $league, 'teams' => $teams, 'games' => $games));
}


/**
 * Display the list of all weeks and games.
 * The user would pick a week link from here to make their weekly picks.
 */
function theme_mypicks_all($variables) {
  $league = $variables['league'];
  $teams = $variables['teams'];
  $games = $variables['games'];

  $header = array(t('Week'), t('Games'));
  $rows = array();
  foreach ($games as $wid => $weeks_games) {
    $cols = array();
    $current_week = '';
    foreach ($weeks_games as $gid => $game) {

      // Figure out the matchup operator.
      if ($game->neutral_venue == 1) {
        $matchup = "vs ";
      }
      else {
        $matchup = "@";
      }

      // The first column should be the week number link.
      if ( count($cols) == 0 ) {
        $cols[] = array('data' => l(t('Week&nbsp;%wk_number', array('%wk_number' =>  $game->wk_number)), "pickem/$league->lid/mypicks/" . $wid, array('html' => TRUE)), 'class' => 'week');
      }
      $current_week .= '<div class="picks-float">' . $teams[$game->v_id]->short_name . '<br/>' . $matchup . $teams[$game->h_id]->short_name . '</div>';
    }
    $cols[] = $current_week;
    $rows[] = $cols;
  }
  return theme('table', array('header' => $header, 'rows' => $rows, 'attributes' => array('class' => array('pickem-mypicks-all'))));
}


function mypicks_week_form($form, &$form_state, $league, $week) {
  global $user;

  drupal_set_title(t('My Picks - %user_name - %week', array('%user_name' => $user->name, '%week' => $week->wk_name)), PASS_THROUGH);
  
  $breadcrumb = drupal_get_breadcrumb();
  $breadcrumb[] = l($league->name, 'pickem/' . $league->lid);
  $breadcrumb[] = l(t('My Picks'), 'pickem/' . $league->lid . '/mypicks');
  drupal_set_breadcrumb($breadcrumb);
  
  $form = build_picks_week_form($form, $form_state, $league, $week, $user);
  return $form;
}


function theme_mypicks_week_form($variables) {
  global $user;

  $form = $variables['form'];
  $league = league_load($form['league_id']['#value']);

  // Drupal sets the default timezone to be the user's
  //if available, otherwise, the system's.
  $timezone = date_default_timezone_get();

  $header = array();
  $header[] = t('Matchup');
  if ($league->scoring_engine->features['with_confidence'] == 1) {
    $header[] = array('data' => t('Confidence'), 'class' => 'confidence-pts');
  }
  $header[] = t('Status');
  $header[] = array('data'=>t('Game time'), 'title'=> t('Timezone is @timezone', array('@timezone' => $timezone)));

  $rows = array();
  $last = count(element_children($form['games']));
  $count = 0;
  foreach (element_children($form['games']) as $gid) {
    $count++;
    $cols = array();

    if (isset($form['games'][$gid]['pick'])) {
      if ($form['games'][$gid]['pick']['#default_value'] == '') {
        $cols[] = array('data' => drupal_render($form['games'][$gid]['pick']), 'class' => 'no-pick');
      }
      else {
        $cols[] = array('data' => drupal_render($form['games'][$gid]['pick']));
      }
    }
    else {
      $cols[] = drupal_render($form['games'][$gid]['pick_v_score']) . drupal_render($form['games'][$gid]['pick_h_score']);
    }

    // Render confidence boxen
    if ($league->scoring_engine->features['with_confidence'] == 1) {
      if($form['games'][$gid]['confidence']['#default_value'] == '') {
        $cols[] = array('data' => drupal_render($form['games'][$gid]['confidence']), 'class'=>'no-pick');
      } else {
        $cols[] = array('data' => drupal_render($form['games'][$gid]['confidence']));
      }
    }

    $cols[] = drupal_render($form['games'][$gid]['pick_status']);

    $cols[] = drupal_render($form['games'][$gid]['gamestart']);
    $rows[] = $cols;
  }

  $output = $league->scoring_engine->description;
  $output .= '<p>' . t('Game time is represented in the @timezone timezone.', array('@timezone' => $timezone)) . '</p>';
  $output .= theme('table', array('header' => $header, 'rows' => $rows, 'attributes' => array('class' => array('pickem-mypicks-week'))));
  $output .= drupal_render_children($form);
  return $output;
}


function mypicks_week_form_validate($form, &$form_state) {
  $league = league_load($form_state['values']['league_id']);
  if (($league->scoring_engine->features['with_confidence'] == 1)) {
    $conf_values = array();
    $max_conf = count($form_state['values']['games']);
    foreach ($form_state['values']['games'] as $gid => $pick) {

      // Skip empty confidence values.
      if ($pick['confidence'] == '') {
        continue;
      }

      // Out of range confidence numbers
      if (!is_numeric($pick['confidence']) || $pick['confidence'] < 1 || $pick['confidence'] > $max_conf) {
        form_set_error('games][' . $gid . '][confidence', t('Confidence point must be a number from 1 to %max.', array('%max' => $max_conf)));
      }

      // Duplicate confidence numbers
      if (isset($conf_values[$pick['confidence']])) {
        form_set_error('games][' . $gid . '][confidence', t('Confidence point %pt already used.', array('%pt' => $pick['confidence'])));
      }

      $conf_values[$pick['confidence']] = TRUE;
    }
  }
}


function mypicks_week_form_submit($form, &$form_state) {
  global $user;

  $league = league_load($form_state['values']['league_id']);
  $week = week_load($form_state['values']['week_id']);

  // You should not be able to save picks after lockup.
  if ($league->pick_lock == PICKEM_PICK_LOCK_WEEK && $week->week_started == 1) {
    drupal_set_title(t('%week_name picks are locked.', array('%week_name' => $week->wk_name)), PASS_THROUGH);
    return t("You cannot change your picks now.");
  }

  // Check and see if the user wants to save to all their leagues that use the same gameset.
  $save_for_these_leagues = array();
  if ( $form_state['values']['apply_to_all_leagues'] == 1 ) {
    $save_for_these_leagues = get_my_leagues($league->gsid);
  }
  else {
    $save_for_these_leagues[] = $league;
  }

  foreach ($save_for_these_leagues as $l) {
    // Only save for league using the same scoring system.
    $success = TRUE;
    if ($l->scoring_type == $league->scoring_type) {
      foreach ($form_state['values']['games'] as $gid => $pick) {
        $pick['gid'] = $gid;
        if (set_pick($user->uid, $l, $pick, 'user') != TRUE) {
          $success = FALSE;
        }
      }

      if ($success) {
        drupal_set_message(t('The picks have been saved for league <i>%league</i>.', array('%league' => $l->name)));
      }
    }
  }
}
