<?php

/**
 * Page callback for displaying an overview.
 */
function predictiongame_admin_overview() {
  $is_admin = user_access('administer predictiongame');
  $header = array(
    t('Competition'),
    array('data' => t('Operations'), 'colspan' => $is_admin ? 3 : 2),
  );

  $comps = db_select('predictiongame_competition', 'c')
    ->fields('c')
    ->orderBy('id', 'desc')
    ->execute();
  $rows = array();
  foreach ($comps as $comp) {
    $row = array(
      l($comp->name, 'admin/content/predictiongame/' . $comp->shortname),
      l(t('View'), 'predictiongame/' . $comp->shortname),
      l(t('Edit'), 'admin/content/predictiongame/' . $comp->shortname . '/edit'),
    );
    if ($is_admin) {
      $row[] = l(t('Delete'), 'admin/content/predictiongame/' . $comp->shortname . '/delete');
    }
    $rows[] = $row;
  }

  return array(
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => $rows,
    '#empty' => t('There are no competition games defined yet.'),
    '#attributes' => array('class' => array('predictiongame')),
  );
}

/**
 * Page callback for redirecting to another view of the competition.
 */
function predictiongame_admin_competition_redirect($comp, $prefix = 'predictiongame/') {
  drupal_goto($prefix . $comp->shortname);
}

/**
 * Displays a form for adding or editing a competition.
 */
function predictiongame_admin_competition_form(array $form, array &$form_state, $comp = NULL) {
  $form['#tree'] = TRUE;
  $form['name'] = array(
    '#type' => 'textfield',
    '#title' => t('Name'),
    '#description' => t('Enter a name for the competition—e.g., "NFL 2011".'),
    '#maxlength' => 50,
    '#required' => TRUE,
    '#default_value' => $comp ? $comp->name : '',
  );
  $form['shortname'] = array(
    '#type' => 'textfield',
    '#title' => t('Identifier'),
    '#description' => t('Enter a short identifier for the competition, used for URLs—e.g., "nfl2011".'),
    '#maxlength' => 50,
    '#required' => !$comp,
    '#disabled' => $comp,
    '#default_value' => $comp ? $comp->shortname : '',
  );
  $teams = array();
  $results = db_select('predictiongame_team', 't')
    ->fields('t')
    ->orderBy('name', 'asc')
    ->execute();
  foreach ($results as $team) {
    $teams[$team->id] = $team->name;
  }
  if (count($teams) < 2) {
    drupal_set_message(t('You have to create some teams before you can create a competition.'), 'warning');
    drupal_goto('admin/content/predictiongame/teams');
  }
  $form['options']['teams'] = array(
    '#type' => 'select',
    '#title' => t('Teams'),
    '#description' => t('Select the teams participating in this competition.'),
    '#options' => $teams,
    '#multiple' => TRUE,
    '#size' => 8,
    '#required' => TRUE,
    '#default_value' => $comp ? $comp->options['teams'] : NULL,
  );
  $form['options']['deadline'] = array(
    '#type' => 'select',
    '#title' => t('Deadline'),
    '#description' => t('Select the time before the start of a match until which bets are accepted.'),
    '#options' => array(
      0 => t('Until match start'),
      30 * 60 => t('@count minutes', array('@count' => 30)),
      60 * 60 => t('1 hour'),
      2 * 60 * 60 => t('@count hours', array('@count' => 2)),
      3 * 60 * 60 => t('@count hours', array('@count' => 3)),
      4 * 60 * 60 => t('@count hours', array('@count' => 4)),
      6 * 60 * 60 => t('@count hours', array('@count' => 6)),
      12 * 60 * 60 => t('@count hours', array('@count' => 12)),
      24 * 60 * 60 => t('1 day'),
    ),
    '#default_value' => $comp ? $comp->options['deadline'] : 60 * 60,
  );
  $form['options']['matches_per_page'] = array(
    '#type' => 'select',
    '#title' => t('Matches per page', array(), array('context' => 'sports')),
    '#description' => t('Select the number of matches that will be displayed on one page when browsing them.', array(), array('context' => 'sports')),
    '#options' => array(
      0 => t('Automatic (one round)'),
      5 => 5,
      10 => 10,
      15 => 15,
      20 => 20,
      30 => 30,
      40 => 40,
      50 => 50,
    ),
    '#default_value' => $comp ? $comp->options['matches_per_page'] : 0,
  );
  if ($comp) {
    unset($form['options']['matches_per_page']['#options'][0]);
    $round = (int) (count($comp->options['teams']) / 2);
    $form['options']['matches_per_page']['#options'][$round] = $round;
    $form['options']['matches_per_page']['#options'][$comp->options['matches_per_page']] = $comp->options['matches_per_page'];
    asort($form['options']['matches_per_page']['#options'], SORT_NUMERIC);
  }

  $form['options']['points'] = array(
    '#type' => 'fieldset',
    '#title' => t('Points'),
    '#collapsible' => TRUE,
    '#collapsed' => $comp,
  );
  $form['options']['points']['winner'] = array(
    '#type' => 'select',
    '#title' => t('Winner'),
    '#description' => t('Points for correctly guessing the match winner.'),
    '#options' => range(0, 10),
    '#default_value' => $comp ? $comp->options['points']['winner'] : 1,
  );
  $form['options']['points']['diff'] = array(
    '#type' => 'select',
    '#title' => t('Difference'),
    '#description' => t('Points for correctly guessing the match winner and the score difference.'),
    '#options' => range(0, 10),
    '#default_value' => $comp ? $comp->options['points']['diff'] : 2,
  );
  $form['options']['points']['result'] = array(
    '#type' => 'select',
    '#title' => t('Result'),
    '#description' => t('Points for correctly guessing the exact result.'),
    '#options' => range(0, 10),
    '#default_value' => $comp ? $comp->options['points']['result'] : 3,
  );

  $form['id'] = array(
    '#type' => 'value',
    '#value' => $comp ? $comp->id : NULL,
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );

  return $form;
}

