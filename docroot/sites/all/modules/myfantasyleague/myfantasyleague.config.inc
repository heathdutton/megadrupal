<?php

/**
 * @file
 * Configuration options for the MyFantasyLeague module.
 */

/**
 * Configuration options for the MyFantasyLeague module.
 *
 * @param array $form
 *   The form array.
 * @param array $form_state
 *   The current form state.
 * @param object $user
 *   The user object of the user administering MyFantasyLeague.
 */
function myfantasyleague_config($form, &$form_state, $user = NULL) {
  $uid = 0;
  // Check if the user was passed in.
  if (isset($user)) {
    $uid = $user->uid;
    $verb = 'Your';
  }
  else {
    $verb = 'Global';
  }

  // Get the league info and standings columns from the database.
  $league = myfantasyleague_get_league_info($uid);
  $columns = myfantasyleague_standings_columns($uid);

  $form['#tree'] = TRUE;
  $form['uid'] = array(
    '#type' => 'hidden',
    '#default_value' => $uid,
  );
  $form['league_id'] = array(
    '#type' => 'textfield',
    '#title' => t($verb . ' MyFantasyLeague League ID'),
    '#default_value' => $league['league_id'],
    '#description' => t('To find your league ID, go to your league home page and look at the URL: http://www.myfantasyleague.com/2011/home/xxxxx. The \'xxxxx\' part is your league ID.'),
  );
  $form['year'] = array(
    '#type' => 'textfield',
    '#title' => t($verb . ' MyFantasyLeague League Year'),
    '#default_value' => $league['league_year'],
    '#description' => '',
  );
  $form['columns_title'] = array(
    '#type' => 'item',
    '#title' => t('Standings Columns Order'),
  );
  foreach ($columns as $id => $column) {
    $form['columns'][$id]['name'] = array('#markup' => drupal_placeholder($column->name));
    $form['columns'][$id]['weight'] = array(
      '#type' => 'weight',
      '#title' => t('Weight for @title', array('@title' => $column->name)),
      '#title_display' => 'invisible',
      '#delta' => 50,
      '#default_value' => $column->weight,
    );
    $form['columns'][$id]['status'] = array(
      '#type' => 'checkbox',
      '#default_value' => $column->status,
    );
  }
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Submit',
  );

  return $form;
}

/**
 * MyFantasyLeague configuration validator.
 */
function myfantasyleague_config_validate($form, &$form_state) {
  // Make sure the league ID and year are numeric or empty.
  if (!is_numeric($form['league_id']['#value']) && $form['league_id']['#value'] != '') {
    form_error($form['league_id'], t('The League ID must be numeric!'));
  }
  if (!is_numeric($form['year']['#value']) && $form['year']['#value'] != '') {
    form_error($form['year'], t('The year must be numeric!'));
  }
  // Make sure that if either league ID or year are blank, that the other is too.
  if ($form['league_id']['#value'] == '' && $form['year']['#value'] != '') {
    form_error($form['league_id'], t('The league ID must be set.'));
  }
  elseif ($form['league_id']['#value'] != '' && $form['year']['#value'] == '') {
    form_error($form['year'], t('The league year must be set.'));
  }
}

/**
 * MyFantasyLeague configuration submit handler.
 */
function myfantasyleague_config_submit($form, &$form_state) {
  // If the league ID and year aren't set, then delete entries from the database.
  if ($form['league_id']['#value'] == '' && $form['year']['#value'] == '') {
    db_delete('myfantasyleague_standings_columns')
    ->condition('uid', $form['uid']['#value'])
    ->execute();
    db_delete('myfantasyleague_users')
    ->condition('uid', $form['uid']['#value'])
    ->execute();
  }
  else {
    // If standings columns are already in database, then update them.
    if (myfantasyleague_standings_columns_set($form['uid']['#value'])) {
      foreach ($form_state['values']['columns'] as $id => $data) {
        if (is_array($data) && isset($data['weight']) && isset($data['status'])) {
          db_update('myfantasyleague_standings_columns')
          ->fields(array('weight' => $data['weight'], 'status' => $data['status']))
          ->condition('name', $id)
          ->condition('uid', $form['uid']['#value'])
          ->execute();
        }
      }
    }
    // Standings columns aren't in database, so insert them.
    else {
      foreach ($form_state['values']['columns'] as $id => $data) {
        if (is_array($data) && isset($data['weight']) && isset($data['status'])) {
          db_insert('myfantasyleague_standings_columns')
          ->fields(array(
            'uid' => $form['uid']['#value'],
            'name' => $id,
            'status' => $data['status'],
            'weight' => $data['weight'],
          ))
          ->execute();
        }
      }
    }

    // Save the league information.
    if (myfantasyleague_get_league_info($form['uid']['#value'])) {
      db_update('myfantasyleague_users')
      ->fields(array('league_id' => $form['league_id']['#value'], 'league_year' => $form['year']['#value']))
      ->condition('uid', $form['uid']['#value'])
      ->execute();
    }
    else {
      // Insert a new entry for this user's league.
      db_insert('myfantasyleague_users')
      ->fields(array(
        'uid' => $form['uid']['#value'],
        'league_id' => $form['league_id']['#value'],
        'league_year' => $form['year']['#value'],
      ))
      ->execute();
    }
  }

  // Clear all cached data since league info might have changed.
  cache_clear_all('myfantasyleague_' . $form['uid']['#value'] . '_', 'cache', TRUE);

  drupal_set_message(t('Saved changes.'));
}

/**
 * Returns HTML for the MyFantasyLeague configuration form.
 *
 * @param array $variables
 *   An associative array containing:
 *   - form: A render element representing the form.
 *
 * @ingroup themeable
 */
function theme_myfantasyleague_config($variables) {
  $form = $variables['form'];

  $rows = array();
  foreach (element_children($form['columns']) as $id) {
    $form['columns'][$id]['weight']['#attributes']['class'] = array('myfantasyleague-standings-columns-order-weight');
    $rows[] = array(
      'data' => array(
        drupal_render($form['columns'][$id]['name']),
        drupal_render($form['columns'][$id]['weight']),
        array('data' => drupal_render($form['columns'][$id]['status']), 'class' => 'checkbox'),
      ),
      'class' => array('draggable'),
    );
  }
  $header = array(t('Name'), t('Weight'), array('data' => t('Enabled'), 'class' => array('checkbox')));
  $output = drupal_render($form['league_id']);
  $output .= drupal_render($form['year']);
  $output .= drupal_render($form['columns_title']);
  $output .= theme('table', array('header' => $header, 'rows' => $rows, 'attributes' => array('id' => 'myfantasyleague-standings-columns-order')));
  $output .= drupal_render_children($form);

  drupal_add_tabledrag('myfantasyleague-standings-columns-order', 'order', 'sibling', 'myfantasyleague-standings-columns-order-weight');

  return $output;
}
