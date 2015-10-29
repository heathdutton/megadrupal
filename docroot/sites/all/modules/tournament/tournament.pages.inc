<?php

/**
 * Menu callback; Retrieve a JSON object containing autocomplete suggestions for existing tournaments.
 */
function tournament_autocomplete($string = '') {
  $matches = array();

  if ($string) {
    $types = tournament_supported_type();

    $result = db_select('node')
      ->fields('node', array('title'))
      ->condition('title', db_like($string) . '%', 'LIKE')
      ->condition('type', array_keys($types))
      ->condition('status', 1)
      ->orderBy('title')
      ->range(0, 10)
      ->execute();

    foreach ($result as $node) {
      $matches[$node->title] = check_plain($node->title);
    }
  }

  drupal_json_output($matches);
}

/**
 * @file
 * Page callbacks for the Tournament module.
 */

function tournament_edit_participants($node) {
  drupal_set_title(t('Manage participants'));

  $participants = db_select('tournament_participants', 'tp')
    ->fields('tp')
    ->condition('nid', $node->nid)
    ->orderBy('status', 'DESC')
    ->orderBy('registered')
    ->execute()
    ->fetchAll();

  foreach ($participants as $participant) {
    $ids[] = $participant->id;
  }

  if (!isset($ids)) {
    return t('No participants signed up for this tournament.');
  }

  // Load participant entities at once
  $entities = entity_load($node->entity_type, $ids);

  $header = array(
    array('data' => t('Name'), 'width' => '40%'),
    array('data' => t('Registered')),
    array('data' => t('Action')),
  );

  // Determine action access just once to save a little memory
  $access = $node->tournament_status != TOURNAMENT_STATUS_FINISHED;

  $rows = array();
  foreach ($participants as $participant) {
    $id = $participant->id;

    if (isset($entities[$id])) {
      $entity = $entities[$id];
      $actions = array();

      // Set up a table row
      $row = array(
        'label' => match_participant_label($node->entity_type, $entity),
        'registered' => format_date($participant->registered),
      );

      if (!$participant->status) {
        // Strikethrough deleted participants
        $row['label'] = '<del>' . entity_label($node->entity_type, $entity) . '</del>';
      }
      elseif ($access) {
        // Participant replace link
        if ($participant->status == 1) {
          $actions[] = l(t('Replace'), 'node/' . $node->nid . '/edit/participants/' . $id . '/replace');
        }

        // Participant removal link
        $actions[] = l(t('Remove'), 'node/' . $node->nid . '/edit/participants/' . $id . '/remove');
      }

      // Participant sign up author information
      if ($node->entity_type != 'user') {
        $author = user_load($participant->uid);
        $row['registered'] .= ' (' . t('By @name', array('@name' => format_username($author))) . ')';
      }

      // Convert actions into links
      $row['actions'] = !empty($actions) ? implode('<br/>', $actions) : '';

      // Add the participant to the correct table
      $participant->status == 2 ? $waiting[] = $row : $rows[] = $row;
    }
  }

  $build['participants'] = array(
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => $rows,
    '#empty' => t('No participants signed up for this tournament.'),
  );
  if (!empty($waiting)) {
    $build['waiting'] = array(
      '#theme' => 'table',
      '#header' => $header,
      '#rows' => $waiting,
      '#caption' => t('Waiting list'),
    );
  }

  return $build;
}

/**
 * Menu callback - manually add a participant to a tournament
 */
function tournament_participant_add_form($form, &$form_state, $node) {
  $form_state['node'] = $node;
  $info = entity_get_info();

  $form['participant'] = array(
    '#type' => 'textfield',
    '#title' => t('@label name', array('@label' => $info[$node->entity_type]['label'])),
    '#required' => TRUE,
  );

  if ($node->entity_type == 'user') {
    $form['participant']['#autocomplete_path'] = 'user/autocomplete';
    $form['participant']['#element_validate'][] = 'tournament_participant_add_user_validate';
  }

  $form['submit'] = array('#type' => 'submit', '#value' => t('Submit'));
  $form['cancel'] = array('#markup' => l(t('Cancel'), 'node/' . $node->nid . '/edit/participants'));

  return $form;
}

/**
 * Element validation handler for user participants
 */
function tournament_participant_add_user_validate($element, &$form_state, $form) {
  if (!$entity = user_load_by_name($form_state['values']['participant'])) {
    form_set_error('participant', t('Username not found'));
  }
  else {
    // Store for use in submit handler
    $form_state['entity'] = $entity;
  }
}

