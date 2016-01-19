<?php

/**
 * @file
 * includes all pickem admin functions.
*/

/**
 * Menu callback for the high-level pickem administration screen.
 */
function pickem_overview_page() {
  $items = pickem_menu();
  $content = '<dl class="admin-list">';
  foreach ($items as $path => $menu) {
    $t = explode('/', $path);
    if ( count($t) == 3 ) {
      if ( ( $t[0] == 'admin' ) && $t[1] == 'pickem'  && $menu['type'] == MENU_NORMAL_ITEM) {
        $content .= '<dt>' . l($menu['title'], $path) . '</dt>';
        if ( isset($menu['description']) ) {
          $content .= '<dd>' . $menu['description'] . '</dd>';
        }
      }
    }
  }
  $content .= "</dl>";
  return $content;
}


/**
 * Pickem administration screen global settings form.
 */
function pickem_settings() {
  $form = array();
  $form['pickem_commisioner_email'] = array(
    '#type' => 'textfield',
    '#size' => 100,
    '#title' => t('Email Address of the Commisioner'),
    '#default_value' => variable_get('pickem_commisioner_email', ''),
    '#description' => t("This is the return address for all leaguewide pickem email."),
  );

  $form['pickem_default_email_reminders'] = array(
    '#type' => 'checkbox',
    '#title' => t('Send email reminders'),
    '#description' => t("If this is checked, each new user will get 'Your picks are not complete' emails.  Each user can override this if they wish."),
    '#return_value' => 1,
    '#default_value' => variable_get('pickem_default_email_reminders', 1)
  );

  $form['pickem_auto_signup'] = array(
    '#type' => 'fieldset',
    '#title' => t('Auto Signup'),
    '#collapsible' => TRUE,
  );

  $form['pickem_auto_signup']['pickem_auto_signup_enable'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable'),
    '#return_value' => 1,

    '#default_value' => variable_get('pickem_auto_signup_enable', 0),
    '#description' => t("This will automatically assign new users to the selected league(s)."),
  );

  $all_leagues = get_leagues();
  $leagues = array();
  foreach ($all_leagues as $l) {
    $leagues[$l->lid] = $l->name;
  }

  $form['pickem_auto_signup']['pickem_auto_signup_default_league'] = array(
    '#type' => 'select',
    '#multiple' => TRUE,
    '#attributes' => array('size' => 8),
    '#default_value' => variable_get('pickem_auto_signup_default_league', array()),
    '#options' => $leagues,
    '#title' => t('League'),
    '#description' => t("Which league(s) should users be signed up for?"),
  );

  $form['pickem_prefer_long_team_names'] = array(
    '#type' => 'checkbox',
    '#title' => t('Prefer long team names'),
    '#return_value' => 1,
    '#default_value' => variable_get('pickem_prefer_long_team_names', 0),
    '#description' => t("When there is room, use the full team name and not the abbreviation.  Some leagues have ambiguous team abbreviations."),
  );

  $form['pickem_show_team_win_loss_record'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show team win/loss record on picks page'),
    '#return_value' => 1,
    '#default_value' => variable_get('pickem_show_team_win_loss_record', 1),
    '#description' => t("This shows the team win loss record next to the team name on the picks page.  This can be helpful in deciding who to choose."),
  );

  $form['pickem_full_user_load'] = array(
    '#type' => 'checkbox',
    '#title' => t('Fully themeable user links'),
    '#return_value' => 1,
    '#default_value' => variable_get('pickem_full_user_load', 0),
    '#description' => t("This enables compatibility with user name formatters like the RealName module.  With large user lists (>100) it may cause slower performance."),
  );

  $form['pagers'] = array(
    '#type' => 'fieldset',
    '#title' => t('Configure pagers for various pickem screens'),
    '#description' => t("These values represent games/page for each screen.  Set these to a big numbers (ie 1000) if you don't want a pagers to show on the screens."),
    '#collapsible' => TRUE
  );
  $form['pagers']['pickem_summary_items_per_page'] = array(
    '#type' => 'textfield',
    '#size' => 5,
    '#title' => t('Pick summary screen'),
    '#default_value' => variable_get('pickem_summary_items_per_page', 1000),
  );

  $form['logo'] = array(
    '#type' => 'fieldset',
    '#title' => t('Logo options'),
    '#collapsible' => FALSE
  );
  $form['logo']['pickem_random_site_logo'] = array(
    '#type' => 'checkbox',
    '#title' => t('Use random site logo from the pickem/logos directory'),
    '#return_value' => 1,
    '#default_value' => variable_get('pickem_random_site_logo', 0),
    '#description' => t("This will change the site logo randomly using any files from the logos directory."),
  );
  $form['logo']['pickem_random_site_logo_pattern'] = array(
    '#type' => 'textfield',
    '#size' => 10,
    '#title' => t('File name pattern'),
    '#default_value' => variable_get('pickem_random_site_logo_pattern', '_100.png'),
    '#description' => t("Random logos in the logos directory must match this file name pattern (ex: _100.png).  The pattern is anchored on the end of the file name."),
  );

  $form['blocks'] = array(
    '#type' => 'fieldset',
    '#title' => t('Block options'),
    '#collapsible' => FALSE
  );
  $form['blocks']['pickem_block_leaderboard_how_many'] = array(
    '#type' => 'textfield',
    '#size' => 10,
    '#title' => t('Leaderboard list length'),
    '#default_value' => variable_get('pickem_block_leaderboard_how_many', 10),
    '#description' => t("How many users do you want in the leaderboard lists?  The logged in user will always show.  This affects the league dashboard page and if you place blocks in the sidebars."),
  );

  $form['blocks']['navigation'] = array(
    '#type' => 'fieldset',
    '#title' => t('Pickem league navigation'),
    '#collapsible' => FALSE
  );
  $form['blocks']['navigation']['pickem_block_nav_show_only_joined'] = array(
    '#title' => t('Show only joined leagues'),
    '#type' => 'checkbox',
    '#return_value' => 1,
    '#default_value' => variable_get('pickem_block_nav_show_only_joined', 0),
    '#description' => t("Limit the pickem navigation block to show only leagues that user has joined."),
  );

  // Check to see if the jstimer is configured correctly.
  if (module_exists("jstimer")) {
    if (!module_exists("jst_timer")) {
      drupal_set_message(l(t('Javascript Timer widget'), 'admin/build/modules') . ' ' . t('is not enabled for the pickem module.  Make sure "Widget: timer" is enabled under the Javascript timer package or the timer will not show properly.'), 'error');
    }
    if (variable_get('jstimer_js_load_option', 0) != 1 ) {
      drupal_set_message(l(t('Javascript Timer settings'), 'admin/config/system/jstimer') . ' ' . t('are set incorrectly for the pickem module.  Make sure Javascript load options are set to EVERY PAGE or the timer will not show properly.'), 'error');
    }
  }

  return system_settings_form($form);
}


/*
 * Gameset list form
 */
function pickem_gamesets_list($form, &$form_state) {

  $form = array('#tree' => TRUE);
  $gamesets = get_gamesets();
  foreach ($gamesets as $gsid => $gameset) {
    $form['gamesets'][$gsid] = array(
      'gs_name' => array(
        '#markup' => $gameset->gs_name
      ),
      'sport' => array(
        '#markup' => $gameset->sport
      ),
      'timezone' => array(
        '#markup' => $gameset->timezone
      ),
      'weeks_link' => array(
        '#markup' => l(t('weeks'), 'admin/pickem/weeks/' . $gsid . '/list')
      ),
      'teams_link' => array(
        '#markup' => l(t('teams'), 'admin/pickem/teams/' . $gsid . '/list')
      ),
      'edit_link' => array(
        '#markup' => l(t('edit game set'), 'admin/pickem/gamesets/edit/' . $gsid)
      ),
      'delete_link' => array(
        '#markup' => l(t('delete game set'), 'admin/pickem/gamesets/delete/' . $gsid)
      ),
      'export_link' => array(
        '#markup' => l(t('export'), 'admin/pickem/gamesets/export/' . $gsid)
      ),
    );
  }

  return $form;
}