/**
 * Validation handler for predictiongame_admin_competition_form.
 */
function predictiongame_admin_competition_form_validate(array $form, array &$form_state) {
  if (isset($form_state['values']['id'])) {
    return;
  }
  $shortname = $form_state['values']['shortname'];
  if (db_query('SELECT COUNT(1) FROM {predictiongame_competition} WHERE shortname = :name', array(':name' => $shortname))->fetchField()) {
    form_set_error('shortname', t('This identifier is already in use.'));
  }
  elseif (array_search($shortname, array('add', 'overview', 'teams', 'match')) !== FALSE) {
    form_set_error('shortname', t('This identifier is forbidden. Please choose another one.'));
  }
  elseif (is_numeric($shortname)) {
    form_set_error('shortname', t("Please don't use pure numeric identifiers."));
  }
}

/**
 * Submit handler for predictiongame_admin_competition_form.
 */
function predictiongame_admin_competition_form_submit(array $form, array &$form_state) {
  form_state_values_clean($form_state);
  $comp = (object) $form_state['values'];
  if (!$comp->options['matches_per_page']) {
    $comp->options['matches_per_page'] = (int) (count($comp->options['teams']) / 2);
  }
  predictiongame_competition_save($comp);
  if (isset($form_state['values']['id'])) {
    drupal_set_message(t('Competition %name was successfully edited.', array('%name' => $comp->name)));
  }
  else {
    drupal_set_message(t('Competition %name was successfully created.', array('%name' => $comp->name)));
  }
  $form_state['redirect'] = 'admin/content/predictiongame/' . $comp->shortname;
}

/**
 * Displays the list of existing teams.
 */
function predictiongame_admin_teams(array $form, array &$form_state) {
  $form['description']['#markup'] = t('Use this form to alter the defined teams. ' .
      'You can either create new teams by filling out empty fields, rename existing teams or delete them by removing their name.');

  $teams = db_select('predictiongame_team', 't')
    ->fields('t')
    ->orderBy('name', 'asc')
    ->execute();
  $form['teams'] = array(
    '#tree' => TRUE,
    '#theme' => 'predictiongame_match_table',
    'header' => array(
      '#type' => 'value',
      '#value' => array(
        t('ID'),
        t('Name'),
      ),
    ),
  );
  foreach ($teams as $team) {
    $form_state['teams']['team' . $team->id] = $team;
    $form['teams']['team' . $team->id]['id'] = array(
      '#markup' => $team->id,
    );
    $form['teams']['team' . $team->id]['name'] = array(
      '#type' => 'textfield',
      '#maxlength' => 50,
      '#default_value' => $team->name,
    );
  }
  for ($i = 1; $i <= 5; ++$i) {
    $form_state['teams']['new' . $i] = (object) array(
      'id' => NULL,
      'name' => '',
    );
    $form['teams']['new' . $i]['id'] = array(
      '#markup' => t('New'),
    );
    $form['teams']['new' . $i]['name'] = array(
      '#type' => 'textfield',
      '#maxlength' => 50,
      '#default_value' => '',
    );
  }

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );

  return $form;
}

