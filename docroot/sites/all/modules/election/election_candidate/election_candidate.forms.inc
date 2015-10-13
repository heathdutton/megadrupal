<?php
/**
 * @file
 * Form functions for the Election Candidate module.
 */

/**
 * Form builder function for editing a single candidate.
 */
function election_candidate_form($form, &$form_state, stdClass $candidate, $post) {

  global $user;

  $election = $post ? $post->election : election_load($candidate->election_id);
  $candidate->is_new = empty($candidate->is_new) ? FALSE : TRUE;

  $form['#id'] = 'election-candidate-form';
  $form['#candidate'] = $candidate;

  $form_state['election'] = $election;
  $form_state['candidate'] = $candidate;

  // Determine whether the user is an 'election admin', i.e. he/she has access
  // to update the election. This automatically gives access to create, update,
  // or delete candidates in this election.
  $election_admin = election_access('update', $election);

  $post_name = election_get_post_name($election->type);
  $posts = election_post_load_by_election($election, !$election_admin);
  $post_options = array();
  foreach ($posts as $post_id => $post_option) {
    $post_options[$post_id] = $post_option->title;
  }
  $form['post_id'] = array(
    '#type' => 'select',
    '#title' => check_plain(drupal_ucfirst($post_name)),
    '#required' => TRUE,
    '#options' => $post_options,
    '#default_value' => $post ? $post->post_id : NULL,
    '#disabled' => $post && $candidate->is_new,
  );

  field_attach_form('election_candidate', $candidate, $form, $form_state);

  foreach ($form as $key => &$field) {
    if (strpos($key, 'field_') === 0 && isset($field['#required'])) {
      $field['#required'] = FALSE;
    }
  }

  if ($election_admin) {
    $form['cstatus'] = array(
      '#type' => 'select',
      '#options' => _election_candidate_get_cstatus_options(),
      '#title' => t('Status'),
      '#required' => TRUE,
      '#default_value' => $candidate->cstatus,
      '#empty_option' => t('- Select status -'),
      '#weight' => 97,
    );
    $candidate_account = NULL;
    if (!empty($candidate->uid)) {
      $candidate_account = user_load($candidate->uid);
    }
    $form['user'] = array(
      '#title' => t('Associated user account'),
      '#type' => 'textfield',
      '#maxlength' => 60,
      '#autocomplete_path' => 'user/autocomplete',
      '#default_value' => empty($candidate_account) ? $user->name : $candidate_account->name,
      '#weight' => 98,
    );
    $form['published'] = array(
      '#type' => 'checkbox',
      '#title' => t('Published'),
      '#default_value' => isset($candidate->published) ? $candidate->published : 0,
      '#weight' => 99,
      '#description' => t('When published, candidates can be viewed by those who are allowed to view the election.'),
    );
  }

  $form['buttons'] = array(
    '#type' => 'actions',
    '#weight' => 100,
  );
  $form['buttons']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
    '#submit' => array('election_candidate_form_submit'),
  );

  if ($election_admin && !$candidate->is_new) {
    $form['buttons']['delete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete'),
      '#submit' => array('election_candidate_form_delete_submit'),
      '#access' => entity_access('delete', 'election_candidate', $candidate),
      '#limit_validation_errors' => array(),
    );
  }

  if (!empty($_GET['destination'])) {
    $options = drupal_parse_url($_GET['destination']);
    $cancel_path = $options['path'];
  }
  elseif (!$candidate->is_new) {
    $cancel_path = election_candidate_uri_path($candidate);
  }
  else {
    $cancel_path = election_post_uri_path($post);
  }
  $form['buttons']['cancel'] = array(
    '#type' => 'link',
    '#title' => t('Cancel'),
    '#href' => $cancel_path,
  );

  form_load_include($form_state, 'inc', 'election_candidate', 'election_candidate.forms');
  $form['#validate'][] = 'election_candidate_form_validate';

  return $form;

}

/**
 * Form submit function for the Save button in election_candidate_form.
 */
function election_candidate_form_submit($form, &$form_state) {

  $candidate = &$form_state['candidate'];

  foreach ($candidate as $property => $value) {
    if (isset($form_state['values'][$property])) {
      $candidate->$property = $form_state['values'][$property];
    }
  }

  if (!empty($form_state['values']['user'])) {
    if ($account = user_load_by_name($form_state['values']['user'])) {
      $candidate->uid = $account->uid;
    }
  }

  field_attach_submit('election_candidate', $candidate, $form, $form_state);

  election_candidate_save($candidate);

  drupal_set_message(
    t(
      'The candidate %name was saved.',
      array(
        '%name' => election_candidate_get_name($candidate, FALSE),
      )
    )
  );

  if (empty($_GET['destination'])) {
    $form_state['redirect'] = election_candidate_uri_path($candidate);
  }

}

/**
 * Form submit function for the Delete button in election_candidate_form.
 */
function election_candidate_form_delete_submit($form, &$form_state) {
  $destination = array();
  if (isset($_GET['destination'])) {
    $destination = drupal_get_destination();
    unset($_GET['destination']);
  }
  $candidate = $form['#candidate'];
  $form_state['redirect'] = array(
    election_candidate_uri_path($candidate) . '/delete',
    array('query' => $destination),
  );
}

/**
 * Form builder function to confirm the delete action for a candidate.
 */