/*
 * Gameset list theme function.
 */
function theme_pickem_gamesets_list($variables) {
  $form = $variables['form'];

  $header = array(
    t('Name'),
    t('Sport'),
    t('Timezone'),
    array('data' => t('Operations'), 'colspan' => '5')
  );

  $output = '';
  $rows = array();
  if (isset($form['gamesets'])) {
    foreach (element_children($form['gamesets']) as $key) {
      $rows[] = array(
        drupal_render($form['gamesets'][$key]['gs_name']),
        drupal_render($form['gamesets'][$key]['sport']),
        drupal_render($form['gamesets'][$key]['timezone']),
        drupal_render($form['gamesets'][$key]['edit_link']),
        drupal_render($form['gamesets'][$key]['teams_link']),
        drupal_render($form['gamesets'][$key]['weeks_link']),
        drupal_render($form['gamesets'][$key]['export_link']),

        drupal_render($form['gamesets'][$key]['delete_link'])
      );
    }
  }
  $output .= theme('table', array('header' => $header, 'rows' => $rows));

  return $output;
}


/**
 * Gameset edit form.
 */
function pickem_gameset_edit($form, &$form_state, $type, $gameset) {

  if ( is_null($gameset) ) {
    // this should match the table field names
    $gameset = new stdClass();
    $gameset->gsid = 0;
    $gameset->gs_name = '';
    $gameset->sport = '';
  }

  $form = array();
  $form['gameset']['gsid'] = array(
    '#title'  => t('Game Set ID'),
    '#type' => 'hidden',
    '#size' => 20,
    '#maxlength' => 50,
    '#default_value' => $gameset->gsid
  );
  $form['gameset']['gs_name'] = array(
    '#title'  => t('Name'),
    '#required' => TRUE,
    '#type' => 'textfield',
    '#size' => 50,
    '#maxlength' => 50,
    '#default_value' => $gameset->gs_name
  );

  $form['gameset']['sport'] = array(
    '#title'  => t('Sport'),
    '#type' => 'textfield',
    '#size' => 32,
    '#maxlength' => 32,
    '#default_value' => $gameset->sport,
    '#description' => t('Used to include sport specific style sheet and by Sports Scores module.')
  );

  $timezones = array('' => t('Choose a time zone'));
  $zones = timezone_identifiers_list();
  foreach ($zones as $zone) {
    $timezones[$zone] = $zone;
  }
  $form['timezone'] = array(
    '#title' => t('Time zone'),
    '#type' => 'select',
    '#default_value' => !empty($gameset->timezone) ? $gameset->timezone : '',
    '#required' => TRUE,
    '#options' => $timezones
  );

  $form['submit'] = array('#type' => 'submit', '#value' => t('Save'));

  return $form;
}


/*
 * Gameset form submit handler.
 */
function pickem_gameset_edit_submit($form, &$form_state) {
  $gameset = $form_state['values'];

  db_merge('pickem_gamesets')
    ->key(array(
      'gsid' => $gameset['gsid'],
    ))
    ->fields(array(
      'gs_name' => $gameset['gs_name'],
      'sport' => $gameset['sport'],
      'timezone' => $gameset['timezone']
    ))
    ->execute();

  $t_string = t('Saved game set %name.', array('%name' => $gameset['gs_name']));
  drupal_set_message($t_string);
  watchdog('pickem', $t_string, NULL, WATCHDOG_NOTICE);

  $form_state['redirect'] = 'admin/pickem/gamesets';
}

/*
 * Gameset delete confirm form.
 */
function pickem_gameset_delete_confirm($form, &$form_state, $gameset) {
  $form['#gameset'] = $gameset;
  $caption = '<p>' . t('This action cannot be undone.  All related teams, weeks, and games will be deleted.') . '</p>';
  return confirm_form($form, t('Are you sure you want to delete the game set %name?', array('%name' => $gameset->gs_name)), 'admin/pickem/gamesets', $caption, t('Delete'));
}


/*
 * Gameset delete submit handler.
 */
function pickem_gameset_delete_confirm_submit($form, &$form_state) {
  $gameset = $form['#gameset'];
  db_query("DELETE FROM {pickem_gamesets} WHERE gsid=:gsid", array(':gsid' => $gameset->gsid));
  db_query("DELETE FROM {pickem_teams} WHERE gsid=:gsid", array(':gsid' => $gameset->gsid));
  db_query("DELETE FROM {pickem_weeks} WHERE gsid=:gsid", array(':gsid' => $gameset->gsid));
  db_query("DELETE FROM {pickem_games} WHERE gsid=:gsid", array(':gsid' => $gameset->gsid));

  $t_string = t('Deleted game set %name.', array('%name' => $gameset->gs_name));
  drupal_set_message($t_string);
  watchdog('pickem', $t_string, NULL, WATCHDOG_NOTICE);

  $form_state['redirect'] = 'admin/pickem/gamesets';
}


/*
 * Week list form.
 */
function pickem_weeks_list($form, &$form_state, $gameset) {

  $breadcrumb = drupal_get_breadcrumb();
  $breadcrumb[] = l($gameset->gs_name, 'admin/pickem/gamesets');
  drupal_set_breadcrumb($breadcrumb);

  $form = array(
    '#theme' => 'pickem_weeks_list',
    '#tree' => TRUE,
    '#gameset' => $gameset,
  );

  // Save the original time zone and set the gameset time zone for time rendering.
  $orig_timezone = date_default_timezone_get();
  date_default_timezone_set($gameset->timezone);

  $weeks = get_weeks(array('gameset_id' => $gameset->gsid));
  foreach ( $weeks as $wid => $week ) {
    $form['weeks'][] = array(
      'number' => array(
        '#markup' => $week->wk_number,
      ),
      'name' => array(
        '#markup' => $week->wk_name,
      ),
      'firstgame' => array(
        '#markup' => !empty($week->first_gamestart) ? format_date($week->first_gamestart, 'medium') : '',
      ),
      'lastgame' => array(
        '#markup' => !empty($week->last_gamestart) ? format_date($week->last_gamestart, 'medium') : '',
      ),
      'edit_link' => array(
        '#markup' => l(t('edit week'), 'admin/pickem/weeks/' . $gameset->gsid . '/edit/' . $wid)
      ),
      'list_games_link' => array(
        '#markup' => l(t('games'), 'admin/pickem/games/' . $wid)
      ),
      'delete_link' => array(
        '#markup' => l(t('delete week'), 'admin/pickem/weeks/' . $gameset->gsid . '/delete/' . $wid)
      ),
    );
  }
  date_default_timezone_set($orig_timezone);

  $form['save'] = array(
    '#type' => 'submit',
    '#value' => t('Save Weeks')
  );
  return $form;
}


/**
 * Week list theme function.
 */
function theme_pickem_weeks_list($variables) {
  $form = $variables['form'];

  $header = array(
    t('Week'),
    t('Name'),
    t('First Game') . ' (' . $form['#gameset']->timezone . ')',
    t('Last Game'),
    array('data' => t('Operations'), 'colspan' => '3')
  );

  $output = '';
  $rows = array();
  if (isset($form['weeks'])) {
    foreach (element_children($form['weeks']) as $key) {
      $rows[] = array(
        drupal_render($form['weeks'][$key]['number']),
        drupal_render($form['weeks'][$key]['name']),
        drupal_render($form['weeks'][$key]['firstgame']),
        drupal_render($form['weeks'][$key]['lastgame']),
        drupal_render($form['weeks'][$key]['edit_link']),
        drupal_render($form['weeks'][$key]['list_games_link']),
        drupal_render($form['weeks'][$key]['delete_link']),
      );
    }
  }
  $output = theme('table', array('header' => $header, 'rows' => $rows));

  return $output;
}


/**
 * Week edit form.
 */
