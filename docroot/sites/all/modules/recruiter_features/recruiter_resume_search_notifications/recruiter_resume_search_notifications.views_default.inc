<?php
/**
 * @file
 * recruiter_resume_search_notifications.views_default.inc
 */

/**
 * Implements hook_views_default_views().
 */
function recruiter_resume_search_notifications_views_default_views() {
  $export = array();

  $view = new view();
  $view->name = 'resume_search_notififactions_recruiter';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'search_api_saved_search';
  $view->human_name = 'resume_search_notififactions_recruiter';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Saved resume searches';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'role';
  $handler->display->display_options['access']['role'] = array(
    30037204 => '30037204',
    29617163 => '29617163',
    100482362 => '100482362',
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
  $handler->display->display_options['empty']['area']['content'] = 'You have not saved any resume searches yet.
<ul>
  <li><a href="/search/resume">Search for resumes</a></li>
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
  $handler->display->display_options['path'] = 'recruiter/saved-searches';
  $translatables['resume_search_notififactions_recruiter'] = array(
    t('Master'),
    t('Saved resume searches'),
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
    t('You have not saved any resume searches yet.
<ul>
  <li><a href="/search/resume">Search for resumes</a></li>
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
  $export['resume_search_notififactions_recruiter'] = $view;

  return $export;
}