function election_candidate_form_delete_confirm($form, &$form_state, stdClass $candidate) {

  $election = election_load($candidate->election_id);

  election_set_breadcrumb($election);

  $form['candidate_id'] = array(
    '#type' => 'value',
    '#value' => $candidate->candidate_id,
  );

  return confirm_form(
    $form,
    t('Are you sure you want to delete the candidate %name?', array('%name' => election_candidate_get_name($candidate, FALSE))),
    election_candidate_uri_path($candidate),
    t('Deleting a candidate also deletes ALL associated data, for example votes. This action cannot be undone.'),
    t('Delete'),
    t('Cancel')
  );
}

/**
 * Form submit function for confirming delete for an election candidate.
 */
function election_candidate_form_delete_confirm_submit($form, &$form_state) {
  if ($form_state['values']['confirm']) {

    $candidate = election_candidate_load($form_state['values']['candidate_id']);
    $election = election_load($candidate->election_id);

    $info = array(
      '%name' => election_candidate_get_name($candidate, FALSE),
      '%election_title' => !empty($election) ? $election->title : t('unknown'),
    );

    // Delete the candidate.
    election_candidate_delete($form_state['values']['candidate_id']);
    watchdog('election_candidate', 'Deleted candidate %name.', $info);
    drupal_set_message(
      t('The candidate %name has been deleted from the election %election_title.', $info)
    );
  }

  if (empty($_GET['destination']) && $election) {
    $form_state['redirect'] = election_uri_path($election) . '/candidates';
  }

}

/**
 * Nomination form for a post. This creates election_candidate entities.
 */
function election_candidate_nomination_form($form, &$form_state, stdClass $post) {

  $candidate_type = !empty($post->settings['candidate_type']) ? $post->settings['candidate_type'] : 'candidate';
  $candidate = entity_get_controller('election_candidate')->create(array(
    'type' => $candidate_type,
    'is_new' => TRUE,
  ));

  $form['#id'] = 'election-nomination-form';

  if (!isset($form_state['seconder_count'])) {
    $form_state['seconder_count'] = 1;
  }

  $election = $post->election;

  $form_state['candidate'] = $candidate;
  $form_state['election'] = $election;
  $form_state['post'] = $post;

  $previous_nominations = (array) election_candidate_get_nominations($election);
  if (count($previous_nominations)) {
    $form['previous_nominations'] = array(
      '#type' => 'fieldset',
      '#title' => t('Your previous nominations'),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
      '#weight' => -50,
    );
    $previous_nominations_display = election_candidate_display_previous_nominations($election, $previous_nominations);
    unset($previous_nominations_display['#caption']);
    $form['previous_nominations']['display'] = $previous_nominations_display;
  }

  $form['post'] = array(
    '#type' => 'item',
    '#title' => check_plain(drupal_ucfirst(election_get_post_name($election->type))),
    '#markup' => check_plain($post->title),
  );

  // Add custom fields from the Field system.
  field_attach_form('election_candidate', $candidate, $form, $form_state);

  $form['buttons'] = array(
    '#type' => 'actions',
    '#weight' => 100,
  );
  $form['buttons']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit nomination'),
    '#submit' => array('election_candidate_nomination_form_submit'),
  );

  if (!empty($_GET['destination'])) {
    $options = drupal_parse_url($_GET['destination']);
    $cancel_path = $options['path'];
    $form['buttons']['cancel'] = array(
      '#type' => 'link',
      '#title' => t('Cancel'),
      '#href' => $cancel_path,
    );
  }

  form_load_include($form_state, 'inc', 'election_candidate', 'election_candidate.forms');
  $form['#validate'][] = 'election_candidate_form_validate';

  return $form;
}


/**
 * Validation function for both the candidate edit form and the nomination form.
 */
function election_candidate_form_validate($form, &$form_state) {

  field_attach_form_validate('election_candidate', $form_state['candidate'], $form, $form_state);

}

/**
 * Submit callback for the nomination form.
 */
function election_candidate_nomination_form_submit($form, &$form_state) {

  $candidate = &$form_state['candidate'];

  $election = $form_state['election'];
  $post = $form_state['post'];

  $candidate->election_id = $election->election_id;
  $candidate->post_id = $post->post_id;

  // Newly nominated candidates start with the 'Hopeful' status, and
  // unpublished.
  $candidate->cstatus = ELECTION_CANDIDATE_HOPEFUL;
  $candidate->published = 0;

  field_attach_submit('election_candidate', $candidate, $form, $form_state);

  if (!empty($GLOBALS['user']->uid)) {
    $candidate->uid = $GLOBALS['user']->uid;
  }

  // Save the nomination.
  election_candidate_save($candidate);

  // Notify the user that the nomination was saved.
  drupal_set_message(t(
    'The nomination for the @post %title (name: %name) was successfully submitted.',
    array(
      '@post' => election_get_post_name($election->type),
      '%title' => $post->title,
      '%name' => election_candidate_get_name($candidate, FALSE),
    )
  ));

  // Invoke rules for "After submitting a nomination".
  if (module_exists('rules')) {
    rules_invoke_event('election_candidate_nomination_submitted', $GLOBALS['user'], $candidate, $post, $election);
  }

  if (empty($_GET['destination'])) {
    $form_state['redirect'] = election_uri_path($election);
  }

}
