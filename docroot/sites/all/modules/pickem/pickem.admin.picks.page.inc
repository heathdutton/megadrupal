<?php

/**
 * @file
 * Page callbacks for the admin picks pages.
 */


function admin_picks_router($league = NULL, $account = NULL, $week = NULL) {
  if (isset($_POST['league_select']) && is_numeric($_POST['league_select'])) {
    drupal_goto("admin/pickem/picks/" . $_POST['league_select']);
  }
  if (isset($_POST['user_select']) && is_numeric($_POST['user_select'])) {
    drupal_goto("admin/pickem/picks/$league->lid/" . $_POST['user_select']);
  }

  if (is_null($league)) {
    drupal_set_title(t('Administer picks/choose league'), PASS_THROUGH);
    return drupal_get_form('league_select_form');
  }
  if (is_null($account)) {
    drupal_set_title(t('Administer picks/choose user'), PASS_THROUGH);
    return drupal_get_form('user_select_form', $league);
  }
}


/**
 * Menu callback; presents the list of all weeks and games.
 */
function admin_picks_all_page($league, $account) {
  return theme('admin_picks_all_page', array('league' => $league, 'account' => $account));
}


/**
 * Display the list of all weeks and games.
 * The user would pick a week link from here to make their weekly picks.
 */
function theme_admin_picks_all_page($variables) {

  $league = $variables['league'];
  $account = $variables['account'];

  // Param check: user must be a member of this league to continue.
  if (!is_league_member($league, $account)) {
    return drupal_not_found();
  }

  drupal_set_title(t("Administer Picks/Choose Week"), PASS_THROUGH);

  // Get rid of extra breakcrumb.
  $breadcrumb = drupal_get_breadcrumb();
  array_pop($breadcrumb);
  drupal_set_breadcrumb($breadcrumb);

  $teams = get_teams(array('gameset_id' => $league->gsid));
  $games = get_games(array('league_id' => $league->lid), array('by_week' => 1, 'order_by' => 'week_ended, g.gamestart, g.gid'));
  $header = array(t('Week'), t('Games'));
  $rows = array();
  foreach ($games as $wid => $weeks_games) {
    $cols = array();
    $current_week = '';
    foreach ($weeks_games as $gid => $game) {
      // The first column should be the week number link.
      if ( count($cols) == 0 ) {
        $cols[] = array('data' => l($game->wk_number, "admin/pickem/picks/$league->lid/$account->uid/" . $wid), 'class' => 'week');
      }
      $current_week .= '<div class="picks-float">' . $teams[$game->v_id]->short_name . '<br/>@' . $teams[$game->h_id]->short_name . '</div>';
    }
    $cols[] = $current_week;
    $rows[] = $cols;
  }

  $ouput = '';
  $output = t('League:') . ' ' . $league->name . '<br/>';
  $output .= t('User:') . ' ' . $account->name . '<br/>';
  $output .= theme('table', array(
    'header' => $header,
    'rows' => $rows,
    'attributes' => array('class' => array('pickem-mypicks-all')))
  );
  return $output;
}


/**
 * Menu callback; presents a list of a single weeks picks.
 */
function admin_picks_week_page($league, $week, $account) {
  drupal_set_title(t('Administer Picks'), PASS_THROUGH);

  // Get rid of extra breakcrumbs.
  $breadcrumb = drupal_get_breadcrumb();
  array_pop($breadcrumb);
  array_pop($breadcrumb);
  drupal_set_breadcrumb($breadcrumb);

  return drupal_get_form('admin_picks_week_form', $league, $week, $account);
}


function admin_picks_week_form($form, &$form_state=NULL, $league, $week, $account) {

  $form = build_picks_week_form($form, $form_state, $league, $week, $account);

  // Enable the input boxes and radio boxes, this is an admin screen.
  // It doesn't matter the week may be past.
  foreach (element_children($form['games']) as $gid) {
    unset($form['games'][$gid]['pick_v_score']['#attributes']['readonly']);
    unset($form['games'][$gid]['pick_v_score']['#attributes']['disabled']);
    unset($form['games'][$gid]['pick_h_score']['#attributes']['readonly']);
    unset($form['games'][$gid]['pick_h_score']['#attributes']['disabled']);
    unset($form['games'][$gid]['pick']['#attributes']['readonly']);
    unset($form['games'][$gid]['pick']['#attributes']['disabled']);
	unset($form['games'][$gid]['confidence']['#attributes']['readonly']);
    unset($form['games'][$gid]['confidence']['#attributes']['disabled']);
  }

  // If $account is entered in more than one league with the same scoring type,
  // add a checkbox to apply these picks to related leagues.
  $leagues = get_my_leagues($league->gsid, $account->uid);
  $apply_to_leagues = array();
  foreach ($leagues as $l) {
    if ($l->scoring_type == $league->scoring_type) {
      $apply_to_leagues[] = $l->name;
    }
  }

  // Turn on the apply to all leagues button if needed.
  $form['apply_to_all_leagues']['#access'] = count($apply_to_leagues) > 1;

  // Turn on the submit button.
  unset($form['submit']['#access']);

  return $form;
}


function theme_admin_picks_week_form($variables) {
  global $_PICKEM_TIMEZONE;

  $form = $variables['form'];
  $league = league_load($form['league_id']['#value']);
  $week = week_load($form['week_id']['#value']);
  $account = user_load($form['account_id']['#value']);

  $header = array();
  $header[] = t('Matchup');
  if ($league->scoring_engine->features['with_confidence'] == 1) {
    $header[] = array('data' => t('Confidence'), 'class' => 'confidence-pts');
  }
  $header[] = t('Status');
  $header[] = t('Time(%pickem-timezone)', array('%pickem-timezone' => $_PICKEM_TIMEZONE));

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

  $output = $league->scoring_engine->description . '<br/>';
  $output .= t('League: %league_name', array('%league_name' => $league->name)) . '<br/>';
  $output .= t('Week: %week_name', array('%week_name' => $week->wk_name)) . '<br/>';
  $output .= t('User: %user_name', array('%user_name' => $account->name)) . '<br/>';
  $output .= theme('table', array('header' => $header, 'rows' => $rows, 'attributes' => array('class' => array('pickem-mypicks-week'))));
  $output .= drupal_render_children($form);
  return $output;
}


function admin_picks_week_form_validate($form, &$form_state) {
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


function admin_picks_week_form_submit($form, &$form_state) {

  $league = league_load($form_state['values']['league_id']);
  $week = week_load($form_state['values']['week_id']);
  $account = user_load($form_state['values']['account_id']);

  // Check and see if the user wants to save to all their leagues that use the same gameset.
  $save_for_these_leagues = array();
  if ( $form_state['values']['apply_to_all_leagues'] == 1 ) {
    $save_for_these_leagues = get_my_leagues($league->gsid, $account->uid);
  }
  else {
    $save_for_these_leagues[] = $league;
  }

  foreach ($save_for_these_leagues as $l) {
    // Only save for league using the same scoring system.
    if ($l->scoring_type == $league->scoring_type) {
      drupal_set_message(t("Saving picks for %league_name", array('%league_name' => $l->name)));
      foreach ($form_state['values']['games'] as $gid => $pick) {
        $pick['gid'] = $gid;
        set_pick($account->uid, $l, $pick, 'admin');
      }
    }
  }

  // Clear pickem_ caches, since admin picks is usually used after lockup and points may
  // be affected.
  cache_clear_all('pickem_', 'cache', TRUE);
}
