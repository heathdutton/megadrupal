<?php

/**
 * @file
 * Menu callback, arrange userqueue form
 */
function userqueue_view_queue($queue) {
  drupal_set_title($queue->title);
  $users = userqueue_get_users($queue);
  return drupal_get_form('userqueue_arrange_subqueue_form', $queue, $users);
}

/**
 * Menu callback, remove user from a userqueue
 */
function userqueue_admin_remove_user($queue, $account) {
  db_delete('userqueue_user')
      ->condition('uid', $account->uid)
      ->condition('uqid', $queue->uqid)
      ->execute();
  drupal_goto($_GET['destination']);
}

/**
 * Form definition for userqueue drag'n'drop form.
 */
function userqueue_arrange_subqueue_form($form, $form_state, $queue, $users) {
  $form = array('#tree' => TRUE);
  // Prepare the main part of the form which will be themed as a table.
  $count = count($users);
  $form['users'] = array();
  $form['users']['#theme'] = 'userqueue_arrange_subqueue_form_table';

  // Theme function needs these.
  $form['users']['#queue'] = (array) $queue;
  foreach ($users as $account) {
    $form['users'][$account->uid]['#user'] = (array) $account;
    $form['users'][$account->uid]['title'] = array('#markup' => theme('username', array('account' => $account)));
    $form['users'][$account->uid]['date'] = array(
      '#markup' => format_date($account->created, 'short'),
    );
    $form['users'][$account->uid]['edit'] = array('#markup' => l(t('edit'), 'user/' . $account->uid . '/edit', array('attributes' => array('title' => t('Edit this user')))));
    $form['users'][$account->uid]['position'] = array(
      '#type' => 'weight',
      '#delta' => $count,
      '#default_value' => $account->position,
      '#attributes' => array(
        'class' => array('user-position'),
      ),
    );
    $attr = array(
      '#attributes' => array(
        'title' => t('Remove from queue'),
        'style' => 'display: none;',
        'class' => array('userqueue-remove'),
        'id' => 'userqueue-remove-' . $account->uid,
      ),
      'query' => drupal_get_destination(),
    );
    $form['users'][$account->uid]['remove'] = array(
      '#markup' => l(t('remove'), 'userqueue/' . $queue->uqid . '/remove-user/' . $account->uid, $attr),
    );
  }

  // Textfield for adding users to the queue.
  $form['add'] = array(
    '#type' => 'container',
    '#attributes' => array('class' => array('container-inline')),
  );
  $form['add']['user'] = array(
    '#type' => 'textfield',
    '#autocomplete_path' => 'user/autocomplete',
    '#maxlength' => 1024,
  );
  $form['add']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Add user'),
    '#submit' => array('userqueue_arrange_subqueue_form_add_submit'),
  );
  // Submit, reverse, shuffle, and clear actions.
  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );
  // Disable all buttons if the queue is empty.
  if ($count == 0) {
    $form['actions']['submit']['#disabled'] = TRUE;
  }

  return $form;
}

/**
 * Submit handler, add a new user in the queue.
 */
function userqueue_arrange_subqueue_form_add_submit($form, $form_state) {
  $val = $form_state['values']['add'];
  $uid = db_select('users', 'u')
      ->fields('u', array('uid'))
      ->condition('name', $val['user'])
      ->execute()
      ->fetchField();
  $uqid = $form['users']['#queue']['uqid'];
  $query = db_select('userqueue_user', 'uu');
  $query->addExpression('MAX(position)', 'position');
  $position = $query->execute()->fetchField();
  if ($uid) {
    $item = new stdClass();
    $item->uid = $uid;
    $item->position = ($position + 1);
    $item->uqid = $uqid;
    drupal_write_record('userqueue_user', $item);
    drupal_set_message(t('User %username has been added', array('%username' => $val['user'])));
  }
}

/**
 * Submit handler.
 */
function userqueue_arrange_subqueue_form_submit($form, $form_state) {
  $users = array();
  foreach ($form_state['values']['users'] as $uid => $element) {
    $users[$form_state['values']['users'][$uid]['position']] = array(
      'uid' => $uid,
      'data' => $form_state['values']['users'][$uid],
    );
  }
  $uqid = $form['users']['#queue']['uqid'];
  userqueue_save_queue_order($users, $uqid);
}

/**
 * Validate handler.
 */
function userqueue_arrange_subqueue_form_validate($form, $form_state) {
  $positions = array();
  $error = FALSE;
  foreach ($form_state['values']['users'] as $uid => $element) {
    if (is_numeric($uid) && is_numeric($element['position'])) {
      $positions[$uid] = $element['position'];
    }
  }
  if (count(array_unique($positions)) < count($positions)) {
    $seen = array();
    foreach ($positions as $uid => $position) {
      if (isset($seen[$position])) {
        form_set_error('users][' . $uid . '][position');
        $error = TRUE;
      }
      $seen[$position] = TRUE;
    }
    if ($error) {
      drupal_set_message(t('There are one or more duplicate position value(s).'), 'error');
    }
  }
}

/**
 * Validates new queue order information and if it passes validation,
 * deletes the old queue data from the database and saves the new data.
 */
function userqueue_save_queue_order($users, $uqid) {
  $positions = array();
  $now = REQUEST_TIME;
  $queue = userqueue_load($uqid);

  // Cleanup the node array.
  $clean = array();
  $count = 1;
  ksort($users);
  foreach ($users as $pos => $account) {
    if (is_numeric($pos)) {
      $clean[$count] = $account;
      $count++;
    }
  }
  $users = $clean;
  // Clear the queue and save the new positions.
  db_delete('userqueue_user')
      ->condition('uqid', $uqid)
      ->execute();

  foreach ($users as $pos => $user) {
    $args = array();
    $query = db_insert('userqueue_user')
        ->fields(array(
          'uqid' => $uqid,
          'uid' => $user['uid'],
          'position' => $pos,
        ))
        ->execute();
  }
  return drupal_set_message(t('The queue has been updated.'));
}