/**
 * Validation handler for predictiongame_admin_teams.
 */
function predictiongame_admin_teams_validate(array $form, array &$form_state) {
  $teams = $form_state['values']['teams'];
  foreach (element_children($teams) as $key) {
    if ($key == 'header') {
      continue;
    }
    $team = $form_state['teams'][$key];
    $new = trim($teams[$key]['name']);
    if ($team->name == $new) {
      continue;
    }
    if ($new !== '') {
      // Renaming or insertion
      if (db_query('SELECT COUNT(1) FROM {predictiongame_team} WHERE name = :name', array(':name' => $new))->fetchField()) {
        form_set_error('teams][' . $key . '][name', t('The selected name is already in use.'));
      }
    }
    else {
      // Deletion
      if (db_query('SELECT COUNT(1) FROM {predictiongame_match} WHERE home = :id OR away = :id',
          array(':id' => $team->id))->fetchField()) {
        form_set_error('teams][' . $key . '][name', t("Can't delete this team: There are already matches in which it participates."));
      }
    }
  }
}

/**
 * Submit handler for predictiongame_admin_teams.
 */
function predictiongame_admin_teams_submit(array $form, array &$form_state) {
  $renamed = $inserted = $deleted = 0;

  $teams = $form_state['values']['teams'];
  foreach (element_children($teams) as $key) {
    if ($key == 'header') {
      continue;
    }
    $team = $form_state['teams'][$key];
    $new = trim($teams[$key]['name']);
    if ($team->name === $new) {
      continue;
    }
    if (!isset($team->id)) {
      // Insertion
      db_insert('predictiongame_team')
        ->fields(array('name' => $new))
        ->execute();
      ++$inserted;
    }
    elseif ($new !== '') {
      // Renaming
      $renamed += db_update('predictiongame_team')
        ->fields(array('name' => $new))
        ->condition('id', $team->id)
        ->execute();
    }
    else {
      // Deletion
      db_delete('predictiongame_team')
        ->condition('id', $team->id)
        ->execute();
      ++$deleted;
    }
  }

  if ($inserted) {
    drupal_set_message(format_plural($inserted, 'Successfully inserted 1 team.', 'Successfully inserted @count teams.'));
  }
  if ($deleted) {
    drupal_set_message(format_plural($deleted, 'Successfully deleted 1 team.', 'Successfully deleted @count teams.'));
  }
  if ($renamed) {
    drupal_set_message(format_plural($renamed, 'Successfully renamed 1 team.', 'Successfully renamed @count teams.'));
  }

  $form_state['redirect'] = 'admin/content/predictiongame/teams';
}

/**
 * Show a table containing all defined matches.
 */
