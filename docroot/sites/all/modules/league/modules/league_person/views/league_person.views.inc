<?php

/**
 * @file
 * Providing extra functionality for the LeaguePerson UI via views.
 */


/**
 * Implements hook_views_data()
 */
function league_person_views_data_alter(&$data) { 
  $data['league_person']['link_league_person'] = array(
    'field' => array(
      'title' => t('Link'),
      'help' => t('Provide a link to the league_person.'),
      'handler' => 'league_person_handler_link_field',
    ),
  );
  $data['league_person']['edit_league_person'] = array(
    'field' => array(
      'title' => t('Edit Link'),
      'help' => t('Provide a link to the edit form for the league_person.'),
      'handler' => 'league_person_handler_edit_link_field',
    ),
  );
  $data['league_person']['delete_league_person'] = array(
    'field' => array(
      'title' => t('Delete Link'),
      'help' => t('Provide a link to delete the league_person.'),
      'handler' => 'league_person_handler_delete_link_field',
    ),
  );
  // This content of this field are decided based on the menu structure that
  // follows league_persons/league_person/%league_person_id/op
  $data['league_person']['operations'] = array(
    'field' => array(
      'title' => t('Operations links'),
      'help' => t('Display all operations available for this league_person.'),
      'handler' => 'league_person_handler_league_person_operations_field',
    ),
  );
}

