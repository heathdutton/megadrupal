<?php
/**
 * @file
 * Theme and preprocess functions for the Election Vote module.
 */

/**
 * Display content on the voting confirm form.
 *
 * Variables:
 *   $confirm  - A render array, to be rendered on the confirm form.
 */
function theme_election_vote_confirm($variables) {

  $confirm = $variables['confirm'];

  return render($confirm);

}

/**
 * Add the default message for the confirm form.
 */
function template_preprocess_election_vote_confirm(&$variables) {

  $variables['confirm'] = array();

  if (!user_access('undo own vote')) {
    $variables['confirm']['warning_undo'] = array(
      '#markup' => '<p>' . t('This action cannot be undone.') . '</p>',
      '#weight' => 50,
    );
  }

}

/**
 * Determine the user's eligibility for a given post.
 */
function template_preprocess_election_vote_eligibility(&$variables) {

  $account = $variables['account'];

  // If an account is not provided, check for the logged-in user.
  global $user;
  if (empty($account)) {
    $account = $user;
  }
  $account_is_user = ($account->uid == $user->uid);

  $variables['eligibility'] = NULL;

  $post = $variables['post'];
  if (empty($post)) {
    return;
  }

  $election = $post->election;
  $post_name = election_get_post_name($election->type);

  // Check whether the post is closed individually.
  if ($post->vstatus_inheritance == ELECTION_POST_STATUS_CLOSED) {
    $eligibility = t('Voting is currently closed for this @post.', array('@post' => $post_name));
  }
  // Check whether the whole election is closed.
  elseif (!election_voting_open($election)) {
    // If scheduled, explain when voting will open (or when it closed).
    if ($election->vstatus == ELECTION_STATUS_SCHEDULED) {
      if ($election->vopen_time > REQUEST_TIME) {
        $eligibility = t('Voting will open in %interval.', array(
          '%interval' => format_interval($election->vopen_time - REQUEST_TIME, 2),
        ));
        if (user_access('view voting access explanation', $account)) {
          $eligibility = rtrim($eligibility, '.');
          $eligibility .= '. ' . t('<a href="@explain_url">Check access</a>.', array(
            '@explain_url' => url('election-post/' . $post->post_id . '/explain'),
          ));
        }
      }
      elseif ($election->vclose_time < REQUEST_TIME) {
        $eligibility = t('Voting closed %interval ago.', array(
          '%interval' => format_interval(REQUEST_TIME - $election->vclose_time, 1),
        ));
      }
    }
    else {
      $eligibility = t('Voting is currently closed.');
    }
  }
  // Check if there are enough candidates to vote for.
  elseif (module_exists('election_candidate') && !election_candidate_post_has_enough($post)) {
    $eligibility = t('Voting is currently closed (not enough candidates).');
  }
  // Check voting access.
  elseif (!election_vote_access('vote', $post)) {
    // Access denied. If the user is not logged in, provide a link.
    if ($account_is_user && !user_is_logged_in()) {
      $eligibility = t('<a href="@login_uri" class="election-vote-link-login">Log in</a> to vote.', array(
          '@login_uri' => url(
            'user/login',
            array('query' => array('destination' => $_GET['q']))
          ),
      ));
    }
    // The account is not eligible.
    elseif ($account_is_user) {
      $eligibility = t(
        'You are not eligible to vote for this @post.',
        array('@post' => $post_name)
      );
    }
    else {
      $eligibility = t('Not eligible');
    }
    if (user_access('view voting access explanation', $account)) {
      $eligibility = rtrim($eligibility, '.');
      $eligibility .= ' ' . t('(<a href="@explain_url">explain</a>).', array(
        '@explain_url' => url('election-post/' . $post->post_id . '/explain'),
      ));
    }
  }
  // The account is eligible. Check whether the user has already voted.
  elseif (election_vote_check_account_previously_voted($post, $account)) {
    if ($post->allow_abstention) {
      $eligibility = $account_is_user ? t('You have already voted or abstained.') : t('Already voted or abstained');
    }
    else {
      $eligibility = $account_is_user ? t('You have already voted.') : t('Already voted');
    }
    if ($account_is_user && election_vote_access('undo', $post)) {
      $eligibility .= ' ' . l(t('Undo'), election_post_uri_path($post) . '/vote/undo', array('query' => drupal_get_destination()));
    }
  }
  // The user may vote. Provide a link to the voting form.
  elseif ($account_is_user) {
    $eligibility = t('You can <a href="@vote_link" class="election-vote-link-vote">vote</a>.', array(
      '@vote_link' => url(election_post_uri_path($post) . '/vote'),
    ));
  }
  else {
    $eligibility = t('Eligible');
  }

  $variables['eligibility'] = $eligibility;

}

/**
 * Display a user's voting eligibility for a given post.
 *
 * Variables are:
 *   $post         - The election_post entity.
 *   $account      - The user account to check (default: the logged-in user).
 *   $eligibility  - A string showing the user's eligibility.
 */
function theme_election_vote_eligibility($variables) {

  return '<span class="election-vote-eligibility">'
    . $variables['eligibility']
    . '</span>';

}

/**
 * Generate variables for theme_election_vote_button().
 *
 * @see theme_election_vote_button()
 */
function template_preprocess_election_vote_button(&$variables) {

  $post = $variables['post'];

  $variables['path'] = election_post_uri_path($post) . '/vote';

  $variables['can_vote'] = (
    election_vote_access('vote', $post)
    && !election_vote_check_account_previously_voted($post)
  );

}

/**
 * Display a button linking to the voting form.
 *
 * Variables:
 *   $post        - The election post object.
 *   $can_vote    - A Boolean indicating whether the user may vote.
 *   $path        - The path to the voting page.
 */
function theme_election_vote_button($variables) {

  $output = '';

  if ($variables['can_vote']) {
    $output .= '<div class="election-vote-button">';
    $output .= l(
      t('Vote'),
      $variables['path']
    );
    $output .= '</div>';
  }

  return $output;

}

/**
 * Explain the voting access limitations for a user/post.
 */
function theme_election_vote_access_explain($variables) {
  $header = array(t('Test'), t('Result'));
  $rows = array();

  foreach ($variables['limitations'] as $limitation) {
    $result = $limitation['result'] ? t('Passed') : t('Failed');
    $class = $limitation['result'] ? 'election-vote-access-passed' : 'election-vote-access-failed';
    $rows[] = array(
      $limitation['explanation'],
      '<span class="' . $class . '">' . $result . '</span>',
    );
  }

  return theme('table', array(
    'header' => $header,
    'rows' => $rows,
    'empty' => t('No explanation available'),
  ));
}
