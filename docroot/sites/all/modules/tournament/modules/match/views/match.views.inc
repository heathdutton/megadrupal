<?php

/**
 * @file
 * Views interface for Match module.
 */

/**
 * Implements hook_views_data().
 */
function match_views_data() {
  $data['matches'] = array(
    'table' => array(
      'group' => t('Match'),
      'join' => array(
        'node' => array(
          'left_field' => 'nid',
          'field' => 'nid',
        ),
      ),
    ),
    'tid' => array(
      'title' => t('Tournament ID'),
      'help' => t('Match tournament nid.'),
      'argument' => array(
        'handler' => 'views_handler_argument_node_nid',
        'name field' => 'title', // the field to display in the summary.
        'numeric' => TRUE,
        'validate type' => 'nid',
      ),
    'relationship' => array(
        'base' => 'node',
        'base field' => 'nid',
        'label' => t('Match tournament node'),
        'handler' => 'views_handler_relationship',
      ),
    ),
    'nid' => array(
      'title' => t('Match node ID'),
      'help' => t('The match node ID.'),
      'field' => array(
        'handler' => 'views_handler_field_numeric',
      ),
    ),
    'entity_type' => array(
      'title' => t('Participant type'),
      'help' => t('The participant entity type of this match (e.g. user/team)'),
      'field' => array(
        'handler' => 'match_handler_field_entity_type',
      ),
      'filter' => array(
        'handler' => 'match_handler_filter_entity_type',
      ),
    ),
    'match_title' => array(
      'title' => t('Match title'),
      'help' => t('Use this title instead of Content titles to show match scores.'),
      'field' => array(
        'handler' => 'match_handler_field_match_title',
      ),
    ),
    'match_date' => array(
      'title' => t('Match date'),
      'help' => t('When the match is to be played.'),
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
    'match_status' => array(
      'title' => t('Match status'),
      'help' => t('The status of the match (e.g. awaiting result, closed etc).'),
      'field' => array(
        'handler' => 'match_handler_field_match_status',
      ),
      'sort' => array(
        'handler' => 'views_handler_sort',
      ),
      'filter' => array(
        'handler' => 'match_handler_filter_match_status',
      ),
    ),
    'actions' => array(
      'title' => t('Match actions'),
      'real field' => 'match_status',
      'help' => t('A list of match action links (e.g. enter result, accept result etc).'),
      'field' => array(
        'handler' => 'match_handler_field_match_actions',
      ),
    ),
  );

  $data['matches_participants'] = array(
    'table' => array(
      'group' => t('Match'),
      'base' => array(
        'field' => 'nid',
        'title' => t('Match participants'),
        'help' => t('Participants of a Match (e.g. users/teams).'),
      ),
      'join' => array(
        'node' => array(
          'left_field' => 'nid',
          'field' => 'nid',
        ),
      ),
    ),
    'user_id' => array(
      'title' => t('Participant ID (User)'),
      'help' => t('Relates the user table to match participants through the entity ID.'),
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
    'participant' => array(
      'title' => t('Participant'),
      'help' => t('Relates the user to match participants through any entity they are a member of.'),
      'real field' => 'id',
      'relationship' => array(
        'handler' => 'views_handler_relationship',
        'base' => 'users',
        'base field' => 'uid',
        'label' => t('Participant'),
      ),
      'argument' => array(
        'handler' => 'match_handler_argument_user_uid',
        'name field' => 'name', // display this field in the summary
      ),
    ),
    'score' => array(
      'title' => t('Match score'),
      'help' => t('The score of the participant.'),
      'field' => array(
        'handler' => 'views_handler_field_numeric',
      ),
      'filter' => array(
        'handler' => 'views_handler_filter_numeric',
      ),
    ),
  );

  return $data;
}
