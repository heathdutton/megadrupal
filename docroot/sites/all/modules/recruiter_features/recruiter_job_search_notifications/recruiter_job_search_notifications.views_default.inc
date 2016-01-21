<?php
/**
 * @file
 * recruiter_job_search_notifications.views_default.inc
 */

/**
 * Implements hook_views_default_views().
 */
function recruiter_job_search_notifications_views_default_views() {
  $export = array();

  $view = new view();
  $view->name = 'job_search_notififactions_applicant';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'search_api_saved_search';
  $view->human_name = 'job_search_notififactions_applicant';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Saved job searches';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'role';
  $handler->display->display_options['access']['role'] = array(
    30037204 => '30037204',
    115362423 => '115362423',
    29617163 => '29617163',
  );
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '10';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'id' => 'id',
    'name' => 'name',
    'created' => 'created',
    'last_execute' => 'last_execute',
    'notify_interval' => 'notify_interval',
    'edit_link' => 'edit_link',
    'delete_link' => 'edit_link',
  );
  $handler->display->display_options['style_options']['default'] = 'created';
  $handler->display->display_options['style_options']['info'] = array(
    'id' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'name' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'created' => array(
      'sortable' => 1,
      'default_sort_order' => 'desc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'last_execute' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'notify_interval' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'edit_link' => array(
      'align' => '',
      'separator' => ' | ',
      'empty_column' => 0,
    ),
    'delete_link' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
  );
  /* No results behavior: Global: Text area */
  $handler->display->display_options['empty']['area']['id'] = 'area';
  $handler->display->display_options['empty']['area']['table'] = 'views';
  $handler->display->display_options['empty']['area']['field'] = 'area';
  $handler->display->display_options['empty']['area']['empty'] = TRUE;
  $handler->display->display_options['empty']['area']['content'] = 'You have not saved any job searches yet.
<ul>
  <li><a href="/search/job">Search for jobs</a></li>
