<?php

/**
 * @file
 * Providing extra functionality for the LeaguePerson UI via views.
 */

/**
 * Implements hook_views_default_views().
 */
function league_person_views_default_views() {
$view = new view();
$view->name = 'league_persons';
$view->description = 'An administrative list of League Persons';
$view->tag = 'league';
$view->base_table = 'league_person';
$view->human_name = 'League Persons';
$view->core = 7;
$view->api_version = '3.0';
$view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

/* Display: Master */
$handler = $view->new_display('default', 'Master', 'default');
$handler->display->display_options['title'] = 'LeaguePersons';
$handler->display->display_options['use_more_always'] = FALSE;
$handler->display->display_options['access']['type'] = 'perm';
$handler->display->display_options['access']['perm'] = 'create any league_person type';
$handler->display->display_options['cache']['type'] = 'none';
$handler->display->display_options['query']['type'] = 'views_query';
$handler->display->display_options['exposed_form']['type'] = 'basic';
$handler->display->display_options['pager']['type'] = 'full';
$handler->display->display_options['pager']['options']['items_per_page'] = '10';
$handler->display->display_options['style_plugin'] = 'table';
$handler->display->display_options['style_options']['columns'] = array(
  'name' => 'name',
  'league_person_id' => 'league_person_id',
);
$handler->display->display_options['style_options']['default'] = '-1';
$handler->display->display_options['style_options']['info'] = array(
  'name' => array(
    'sortable' => 0,
    'default_sort_order' => 'asc',
    'align' => '',
    'separator' => '',
  ),
  'league_person_id' => array(
    'sortable' => 0,
    'default_sort_order' => 'asc',
    'align' => '',
    'separator' => '',
  ),
);
/* No results behavior: Global: Text area */
$handler->display->display_options['empty']['area']['id'] = 'area';
$handler->display->display_options['empty']['area']['table'] = 'views';
$handler->display->display_options['empty']['area']['field'] = 'area';
$handler->display->display_options['empty']['area']['label'] = 'Empty ';
$handler->display->display_options['empty']['area']['content'] = 'No league_persons have been created yet';
/* Field: LeaguePerson: Leagueperson ID */
$handler->display->display_options['fields']['league_person_id']['id'] = 'league_person_id';
$handler->display->display_options['fields']['league_person_id']['table'] = 'league_person';
$handler->display->display_options['fields']['league_person_id']['field'] = 'league_person_id';
$handler->display->display_options['fields']['league_person_id']['label'] = 'ID';
/* Field: LeaguePerson: Type */
$handler->display->display_options['fields']['type']['id'] = 'type';
$handler->display->display_options['fields']['type']['table'] = 'league_person';
$handler->display->display_options['fields']['type']['field'] = 'type';
/* Field: LeaguePerson: Name */
$handler->display->display_options['fields']['name']['id'] = 'name';
$handler->display->display_options['fields']['name']['table'] = 'league_person';
$handler->display->display_options['fields']['name']['field'] = 'name';
/* Field: LeaguePerson: Team */
$handler->display->display_options['fields']['team']['id'] = 'team';
$handler->display->display_options['fields']['team']['table'] = 'league_person';
$handler->display->display_options['fields']['team']['field'] = 'team';
/* Field: LeaguePerson: Link */
$handler->display->display_options['fields']['link_league_person']['id'] = 'link_league_person';
$handler->display->display_options['fields']['link_league_person']['table'] = 'league_person';
$handler->display->display_options['fields']['link_league_person']['field'] = 'link_league_person';
$handler->display->display_options['fields']['link_league_person']['label'] = 'View';
/* Field: LeaguePerson: Edit Link */
$handler->display->display_options['fields']['edit_league_person']['id'] = 'edit_league_person';
$handler->display->display_options['fields']['edit_league_person']['table'] = 'league_person';
$handler->display->display_options['fields']['edit_league_person']['field'] = 'edit_league_person';
$handler->display->display_options['fields']['edit_league_person']['label'] = 'Edit';
/* Field: LeaguePerson: Delete Link */
$handler->display->display_options['fields']['delete_league_person']['id'] = 'delete_league_person';
$handler->display->display_options['fields']['delete_league_person']['table'] = 'league_person';
$handler->display->display_options['fields']['delete_league_person']['field'] = 'delete_league_person';
$handler->display->display_options['fields']['delete_league_person']['label'] = 'Delete';
/* Filter criterion: LeaguePerson: Name */
$handler->display->display_options['filters']['name']['id'] = 'name';
$handler->display->display_options['filters']['name']['table'] = 'league_person';
$handler->display->display_options['filters']['name']['field'] = 'name';
$handler->display->display_options['filters']['name']['group'] = 1;
$handler->display->display_options['filters']['name']['exposed'] = TRUE;
$handler->display->display_options['filters']['name']['expose']['operator_id'] = 'name_op';
$handler->display->display_options['filters']['name']['expose']['label'] = 'Name';
$handler->display->display_options['filters']['name']['expose']['operator'] = 'name_op';
$handler->display->display_options['filters']['name']['expose']['identifier'] = 'name';
$handler->display->display_options['filters']['name']['expose']['remember_roles'] = array(
  2 => '2',
  1 => 0,
  3 => 0,
);
/* Filter criterion: LeaguePerson: Type */
$handler->display->display_options['filters']['type']['id'] = 'type';
$handler->display->display_options['filters']['type']['table'] = 'league_person';
$handler->display->display_options['filters']['type']['field'] = 'type';
$handler->display->display_options['filters']['type']['group'] = 1;
$handler->display->display_options['filters']['type']['exposed'] = TRUE;
$handler->display->display_options['filters']['type']['expose']['operator_id'] = 'type_op';
$handler->display->display_options['filters']['type']['expose']['label'] = 'Type';
$handler->display->display_options['filters']['type']['expose']['operator'] = 'type_op';
$handler->display->display_options['filters']['type']['expose']['identifier'] = 'type';
$handler->display->display_options['filters']['type']['expose']['remember_roles'] = array(
  2 => '2',
  1 => 0,
  3 => 0,
);
/* Filter criterion: LeaguePerson: Team */
$handler->display->display_options['filters']['team']['id'] = 'team';
$handler->display->display_options['filters']['team']['table'] = 'league_person';
$handler->display->display_options['filters']['team']['field'] = 'team';
$handler->display->display_options['filters']['team']['exposed'] = TRUE;
$handler->display->display_options['filters']['team']['expose']['operator_id'] = 'team_op';
$handler->display->display_options['filters']['team']['expose']['label'] = 'Team';
$handler->display->display_options['filters']['team']['expose']['operator'] = 'team_op';
$handler->display->display_options['filters']['team']['expose']['identifier'] = 'team';
$handler->display->display_options['filters']['team']['expose']['remember_roles'] = array(
  2 => '2',
  1 => 0,
  3 => 0,
);

/* Display: Page */
$handler = $view->new_display('page', 'Page', 'league_persons_admin_page');
$handler->display->display_options['path'] = 'admin/content/league_persons/list';
$handler->display->display_options['menu']['type'] = 'default tab';
$handler->display->display_options['menu']['title'] = 'List';
$handler->display->display_options['menu']['weight'] = '-10';
$handler->display->display_options['tab_options']['type'] = 'tab';
$handler->display->display_options['tab_options']['title'] = 'LeaguePersons';
$handler->display->display_options['tab_options']['description'] = 'Manage league_persons';
$handler->display->display_options['tab_options']['weight'] = '0';
$handler->display->display_options['tab_options']['name'] = 'management';

  $views[$view->name] = $view;
  return $views;

}
