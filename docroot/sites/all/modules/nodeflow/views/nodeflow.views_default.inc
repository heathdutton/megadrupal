<?php
/**
 * @file
 * Default views for nodeflow module.
 */

/**
 * Implements hook_views_default_views().
 */
function nodeflow_views_default_views() {
  $views = array();

  /* Draft view */
  $view = new view();
  $view->name = 'nodeflow_draft';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'node';
  $view->human_name = 'Nodeflow draft';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['use_ajax'] = TRUE;
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'access nodeflow';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['exposed_form']['options']['autosubmit'] = TRUE;
  $handler->display->display_options['pager']['type'] = 'some';
  $handler->display->display_options['pager']['options']['items_per_page'] = '10';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['style_plugin'] = 'table';
  /* No results behavior: Global: Text area */
  $handler->display->display_options['empty']['area']['id'] = 'area';
  $handler->display->display_options['empty']['area']['table'] = 'views';
  $handler->display->display_options['empty']['area']['field'] = 'area';
  $handler->display->display_options['empty']['area']['label'] = 'No source nodes';
  $handler->display->display_options['empty']['area']['empty'] = TRUE;
  $handler->display->display_options['empty']['area']['content'] = 'No nodes found.';
  $handler->display->display_options['empty']['area']['format'] = 'filtered_html';
  /* Relationship: Nodeflow: positions */
  $handler->display->display_options['relationships']['nodeflow_rel']['id'] = 'nodeflow_rel';
  $handler->display->display_options['relationships']['nodeflow_rel']['table'] = 'node';
  $handler->display->display_options['relationships']['nodeflow_rel']['field'] = 'nodeflow_rel';
  /* Field: Content: Nid */
  $handler->display->display_options['fields']['nid']['id'] = 'nid';
  $handler->display->display_options['fields']['nid']['table'] = 'node';
  $handler->display->display_options['fields']['nid']['field'] = 'nid';
  $handler->display->display_options['fields']['nid']['label'] = '';
  $handler->display->display_options['fields']['nid']['element_label_colon'] = FALSE;
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['alter']['max_length'] = '30';
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
  $handler->display->display_options['fields']['title']['alter']['trim'] = TRUE;
  /* Field: Content: Post date */
  $handler->display->display_options['fields']['created']['id'] = 'created';
  $handler->display->display_options['fields']['created']['table'] = 'node';
  $handler->display->display_options['fields']['created']['field'] = 'created';
  $handler->display->display_options['fields']['created']['date_format'] = 'custom';
  $handler->display->display_options['fields']['created']['custom_date_format'] = 'd.m - H:i';
  /* Field: Scheduler: Publish on */
  $handler->display->display_options['fields']['publish_on']['id'] = 'publish_on';
  $handler->display->display_options['fields']['publish_on']['table'] = 'scheduler';
  $handler->display->display_options['fields']['publish_on']['field'] = 'publish_on';
  $handler->display->display_options['fields']['publish_on']['label'] = 'Schedule';
  $handler->display->display_options['fields']['publish_on']['date_format'] = 'custom';
  $handler->display->display_options['fields']['publish_on']['custom_date_format'] = 'Y-m-d H:i';
  /* Field: Content: Published */
  $handler->display->display_options['fields']['status']['id'] = 'status';
  $handler->display->display_options['fields']['status']['table'] = 'node';
  $handler->display->display_options['fields']['status']['field'] = 'status';
  $handler->display->display_options['fields']['status']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['status']['not'] = 0;
  /* Sort criterion: Content: Post date */
  $handler->display->display_options['sorts']['created']['id'] = 'created';
  $handler->display->display_options['sorts']['created']['table'] = 'node';
  $handler->display->display_options['sorts']['created']['field'] = 'created';
  $handler->display->display_options['sorts']['created']['order'] = 'DESC';
  /* Filter criterion: Content: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'node';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'article' => 'article',
  );
  $handler->display->display_options['filters']['type']['group'] = 1;
  /* Filter criterion: Nodeflow: Position */
  $handler->display->display_options['filters']['position']['id'] = 'position';
  $handler->display->display_options['filters']['position']['table'] = 'nodeflow';
  $handler->display->display_options['filters']['position']['field'] = 'position';
  $handler->display->display_options['filters']['position']['relationship'] = 'nodeflow_rel';
  $handler->display->display_options['filters']['position']['operator'] = 'empty';
  $handler->display->display_options['filters']['position']['group'] = 1;
  /* Filter criterion: Content: Title */
  $handler->display->display_options['filters']['title']['id'] = 'title';
  $handler->display->display_options['filters']['title']['table'] = 'node';
  $handler->display->display_options['filters']['title']['field'] = 'title';
  $handler->display->display_options['filters']['title']['operator'] = 'contains';
  $handler->display->display_options['filters']['title']['group'] = 1;
  $handler->display->display_options['filters']['title']['exposed'] = TRUE;
  $handler->display->display_options['filters']['title']['expose']['operator_id'] = 'title_op';
  $handler->display->display_options['filters']['title']['expose']['label'] = 'Search in titles';
  $handler->display->display_options['filters']['title']['expose']['operator'] = 'title_op';
  $handler->display->display_options['filters']['title']['expose']['identifier'] = 'title';
  $handler->display->display_options['filters']['title']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
  );
  /* Filter criterion: Content: Published */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'node';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = '0';
  $handler->display->display_options['filters']['status']['group'] = 1;
  $handler->display->display_options['filters']['status']['exposed'] = TRUE;
  $handler->display->display_options['filters']['status']['expose']['operator_id'] = '';
  $handler->display->display_options['filters']['status']['expose']['label'] = 'Published';
  $handler->display->display_options['filters']['status']['expose']['operator'] = 'status_op';
  $handler->display->display_options['filters']['status']['expose']['identifier'] = 'status';
  $handler->display->display_options['filters']['status']['expose']['required'] = TRUE;
  $handler->display->display_options['filters']['status']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
  );

  /* Display: Block */
  $handler = $view->new_display('block', 'Block', 'block');
  $handler->display->display_options['defaults']['hide_admin_links'] = FALSE;

  $views[$view->name] = $view;


  /* Published view */
  $view = new view();
  $view->name = 'nodeflow_published';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'node';
  $view->human_name = t('Nodeflow published');
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */
  
  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = t('Frontpage flow');
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'access nodeflow';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'some';
  $handler->display->display_options['pager']['options']['items_per_page'] = '10';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['style_plugin'] = 'table';
  /* Relationship: Nodeflow: positions */
  $handler->display->display_options['relationships']['nodeflow_rel']['id'] = 'nodeflow_rel';
  $handler->display->display_options['relationships']['nodeflow_rel']['table'] = 'node';
  $handler->display->display_options['relationships']['nodeflow_rel']['field'] = 'nodeflow_rel';
  $handler->display->display_options['relationships']['nodeflow_rel']['required'] = TRUE;
  /* Field: Content: Nid */
  $handler->display->display_options['fields']['nid']['id'] = 'nid';
  $handler->display->display_options['fields']['nid']['table'] = 'node';
  $handler->display->display_options['fields']['nid']['field'] = 'nid';
  $handler->display->display_options['fields']['nid']['label'] = '';
  $handler->display->display_options['fields']['nid']['element_label_colon'] = FALSE;
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
  /* Field: Content: Post date */
  $handler->display->display_options['fields']['created']['id'] = 'created';
  $handler->display->display_options['fields']['created']['table'] = 'node';
  $handler->display->display_options['fields']['created']['field'] = 'created';
  $handler->display->display_options['fields']['created']['label'] = '';
  $handler->display->display_options['fields']['created']['exclude'] = TRUE;
  $handler->display->display_options['fields']['created']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['created']['date_format'] = 'custom';
  $handler->display->display_options['fields']['created']['custom_date_format'] = 'd.m - H:i';
  /* Field: Scheduler: Publish on */
  $handler->display->display_options['fields']['publish_on']['id'] = 'publish_on';
  $handler->display->display_options['fields']['publish_on']['table'] = 'scheduler';
  $handler->display->display_options['fields']['publish_on']['field'] = 'publish_on';
  $handler->display->display_options['fields']['publish_on']['label'] = 'Publish date';
  $handler->display->display_options['fields']['publish_on']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['publish_on']['empty'] = '[created]';
  $handler->display->display_options['fields']['publish_on']['date_format'] = 'custom';
  $handler->display->display_options['fields']['publish_on']['custom_date_format'] = 'd.m - H:i';
  /* Field: Content: Published */
  $handler->display->display_options['fields']['status']['id'] = 'status';
  $handler->display->display_options['fields']['status']['table'] = 'node';
  $handler->display->display_options['fields']['status']['field'] = 'status';
  $handler->display->display_options['fields']['status']['label'] = '';
  $handler->display->display_options['fields']['status']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['status']['not'] = 0;
  /* Contextual filter: Nodeflow: Nodeflow queue ID */
  $handler->display->display_options['arguments']['nfid']['id'] = 'nfid';
  $handler->display->display_options['arguments']['nfid']['table'] = 'nodeflow';
  $handler->display->display_options['arguments']['nfid']['field'] = 'nfid';
  $handler->display->display_options['arguments']['nfid']['relationship'] = 'nodeflow_rel';
  $handler->display->display_options['arguments']['nfid']['default_action'] = 'default';
  $handler->display->display_options['arguments']['nfid']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['nfid']['default_argument_options']['argument'] = '1';
  $handler->display->display_options['arguments']['nfid']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['nfid']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['nfid']['summary_options']['items_per_page'] = '25';
  /* Sort criterion: Nodeflow: Position */
  $handler->display->display_options['sorts']['position']['id'] = 'position';
  $handler->display->display_options['sorts']['position']['table'] = 'nodeflow';
  $handler->display->display_options['sorts']['position']['field'] = 'position';
  $handler->display->display_options['sorts']['position']['relationship'] = 'nodeflow_rel';
  /* Filter criterion: Content: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'node';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'article' => 'article',
  );
  /* Filter criterion: Content: Promoted to front page */
  $handler->display->display_options['filters']['promote']['id'] = 'promote';
  $handler->display->display_options['filters']['promote']['table'] = 'node';
  $handler->display->display_options['filters']['promote']['field'] = 'promote';
  $handler->display->display_options['filters']['promote']['value'] = '1';

  /* Display: Block */
  $handler = $view->new_display('block', 'Block', 'block');
  $handler->display->display_options['defaults']['hide_admin_links'] = FALSE;

  $views[$view->name] = $view;
  
  $view = new view();
  $view->name = 'nodeflow_viewer_1';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'node';
  $view->human_name = 'Nodeflow queue viewer';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Nodeflow queue viewer';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'access nodeflow';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'some';
  $handler->display->display_options['pager']['options']['items_per_page'] = '5';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['style_plugin'] = 'default';
  $handler->display->display_options['row_plugin'] = 'fields';
  /* Relationship: Nodeflow: positions */
  $handler->display->display_options['relationships']['nodeflow_rel']['id'] = 'nodeflow_rel';
  $handler->display->display_options['relationships']['nodeflow_rel']['table'] = 'node';
  $handler->display->display_options['relationships']['nodeflow_rel']['field'] = 'nodeflow_rel';
  $handler->display->display_options['relationships']['nodeflow_rel']['required'] = TRUE;
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = '';
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
  $handler->display->display_options['fields']['title']['element_label_colon'] = FALSE;
  /* Field: Content: Body */
  $handler->display->display_options['fields']['body']['id'] = 'body';
  $handler->display->display_options['fields']['body']['table'] = 'field_data_body';
  $handler->display->display_options['fields']['body']['field'] = 'body';
  $handler->display->display_options['fields']['body']['label'] = '';
  $handler->display->display_options['fields']['body']['element_label_colon'] = FALSE;
  /* Sort criterion: Nodeflow: Position */
  $handler->display->display_options['sorts']['position']['id'] = 'position';
  $handler->display->display_options['sorts']['position']['table'] = 'nodeflow';
  $handler->display->display_options['sorts']['position']['field'] = 'position';
  $handler->display->display_options['sorts']['position']['relationship'] = 'nodeflow_rel';
  /* Contextual filter: Nodeflow: Nodeflow queue ID */
  $handler->display->display_options['arguments']['nfid']['id'] = 'nfid';
  $handler->display->display_options['arguments']['nfid']['table'] = 'nodeflow';
  $handler->display->display_options['arguments']['nfid']['field'] = 'nfid';
  $handler->display->display_options['arguments']['nfid']['relationship'] = 'nodeflow_rel';
  $handler->display->display_options['arguments']['nfid']['default_action'] = 'default';
  $handler->display->display_options['arguments']['nfid']['default_argument_type'] = 'raw';
  $handler->display->display_options['arguments']['nfid']['default_argument_options']['index'] = '1';
  $handler->display->display_options['arguments']['nfid']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['nfid']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['nfid']['summary_options']['items_per_page'] = '25';
  /* Filter criterion: Content: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'node';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'article' => 'article',
  );
  /* Filter criterion: Content: Promoted to front page */
  $handler->display->display_options['filters']['promote']['id'] = 'promote';
  $handler->display->display_options['filters']['promote']['table'] = 'node';
  $handler->display->display_options['filters']['promote']['field'] = 'promote';
  $handler->display->display_options['filters']['promote']['value'] = '1';
  /* Filter criterion: Content: Published */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'node';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = '1';

  /* Display: Nodeflow queue viewer */
  $handler = $view->new_display('page', 'Nodeflow queue viewer', 'page_1');
  $handler->display->display_options['defaults']['hide_admin_links'] = FALSE;
  $handler->display->display_options['path'] = 'nodeflow/%';

  
  $views[$view->name] = $view;

  return $views;
}
