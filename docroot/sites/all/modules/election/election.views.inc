<?php
/**
 * @file
 * Views integration for the Election module.
 */

/**
 * Implements hook_views_query_substitutions().
 */
function election_views_query_substitutions() {
  return array(
    '***EDIT_ANY_ELECTION***' => intval(user_access('edit any election')),
  );
}

/**
 * Implements hook_views_data().
 */
function election_views_data() {

  $data = array();

  $data['election']['published_or_admin'] = array(
    'title' => t('Published or admin'),
    'help' => t('Filters out unpublished content if the current user cannot view it.'),
    'filter' => array(
      'field' => 'status',
      'handler' => 'election_views_handler_filter_published',
      'label' => t('Published or admin'),
    ),
  );

  $data['election']['view_election'] = array(
    'field' => array(
      'title' => t('Link'),
      'help' => t('A link to the election.'),
      'handler' => 'election_views_handler_field_link',
    ),
  );

  $data['election']['edit_election'] = array(
    'field' => array(
      'title' => t('Edit link'),
      'help' => t('A link to edit the election.'),
      'handler' => 'election_views_handler_field_link_edit',
    ),
  );

  $data['election']['delete_election'] = array(
    'field' => array(
      'title' => t('Delete link'),
      'help' => t('A link to delete the election.'),
      'handler' => 'election_views_handler_field_link_delete',
    ),
  );

  return $data;

}
