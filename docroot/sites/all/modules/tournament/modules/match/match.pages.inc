<?php

/**
 * @file
 * Page callback file for the team module.
 */

/**
 * Menu callback - ask for confirmation of team deletion
 */
function match_remove_participant_confirm($form, &$form_state, $node, $id) {
  $form_state['node'] = $node;
  $form_state['id'] = $id;

  return confirm_form($form,
    t('Are you sure you want to remove %name?', array('%name' => $node->participants[$id]['name'])),
    'node/' . $node->nid . '/edit',
    t('This action cannot be undone.'),
    t('Remove'),
    t('Cancel')
  );
}

/**
 * Execute team deletion
 */
function match_remove_participant_confirm_submit($form, &$form_state) {
  if ($form_state['values']['confirm']) {
    $node = $form_state['node'];
    $id = $form_state['id'];

    match_remove_participant($node, $id);
    drupal_set_message(t('Participant %name has been removed.', array('%name' => $node->participants[$id]['name'])));
  }

  $form_state['redirect'] = 'node/' . $node->nid . '/edit';
}

/**
 * Form builder; the match challenge form.
 */
function match_challenge_form($form, &$form_state, $entity_type, $entity) {
  $info = entity_get_info();

  drupal_set_title(t('Challenge'));

  // Entity key
  list($opponent_id) = entity_extract_ids($entity_type, $entity);

  // Retrieve the participant ids that the current user has access over
  $ids = match_participants_access_list($entity_type);

  // Make this available for other module form alterations
  $form_state['entity'] = $entity;
  $form_state['entity_type'] = $entity_type;

  // These values will be used in the form submission
  $form['entity_type'] = array(
    '#type' => 'value',
    '#value' => $entity_type
  );
  $form['opponent_id'] = array(
    '#type' => 'value',
    '#value' => $opponent_id,
  );

  if (empty($ids)) {
    $form['error'] = array(
      '#markup' => t('You are unable to create match challenges because you are not associated with a @entity.', array('@entity' => $info[$entity_type]['label'])),
    );
  }
  else {
    $opponent = entity_label($entity_type, $entity);
    $form['message'] = array(
      '#prefix' => '<p>',
      '#suffix' => '</p>',
      '#markup' => t('You are challenging @opponent to a match. This match has no relation to any tournament.', array('@opponent' => $opponent)),
      '#weight' => -10,
    );

    if (count($ids) > 1) {
      $entity_types = match_entity_types_list();

      $participants = entity_load($entity_type, $ids);
      foreach ($participants as $id => $participant) {
        $options[$id] = entity_label($entity_type, $participant);
      }

      $form['request_id'] = array(
        '#type' => 'select',
        '#title' => $entity_types[$entity_type],
        '#options' => $options,
        '#description' => t('Select who you would like to perform this action as.'),
      );
    }
    else {
      // Returns the first and only participant id
      $id = array_shift($ids);

      $form['request_id'] = array(
        '#type' => 'value',
        '#value' => $id,
      );
    }

    $form['match_date'] = array(
      '#type' => 'match_date',
      '#title' => t('Match date'),
      '#size' => 10,
      '#time_increment' => 10,
      '#default_value' => REQUEST_TIME,
      '#required' => TRUE,
    );
    $form['comment'] = array(
      '#type' => 'textfield',
      '#title' => t('Comment'),
      '#maxlength' => 140,
      '#resizable' => FALSE,
      '#required' => TRUE,
    );
    $form['actions']['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Send challenge'),
    );
  }

  return $form;
}

/**
 * Validate handler for the match challenge form.
 *
 * @see match_challenge_form()
 */
function match_challenge_form_validate($form, &$form_state) {
  $query = db_select('matches', 'm');
  $query->join('matches_participants', 'mp1', 'm.nid = mp1.nid');
  $query->join('matches_participants', 'mp2', 'm.nid = mp2.nid');

  $count = $query->condition('m.entity_type', $form_state['values']['entity_type'])
    ->condition('m.match_status', MATCH_STATUS_AWAITING_CHALLENGE_ACCEPT)
    ->condition('mp1.id', $form_state['values']['request_id'])
    ->condition('mp2.id', $form_state['values']['opponent_id'])
    ->countQuery()
    ->execute()
    ->fetchField();

  if (!empty($count)) {
    form_set_error('request_id', t('There is already a match challenge open between these participants that has not yet been responded to.'));
  }
}

/**
 * Submit handler for the match challenge form.
 *
 * @see match_challenge_form()
 */
