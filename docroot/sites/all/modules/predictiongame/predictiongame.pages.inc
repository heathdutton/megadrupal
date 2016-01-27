<?php

/**
 * Page callback for displaying a competition overview.
 */
function predictiongame_competition_overview($comp) {
  global $user;

  if (user_access('use predictiongame')) {
    $points = db_query('SELECT SUM(points) FROM {predictiongame_bet} WHERE uid = :uid AND competition = :comp',
        array(':uid' => $user->uid, ':comp' => $comp->id))->fetchField();
    $ret['points'] = '<p>' . format_plural((int) $points, 'You currently have 1 point.', 'You currently have @count points.') . '</p>';
  }

  $select = db_select('predictiongame_match', 'm');
  $select->addExpression('COUNT(*)', 'num');
  $select
    ->fields('m', array('time'))
    ->condition('competition', $comp->id)
    ->groupBy('time')
    ->orderBy('time', 'asc');
  $matches = $select->execute();

  $ret['matches'] = array(
    '#theme' => 'links',
    '#heading' => t('Matches', array(), array('context' => 'sports')),
  );
  $i = 0;
  foreach ($matches as $match) {
    $ret['matches']['#links'][] = array(
      'title' => t('Round #!num: !date (!match_count matches)',
          array('!num' => ++$i,'!date' => format_date($match->time, 'short'), '!match_count' => $match->num), array('context' => 'sports')),
      'href' => 'predictiongame/' . $comp->shortname . '/list/' . $match->time,
    );
  }
  if (empty($ret['matches']['#links'])) {
    $ret['matches'] = t('There are currently no matches defined for this competition.');
  }

  return $ret;
}

/**
 * Page callback for displaying a table showing all players' scores.
 */
function predictiongame_competition_scores($comp) {
  global $user;

  $header = array(
    t('Position'),
    array('data' => t('Name'), 'field' => 'name'),
    array('data' => t('Points'), 'field' => 'points', 'sort' => 'desc', 'descending' => TRUE),
    array('data' => t('Bets'), 'field' => 'bets', 'descending' => TRUE),
  );

  $select = db_select('predictiongame_bet', 'b');
  $select->join('users', 'u', 'b.uid = u.uid');
  $select->addExpression('SUM(points)', 'points');
  $select->addExpression('COUNT(*)', 'bets');
  $select
    ->fields('u', array('uid', 'name'))
    ->condition('competition', $comp->id)
    ->groupBy('uid')
    ->groupBy('name')
    ->extend('TableSort')
    ->orderByHeader($header);
  $scores = $select->execute();

  $rows = array();
  $pos = 0;
  $prev = -1;
  $delta = 1;
  $default_sort = empty($_GET['order']) || empty($_GET['sort']) || ($_GET['order'] == t('Points') && $_GET['sort'] == 'desc');
  foreach ($scores as $score) {
    // If the sort is the default, we can set the positions right away.
    if ($default_sort) {
      if ($prev != $score->points) {
        $pos += $delta;
        $delta = 1;
        $prev = $score->points;
      }
      else {
        ++$delta;
      }
    }
    $row = array(
      $pos,
      l($score->name, 'user/' . $score->uid),
      $score->points,
      $score->bets,
    );
    if ($user->uid == $score->uid) {
      $row = array('data' => $row);
      $row['class'] = array('predictiongame-active');
    }
    $rows[] = $row;
  }

  if (!empty($_GET['sort'])) {
    $rows_sorted = $rows;
    uasort($rows_sorted, 'predictiongame_rows_compare');
    foreach ($rows_sorted as $i => $row) {
      if (isset($row['data'])) {
        if ($prev != $row['data'][2]) {
          $pos += $delta;
          $delta = 1;
          $prev = $row['data'][2];
        }
        else {
          ++$delta;
        }
        $rows[$i]['data'][0] = $pos;
      }
      else {
        if ($prev != $row[2]) {
          $pos += $delta;
          $delta = 1;
          $prev = $row[2];
        }
        else {
          ++$delta;
        }
        $rows[$i][0] = $pos;
      }
    }
  }

  return array(
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => $rows,
    '#sticky' => TRUE,
    '#empty' => t("There haven't been any matches yet in this competition."),
    '#attributes' => array('class' => array('predictiongame')),
  );
}

/**
 * Compare score table rows by points.
 *
 * @see element_sort
 */
