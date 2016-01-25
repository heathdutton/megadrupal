<?php
/**
 * @file
 * ad_example.views_default.inc
 */

/**
 * Implements hook_views_default_views().
 */
function ad_example_views_default_views() {
  $export = array();

  $view = new view();
  $view->name = 'ad_example_administration';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'node';
  $view->human_name = 'Ad Example Administration';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Ad Example Administration';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'edit any ad_example content';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '50';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'nid' => 'nid',
    'title' => 'title',
    'name' => 'name',
    'rendered_entity' => 'rendered_entity',
    'field_ad_link' => 'field_ad_link',
    'field_ad_size' => 'field_ad_size',
    'total_impressions' => 'total_impressions',
    'total_clicks' => 'total_clicks',
    'expression' => 'expression',
    'status' => 'status',
    'created' => 'created',
    'changed' => 'changed',
    'edit_node' => 'edit_node',
    'delete_node' => 'delete_node',
    'nid_1' => 'nid_1',
  );
  $handler->display->display_options['style_options']['default'] = '-1';
  $handler->display->display_options['style_options']['info'] = array(
    'nid' => array(
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
    'name' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'rendered_entity' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_ad_link' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_ad_size' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'total_impressions' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'total_clicks' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'expression' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'status' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'created' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'changed' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'edit_node' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'delete_node' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'nid_1' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
  );
  /* Relationship: Content: Author */
  $handler->display->display_options['relationships']['uid']['id'] = 'uid';
  $handler->display->display_options['relationships']['uid']['table'] = 'node';
  $handler->display->display_options['relationships']['uid']['field'] = 'uid';
  $handler->display->display_options['relationships']['uid']['label'] = 'Author';
  $handler->display->display_options['relationships']['uid']['required'] = TRUE;
  /* Field: Content: Nid */
  $handler->display->display_options['fields']['nid']['id'] = 'nid';
  $handler->display->display_options['fields']['nid']['table'] = 'node';
  $handler->display->display_options['fields']['nid']['field'] = 'nid';
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  /* Field: User: Name */
  $handler->display->display_options['fields']['name']['id'] = 'name';
  $handler->display->display_options['fields']['name']['table'] = 'users';
  $handler->display->display_options['fields']['name']['field'] = 'name';
  $handler->display->display_options['fields']['name']['relationship'] = 'uid';
  $handler->display->display_options['fields']['name']['label'] = 'Author';
  $handler->display->display_options['fields']['name']['link_to_user'] = FALSE;
  /* Field: Content: Rendered Node */
  $handler->display->display_options['fields']['rendered_entity']['id'] = 'rendered_entity';
  $handler->display->display_options['fields']['rendered_entity']['table'] = 'views_entity_node';
  $handler->display->display_options['fields']['rendered_entity']['field'] = 'rendered_entity';
  $handler->display->display_options['fields']['rendered_entity']['label'] = '';
  $handler->display->display_options['fields']['rendered_entity']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['rendered_entity']['link_to_entity'] = 1;
  $handler->display->display_options['fields']['rendered_entity']['display'] = 'view';
  $handler->display->display_options['fields']['rendered_entity']['view_mode'] = 'example_advertisement_preview';
  $handler->display->display_options['fields']['rendered_entity']['bypass_access'] = 0;
  /* Field: Content: Ad link */
  $handler->display->display_options['fields']['field_ad_link']['id'] = 'field_ad_link';
  $handler->display->display_options['fields']['field_ad_link']['table'] = 'field_data_field_ad_link';
  $handler->display->display_options['fields']['field_ad_link']['field'] = 'field_ad_link';
  $handler->display->display_options['fields']['field_ad_link']['label'] = 'Link';
  $handler->display->display_options['fields']['field_ad_link']['click_sort_column'] = 'url';
  $handler->display->display_options['fields']['field_ad_link']['type'] = 'link_url';
  /* Field: Content: Ad size */
  $handler->display->display_options['fields']['field_ad_size']['id'] = 'field_ad_size';
  $handler->display->display_options['fields']['field_ad_size']['table'] = 'field_data_field_ad_size';
  $handler->display->display_options['fields']['field_ad_size']['field'] = 'field_ad_size';
  $handler->display->display_options['fields']['field_ad_size']['label'] = 'Size';
  $handler->display->display_options['fields']['field_ad_size']['type'] = 'taxonomy_term_reference_plain';
  /* Field: Ad Node: Total Impressions */
  $handler->display->display_options['fields']['total_impressions']['id'] = 'total_impressions';
  $handler->display->display_options['fields']['total_impressions']['table'] = 'ad_node';
  $handler->display->display_options['fields']['total_impressions']['field'] = 'total_impressions';
  $handler->display->display_options['fields']['total_impressions']['separator'] = '';
  /* Field: Ad Node: Total Clicks */
  $handler->display->display_options['fields']['total_clicks']['id'] = 'total_clicks';
  $handler->display->display_options['fields']['total_clicks']['table'] = 'ad_node';
  $handler->display->display_options['fields']['total_clicks']['field'] = 'total_clicks';
  $handler->display->display_options['fields']['total_clicks']['separator'] = '';
  /* Field: Global: Math expression */
  $handler->display->display_options['fields']['expression']['id'] = 'expression';
  $handler->display->display_options['fields']['expression']['table'] = 'views';
  $handler->display->display_options['fields']['expression']['field'] = 'expression';
  $handler->display->display_options['fields']['expression']['label'] = 'Click through';
  $handler->display->display_options['fields']['expression']['set_precision'] = TRUE;
  $handler->display->display_options['fields']['expression']['precision'] = '2';
  $handler->display->display_options['fields']['expression']['separator'] = '';
  $handler->display->display_options['fields']['expression']['suffix'] = '%';
  $handler->display->display_options['fields']['expression']['expression'] = '([total_clicks] / [total_impressions]) * 100';
  /* Field: Content: Published */
  $handler->display->display_options['fields']['status']['id'] = 'status';
  $handler->display->display_options['fields']['status']['table'] = 'node';
  $handler->display->display_options['fields']['status']['field'] = 'status';
  $handler->display->display_options['fields']['status']['type'] = 'unicode-yes-no';
  $handler->display->display_options['fields']['status']['not'] = 0;
  /* Field: Content: Post date */
  $handler->display->display_options['fields']['created']['id'] = 'created';
  $handler->display->display_options['fields']['created']['table'] = 'node';
  $handler->display->display_options['fields']['created']['field'] = 'created';
  $handler->display->display_options['fields']['created']['date_format'] = 'short';
  $handler->display->display_options['fields']['created']['second_date_format'] = 'long';
  /* Field: Content: Updated date */
  $handler->display->display_options['fields']['changed']['id'] = 'changed';
  $handler->display->display_options['fields']['changed']['table'] = 'node';
  $handler->display->display_options['fields']['changed']['field'] = 'changed';
  $handler->display->display_options['fields']['changed']['date_format'] = 'short';
  $handler->display->display_options['fields']['changed']['second_date_format'] = 'long';
  /* Field: Content: Edit link */
  $handler->display->display_options['fields']['edit_node']['id'] = 'edit_node';
  $handler->display->display_options['fields']['edit_node']['table'] = 'views_entity_node';
  $handler->display->display_options['fields']['edit_node']['field'] = 'edit_node';
  $handler->display->display_options['fields']['edit_node']['label'] = '';
  $handler->display->display_options['fields']['edit_node']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['edit_node']['text'] = 'edit';
  /* Field: Content: Delete link */
  $handler->display->display_options['fields']['delete_node']['id'] = 'delete_node';
  $handler->display->display_options['fields']['delete_node']['table'] = 'views_entity_node';
  $handler->display->display_options['fields']['delete_node']['field'] = 'delete_node';
  $handler->display->display_options['fields']['delete_node']['label'] = '';
  $handler->display->display_options['fields']['delete_node']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['delete_node']['text'] = 'delete';
  /* Field: Content: Stats link */
  $handler->display->display_options['fields']['nid_1']['id'] = 'nid_1';
  $handler->display->display_options['fields']['nid_1']['table'] = 'node';
  $handler->display->display_options['fields']['nid_1']['field'] = 'nid';
  $handler->display->display_options['fields']['nid_1']['ui_name'] = 'Content: Stats link';
  $handler->display->display_options['fields']['nid_1']['label'] = '';
  $handler->display->display_options['fields']['nid_1']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['nid_1']['alter']['text'] = 'stats';
  $handler->display->display_options['fields']['nid_1']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['nid_1']['alter']['path'] = 'admin/ad/stats/[nid]';
  $handler->display->display_options['fields']['nid_1']['element_label_colon'] = FALSE;
  /* Filter criterion: Content: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'node';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'ad_example' => 'ad_example',
  );
  /* Filter criterion: Content: Ad size (field_ad_size) */
  $handler->display->display_options['filters']['field_ad_size_tid']['id'] = 'field_ad_size_tid';
  $handler->display->display_options['filters']['field_ad_size_tid']['table'] = 'field_data_field_ad_size';
  $handler->display->display_options['filters']['field_ad_size_tid']['field'] = 'field_ad_size_tid';
  $handler->display->display_options['filters']['field_ad_size_tid']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_ad_size_tid']['expose']['operator_id'] = 'field_ad_size_tid_op';
  $handler->display->display_options['filters']['field_ad_size_tid']['expose']['label'] = 'Size';
  $handler->display->display_options['filters']['field_ad_size_tid']['expose']['operator'] = 'field_ad_size_tid_op';
  $handler->display->display_options['filters']['field_ad_size_tid']['expose']['identifier'] = 'field_ad_size_tid';
  $handler->display->display_options['filters']['field_ad_size_tid']['expose']['remember'] = TRUE;
  $handler->display->display_options['filters']['field_ad_size_tid']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    4 => 0,
    3 => 0,
  );
  $handler->display->display_options['filters']['field_ad_size_tid']['type'] = 'select';
  $handler->display->display_options['filters']['field_ad_size_tid']['vocabulary'] = 'advertisement_size';
  /* Filter criterion: Content: Title */
  $handler->display->display_options['filters']['title']['id'] = 'title';
  $handler->display->display_options['filters']['title']['table'] = 'node';
  $handler->display->display_options['filters']['title']['field'] = 'title';
  $handler->display->display_options['filters']['title']['operator'] = 'contains';
  $handler->display->display_options['filters']['title']['exposed'] = TRUE;
  $handler->display->display_options['filters']['title']['expose']['operator_id'] = 'title_op';
  $handler->display->display_options['filters']['title']['expose']['label'] = 'Title';
  $handler->display->display_options['filters']['title']['expose']['operator'] = 'title_op';
  $handler->display->display_options['filters']['title']['expose']['identifier'] = 'title';
  $handler->display->display_options['filters']['title']['expose']['remember'] = TRUE;
  $handler->display->display_options['filters']['title']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    4 => 0,
    3 => 0,
  );
  /* Filter criterion: Content: Ad link (field_ad_link:url) */
  $handler->display->display_options['filters']['field_ad_link_url']['id'] = 'field_ad_link_url';
  $handler->display->display_options['filters']['field_ad_link_url']['table'] = 'field_data_field_ad_link';
  $handler->display->display_options['filters']['field_ad_link_url']['field'] = 'field_ad_link_url';
  $handler->display->display_options['filters']['field_ad_link_url']['operator'] = 'contains';
  $handler->display->display_options['filters']['field_ad_link_url']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_ad_link_url']['expose']['operator_id'] = 'field_ad_link_url_op';
  $handler->display->display_options['filters']['field_ad_link_url']['expose']['label'] = 'Link';
  $handler->display->display_options['filters']['field_ad_link_url']['expose']['operator'] = 'field_ad_link_url_op';
  $handler->display->display_options['filters']['field_ad_link_url']['expose']['identifier'] = 'field_ad_link_url';
  $handler->display->display_options['filters']['field_ad_link_url']['expose']['remember'] = TRUE;
  $handler->display->display_options['filters']['field_ad_link_url']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    4 => 0,
    3 => 0,
  );

  /* Display: Administrators */
  $handler = $view->new_display('page', 'Administrators', 'page');
  $handler->display->display_options['path'] = 'admin/ad-example';
  $handler->display->display_options['menu']['type'] = 'normal';
  $handler->display->display_options['menu']['title'] = 'Admin Example Advertisement';
  $handler->display->display_options['menu']['weight'] = '0';
  $handler->display->display_options['menu']['name'] = 'management';
  $handler->display->display_options['menu']['context'] = 0;
  $handler->display->display_options['menu']['context_only_inline'] = 0;
  $export['ad_example_administration'] = $view;

  $view = new view();
  $view->name = 'ad_example_display';
  $view->description = '';
  $view->tag = 'ad_example';
  $view->base_table = 'node';
  $view->human_name = 'Ad Example Display';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Leaderboard';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'none';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['style_plugin'] = 'default';
  $handler->display->display_options['row_plugin'] = 'node';
  $handler->display->display_options['row_options']['view_mode'] = 'example_advertisement_display';
  $handler->display->display_options['row_options']['links'] = FALSE;
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = '';
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
  /* Sort criterion: Content: Post date */
  $handler->display->display_options['sorts']['created']['id'] = 'created';
  $handler->display->display_options['sorts']['created']['table'] = 'node';
  $handler->display->display_options['sorts']['created']['field'] = 'created';
  $handler->display->display_options['sorts']['created']['order'] = 'DESC';
  /* Filter criterion: Content: Published */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'node';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = 1;
  $handler->display->display_options['filters']['status']['group'] = 1;
  $handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
  /* Filter criterion: Content: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'node';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'ad_example' => 'ad_example',
  );
  /* Filter criterion: Content: Ad size (field_ad_size) */
  $handler->display->display_options['filters']['field_ad_size_tid']['id'] = 'field_ad_size_tid';
  $handler->display->display_options['filters']['field_ad_size_tid']['table'] = 'field_data_field_ad_size';
  $handler->display->display_options['filters']['field_ad_size_tid']['field'] = 'field_ad_size_tid';
  $handler->display->display_options['filters']['field_ad_size_tid']['value'] = array(
    3 => '3',
  );
  $handler->display->display_options['filters']['field_ad_size_tid']['type'] = 'select';
  $handler->display->display_options['filters']['field_ad_size_tid']['vocabulary'] = 'advertisement_size';

  /* Display: Leaderboard */
  $handler = $view->new_display('block', 'Leaderboard', 'leaderboard');
  $handler->display->display_options['defaults']['filter_groups'] = FALSE;
  $handler->display->display_options['defaults']['filters'] = FALSE;
  /* Filter criterion: Content: Published */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'node';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = 1;
  $handler->display->display_options['filters']['status']['group'] = 1;
  $handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
  /* Filter criterion: Content: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'node';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'ad_example' => 'ad_example',
  );
  /* Filter criterion: Content: Ad size (field_ad_size) */
  $handler->display->display_options['filters']['field_ad_size_tid']['id'] = 'field_ad_size_tid';
  $handler->display->display_options['filters']['field_ad_size_tid']['table'] = 'field_data_field_ad_size';
  $handler->display->display_options['filters']['field_ad_size_tid']['field'] = 'field_ad_size_tid';
  $handler->display->display_options['filters']['field_ad_size_tid']['value'] = array(
    1 => '1',
  );
  $handler->display->display_options['filters']['field_ad_size_tid']['type'] = 'select';
  $handler->display->display_options['filters']['field_ad_size_tid']['vocabulary'] = 'advertisement_size';

  /* Display: Rectangle */
  $handler = $view->new_display('block', 'Rectangle', 'rectangle');
  $handler->display->display_options['defaults']['filter_groups'] = FALSE;
  $handler->display->display_options['defaults']['filters'] = FALSE;
  /* Filter criterion: Content: Published */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'node';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = 1;
  $handler->display->display_options['filters']['status']['group'] = 1;
  $handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
  /* Filter criterion: Content: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'node';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'ad_example' => 'ad_example',
  );
  /* Filter criterion: Content: Ad size (field_ad_size) */
  $handler->display->display_options['filters']['field_ad_size_tid']['id'] = 'field_ad_size_tid';
  $handler->display->display_options['filters']['field_ad_size_tid']['table'] = 'field_data_field_ad_size';
  $handler->display->display_options['filters']['field_ad_size_tid']['field'] = 'field_ad_size_tid';
  $handler->display->display_options['filters']['field_ad_size_tid']['value'] = array(
    2 => '2',
  );
  $handler->display->display_options['filters']['field_ad_size_tid']['type'] = 'select';
  $handler->display->display_options['filters']['field_ad_size_tid']['vocabulary'] = 'advertisement_size';

  /* Display: Skyscraper */
  $handler = $view->new_display('block', 'Skyscraper', 'skyscraper');
  $export['ad_example_display'] = $view;

  return $export;
}