function match_challenge_form_submit($form, &$form_state) {
  $opponent_id = $form_state['values']['opponent_id'];
  $request_id = $form_state['values']['request_id'];
  $entity_type = $form_state['values']['entity_type'];

  $entities = entity_load($entity_type, array($opponent_id, $request_id));

  $node = array(
    'challenge' => 1,
    'match_status' => MATCH_STATUS_AWAITING_CHALLENGE_ACCEPT,
    'request' => $form_state['values']['match_date'],
    'request_id' => $form_state['values']['request_id'],
    'challenge_comment' => $form_state['values']['comment'],
    'participants' => array(
      $request_id => array(
        'score' => 0,
        'confirm' => 1,
        'weight' => 0,
        'name' => entity_label($entity_type, $entities[$request_id]),
      ),
      $opponent_id => array(
        'score' => 0,
        'confirm' => 0,
        'weight' => 1,
        'name' => entity_label($entity_type, $entities[$opponent_id]),
      ),
    ),
  );

  $node = array_merge($node, $form_state['values']);
  $node = (object) $node;

  // We create match nodes for challenges.
  // After this call, $node will contain the updated node data (such as nid)
  match_create_match($node);

  $entities = entity_load($node->entity_type, array($opponent_id));
  $variables['entity'] = $entities[$opponent_id];
  match_send_mail($node, 'challenge_enter', $variables);

  drupal_set_message(t('You have successfully challenged @opponent.', array('@opponent' => $node->participants[$opponent_id]['name'])));

  module_invoke_all('match_challenge', $node);
}

/**
 * Form builder; the match action form.
 */
function match_action_form($form, &$form_state, $op, $node) {
  $title = match_title($node);
  drupal_set_title($title);

  $form_state['node'] = $node;
  $form_state['op'] = $op;

  // Gather a list of participants from this match that the current
  // user has access over
  $ids = array_keys($node->participants);
  $ids = match_participants_access_list($node->entity_type, $ids);

  // Describe what action the user is about to make
  $form['description'] = array(
    '#prefix' => '<p>',
    '#suffix' => '</p>',
    '#markup' => _match_action_description($node, $op),
    '#weight' => -10,
  );

  $form['actions'] = array('#type' => 'actions');

  switch ($op) {
    case 'result_enter':
      $form['participants_wrapper'] = array(
        '#type' => 'fieldset',
        '#title' => t('Result'),
      );
      $form['participants_wrapper']['participants'] = array(
        '#tree' => TRUE,
      );
      foreach ($node->participants as $id => $participant) {
        $form['participants_wrapper']['participants'][$id]['score'] = array(
          '#type' => 'textfield',
          '#title' => $participant['name'],
          '#default_value' => isset($participant['score']) ? $participant['score'] : 0,
          '#element_validate' => array('element_validate_integer'),
          '#size' => 15,
          '#required' => TRUE,
        );
      }
      $form['actions']['submit'] = array(
        '#type' => 'submit',
        '#value' => t('Submit result'),
        '#name' => $op,
      );
      break;

    case 'date_enter':
    case 'challenge_reverse':
      $form['original_date'] = array(
        '#type' => 'item',
        '#title' => t('Original match date'),
        '#markup' => format_date($node->match_date),
      );
      $form['match_date'] = array(
        '#type' => 'match_date',
        '#title' => t('New match date'),
        '#size' => 10,
        '#time_increment' => 10,
        '#default_value' => isset($node->match_date) ? $node->match_date : REQUEST_TIME,
        '#required' => TRUE,
      );
      $form['actions']['submit'] = array(
        '#type' => 'submit',
        '#value' => t('Submit date'),
        '#name' => $op,
      );
      break;

    case 'result_cancel':
    case 'date_cancel':
    case 'challenge_cancel':
      // Action canceling is performed by the request_id
      foreach ($ids as $key => $id) {
        if ($node->request_id != $id) {
          unset($ids[$key]);
        }
      }
      $form['actions']['submit'] = array(
        '#type' => 'submit',
        '#value' => t('Confirm'),
        '#name' => $op,
      );

      break;

    case 'result_respond':
    case 'date_respond':
    case 'challenge_respond':
      // Remove ids that have already confirmed the request
      foreach ($ids as $key => $id) {
        if ($node->participants[$id]['confirm']) {
          unset($ids[$key]);
        }
      }

      // Build review table markup
      if ($op == 'result_respond') {
        $header = array(t('Name'), t('Score'));
        foreach ($node->participants as $participant) {
          $rows[] = array(
            $participant['name'],
            $participant['score'],
          );
        }
      }
      elseif ($op == 'date_respond' || $op == 'challenge_reverse') {
        $rows = array(
          array(t('Original match date'), format_date($node->match_date)),
          array(t('New match date'), format_date($node->request)),
        );
      }
      elseif ($op == 'challenge_respond') {
        $rows = array(
          array(t('Match date'), format_date($node->request)),
        );

        if ($comment = match_challenge_comment($node)) {
          $rows[] = array(t('Comment'), $comment);
        }

        $form['reverse'] = array(
          '#prefix' => '<div class="request-match-date">',
          '#suffix' => '</div>',
          '#markup' => l(t('Request a new date'), 'node/' . $node->nid . '/action/challenge_reverse'),
          '#weight' => 5,
        );
      }

      // Render table as a render array for easy hooking
      $form['review'] = array(
        '#theme' => 'table',
        '#rows' => $rows,
        '#header' => isset($header) ? $header : array(),
      );

      $form['actions']['accept'] = array(
        '#type' => 'submit',
        '#value' => t('Accept'),
        '#name' => $op . '_accept',
      );
      $form['actions']['reject'] = array(
        '#type' => 'submit',
        '#value' => t('Reject'),
        '#name' => $op . '_reject',
      );
      break;
  }

  if (count($ids) > 1) {
    $entity_types = match_entity_types_list();

    foreach ($ids as $id) {
      $options[$id] = $node->participants[$id]['name'];
    }

    $form['id'] = array(
      '#type' => 'select',
      '#title' => $entity_types[$node->entity_type],
      '#options' => $options,
      '#description' => t('You are the administrator of multiple participants in this match. Select which participant you would like to perform this action as.'),
      '#weight' => -5,
    );
  }
  elseif (!empty($ids)) {
    // Returns the first and only participant id
    $id = array_shift($ids);

    $form['id'] = array(
      '#type' => 'value',
      '#value' => $id,
    );
  }

  // Provide "Force result" checkbox to admins for tournament match scores that
  // are not yet entered
  if ($op == 'result_enter' && !empty($node->tid)) {
    $tournament = node_load($node->tid);
    if (node_access('update', $tournament)) {
      // Check if the admin is also a match participant
      $ids = array_keys($node->participants);
      $ids = match_participants_access_list($node->entity_type, $ids);

      $form['admin'] = array(
        '#type' => 'checkbox',
        '#title' => t('Final result'),
        '#description' => t('This result is final and the match will be immediately closed. Disable to require participants to confirm this result before it becomes final.'),
        '#default_value' => empty($ids) ? 1 : 0,
        '#weight' => 10,
      );
    }
  }

  $url = isset($_GET['destination']) ? $_GET['destination'] : 'node/' . $node->nid;
  $form['actions']['cancel'] = array(
    '#markup' => l(t('Cancel'), $url),
  );

  return $form;
}