function predictiongame_admin_competition(array $form, array &$form_state, $comp, $time = NULL) {
  $is_admin = user_access('administer predictiongame');

  $form['#tree'] = TRUE;
  $form['competition'] = array(
    '#type' => 'value',
    '#value' => $comp,
  );
  $form['notice']['#markup'] = '<p>' . t('Use this table to enter or alter match results. ' .
      'Please remember to click the "@button" button before going to another page.',
      array('@button' => t('Save changes'))) . '</p>';

  // Quick switcher

  $select = db_select('predictiongame_match', 'm');
  $select->addExpression('COUNT(*)', 'num');
  $select
    ->fields('m', array('time'))
    ->condition('competition', $comp->id)
    ->groupBy('time')
    ->orderBy('time', 'asc');
  $matches = $select->execute();

  $i = 0;
  $default = NULL;
  foreach ($matches as $match) {
    if ($match->time < REQUEST_TIME) {
      $default = $match->time;
    }
    $rounds[$match->time] = t('Round #!num: !date (!match_count matches)',
        array('!num' => ++$i,'!date' => format_date($match->time), '!match_count' => $match->num), array('context' => 'sports'));
  }
  if (!empty($rounds)) {
    $form['switcher'] = array(
      '#type' => 'fieldset',
      '#title' => t('Jump to round'),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
      '#attributes' => array('class' => array('container-inline')),
    );
    $form['switcher']['time'] = array(
      '#type' => 'select',
      '#options' => $rounds,
      '#default_value' => $default,
    );
    $form['switcher']['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Jump to round'),
    );
  }

  // Matches table

  $header = array(
    array('data' => t('Home team'), 'colspan' => 2, 'field' => 'home_team'),
    array('data' => t('Away team'), 'colspan' => 2, 'field' => 'away_team'),
    array('data' => t('Time'), 'field' => 'time', 'sort' => 'asc'),
  );
  if ($is_admin) {
    $header[] = t('Details');
  }

  $select = db_select('predictiongame_match', 'm');
  $home = $select->join('predictiongame_team', 't1', 'm.home = %alias.id');
  $away = $select->join('predictiongame_team', 't2', 'm.away = %alias.id');
  $select->addField($home, 'name', 'home_team');
  $select->addField($away, 'name', 'away_team');
  $select->fields('m', array('id', 'score_home', 'score_away', 'time',));
  $select->condition('m.competition', $comp->id);

  if ($time && !isset($_GET['page']) && (!isset($_GET['order']) || $_GET['order'] == t('Time'))) {
    $count = clone $select;
    $count = $count
      ->condition('time', $time, isset($_GET['sort']) && $_GET['sort'] == 'desc' ? '>' : '<')
      ->countQuery()
      ->execute()
      ->fetchField();
    $_GET['page'] = (int) ($count / $comp->options['matches_per_page']);
  }

  $matches = $select
    ->extend('PagerDefault')
    ->limit($comp->options['matches_per_page'])
    ->extend('TableSort')
    ->orderByHeader($header)
    ->execute();

  $alterable = $is_admin;
  foreach ($matches as $match) {
    $disabled = REQUEST_TIME < $match->time && !$is_admin;
    if (!$disabled) {
      $alterable = TRUE;
    }
    $f = &$form['matches'][$match->id];
    $f['home_team'] = array(
      '#markup' => check_plain($match->home_team),
    );
    $f['score_home'] = array(
      '#type' => 'textfield',
      '#size' => 4,
      '#maxlength' => 2,
      '#default_value' => $match->score_home,
      '#disabled' => $disabled,
    );
    $f['score_away'] = array(
      '#type' => 'textfield',
      '#size' => 4,
      '#maxlength' => 2,
      '#default_value' => $match->score_away,
      '#disabled' => $disabled,
    );
    $f['away_team'] = array(
      '#markup' => check_plain($match->away_team),
    );
    $f['time'] = array(
      '#markup' => format_date($match->time),
    );

    if ($is_admin) {
      $f['details'] = array(
        '#markup' => l(t('View'), 'admin/content/predictiongame/match/' . $match->id),
      );
    }

    $f['match'] = array(
      '#type' => 'value',
      '#value' => $match,
    );
  }

  if (empty($form['matches'])) {
    return array('notice' => array('#markup' => t('There are no matches defined yet for this competition.')));
  }

  $form['matches']['#theme'] = 'predictiongame_match_table';
  $form['matches']['header'] = array(
    '#type' => 'value',
    '#value' => $header,
  );

  $form['#suffix'] = theme('pager', array(
    'quantity' => 9,
  ));

  if ($alterable) {
    $form['actions']['#type'] = 'actions';
    $form['actions']['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Save changes'),
    );
  }

  return $form;
}

/**
 * Validation handler for predictiongame_admin_competition.
 */
function predictiongame_admin_competition_validate(array $form, array &$form_state) {
  if ($form_state['values']['op'] == t('Jump to round')) {
    return;
  }
  foreach (element_children($form_state['values']['matches']) as $id) {
    if ($id != 'header' && (is_numeric($form_state['values']['matches'][$id]['score_home']) xor is_numeric($form_state['values']['matches'][$id]['score_away']))) {
      form_set_error('matches][' . $id, t('You have to either set both scores or none.'));
    }
  }
}

/**
 * Submit handler for predictiongame_admin_competition.
 */