</ul>';
  $handler->display->display_options['empty']['area']['format'] = 'filtered_html';
  /* Relationship: Saved search: User */
  $handler->display->display_options['relationships']['uid']['id'] = 'uid';
  $handler->display->display_options['relationships']['uid']['table'] = 'search_api_saved_search';
  $handler->display->display_options['relationships']['uid']['field'] = 'uid';
  $handler->display->display_options['relationships']['uid']['required'] = TRUE;
  /* Field: Saved search: Saved search ID */
  $handler->display->display_options['fields']['id']['id'] = 'id';
  $handler->display->display_options['fields']['id']['table'] = 'search_api_saved_search';
  $handler->display->display_options['fields']['id']['field'] = 'id';
  $handler->display->display_options['fields']['id']['label'] = '';
  $handler->display->display_options['fields']['id']['exclude'] = TRUE;
  $handler->display->display_options['fields']['id']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['id']['separator'] = '';
  /* Field: Saved search: Label */
  $handler->display->display_options['fields']['name']['id'] = 'name';
  $handler->display->display_options['fields']['name']['table'] = 'search_api_saved_search';
  $handler->display->display_options['fields']['name']['field'] = 'name';
  $handler->display->display_options['fields']['name']['label'] = 'Name';
  $handler->display->display_options['fields']['name']['link_to_page'] = 1;
  /* Field: Saved search: Created */
  $handler->display->display_options['fields']['created']['id'] = 'created';
  $handler->display->display_options['fields']['created']['table'] = 'search_api_saved_search';
  $handler->display->display_options['fields']['created']['field'] = 'created';
  $handler->display->display_options['fields']['created']['date_format'] = 'short';
  /* Field: Saved search: Last_execute */
  $handler->display->display_options['fields']['last_execute']['id'] = 'last_execute';
  $handler->display->display_options['fields']['last_execute']['table'] = 'search_api_saved_search';
  $handler->display->display_options['fields']['last_execute']['field'] = 'last_execute';
  $handler->display->display_options['fields']['last_execute']['label'] = 'Last execution';
  $handler->display->display_options['fields']['last_execute']['date_format'] = 'short';
  /* Field: Saved search: Notify_interval */
  $handler->display->display_options['fields']['notify_interval']['id'] = 'notify_interval';
  $handler->display->display_options['fields']['notify_interval']['table'] = 'search_api_saved_search';
  $handler->display->display_options['fields']['notify_interval']['field'] = 'notify_interval';
  $handler->display->display_options['fields']['notify_interval']['label'] = 'Interval';
  $handler->display->display_options['fields']['notify_interval']['granularity'] = '2';
  $handler->display->display_options['fields']['notify_interval']['custom_labels'] = 1;
  /* Field: Saved search: Edit link */
  $handler->display->display_options['fields']['edit_link']['id'] = 'edit_link';
  $handler->display->display_options['fields']['edit_link']['table'] = 'search_api_saved_search';
  $handler->display->display_options['fields']['edit_link']['field'] = 'edit_link';
  $handler->display->display_options['fields']['edit_link']['label'] = 'Operations';
  /* Field: Saved search: Delete link */
  $handler->display->display_options['fields']['delete_link']['id'] = 'delete_link';
  $handler->display->display_options['fields']['delete_link']['table'] = 'search_api_saved_search';
  $handler->display->display_options['fields']['delete_link']['field'] = 'delete_link';
  /* Filter criterion: User: Current */
  $handler->display->display_options['filters']['uid_current']['id'] = 'uid_current';
  $handler->display->display_options['filters']['uid_current']['table'] = 'users';
  $handler->display->display_options['filters']['uid_current']['field'] = 'uid_current';
  $handler->display->display_options['filters']['uid_current']['relationship'] = 'uid';
  $handler->display->display_options['filters']['uid_current']['value'] = '1';
  /* Filter criterion: Saved search: Uid */
  $handler->display->display_options['filters']['uid']['id'] = 'uid';
  $handler->display->display_options['filters']['uid']['table'] = 'search_api_saved_search';
  $handler->display->display_options['filters']['uid']['field'] = 'uid';
  $handler->display->display_options['filters']['uid']['operator'] = '!=';
  $handler->display->display_options['filters']['uid']['value']['value'] = '0';

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['defaults']['hide_admin_links'] = FALSE;
  $handler->display->display_options['path'] = 'applicant/saved-searches';
  $translatables['job_search_notififactions_applicant'] = array(
    t('Master'),
    t('Saved job searches'),
    t('more'),
    t('Apply'),
    t('Reset'),
    t('Sort by'),
    t('Asc'),
    t('Desc'),
    t('Items per page'),
    t('- All -'),
    t('Offset'),
    t('« first'),
    t('‹ previous'),
    t('next ›'),
    t('last »'),
    t('You have not saved any job searches yet.
<ul>
  <li><a href="/search/job">Search for jobs</a></li>
</ul>'),
    t('user'),
    t('.'),
    t('Name'),
    t('Created'),
    t('Last execution'),
    t('Interval'),
    t('Operations'),
    t('Delete link'),
    t('Page'),
  );
  $export['job_search_notififactions_applicant'] = $view;

  $view = new view();
  $view->name = 'saved_searches';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'search_api_saved_search';
  $view->human_name = 'Saved searches';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Saved searches';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'administer users';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['query']['options']['query_comment'] = FALSE;
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '25';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'name' => 'name',
    'name_1' => 'name_1',
    'mail' => 'mail',
    'notify_interval' => 'notify_interval',
    'enabled' => 'enabled',
    'created' => 'created',
  );
  $handler->display->display_options['style_options']['default'] = 'created';
  $handler->display->display_options['style_options']['info'] = array(
    'name' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'name_1' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'mail' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'notify_interval' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'enabled' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'created' => array(
      'sortable' => 1,
      'default_sort_order' => 'desc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
  );
  /* Header: Global: Result summary */
  $handler->display->display_options['header']['result']['id'] = 'result';
  $handler->display->display_options['header']['result']['table'] = 'views';
  $handler->display->display_options['header']['result']['field'] = 'result';
  /* Relationship: Saved search: User */
  $handler->display->display_options['relationships']['uid']['id'] = 'uid';
  $handler->display->display_options['relationships']['uid']['table'] = 'search_api_saved_search';
  $handler->display->display_options['relationships']['uid']['field'] = 'uid';
  /* Field: Saved search: Created */
  $handler->display->display_options['fields']['created']['id'] = 'created';
  $handler->display->display_options['fields']['created']['table'] = 'search_api_saved_search';
  $handler->display->display_options['fields']['created']['field'] = 'created';
  $handler->display->display_options['fields']['created']['date_format'] = 'short';
  /* Field: Saved search: Label */
  $handler->display->display_options['fields']['name']['id'] = 'name';
  $handler->display->display_options['fields']['name']['table'] = 'search_api_saved_search';
  $handler->display->display_options['fields']['name']['field'] = 'name';
  /* Field: User: Name */
  $handler->display->display_options['fields']['name_1']['id'] = 'name_1';
  $handler->display->display_options['fields']['name_1']['table'] = 'users';
  $handler->display->display_options['fields']['name_1']['field'] = 'name';
  $handler->display->display_options['fields']['name_1']['relationship'] = 'uid';
  $handler->display->display_options['fields']['name_1']['label'] = 'User';
  /* Field: Saved search: Mail */
  $handler->display->display_options['fields']['mail']['id'] = 'mail';
  $handler->display->display_options['fields']['mail']['table'] = 'search_api_saved_search';
  $handler->display->display_options['fields']['mail']['field'] = 'mail';
  /* Field: Saved search: Notify_interval */
  $handler->display->display_options['fields']['notify_interval']['id'] = 'notify_interval';
  $handler->display->display_options['fields']['notify_interval']['table'] = 'search_api_saved_search';
  $handler->display->display_options['fields']['notify_interval']['field'] = 'notify_interval';
  $handler->display->display_options['fields']['notify_interval']['granularity'] = '2';
  $handler->display->display_options['fields']['notify_interval']['custom_labels'] = 1;
  /* Field: Saved search: Enabled */
  $handler->display->display_options['fields']['enabled']['id'] = 'enabled';
  $handler->display->display_options['fields']['enabled']['table'] = 'search_api_saved_search';
  $handler->display->display_options['fields']['enabled']['field'] = 'enabled';
  /* Sort criterion: Saved search: Created */
  $handler->display->display_options['sorts']['created']['id'] = 'created';
  $handler->display->display_options['sorts']['created']['table'] = 'search_api_saved_search';
  $handler->display->display_options['sorts']['created']['field'] = 'created';
  $handler->display->display_options['sorts']['created']['order'] = 'DESC';
  /* Filter criterion: User: Name */
  $handler->display->display_options['filters']['uid']['id'] = 'uid';
  $handler->display->display_options['filters']['uid']['table'] = 'users';
  $handler->display->display_options['filters']['uid']['field'] = 'uid';
  $handler->display->display_options['filters']['uid']['relationship'] = 'uid';
  $handler->display->display_options['filters']['uid']['value'] = '';
  $handler->display->display_options['filters']['uid']['group'] = 1;
  $handler->display->display_options['filters']['uid']['exposed'] = TRUE;
  $handler->display->display_options['filters']['uid']['expose']['operator_id'] = 'uid_op';
  $handler->display->display_options['filters']['uid']['expose']['label'] = 'User';
  $handler->display->display_options['filters']['uid']['expose']['operator'] = 'uid_op';
  $handler->display->display_options['filters']['uid']['expose']['identifier'] = 'uid';
  $handler->display->display_options['filters']['uid']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    30037204 => 0,
    100482362 => 0,
    115362423 => 0,
    139881272 => 0,
    29617163 => 0,
  );
  /* Filter criterion: Saved search: Mail */
  $handler->display->display_options['filters']['mail']['id'] = 'mail';
  $handler->display->display_options['filters']['mail']['table'] = 'search_api_saved_search';
  $handler->display->display_options['filters']['mail']['field'] = 'mail';
  $handler->display->display_options['filters']['mail']['operator'] = 'contains';
  $handler->display->display_options['filters']['mail']['group'] = 1;
  $handler->display->display_options['filters']['mail']['exposed'] = TRUE;
  $handler->display->display_options['filters']['mail']['expose']['operator_id'] = 'mail_op';
  $handler->display->display_options['filters']['mail']['expose']['label'] = 'Email address';
  $handler->display->display_options['filters']['mail']['expose']['operator'] = 'mail_op';
  $handler->display->display_options['filters']['mail']['expose']['identifier'] = 'mail';
  /* Filter criterion: Saved search: Enabled */
  $handler->display->display_options['filters']['enabled']['id'] = 'enabled';
  $handler->display->display_options['filters']['enabled']['table'] = 'search_api_saved_search';
  $handler->display->display_options['filters']['enabled']['field'] = 'enabled';
  $handler->display->display_options['filters']['enabled']['value'] = 'All';
  $handler->display->display_options['filters']['enabled']['group'] = 1;
  $handler->display->display_options['filters']['enabled']['exposed'] = TRUE;
  $handler->display->display_options['filters']['enabled']['expose']['operator_id'] = '';
  $handler->display->display_options['filters']['enabled']['expose']['label'] = 'Enabled';
  $handler->display->display_options['filters']['enabled']['expose']['operator'] = 'enabled_op';
  $handler->display->display_options['filters']['enabled']['expose']['identifier'] = 'enabled';
  $handler->display->display_options['filters']['enabled']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    30037204 => 0,
    100482362 => 0,
    115362423 => 0,
    139881272 => 0,
    29617163 => 0,
  );

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['defaults']['hide_admin_links'] = FALSE;
  $handler->display->display_options['path'] = 'admin/recruiter/saved-searches';
  $handler->display->display_options['menu']['type'] = 'normal';
  $handler->display->display_options['menu']['title'] = 'Saved searches';
  $handler->display->display_options['menu']['description'] = 'Provides an overview to all saved searches.';
  $handler->display->display_options['menu']['weight'] = '0';
  $handler->display->display_options['menu']['name'] = 'management';
  $handler->display->display_options['menu']['context'] = 0;
  $translatables['saved_searches'] = array(
    t('Master'),
    t('Saved searches'),
    t('more'),
    t('Apply'),
    t('Reset'),
    t('Sort by'),
    t('Asc'),
    t('Desc'),
    t('Items per page'),
    t('- All -'),
    t('Offset'),
    t('« first'),
    t('‹ previous'),
    t('next ›'),
    t('last »'),
    t('Displaying @start - @end of @total'),
    t('user'),
    t('Created'),
    t('Label'),
    t('User'),
    t('Mail'),
    t('Notify_interval'),
    t('Enabled'),
    t('Email address'),
    t('Page'),
  );
  $export['saved_searches'] = $view;

  $view = new view();
  $view->name = 'saved_searches_dashboard_block';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'search_api_saved_search';
  $view->human_name = 'Saved searches dashboard block';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Latest saved searches';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'administer users';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'some';
  $handler->display->display_options['pager']['options']['items_per_page'] = '5';
  $handler->display->display_options['style_plugin'] = 'table';
  /* Footer: Global: Unfiltered text */
  $handler->display->display_options['footer']['area_text_custom']['id'] = 'area_text_custom';
  $handler->display->display_options['footer']['area_text_custom']['table'] = 'views';
  $handler->display->display_options['footer']['area_text_custom']['field'] = 'area_text_custom';
  $handler->display->display_options['footer']['area_text_custom']['content'] = '<a href="/admin/recruiter/saved-searches">More</a>';
  /* Field: Saved search: Created */
  $handler->display->display_options['fields']['created']['id'] = 'created';
  $handler->display->display_options['fields']['created']['table'] = 'search_api_saved_search';
  $handler->display->display_options['fields']['created']['field'] = 'created';
  $handler->display->display_options['fields']['created']['date_format'] = 'short';
  /* Field: Saved search: Mail */
  $handler->display->display_options['fields']['mail']['id'] = 'mail';
  $handler->display->display_options['fields']['mail']['table'] = 'search_api_saved_search';
  $handler->display->display_options['fields']['mail']['field'] = 'mail';
  $handler->display->display_options['fields']['mail']['label'] = 'E-Mail';
  /* Field: Saved search: Label */
  $handler->display->display_options['fields']['name']['id'] = 'name';
  $handler->display->display_options['fields']['name']['table'] = 'search_api_saved_search';
  $handler->display->display_options['fields']['name']['field'] = 'name';
  $handler->display->display_options['fields']['name']['label'] = 'Title';
  $handler->display->display_options['fields']['name']['link_to_page'] = 1;

  /* Display: Block */
  $handler = $view->new_display('block', 'Block', 'block');
  $translatables['saved_searches_dashboard_block'] = array(
    t('Master'),
    t('Latest saved searches'),
    t('more'),
    t('Apply'),
    t('Reset'),
    t('Sort by'),
    t('Asc'),
    t('Desc'),
    t('<a href="/admin/recruiter/saved-searches">More</a>'),
    t('Created'),
    t('E-Mail'),
    t('Title'),
    t('Block'),
  );
  $export['saved_searches_dashboard_block'] = $view;

  return $export;
}
