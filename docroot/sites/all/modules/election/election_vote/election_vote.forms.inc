<?php
/**
 * @file
 * Form functions for the Election Vote module.
 */

/**
 * Form builder function for the voting form.
 *
 * This just builds a basic form with two submit buttons ('Vote' and, if
 * enabled, 'Abstain'), and deals with the logic for subsequent confirmation
 * forms. The confirmation stages build upon this form; they do not have
 * separate form IDs.
 *
 * Election types should define 'vote form' callbacks to construct the rest of
 * the voting form. They act similarly to hook_form_FORM_ID_alter(), but only
 * apply to the voting stage, rather than altering the confirmation forms at
 * the same time.
 */
function election_vote_form($form, &$form_state, stdClass $post) {

  if (isset($form_state['vote_require_confirm'])) {
    return election_vote_form_vote_confirm($form, $form_state);
  }

  if (isset($form_state['abstain_require_confirm'])) {
    return election_vote_form_abstain_confirm($form, $form_state);
  }

  $election = $post->election;

  drupal_set_title(
    t('Vote: %title_trim', array(
      '%title_trim' => truncate_utf8($post->title, 50, TRUE, TRUE),
    )),
    PASS_THROUGH
  );
  election_post_set_breadcrumb($election, $post);

  $form_state['election'] = $election;
  $form_state['post'] = $post;

  if (election_vote_check_account_previously_voted($post)) {
    drupal_set_message(
      t(
        'You have already voted (or abstained) for this @post.',
        array('@post' => election_get_post_name($election->type))
      ),
      'status',
      FALSE
    );
    drupal_goto(election_post_uri_path($post));
    return;
  }
  else {
    // At this stage it's safe to assume the user has not voted.
    // The session-based check can therefore be cleared if it exists.
    $_SESSION['election_vote_cast_' . $post->post_id] = FALSE;
  }

  $form['buttons'] = array(
    '#type' => 'actions',
    '#weight' => 100,
  );
  $form['buttons']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Vote…'),
    '#submit' => array('election_vote_form_vote'),
  );

  if (!empty($post->allow_abstention)) {
    $form['buttons']['abstain'] = array(
      '#type' => 'submit',
      '#value' => t('Abstain…'),
      '#submit' => array('election_vote_form_abstain'),
      '#limit_validation_errors' => array(),
    );
  }

  $form['#attached']['js'][] = drupal_get_path('module', 'election') . '/submit-single.min.js';

  // Allow the form to be modified via the election type's 'vote form' callback.
  if (!empty($election->type_info['vote form'])) {
    $function = $election->type_info['vote form'];
    if (function_exists($function)) {
      $function($form, $form_state);
    }
    else {
      watchdog('election_vote', 'Vote form callback not found: @cb()', array('@cb' => $function), WATCHDOG_ERROR);
    }
  }

  return $form;

}

/**
 * The confirmation stage for voting.
 *
 * @see election_vote_form()
 */
function election_vote_form_vote_confirm($form, &$form_state) {

  $form['vote_confirmed'] = array(
    '#type' => 'hidden',
    '#value' => TRUE,
  );

  $form['#submit'] = array('election_vote_form_vote');

  $form['#attached']['js'][] = drupal_get_path('module', 'election') . '/submit-single.min.js';

  // confirm_form() ignores its third argument when $_GET['destination'] is set.
  unset($_GET['destination']);

  return confirm_form(
    $form,
    t('Are you sure you want to cast this vote?'),
    current_path(),
    theme('election_vote_confirm', array('form_state' => $form_state)),
    t('Confirm'),
    t('Cancel')
  );

}

/**
 * Form for undoing a vote or abstention.
 */
function election_vote_undo_form($form, &$form_state, $post) {

  $form['#attached']['js'][] = drupal_get_path('module', 'election') . '/submit-single.min.js';

  $form_state['post'] = $post;
  $election = $post->election;

  election_post_set_breadcrumb($election, $post);

  return confirm_form(
    $form,
    t('Undo: are you sure?'),
    election_uri_path($election),
    t("This will undo your vote !or_abstention for the @post %post_title.", array(
      '@post' => election_get_post_name($election->type),
      '%post_title' => $post->title,
      '!or_abstention' => $post->allow_abstention ? t('or abstention') : '',
    )),
    t('Confirm'),
    t('Cancel')
  );

}

/**
 * Submit callback for undoing a vote or abstention.
 *
 * @see election_vote_undo_form()
 */
function election_vote_undo_form_submit($form, &$form_state) {

  $post = $form_state['post'];
  $election = $post->election;

  $success = FALSE;

  $transaction = db_transaction();

  try {

    $ballot_ids = db_select('election_ballot', 'eb')
      ->fields('eb', array('ballot_id'))
      ->condition('uid', $GLOBALS['user']->uid)
      ->condition('post_id', $post->post_id)
      ->condition('valid', 1)
      ->execute()
      ->fetchCol();

    if (!count($ballot_ids)) {
      throw new Exception(t('No votes found.'));
    }

    db_update('election_vote')
      ->fields(array('valid' => 0))
      ->condition('ballot_id', $ballot_ids, 'IN')
      ->execute();

    db_update('election_ballot')
      ->fields(array('valid' => 0))
      ->condition('ballot_id', $ballot_ids, 'IN')
      ->execute();

    $success = TRUE;

  }
  catch (Exception $e) {
    $transaction->rollback();
    watchdog_exception('election_vote', $e, NULL, WATCHDOG_ERROR);
  }

  if ($success) {

    $post_name = election_get_post_name($election->type);
    drupal_set_message(
      t('You have undone your vote !or_abstention for the @post <a href="!post_url">%post_title</a>.', array(
        '@post' => $post_name,
        '!post_url' => url(election_post_uri_path($post)),
        '%post_title' => $post->title,
        '!or_abstention' => $post->allow_abstention ? t('or abstention') : '',
      ))
    );

    watchdog(
      'election_vote',
      'User @uid has undone a vote for %post_title (@pid) in %election_title (@eid).',
      array(
        '@uid' => $GLOBALS['user']->uid,
        '%post_title' => $post->title,
        '%election_title' => $election->title,
        '@pid' => $post->post_id,
        '@eid' => $election->election_id,
      )
    );

    $form_state['redirect'] = election_uri_path($election);

  }

}