function pickem_week_edit($form, &$form_state, $type, $gameset, $week) {

  // Save the original time zone and use the gameset time zone for rendering.
  $orig_timezone = date_default_timezone_get();
  date_default_timezone_set($gameset->timezone);

  if (is_null($week)) {
    // this should match the table field names
    $week = new stdClass();
    $week->wid = 0;
    $week->gsid = $gameset->gsid;
    $week->wk_number = 0;
    $week->wk_name = '';
    $week->wk_abbrev = '';
    $week->first_gamestart = 0;
    $week->last_gamestart = 0;
    $week->wk_points = 1;
    $week->playoffs = 0;
  }

  $form = array();
  $form['#gameset'] = $gameset;

  $form['week'] = array(
    'wid' => array(
      '#title' => 'Week ID',
      '#type' => 'hidden',
      '#default_value' => $week->wid,
    ),
    'gsid' => array(
      '#title' => 'Game Set ID',
      '#type' => 'hidden',
      '#default_value' => $gameset->gsid,
    ),

    'wk_number' => array(
      '#title' => 'Week Number',
      '#type' => 'textfield',
      '#size' => 3,
      '#maxlength' => 3,
      '#default_value' => $week->wk_number,
    ),
    'wk_name' => array(
      '#title' => 'Name',
      '#type' => 'textfield',
      '#size' => 30,
      '#maxlength' => 45,
      '#default_value' => $week->wk_name,
    ),
    'wk_abbrev' => array(
      '#title' => 'Abbrev',
      '#type' => 'textfield',
      '#size' => 10,
      '#maxlength' => 15,
      '#default_value' => $week->wk_abbrev,
    ),
    'wk_points' => array(
      '#title' => 'Points',
      '#type' => 'textfield',
      '#size' => 5,
      '#maxlength' => 5,
      '#default_value' => $week->wk_points,
    ),
    'playoffs' => array(
      '#title' => 'Week Type',
      '#type' => 'radios',
      '#default_value' => $week->playoffs,
      '#options' => array( 0 => t('Regular Season'), 1 => t('Playoffs') ),
      '#description' => t('This is used by some screens to limit the amount of content shown to the user.'),
    ),
    'first_gamestart' => array(
      '#title' => 'First game start time',
      '#type' => 'textfield',
      '#size' => 30,
      '#maxlength' => 35,
      '#default_value' => !empty($week->first_gamestart) ? format_date($week->first_gamestart, 'custom', 'Y-m-d H:i:s') : '',
      '#description' => t('Format: %time.', array('%time' => !empty($week->first_gamestart) ? format_date($week->first_gamestart, 'custom', 'Y-m-d H:i:s') : format_date(time(), 'custom', 'Y-m-d H:i:s'))) .
      '<br/>' . t('First game start time may be left blank as it will update when you add/edit games.') .
      '<br/>' . t('The timezone is the timezone of the gameset.'),
    ),
    'last_gamestart' => array(
      '#title' => 'Last game start time',
      '#type' => 'textfield',
      '#size' => 30,
      '#maxlength' => 35,
      '#default_value' => !empty($week->last_gamestart) ? format_date($week->last_gamestart, 'custom', 'Y-m-d H:i:s') : '',
      '#description' => t('Format: %time.', array('%time' => !empty($week->last_gamestart) ? format_date($week->last_gamestart, 'custom', 'Y-m-d H:i:s') : format_date(time(), 'custom', 'Y-m-d H:i:s'))) .
      '<br/>' . t('Last game start time may be left blank as it will update when you add/edit games.') .
      '<br/>' . t('The timezone is the timezone of the gameset.'),
    ),

  );

  $form['submit'] = array('#type' => 'submit', '#value' => t('Save'));

  // Restore original time zone.
  date_default_timezone_set($orig_timezone);

  return $form;
}


/**
 * Week edit form validation function.
 */
function pickem_week_edit_validate($form, &$form_state) {
  $week = $form_state['values'];
  if (!empty($week['first_gamestart']) && strtotime($week['first_gamestart']) == '' ) {
    form_set_error('first_gamestart', t('Bad date format, use: %time', array('%time' => format_date(time(), 'custom', 'Y-m-d H:i:s'))));
  }
  if (!empty($week['last_gamestart']) && strtotime($week['last_gamestart']) == '' ) {
    form_set_error('last_gamestart', t('Bad date format, use: %time', array('%time' => format_date(time(), 'custom', 'Y-m-d H:i:s'))));
  }
}


/*
 * Week edit submit handler.
 */
function pickem_week_edit_submit($form, &$form_state) {
  $week = $form_state['values'];

  // Save the original time zone and use the gameset time zone for rendering.
  $orig_timezone = date_default_timezone_get();
  date_default_timezone_set($form['#gameset']->timezone);

  db_merge('pickem_weeks')
    ->key(array(
      'wid' => $week['wid'],
    ))
    ->fields(array(
      'gsid' => $week['gsid'],
      'wk_number' => $week['wk_number'],
      'wk_name' => $week['wk_name'],
      'wk_abbrev' => $week['wk_abbrev'],
      'first_gamestart' => !empty($week['first_gamestart']) ? strtotime($week['first_gamestart']) : NULL,
      'last_gamestart' => !empty($week['last_gamestart']) ? strtotime($week['last_gamestart']) : NULL,
      'wk_points' => $week['wk_points'],
      'playoffs' => $week['playoffs'],
    ))
    ->execute();

  // Restore original time zone.
  date_default_timezone_set($orig_timezone);

  $t_string = t('Updated week %name.', array('%name' => $week['wk_name']));
  drupal_set_message($t_string);
  watchdog('pickem', $t_string, NULL, WATCHDOG_NOTICE);

  cache_clear_all('pickem_', 'cache', TRUE);
  $form_state['redirect'] = 'admin/pickem/weeks/' . $week['gsid'];
}


/**
 * Week delete confirmation form.
 */
function pickem_week_delete_confirm($form, &$form_state, $gameset, $week) {
  $form['#week'] = $week;
  $caption = '<p>' . t('This action cannot be undone.') . '</p>';
  return confirm_form($form, t('Are you sure you want to delete the week %name?', array('%name' => $week->wk_name)), 'admin/pickem/weeks', $caption, t('Delete'));
}


/*
 * Week delete submit handler.
 * Delete a gameset-week and related games.
 */
function pickem_week_delete_confirm_submit($form, &$form_state) {
  $week = $form['#week'];
  db_query("DELETE FROM {pickem_weeks} WHERE wid=:wid", array(':wid' => $week->wid));
  db_query("DELETE FROM {pickem_games} WHERE wid=:wid", array(':wid' => $week->wid));

  $t_string = t('Deleted week %name.', array('%name' => $week->wk_name));
  drupal_set_message($t_string);
  watchdog('pickem', $t_string, NULL, WATCHDOG_NOTICE);

  $form_state['redirect'] = 'admin/pickem/weeks/' . $week->gsid;
}


/*
 * Game list form.
 */
function pickem_games_list($form, &$form_state=NULL, $week) {

  $gameset = gameset_load($week->gsid);

  $breadcrumb = drupal_get_breadcrumb();
  $breadcrumb[] = l($gameset->gs_name, 'admin/pickem/gamesets');
  $breadcrumb[] = l($week->wk_name, 'admin/pickem/weeks/' . $week->gsid . '/list');
  drupal_set_breadcrumb($breadcrumb);

  // Save the original time zone and use the gameset time zone for rendering.
  $orig_timezone = date_default_timezone_get();
  date_default_timezone_set($gameset->timezone);

  $form = array('#tree' => TRUE);
  $form['#gameset'] = $gameset;
  $teams = get_teams();
  $games = get_games(array('week_id' => $week->wid));
  foreach ($games as $gid => $game) {
    if ( variable_get('pickem_prefer_long_team_names', 0) == 1 ) {
      $home_team = $teams[$game->h_id]->long_name;
      $vis_team = $teams[$game->v_id]->long_name;
    }
    else {
      $home_team = $teams[$game->h_id]->short_name;
      $vis_team = $teams[$game->v_id]->short_name;
    }

    if ($game->neutral_venue == 1) {
      $matchup = "$vis_team vs $home_team (" . format_date($game->gamestart, 'medium') . ")";
    }
    else {
      $matchup = "$vis_team @ $home_team (" . format_date($game->gamestart, 'medium') . ")";
    }

    $form['games'][] = array(
      'matchup' => array(
        '#markup' => $matchup
      ),
      'edit_link' => array(
        '#markup' => l(t('edit'), 'admin/pickem/games/' . $week->wid . '/edit/' . $gid)
      ),
      'delete_link' => array(
        '#markup' => l(t('delete'), 'admin/pickem/games/' . $week->wid . '/delete/' . $gid)
      ),
    );
  }

  // Restore original time zone.
  date_default_timezone_set($orig_timezone);

  return $form;
}


