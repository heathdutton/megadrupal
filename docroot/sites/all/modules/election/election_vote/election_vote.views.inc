<?php
/**
 * @file
 * Views integration for the Election Vote module.
 */

/**
 * Implements hook_views_data().
 */
function election_vote_views_data() {

  $data = array();

  $data['election_post']['vote_link'] = array(
    'field' => array(
      'title' => t('Vote link'),
      'help' => t('Provide a link to cast a vote for this post.'),
      'handler' => 'election_vote_views_handler_field_vote_link',
    ),
  );

  $data['election_post']['eligibility'] = array(
    'title' => t('Eligibility'),
    'help' => t('A detailed description of whether the current user is eligible to vote.'),
    'real field' => 'post_id',
    'field' => array(
      'handler' => 'election_vote_views_handler_field_eligibility',
      'click sortable' => FALSE,
    ),
  );

  $data['election_post']['eligibility_boolean'] = array(
    'title' => t('Eligible to vote?'),
    'help' => t('A Boolean (yes/no) for whether the current user is eligible to vote.'),
    'real field' => 'post_id',
    'field' => array(
      'handler' => 'election_vote_views_handler_field_eligibility_boolean',
      'click sortable' => TRUE,
    ),
  );

  return $data;

}
