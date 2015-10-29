<?php

/**
 * @file
 * Views interface for Tournament module.
 */

/**
 * Implements hook_views_data().
 */
function tournament_views_data() {
  $data['tournament'] = array(
    'table' => array(
      'group' => t('Tournament'),
      'join' => array(
        'node' => array(
          'left_field' => 'nid',
          'field' => 'nid',
        ),
      ),
    ),
    'nid' => array(
      'title' => t('Tournament participant'),
      'help' => t('Relate the tournament participants table to the tournament table.'),
      'relationship' => array(
          'base' => 'tournament_participants',
          'field' => 'nid',
          'label' => t('Tournament participants'),
          'handler' => 'views_handler_relationship',
      ),
    ),
    'entity_type' => array(
      'title' => t('Participant type'),
      'help' => t('The participant entity type of this tournament (e.g. user/team)'),
      'field' => array(
        'handler' => 'tournament_handler_field_entity_type',
      ),
      'filter' => array(
        'handler' => 'tournament_handler_filter_entity_type',
      ),
    ),
    'size' => array(
      'title' => t('Size'),
      'help' => t('Maximum number of participants that can sign up.'),
      'field' => array(
        'handler' => 'views_handler_field_numeric',
      ),
    ),
    'start' => array(
      'title' => t('Start date'),
      'help' => t('When the tournament should start.'),
      'field' => array(
        'handler' => 'views_handler_field_date',
        'click sortable' => TRUE,
      ),
      'sort' => array(
        'handler' => 'views_handler_sort_date',
      ),
      'filter' => array(
        'handler' => 'views_handler_filter_date',
      ),
    ),
    'tournament_status' => array(
      'title' => t('Tournament status'),
      'help' => t('The status of the tournament (Active, Started or Finished).'),
      'field' => array(
        'handler' => 'tournament_handler_field_tournament_status',
      ),
      'filter' => array(
        'handler' => 'tournament_handler_filter_tournament_status',
      ),
    ),
  );

  $data['tournament_participants'] = array(
    'table' => array(
      'group' => t('Tournament'),
      'base' => array(
        'field' => 'nid',
        'title' => t('Tournament participants'),
        'help' => t('Participants of a Tournament.'),
      ),
    ),
    'id' => array(
      'title' => t('Participant ID'),
      'help' => t('The participant entity ID.'),
      'field' => array(
        'title' => t('Participant ID'),
        'handler' => 'views_handler_field_numeric',
      ),
      'filter' => array(
        'title' => t('Participant ID'),
        'handler' => 'views_handler_filter_numeric',
      ),
    ),
    'user_id' => array(
      'title' => t('Participant ID (User)'),
      'help' => t('Relates the user table to tournament participants through the user ID.'),
      'real field' => 'id',
      'relationship' => array(
        'handler' => 'views_handler_relationship',
        'base' => 'users',
        'base field' => 'uid',
        'label' => t('Participant ID'),
      ),
      'argument' => array(
        'handler' => 'views_handler_argument_user_uid',
        'name field' => 'name', // display this field in the summary
      ),
      'filter' => array(
        'title' => t('User name'),
        'handler' => 'views_handler_filter_user_name',
      ),
    ),
    'team_id' => array(
      'title' => t('Participant ID (Team)'),
      'help' => t('Relates the team table to match participants through the entity ID.'),
      'real field' => 'id',
      'relationship' => array(
        'handler' => 'views_handler_relationship',
        'base' => 'teams',
        'base field' => 'tid',
        'label' => t('Participant ID'),
      ),
      'argument' => array(
        'handler' => 'team_handler_argument_team_tid',
        'name field' => 'name', // display this field in the summary
      ),
      'filter' => array(
        'title' => t('Name'),
        'handler' => 'team_handler_filter_team_name',
      ),
    ),
    'status' => array(
      'title' => t('Participant status'),
      'help' => t('The status of the participant (Active or Not active).'),
      'field' => array(
        'handler' => 'tournament_handler_field_tournament_participants_status',
      ),
      'filter' => array(
        'handler' => 'tournament_handler_filter_tournament_participants_status',
      ),
    ),
    'rank' => array(
      'title' => t('Participant rank'),
      'help' => t('The final rank of the participant (Only for finished tournaments).'),
      'field' => array(
        'handler' => 'tournament_handler_field_tournament_participants_rank',
      ),
    ),
  );

  // Current participants
  $data['tournament_participants']['id_current'] = array(
    'real field' => 'id',
    'title' => t('My Participant IDs'),
    'help' => t('Filter the view to the entities that the currently logged in user has access to.'),
    'filter' => array(
      'handler' => 'tournament_handler_filter_tournament_participants_current',
      'type' => 'yes-no',
    ),
  );
  
  $data['tournament_participants']['id_uid'] = array(
    'real field' => 'id',
    'title' => t('Participant user ID'),
    'help' => t('Filter the view to the entities that the given user has access to.'),
    'argument' => array(
      'handler' => 'tournament_handler_argument_tournament_participants_uid',
      'name field' => 'id',
    ),
  );  

  $data['tournament_admins'] = array(
    'table' => array(
      'group' => t('Tournament'),
      'base' => array(
        'field' => 'nid',
        'title' => t('Tournament admins'),
        'help' => t('Administrator users of a Tournament.'),
      ),
      'join' => array(
        'node' => array(
          'left_field' => 'nid',
          'field' => 'nid',
        ),
      ),
    ),
    'nid' => array(
      'title' => t('Tournament nid'),
      'help' => t('The tournament node ID.'), // The help that appears on the UI,
      'field' => array(
        'handler' => 'views_handler_field_numeric',
        'click sortable' => TRUE,
      ),
      // Information for accepting a nid as an argument
      'argument' => array(
        'handler' => 'views_handler_argument_node_nid',
        'name field' => 'title', // the field to display in the summary.
        'numeric' => TRUE,
        'validate type' => 'nid',
      ),
      // Information for accepting a nid as a filter
      'filter' => array(
        'handler' => 'views_handler_filter_numeric',
      ),
      // Information for sorting on a nid.
      'sort' => array(
        'handler' => 'views_handler_sort',
      ),
      'relationship' => array(
        'handler' => 'views_handler_relationship',
        'base' => 'node',
        'base field' => 'nid',
        'label' => t('Tournament node'),
      ),
    ),
    'uid' => array(
      'title' => t('Admin user ID'),
      'help' => t('Relates the user table to tournament admins through the user ID.'),
      'relationship' => array(
        'handler' => 'views_handler_relationship',
        'base' => 'users',
        'base field' => 'uid',
        'label' => t('Admin user ID'),
      ),
    ),
  );

  return $data;
}
