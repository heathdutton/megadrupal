<?php

/**
 * @file
 * Views interface for Team module.
 */

/**
 * Implements hook_views_data().
 */
function team_views_data() {
  $data['teams'] = array(
    'table' => array(
      'group' => t('Team'),
      'base' => array(
        'field' => 'tid',
        'title' => t('Teams'),
        'help' => t('Groups of users that can participate in Tournaments.'),
      ),
      'join' => array(
        'tournament_participants' => array(
          'left_field' => 'id',
          'field' => 'tid',
        ),
      ),
      'entity type' => 'team',
    ),
    'tid' => array(
      'title' => t('Tid'),
      'help' => t('The team ID'),
      'field' => array(
        'handler' => 'team_handler_field_team',
        'click sortable' => TRUE,
      ),
      'argument' => array(
        'handler' => 'team_handler_argument_team_tid',
        // display this field in the summary
        'name field' => 'name',
      ),
      'filter' => array(
        'title' => t('Name'),
        'handler' => 'team_handler_filter_team_name',
      ),
      'sort' => array(
        'handler' => 'views_handler_sort',
      ),
    ),
    'name' => array(
      'title' => t('Name'),
      'help' => t('The team name.'),
      'field' => array(
        'handler' => 'team_handler_field_team_name',
        'click sortable' => TRUE,
      ),
      'sort' => array(
        'handler' => 'views_handler_sort',
      ),
      'argument' => array(
        'handler' => 'views_handler_argument_string',
      ),
      'filter' => array(
        'handler' => 'views_handler_filter_string',
        'title' => t('Name (raw)'),
        'help' => t('The team name. This filter does not check if the teams exists and allows partial matching. Does not utilize autocomplete.'),
      ),
    ),
    'status' => array(
      'title' => t('Status'),
      'help' => t('Whether or not the team is active.'),
      'field' => array(
        'handler' => 'views_handler_field_boolean',
        'click sortable' => TRUE,
        'output formats' => array(
          'active-notactive' => array(t('Active'), t('Not active')),
        ),
      ),
      'sort' => array(
        'handler' => 'views_handler_sort',
      ),
      'filter' => array(
        'handler' => 'views_handler_filter_boolean_operator',
        'label' => t('Active'),
        'type' => 'yes-no',
        // Use status = 1 instead of status <> 0 in WHERE statment
        'use equal' => TRUE,
      ),
    ),
    'created' => array(
      'title' => t('Created'),
      'help' => t('When the team was created.'),
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
    'changed' => array(
      'title' => t('Updated'),
      'help' => t('When the team was updated.'),
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
  );

  $data['teams_members'] = array(
    'table' => array(
      'group' => t('Team'),
      'join' => array(
        'tournament_participants' => array(
          'left_field' => 'id',
          'field' => 'tid',
        ),
      ),
      'join' => array(
        'teams' => array(
          'left_field' => 'tid',
          'field' => 'tid',
        ),
      ),
    ),
    'uid' => array(
      'title' => t('Team member UID'),
      'help' => t('The team member user ID'),
      'filter' => array(
        'handler' => 'views_handler_filter_numeric',
      ),
      'argument' => array(
        'handler' => 'views_handler_argument_numeric',
      ),
      'field' => array(
        'handler' => 'views_handler_field_numeric',
      ),
    ),
    'role' => array(
      'title' => t('Member role'),
      'help' => t('The role of the team member.'),
      'field' => array(
        'handler' => 'team_handler_field_member_role',
      ),
      'sort' => array(
        'handler' => 'views_handler_sort',
      ),
      'filter' => array(
        'handler' => 'team_handler_filter_member_role',
      ),
    ),
    'team' => array(
      'title' => t('Team'),
      'help' => t('Relates the teams table to the teams_members table'),
      'real field' => 'tid',
      'relationship' => array(
        'handler' => 'views_handler_relationship',
        'base' => 'teams',
        'base field' => 'tid',
        'label' => t('Team ID'),
      ),
    ),
    'user_id' => array(
      'title' => t('Team member user name'),
      'help' => t('Relates the user table to a teams members through the user ID.'),
      'real field' => 'uid',
      'relationship' => array(
        'handler' => 'views_handler_relationship',
        'base' => 'users',
        'base field' => 'uid',
        'label' => t('User ID'),
      ),
      'argument' => array(
        'handler' => 'views_handler_argument_user_uid',
        // display this field in the summary
        'name field' => 'name',
      ),
      'filter' => array(
        'title' => t('User name'),
        'handler' => 'views_handler_filter_user_name',
      ),
    ),
  );

  // Current teams
  $data['teams']['tid_current'] = array(
    'real field' => 'tid',
    'title' => t('Current teams'),
    'help' => t('Filter the view to the teams of the currently logged in user.'),
    'filter' => array(
      'handler' => 'team_handler_filter_teams_current',
      'type' => 'yes-no',
    ),
  );

  return $data;
}

/**
 * Implements hook_views_plugins().
 */
function team_views_plugins() {
  return array(
    'row' => array(
      'team' => array(
        'title' => t('Team'),
        'help' => t('Display the team with standard team view.'),
        'handler' => 'team_plugin_row_team_view',
        'base' => array('teams'),
        'uses options' => TRUE,
        'type' => 'normal',
        'help topic' => 'style-teams',
      ),
    ),
    'argument default' => array(
      'team' => array(
        'title' => t('Team ID from URL'),
        'handler' => 'team_plugin_argument_default_team',
      ),
    ),
    'argument validator' => array(
      'team' => array(
        'title' => t('Team'),
        'handler' => 'team_plugin_argument_validate_user',
      ),
    ),
  );
}

/**
 * Implements hook_views_data_alter().
 */
function team_views_data_alter(&$data) {
  $data['users']['teams_members'] = array(
    'title' => t('Team related to user'),
    'help' => t('Relates the team_members table to a user.'),
    'real field' => 'uid',
    'relationship' => array(
      'handler' => 'views_handler_relationship',
      'base' => 'teams_members',
      'base field' => 'uid',
      'label' => t('Team ID'),
    ),
  );
}