function predictiongame_admin_competition_submit(array $form, array &$form_state) {
  $comp = $form_state['values']['competition'];
  if ($form_state['values']['op'] == t('Jump to round')) {
    $form_state['redirect'] = 'admin/content/predictiongame/' . $comp->id . '/' . $form_state['values']['switcher']['time'];
    return;
  }
  $changes = FALSE;
  foreach (element_children($form_state['values']['matches']) as $id) {
    if ($id == 'header') {
      continue;
    }
    $match = $form_state['values']['matches'][$id];
    $home = is_numeric($match['score_home']) ? (int) $match['score_home'] : NULL;
    $away = is_numeric($match['score_away']) ? (int) $match['score_away'] : NULL;
    $match = $match['match'];
    if ($home !== $match->score_home || $away !== $match->score_away) {
      $changes = TRUE;
      db_update('predictiongame_match')
        ->condition('id', $match->id)
        ->fields(array(
          'score_home' => $home,
          'score_away' => $away,
        ))
        ->execute();
      $match->score_home = $home;
      $match->score_away = $away;
      predictiongame_admin_match_refresh($match, $comp);
    }
  }
  if ($changes) {
    drupal_set_message(t('The new scores were successfully saved.'));
  }
  else {
    drupal_set_message(t('No values changed.'), 'warning');
  }
}

/**
 * Refreshes the points awarded for bets on a certain match, or on all matches.
 */
function predictiongame_admin_match_refresh($match = NULL, $comp = NULL) {
  if (!$match) {
    $matches = db_select('predictiongame_match', 'm')
      ->fields('m');
    if ($comp) {
      $matches->condition('competition', $comp->id);
    }
    foreach ($matches->execute() as $match) {
      predictiongame_admin_match_refresh($match, $comp);
    }
    return;
  }
  if (!$comp) {
    $comp = predictiongame_competition_load($match->competition);
  }
  // Reset all associated bets' points.
  db_update('predictiongame_bet')
    ->condition('mid', $match->id)
    ->fields(array(
      'points' => 0,
    ))
    ->execute();
  // Then update them again, if the result wasn't simply deleted.
  if (isset($match->score_home)) {
    if ($match->score_home != $match->score_away) {
      db_update('predictiongame_bet')
        ->condition('mid', $match->id)
        ->where('bet_home ' . ($match->score_home > $match->score_away ? '>' : '<') . ' bet_away')
        ->fields(array(
          'points' => $comp->options['points']['winner'],
        ))
        ->execute();
    }
    db_update('predictiongame_bet')
      ->condition('mid', $match->id)
      ->where('(bet_home - bet_away) = :diff', array(':diff' => $match->score_home - $match->score_away))
      ->fields(array(
        'points' => $comp->options['points']['diff'],
      ))
      ->execute();
    db_update('predictiongame_bet')
      ->condition('mid', $match->id)
      ->condition('bet_home', $match->score_home)
      ->condition('bet_away', $match->score_away)
      ->fields(array(
        'points' => $comp->options['points']['result'],
      ))
      ->execute();
  }
}

/**
 * Shows a form for deleting a competition.
 */
function predictiongame_admin_competition_delete(array $form, array &$form_state, $comp) {
  $running = db_query('SELECT COUNT(1) FROM {predictiongame_match} WHERE competition = :comp AND score_home IS NULL',
      array(':comp' => $comp->id))->fetchField();
  $form['comp'] = array(
    '#type' => 'value',
    '#value' => $comp->id,
  );
  $question = t('Do you really want to delete the competition %name?', array('%name' => $comp->name));
  $desc = t('This will irrecoverably delete all data associated with this competition – i.e., settings, matches and bets.');
  if ($running) {
    $desc = "<p>$desc</p><p><strong>" . t('There are still unfinished matches for this competition!') . '</strong></p>';
  }
  return confirm_form($form, $question, 'admin/content/predictiongame/' . $comp->shortname, $desc, t('Delete competition'), t('Cancel'));
}

/**
 * Submit handler for predictiongame_admin_competition_delete.
 */