/**
 * The confirmation stage for abstaining.
 *
 * @see election_vote_form()
 */
function election_vote_form_abstain_confirm($form, &$form_state) {

  $form['abstain_confirmed'] = array(
    '#type' => 'hidden',
    '#value' => TRUE,
  );

  $form['#submit'] = array('election_vote_form_abstain');

  $form['#attached']['js'][] = drupal_get_path('module', 'election') . '/submit-single.min.js';

  // confirm_form() ignores its third argument when $_GET['destination'] is set.
  unset($_GET['destination']);

  // @todo Make this confirmation page themeable.
  $warning = t('This action cannot be undone.');
  if (user_access('undo own vote')) {
    $warning = '';
  }

  return confirm_form(
    $form,
    t('Are you sure you want to abstain?'),
    current_path(),
    $warning
  );

}

/**
 * Submit callback for abstaining.
 *
 * @see election_vote_form()
 */
function election_vote_form_abstain($form, &$form_state) {

  if (empty($form_state['values']['abstain_confirmed'])) {
    $form_state['abstain_require_confirm'] = TRUE;
    $form_state['rebuild'] = TRUE;
    return;
  }

  $post = $form_state['post'];
  $election = $form_state['election'];

  // Check to prevent duplicate form submissions.
  if (!empty($_SESSION['election_vote_cast_' . $post->post_id])) {
    $form_state['redirect'] = election_uri_path($election);
    return;
  }

  $success = FALSE;

  $transaction = db_transaction();

  try {

    $ballot = array(
      'election_id' => $election->election_id,
      'post_id' => $post->post_id,
      'uid' => $GLOBALS['user']->uid,
      'ip' => ip_address(),
      'agent' => $_SERVER['HTTP_USER_AGENT'],
      'sid_hash' => _election_vote_session_hash(),
      'timestamp' => REQUEST_TIME,
    );
    $ballot_id = db_insert('election_ballot')
      ->fields($ballot)
      ->execute();
    if (empty($ballot_id)) {
      throw new Exception('The vote record could not be saved.');
    }

    $vote = array(
      'ballot_id' => $ballot_id,
      'election_id' => $election->election_id,
      'post_id' => $post->post_id,
      'abstain' => 1,
    );
    $success = db_insert('election_vote')
      ->fields($vote)
      ->execute();

  }
  catch (Exception $e) {
    $transaction->rollback();
    watchdog_exception('election_vote', $e, NULL, WATCHDOG_ERROR);
  }

  if ($success) {

    // Save a session variable to prevent duplicate form submissions.
    $_SESSION['election_vote_cast_' . $post->post_id] = TRUE;

    $form_state['redirect'] = election_uri_path($election);

    // Invoke "After abstaining" hooks and rules.
    rules_invoke_all('election_vote_abstain', $GLOBALS['user'], $post, $election);

  }

}

/**
 * Submit callback for voting.
 *
 * @see election_vote_form()
 */
function election_vote_form_vote($form, &$form_state) {

  if (empty($form_state['values']['vote_confirmed'])) {
    $form_state['vote_require_confirm'] = TRUE;
    $form_state['rebuild'] = TRUE;
    return;
  }

  $post = $form_state['post'];
  $election = $form_state['election'];

  // Check to prevent duplicate form submissions.
  if (!empty($_SESSION['election_vote_cast_' . $post->post_id])) {
    $form_state['redirect'] = election_uri_path($election);
    return;
  }

  $success = FALSE;

  $transaction = db_transaction();

  try {

    $ballot = array(
      'election_id' => $election->election_id,
      'post_id' => $post->post_id,
      'uid' => $GLOBALS['user']->uid,
      'ip' => ip_address(),
      'agent' => $_SERVER['HTTP_USER_AGENT'],
      'sid_hash' => _election_vote_session_hash(),
      'timestamp' => REQUEST_TIME,
    );
    $ballot_id = db_insert('election_ballot')
      ->fields($ballot)
      ->execute();
    if (empty($ballot_id)) {
      throw new Exception('The ballot record could not be saved.');
    }

    // Invoke hook_election_vote_ELECTION_TYPE_save().
    $hook = 'election_vote_' . $election->type . '_save';
    $vote_save = module_invoke_all($hook, $ballot_id, $post, $form, $form_state);
    // Fail if any of the hooks returned something other than TRUE.
    foreach ($vote_save as $return_value) {
      if ($return_value !== TRUE) {
        throw new Exception('The vote record could not be saved.');
      }
    }

    $success = TRUE;
  }
  catch (Exception $e) {
    $transaction->rollback();
    $success = FALSE;
    watchdog_exception('election_vote', $e, NULL, WATCHDOG_ERROR);
  }

  if ($success) {

    // Save a session variable to prevent duplicate form submissions.
    $_SESSION['election_vote_cast_' . $post->post_id] = TRUE;

    $form_state['redirect'] = election_uri_path($election);

    // Invoke "After casting a vote" hooks and rules.
    rules_invoke_all('election_vote_vote', $GLOBALS['user'], $post, $election);

  }

}
