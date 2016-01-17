<?php
/**
 * @file
 * Views integration for the Election Post module.
 */

/**
 * Implements hook_views_data().
 */
function election_post_views_data() {

  $data = array();

  // Join posts to elections.
  $data['election']['table']['join']['election_post'] = array(
    'left_field' => 'election_id',
    'field' => 'election_id',
  );

  $data['election_post']['published_or_admin'] = array(
    'title' => t('Published or admin'),
    'help' => t('Filters out unpublished posts if the current user may not view them.'),
    'filter' => array(
      'field' => 'published',
      'handler' => 'election_post_views_handler_filter_published',
    ),
  );

  $data['election_post']['vstatus'] = array(
    'title' => t('Voting status'),
    'help' => t('The voting status for this election post.'),
    'real field' => 'vstatus_inheritance',
    'filter' => array(
      'label' => t('Voting is open'),
      'handler' => 'election_post_views_handler_filter_vstatus',
      'type' => 'yes-no',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  $data['election_post']['view_link'] = array(
    'field' => array(
      'title' => t('Link'),
      'help' => t('Provide a simple link to the post.'),
      'handler' => 'election_post_views_handler_field_link',
    ),
  );

  $data['election_post']['edit_link'] = array(
    'field' => array(
      'title' => t('Edit link'),
      'help' => t('A link to edit the post.'),
      'handler' => 'election_post_views_handler_field_link_edit',
    ),
  );

  return $data;

}