function predictiongame_admin_competition_delete_submit(array $form, array &$form_state) {
  $comp = $form_state['values']['comp'];
  $form_state['redirect'] = 'admin/content/predictiongame';
  $success = db_delete('predictiongame_competition')
    ->condition('id', $comp)
    ->execute();
  if (!$success) {
    drupal_set_message(t("Couldn't delete the competition."), 'warning');
    return;
  }
  db_delete('predictiongame_match')
    ->condition('competition', $comp)
    ->execute();
  db_delete('predictiongame_bet')
    ->condition('competition', $comp)
    ->execute();
  drupal_set_message(t('The competition was successfully deleted.'));
}

/**
 * Displays a form for adding matches to a competition.
 */
function predictiongame_admin_match_add(array $form, array &$form_state, $comp) {
  $form['#tree'] = TRUE;
  $form['desc']['#markup'] = '<p>' . t('Use this form to add new matches to this competition. ' .
      'If subsequent date fields are left empty, previous values will be copied down.') . '</p>';

  $teams = array('' => '- ' . t('None') . ' -');
  $result = db_query('SELECT * FROM {predictiongame_team} WHERE id IN (:ids)', array(':ids' => $comp->options['teams']));
  foreach ($result as $row) {
    $teams[$row->id] = $row->name;
  }

  $last = db_query_range('SELECT time FROM {predictiongame_match} WHERE competition = :comp ORDER BY time DESC', 0, 1,
      array(':comp' => $comp->id))->fetchField();
  $last = $last ? $last + 7 * 24 * 60 * 60 : REQUEST_TIME;
  $last = format_date($last, 'custom', 'd.m.Y, H:i');

  $count = count($comp->options['teams']) / 2;
  $form['matches']['#theme'] = 'predictiongame_match_table';
  $form['matches']['header'] = array(
    '#type' => 'value',
    '#value' => array(
      t('Home team'),
      t('Away team'),
      t('Date'),
    ),
  );
  for ($i = 1; $i <= $count; ++$i) {
    $f = &$form['matches']['match' . $i];
    $f['home'] = array(
      '#type' => 'select',
      '#options' => $teams,
    );
    $f['away'] = $f['home'];
    $f['date']['#type'] = 'textfield';
    if ($i == 1) {
      $f['date']['#default_value'] = $last;
    }
  }

  $form['comp'] = array(
    '#type' => 'value',
    '#value' => $comp,
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );

  return $form;
}

/**
 * Validation handler of predictiongame_admin_match_add.
 */
function predictiongame_admin_match_add_validate(array $form, array &$form_state) {
  $values = &$form_state['values'];
  foreach (element_children($values['matches']) as $key) {
    if ($key == 'header') {
      continue;
    }
    $match = &$values['matches'][$key];
    if ($match['date']) {
      $parse = strtotime($match['date']);
      if ($parse === FALSE) {
        form_set_error("matches][$key][date", t('The date could not be read. To be safe, please use a format like "@format".',
            array('@format' => format_date(REQUEST_TIME, 'custom', 'd.m.Y, H:i'))));
      }
      else {
        $date = $match['date'];
      }
    }
    elseif (is_numeric($match['home'])) {
      if (isset($date)) {
        $match['date'] = $date;
      }
      else {
        form_set_error("matches][$key][date", t('There is no date to be copied down. Please set the match date.'));
      }
    }
    if (is_numeric($match['home']) xor is_numeric($match['away'])) {
      form_set_error("matches][$key", t('Please set either both teams or none.'));
    }
  }
}

/**
 * Submit handler of predictiongame_admin_match_add.
 */
function predictiongame_admin_match_add_submit(array $form, array &$form_state) {
  $values = $form_state['values'];
  $comp = $values['comp'];
  $form_state['redirect'] = 'admin/content/predictiongame/' . $comp->id . '/add';

  $rows = 0;
  $insert = db_insert('predictiongame_match')
    ->fields(array('competition', 'home', 'away', 'time'));
  foreach (element_children($values['matches']) as $key) {
    if ($key == 'header') {
      continue;
    }
    $match = &$values['matches'][$key];
    if (is_numeric($match['home'])) {
      ++$rows;
      $insert->values(array(
          'competition' => $comp->id,
          'home' => $match['home'],
          'away' => $match['away'],
          'time' => strtotime($match['date']),
        ));
    }
  }

  if (!$rows) {
    drupal_set_message(t('No matches were added.', array(), array('context' => 'sports')), 'warning');
    return;
  }

  $insert->execute();
  drupal_set_message(format_plural($rows, 'Successfully added 1 match.', 'Successfully added @count matches.'));
}