/*
 * Games list theme function.
 */
function theme_pickem_games_list($variables) {
  $form = $variables['form'];

  $header = array(
    t('Matchup') . ' (' . $form['#gameset']->timezone . ')',
    array('data' => t('Operations'), 'colspan' => '3')
  );

  $output = '';
  $rows = array();
  if (isset($form['games'])) {
    foreach (element_children($form['games']) as $key) {
      $rows[] = array(
        drupal_render($form['games'][$key]['matchup']),
        drupal_render($form['games'][$key]['edit_link']),
        drupal_render($form['games'][$key]['delete_link']),
      );
    }
  }
  $output = theme('table', array('header' => $header, 'rows' => $rows));
  return $output;
}


/*
 * Game edit form.
 */
function pickem_game_edit($form, &$form_state, $type, $week, $game) {

  $gameset = gameset_load($week->gsid);

  // Save the original time zone and use the gameset time zone for rendering.
  $orig_timezone = date_default_timezone_get();
  date_default_timezone_set($gameset->timezone);

  if (is_null($game)) {
    // this should match the table field names
    $game = new stdClass();
    $game->gid = 0;
    $game->wid = $week->wid;
    $game->gamestart = $week->first_gamestart;
    $game->h_id = '';
    $game->v_id = '';
    $game->h_spread = '';
    $game->v_spread = '';
    $game->h_score = '';
    $game->v_score = '';
    $game->neutral_venue = 0;
  }

  $teams = get_teams(array('gameset_id' => $week->gsid));
  $team_list = array();
  $team_list[0] = "";
  foreach ($teams as $t_id => $team_data) {
    if ( variable_get('pickem_prefer_long_team_names', 0) == 1 ) {
      $team_list[$t_id] = $team_data->long_name;
    }
    else {
      $team_list[$t_id] = $team_data->short_name;
    }
  }

  $form = array();
  $form['#gameset'] = $gameset;
  $form['game']['gid'] = array(
    '#title' => t('Game ID'),
    '#type' => 'hidden',
    '#value' => $game->gid
  );
  $form['game']['wid'] = array(
    '#title' => t('Week ID'),
    '#type' => 'hidden',
    '#value' => $game->wid,
  );
  $form['game']['gamestart'] = array(
    '#title' => t('Game Time'),
    '#type' => 'textfield',
    '#size' => 30,
    '#maxlength' => 45,
    '#default_value' => !empty($game->gamestart) ? format_date($game->gamestart, 'custom', 'Y-m-d H:i:s') : '',
    '#description' => t('Format: %time', array('%time' => !empty($game->gamestart) ? format_date($game->gamestart, 'custom', 'Y-m-d H:i:s') : format_date(time(), 'custom', 'Y-m-d H:i:s'))) . '<br/>' .
    t('The timezone is the timezone of the gameset.'),
    '#required' => TRUE,
  );
  $form['game']['visitor_team'] = array(
    '#title' => t('Visitor Team'),
    '#type' => 'select',
    '#default_value' => $game->v_id,
    '#options' => $team_list,
    '#required' => TRUE,
  );
  $form['game']['v_spread'] = array(
    '#title' => t('Visitor Spread'),
    '#type' => 'textfield',
    '#size' => 4,
    '#maxlength' => 10,
    '#default_value' => $game->v_spread,
  );
  $form['game']['v_score'] = array(
    '#title' => t('Visitor Score'),
    '#type' => 'textfield',
    '#size' => 4,
    '#maxlength' => 10,
    '#default_value' => $game->v_score,
  );
  $form['game']['home_team'] = array(
    '#title' => t('Home Team'),
    '#type' => 'select',
    '#default_value' => $game->h_id,
    '#options' => $team_list,
    '#required' => TRUE,
  );
  $form['game']['h_spread'] = array(
    '#title' => t('Home Spread'),
    '#type' => 'textfield',
    '#size' => 4,
    '#maxlength' => 10,
    '#default_value' => $game->h_spread,
  );
  $form['game']['h_score'] = array(
    '#title' => t('Home Score'),
    '#type' => 'textfield',
    '#size' => 4,
    '#maxlength' => 10,
    '#default_value' => $game->h_score,
  );
  $form['game']['neutral_venue'] = array(
    '#type' => 'checkbox',
    '#title' => t('Neutral venue'),
    '#return_value' => 1,
    '#default_value' => $game->neutral_venue,
    '#description' => t("Is the game taking place at a neutral venue?"),
  );

  $form['submit'] = array('#type' => 'submit', '#value' => t('Save'));

  // Restore original time zone.
  date_default_timezone_set($orig_timezone);

  return $form;
}


/**
 * Game edit form validation function.
 */
function pickem_game_edit_validate($form, &$form_state) {
  $game = $form_state['values'];

  if ( $game['v_spread'] != -$game['h_spread']) {
    form_set_error('h_spread', t('The spread fields must be equal and opposite (ie 3 and -3).'));
    form_set_error('v_spread', t('The spread fields must be equal and opposite (ie 3 and -3).'));
  }

  if ( strtotime($game['gamestart']) == '' ) {
    form_set_error('gamestart', t('Bad date format, use: %time', array('%time' => format_date(time(), 'custom', 'Y-m-d H:i:s'))));
  }
}


/*
 * Game edit submit handler.
 */
function pickem_game_edit_submit($form, &$form_state) {
  $game = $form_state['values'];

  // clear pickem_ caches, since you are usually updating scores here.
  cache_clear_all('pickem_', 'cache', TRUE);

  // Save the original time zone and use the gameset time zone for rendering.
  $orig_timezone = date_default_timezone_get();
  date_default_timezone_set($form['#gameset']->timezone);

  db_merge('pickem_games')
    ->key(array(
      'gid' => $game['gid'],
    ))
    ->fields(array(
      'wid' => $game['wid'],
      'gamestart' => !empty($game['gamestart']) ? strtotime($game['gamestart']) : NULL,
      'h_id' => $game['home_team'],
      'h_spread' => !empty($game['h_spread']) ? $game['h_spread'] : NULL,
      'h_score' => !empty($game['h_score']) ? $game['h_score'] : NULL,
      'v_id' => $game['visitor_team'],
      'v_spread' => !empty($game['v_spread']) ? $game['v_spread'] : NULL,
      'v_score' => !empty($game['v_score']) ? $game['v_score'] : NULL,
      'neutral_venue' => $game['neutral_venue'],
    ))
    ->execute();

  // Update the first_gamestart field in the weeks table.
  $query = 'update {pickem_weeks} w set w.first_gamestart = (select min(g.gamestart) from {pickem_games} g where g.wid=w.wid) where w.wid=:wid';
  db_query($query, array(':wid' => $game['wid']));

  // Update the last_gamestart field in the weeks table.
  $query = 'update {pickem_weeks} w set w.last_gamestart = (select max(g.gamestart) from {pickem_games} g where g.wid=w.wid) where w.wid=:wid';
  db_query($query, array(':wid' => $game['wid']));

  // Restore original time zone.
  date_default_timezone_set($orig_timezone);

  drupal_set_message(t('Game saved.'));
  watchdog('pickem', 'Game saved.', NULL, WATCHDOG_NOTICE);

  $form_state['redirect'] = array('admin/pickem/games/' . $game['wid']);
}


/**
 * Game delete confirmation form.
 */
function pickem_game_delete_confirm($form, &$form_state, $week, $game) {
  $form['#game'] = $game;
  $caption = '<p>' . t('This action cannot be undone.') . '</p>';
  return confirm_form($form, check_plain(t('Are you sure you want to delete this game?')), 'admin/pickem/games/' . $week->wid, $caption, t('Delete'));
}


/*
 * Game delete submit handler.
 */