function predictiongame_rows_compare($a, $b) {
  if (isset($a['data'])) {
    $a = $a['data'];
  }
  if (isset($b['data'])) {
    $b = $b['data'];
  }
  $a_points = (is_array($a) && isset($a[2])) ? $a[2] : 0;
  $b_points = (is_array($b) && isset($b[2])) ? $b[2] : 0;
  if ($a_points == $b_points) {
    return 0;
  }
  return ($a_points < $b_points) ? 1 : -1;
}

/**
 * Page callback for displaying a list of matches.
 */
function predictiongame_list_matches($comp, $time = NULL) {
  if (user_access('use predictiongame')) {
    return drupal_get_form('predictiongame_form_list_matches', $comp, $GLOBALS['user'], $time);
  }

  $header = array(
    array('data' => t('Home team'), 'field' => 'home_team'),
    t('Result'),
    array('data' => t('Away team'), 'field' => 'away_team'),
    array('data' => t('Time'), 'field' => 'time', 'sort' => 'asc'),
  );

  $select = db_select('predictiongame_match', 'm');
  $home = $select->join('predictiongame_team', 't1', 'm.home = %alias.id');
  $away = $select->join('predictiongame_team', 't2', 'm.away = %alias.id');
  $select->addField($home, 'name', 'home_team');
  $select->addField($away, 'name', 'away_team');
  $select->fields('m', array('score_home', 'score_away', 'time',));
  if ($time) {
    $select->condition('time', $time, '>=');
  }
  $matches = $select
    ->extend('PagerDefault')
    ->limit($comp->options['matches_per_page'])
    ->extend('TableSort')
    ->orderByHeader($header)
    ->execute();

  $rows = array();
  foreach ($matches as $match) {
    $rows[] = array(
      $match->home_team,
      isset($match->score_home)
          ? t('@score_home : @score_away', array('@score_home' => $match->score_home, '@score_away' => $match->score_away))
          : '-',
      $match->away_team,
      format_date($match->time),
    );
  }

  return array(
    'matches' => array(
      '#theme' => 'table',
      '#header' => $header,
      '#rows' => $rows,
      '#sticky' => TRUE,
      '#empty' => t('There are no matches defined yet for this competition.'),
      '#attributes' => array('class' => array('predictiongame')),
    ),
    'pager' => array(
      '#theme' => 'pager',
      '#quantity' => 9,
      '#weight' => 5,
    ),
  );
}

/**
 * Form callback for displaying a list of matches for making bets.
 */
function predictiongame_form_list_matches(array $form, array &$form_state, $comp, $account, $time = NULL) {
  $is_admin = user_access('administer predictiongame');

  $header = array(
    array('data' => t('Home team'), 'colspan' => 2, 'field' => 'home_team'),
    t('Result'),
    array('data' => t('Away team'), 'colspan' => 2, 'field' => 'away_team'),
    array('data' => t('Time'), 'field' => 'time', 'sort' => 'asc'),
    array('data' => t('Points'), 'field' => 'points', 'descending' => TRUE),
  );

  $select = db_select('predictiongame_match', 'm');
  $home = $select->join('predictiongame_team', 't1', 'm.home = %alias.id');
  $away = $select->join('predictiongame_team', 't2', 'm.away = %alias.id');
  $bets = $select->leftJoin('predictiongame_bet', 'b', 'm.id = %alias.mid AND %alias.uid = :uid', array(':uid' => $account->uid));
  $select->addField($home, 'name', 'home_team');
  $select->addField($away, 'name', 'away_team');
  $select->fields('m', array('id', 'competition', 'score_home', 'score_away', 'time',));
  $select->fields($bets, array('bet_home', 'bet_away', 'points',));
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

  $form['#tree'] = TRUE;
  $form['notice']['#markup'] = '<p>' . t('Use this table to submit your bets. ' .
      'Please remember to click the "@button" button before going to another page.',
      array('@button' => t('Save changes'))) . ' ' .
      ($comp->options['deadline']
        ? t('Bets can be submitted until @timespan before the match starts.', array('@timespan' => format_interval($comp->options['deadline'])))
        : t('Bets can be submitted until the match starts.')) . '</p>';

  $alterable = FALSE;
  foreach ($matches as $match) {
    $disabled = !$is_admin && REQUEST_TIME > $match->time - $comp->options['deadline'];
    if (!$disabled) {
      $alterable = TRUE;
    }
    $f = &$form['matches'][$match->id];
    $f['home_team'] = array(
      '#markup' => check_plain($match->home_team),
    );
    $f['bet_home'] = array(
      '#type' => 'textfield',
      '#size' => 4,
      '#maxlength' => 2,
      '#default_value' => $match->bet_home,
      '#disabled' => $disabled,
    );
    $f['score']['#markup'] = isset($match->score_home)
        ? t('@score_home : @score_away', array('@score_home' => $match->score_home, '@score_away' => $match->score_away))
        : '-';
    $f['bet_away'] = array(
      '#type' => 'textfield',
      '#size' => 4,
      '#maxlength' => 2,
      '#default_value' => $match->bet_away,
      '#disabled' => $disabled,
    );
    $f['away_team'] = array(
      '#markup' => check_plain($match->away_team),
    );
    $f['time'] = array(
      '#markup' => format_date($match->time),
    );
    $f['points'] = array(
      '#markup' => isset($match->score_home) ? ' ' . check_plain((int) $match->points) : '-',
    );

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

  $form['pager'] = array(
    '#theme' => 'pager',
    '#quantity' => 9,
  );

  if ($alterable) {
    $form['uid'] = array(
      '#type' => 'value',
      '#value' => $account->uid,
    );

    $form['actions']['#type'] = 'actions';
    $form['actions']['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Save changes'),
    );
  }

  return $form;
}

