<?php
/**
 * @file
 * Rules integration file for the Election Vote module.
 */

/**
 * Implements hook_rules_event_info().
 */
function election_vote_rules_event_info() {
  $defaults = array(
    'group' => t('Election Vote'),
    'module' => 'election_vote',
    'access callback' => 'election_vote_rules_config_access',
  );
  $events = array();
  $events['election_vote_before_grant'] = $defaults + array(
    'label' => t('Before granting access to vote'),
    'variables' => array(
      'post' => array(
        'type' => 'election_post',
        'label' => t('The election post'),
        'skip save' => TRUE,
      ),
      'account' => array(
        'type' => 'user',
        'label' => t('The user who is attempting to vote'),
      ),
    ),
  );
  $events['election_vote_vote'] = $defaults + array(
    'label' => t('After casting a vote'),
    'variables' => array(
      'account' => array(
        'type' => 'user',
        'label' => t('The user who cast the vote'),
        'skip save' => TRUE,
      ),
      'post' => array(
        'type' => 'election_post',
        'label' => t('The post for which the vote was cast'),
        'skip save' => TRUE,
      ),
      'election' => array(
        'type' => 'election',
        'label' => t('The election to which the post belongs'),
        'skip save' => TRUE,
      ),
    ),
  );
  $events['election_vote_abstain'] = $defaults + array(
    'label' => t('After abstaining'),
    'variables' => array(
      'account' => array(
        'type' => 'user',
        'label' => t('The user who abstained'),
        'skip save' => TRUE,
      ),
      'post' => array(
        'type' => 'election_post',
        'label' => t('The post for which the user abstained'),
        'skip save' => TRUE,
      ),
      'election' => array(
        'type' => 'election',
        'label' => t('The election to which the post belongs'),
        'skip save' => TRUE,
      ),
    ),
  );
  $events['election_vote_undo'] = $defaults + array(
    'label' => t('After undoing a vote/abstention'),
    'variables' => array(
      'account' => array(
        'type' => 'user',
        'label' => t('The user who undid the vote/abstention'),
        'skip save' => TRUE,
      ),
      'post' => array(
        'type' => 'election_post',
        'label' => t('The election post'),
        'skip save' => TRUE,
      ),
      'election' => array(
        'type' => 'election',
        'label' => t('The election'),
        'skip save' => TRUE,
      ),
    ),
  );
  return $events;
}

/**
 * Implements hook_rules_action_info().
 */
function election_vote_rules_action_info() {

  $actions = array();

  $defaults = array(
    'group' => t('Election Vote'),
  );

  $actions['election_vote_deny_access'] = $defaults + array(
    'label' => t('Deny voting access'),
    'parameter' => array(
      'post' => array(
        'type' => 'election_post',
        'label' => t('The election post'),
      ),
      'user' => array(
        'type' => 'user',
        'label' => t('The user account'),
      ),
      'explanation' => array(
        'type' => 'text',
        'label' => t('Explanation'),
        'description' => t('An explanation to the user of why voting access is being denied.'),
      ),
    ),
  );

  $actions['election_vote_abstain_message'] = $defaults + array(
    'label' => t('Display a "You have abstained" message'),
    'parameter' => array(
      'post' => array(
        'type' => 'election_post',
        'label' => t('The election post'),
      ),
    ),
  );

  $actions['election_vote_vote_message'] = $defaults + array(
    'label' => t('Display a "Thank you for voting" message'),
    'parameter' => array(
      'post' => array(
        'type' => 'election_post',
        'label' => t('The election post'),
      ),
    ),
  );

  return $actions;
}

/**
 * Rules action: denies voting access during the current page request.
 *
 * This will only have an effect if it is called in reaction to the Rules event
 * 'Before granting access to vote', which is invoked in election_vote_access().
 *
 * @see election_vote_access()
 *
 * @param object $post
 *   The election post.
 * @param object $account
 *   The user account.
 * @param string $explanation
 *   An explanation to the user of why voting was denied.
 */
function election_vote_deny_access($post, $account, $explanation) {
  $grants = &drupal_static('election_vote_grants', array());
  if (!$account) {
    $account = $GLOBALS['user'];
  }
  $grants[$post->post_id][$account->uid]['vote'] = FALSE;
  $grants[$post->post_id][$account->uid]['undo'] = FALSE;
  $grants['explanations'][$post->post_id][$account->uid][] = array(
    'explanation' => filter_xss($explanation),
    'test' => FALSE,
  );
}

/**
 * Rules action: displays a "You have abstained" message.
 */
function election_vote_abstain_message($post) {
  drupal_set_message(
    t('You have abstained from voting for the @post <a href="!post_url">%post_title</a>.', array(
      '@post' => election_get_post_name($post->election->type),
      '!post_url' => url(election_post_uri_path($post)),
      '%post_title' => $post->title,
    ))
  );
}

/**
 * Rules action: displays a "Thank you for voting" message.
 */
function election_vote_vote_message($post) {
  drupal_set_message(
    t('Thank you for voting for the @post <a href="!post_url">%post_title</a>.', array(
      '@post' => election_get_post_name($post->election->type),
      '!post_url' => url(election_post_uri_path($post)),
      '%post_title' => $post->title,
    ))
  );
}

/**
 * Access callback for configuring Rules related to voting.
 *
 * @see election_vote_rules_event_info()
 */
function election_vote_rules_config_access($type, $name) {
  if ($type == 'event' || $type == 'condition') {
    return election_access('update') && entity_access('view', 'user');
  }
}
