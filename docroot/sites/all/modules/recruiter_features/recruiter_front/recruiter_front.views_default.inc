<?php
/**
 * @file
 * recruiter_front.views_default.inc
 */

/**
 * Implements hook_views_default_views().
 */
function recruiter_front_views_default_views() {
  $export = array();

  $view = new view();
  $view->name = 'front_page_job_search';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'search_api_index_job_search';
  $view->human_name = 'Front page job search';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'none';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'some';
  $handler->display->display_options['pager']['options']['items_per_page'] = '5';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'title' => 'title',
    'field_job_organization' => 'title',
    'field_job_location' => 'title',
  );
  $handler->display->display_options['style_options']['default'] = '-1';
  $handler->display->display_options['style_options']['info'] = array(
    'title' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_job_organization' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_job_location' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
  );
  /* Header: Global: Result summary */
  $handler->display->display_options['header']['result']['id'] = 'result';
  $handler->display->display_options['header']['result']['table'] = 'views';
  $handler->display->display_options['header']['result']['field'] = 'result';
  $handler->display->display_options['header']['result']['content'] = '<h2 class="hot-jobs-summary">Top @per_page of <a href="/search/job">@total</a> total jobs.</h2>';
  /* Field: Indexed Node: Job URL */
  $handler->display->display_options['fields']['job_url']['id'] = 'job_url';
  $handler->display->display_options['fields']['job_url']['table'] = 'search_api_index_job_search';
  $handler->display->display_options['fields']['job_url']['field'] = 'job_url';
  $handler->display->display_options['fields']['job_url']['label'] = '';
  $handler->display->display_options['fields']['job_url']['exclude'] = TRUE;
  $handler->display->display_options['fields']['job_url']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['job_url']['display_as_link'] = FALSE;
  $handler->display->display_options['fields']['job_url']['link_to_entity'] = 0;
  /* Field: Indexed Node: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'search_api_index_job_search';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['title']['alter']['path'] = '[job_url]';
  $handler->display->display_options['fields']['title']['alter']['absolute'] = TRUE;
  $handler->display->display_options['fields']['title']['alter']['link_class'] = 'recruiter-job-link';
  $handler->display->display_options['fields']['title']['element_type'] = 'div';
  $handler->display->display_options['fields']['title']['link_to_entity'] = 0;
  /* Field: Indexed Node: Organization */
  $handler->display->display_options['fields']['field_job_organization']['id'] = 'field_job_organization';
  $handler->display->display_options['fields']['field_job_organization']['table'] = 'search_api_index_job_search';
  $handler->display->display_options['fields']['field_job_organization']['field'] = 'field_job_organization';
  /* Field: Indexed Node: Location */
  $handler->display->display_options['fields']['field_job_location']['id'] = 'field_job_location';
  $handler->display->display_options['fields']['field_job_location']['table'] = 'search_api_index_job_search';
  $handler->display->display_options['fields']['field_job_location']['field'] = 'field_job_location';
  $handler->display->display_options['fields']['field_job_location']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['field_job_location']['alter']['text'] = ', [field_job_location]';
  /* Sort criterion: Indexed Node: Date changed */
  $handler->display->display_options['sorts']['changed']['id'] = 'changed';
  $handler->display->display_options['sorts']['changed']['table'] = 'search_api_index_job_search';
  $handler->display->display_options['sorts']['changed']['field'] = 'changed';
  $handler->display->display_options['sorts']['changed']['order'] = 'DESC';
  /* Filter criterion: Indexed Node: Content type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'search_api_index_job_search';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'job_per_file' => 'job_per_file',
    'job_per_link' => 'job_per_link',
    'job_per_template' => 'job_per_template',
  );
  /* Filter criterion: Indexed Node: Status */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'search_api_index_job_search';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = array(
    1 => '1',
  );
  /* Filter criterion: Indexed Node: Workflow state */
  $handler->display->display_options['filters']['field_job_workflow_state']['id'] = 'field_job_workflow_state';
  $handler->display->display_options['filters']['field_job_workflow_state']['table'] = 'search_api_index_job_search';
  $handler->display->display_options['filters']['field_job_workflow_state']['field'] = 'field_job_workflow_state';
  $handler->display->display_options['filters']['field_job_workflow_state']['value'] = array(
    'published' => 'published',
  );

  /* Display: Block */
  $handler = $view->new_display('block', 'Block', 'block_1');
  $handler->display->display_options['defaults']['hide_admin_links'] = FALSE;
  $handler->display->display_options['block_description'] = 'Front page job search';
  $export['front_page_job_search'] = $view;

  return $export;
}