/**
 * Validation callback for predictiongame_form_list_matches().
 */
function predictiongame_form_list_matches_validate($form, &$form_state) {
  foreach (element_children($form_state['values']['matches']) as $id) {
    if ($id == 'header') {
      continue;
    }
    $match = $form_state['values']['matches'][$id];
    if (is_numeric($match['bet_home']) xor is_numeric($match['bet_away'])) {
      form_set_error('matches][' . $id, t('You have to either set both scores or none.'));
    }
  }
}

/**
 * Submit callback for predictiongame_form_list_matches().
 */
function predictiongame_form_list_matches_submit($form, &$form_state) {
  $uid = $form_state['values']['uid'];
  $changes = FALSE;
  foreach (element_children($form_state['values']['matches']) as $id) {
    if ($id == 'header') {
      continue;
    }
    $match = $form_state['values']['matches'][$id];
    if ($match['bet_home'] != (string) $match['match']->bet_home || $match['bet_away'] != (string) $match['match']->bet_away) {
      $changes = TRUE;
      if (!is_numeric($match['bet_home'])) {
        db_delete('predictiongame_bet')
          ->condition('uid', $uid)
          ->condition('mid', $match['match']->id)
          ->execute();
      }
      elseif ($match['match']->bet_home === NULL) {
        db_insert('predictiongame_bet')
          ->fields(array(
            'uid' => $uid,
            'mid' => $match['match']->id,
            'competition' => $match['match']->competition,
            'bet_home' => $match['bet_home'],
            'bet_away' => $match['bet_away'],
          ))
          ->execute();
      }
      else {
        db_update('predictiongame_bet')
          ->condition('uid', $uid)
          ->condition('mid', $match['match']->id)
          ->fields(array(
            'bet_home' => $match['bet_home'],
            'bet_away' => $match['bet_away'],
          ))
          ->execute();
      }
    }
  }

  if ($changes) {
    drupal_set_message(t('Your bets were successfully saved.'));
  }
  else {
    drupal_set_message(t('No values changed.'), 'warning');
  }

  $form_state['redirect'] = array($_GET['q'], array('query' => drupal_get_query_parameters()));
}

/**
 * Returns HTML for a match list form.
 *
 * @param array $variables
 *   An associative array containing:
 *   - element: A render element representing the form.
 */
function theme_predictiongame_match_table(array $variables) {
  $form = $variables['element'];

  $rows = array();
  foreach (element_children($form) as $id) {
    $row = array();
    foreach (element_children($form[$id]) as $field) {
      if ($cell = render($form[$id][$field])) {
        $row[] = $cell;
      }
    }
    $rows[] = $row;
  }

  $theme['rows'] = $rows;
  $theme['attributes']['class'][] = 'predictiongame';
  if (isset($form['header']['#value'])) {
    $theme['header'] = $form['header']['#value'];
  }
  if (isset($form['empty']['#value'])) {
    $theme['empty'] = $form['empty']['#value'];
  }
  return theme('table', $theme);
}