/**
 * Validation handler for adding tournament participants
 */
function tournament_participant_add_form_validate($form, &$form_state) {
  if (empty($form_state['entity'])) {
    form_set_error('participant', t('Participant not found'));
  }
  else {
    $node = $form_state['node'];

    // Check not already in tournament
    list($id) = entity_extract_ids($node->entity_type, $form_state['entity']);
    if (tournament_participants_exist($node, $id, 1)) {
      form_set_error('participant', t('@name is already a participant of this tournament.', array('@name' => $form_state['values']['participant'])));
    }
  }
}

/**
 * Submit handler for adding tournament participants
 */
function tournament_participant_add_form_submit($form, &$form_state) {
  $node = $form_state['node'];
  list($id) = entity_extract_ids($node->entity_type, $form_state['entity']);

  tournament_participant_add($node, $id);

  drupal_set_message(t('You have successfully added @name to this tournament.', array('@name' => $form_state['values']['participant'])));
}

/**
 * Menu callback - import participants from another tournament
 */
function tournament_participant_import_form($form, &$form_state, $node) {
  $form_state['node'] = $node;
  $info = entity_get_info();

  $form['tournament'] = array(
    '#type' => 'textfield',
    '#title' => t('Source tournament'),
    '#description' => t('Participants will be imported from this tournament.'),
    '#required' => TRUE,
    '#autocomplete_path' => 'tournament/autocomplete',
  );

  $form['submit'] = array('#type' => 'submit', '#value' => t('Import'));
  $form['cancel'] = array('#markup' => l(t('Cancel'), 'node/' . $node->nid . '/edit/participants'));

  return $form;
}

/**
 * Validate handler for the tournament participant import form.
 */
function tournament_participant_import_form_validate($form, &$form_state) {
  $node = $form_state['node'];

  if (!$tournament = tournament_load_by_name($form_state['values']['tournament'])) {
    form_set_error('tournament', t('Tournament not found'));
  }
  elseif ($tournament->nid == $node->nid) {
    form_set_error('tournament', t('You can not import participants from that tournament'));
  }
  elseif ($tournament->entity_type != $node->entity_type) {
    form_set_error('tournament', t('You can not import participants from that tournament'));
  }
  else {
    // Store for use in submit handler
    $form_state['tournament'] = $tournament;
  }
}

/**
 * Submit handler for the tournament participant import form.
 */
function tournament_participant_import_form_submit($form, &$form_state) {
  $tournament = $form_state['tournament'];
  $node = $form_state['node'];

  $participants = tournament_participants_data($tournament);

  foreach (array_keys($participants) as $id) {
    tournament_participant_add($node, $id);
  }

  drupal_set_message(t('@count participants imported successfully.', array('@count' => count($participants))));
  $form_state['redirect'] = 'node/' . $node->nid . '/edit/participants';
}

/**
 * Menu callback - ask for confirmation of tournament participant removal
 */
function tournament_participant_remove_confirm($form, &$form_state, $node, $id) {
  $entity = entity_load($node->entity_type, array($id));
  $name = entity_label($node->entity_type, $entity[$id]);

  $form_state['node'] = $node;
  $form_state['entity_id'] = $id;
  $form_state['entity_name'] = $name;

  return confirm_form($form,
    t('Are you sure you want to remove participant %name from tournament %tournament?', array('%name' => $name, '%tournament' => $node->title)),
    'node/' . $node->nid . '/edit/participants',
    t('This action cannot be undone.'),
    t('Remove'),
    t('Cancel')
  );
}

/**
 * Submit handler for the tournament participant removal confirm form.
 *
 * @see tournament_participant_remove_confirm()
 */
function tournament_participant_remove_confirm_submit($form, &$form_state) {
  $node = $form_state['node'];
  $id = $form_state['entity_id'];
  $name = $form_state['entity_name'];

  if ($form_state['values']['confirm']) {
    tournament_participant_delete($id, $node->entity_type, $node);
    drupal_set_message(t('Participant @name has been deleted from tournament @tournament.', array('@name' => $name, '@tournament' => $node->title)));
  }

  // Redirect to the manage participants page
  $form_state['redirect'] = 'node/' . $node->nid . '/edit/participants';
}

/**
 * Form generator; Tournament replace form
 */
