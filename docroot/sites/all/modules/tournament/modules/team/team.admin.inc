<?php

/**
 * @file
 * Admin page callback file for the team module.
 */

/**
 * Form builder; Configure team settings for this site.
 *
 * @ingroup forms
 * @see system_settings_form()
 */
function team_admin_settings() {
  $form = array();

  $form['team_list_limit'] = array(
    '#type' => 'select',
    '#title' => t('Number of teams in lists'),
    '#description' => t('The maximum number of teams displayed on list pages such as <em>search/team/*</em>.'),
    '#options' => drupal_map_assoc(array(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 15, 20, 25, 30)),
    '#default_value' => variable_get('team_list_limit', 10),
  );
  $form['team_name_max'] = array(
    '#type' => 'textfield',
    '#title' => t('Maximum name length'),
    '#description' => t('The maximum character length allowed for team name field. The maximum value for this field is 64.'),
    '#default_value' => variable_get('team_name_max', 64),
    '#element_validate' => array('element_validate_integer'),
  );
  $form['team_joining'] = array(
    '#type' => 'checkbox',
    '#title' => t('Joining'),
    '#description' => t('By checking this box team administrators will be able to control if users can apply to join their team.'),
    '#default_value' => variable_get('team_joining', 1),
  );
  $form['team_challenging'] = array(
    '#type' => 'checkbox',
    '#title' => t('Challenging'),
    '#description' => t('By checking this box team administrators will be able to control if other teams can directly challenge their team to practice matches.'),
    '#default_value' => variable_get('team_challenging', 1),
  );
  $form['team_alias'] = array(
    '#type' => 'checkbox',
    '#title' => t('Team Aliases'),
    '#description' => t('By checking this box team administrators will be able to set a custom URL alias to their team profile page.'),
    '#default_value' => variable_get('team_alias', 1),
  );
  $form['team_user_picture'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show user pictures for team members'),
    '#default_value' => variable_get('team_user_picture', 0),
  );
  $form['team_user_list'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show joined teams on user profiles'),
    '#default_value' => variable_get('team_user_list', 1),
  );
  $form['team_multiple_signup'] = array(
    '#type' => 'checkbox',
    '#title' => t('Display an option to tournament admins to toggle multiple team signup access.'),
    '#default_value' => variable_get('team_multiple_signup', 1),
  );
  $form['team_tournament_size_max'] = array(
    '#type' => 'select',
    '#title' => t('Maxmium team size selectable when creating team tournaments'),
    '#description' => t('When creating a team tournament, the admin must pick the size of teams that will compete. This setting lets you decide the largest team size they can choose from.'),
    '#options' => array_merge(drupal_map_assoc(range(2, 50))),
    '#default_value' => variable_get('team_tournament_size_max', 8),
  );

  return system_settings_form($form);
}

/**
 * Form validate function for team_admin_settings().
 */
function team_admin_settings_validate($form, &$form_state) {
  if ($form_state['values']['team_name_max'] > 64) {
    form_set_error('team_name_max', t('Due to database restrictions, the maximum team name length is 64 characters.'));
  }
}