function pickem_game_delete_confirm_submit($form, &$form_state) {
  $game = $form['#game'];
  db_query('DELETE FROM {pickem_games} WHERE gid=:gid', array(':gid' => $game->gid));

  // Delete any picks as well since even if this game were re-added, the id would be different.
  db_query('DELETE FROM {pickem_picks} WHERE gid=:gid', array(':gid' => $game->gid));

  // Update the first_gamestart field in the weeks table.
  $query = 'update {pickem_weeks} w set w.first_gamestart = (select min(g.gamestart) from {pickem_games} g where g.wid=w.wid) where w.wid=:wid';
  db_query($query, array(':wid' => $game->wid));

  // Update the last_gamestart field in the weeks table.
  $query = 'update {pickem_weeks} w set w.last_gamestart = (select max(g.gamestart) from {pickem_games} g where g.wid=w.wid) where w.wid=:wid';
  db_query($query, array(':wid' => $game->wid));

  $t_string = t('Deleted game id %gameid.', array('%gameid' => $game->gid));
  drupal_set_message($t_string);
  watchdog('pickem', $t_string, NULL, WATCHDOG_NOTICE);

  $form_state['redirect'] = array('admin/pickem/games/' . $game->wid);
}


/*
 * Teams list form.
 */
function pickem_teams_list($form, &$form_state, $gameset) {

  $breadcrumb = drupal_get_breadcrumb();
  $breadcrumb[] = l($gameset->gs_name, 'admin/pickem/gamesets');
  drupal_set_breadcrumb($breadcrumb);

  $form = array('#tree' => TRUE);

  $teams = get_teams(array('gameset_id' => $gameset->gsid));
  foreach ($teams as $tid => $team) {
    $form['teams'][$tid] = array(
      'long_name' => array(
        '#markup' => $team->long_name
      ),
      'conference' => array(
        '#markup' => $team->conference
      ),
      'edit_link' => array(
        '#markup' => l(t('edit'), 'admin/pickem/teams/' . $gameset->gsid . '/edit/' . $tid)
      ),
      'delete_link' => array(
        '#markup' => l(t('delete'), 'admin/pickem/teams/' . $gameset->gsid . '/delete/' . $tid)
      ),
    );
  }

  return $form;
}


/*
 * Teams list theme function.
 */
function theme_pickem_teams_list($variables) {
  $form = $variables['form'];

  $header = array(
    t('Team'),
    t('Conference'),
    array('data' => t('Operations'), 'colspan' => '3')
  );

  $output = '';
  $rows = array();
  if (isset($form['teams'])) {
    foreach (element_children($form['teams']) as $key) {
      $rows[] = array(
        drupal_render($form['teams'][$key]['long_name']),
        drupal_render($form['teams'][$key]['conference']),
        drupal_render($form['teams'][$key]['edit_link']),
        drupal_render($form['teams'][$key]['delete_link']),
      );
    }
  }
  $output = theme('table', array('header' => $header, 'rows' => $rows));

  return $output;
}


/*
 * Team edit form.
 */
function pickem_team_edit($form, &$form_state, $type, $gameset, $team) {

  if ( is_null($team) ) {
    // this should match the table field names
    $team = new stdClass();
    $team->tid = 0;
    $team->gsid = $gameset->gsid;
    $team->tm_city = '';
    $team->tm_abbrev = '';
    $team->tm_nick = '';
    $team->tm_name = '';
    $team->conference = '';
    $team->division = '';
    $team->assetid = '';
  }

  $form = array();
  $form['team']['tid'] = array(
      '#title'  => t('Team ID'),
      '#type' => 'hidden',
      '#size' => 20,
      '#maxlength' => 50,
      '#default_value' => $team->tid
  );
  $form['team']['gsid'] = array(
      '#title'  => t('Game Set ID'),
      '#type' => 'hidden',
      '#size' => 20,
      '#maxlength' => 50,
      '#default_value' => $team->gsid
  );

  $form['team']['tm_city'] = array(
      '#title'  => t('City'),
      '#type' => 'textfield',
      '#size' => 30,
      '#maxlength' => 50,
      '#default_value' => $team->tm_city
  );
  $form['team']['tm_abbrev'] = array(
    '#title'  => t('Abbreviation(5 max)'),
    '#type' => 'textfield',
    '#size' => 5,
    '#maxlength' => 5,
    '#default_value' => $team->tm_abbrev
  );
  $form['team']['tm_nick'] = array(
    '#title'  => t('Nickname'),
    '#type' => 'textfield',
    '#size' => 30,
    '#maxlength' => 50,
    '#default_value' => $team->tm_nick
  );
  $form['team']['tm_name'] = array(
    '#title'  => t('Name'),
    '#type' => 'textfield',
    '#size' => 30,
    '#maxlength' => 50,
    '#default_value' => $team->tm_name
  );
  $form['team']['conference'] = array(
    '#title'  => t('Conference'),
    '#type' => 'textfield',
    '#size' => 30,
    '#maxlength' => 50,
    '#default_value' => $team->conference
  );
  $form['team']['division'] = array(
    '#title'  => t('Division'),
    '#type' => 'textfield',
    '#size' => 30,
    '#maxlength' => 50,
    '#default_value' => $team->division
  );
  $form['team']['assetid'] = array(
    '#title'  => t('Asset ID'),
    '#description' => t('ID used to locate assets(e.g. logos).  Usually lowercase team abbreviation.'),
    '#type' => 'textfield',
    '#size' => 5,
    '#maxlength' => 50,
    '#default_value' => $team->assetid
  );

  $form['submit'] = array('#type' => 'submit', '#value' => t('Save'));

  return $form;
}


/*
 * Team edit form submit handler.
 */
function pickem_team_edit_submit($form, &$form_state) {
  $team = $form_state['values'];

  if ( $team['tid'] == 0 ) {
    $query = "insert into {pickem_teams} (gsid, tm_city, tm_abbrev, tm_name, tm_nick, conference, division, assetid) values(:gsid, :tm_city, :tm_abbrev, :tm_name, :tm_nick, :conference, :division, :assetid)";
    $result = db_query($query, array(':gsid' => $team['gsid'], ':tm_city' => $team['tm_city'], ':tm_abbrev' => $team['tm_abbrev'], ':tm_name' => $team['tm_name'], ':tm_nick' => $team['tm_nick'], ':conference' => $team['conference'], ':division' => $team['division'], ':assetid' => $team['assetid']));

    $t_string = t('Added team %name.', array('%name' => $team['tm_name']));
    drupal_set_message($t_string);
    watchdog('pickem', $t_string, NULL, WATCHDOG_NOTICE);
  }
  else {
    $query = "update {pickem_teams} set gsid=:gsid, tm_city=:tm_city, tm_abbrev=:tm_abbrev, tm_name=:tm_name, tm_nick=:tm_nick, conference=:conference, division=:division, assetid=:assetid where tid=:tid";
    $result = db_query($query, array(':gsid' => $team['gsid'], ':tm_city' => $team['tm_city'], ':tm_abbrev' => $team['tm_abbrev'], ':tm_name' => $team['tm_name'], ':tm_nick' => $team['tm_nick'], ':conference' => $team['conference'], ':division' => $team['division'], ':assetid' => $team['assetid'], ':tid' => $team['tid']));

    $t_string = t('Updated team %name.', array('%name' => $team['tm_name']));
    drupal_set_message($t_string);
    watchdog('pickem', $t_string, NULL, WATCHDOG_NOTICE);
  }

  $form_state['redirect'] = 'admin/pickem/teams/' . $team['gsid'];
}


/**
 * Team delete confirmation form.
 */
function pickem_team_delete_confirm($form, &$form_state, $gameset, $team) {
  $form['#team'] = $team;
  $caption = '';
  $caption .= '<p>' . t('This action cannot be undone.') . '</p>';
  return confirm_form($form, t('Are you sure you want to delete the team %name from %gameset?', array('%name' => $team->tm_city . ' ' . $team->tm_name, '%gameset' => $gameset->gs_name)), 'admin/pickem/teams', $caption, t('Delete'));
}


/*
 * Team delete submit handler.
 */