function tournament_participant_replace_form($form, &$form_state, $node, $old_id) {
  $form_state['node'] = $node;

  // Load the old participant details
  $entity = entity_load($node->entity_type, array($old_id));
  $old = array(
    'id' => $old_id,
    'name' => entity_label($node->entity_type, $entity[$old_id]),
    'status' => tournament_participant_status($node, $old_id),
  );

  $form_state['old'] = $old;

  $ids = db_select('tournament_participants')
    ->fields('tournament_participants', array('id'))
    ->condition('nid', $node->nid)
    ->condition('status', 2)
    ->orderBy('status')
    ->execute()
    ->fetchCol();

  if (empty($ids)) {
    $form['error'] = array(
      '#prefix' => '<p>',
      '#suffix' => '</p>',
      '#markup' => t('The waiting list is empty. There must be a participant in the waiting list before you can replace a participant.'),
    );

    $form['cancel'] = array('#markup' => l(t('Back'), 'node/' . $node->nid . '/edit/participants'));

    return $form;
  }

  $entities = entity_load($node->entity_type, $ids);

  $options = array();
  foreach ($entities as $id => $entity) {
    $options[$id] = entity_label($node->entity_type, $entity);
  }

  $info = entity_get_info();

  $form['warning'] = array(
    '#prefix' => '<p>',
    '#suffix' => '</p>',
    '#markup' => t('WARNING: All matches and tournament data will be moved to the new participant. This includes any matches that have already been played.'),
  );

  if (count($options) > 1) {
    $form['new'] = array(
      '#type' => 'select',
      '#title' => t('Replace @entity_type', array('@entity_type' => $info[$node->entity_type]['label'])),
      '#options' => $options,
      '#description' => t('This participant will replace @name', array('@name' => $old['name']))
    );
  }
  else {
    $keys = array_keys($options);
    $id = array_shift($keys);

    $form['new'] = array(
      '#type' => 'value',
      '#value' => $id,
    );

    $form['description'] = array(
      '#prefix' => '<p>',
      '#suffix' => '</p>',
      '#markup' => t('You are replacing <strong>@name1</strong> with <strong>@name2</strong>.', array('@name1' => $old['name'], '@name2' => $options[$id])),
    );
  }

  $form['submit'] = array('#type' => 'submit', '#value' => t('Replace'));
  $form['cancel'] = array('#markup' => l(t('Cancel'), 'node/' . $node->nid . '/edit/participants'));

  return $form;
}

/**
 * Submit handler for the tournament participant replace form.
 */
function tournament_participant_replace_form_submit($form, &$form_state) {
  $node = $form_state['node'];
  $old = $form_state['old'];
  $new = $form_state['values']['new'];

  // Promote the new participant
  db_update('tournament_participants')
    ->fields(array(
      'status' => 1,
    ))
    ->condition('id', $new)
    ->condition('nid', $node->nid)
    ->execute();

  // Keep the replaced participants previous status or move them to the
  // waiting list.
  if ($old['status'] > 0) {
    db_update('tournament_participants')
      ->fields(array(
        'status' => 2,
      ))
      ->condition('id', $old['id'])
      ->condition('nid', $node->nid)
      ->execute();
  }

  // Notify other modules of the action
  module_invoke_all('tournament_participant_replace', $node, $new, $old['id']);

  drupal_set_message(t('Participant @name replaced successfully.', array('@name' => $old['name'])));

  // Redirect to the manage participants page
  $form_state['redirect'] = 'node/' . $node->nid . '/edit/participants';
}

/**
 * Menu callback; Form for managing tournament admins
 */
