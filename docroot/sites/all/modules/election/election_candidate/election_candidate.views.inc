<?php
/**
 * @file
 * Views integration for the Election Candidate module.
 */

/**
 * Implements hook_views_data().
 */
function election_candidate_views_data() {

  $data = array();

  // Join candidates to elections.
  $data['election']['table']['join']['election_candidate'] = array(
    'left_field' => 'election_id',
    'field' => 'election_id',
  );

  // Join candidates to posts (positions).
  $data['election_post']['table']['join']['election_candidate'] = array(
    'left_field' => 'post_id',
    'field' => 'post_id',
  );

  $data['election_candidate']['published_or_admin'] = array(
    'title' => t('Published or admin'),
    'help' => t('Filters out unpublished candidates if the current user may not view them.'),
    'filter' => array(
      'field' => 'published',
      'handler' => 'election_candidate_views_handler_filter_published',
    ),
  );

  $data['election_candidate']['view_link'] = array(
    'field' => array(
      'title' => t('Profile link'),
      'help' => t("Provide a simple link to the candidate's profile page."),
      'handler' => 'election_candidate_views_handler_field_link',
    ),
  );

  $data['election_candidate']['edit_link'] = array(
    'field' => array(
      'title' => t('Edit link'),
      'help' => t('A link to edit the candidate.'),
      'handler' => 'election_candidate_views_handler_field_link_edit',
    ),
  );

  $data['election_post']['nominate_link'] = array(
    'field' => array(
      'title' => t('Nomination form link'),
      'help' => t('Link to the nomination form.'),
      'handler' => 'election_candidate_views_handler_field_link_nominate',
      'real field' => 'post_id',
    ),
  );

  return $data;

}