function pickem_team_delete_confirm_submit($form, &$form_state) {
  $team = $form['#team'];
  db_query("DELETE FROM {pickem_teams} WHERE tid = :tid", array(':tid' => $team->tid));

  $t_string = t('Deleted team %name.', array('%name' => $team->tm_city . ' ' . $team->tm_name));
  drupal_set_message($t_string);
  watchdog('pickem', $t_string, NULL, WATCHDOG_NOTICE);

  $form_state['redirect'] = 'admin/pickem/teams/' . $team->gsid;
}


/*
 * League list form.
 */
function pickem_leagues_list($form, &$form_state) {

  $form = array('#tree' => TRUE);
  $gamesets = get_gamesets();
  $result = db_query("SELECT * from {pickem_leagues} order by lid");
  foreach ($result as $record) {
    $form['leagues'][$record->lid] = array(
      'name' => array(
        '#markup' => $record->name
      ),
      'gs_name' => array(
        '#markup' => !empty($gamesets[$record->gsid]->gs_name) ? $gamesets[$record->gsid]->gs_name : ''
      ),
      'edit_link' => array(
        '#markup' => l(t('edit league'), 'admin/pickem/leagues/edit/' . $record->lid)
      ),
      'list_users_link' => array(
        '#markup' => l(t('users'), 'admin/pickem/users/' . $record->lid)
      ),
      'enter_scores_link' => array(
        '#markup' => l(t('scores'), 'admin/pickem/scores/' . $record->lid)
      ),
      'delete_link' => array(
        '#markup' => l(t('delete league'), 'admin/pickem/leagues/delete/' . $record->lid)
      ),
    );
  }

  return $form;
}


/*
 * League list theme function.
 */
function theme_pickem_leagues_list($variables) {
  $form = $variables['form'];

  $header = array(
    t('League'),
    t('Game Set'),
    array('data' => t('Operations'), 'colspan' => '4')
  );

  $output = '';
  $rows = array();
  if (isset($form['leagues'])) {
    foreach (element_children($form['leagues']) as $key) {
      $rows[] = array(
        drupal_render($form['leagues'][$key]['name']),
        drupal_render($form['leagues'][$key]['gs_name']),
        drupal_render($form['leagues'][$key]['edit_link']),
        drupal_render($form['leagues'][$key]['list_users_link']),
        drupal_render($form['leagues'][$key]['enter_scores_link']),
        drupal_render($form['leagues'][$key]['delete_link'])
      );
    }
  }

  $output .= theme('table', array('header' => $header, 'rows' => $rows));

  return $output;
}


/**
 * League edit form.
 */
function pickem_league_edit($form, &$form_state, $type, $league) {

  if ( is_null($league) ) {
    // this should match the table field names
    $league = new stdClass();
    $league->lid = 0;
    $league->name = '';
    $league->forum_id = 0;
    $league->rules_node_id = '';
    $league->scoring_type = 0;
    $league->gsid = 0;
    $league->partial_league = 0;
    $league->pick_lock = PICKEM_PICK_LOCK_WEEK;
    $league->show_schedule_grid = 0;
  }
  $league_forums_options = league_forums();
  $league_gameset_options = gameset_options();

  $form = array();
  $form['league']['lid'] = array(
    '#title'  => t('League ID'),
    '#type' => 'hidden',
    '#size' => 20,
    '#maxlength' => 50,
    '#default_value' => $league->lid
  );
  $form['league']['name'] = array(
    '#title'  => t('Name'),
    '#required' => TRUE,
    '#type' => 'textfield',
    '#size' => 50,
    '#maxlength' => 50,
    '#default_value' => $league->name
  );
  $form['league']['gsid'] = array(
    '#title'  => t('Game Set'),
    '#required' => TRUE,
    '#type' => 'select',
    '#default_value' => $league->gsid,
    '#options' => $league_gameset_options,
  );
  $form['league']['forum_id'] = array(
    '#title'  => t('Forum ID'),
    '#type' => 'select',
    '#default_value' => $league->forum_id,
    '#options' => $league_forums_options,
   );
  $form['league']['rules_node_id'] = array(
    '#title'  => t('Rules Node ID'),
    '#type' => 'textfield',
    '#size' => 5,
    '#maxlength' => 5,
    '#default_value' => $league->rules_node_id
   );
  $form['league']['scoring_type'] = array(
    '#title'  => t('Scoring Type'),
    '#type' => 'radios',
    '#default_value' => $league->scoring_type,
    '#options' => get_scoring_engine_name_options(),
    '#description' => get_scoring_engine_descriptions()
  );
  $form['league']['partial_league'] = array(
    '#type' => 'checkbox',
    '#title' => t('Partial league support'),
    '#return_value' => 1,
    '#default_value' => $league->partial_league,
    '#description' => t("If you don't pick every game for every team, use this setting.  It will disable the screens that compare team's records (which will be incorrect for cherry-picked leagues.)"),
  );
  $form['league']['show_schedule_grid'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show schedule grid'),
    '#return_value' => 1,
    '#default_value' => $league->show_schedule_grid,
    '#description' => t("Show the schedule grid page. The schedule grid page doesn't work for game sets where a team may play more than one game per week."),
  );
  $form['league']['pick_lock'] = array(
    '#type' => 'radios',
    '#title' => t('Pick lockup'),
    '#options' => array( PICKEM_PICK_LOCK_WEEK => t('Start of each week'), PICKEM_PICK_LOCK_GAME => t('Start of each game') ),
    '#default_value' => $league->pick_lock,
    '#description' => t('When should the users no longer be able to change their picks.'),
  );

  $form['submit'] = array('#type' => 'submit', '#value' => t('Save'));

  return $form;
}


/*
 * League edit submit handler.
 */
function pickem_league_edit_submit($form, &$form_state) {
  $league = $form_state['values'];

  // Clear the pickem_* caches in case you change league scoring type.
  cache_clear_all('pickem_', 'cache', TRUE);

  if ($league['rules_node_id'] == '') {
    $league['rules_node_id'] = 0;
  }
  if ($league['forum_id'] == '') {
    $league['forum_id'] = 0;
  }

  if ( $league['lid'] == 0 ) {
    $game_id = db_insert('pickem_leagues')
      ->fields(array(
        'name' => $league['name'],
        'rules_node_id' => $league['rules_node_id'],
        'forum_id' => $league['forum_id'],
        'scoring_type' => $league['scoring_type'],
        'gsid' => $league['gsid'],
        'partial_league' => $league['partial_league'],
        'pick_lock' => $league['pick_lock'],
        'show_schedule_grid' => $league['show_schedule_grid'],
      ))
      ->execute();

    $t_string = t('Added league %league-name.', array('%league-name' => $league['name']));
    drupal_set_message($t_string);
    watchdog('pickem', $t_string, NULL, WATCHDOG_NOTICE);
  }
  else {
    db_update('pickem_leagues')
      ->fields(array(
        'name' => $league['name'],
        'rules_node_id' => $league['rules_node_id'],
        'forum_id' => $league['forum_id'],
        'scoring_type' => $league['scoring_type'],
        'gsid' => $league['gsid'],
        'partial_league' => $league['partial_league'],
        'pick_lock' => $league['pick_lock'],
        'show_schedule_grid' => $league['show_schedule_grid'],
      ))
      ->condition('lid', $league['lid'])
      ->execute();

    $t_string = t('Updated league %league-name.', array('%league-name' => $league['name']));
    drupal_set_message($t_string);
    watchdog('pickem', $t_string, NULL, WATCHDOG_NOTICE);
  }

  $form_state['redirect'] = 'admin/pickem/leagues';
}


/**
 * Lague delete confirmation form.
 */
function pickem_league_delete_confirm($form, &$form_state, $league) {
  // Always provide id in the same form key as in the edit form.
  $form['lid'] = array('#type' => 'value', '#value' => $league->lid);
 return confirm_form(
    $form,
    t('Are you sure you want to delete the team %name?', array('%name' => $league->name)),
    'admin/pickem/leagues',
    t('This action cannot be undone.'),
    t('Delete'),
    t('Cancel')
  );
}