function tournament_admins_form($form, &$form_state, $node) {
  $form_state['node'] = $node;

  if (!isset($form_state['admins'])) {
    $query = db_select('tournament_admins', 'ta');
    $query->join('users', 'u', 'ta.uid = u.uid');
    $result = $query->fields('ta', array('uid'))
      ->fields('u', array('name'))
      ->condition('nid', $node->nid)
      ->execute();

    $form_state['admins'] = array();
    foreach ($result as $record) {
      $form_state['admins'][$record->uid] = $record->name;
    }
  }

  $form['admins'] = array(
    '#theme' => 'tournament_admins_form_admins',
    '#tree' => TRUE,
  );

  foreach ($form_state['admins'] as $uid => $name) {
    $form['admins'][$uid]['name'] = array(
      '#markup' => filter_xss($name),
    );

    // Tournament author cannot be removed
    if ($uid != $node->uid) {
      $form['admins'][$uid]['action'] = array(
        '#type' => 'submit',
        '#value' => t('Remove'),
        '#submit' => array('tournament_admins_form_remove_submit'),
        '#name' => 'remove-' . $uid,
        '#ajax' => array(
          'callback' => 'tournament_admins_form_remove_callback',
        ),
      );
    }
  }

  // Add new admin elements
  if (!isset($form_state['num_admins'])) {
    $form_state['num_admins'] = 1;
  }

  for ($i=1;$i<=$form_state['num_admins'];$i++) {
    $form['admins']['add-' . $i]['name'] = array(
      '#type' => 'textfield',
      '#title' => t('User name'),
      '#autocomplete_path' => 'user/autocomplete',
    );

    // Add another admin button just for the last row
    if ($i == $form_state['num_admins']) {
      $form['admins']['add-' . $i]['action'] = array(
        '#type' => 'submit',
        '#value' => t('Add another admin'),
        '#submit' => array('tournament_admins_form_add_submit'),
        '#ajax' => array(
          'callback' => 'tournament_admins_form_add_callback',
          'wrapper' => 'tournament-admins',
        ),
      );
    }
  }

  $form['submit'] = array('#type' => 'submit', '#value' => t('Save changes'));
  $form['cancel'] = array('#markup' => l(t('Cancel'), 'node/' . $node->nid . '/edit/admins'));

  return $form;
}

/**
 * Form validation handler for adding tournament admins
 */
function tournament_admins_form_validate($form, &$form_state) {
  $node = $form_state['node'];

  foreach (element_children($form_state['values']['admins']) as $key) {
    if (!is_numeric($key) && !empty($form_state['values']['admins'][$key]['name'])) {
      if (!$account = user_load_by_name($form_state['values']['admins'][$key]['name'])) {
        form_error($form['admins'][$key]['name'], t('Username not found'));
      }
      elseif (array_key_exists($account->uid, $form_state['values']['admins'])) {
        form_error($form['admins'][$key]['name'], t('@name is already an admin of this tournament', array('@name' => $form_state['values']['admins'][$key]['name'])));
      }
    }
  }
}

/**
 * Ajax callback for removing tournament admins
 */
function tournament_admins_form_remove_callback($form, $form_state) {
  $uid = $form_state['triggering_element']['#parents'][1];

  // Remove the table row
  $commands[] = ajax_command_remove('#admin-' . $uid);
  $commands[] = ajax_command_after('#tournament-admins', theme('status_messages'));

  $build = array(
    '#type' => 'ajax',
    '#commands' => $commands,
  );

  return $build;
}

/**
 * Submit handler for removing tournament admins
 */
function tournament_admins_form_remove_submit($form, &$form_state) {
  // Determine the admin uid from the triggering element
  $uid = $form_state['triggering_element']['#parents'][1];

  // Remove the admin from our form state
  unset($form_state['admins'][$uid]);

  // Our #ajax callback will handle the display of this
  drupal_set_message(t('You must click <strong>Save changes</strong> below to finalize any changes.'), 'warning');

  // Add the admin to a list of users pending deletion
  $form_state['del_admins'][] = $uid;

  // Force a rebuild to update form state values
  $form_state['rebuild'] = TRUE;
}

/**
 * Ajax callback for adding tournament admins
 */
function tournament_admins_form_add_callback($form, $form_state) {
  return $form['admins'];
}

/**
 * Submit handler for adding tournament admins
 */
function tournament_admins_form_add_submit($form, &$form_state) {
  $form_state['num_admins']++;
  $form_state['rebuild'] = TRUE;
}

/**
 * Submit handler for editing tournament admins
 */
function tournament_admins_form_submit($form, &$form_state) {
  $node = $form_state['node'];

  // Delete admins
  if (!empty($form_state['del_admins'])) {
    db_delete('tournament_admins')
      ->condition('nid', $node->nid)
      ->condition('uid', $form_state['del_admins'])
      ->execute();
  }

  foreach ($form_state['values']['admins'] as $key => $value) {
    // Determine the uid of the admin
    if (is_numeric($key)) {
      $uid = $key;
    }
    elseif (!empty($value['name'])) {
      $account = user_load_by_name($value['name']);
      $uid = $account->uid;
    }
    else {
      $uid = NULL;
    }

    // Insert/update admins table
    if (!empty($uid)) {
      db_merge('tournament_admins')
        ->key(array(
          'nid' => $node->nid,
          'uid' => $uid,
        ))
        ->insertFields(array(
          'nid' => $node->nid,
          'uid' => $uid,
          'created' => REQUEST_TIME,
        ))
        ->execute();
    }
  }
}

