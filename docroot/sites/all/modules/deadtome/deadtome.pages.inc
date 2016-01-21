<?php

function deadtome_kill_list($account) {
  $destination = drupal_get_destination();

  $header = array(
    'user' => array('data' => t('Victim')),
    'killed_on' => array('data' => t('Killed on'), 'field' => 'killed_on', 'sort' => 'desc'),
    'operations' => array('data' => t('Operations')),
  );

  $query = db_select('deadtome')->extend('TableSort');
  $query->fields('deadtome');
  $query->condition('uid', $account->uid);
  $query->orderByHeader($header);
  $records = $query->execute()->fetchAllAssoc('dead_uid');

  $accounts = user_load_multiple(array_keys($records));

  $rows = array();
  foreach ($records as $record) {
    $record->target = $accounts[$record->dead_uid];
    $row = array();
    $row[] = format_username($record->target);
    $row[] = !empty($record->killed_on) ? format_date($record->killed_on) : t('Unknown');
    $row[] = l(t('Resurrect'), deadtome_get_url($record->target, 'resurrect', $account), array('query' => $destination));
    $rows[] = $row;
  }

  $page['records'] = array(
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => $rows,
    '#empty' => t("You have not killed anyone... yet."),// . ' ' . l(t('Kill a user.'), "user/{$account->uid}/deadtome/kill", array('query' => $destination)),
  );

  $page['kill'] = drupal_get_form('deadtome_kill_form', $account);
  $page['kill']['name']['#title'] = t('Would you like to kill someone?');
  $page['kill']['actions']['cancel']['#access'] = FALSE;

  return $page;
}

function deadtome_kill_form($form, $form_state, $account) {
  $form['account'] = array(
    '#type' => 'value',
    '#value' => $account,
  );
  $form['killed'] = array(
    '#type' => 'value',
    '#value' => NULL,
  );

  $form['name'] = array(
    '#type' => 'textfield',
    '#title' => t('Who do you want to kill?'),
    '#maxlength' => 60,
    '#autocomplete_path' => 'user/autocomplete',
    '#default_value' => '',
    '#required' => TRUE,
    '#attributes' => array(
      'placeholder' => t("Your victim's name"),
    ),
  );

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Kill'),
  );
  $form['actions']['cancel'] = array(
    '#type' => 'link',
    '#title' => t('Cancel'),
    '#href' => isset($_GET['destination']) ? $_GET['destination'] : "user/{$account->uid}/deadtome",
  );

  return $form;
}

function deadtome_kill_form_validate($form, &$form_state) {
  $killed = user_load_by_name($form_state['values']['name']);

  if (empty($killed)) {
    form_set_error('name', t('User %name not found.', array('%name' => $form_state['values']['name'])));
  }
  elseif ($killed->uid == $form_state['values']['account']->uid) {
    form_set_error('name', t('You cannot kill yourself.'));
  }
  elseif (deadtome_user_is_dead($killed->uid, $form_state['values']['account'])) {
    form_set_error('name', t('You have already killed this user.'));
  }
  else {
    // Set the dead user's UID value back into the form.
    $form_state['values']['killed'] = $killed;
  }
}

function deadtome_kill_form_submit($form, &$form_state) {
  $account = $form_state['values']['account'];
  $killed = $form_state['values']['killed'];
  deadtome_kill_users($account, array($killed->uid));
  drupal_set_message(t('You have killed %user, who is now dead to you.', array('%user' => format_username($killed))));
  $form_state['redirect'] = "user/{$account->uid}/deadtome";
}

/**
 * Menu callback to kill a user.
 */
function deadtome_kill_user($account, $target, $token) {
  if (!drupal_valid_token($token, "user/{$account->uid}/deadtome/kill/{$target->uid}")) {
    return MENU_ACCESS_DENIED;
  }

  deadtome_kill_users($account, array($target->uid));
  drupal_set_message(t('You have killed %user, who is now dead to you.', array('%user' => format_username($target))));
  drupal_goto("user/{$account->uid}/deadtome");
}

/**
 * Menu callback to resurrect a user.
 */
function deadtome_resurrect_user($account, $target, $token) {
  if (!drupal_valid_token($token, "user/{$account->uid}/deadtome/resurrect/{$target->uid}")) {
    return MENU_ACCESS_DENIED;
  }

  deadtome_resurrect_users($account, array($target->uid));
  drupal_set_message(t('You have resurrected %user, who is now undead to you.', array('%user' => format_username($target))));
  drupal_goto("user/{$account->uid}/deadtome");
}