/**
 * Displays administration data about a match.
 */
function predictiongame_admin_match_view($match) {
  $home = predictiongame_team_load($match->home)->name;
  $away = predictiongame_team_load($match->away)->name;
  $title = isset($match->score_home)
      ? t('@home_team vs. @away_team (@score_home : @score_away)', array('@home_team' => $home, '@away_team' => $away, '@score_home' => $match->score_home, '@score_away' => $match->score_away))
      : t('@home_team vs. @away_team', array('@home_team' => $home, '@away_team' => $away));
  drupal_set_title($title);

  $comp = predictiongame_competition_load($match->competition);

  $bets = array();
  $result = db_query('SELECT * FROM {predictiongame_bet} WHERE mid = :match', array(':match' => $match->id));
  foreach ($result as $row) {
    $bets[] = array(
      'bet' => $row,
      'username' => theme('username', array('account' => user_load($row->uid))),
      'home' => $row->bet_home,
      'away' => $row->bet_away,
      'points' => $row->points,
    );
  }

  return array(
    '#theme' => 'predictiongame_admin_match_view',
    '#title' => $title,
    '#match' => $match,
    '#time' => format_date($match->time, 'long'),
    '#done' => $match->score_home !== NULL,
    '#competition' => $comp,
    '#competition_link' => l($comp->name, 'predictiongame/' . $comp->shortname),
    '#bets' => $bets,
  );
}

/**
 * Themes a match viewed by an admin.
 *
 * @param array $variables
 *   An associative array containing:
 *   - title: A short string describing the match.
 *   - match: The match object.
 *   - time: A formatted time string indicating the date of the match.
 *   - done: A boolean  indicating whether this match already took place.
 *   - competition: The associated competition object.
 *   - competition_link: A themed link to the competition.
 *   - bets: An array containing bet arrays with the following keys:
 *     - bet: The bet object.
 *     - username: A fully themed username.
 *     - home: The user's guess of the home score.
 *     - away: The user's guess of the away score.
 *     - points: If the match already took place, the points awarded to the
 *       user.
 */
function theme_predictiongame_admin_match_view(array $variables) {
  $out = '';

  if ($variables['time']) {
    $out .= '<h3>' . t('Match start') . '</h3>' . "\n";
    $out .= '<p>' . check_plain($variables['time']) . '</p>' . "\n";
  }
  if ($variables['competition_link']) {
    $out .= '<h3>' . t('Competition') . '</h3>' . "\n";
    $out .= '<p>' . $variables['competition_link'] . '</p>' . "\n";
  }
  if ($variables['bets']) {
    $out .= '<h3>' . t('Bets') . '</h3>' . "\n";
    $done = $variables['done'];
    $header = array(
      t('ID'),
      t('User'),
      t('Bet'),
    );
    if ($done) {
      $header[] = t('Points');
    }
    $rows = array();
    foreach ($variables['bets'] as $bet) {
      $row = array(
        $bet['bet']->id,
        $bet['username'],
        t('@score_home : @score_away', array('@score_home' => $bet['home'], '@score_away' => $bet['away'])),
      );
      if ($done) {
        $row[] = $bet['points'];
      }
      $rows[] = $row;
    }
    $out .= theme('table', array(
      'header' => $header,
      'rows' => $rows,
      'attributes' => array('class' => array('predictiongame')),
    ));
  }

  return $out;
}

/**
 * View all competitions a user has participated in.
 */
function predictiongame_admin_list_user_competitions($uid) {
  $select = db_select('predictiongame_competition', 'c')->distinct();
  $select->leftJoin('predictiongame_bet', 'b', 'c.id = b.competition AND b.uid = :uid', array(':uid' => $uid));
  $select->fields('c', array('name', 'shortname'));
  $select->addExpression('COUNT(b.id)', 'num');
  $select->groupBy('name');
  $select->groupBy('shortname');
  $comps = $select->execute();

  $links = array();
  foreach ($comps as $comp) {
    $links[] = array(
      'title' => $comp->name . ' (' . $comp->num . ')',
      'href' => 'user/' . $uid . '/predictiongame/' . $comp->shortname,
    );
  }

  return array(
    '#theme' => 'links',
    '#links' => $links,
    '#heading' => t('Competitions'),
  );
}