/**
 * Returns HTML for the edit tournament admins form.
 *
 * @param $variables
 *   An associative array containing:
 *   - form: A render element representing the form.
 */
function theme_tournament_admins_form_admins($variables) {
  $element = $variables['element'];

  // Tournament admins
  foreach (element_children($element) as $uid) {
    $row = array(
      'data' => array(
        'name' => array('data' => drupal_render($element[$uid]['name'])),
      ),
      'class' => array('admin'),
      'id' => 'admin-' . $uid,
    );

    if (!empty($element[$uid]['action'])) {
      $row['data'][] = drupal_render($element[$uid]['action']);
    }
    else {
      $row['data']['name']['colspan'] = 2;
    }

    $rows[] = $row;
  }

  $header = array(array('data' => t('Admins'), 'colspan' => 2));
  $output = theme('table', array('header' => $header, 'rows' => $rows, 'attributes' => array('id' => 'tournament-admins')));
  return $output;
}

/**
 * Form generator; Tournament signup form
 */
function tournament_signup_form($form, &$form_state, $node) {
  global $user;

  drupal_set_title(t('Sign up for @tournament', array('@tournament' => $node->title)));

  // Full list of participant the user has access over
  $ids = match_participants_access_list($node->entity_type);

  // If multiple participants are returned, remove any that are already signed up.
  if (count($ids) > 1) {
    // Return participants that are already signed up from $ids
    $ids_exist = tournament_participants_exist_list($node, $ids);

    // Remove pre-existing participants from the list
    $ids = array_diff($ids, $ids_exist);
  }

  $entities = entity_load($node->entity_type, $ids);

  $options = array();
  foreach ($entities as $id => $entity) {
    $options[$id] = entity_label($node->entity_type, $entity);
  }

  // If tournament has signup deadline check signup deadline
  if (!$node->signup_access) {
    $error = t('The administrator has disabled tournament registration.');
  }
  elseif (!empty($node->signup_deadline) && $node->signup_deadline < REQUEST_TIME) {
    $error = t('The deadline for signups has passed. The deadline was @deadline.', array('@deadline' => format_date($node->signup_deadline)));
  }
  elseif (!$user->uid) {
    $error = t('A user account is required to participate in tournaments. Please <a href="@register">register</a> a user account or <a href="@login">login</a> to continue.', array('@register' => url('user/register'), '@login' => url('user/login')));
  }
  elseif (empty($options)) {
    $info = entity_get_info();
    $error = t('You are not able to sign up to this tournament. This may be because you are not the administrator of any @entity_type.', array('@entity_type' => $info[$node->entity_type]['label']));
  }
  elseif ($node->entity_type == 'user' && !empty($node->signup_role)) {
    // Check user role for user tournaments, other entity types should handle their own errors
    $role = user_role_load($node->signup_role);
    if (!in_array($role->name, $user->roles)) {
      $error = t('Only users with role %name are able to sign up to this tournament.', array('%name' => $role->name));
    }
  }

  // Return a short form if errors existed
  if (isset($error)) {
    $form['error'] = array(
      '#markup' => $error,
    );

    return $form;
  }

  // Determine whether sign up is full (waiting list)
  $count = db_select('tournament_participants')
    ->condition('nid', $node->nid)
    ->condition('status', 1)
    ->countQuery()
    ->execute()
    ->fetchField();

  if (!empty($node->size) && $node->size <= $count) {
    $form['warning'] = array(
      '#prefix' => '<p>',
      '#suffix' => '</p>',
      '#markup' => t('The maximum size for this tournament has been reached. You can still sign up but you will be added to the waiting list.'),
    );
  }

  $info = entity_get_info();
  $form_state['node'] = $node;

  if (count($options) > 1) {
    $form['id'] = array(
      '#type' => 'select',
      '#title' => t('Choose a @entity_type', array('@entity_type' => $info[$node->entity_type]['label'])),
      '#options' => $options,
      '#default_value' => key($options),
    );
  }
  else {
    $keys = array_keys($options);
    $id = array_shift($keys);

    $form['id'] = array(
      '#type' => 'value',
      '#value' => $id,
    );

    $form['description'] = array(
      '#prefix' => '<p>',
      '#suffix' => '</p>',
      '#markup' => t('By clicking Sign up below, you will register for this tournament as <strong>@name</strong>.', array('@name' => $options[$id])),
    );
  }

  if ($node->signup_pass) {
    $form['pass'] = array(
      '#type' => 'password',
      '#title' => t('Password'),
      '#required' => TRUE,
      '#attributes' => array('autocomplete' => 'off'),
      '#description' => t('The tournament administrator has set a password for signing up.'),
    );
  }

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Sign up')
  );

  return $form;
}