/**
 * Submit handler for the match action form.
 *
 * @see match_action_form()
 */
function match_action_form_submit($form, &$form_state) {
  $node = $form_state['node'];
  $op = $form_state['triggering_element']['#name'];

  if (!empty($form_state['values']['admin'])) {
    $op = 'result_enter_admin';
  }
  elseif (!empty($form_state['values']['id'])) {
    $request_id = $form_state['values']['id'];
  }

  if ($op == 'result_respond_accept' && !empty($node->tid)) {
    global $user;
    $tnode = node_load($node->tid);
    if (node_access('update', $tnode, $user)) {
      $op .= '_admin';
      unset($request_id);
    }
  }

  // Data store for match logging
  $variables = array();

  // Data store for match notification mails
  $params = array();

  switch ($op) {
    case 'result_enter_admin':

      // Only update score on admin entering score, not approving score via result response
      if (isset($form_state['values']['participants'])) {
        foreach ($form_state['values']['participants'] as $id => $participant) {
          // Force participant confirmation
          $node->participants[$id]['score'] = $participant['score'];
          $node->participants[$id]['confirm'] = 1;
        }
      }

      foreach ($node->participants as $id => $participant) {
        // Prepare match log data
        $variables['scores'][$id] = $participant['score'];
      }

      $node->match_status = MATCH_STATUS_FINISHED;
      drupal_set_message(t('Match result entered successfully.'));
      break;

    case 'result_enter':

      $participants = $form_state['values']['participants'];
      foreach ($participants as $id => $participant) {
        $node->participants[$id]['score'] = $participant['score'];
        $node->participants[$id]['confirm'] = 0;
        $variables['scores'][$id] = $participant['score'];
      }

      if (!empty($request_id)) {
        // The participant submitting this score confirms by default
        $node->participants[$request_id]['confirm'] = 1;
        $node->request_id = $request_id;
      }

      $node->request = REQUEST_TIME;
      $node->match_status = MATCH_STATUS_AWAITING_RESULT_ACCEPT;

      drupal_set_message(t('Match participants have been notified of your result entry. They are required to accept the result before it will become final.'));
      break;

    case 'result_respond_accept':
    case 'result_respond_accept_admin':

      if (isset($request_id)) {
        $node->participants[$request_id]['confirm'] = 1;
      }
      elseif ($op == 'result_respond_accept_admin') {
        // When an admin is accepting the result, set all participants to confirmed
        foreach ($node->participants as $id => $participant)  {
          $node->participants[$id]['confirm'] = 1;
        }
      }

      if (_match_confirmed($node)) {
        $node->match_status = MATCH_STATUS_FINISHED;
        drupal_set_message(t('You have successfully accepted the match result. The match is now finished.'));
      }
      else {
        drupal_set_message(t('You have successfully accepted the match result. Before the result is final, the matches other participants must also accept.'));
      }
      break;

    case 'result_respond_reject':
    case 'result_cancel':
      // Reset all match participant confirmations and scores
      foreach ($node->participants as &$participant) {
        $participant['score'] = 0;
        $participant['confirm'] = 0;
      }

      $node->match_status = MATCH_STATUS_AWAITING_RESULT;

      if ($op == 'result_respond_reject') {
        drupal_set_message(t('You have successfully rejected the result.'));
      }
      else {
        drupal_set_message(t('You have successfully cancelled the result.'));
      }
      break;

    case 'date_enter':
    case 'challenge_reverse':

      // Reset all match participant confirmations except the submiter
      foreach ($node->participants as $id => &$participant) {
        $participant['confirm'] = $id == $request_id ? 1 : 0;
      }

      $match_date = match_date_convert($form_state['values']['match_date']);
      $variables['date'] = $match_date;
      $node->request = $match_date;
      $node->request_id = $request_id;

      if ($op == 'date_enter') {
        $node->match_status = MATCH_STATUS_AWAITING_DATE_ACCEPT;
      }
      else {
        $node->match_date = $match_date;
        $node->match_status = MATCH_STATUS_AWAITING_CHALLENGE_ACCEPT;
      }

      drupal_set_message(t('Match participants have been notified of your date request. They are required to accept the request before it will become final.'));
      break;

    case 'date_respond_accept':

      $node->participants[$request_id]['confirm'] = 1;

      if (_match_confirmed($node)) {
        $node->match_status = MATCH_STATUS_AWAITING_RESULT;
        $node->match_date = $node->request;
        drupal_set_message(t('You have successfully accepted the date change request.'));
      }
      else {
        drupal_set_message(t('You have successfully accepted the date change request. Before the date is final, the matches other participants must also accept.'));
      }
      break;

    case 'date_respond_reject':
    case 'date_cancel':

      // Reset all match participant confirmations
      foreach ($node->participants as &$participant) {
        $participant['confirm'] = 0;
      }

      $node->match_status = MATCH_STATUS_AWAITING_RESULT;

      if ($op == 'result_respond_reject') {
        drupal_set_message(t('You have successfully rejected the date change request.'));
      }
      else {
        drupal_set_message(t('You have successfully cancelled the date change request.'));
      }
      break;

    case 'challenge_respond_accept':

      // Reset all match participant confirmations and scores
      foreach ($node->participants as &$participant) {
        $participant['confirm'] = 0;
      }

      // Delete any challenge comments
      db_delete('matches_challenges')->condition('nid', $node->nid)->execute();

      $node->match_status = MATCH_STATUS_AWAITING_RESULT;
      drupal_set_message(t('You have successfully accepted the match challenge.'));
      break;

    case 'challenge_respond_reject':
    case 'challenge_cancel':

      node_delete($node->nid);

      drupal_set_message(t('The match challenge has successfully been deleted.'));
      return;
  }

  // Save any match changes
  node_save($node);

  // If match status is now final, clear cache, since tournament HTML needs updating
  if ($node->match_status == MATCH_STATUS_FINISHED) {
    global $base_root;
    cache_clear_all($base_root . '/node/' . $node->tid, 'cache_page');
  }

  // Log match actions
  match_log($node, $op, $variables);

  // Mail notifications
  if (!in_array($op, array('challenge_cancel', 'date_cancel', 'result_cancel'))) {
    $ids = array_keys($node->participants);

    // Remove the participant making this request
    if ($op != 'result_enter_admin' && !empty($request_id)) {
      $ids = array_diff($ids, array($request_id));
    }

    $entities = entity_load($node->entity_type, $ids);
    foreach ($entities as $entity) {
      $variables['entity'] = $entity;
      match_send_mail($node, $op, $variables);
    }
  }

  if (!isset($_GET['destination'])) {
    $form_state['redirect'] = 'node/' . $node->nid;
  }
}
