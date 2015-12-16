<?php
/**
 * @file
 * comstack_friends.views_default.inc
 */

/**
 * Implements hook_views_default_views().
 */
function comstack_friends_views_default_views() {
  $export = array();

  $view = new view();
  $view->name = 'comstack_friends';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'user_relationships';
  $view->human_name = 'Comstack Friends';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Friends';
  $handler->display->display_options['css_class'] = 'friends-list';
  $handler->display->display_options['use_ajax'] = TRUE;
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'comstack_friends_current_perm';
  $handler->display->display_options['access']['perm'] = 'can have comstack_friends relationships';
  $handler->display->display_options['access']['bypass_perm'] = 'access other users comstack_friends';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '10';
  $handler->display->display_options['style_plugin'] = 'default';
  $handler->display->display_options['row_plugin'] = 'fields';
  $handler->display->display_options['row_options']['default_field_elements'] = FALSE;
  /* No results behavior: Global: Unfiltered text */
  $handler->display->display_options['empty']['area_text_custom']['id'] = 'area_text_custom';
  $handler->display->display_options['empty']['area_text_custom']['table'] = 'views';
  $handler->display->display_options['empty']['area_text_custom']['field'] = 'area_text_custom';
  $handler->display->display_options['empty']['area_text_custom']['empty'] = TRUE;
  $handler->display->display_options['empty']['area_text_custom']['content'] = '<p class="text-center">You haven\'t added any friends yet.</p>';
  /* Relationship: User relationships: Requestee user */
  $handler->display->display_options['relationships']['requestee_id']['id'] = 'requestee_id';
  $handler->display->display_options['relationships']['requestee_id']['table'] = 'user_relationships';
  $handler->display->display_options['relationships']['requestee_id']['field'] = 'requestee_id';
  /* Relationship: User relationships: Requester user */
  $handler->display->display_options['relationships']['requester_id']['id'] = 'requester_id';
  $handler->display->display_options['relationships']['requester_id']['table'] = 'user_relationships';
  $handler->display->display_options['relationships']['requester_id']['field'] = 'requester_id';
  /* Relationship: User relationships: RTID */
  $handler->display->display_options['relationships']['rtid']['id'] = 'rtid';
  $handler->display->display_options['relationships']['rtid']['table'] = 'user_relationships';
  $handler->display->display_options['relationships']['rtid']['field'] = 'rtid';
  $handler->display->display_options['relationships']['rtid']['required'] = TRUE;
  /* Field: User: Name */
  $handler->display->display_options['fields']['name']['id'] = 'name';
  $handler->display->display_options['fields']['name']['table'] = 'users';
  $handler->display->display_options['fields']['name']['field'] = 'name';
  $handler->display->display_options['fields']['name']['relationship'] = 'requestee_id';
  /* Field: User: Picture */
  $handler->display->display_options['fields']['picture']['id'] = 'picture';
  $handler->display->display_options['fields']['picture']['table'] = 'users';
  $handler->display->display_options['fields']['picture']['field'] = 'picture';
  $handler->display->display_options['fields']['picture']['relationship'] = 'requestee_id';
  $handler->display->display_options['fields']['picture']['label'] = '';
  $handler->display->display_options['fields']['picture']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['picture']['image_style'] = 'comstack-200-200';
  /* Field: User relationships: Comstack Friends user relationship friend action link */
  $handler->display->display_options['fields']['comstack_friends_link_friend']['id'] = 'comstack_friends_link_friend';
  $handler->display->display_options['fields']['comstack_friends_link_friend']['table'] = 'users';
  $handler->display->display_options['fields']['comstack_friends_link_friend']['field'] = 'comstack_friends_link_friend';
  $handler->display->display_options['fields']['comstack_friends_link_friend']['relationship'] = 'requestee_id';
  $handler->display->display_options['fields']['comstack_friends_link_friend']['label'] = '';
  $handler->display->display_options['fields']['comstack_friends_link_friend']['element_label_colon'] = FALSE;
  /* Field: User relationships: Comstack Friends user relationship block action link */
  $handler->display->display_options['fields']['comstack_friends_link_block']['id'] = 'comstack_friends_link_block';
  $handler->display->display_options['fields']['comstack_friends_link_block']['table'] = 'users';
  $handler->display->display_options['fields']['comstack_friends_link_block']['field'] = 'comstack_friends_link_block';
  $handler->display->display_options['fields']['comstack_friends_link_block']['relationship'] = 'requestee_id';
  $handler->display->display_options['fields']['comstack_friends_link_block']['label'] = '';
  $handler->display->display_options['fields']['comstack_friends_link_block']['element_label_colon'] = FALSE;
  /* Sort criterion: User: Name */
  $handler->display->display_options['sorts']['name']['id'] = 'name';
  $handler->display->display_options['sorts']['name']['table'] = 'users';
  $handler->display->display_options['sorts']['name']['field'] = 'name';
  $handler->display->display_options['sorts']['name']['relationship'] = 'requestee_id';
  /* Contextual filter: User relationships: Requester user */
  $handler->display->display_options['arguments']['requester_id']['id'] = 'requester_id';
  $handler->display->display_options['arguments']['requester_id']['table'] = 'user_relationships';
  $handler->display->display_options['arguments']['requester_id']['field'] = 'requester_id';
  $handler->display->display_options['arguments']['requester_id']['default_action'] = 'default';
  $handler->display->display_options['arguments']['requester_id']['default_argument_type'] = 'user';
  $handler->display->display_options['arguments']['requester_id']['default_argument_options']['user'] = FALSE;
  $handler->display->display_options['arguments']['requester_id']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['requester_id']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['requester_id']['summary_options']['items_per_page'] = '25';
  $handler->display->display_options['arguments']['requester_id']['specify_validation'] = TRUE;
  $handler->display->display_options['arguments']['requester_id']['validate']['type'] = 'user';
  $handler->display->display_options['arguments']['requester_id']['validate_options']['type'] = 'either';
  /* Filter criterion: User relationships: Relationship status */
  $handler->display->display_options['filters']['approved']['id'] = 'approved';
  $handler->display->display_options['filters']['approved']['table'] = 'user_relationships';
  $handler->display->display_options['filters']['approved']['field'] = 'approved';
  $handler->display->display_options['filters']['approved']['value'] = '1';
  /* Filter criterion: User: Active */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'users';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['relationship'] = 'requestee_id';
  $handler->display->display_options['filters']['status']['value'] = '1';
  /* Filter criterion: User relationship types: Machine name */
  $handler->display->display_options['filters']['machine_name']['id'] = 'machine_name';
  $handler->display->display_options['filters']['machine_name']['table'] = 'user_relationship_types';
  $handler->display->display_options['filters']['machine_name']['field'] = 'machine_name';
  $handler->display->display_options['filters']['machine_name']['relationship'] = 'rtid';
  $handler->display->display_options['filters']['machine_name']['value'] = array(
    'comstack_friends' => 'comstack_friends',
  );

  /* Display: Friends */
  $handler = $view->new_display('page', 'Friends', 'page');
  $handler->display->display_options['defaults']['access'] = FALSE;
  $handler->display->display_options['access']['type'] = 'comstack_friends_current_perm';
  $handler->display->display_options['access']['perm'] = 'can have comstack_friends relationships';
  $handler->display->display_options['access']['bypass_perm'] = 'access other users comstack_friends';
  $handler->display->display_options['path'] = 'friends/%/friends';
  $handler->display->display_options['menu']['type'] = 'default tab';
  $handler->display->display_options['menu']['title'] = 'Friends';
  $handler->display->display_options['menu']['weight'] = '1';
  $handler->display->display_options['menu']['context'] = 0;
  $handler->display->display_options['menu']['context_only_inline'] = 0;
  $handler->display->display_options['tab_options']['type'] = 'normal';
  $handler->display->display_options['tab_options']['title'] = 'Friends';
  $handler->display->display_options['tab_options']['weight'] = '0';

  /* Display: Pending */
  $handler = $view->new_display('page', 'Pending', 'page_1');
  $handler->display->display_options['defaults']['empty'] = FALSE;
  /* No results behavior: Global: Unfiltered text */
  $handler->display->display_options['empty']['area_text_custom']['id'] = 'area_text_custom';
  $handler->display->display_options['empty']['area_text_custom']['table'] = 'views';
  $handler->display->display_options['empty']['area_text_custom']['field'] = 'area_text_custom';
  $handler->display->display_options['empty']['area_text_custom']['empty'] = TRUE;
  $handler->display->display_options['empty']['area_text_custom']['content'] = '<p class="text-center">You have no pending friend requests.</p>';
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: User: Name */
  $handler->display->display_options['fields']['name']['id'] = 'name';
  $handler->display->display_options['fields']['name']['table'] = 'users';
  $handler->display->display_options['fields']['name']['field'] = 'name';
  $handler->display->display_options['fields']['name']['relationship'] = 'requester_id';
  /* Field: User: Picture */
  $handler->display->display_options['fields']['picture']['id'] = 'picture';
  $handler->display->display_options['fields']['picture']['table'] = 'users';
  $handler->display->display_options['fields']['picture']['field'] = 'picture';
  $handler->display->display_options['fields']['picture']['relationship'] = 'requester_id';
  $handler->display->display_options['fields']['picture']['label'] = '';
  $handler->display->display_options['fields']['picture']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['picture']['image_style'] = 'comstack-200-200';
  /* Field: User relationships: Comstack Friends user relationship friend action link */
  $handler->display->display_options['fields']['comstack_friends_link_friend']['id'] = 'comstack_friends_link_friend';
  $handler->display->display_options['fields']['comstack_friends_link_friend']['table'] = 'users';
  $handler->display->display_options['fields']['comstack_friends_link_friend']['field'] = 'comstack_friends_link_friend';
  $handler->display->display_options['fields']['comstack_friends_link_friend']['relationship'] = 'requester_id';
  $handler->display->display_options['fields']['comstack_friends_link_friend']['label'] = '';
  $handler->display->display_options['fields']['comstack_friends_link_friend']['element_label_colon'] = FALSE;
  /* Field: User relationships: Comstack Friends user relationship block action link */
  $handler->display->display_options['fields']['comstack_friends_link_block']['id'] = 'comstack_friends_link_block';
  $handler->display->display_options['fields']['comstack_friends_link_block']['table'] = 'users';
  $handler->display->display_options['fields']['comstack_friends_link_block']['field'] = 'comstack_friends_link_block';
  $handler->display->display_options['fields']['comstack_friends_link_block']['relationship'] = 'requester_id';
  $handler->display->display_options['fields']['comstack_friends_link_block']['label'] = '';
  $handler->display->display_options['fields']['comstack_friends_link_block']['element_label_colon'] = FALSE;
  $handler->display->display_options['defaults']['sorts'] = FALSE;
  /* Sort criterion: User: Name */
  $handler->display->display_options['sorts']['name']['id'] = 'name';
  $handler->display->display_options['sorts']['name']['table'] = 'users';
  $handler->display->display_options['sorts']['name']['field'] = 'name';
  $handler->display->display_options['sorts']['name']['relationship'] = 'requester_id';
  $handler->display->display_options['defaults']['arguments'] = FALSE;
  /* Contextual filter: User relationships: Requestee user */
  $handler->display->display_options['arguments']['requestee_id']['id'] = 'requestee_id';
  $handler->display->display_options['arguments']['requestee_id']['table'] = 'user_relationships';
  $handler->display->display_options['arguments']['requestee_id']['field'] = 'requestee_id';
  $handler->display->display_options['arguments']['requestee_id']['default_action'] = 'default';
  $handler->display->display_options['arguments']['requestee_id']['default_argument_type'] = 'user';
  $handler->display->display_options['arguments']['requestee_id']['default_argument_options']['user'] = FALSE;
  $handler->display->display_options['arguments']['requestee_id']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['requestee_id']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['requestee_id']['summary_options']['items_per_page'] = '25';
  $handler->display->display_options['arguments']['requestee_id']['specify_validation'] = TRUE;
  $handler->display->display_options['arguments']['requestee_id']['validate']['type'] = 'user';
  $handler->display->display_options['arguments']['requestee_id']['validate_options']['type'] = 'either';
  $handler->display->display_options['defaults']['filter_groups'] = FALSE;
  $handler->display->display_options['defaults']['filters'] = FALSE;
  /* Filter criterion: User relationships: Relationship status */
  $handler->display->display_options['filters']['approved']['id'] = 'approved';
  $handler->display->display_options['filters']['approved']['table'] = 'user_relationships';
  $handler->display->display_options['filters']['approved']['field'] = 'approved';
  $handler->display->display_options['filters']['approved']['value'] = '0';
  /* Filter criterion: User: Active */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'users';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['relationship'] = 'requester_id';
  $handler->display->display_options['filters']['status']['value'] = '1';
  /* Filter criterion: User relationship types: Machine name */
  $handler->display->display_options['filters']['machine_name']['id'] = 'machine_name';
  $handler->display->display_options['filters']['machine_name']['table'] = 'user_relationship_types';
  $handler->display->display_options['filters']['machine_name']['field'] = 'machine_name';
  $handler->display->display_options['filters']['machine_name']['relationship'] = 'rtid';
  $handler->display->display_options['filters']['machine_name']['value'] = array(
    'comstack_friends' => 'comstack_friends',
  );
  $handler->display->display_options['path'] = 'friends/%/pending';
  $handler->display->display_options['menu']['type'] = 'tab';
  $handler->display->display_options['menu']['title'] = 'Pending requests';
  $handler->display->display_options['menu']['weight'] = '2';
  $handler->display->display_options['menu']['context'] = 0;
  $handler->display->display_options['menu']['context_only_inline'] = 0;

  /* Display: Blocked */
  $handler = $view->new_display('page', 'Blocked', 'page_2');
  $handler->display->display_options['defaults']['access'] = FALSE;
  $handler->display->display_options['access']['type'] = 'comstack_friends_current_perm';
  $handler->display->display_options['access']['perm'] = 'can have comstack_blocked relationships';
  $handler->display->display_options['access']['bypass_perm'] = 'access other users comstack_blocked';
  $handler->display->display_options['defaults']['empty'] = FALSE;
  /* No results behavior: Global: Unfiltered text */
  $handler->display->display_options['empty']['area_text_custom']['id'] = 'area_text_custom';
  $handler->display->display_options['empty']['area_text_custom']['table'] = 'views';
  $handler->display->display_options['empty']['area_text_custom']['field'] = 'area_text_custom';
  $handler->display->display_options['empty']['area_text_custom']['empty'] = TRUE;
  $handler->display->display_options['empty']['area_text_custom']['content'] = '<p class="text-center">You haven\'t blocked anyone yet.</p>';
  $handler->display->display_options['defaults']['filter_groups'] = FALSE;
  $handler->display->display_options['defaults']['filters'] = FALSE;
  /* Filter criterion: User relationships: Relationship status */
  $handler->display->display_options['filters']['approved']['id'] = 'approved';
  $handler->display->display_options['filters']['approved']['table'] = 'user_relationships';
  $handler->display->display_options['filters']['approved']['field'] = 'approved';
  $handler->display->display_options['filters']['approved']['value'] = '1';
  /* Filter criterion: User: Active */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'users';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['relationship'] = 'requestee_id';
  $handler->display->display_options['filters']['status']['value'] = '1';
  /* Filter criterion: User relationship types: Machine name */
  $handler->display->display_options['filters']['machine_name']['id'] = 'machine_name';
  $handler->display->display_options['filters']['machine_name']['table'] = 'user_relationship_types';
  $handler->display->display_options['filters']['machine_name']['field'] = 'machine_name';
  $handler->display->display_options['filters']['machine_name']['relationship'] = 'rtid';
  $handler->display->display_options['filters']['machine_name']['value'] = array(
    'comstack_blocked' => 'comstack_blocked',
  );
  $handler->display->display_options['path'] = 'friends/%/blocked';
  $handler->display->display_options['menu']['type'] = 'tab';
  $handler->display->display_options['menu']['title'] = 'Blocked';
  $handler->display->display_options['menu']['weight'] = '3';
  $handler->display->display_options['menu']['context'] = 0;
  $handler->display->display_options['menu']['context_only_inline'] = 0;
  $export['comstack_friends'] = $view;

  $view = new view();
  $view->name = 'comstack_friends_search';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'users';
  $view->human_name = 'Comstack Friends Search';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Find friends';
  $handler->display->display_options['css_class'] = 'friends-list';
  $handler->display->display_options['use_ajax'] = TRUE;
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['group_by'] = TRUE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'access user profiles';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['exposed_form']['options']['submit_button'] = 'Search';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '10';
  $handler->display->display_options['style_plugin'] = 'default';
  $handler->display->display_options['row_plugin'] = 'fields';
  $handler->display->display_options['row_options']['default_field_elements'] = FALSE;
  /* No results behavior: Global: Unfiltered text */
  $handler->display->display_options['empty']['area_text_custom']['id'] = 'area_text_custom';
  $handler->display->display_options['empty']['area_text_custom']['table'] = 'views';
  $handler->display->display_options['empty']['area_text_custom']['field'] = 'area_text_custom';
  $handler->display->display_options['empty']['area_text_custom']['empty'] = TRUE;
  $handler->display->display_options['empty']['area_text_custom']['content'] = '<p class="text-center">No users were found.</p>';
  /* Field: User: Name */
  $handler->display->display_options['fields']['name']['id'] = 'name';
  $handler->display->display_options['fields']['name']['table'] = 'users';
  $handler->display->display_options['fields']['name']['field'] = 'name';
  $handler->display->display_options['fields']['name']['label'] = '';
  $handler->display->display_options['fields']['name']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['name']['alter']['ellipsis'] = FALSE;
  /* Field: User: Picture */
  $handler->display->display_options['fields']['picture']['id'] = 'picture';
  $handler->display->display_options['fields']['picture']['table'] = 'users';
  $handler->display->display_options['fields']['picture']['field'] = 'picture';
  $handler->display->display_options['fields']['picture']['label'] = '';
  $handler->display->display_options['fields']['picture']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['picture']['image_style'] = 'comstack-200-200';
  /* Field: User relationships: Comstack Friends user relationship friend action link */
  $handler->display->display_options['fields']['comstack_friends_link_friend']['id'] = 'comstack_friends_link_friend';
  $handler->display->display_options['fields']['comstack_friends_link_friend']['table'] = 'users';
  $handler->display->display_options['fields']['comstack_friends_link_friend']['field'] = 'comstack_friends_link_friend';
  $handler->display->display_options['fields']['comstack_friends_link_friend']['label'] = '';
  $handler->display->display_options['fields']['comstack_friends_link_friend']['element_label_colon'] = FALSE;
  /* Field: User relationships: Comstack Friends user relationship block action link */
  $handler->display->display_options['fields']['comstack_friends_link_block']['id'] = 'comstack_friends_link_block';
  $handler->display->display_options['fields']['comstack_friends_link_block']['table'] = 'users';
  $handler->display->display_options['fields']['comstack_friends_link_block']['field'] = 'comstack_friends_link_block';
  $handler->display->display_options['fields']['comstack_friends_link_block']['label'] = '';
  $handler->display->display_options['fields']['comstack_friends_link_block']['element_label_colon'] = FALSE;
  /* Sort criterion: User: Created date */
  $handler->display->display_options['sorts']['created']['id'] = 'created';
  $handler->display->display_options['sorts']['created']['table'] = 'users';
  $handler->display->display_options['sorts']['created']['field'] = 'created';
  $handler->display->display_options['sorts']['created']['order'] = 'DESC';
  /* Contextual filter: User: Uid */
  $handler->display->display_options['arguments']['uid']['id'] = 'uid';
  $handler->display->display_options['arguments']['uid']['table'] = 'users';
  $handler->display->display_options['arguments']['uid']['field'] = 'uid';
  $handler->display->display_options['arguments']['uid']['default_action'] = 'default';
  $handler->display->display_options['arguments']['uid']['default_argument_type'] = 'current_user';
  $handler->display->display_options['arguments']['uid']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['uid']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['uid']['summary_options']['items_per_page'] = '25';
  $handler->display->display_options['arguments']['uid']['not'] = TRUE;
  /* Filter criterion: User: Active */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'users';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = '1';
  $handler->display->display_options['filters']['status']['group'] = 1;
  $handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
  /* Filter criterion: User: Name (raw) */
  $handler->display->display_options['filters']['name']['id'] = 'name';
  $handler->display->display_options['filters']['name']['table'] = 'users';
  $handler->display->display_options['filters']['name']['field'] = 'name';
  $handler->display->display_options['filters']['name']['operator'] = 'starts';
  $handler->display->display_options['filters']['name']['exposed'] = TRUE;
  $handler->display->display_options['filters']['name']['expose']['operator_id'] = 'name_op';
  $handler->display->display_options['filters']['name']['expose']['operator'] = 'name_op';
  $handler->display->display_options['filters']['name']['expose']['identifier'] = 'name';
  $handler->display->display_options['filters']['name']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    4 => 0,
    3 => 0,
  );
  /* Filter criterion: User preferences: Comstack Friends - Allow friend requests */
  $handler->display->display_options['filters']['comstack_friends_allow_requests']['id'] = 'comstack_friends_allow_requests';
  $handler->display->display_options['filters']['comstack_friends_allow_requests']['table'] = 'user_preferences';
  $handler->display->display_options['filters']['comstack_friends_allow_requests']['field'] = 'comstack_friends_allow_requests';
  $handler->display->display_options['filters']['comstack_friends_allow_requests']['value'] = '1';
  $handler->display->display_options['filters']['comstack_friends_allow_requests']['treat_null_as_default'] = TRUE;
  /* Filter criterion: User relationships: Comstack Friends exclude roles */
  $handler->display->display_options['filters']['comstack_friends_filter_exclude_roles']['id'] = 'comstack_friends_filter_exclude_roles';
  $handler->display->display_options['filters']['comstack_friends_filter_exclude_roles']['table'] = 'users_roles';
  $handler->display->display_options['filters']['comstack_friends_filter_exclude_roles']['field'] = 'comstack_friends_filter_exclude_roles';

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['exposed_block'] = TRUE;
  $handler->display->display_options['path'] = 'friends/%/find';
  $handler->display->display_options['menu']['type'] = 'tab';
  $handler->display->display_options['menu']['title'] = 'Find friends';
  $handler->display->display_options['menu']['weight'] = '4';
  $handler->display->display_options['menu']['context'] = 0;
  $handler->display->display_options['menu']['context_only_inline'] = 0;
  $export['comstack_friends_search'] = $view;

  return $export;
}