/**
 * Validate handler for the tournament signup form.
 *
 * @see tournament_signup_form()
 */
function tournament_signup_form_validate($form, &$form_state) {
  $node = $form_state['node'];

  if (isset($form_state['values']['pass'])) {
    // Validate password against the hashed tournament signup pass
    if (!_tournament_check_password($node, trim($form_state['values']['pass']))) {
      form_set_error('pass', t('The password entered was incorrect.'));
    }
  }
}

/**
 * Submit handler for the tournament signup form.
 *
 * @see tournament_signup_form()
 */
function tournament_signup_form_submit($form, &$form_state) {
  $node = $form_state['node'];
  $id = $form_state['values']['id'];

  tournament_participant_add($node, $id);

  drupal_set_message(t('You have successfully signed up to this tournament.'));
  $form_state['redirect'] = 'node/' . $node->nid;
}

/**
 * Form generator; Tournament signup form
 */
function tournament_leave_form($form, &$form_state, $node) {
  global $user;

  drupal_set_title(t('Leave !tournament', array('!tournament' => $node->title)));

  // Full list of participant the user has access over
  $ids = match_participants_access_list($node->entity_type);

  // If multiple participants are returned, leave only those that are signed up.
  if (count($ids) > 1) {
    // Return participants that are already signed up from $ids
    $ids = tournament_participants_exist_list($node, $ids);
  }

  $entities = entity_load($node->entity_type, $ids);

  $options = array();
  foreach ($entities as $id => $entity) {
    $options[$id] = entity_label($node->entity_type, $entity);
  }

  $info = entity_get_info();
  $form_state['node'] = $node;

  if (count($options) > 1) {
    $form['id'] = array(
      '#type' => 'select',
      '#title' => t('Choose a @entity_type', array('@entity_type' => $info[$node->entity_type]['label'])),
      '#options' => $options,
      '#description' => t('By clicking Leave below, the chosen participant will be removed from this tournament.')
    );
  }
  else {
    $keys = array_keys($options);
    $id = array_shift($keys);

    $form['id'] = array(
      '#type' => 'value',
      '#value' => $id,
    );

    $form['description'] = array(
      '#prefix' => '<p>',
      '#markup' => t('By clicking Leave below, <strong>@name</strong> will be removed from this tournament.', array('@name' => $options[$id])),
      '#suffix' => '</p>'
    );
  }

  $form['submit'] = array('#type' => 'submit', '#value' => t('Leave'));

  return $form;
}

/**
 * Submit handler for the tournament participant leave form.
 *
 * @see tournament_leave_form()
 */
function tournament_leave_form_submit($form, &$form_state) {
  $node = $form_state['node'];
  $id = $form_state['values']['id'];

  tournament_participant_delete($id, $node->entity_type, $node);
  drupal_set_message(t('You have left tournament @tournament successfully.', array('@tournament' => $node->title)));

  // Redirect to the tournament page
  $form_state['redirect'] = 'node/' . $node->nid;
}

/**
 * Form generator; Tournament start form
 */
function tournament_start_form($form, &$form_state, $node) {
  $form_state['node'] = $node;

  $form['warning'] = array(
    '#type' => 'fieldset',
    '#title' => t('Warning'),
    '#description' => t('By starting this tournament, all participants will be notified. You cannot delete tournaments after they have started.'),
    '#weight' => 40,
  );
  $form['warning']['agree'] = array(
    '#type' => 'checkbox',
    '#title' => t('I have read and understand the warning'),
    '#required' => TRUE,
  );

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Start tournament'),
  );
  $form['actions']['cancel'] = array(
    '#markup' => l(t('Cancel'), 'node/' . $node->nid),
  );

  return $form;
}

/**
 * Submit handler for the tournament start form.
 *
 * @see tournament_start_form()
 */
function tournament_start_form_submit($form, &$form_state) {
  global $user;
  $node = $form_state['node'];

  // Start the tournament
  $node->tournament_status = TOURNAMENT_STATUS_STARTED;
  node_save($node);

  // TODO: Notify participants
  module_invoke_all('tournament_start', $node);

  // Redirect to the tournament page
  drupal_set_message(t('The tournament has been started successfully.'));
  $form_state['redirect'] = 'node/' . $node->nid;
}