/*
 * League delete submit handler.
 */
function pickem_league_delete_confirm_submit($form, &$form_state) {
  if ($form_state['values']['confirm']) {
    $league = league_load($form_state['values']['lid']);
    pickem_league_delete($league->lid);
    watchdog('pickem', 'League: deleted %name.', array('%name' => $league->name));
    drupal_set_message(t('League %name has been deleted.', array('%name' => $league->name)));
    $form_state['redirect'] = 'admin/pickem/leagues';
  }
}

/**
 * Delete a league.
 *
 * @param $lid
 * A league ID.
 */
function pickem_league_delete($lid) {
 pickem_league_delete_multiple(array($lid));
}


/**
 * Delete multiple leagues.
 *
 * @param $lids
 * An array of league IDs.
 */
function pickem_league_delete_multiple($lids) {
  $transaction = db_transaction();
  if (!empty($lids)) {
    try {
      db_delete('pickem_leagues')
      ->condition('lid', $lids, 'IN')
      ->execute();
    }
    catch (Exception $e) {
      $transaction->rollback();
      watchdog_exception('league', $e);
      throw $e;
    }
  }
}


/*
 * League users list form.
 */
function pickem_users_list($form, &$form_state, $league) {

  $form = array(
    '#theme' => 'pickem_users_list',
    '#tree' => TRUE,
  );

  $users = get_users($league, 0, variable_get('pickem_full_user_load', 0));
  foreach ( $users as $user ) {
    $form['users'][] = array(
      'user_link' => array(
        '#markup' => theme('username', array('account' => $user->user_obj))
      ),
      'remove_link' => array(
        '#markup' => l(t('Remove'), 'admin/pickem/users/' . $league->lid . '/remove/' . $user->uid)
      ),
    );
  }

  return $form;
}


/*
 * League users list theme function.
 */
function theme_pickem_users_list($variables) {
  $form = $variables['form'];

  $header = array(
    t('User'),
    array('data' => t('Operations'), 'colspan' => '3')
  );

  $output = '';
  $rows = array();
  if (isset($form['users'])) {
    foreach (element_children($form['users']) as $key) {
      $rows[] = array(
        drupal_render($form['users'][$key]['user_link']),
        drupal_render($form['users'][$key]['remove_link'])
      );
    }
  }
  $output .= theme('table', array('header' => $header, 'rows' => $rows));

  return $output;
}


/*
 * League users add form.
 */
function pickem_users_add($form, &$form_state, $league) {
  // Don't use drupal_get_form here.
  $form = user_select_form($form, $form_state, $league, 'Add user', 1);
  // Submit button.
  $form['submit'] = array('#type' => 'submit', '#value' => t('Add User'));
  // Don't need the op value.
  unset($form['op']);
  // Submit handler.
  $form['#submit'][] = 'pickem_users_add_submit';  // add submit handler
  return $form;
}


/*
 * League users add submit handler.
 */
function pickem_users_add_submit($form, &$form_state) {
  $league = $form['#league'];
  $user_id = $form_state['values']['user_select'];
  $user = user_load($user_id);

  db_insert('pickem_users')
    ->fields(array(
      'lid' => $league->lid,
      'uid' => $user_id,
    ))
    ->execute();




  // Clear the pickem_* caches so user will show up on various screens.
  cache_clear_all('pickem_', 'cache', TRUE);

  $t_string = t('"%name" was added to league "%league_name".', array('%name' => $user->name, '%league_name' => $league->name));
  drupal_set_message($t_string);
  watchdog('pickem', $t_string, NULL, WATCHDOG_NOTICE);

  $form_state['redirect'] = 'admin/pickem/users/' . $league->lid;
}


/*
 * League users remove confirm form.
 */
function pickem_users_remove_confirm($form, &$form_state, $league, $user) {
  $form['#user'] = $user;
  $form['#league'] = $league;
  $caption = '<p>' . t('This action cannot be undone.') . '</p>';
  return confirm_form($form, t('Are you sure you want to remove "%name" from league "%league_name"?', array('%name' => $user->name, '%league_name' => $league->name)), 'admin/pickem/users/' . $league->lid, $caption, t('Delete'));
}


/*
 * League users confirm submit handler.
 */
function pickem_users_remove_confirm_submit($form, &$form_state) {
  $user = $form['#user'];
  $league = $form['#league'];
  db_query("delete from {pickem_users} where lid=:lid and uid=:uid", array(':lid' => $league->lid, ':uid' => $user->uid));

  // Clear the pickem_* caches so user won't show up on various screens.
  cache_clear_all('pickem_', 'cache', TRUE);

  $t_string = t('"%name" was removed from league "%league_name".', array('%name' => $user->name, '%league_name' => $league->name));
  drupal_set_message($t_string);
  watchdog('pickem', $t_string, NULL, WATCHDOG_NOTICE);

  $form_state['redirect'] = 'admin/pickem/users/' . $league->lid;
}


/*
 * Scores list form.
 */
function pickem_scores_list($form, $form_state, $league) {

  $form = array('#tree' => TRUE);

  $games = get_games(array('league_id' => $league->lid), array('by_week' => 1));
  $teams = get_teams(array('gameset_id' => $league->gsid));

  $last_wid = 0;
  foreach ( $games as $wid => $weeks_games ) {

    foreach ( $weeks_games as $gid => $game ) {

      if ($last_wid != $wid) {
        $form['scores'][$wid]['week_link'] = array(
          '#markup' => l($game->wk_number, 'admin/pickem/scores/' . $league->lid . '/edit/' . $wid)
         );
         $last_wid = $wid;
      }

      $v_score = $game->v_score <> ''?$game->v_score:'&nbsp;';
      $h_score = $game->h_score <> ''?$game->h_score:'&nbsp;';

      $form['scores'][$wid]['weeks_games'][] = array(
        '#markup' => '<div class="game-list-horiz">' . $teams[$game->v_id]->short_name . " $v_score<br/>" . $teams[$game->h_id]->short_name . " $h_score</div>"
      );
    }
  }
  return $form;
}


/*
 * Scores list theme function.
 */
function theme_pickem_scores_list($variables) {
  $form = $variables['form'];

  $rows = array();
  if (isset($form['scores'])) {
    foreach (element_children($form['scores']) as $key) {
      $rows[] = array(
        array('data' => drupal_render($form['scores'][$key]['week_link'])),
        array('data' => drupal_render($form['scores'][$key]['weeks_games']))
      );
    }
  }

  $header = array(
    t('Week'),
    t('Matchups')
  );

  $output = '<div class="pickem">';
  $output .= theme('table', array('header' => $header, 'rows' => $rows));
  $output .= '</div>';

  return $output;
}


/*
 * Week's scores edit form.
 */
function pickem_scores_week_edit($form, $form_state, $league, $week) {

  $gameset = gameset_load($week->gsid);
  $form = array(
    '#tree' => TRUE,
    '#gameset' => $gameset,
  );

  // Save the original time zone and set the gameset time zone for time rendering.
  $orig_timezone = date_default_timezone_get();
  date_default_timezone_set($gameset->timezone);

  $games = get_games(array('week_id' => $week->wid));
  $teams = get_teams(array('gameset_id' => $league->gsid));
  $team_list = array();
  foreach ($teams as $t_id => $team_data) {
    if ( variable_get('pickem_prefer_long_team_names', 0) == 1 ) {
      $team_list[$t_id] = $team_data->long_name;
    }
    else {
      $team_list[$t_id] = $team_data->short_name;
    }
  }

  foreach ($games as $gid => $game) {
    $form['weeks_games'][$gid]['home_team'] = array(
      '#markup' => $team_list[$game->h_id],
    );
    $form['weeks_games'][$gid]['h_score'] = array(
      '#type' => 'textfield',
      '#size' => 3,
      '#maxlength' => 5,
      '#default_value' => $game->h_score,
    );
    $form['weeks_games'][$gid]['visitor_team'] = array(
      '#markup' => $team_list[$game->v_id],
    );
    $form['weeks_games'][$gid]['v_score'] = array(
      '#type' => 'textfield',
      '#size' => 3,
      '#maxlength' => 5,
      '#default_value' => $game->v_score,
    );
    $form['weeks_games'][$gid]['gamestart'] = array(
      '#markup' => format_date($game->gamestart, 'medium'),
    );
  }

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save Scores'),
  );

  // Restore original time zone.
  date_default_timezone_set($orig_timezone);

  return $form;
}


