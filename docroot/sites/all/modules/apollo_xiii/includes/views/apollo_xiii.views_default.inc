<?php

/**
 * Views for line item reference displays.
 */

/**
 * Implements hook_views_default_views().
 */
function apollo_xiii_views_default_views() {
  $views = array();

  $view = new view();
  $view->name = 'apollo_xiii_issue_view';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'apollo_xiii';
  $view->human_name = 'Apollo XIII issue view';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = TRUE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Apollo XIII issue view';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'none';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['query']['options']['apollo_xiii_projects'] = 'commerce_kickstart';
  $handler->display->display_options['query']['options']['apollo_xiii_status'] = array(
    'Open' => 'Open',
  );
  $handler->display->display_options['query']['options']['apollo_xiii_version'] = array(
    '7.x' => '7.x',
  );
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '0';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['pager']['options']['id'] = '0';
  $handler->display->display_options['pager']['options']['quantity'] = '9';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['grouping'] = array(
    0 => array(
      'field' => 'project_title',
      'rendered' => 1,
      'rendered_strip' => 0,
    ),
  );
  $handler->display->display_options['style_options']['columns'] = array(
    'issue_url' => 'issue_url',
    'project_homepage' => 'project_homepage',
    'title' => 'title',
    'creator' => 'creator',
    'date' => 'date',
    'project_title' => 'project_title',
  );
  $handler->display->display_options['style_options']['default'] = 'date';
  $handler->display->display_options['style_options']['info'] = array(
    'issue_url' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'project_homepage' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'title' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'creator' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'date' => array(
      'sortable' => 1,
      'default_sort_order' => 'desc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'project_title' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
  );
  /* Field: Entries from Drupal projects issues: Issue URL */
  $handler->display->display_options['fields']['issue_url']['id'] = 'issue_url';
  $handler->display->display_options['fields']['issue_url']['table'] = 'apollo_xiii';
  $handler->display->display_options['fields']['issue_url']['field'] = 'issue_url';
  $handler->display->display_options['fields']['issue_url']['label'] = '';
  $handler->display->display_options['fields']['issue_url']['exclude'] = TRUE;
  $handler->display->display_options['fields']['issue_url']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['issue_url']['display_as_link'] = FALSE;
  /* Field: Entries from Drupal projects issues: Project Homepage */
  $handler->display->display_options['fields']['project_homepage']['id'] = 'project_homepage';
  $handler->display->display_options['fields']['project_homepage']['table'] = 'apollo_xiii';
  $handler->display->display_options['fields']['project_homepage']['field'] = 'project_homepage';
  $handler->display->display_options['fields']['project_homepage']['label'] = '';
  $handler->display->display_options['fields']['project_homepage']['exclude'] = TRUE;
  $handler->display->display_options['fields']['project_homepage']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['project_homepage']['display_as_link'] = FALSE;
  /* Field: Entries from Drupal projects issues: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'apollo_xiii';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['title']['alter']['path'] = '[issue_url]';
  /* Field: Entries from Drupal projects issues: Creator */
  $handler->display->display_options['fields']['creator']['id'] = 'creator';
  $handler->display->display_options['fields']['creator']['table'] = 'apollo_xiii';
  $handler->display->display_options['fields']['creator']['field'] = 'creator';
  /* Field: Entries from Drupal projects issues: Created date */
  $handler->display->display_options['fields']['date']['id'] = 'date';
  $handler->display->display_options['fields']['date']['table'] = 'apollo_xiii';
  $handler->display->display_options['fields']['date']['field'] = 'date';
  $handler->display->display_options['fields']['date']['date_format'] = 'short';
  /* Field: Entries from Drupal projects issues: Project Title */
  $handler->display->display_options['fields']['project_title']['id'] = 'project_title';
  $handler->display->display_options['fields']['project_title']['table'] = 'apollo_xiii';
  $handler->display->display_options['fields']['project_title']['field'] = 'project_title';
  $handler->display->display_options['fields']['project_title']['label'] = '';
  $handler->display->display_options['fields']['project_title']['exclude'] = TRUE;
  $handler->display->display_options['fields']['project_title']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['project_title']['alter']['path'] = '[project_homepage]';
  $handler->display->display_options['fields']['project_title']['element_label_colon'] = FALSE;
  /* Filter criterion: Entries from Drupal projects issues: Title */
  $handler->display->display_options['filters']['title']['id'] = 'title';
  $handler->display->display_options['filters']['title']['table'] = 'apollo_xiii';
  $handler->display->display_options['filters']['title']['field'] = 'title';
  $handler->display->display_options['filters']['title']['exposed'] = TRUE;
  $handler->display->display_options['filters']['title']['expose']['operator_id'] = 'title_op';
  $handler->display->display_options['filters']['title']['expose']['label'] = 'Issue';
  $handler->display->display_options['filters']['title']['expose']['description'] = 'Search for a specific issue.';
  $handler->display->display_options['filters']['title']['expose']['operator'] = 'title_op';
  $handler->display->display_options['filters']['title']['expose']['identifier'] = 'title';
  $handler->display->display_options['filters']['title']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    4 => 0,
  );

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page_1');
  $handler->display->display_options['defaults']['hide_admin_links'] = FALSE;
  $handler->display->display_options['path'] = 'apollo_xiii';
  $translatables['apollo_xiii_issue_view'] = array(
    t('Master'),
    t('Apollo XIII issue view'),
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
    t('Title'),
    t('Creator'),
    t('Created date'),
    t('Issue'),
    t('Search for a specific issue.'),
    t('Page'),
  );

  $views[$view->name] = $view;

  return $views;
}