/*
 * Week's scores edit theme function.
 */
function theme_pickem_scores_week_edit($variables) {
  $form = $variables['form'];

  $header = array(
    array('data' => t('Matchup'), 'colspan' => '4'),
    array('data' => t('Game time') . ' (' . $form['#gameset']->timezone . ')')
  );

  $rows = array();
  if (isset($form['weeks_games'])) {
    foreach (element_children($form['weeks_games']) as $gid) {
      $rows[] = array(
        drupal_render($form['weeks_games'][$gid]['home_team']),
        drupal_render($form['weeks_games'][$gid]['h_score']),
        drupal_render($form['weeks_games'][$gid]['visitor_team']),
        drupal_render($form['weeks_games'][$gid]['v_score']),
        drupal_render($form['weeks_games'][$gid]['gamestart']),
      );
    }
  }
  $output = theme('table', array('header' => $header, 'rows' => $rows));
  $output .= drupal_render_children($form);

  return $output;
}


/*
 * Week's scores edit submit handler.
 */
function pickem_scores_week_edit_submit($form, &$form_state) {

  foreach ($form_state['values']['weeks_games'] as $gid => $game) {

    if ($game['v_score'] == '') {
      $game['v_score'] = NULL;
    }
    if ($game['h_score'] == '') {
      $game['h_score'] = NULL;
    }

    db_update('pickem_games')
      ->fields(array(
        'v_score' => $game['v_score'],
        'h_score' => $game['h_score'],
      ))
      ->condition('gid', $gid)
      ->execute();

   }

  // clear pickem_ caches, since you are updating scores here.
  cache_clear_all('pickem_', 'cache', TRUE);
  drupal_set_message(t("Scores Saved"));
}













/**
 * implementation of hook_mail
 */
function pickem_mail($key, &$message, $params) {
  switch ($key) {
    case 'notice':
      $message['headers'] = $params['headers'];
      $message['subject'] = $params['subject'];
      $message['body'] = $params['body'];
      break;
  }
}


function page_admin_email_league() {
  $output = drupal_get_form('email_league_form');

  return $output;
}



function email_league_form($form, &$form_state) {
  global $user;

  $all_leagues = get_leagues();
  $leagues = array();
  foreach ($all_leagues as $l) {
    $leagues[$l->lid] = $l->name;
  }

  if (count($all_leagues) > 0) {
    $form['#attributes'] = array(
      'enctype' => "multipart/form-data"
    );
    $form['#token'] = $user->name . $user->mail;
    $form['contact_information'] = array(
      '#value' => filter_xss_admin(t('Send an e-mail message using the contact form below.'))
    );

    $form['subject'] = array(
      '#type'      => 'textfield',
      '#title'     => t('Subject'),
      '#maxlength' => 255,
      '#required'  => TRUE,
    );

    // If there is more than one category available and no default category has been selected,
    // prepend a default placeholder value.
    $form['league_id'] = array(
      '#type'          => 'select',
      '#title'         => t('Leagues'),
      '#options'       => $leagues,
      '#required'      => TRUE,
      '#multiple'      => TRUE,
    );

    $form['message'] = array(
      '#type'     => 'textarea',
      '#title'    => t('Message'),
      '#required' => TRUE,
    );
    $form['bcc'] = array(
      '#type'          => 'checkbox',
      '#title'         => t('Send as BCC (hide recipients)'),
      '#default_value' => 0,
    );
    $form['copy_yourself'] = array(
      '#type'          => 'checkbox',
      '#title'         => t('Send a copy to yourself'),
      '#default_value' => 0
    );
    $form['submit'] = array(
      '#type'  => 'submit',
      '#value' => t('Send e-mail'),
    );
  }
  else {
    $form['error'] = array(
      '#value' => '<p><b>' . t('You must create at least one category before using this form.') . '</b>',
    );
  }

  return $form;
}


function email_league_form_submit($form, &$form_state) {
  global $user;

  $bcc = $form_state['values']['bcc'];
  $copy_yourself = $form_state['values']['copy_yourself'];

  $return_address = variable_get('pickem_commisioner_email', '');
  if ( $return_address == '' ) {
    $return_address = $user->mail;
  }

  $headers['From'] = $headers['Reply-To'] = $headers['Sender'] = $headers['Return-Path'] = $headers['Errors-To'] = $return_address;

  $headers['Content-Type'] = 'text/plain; charset=UTF-8; format=flowed';
  //$headers['Content-Type'] = 'text/html; charset=UTF-8;';

  // Format the subject line:
  $subject = $form_state['values']['subject'];

   // Load the recipients
  $recipients = array();
  foreach ($form_state['values']['league_id'] as $league_id) {
    $league = league_load($league_id);
    $recipients = array_merge($recipients, get_users($league));
  }
  if (count($recipients) == 0) {
    drupal_set_message(t('There are no users in this league. Mail not sent.'));
    return '';
  }

  // Use bcc addressing.
  if ($bcc == 1) {
    $bcc_header = array();
    foreach ($recipients as $r) {
      if (!empty($r->mail)) {
        $bcc_header[] = $r->name . " <" . $r->mail . ">";
      }
    }
    $headers['Bcc'] = implode(",", $bcc_header);
    // Some email packages required a $to header.
    $to = $return_address;
    // Add $user if they wanted it, and they are not already on there.
    if ($copy_yourself == 1 &&
        (strpos($headers['Bcc'], "<" . $user->mail . ">") === FALSE) &&
        (strpos($to, "<" . $user->mail . ">") === FALSE)) {
      $headers['Bcc'] = $user->name . " <" . $user->mail . ">," . $headers['Bcc'];
    }
  }
  // Use normal addressing.
  else {
    $to_header = array();
    foreach ($recipients as $r) {
      if (!empty($r->mail)) {
        $to_header[] = $r->name . " <" . $r->mail . ">";
      }
    }
    $to = implode(",", $to_header);
    // Add $user if they wanted it, and they are not already on there.
    if ($copy_yourself == 1 && (strpos($to, "<" . $user->mail . ">") === FALSE)) {
      $to = $user->name . " <" . $user->mail . ">," . $to;
    }
  }

    // Add in the actual message
  $message_body = array(wordwrap($form_state['values']['message']));

  $params = array();
  $params['headers'] = $headers;
  $params['subject'] = $subject;
  $params['body'] = $message_body;

  $message = drupal_mail('pickem', 'notice', $to, language_default(), $params);
  if ($message['result'] == 1) {
    drupal_set_message(t('Email sent.'));
    watchdog('pickem', 'Email sent.', NULL, WATCHDOG_NOTICE);
  }
  else {
    drupal_set_message(t('Error sending email.'));
    watchdog('pickem', 'Error sending email.', NULL, WATCHDOG_ERROR);
  }

}


function league_forums() {
  $options = array();
  $options[0] = 'No Forum Selected';
  if (module_exists("forum")) {
    $forums = forum_forum_load(0);
    if ($forums->forums) {
      foreach ($forums->forums as $tid => $forum) {
         $options[$tid] = $forum->name;
      }
    }
  }
  return $options;
}


function gameset_options() {
  $options = array();
  $gamesets = get_gamesets();
  if ($gamesets) {
    foreach ($gamesets as $gsid => $gameset) {
       $options[$gsid] = $gameset->gs_name;
    }
  }
  return $options;
}


function get_scoring_engine_name_options() {
  $options = array();
  $all_engines = get_scoring_engine_all();
  foreach ($all_engines as $key => $engine) {
    $options[$key] = $engine->name;
  }
  return $options;
}


function get_scoring_engine_descriptions() {
  $descriptions = '';
  $all_engines = get_scoring_engine_all();
   foreach ($all_engines as $key => $engine) {
    $descriptions .= $engine->name . ': ' . $engine->description . '<br/>';
  }
  return $descriptions;
}
