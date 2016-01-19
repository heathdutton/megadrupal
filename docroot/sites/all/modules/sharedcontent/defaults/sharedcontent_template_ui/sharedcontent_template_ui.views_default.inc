<?php
/**
 * @file
 * sharedcontent_template_ui.views_default.inc
 */

/**
 * Implements hook_views_default_views().
 */
function sharedcontent_template_ui_views_default_views() {
  $export = array();

  $view = new view();
  $view->name = 'sharedcontent_assignment';
  $view->description = 'Provides a tab on Shared Content dashboard that displays assigned content.';
  $view->tag = 'Shared Content';
  $view->base_table = 'sharedcontent_assignment';
  $view->human_name = 'Assignments';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Assignments';
  $handler->display->display_options['css_class'] = 'sharedcontent-overlay';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'access sharedcontent ui';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '10';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['pager']['options']['id'] = '0';
  $handler->display->display_options['pager']['options']['quantity'] = '9';
  $handler->display->display_options['style_plugin'] = 'table';
  /* Relationship: Shared Content Assignment: Source_id */
  $handler->display->display_options['relationships']['source_id']['id'] = 'source_id';
  $handler->display->display_options['relationships']['source_id']['table'] = 'sharedcontent_assignment';
  $handler->display->display_options['relationships']['source_id']['field'] = 'source_id';
  $handler->display->display_options['relationships']['source_id']['label'] = 'From';
  /* Relationship: Shared Content Assignment: Target_id */
  $handler->display->display_options['relationships']['target_id']['id'] = 'target_id';
  $handler->display->display_options['relationships']['target_id']['table'] = 'sharedcontent_assignment';
  $handler->display->display_options['relationships']['target_id']['field'] = 'target_id';
  $handler->display->display_options['relationships']['target_id']['label'] = 'To';
  /* Field: Shared Content Index: Rendered Shared Content Index */
  $handler->display->display_options['fields']['rendered_entity']['id'] = 'rendered_entity';
  $handler->display->display_options['fields']['rendered_entity']['table'] = 'views_entity_sharedcontent_index';
  $handler->display->display_options['fields']['rendered_entity']['field'] = 'rendered_entity';
  $handler->display->display_options['fields']['rendered_entity']['relationship'] = 'source_id';
  $handler->display->display_options['fields']['rendered_entity']['label'] = 'From';
  $handler->display->display_options['fields']['rendered_entity']['link_to_entity'] = 1;
  $handler->display->display_options['fields']['rendered_entity']['display'] = 'view';
  $handler->display->display_options['fields']['rendered_entity']['view_mode'] = 'share_board';
  /* Field: Shared Content Index: Rendered Shared Content Index */
  $handler->display->display_options['fields']['rendered_entity_1']['id'] = 'rendered_entity_1';
  $handler->display->display_options['fields']['rendered_entity_1']['table'] = 'views_entity_sharedcontent_index';
  $handler->display->display_options['fields']['rendered_entity_1']['field'] = 'rendered_entity';
  $handler->display->display_options['fields']['rendered_entity_1']['relationship'] = 'target_id';
  $handler->display->display_options['fields']['rendered_entity_1']['label'] = 'To';
  $handler->display->display_options['fields']['rendered_entity_1']['link_to_entity'] = 1;
  $handler->display->display_options['fields']['rendered_entity_1']['display'] = 'view';
  $handler->display->display_options['fields']['rendered_entity_1']['view_mode'] = 'share_board';
  /* Field: Shared Content Assignment: Created */
  $handler->display->display_options['fields']['created']['id'] = 'created';
  $handler->display->display_options['fields']['created']['table'] = 'sharedcontent_assignment';
  $handler->display->display_options['fields']['created']['field'] = 'created';
  $handler->display->display_options['fields']['created']['label'] = 'Assignment Creation';
  $handler->display->display_options['fields']['created']['date_format'] = 'short';
  /* Field: Shared Content Index: Bundle */
  $handler->display->display_options['fields']['origin']['id'] = 'origin';
  $handler->display->display_options['fields']['origin']['table'] = 'sharedcontent_index';
  $handler->display->display_options['fields']['origin']['field'] = 'origin';
  $handler->display->display_options['fields']['origin']['relationship'] = 'source_id';
  $handler->display->display_options['fields']['origin']['label'] = '';
  $handler->display->display_options['fields']['origin']['exclude'] = TRUE;
  $handler->display->display_options['fields']['origin']['element_label_colon'] = FALSE;
  /* Field: Shared Content Index: Status */
  $handler->display->display_options['fields']['status']['id'] = 'status';
  $handler->display->display_options['fields']['status']['table'] = 'sharedcontent_index';
  $handler->display->display_options['fields']['status']['field'] = 'status';
  $handler->display->display_options['fields']['status']['relationship'] = 'source_id';
  $handler->display->display_options['fields']['status']['label'] = '';
  $handler->display->display_options['fields']['status']['exclude'] = TRUE;
  $handler->display->display_options['fields']['status']['element_label_colon'] = FALSE;
  /* Field: Shared Content Index: Bundle */
  $handler->display->display_options['fields']['origin_1']['id'] = 'origin_1';
  $handler->display->display_options['fields']['origin_1']['table'] = 'sharedcontent_index';
  $handler->display->display_options['fields']['origin_1']['field'] = 'origin';
  $handler->display->display_options['fields']['origin_1']['relationship'] = 'target_id';
  $handler->display->display_options['fields']['origin_1']['label'] = '';
  $handler->display->display_options['fields']['origin_1']['exclude'] = TRUE;
  $handler->display->display_options['fields']['origin_1']['element_label_colon'] = FALSE;
  /* Field: Shared Content Index: Url */
  $handler->display->display_options['fields']['url']['id'] = 'url';
  $handler->display->display_options['fields']['url']['table'] = 'sharedcontent_index';
  $handler->display->display_options['fields']['url']['field'] = 'url';
  $handler->display->display_options['fields']['url']['relationship'] = 'source_id';
  $handler->display->display_options['fields']['url']['label'] = '';
  $handler->display->display_options['fields']['url']['exclude'] = TRUE;
  $handler->display->display_options['fields']['url']['element_label_colon'] = FALSE;
  /* Field: Shared Content Index: Url */
  $handler->display->display_options['fields']['url_1']['id'] = 'url_1';
  $handler->display->display_options['fields']['url_1']['table'] = 'sharedcontent_index';
  $handler->display->display_options['fields']['url_1']['field'] = 'url';
  $handler->display->display_options['fields']['url_1']['relationship'] = 'target_id';
  $handler->display->display_options['fields']['url_1']['label'] = '';
  $handler->display->display_options['fields']['url_1']['exclude'] = TRUE;
  $handler->display->display_options['fields']['url_1']['element_label_colon'] = FALSE;
  /* Field: Shared Content Assignment: Target_id */
  $handler->display->display_options['fields']['target_id']['id'] = 'target_id';
  $handler->display->display_options['fields']['target_id']['table'] = 'sharedcontent_assignment';
  $handler->display->display_options['fields']['target_id']['field'] = 'target_id';
  $handler->display->display_options['fields']['target_id']['label'] = '';
  $handler->display->display_options['fields']['target_id']['exclude'] = TRUE;
  $handler->display->display_options['fields']['target_id']['element_label_colon'] = FALSE;
  /* Sort criterion: Shared Content Assignment: Created */
  $handler->display->display_options['sorts']['created']['id'] = 'created';
  $handler->display->display_options['sorts']['created']['table'] = 'sharedcontent_assignment';
  $handler->display->display_options['sorts']['created']['field'] = 'created';
  $handler->display->display_options['sorts']['created']['order'] = 'DESC';
  /* Filter criterion: Shared Content Index: Site */
  $handler->display->display_options['filters']['connection_name']['id'] = 'connection_name';
  $handler->display->display_options['filters']['connection_name']['table'] = 'sharedcontent_index';
  $handler->display->display_options['filters']['connection_name']['field'] = 'connection_name';
  $handler->display->display_options['filters']['connection_name']['relationship'] = 'source_id';
  $handler->display->display_options['filters']['connection_name']['group'] = 1;
  $handler->display->display_options['filters']['connection_name']['exposed'] = TRUE;
  $handler->display->display_options['filters']['connection_name']['expose']['operator_id'] = 'connection_name_op';
  $handler->display->display_options['filters']['connection_name']['expose']['label'] = 'From Server';
  $handler->display->display_options['filters']['connection_name']['expose']['operator'] = 'connection_name_op';
  $handler->display->display_options['filters']['connection_name']['expose']['identifier'] = 'connection_name';
  $handler->display->display_options['filters']['connection_name']['expose']['multiple'] = TRUE;
  /* Filter criterion: Shared Content Index: Entity_bundle */
  $handler->display->display_options['filters']['entity_bundle']['id'] = 'entity_bundle';
  $handler->display->display_options['filters']['entity_bundle']['table'] = 'sharedcontent_index';
  $handler->display->display_options['filters']['entity_bundle']['field'] = 'entity_bundle';
  $handler->display->display_options['filters']['entity_bundle']['relationship'] = 'source_id';
  $handler->display->display_options['filters']['entity_bundle']['group'] = 1;
  $handler->display->display_options['filters']['entity_bundle']['exposed'] = TRUE;
  $handler->display->display_options['filters']['entity_bundle']['expose']['operator_id'] = 'entity_bundle_op';
  $handler->display->display_options['filters']['entity_bundle']['expose']['label'] = 'From Type';
  $handler->display->display_options['filters']['entity_bundle']['expose']['operator'] = 'entity_bundle_op';
  $handler->display->display_options['filters']['entity_bundle']['expose']['identifier'] = 'entity_bundle';
  $handler->display->display_options['filters']['entity_bundle']['expose']['multiple'] = TRUE;
  /* Filter criterion: Shared Content Index: Label */
  $handler->display->display_options['filters']['title']['id'] = 'title';
  $handler->display->display_options['filters']['title']['table'] = 'sharedcontent_index';
  $handler->display->display_options['filters']['title']['field'] = 'title';
  $handler->display->display_options['filters']['title']['relationship'] = 'source_id';
  $handler->display->display_options['filters']['title']['operator'] = 'contains';
  $handler->display->display_options['filters']['title']['group'] = 1;
  $handler->display->display_options['filters']['title']['exposed'] = TRUE;
  $handler->display->display_options['filters']['title']['expose']['operator_id'] = 'title_op';
  $handler->display->display_options['filters']['title']['expose']['label'] = 'From Title';
  $handler->display->display_options['filters']['title']['expose']['operator'] = 'title_op';
  $handler->display->display_options['filters']['title']['expose']['identifier'] = 'title';
  /* Filter criterion: Shared Content Index: Site */
  $handler->display->display_options['filters']['connection_name_1']['id'] = 'connection_name_1';
  $handler->display->display_options['filters']['connection_name_1']['table'] = 'sharedcontent_index';
  $handler->display->display_options['filters']['connection_name_1']['field'] = 'connection_name';
  $handler->display->display_options['filters']['connection_name_1']['relationship'] = 'target_id';
  $handler->display->display_options['filters']['connection_name_1']['group'] = 1;
  $handler->display->display_options['filters']['connection_name_1']['exposed'] = TRUE;
  $handler->display->display_options['filters']['connection_name_1']['expose']['operator_id'] = 'connection_name_1_op';
  $handler->display->display_options['filters']['connection_name_1']['expose']['label'] = 'To Server';
  $handler->display->display_options['filters']['connection_name_1']['expose']['operator'] = 'connection_name_1_op';
  $handler->display->display_options['filters']['connection_name_1']['expose']['identifier'] = 'connection_name_1';
  $handler->display->display_options['filters']['connection_name_1']['expose']['multiple'] = TRUE;
  /* Filter criterion: Shared Content Index: Entity_bundle */
  $handler->display->display_options['filters']['entity_bundle_1']['id'] = 'entity_bundle_1';
  $handler->display->display_options['filters']['entity_bundle_1']['table'] = 'sharedcontent_index';
  $handler->display->display_options['filters']['entity_bundle_1']['field'] = 'entity_bundle';
  $handler->display->display_options['filters']['entity_bundle_1']['relationship'] = 'target_id';
  $handler->display->display_options['filters']['entity_bundle_1']['group'] = 1;
  $handler->display->display_options['filters']['entity_bundle_1']['exposed'] = TRUE;
  $handler->display->display_options['filters']['entity_bundle_1']['expose']['operator_id'] = 'entity_bundle_1_op';
  $handler->display->display_options['filters']['entity_bundle_1']['expose']['label'] = 'To Type';
  $handler->display->display_options['filters']['entity_bundle_1']['expose']['operator'] = 'entity_bundle_1_op';
  $handler->display->display_options['filters']['entity_bundle_1']['expose']['identifier'] = 'entity_bundle_1';
  $handler->display->display_options['filters']['entity_bundle_1']['expose']['multiple'] = TRUE;
  /* Filter criterion: Shared Content Index: Label */
  $handler->display->display_options['filters']['title_1']['id'] = 'title_1';
  $handler->display->display_options['filters']['title_1']['table'] = 'sharedcontent_index';
  $handler->display->display_options['filters']['title_1']['field'] = 'title';
  $handler->display->display_options['filters']['title_1']['relationship'] = 'target_id';
  $handler->display->display_options['filters']['title_1']['operator'] = 'contains';
  $handler->display->display_options['filters']['title_1']['group'] = 1;
  $handler->display->display_options['filters']['title_1']['exposed'] = TRUE;
  $handler->display->display_options['filters']['title_1']['expose']['operator_id'] = 'title_1_op';
  $handler->display->display_options['filters']['title_1']['expose']['label'] = 'To Title';
  $handler->display->display_options['filters']['title_1']['expose']['operator'] = 'title_1_op';
  $handler->display->display_options['filters']['title_1']['expose']['identifier'] = 'title_1';
  /* Filter criterion: Shared Content Assignment: Status */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'sharedcontent_assignment';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = array(
    'value' => '0',
  );
  $handler->display->display_options['filters']['status']['group'] = 1;

  /* Display: Assignments */
  $handler = $view->new_display('page', 'Assignments', 'assignments');
  $handler->display->display_options['defaults']['hide_admin_links'] = FALSE;
  $handler->display->display_options['path'] = 'admin/content/sharedcontent/assignments';
  $handler->display->display_options['menu']['type'] = 'tab';
  $handler->display->display_options['menu']['title'] = 'Assignments';
  $handler->display->display_options['menu']['weight'] = '11';
  $handler->display->display_options['menu']['name'] = 'management';
  $handler->display->display_options['menu']['context'] = 0;
  $export['sharedcontent_assignment'] = $view;

  $view = new view();
  $view->name = 'sharedcontent_local_content';
  $view->description = 'Shared Content remote content.';
  $view->tag = 'Shared Content';
  $view->base_table = 'search_api_index_sharedcontent';
  $view->human_name = 'Local Content';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'All local content';
  $handler->display->display_options['css_class'] = 'sharedcontent-overlay';
  $handler->display->display_options['use_ajax'] = TRUE;
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['link_display'] = 'custom_url';
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'access sharedcontent ui';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '10';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['pager']['options']['id'] = '0';
  $handler->display->display_options['pager']['options']['quantity'] = '9';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'rendered_entity' => 'rendered_entity',
    'status' => 'status',
    'entity_created' => 'entity_created',
    'entity_changed' => 'entity_changed',
    'entity_created_1' => 'entity_created_1',
    'entity_changed_1' => 'entity_changed_1',
    'nothing' => 'nothing',
    'search_api_relevance' => 'search_api_relevance',
  );
  $handler->display->display_options['style_options']['default'] = '-1';
  $handler->display->display_options['style_options']['info'] = array(
    'rendered_entity' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'status' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'entity_created' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'entity_changed' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'entity_created_1' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'entity_changed_1' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'nothing' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'search_api_relevance' => array(
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
  $handler->display->display_options['header']['result']['empty'] = TRUE;
  $handler->display->display_options['header']['result']['content'] = '<span class="total">@total</span>';
  /* No results behavior: Global: Text area */
  $handler->display->display_options['empty']['area']['id'] = 'area';
  $handler->display->display_options['empty']['area']['table'] = 'views';
  $handler->display->display_options['empty']['area']['field'] = 'area';
  $handler->display->display_options['empty']['area']['content'] = 'No results.';
  $handler->display->display_options['empty']['area']['format'] = 'filtered_html';
  /* Field: Shared Content Index: Rendered Shared Content Index */
  $handler->display->display_options['fields']['rendered_entity']['id'] = 'rendered_entity';
  $handler->display->display_options['fields']['rendered_entity']['table'] = 'views_entity_sharedcontent_index';
  $handler->display->display_options['fields']['rendered_entity']['field'] = 'rendered_entity';
  $handler->display->display_options['fields']['rendered_entity']['label'] = 'Title';
  $handler->display->display_options['fields']['rendered_entity']['link_to_entity'] = 0;
  $handler->display->display_options['fields']['rendered_entity']['display'] = 'view';
  $handler->display->display_options['fields']['rendered_entity']['view_mode'] = 'sharedcontent_inline';
  /* Field: Indexed Shared Content Index: Status */
  $handler->display->display_options['fields']['status']['id'] = 'status';
  $handler->display->display_options['fields']['status']['table'] = 'search_api_index_sharedcontent';
  $handler->display->display_options['fields']['status']['field'] = 'status';
  $handler->display->display_options['fields']['status']['label'] = '';
  $handler->display->display_options['fields']['status']['exclude'] = TRUE;
  $handler->display->display_options['fields']['status']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['status']['link_to_entity'] = 0;
  $handler->display->display_options['fields']['status']['format_name'] = 0;
  /* Field: created_short */
  $handler->display->display_options['fields']['entity_created']['id'] = 'entity_created';
  $handler->display->display_options['fields']['entity_created']['table'] = 'search_api_index_sharedcontent';
  $handler->display->display_options['fields']['entity_created']['field'] = 'entity_created';
  $handler->display->display_options['fields']['entity_created']['ui_name'] = 'created_short';
  $handler->display->display_options['fields']['entity_created']['label'] = '';
  $handler->display->display_options['fields']['entity_created']['exclude'] = TRUE;
  $handler->display->display_options['fields']['entity_created']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['entity_created']['date_format'] = 'short';
  $handler->display->display_options['fields']['entity_created']['link_to_entity'] = 0;
  /* Field: changed_short */
  $handler->display->display_options['fields']['entity_changed']['id'] = 'entity_changed';
  $handler->display->display_options['fields']['entity_changed']['table'] = 'search_api_index_sharedcontent';
  $handler->display->display_options['fields']['entity_changed']['field'] = 'entity_changed';
  $handler->display->display_options['fields']['entity_changed']['ui_name'] = 'changed_short';
  $handler->display->display_options['fields']['entity_changed']['label'] = '';
  $handler->display->display_options['fields']['entity_changed']['exclude'] = TRUE;
  $handler->display->display_options['fields']['entity_changed']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['entity_changed']['date_format'] = 'short';
  $handler->display->display_options['fields']['entity_changed']['link_to_entity'] = 0;
  /* Field: created_iso */
  $handler->display->display_options['fields']['entity_created_1']['id'] = 'entity_created_1';
  $handler->display->display_options['fields']['entity_created_1']['table'] = 'search_api_index_sharedcontent';
  $handler->display->display_options['fields']['entity_created_1']['field'] = 'entity_created';
  $handler->display->display_options['fields']['entity_created_1']['ui_name'] = 'created_iso';
  $handler->display->display_options['fields']['entity_created_1']['label'] = '';
  $handler->display->display_options['fields']['entity_created_1']['exclude'] = TRUE;
  $handler->display->display_options['fields']['entity_created_1']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['entity_created_1']['date_format'] = 'custom';
  $handler->display->display_options['fields']['entity_created_1']['custom_date_format'] = 'c';
  $handler->display->display_options['fields']['entity_created_1']['link_to_entity'] = 0;
  /* Field: changed_iso */
  $handler->display->display_options['fields']['entity_changed_1']['id'] = 'entity_changed_1';
  $handler->display->display_options['fields']['entity_changed_1']['table'] = 'search_api_index_sharedcontent';
  $handler->display->display_options['fields']['entity_changed_1']['field'] = 'entity_changed';
  $handler->display->display_options['fields']['entity_changed_1']['ui_name'] = 'changed_iso';
  $handler->display->display_options['fields']['entity_changed_1']['label'] = '';
  $handler->display->display_options['fields']['entity_changed_1']['exclude'] = TRUE;
  $handler->display->display_options['fields']['entity_changed_1']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['entity_changed_1']['date_format'] = 'custom';
  $handler->display->display_options['fields']['entity_changed_1']['custom_date_format'] = 'c';
  $handler->display->display_options['fields']['entity_changed_1']['link_to_entity'] = 0;
  /* Field: Global: Custom text */
  $handler->display->display_options['fields']['nothing']['id'] = 'nothing';
  $handler->display->display_options['fields']['nothing']['table'] = 'views';
  $handler->display->display_options['fields']['nothing']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing']['label'] = 'Created / Changed';
  $handler->display->display_options['fields']['nothing']['alter']['text'] = '<time datetime="[entity_created_1] " title="Created">[entity_created]</time>
<br />
<time datetime="[entity_changed_1] " title="Changed">[entity_changed]</time>';
  /* Sort criterion: Indexed Shared Content Index: Changed */
  $handler->display->display_options['sorts']['changed']['id'] = 'changed';
  $handler->display->display_options['sorts']['changed']['table'] = 'search_api_index_sharedcontent';
  $handler->display->display_options['sorts']['changed']['field'] = 'changed';
  $handler->display->display_options['sorts']['changed']['order'] = 'DESC';
  $handler->display->display_options['sorts']['changed']['exposed'] = TRUE;
  $handler->display->display_options['sorts']['changed']['expose']['label'] = 'Changed';
  /* Sort criterion: Indexed Shared Content Index: Created */
  $handler->display->display_options['sorts']['created']['id'] = 'created';
  $handler->display->display_options['sorts']['created']['table'] = 'search_api_index_sharedcontent';
  $handler->display->display_options['sorts']['created']['field'] = 'created';
  $handler->display->display_options['sorts']['created']['order'] = 'DESC';
  $handler->display->display_options['sorts']['created']['exposed'] = TRUE;
  $handler->display->display_options['sorts']['created']['expose']['label'] = 'Created';
  /* Filter criterion: Indexed Shared Content Index: Is linkable */
  $handler->display->display_options['filters']['is_linkable']['id'] = 'is_linkable';
  $handler->display->display_options['filters']['is_linkable']['table'] = 'search_api_index_sharedcontent';
  $handler->display->display_options['filters']['is_linkable']['field'] = 'is_linkable';
  $handler->display->display_options['filters']['is_linkable']['value'] = '1';
  $handler->display->display_options['filters']['is_linkable']['group'] = 1;
  /* Filter criterion: Indexed Shared Content Index: Status */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'search_api_index_sharedcontent';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['operator'] = '<>';
  $handler->display->display_options['filters']['status']['value'] = array(
    0 => '0',
  );
  $handler->display->display_options['filters']['status']['group'] = 1;
  /* Filter criterion: Indexed Shared Content Index: Entity_bundle */
  $handler->display->display_options['filters']['entity_bundle']['id'] = 'entity_bundle';
  $handler->display->display_options['filters']['entity_bundle']['table'] = 'search_api_index_sharedcontent';
  $handler->display->display_options['filters']['entity_bundle']['field'] = 'entity_bundle';
  $handler->display->display_options['filters']['entity_bundle']['value'] = array();
  $handler->display->display_options['filters']['entity_bundle']['group'] = 1;
  $handler->display->display_options['filters']['entity_bundle']['exposed'] = TRUE;
  $handler->display->display_options['filters']['entity_bundle']['expose']['operator_id'] = 'entity_bundle_op';
  $handler->display->display_options['filters']['entity_bundle']['expose']['label'] = 'Type';
  $handler->display->display_options['filters']['entity_bundle']['expose']['operator'] = 'entity_bundle_op';
  $handler->display->display_options['filters']['entity_bundle']['expose']['identifier'] = 'remote_type';
  $handler->display->display_options['filters']['entity_bundle']['expose']['remember'] = TRUE;
  $handler->display->display_options['filters']['entity_bundle']['expose']['multiple'] = TRUE;
  $handler->display->display_options['filters']['entity_bundle']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
  );
  $handler->display->display_options['filters']['entity_bundle']['expose']['reduce'] = 0;
  /* Filter criterion: Search: Fulltext search */
  $handler->display->display_options['filters']['search_api_views_fulltext']['id'] = 'search_api_views_fulltext';
  $handler->display->display_options['filters']['search_api_views_fulltext']['table'] = 'search_api_index_sharedcontent';
  $handler->display->display_options['filters']['search_api_views_fulltext']['field'] = 'search_api_views_fulltext';
  $handler->display->display_options['filters']['search_api_views_fulltext']['group'] = 1;
  $handler->display->display_options['filters']['search_api_views_fulltext']['exposed'] = TRUE;
  $handler->display->display_options['filters']['search_api_views_fulltext']['expose']['operator_id'] = 'search_api_views_fulltext_op';
  $handler->display->display_options['filters']['search_api_views_fulltext']['expose']['label'] = 'Keyword';
  $handler->display->display_options['filters']['search_api_views_fulltext']['expose']['operator'] = 'search_api_views_fulltext_op';
  $handler->display->display_options['filters']['search_api_views_fulltext']['expose']['identifier'] = 'keyword';
  $handler->display->display_options['filters']['search_api_views_fulltext']['expose']['remember'] = TRUE;
  $handler->display->display_options['filters']['search_api_views_fulltext']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    4 => 0,
  );

  /* Display: All */
  $handler = $view->new_display('panel_pane', 'All', 'all');
  $handler->display->display_options['defaults']['hide_admin_links'] = FALSE;
  $handler->display->display_options['inherit_panels_path'] = '1';

  /* Display: Queue */
  $handler = $view->new_display('panel_pane', 'Queue', 'queue');
  $handler->display->display_options['defaults']['title'] = FALSE;
  $handler->display->display_options['title'] = 'Local content queue';
  $handler->display->display_options['defaults']['hide_admin_links'] = FALSE;
  $handler->display->display_options['link_url'] = 'admin/content/sharedcontent';
  $handler->display->display_options['defaults']['pager'] = FALSE;
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '10';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['pager']['options']['id'] = '1';
  $handler->display->display_options['pager']['options']['quantity'] = '9';
  $handler->display->display_options['defaults']['footer'] = FALSE;
  /* Footer: Global: Text area */
  $handler->display->display_options['footer']['area']['id'] = 'area';
  $handler->display->display_options['footer']['area']['table'] = 'views';
  $handler->display->display_options['footer']['area']['field'] = 'area';
  $handler->display->display_options['footer']['area']['content'] = '<a href="/admin/content/sharedcontent/queue/clear/sharedcontent_local_queue?destination=admin/content/sharedcontent/dashboard">Clear local content queue</a>';
  $handler->display->display_options['footer']['area']['format'] = 'filtered_html';
  $handler->display->display_options['defaults']['filter_groups'] = FALSE;
  $handler->display->display_options['defaults']['filters'] = FALSE;
  /* Filter criterion: Indexed Shared Content Index: Is linkable */
  $handler->display->display_options['filters']['is_linkable']['id'] = 'is_linkable';
  $handler->display->display_options['filters']['is_linkable']['table'] = 'search_api_index_sharedcontent';
  $handler->display->display_options['filters']['is_linkable']['field'] = 'is_linkable';
  $handler->display->display_options['filters']['is_linkable']['value'] = '1';
  $handler->display->display_options['filters']['is_linkable']['group'] = 1;
  /* Filter criterion: Indexed Shared Content Index: Status */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'search_api_index_sharedcontent';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['operator'] = '<>';
  $handler->display->display_options['filters']['status']['value'] = array(
    0 => '0',
  );
  $handler->display->display_options['filters']['status']['group'] = 1;
  /* Filter criterion: Indexed Shared Content Index: Shared Content Local Queue */
  $handler->display->display_options['filters']['sharedcontent_local_queue']['id'] = 'sharedcontent_local_queue';
  $handler->display->display_options['filters']['sharedcontent_local_queue']['table'] = 'search_api_index_sharedcontent';
  $handler->display->display_options['filters']['sharedcontent_local_queue']['field'] = 'sharedcontent_local_queue';
  $handler->display->display_options['filters']['sharedcontent_local_queue']['value'] = '1';
  $handler->display->display_options['filters']['sharedcontent_local_queue']['group'] = 1;
  /* Filter criterion: Indexed Shared Content Index: Entity_bundle */
  $handler->display->display_options['filters']['entity_bundle']['id'] = 'entity_bundle';
  $handler->display->display_options['filters']['entity_bundle']['table'] = 'search_api_index_sharedcontent';
  $handler->display->display_options['filters']['entity_bundle']['field'] = 'entity_bundle';
  $handler->display->display_options['filters']['entity_bundle']['value'] = array();
  $handler->display->display_options['filters']['entity_bundle']['group'] = 1;
  $handler->display->display_options['filters']['entity_bundle']['exposed'] = TRUE;
  $handler->display->display_options['filters']['entity_bundle']['expose']['operator_id'] = 'entity_bundle_op';
  $handler->display->display_options['filters']['entity_bundle']['expose']['label'] = 'Type';
  $handler->display->display_options['filters']['entity_bundle']['expose']['operator'] = 'entity_bundle_op';
  $handler->display->display_options['filters']['entity_bundle']['expose']['identifier'] = 'remote_type';
  $handler->display->display_options['filters']['entity_bundle']['expose']['remember'] = TRUE;
  $handler->display->display_options['filters']['entity_bundle']['expose']['multiple'] = TRUE;
  $handler->display->display_options['filters']['entity_bundle']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
  );
  $handler->display->display_options['filters']['entity_bundle']['expose']['reduce'] = 0;
  /* Filter criterion: Search: Fulltext search */
  $handler->display->display_options['filters']['search_api_views_fulltext']['id'] = 'search_api_views_fulltext';
  $handler->display->display_options['filters']['search_api_views_fulltext']['table'] = 'search_api_index_sharedcontent';
  $handler->display->display_options['filters']['search_api_views_fulltext']['field'] = 'search_api_views_fulltext';
  $handler->display->display_options['filters']['search_api_views_fulltext']['group'] = 1;
  $handler->display->display_options['filters']['search_api_views_fulltext']['exposed'] = TRUE;
  $handler->display->display_options['filters']['search_api_views_fulltext']['expose']['operator_id'] = 'search_api_views_fulltext_op';
  $handler->display->display_options['filters']['search_api_views_fulltext']['expose']['label'] = 'Keyword';
  $handler->display->display_options['filters']['search_api_views_fulltext']['expose']['operator'] = 'search_api_views_fulltext_op';
  $handler->display->display_options['filters']['search_api_views_fulltext']['expose']['identifier'] = 'keyword';
  $handler->display->display_options['filters']['search_api_views_fulltext']['expose']['remember'] = TRUE;
  $handler->display->display_options['filters']['search_api_views_fulltext']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    4 => 0,
  );
  $handler->display->display_options['inherit_panels_path'] = '1';

  /* Display: Selected */
  $handler = $view->new_display('panel_pane', 'Selected', 'selected');
  $handler->display->display_options['defaults']['title'] = FALSE;
  $handler->display->display_options['title'] = 'Selected local content';
  $handler->display->display_options['defaults']['hide_admin_links'] = FALSE;
  $handler->display->display_options['defaults']['style_plugin'] = FALSE;
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['defaults']['style_options'] = FALSE;
  $handler->display->display_options['defaults']['row_plugin'] = FALSE;
  $handler->display->display_options['defaults']['row_options'] = FALSE;
  $handler->display->display_options['defaults']['header'] = FALSE;
  $handler->display->display_options['defaults']['sorts'] = FALSE;
  $handler->display->display_options['defaults']['arguments'] = FALSE;
  $handler->display->display_options['defaults']['filter_groups'] = FALSE;
  $handler->display->display_options['defaults']['filters'] = FALSE;
  /* Filter criterion: Shared Content: Is selected local content */
  $handler->display->display_options['filters']['self_filter']['id'] = 'self_filter';
  $handler->display->display_options['filters']['self_filter']['table'] = 'search_api_index_sharedcontent';
  $handler->display->display_options['filters']['self_filter']['field'] = 'self_filter';
  $handler->display->display_options['filters']['self_filter']['value'] = '1';
  $export['sharedcontent_local_content'] = $view;

  $view = new view();
  $view->name = 'sharedcontent_references';
  $view->description = 'Shared Content Dashboard Current Node References All Field';
  $view->tag = 'Shared Content';
  $view->base_table = 'sharedcontent_index';
  $view->human_name = 'References';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'References';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'access sharedcontent ui';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['query']['options']['query_comment'] = FALSE;
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '10';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['pager']['options']['id'] = '31';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'id' => 'id',
  );
  $handler->display->display_options['style_options']['default'] = '-1';
  $handler->display->display_options['style_options']['info'] = array(
    'id' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
  );
  /* No results behavior: Global: Text area */
  $handler->display->display_options['empty']['area']['id'] = 'area';
  $handler->display->display_options['empty']['area']['table'] = 'views';
  $handler->display->display_options['empty']['area']['field'] = 'area';
  $handler->display->display_options['empty']['area']['content'] = 'No referenced index record yet.';
  $handler->display->display_options['empty']['area']['format'] = 'filtered_html';
  /* Field: Shared Content Index: Rendered Shared Content Index */
  $handler->display->display_options['fields']['rendered_entity']['id'] = 'rendered_entity';
  $handler->display->display_options['fields']['rendered_entity']['table'] = 'views_entity_sharedcontent_index';
  $handler->display->display_options['fields']['rendered_entity']['field'] = 'rendered_entity';
  $handler->display->display_options['fields']['rendered_entity']['label'] = 'Title';
  $handler->display->display_options['fields']['rendered_entity']['link_to_entity'] = 0;
  $handler->display->display_options['fields']['rendered_entity']['display'] = 'view';
  $handler->display->display_options['fields']['rendered_entity']['view_mode'] = 'share_board';
  /* Field: created_short */
  $handler->display->display_options['fields']['entity_created']['id'] = 'entity_created';
  $handler->display->display_options['fields']['entity_created']['table'] = 'sharedcontent_index';
  $handler->display->display_options['fields']['entity_created']['field'] = 'entity_created';
  $handler->display->display_options['fields']['entity_created']['ui_name'] = 'created_short';
  $handler->display->display_options['fields']['entity_created']['label'] = '';
  $handler->display->display_options['fields']['entity_created']['exclude'] = TRUE;
  $handler->display->display_options['fields']['entity_created']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['entity_created']['date_format'] = 'short';
  /* Field: changed_short */
  $handler->display->display_options['fields']['entity_changed']['id'] = 'entity_changed';
  $handler->display->display_options['fields']['entity_changed']['table'] = 'sharedcontent_index';
  $handler->display->display_options['fields']['entity_changed']['field'] = 'entity_changed';
  $handler->display->display_options['fields']['entity_changed']['ui_name'] = 'changed_short';
  $handler->display->display_options['fields']['entity_changed']['label'] = '';
  $handler->display->display_options['fields']['entity_changed']['exclude'] = TRUE;
  $handler->display->display_options['fields']['entity_changed']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['entity_changed']['date_format'] = 'short';
  /* Field: created_iso */
  $handler->display->display_options['fields']['entity_created_1']['id'] = 'entity_created_1';
  $handler->display->display_options['fields']['entity_created_1']['table'] = 'sharedcontent_index';
  $handler->display->display_options['fields']['entity_created_1']['field'] = 'entity_created';
  $handler->display->display_options['fields']['entity_created_1']['ui_name'] = 'created_iso';
  $handler->display->display_options['fields']['entity_created_1']['label'] = '';
  $handler->display->display_options['fields']['entity_created_1']['exclude'] = TRUE;
  $handler->display->display_options['fields']['entity_created_1']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['entity_created_1']['date_format'] = 'custom';
  $handler->display->display_options['fields']['entity_created_1']['custom_date_format'] = 'c';
  /* Field: changed_iso */
  $handler->display->display_options['fields']['entity_changed_1']['id'] = 'entity_changed_1';
  $handler->display->display_options['fields']['entity_changed_1']['table'] = 'sharedcontent_index';
  $handler->display->display_options['fields']['entity_changed_1']['field'] = 'entity_changed';
  $handler->display->display_options['fields']['entity_changed_1']['ui_name'] = 'changed_iso';
  $handler->display->display_options['fields']['entity_changed_1']['label'] = '';
  $handler->display->display_options['fields']['entity_changed_1']['exclude'] = TRUE;
  $handler->display->display_options['fields']['entity_changed_1']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['entity_changed_1']['date_format'] = 'custom';
  $handler->display->display_options['fields']['entity_changed_1']['custom_date_format'] = 'c';
  /* Field: Global: Custom text */
  $handler->display->display_options['fields']['nothing']['id'] = 'nothing';
  $handler->display->display_options['fields']['nothing']['table'] = 'views';
  $handler->display->display_options['fields']['nothing']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing']['label'] = 'Created / Changed';
  $handler->display->display_options['fields']['nothing']['alter']['text'] = '<time datetime="[entity_created_1] " title="Created">[entity_created]</time>
<br />
<time datetime="[entity_changed_1] " title="Changed">[entity_changed]</time>';

  /* Display: Current */
  $handler = $view->new_display('panel_pane', 'Current', 'current');
  $handler->display->display_options['display_description'] = 'Sharedcontent Dashboard Current Content References All Field';
  $handler->display->display_options['defaults']['hide_admin_links'] = FALSE;
  $handler->display->display_options['inherit_panels_path'] = '1';
  $export['sharedcontent_references'] = $view;

  $view = new view();
  $view->name = 'sharedcontent_remote_content';
  $view->description = 'Shared Content remote content.';
  $view->tag = 'Shared Content';
  $view->base_table = 'search_api_index_sharedcontent';
  $view->human_name = 'Remote Content';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'All remote content';
  $handler->display->display_options['css_class'] = 'sharedcontent-overlay';
  $handler->display->display_options['use_ajax'] = TRUE;
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'access sharedcontent ui';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '10';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['pager']['options']['id'] = '6';
  $handler->display->display_options['pager']['options']['quantity'] = '9';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'rendered_entity' => 'rendered_entity',
    'status' => 'status',
    'entity_created' => 'entity_created',
    'entity_changed' => 'entity_changed',
    'entity_created_1' => 'entity_created_1',
    'entity_changed_1' => 'entity_changed_1',
    'nothing' => 'nothing',
    'search_api_relevance' => 'search_api_relevance',
  );
  $handler->display->display_options['style_options']['default'] = '-1';
  $handler->display->display_options['style_options']['info'] = array(
    'rendered_entity' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'status' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'entity_created' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'entity_changed' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'entity_created_1' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'entity_changed_1' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'nothing' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'search_api_relevance' => array(
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
  $handler->display->display_options['header']['result']['empty'] = TRUE;
  $handler->display->display_options['header']['result']['content'] = '<span class="total">@total</span>';
  /* No results behavior: Global: Text area */
  $handler->display->display_options['empty']['area']['id'] = 'area';
  $handler->display->display_options['empty']['area']['table'] = 'views';
  $handler->display->display_options['empty']['area']['field'] = 'area';
  $handler->display->display_options['empty']['area']['content'] = 'No results.';
  $handler->display->display_options['empty']['area']['format'] = 'filtered_html';
  /* Field: Shared Content Index: Rendered Shared Content Index */
  $handler->display->display_options['fields']['rendered_entity']['id'] = 'rendered_entity';
  $handler->display->display_options['fields']['rendered_entity']['table'] = 'views_entity_sharedcontent_index';
  $handler->display->display_options['fields']['rendered_entity']['field'] = 'rendered_entity';
  $handler->display->display_options['fields']['rendered_entity']['label'] = 'Title';
  $handler->display->display_options['fields']['rendered_entity']['link_to_entity'] = 0;
  $handler->display->display_options['fields']['rendered_entity']['display'] = 'view';
  $handler->display->display_options['fields']['rendered_entity']['view_mode'] = 'share_board';
  /* Field: Indexed Shared Content Index: Status */
  $handler->display->display_options['fields']['status']['id'] = 'status';
  $handler->display->display_options['fields']['status']['table'] = 'search_api_index_sharedcontent';
  $handler->display->display_options['fields']['status']['field'] = 'status';
  $handler->display->display_options['fields']['status']['label'] = '';
  $handler->display->display_options['fields']['status']['exclude'] = TRUE;
  $handler->display->display_options['fields']['status']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['status']['link_to_entity'] = 0;
  $handler->display->display_options['fields']['status']['format_name'] = 0;
  /* Field: created_short */
  $handler->display->display_options['fields']['entity_created']['id'] = 'entity_created';
  $handler->display->display_options['fields']['entity_created']['table'] = 'search_api_index_sharedcontent';
  $handler->display->display_options['fields']['entity_created']['field'] = 'entity_created';
  $handler->display->display_options['fields']['entity_created']['ui_name'] = 'created_short';
  $handler->display->display_options['fields']['entity_created']['label'] = '';
  $handler->display->display_options['fields']['entity_created']['exclude'] = TRUE;
  $handler->display->display_options['fields']['entity_created']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['entity_created']['date_format'] = 'short';
  $handler->display->display_options['fields']['entity_created']['link_to_entity'] = 0;
  /* Field: changed_short */
  $handler->display->display_options['fields']['entity_changed']['id'] = 'entity_changed';
  $handler->display->display_options['fields']['entity_changed']['table'] = 'search_api_index_sharedcontent';
  $handler->display->display_options['fields']['entity_changed']['field'] = 'entity_changed';
  $handler->display->display_options['fields']['entity_changed']['ui_name'] = 'changed_short';
  $handler->display->display_options['fields']['entity_changed']['label'] = '';
  $handler->display->display_options['fields']['entity_changed']['exclude'] = TRUE;
  $handler->display->display_options['fields']['entity_changed']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['entity_changed']['date_format'] = 'short';
  $handler->display->display_options['fields']['entity_changed']['link_to_entity'] = 0;
  /* Field: created_iso */
  $handler->display->display_options['fields']['entity_created_1']['id'] = 'entity_created_1';
  $handler->display->display_options['fields']['entity_created_1']['table'] = 'search_api_index_sharedcontent';
  $handler->display->display_options['fields']['entity_created_1']['field'] = 'entity_created';
  $handler->display->display_options['fields']['entity_created_1']['ui_name'] = 'created_iso';
  $handler->display->display_options['fields']['entity_created_1']['label'] = '';
  $handler->display->display_options['fields']['entity_created_1']['exclude'] = TRUE;
  $handler->display->display_options['fields']['entity_created_1']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['entity_created_1']['date_format'] = 'custom';
  $handler->display->display_options['fields']['entity_created_1']['custom_date_format'] = 'c';
  $handler->display->display_options['fields']['entity_created_1']['link_to_entity'] = 0;
  /* Field: changed_iso */
  $handler->display->display_options['fields']['entity_changed_1']['id'] = 'entity_changed_1';
  $handler->display->display_options['fields']['entity_changed_1']['table'] = 'search_api_index_sharedcontent';
  $handler->display->display_options['fields']['entity_changed_1']['field'] = 'entity_changed';
  $handler->display->display_options['fields']['entity_changed_1']['ui_name'] = 'changed_iso';
  $handler->display->display_options['fields']['entity_changed_1']['label'] = '';
  $handler->display->display_options['fields']['entity_changed_1']['exclude'] = TRUE;
  $handler->display->display_options['fields']['entity_changed_1']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['entity_changed_1']['date_format'] = 'custom';
  $handler->display->display_options['fields']['entity_changed_1']['custom_date_format'] = 'c';
  $handler->display->display_options['fields']['entity_changed_1']['link_to_entity'] = 0;
  /* Field: Global: Custom text */
  $handler->display->display_options['fields']['nothing']['id'] = 'nothing';
  $handler->display->display_options['fields']['nothing']['table'] = 'views';
  $handler->display->display_options['fields']['nothing']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing']['label'] = 'Created / Changed';
  $handler->display->display_options['fields']['nothing']['alter']['text'] = '<time datetime="[entity_created_1] " title="Created">[entity_created]</time>
<br />
<time datetime="[entity_changed_1] " title="Changed">[entity_changed]</time>';
  /* Sort criterion: Indexed Shared Content Index: Changed */
  $handler->display->display_options['sorts']['changed']['id'] = 'changed';
  $handler->display->display_options['sorts']['changed']['table'] = 'search_api_index_sharedcontent';
  $handler->display->display_options['sorts']['changed']['field'] = 'changed';
  $handler->display->display_options['sorts']['changed']['order'] = 'DESC';
  $handler->display->display_options['sorts']['changed']['exposed'] = TRUE;
  $handler->display->display_options['sorts']['changed']['expose']['label'] = 'Changed';
  /* Sort criterion: Indexed Shared Content Index: Created */
  $handler->display->display_options['sorts']['created']['id'] = 'created';
  $handler->display->display_options['sorts']['created']['table'] = 'search_api_index_sharedcontent';
  $handler->display->display_options['sorts']['created']['field'] = 'created';
  $handler->display->display_options['sorts']['created']['order'] = 'DESC';
  $handler->display->display_options['sorts']['created']['exposed'] = TRUE;
  $handler->display->display_options['sorts']['created']['expose']['label'] = 'Created';
  /* Filter criterion: Indexed Shared Content Index: Status */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'search_api_index_sharedcontent';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['operator'] = '<>';
  $handler->display->display_options['filters']['status']['value'] = array(
    0 => '0',
  );
  $handler->display->display_options['filters']['status']['group'] = 1;
  /* Filter criterion: Shared Content: Is selected local content */
  $handler->display->display_options['filters']['self_filter']['id'] = 'self_filter';
  $handler->display->display_options['filters']['self_filter']['table'] = 'search_api_index_sharedcontent';
  $handler->display->display_options['filters']['self_filter']['field'] = 'self_filter';
  $handler->display->display_options['filters']['self_filter']['value'] = '0';
  $handler->display->display_options['filters']['self_filter']['group'] = 1;
  /* Filter criterion: Indexed Shared Content Index: Site */
  $handler->display->display_options['filters']['connection_name']['id'] = 'connection_name';
  $handler->display->display_options['filters']['connection_name']['table'] = 'search_api_index_sharedcontent';
  $handler->display->display_options['filters']['connection_name']['field'] = 'connection_name';
  $handler->display->display_options['filters']['connection_name']['value'] = array();
  $handler->display->display_options['filters']['connection_name']['group'] = 1;
  $handler->display->display_options['filters']['connection_name']['exposed'] = TRUE;
  $handler->display->display_options['filters']['connection_name']['expose']['operator_id'] = 'connection_name_op';
  $handler->display->display_options['filters']['connection_name']['expose']['label'] = 'Origin';
  $handler->display->display_options['filters']['connection_name']['expose']['operator'] = 'connection_name_op';
  $handler->display->display_options['filters']['connection_name']['expose']['identifier'] = 'connection_name';
  $handler->display->display_options['filters']['connection_name']['expose']['remember'] = TRUE;
  $handler->display->display_options['filters']['connection_name']['expose']['multiple'] = TRUE;
  $handler->display->display_options['filters']['connection_name']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    4 => 0,
  );
  $handler->display->display_options['filters']['connection_name']['expose']['reduce'] = 0;
  /* Filter criterion: Indexed Shared Content Index: Entity_bundle */
  $handler->display->display_options['filters']['entity_bundle']['id'] = 'entity_bundle';
  $handler->display->display_options['filters']['entity_bundle']['table'] = 'search_api_index_sharedcontent';
  $handler->display->display_options['filters']['entity_bundle']['field'] = 'entity_bundle';
  $handler->display->display_options['filters']['entity_bundle']['value'] = array();
  $handler->display->display_options['filters']['entity_bundle']['group'] = 1;
  $handler->display->display_options['filters']['entity_bundle']['exposed'] = TRUE;
  $handler->display->display_options['filters']['entity_bundle']['expose']['operator_id'] = 'entity_bundle_op';
  $handler->display->display_options['filters']['entity_bundle']['expose']['label'] = 'Type';
  $handler->display->display_options['filters']['entity_bundle']['expose']['operator'] = 'entity_bundle_op';
  $handler->display->display_options['filters']['entity_bundle']['expose']['identifier'] = 'remote_type';
  $handler->display->display_options['filters']['entity_bundle']['expose']['remember'] = TRUE;
  $handler->display->display_options['filters']['entity_bundle']['expose']['multiple'] = TRUE;
  $handler->display->display_options['filters']['entity_bundle']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
  );
  $handler->display->display_options['filters']['entity_bundle']['expose']['reduce'] = 0;
  /* Filter criterion: Search: Fulltext search */
  $handler->display->display_options['filters']['search_api_views_fulltext']['id'] = 'search_api_views_fulltext';
  $handler->display->display_options['filters']['search_api_views_fulltext']['table'] = 'search_api_index_sharedcontent';
  $handler->display->display_options['filters']['search_api_views_fulltext']['field'] = 'search_api_views_fulltext';
  $handler->display->display_options['filters']['search_api_views_fulltext']['group'] = 1;
  $handler->display->display_options['filters']['search_api_views_fulltext']['exposed'] = TRUE;
  $handler->display->display_options['filters']['search_api_views_fulltext']['expose']['operator_id'] = 'search_api_views_fulltext_op';
  $handler->display->display_options['filters']['search_api_views_fulltext']['expose']['label'] = 'Keyword';
  $handler->display->display_options['filters']['search_api_views_fulltext']['expose']['operator'] = 'search_api_views_fulltext_op';
  $handler->display->display_options['filters']['search_api_views_fulltext']['expose']['identifier'] = 'keyword';
  $handler->display->display_options['filters']['search_api_views_fulltext']['expose']['remember'] = TRUE;
  $handler->display->display_options['filters']['search_api_views_fulltext']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    4 => 0,
  );

  /* Display: All */
  $handler = $view->new_display('panel_pane', 'All', 'all');
  $handler->display->display_options['defaults']['hide_admin_links'] = FALSE;
  $handler->display->display_options['inherit_panels_path'] = '1';

  /* Display: Queue */
  $handler = $view->new_display('panel_pane', 'Queue', 'queue');
  $handler->display->display_options['defaults']['title'] = FALSE;
  $handler->display->display_options['title'] = 'Remote content queue';
  $handler->display->display_options['defaults']['hide_admin_links'] = FALSE;
  $handler->display->display_options['defaults']['pager'] = FALSE;
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '10';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['pager']['options']['id'] = '4';
  $handler->display->display_options['pager']['options']['quantity'] = '9';
  $handler->display->display_options['defaults']['footer'] = FALSE;
  /* Footer: Global: Text area */
  $handler->display->display_options['footer']['area']['id'] = 'area';
  $handler->display->display_options['footer']['area']['table'] = 'views';
  $handler->display->display_options['footer']['area']['field'] = 'area';
  $handler->display->display_options['footer']['area']['content'] = '<a href="/admin/content/sharedcontent/queue/clear/sharedcontent_remote_queue?destination=admin/content/sharedcontent/dashboard">Clear remote content queue</a>';
  $handler->display->display_options['footer']['area']['format'] = 'filtered_html';
  $handler->display->display_options['defaults']['filter_groups'] = FALSE;
  $handler->display->display_options['defaults']['filters'] = FALSE;
  /* Filter criterion: Indexed Shared Content Index: Status */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'search_api_index_sharedcontent';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['operator'] = '<>';
  $handler->display->display_options['filters']['status']['value'] = array(
    0 => '0',
  );
  $handler->display->display_options['filters']['status']['group'] = 1;
  /* Filter criterion: Shared Content: Is selected local content */
  $handler->display->display_options['filters']['self_filter']['id'] = 'self_filter';
  $handler->display->display_options['filters']['self_filter']['table'] = 'search_api_index_sharedcontent';
  $handler->display->display_options['filters']['self_filter']['field'] = 'self_filter';
  $handler->display->display_options['filters']['self_filter']['value'] = '0';
  $handler->display->display_options['filters']['self_filter']['group'] = 1;
  /* Filter criterion: Indexed Shared Content Index: Shared Content Remote Queue */
  $handler->display->display_options['filters']['sharedcontent_remote_queue']['id'] = 'sharedcontent_remote_queue';
  $handler->display->display_options['filters']['sharedcontent_remote_queue']['table'] = 'search_api_index_sharedcontent';
  $handler->display->display_options['filters']['sharedcontent_remote_queue']['field'] = 'sharedcontent_remote_queue';
  $handler->display->display_options['filters']['sharedcontent_remote_queue']['value'] = '1';
  $handler->display->display_options['filters']['sharedcontent_remote_queue']['group'] = 1;
  /* Filter criterion: Indexed Shared Content Index: Site */
  $handler->display->display_options['filters']['connection_name']['id'] = 'connection_name';
  $handler->display->display_options['filters']['connection_name']['table'] = 'search_api_index_sharedcontent';
  $handler->display->display_options['filters']['connection_name']['field'] = 'connection_name';
  $handler->display->display_options['filters']['connection_name']['value'] = array();
  $handler->display->display_options['filters']['connection_name']['group'] = 1;
  $handler->display->display_options['filters']['connection_name']['exposed'] = TRUE;
  $handler->display->display_options['filters']['connection_name']['expose']['operator_id'] = 'connection_name_op';
  $handler->display->display_options['filters']['connection_name']['expose']['label'] = 'Origin';
  $handler->display->display_options['filters']['connection_name']['expose']['operator'] = 'connection_name_op';
  $handler->display->display_options['filters']['connection_name']['expose']['identifier'] = 'connection_name';
  $handler->display->display_options['filters']['connection_name']['expose']['remember'] = TRUE;
  $handler->display->display_options['filters']['connection_name']['expose']['multiple'] = TRUE;
  $handler->display->display_options['filters']['connection_name']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
  );
  $handler->display->display_options['filters']['connection_name']['expose']['reduce'] = 0;
  $handler->display->display_options['filters']['connection_name']['group_info']['label'] = 'Site';
  $handler->display->display_options['filters']['connection_name']['group_info']['identifier'] = 'connection_name';
  $handler->display->display_options['filters']['connection_name']['group_info']['remember'] = FALSE;
  /* Filter criterion: Indexed Shared Content Index: Entity_bundle */
  $handler->display->display_options['filters']['entity_bundle']['id'] = 'entity_bundle';
  $handler->display->display_options['filters']['entity_bundle']['table'] = 'search_api_index_sharedcontent';
  $handler->display->display_options['filters']['entity_bundle']['field'] = 'entity_bundle';
  $handler->display->display_options['filters']['entity_bundle']['value'] = array();
  $handler->display->display_options['filters']['entity_bundle']['group'] = 1;
  $handler->display->display_options['filters']['entity_bundle']['exposed'] = TRUE;
  $handler->display->display_options['filters']['entity_bundle']['expose']['operator_id'] = 'entity_bundle_op';
  $handler->display->display_options['filters']['entity_bundle']['expose']['label'] = 'Type';
  $handler->display->display_options['filters']['entity_bundle']['expose']['operator'] = 'entity_bundle_op';
  $handler->display->display_options['filters']['entity_bundle']['expose']['identifier'] = 'remote_type';
  $handler->display->display_options['filters']['entity_bundle']['expose']['remember'] = TRUE;
  $handler->display->display_options['filters']['entity_bundle']['expose']['multiple'] = TRUE;
  $handler->display->display_options['filters']['entity_bundle']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
  );
  $handler->display->display_options['filters']['entity_bundle']['expose']['reduce'] = 0;
  /* Filter criterion: Search: Fulltext search */
  $handler->display->display_options['filters']['search_api_views_fulltext']['id'] = 'search_api_views_fulltext';
  $handler->display->display_options['filters']['search_api_views_fulltext']['table'] = 'search_api_index_sharedcontent';
  $handler->display->display_options['filters']['search_api_views_fulltext']['field'] = 'search_api_views_fulltext';
  $handler->display->display_options['filters']['search_api_views_fulltext']['group'] = 1;
  $handler->display->display_options['filters']['search_api_views_fulltext']['exposed'] = TRUE;
  $handler->display->display_options['filters']['search_api_views_fulltext']['expose']['operator_id'] = 'search_api_views_fulltext_op';
  $handler->display->display_options['filters']['search_api_views_fulltext']['expose']['label'] = 'Keyword';
  $handler->display->display_options['filters']['search_api_views_fulltext']['expose']['operator'] = 'search_api_views_fulltext_op';
  $handler->display->display_options['filters']['search_api_views_fulltext']['expose']['identifier'] = 'keyword';
  $handler->display->display_options['filters']['search_api_views_fulltext']['expose']['remember'] = TRUE;
  $handler->display->display_options['filters']['search_api_views_fulltext']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    4 => 0,
  );
  $handler->display->display_options['inherit_panels_path'] = '1';

  /* Display: Linked */
  $handler = $view->new_display('panel_pane', 'Linked', 'linked');
  $handler->display->display_options['defaults']['title'] = FALSE;
  $handler->display->display_options['title'] = 'Links to current content';
  $handler->display->display_options['defaults']['hide_admin_links'] = FALSE;
  $handler->display->display_options['defaults']['pager'] = FALSE;
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '10';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['pager']['options']['id'] = '5';
  $handler->display->display_options['pager']['options']['quantity'] = '9';
  $handler->display->display_options['defaults']['sorts'] = FALSE;
  /* Sort criterion: Indexed Shared Content Index: Changed */
  $handler->display->display_options['sorts']['changed']['id'] = 'changed';
  $handler->display->display_options['sorts']['changed']['table'] = 'search_api_index_sharedcontent';
  $handler->display->display_options['sorts']['changed']['field'] = 'changed';
  $handler->display->display_options['sorts']['changed']['order'] = 'DESC';
  $handler->display->display_options['sorts']['changed']['expose']['label'] = 'Changed';
  $handler->display->display_options['defaults']['filter_groups'] = FALSE;
  $handler->display->display_options['defaults']['filters'] = FALSE;
  /* Filter criterion: Shared Content: Is assigned to selected local content */
  $handler->display->display_options['filters']['assignment_filter']['id'] = 'assignment_filter';
  $handler->display->display_options['filters']['assignment_filter']['table'] = 'search_api_index_sharedcontent';
  $handler->display->display_options['filters']['assignment_filter']['field'] = 'assignment_filter';
  $handler->display->display_options['filters']['assignment_filter']['value'] = 1;
  $handler->display->display_options['filters']['assignment_filter']['group'] = 1;
  /* Filter criterion: Indexed Shared Content Index: Site */
  $handler->display->display_options['filters']['connection_name']['id'] = 'connection_name';
  $handler->display->display_options['filters']['connection_name']['table'] = 'search_api_index_sharedcontent';
  $handler->display->display_options['filters']['connection_name']['field'] = 'connection_name';
  $handler->display->display_options['filters']['connection_name']['value'] = array();
  $handler->display->display_options['filters']['connection_name']['group'] = 1;
  $handler->display->display_options['filters']['connection_name']['exposed'] = TRUE;
  $handler->display->display_options['filters']['connection_name']['expose']['operator_id'] = 'connection_name_op';
  $handler->display->display_options['filters']['connection_name']['expose']['label'] = 'Origin';
  $handler->display->display_options['filters']['connection_name']['expose']['operator'] = 'connection_name_op';
  $handler->display->display_options['filters']['connection_name']['expose']['identifier'] = 'connection_name';
  $handler->display->display_options['filters']['connection_name']['expose']['remember'] = TRUE;
  $handler->display->display_options['filters']['connection_name']['expose']['multiple'] = TRUE;
  $handler->display->display_options['filters']['connection_name']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    4 => 0,
  );
  $handler->display->display_options['filters']['connection_name']['expose']['reduce'] = 0;
  /* Filter criterion: Indexed Shared Content Index: Entity_bundle */
  $handler->display->display_options['filters']['entity_bundle']['id'] = 'entity_bundle';
  $handler->display->display_options['filters']['entity_bundle']['table'] = 'search_api_index_sharedcontent';
  $handler->display->display_options['filters']['entity_bundle']['field'] = 'entity_bundle';
  $handler->display->display_options['filters']['entity_bundle']['value'] = array();
  $handler->display->display_options['filters']['entity_bundle']['group'] = 1;
  $handler->display->display_options['filters']['entity_bundle']['exposed'] = TRUE;
  $handler->display->display_options['filters']['entity_bundle']['expose']['operator_id'] = 'entity_bundle_op';
  $handler->display->display_options['filters']['entity_bundle']['expose']['label'] = 'Type';
  $handler->display->display_options['filters']['entity_bundle']['expose']['operator'] = 'entity_bundle_op';
  $handler->display->display_options['filters']['entity_bundle']['expose']['identifier'] = 'entity_bundle';
  $handler->display->display_options['filters']['entity_bundle']['expose']['remember'] = TRUE;
  $handler->display->display_options['filters']['entity_bundle']['expose']['multiple'] = TRUE;
  $handler->display->display_options['filters']['entity_bundle']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    4 => 0,
  );
  $handler->display->display_options['filters']['entity_bundle']['expose']['reduce'] = 0;
  $handler->display->display_options['inherit_panels_path'] = '1';

  /* Display: Overview */
  $handler = $view->new_display('page', 'Overview', 'overview');
  $handler->display->display_options['defaults']['hide_admin_links'] = FALSE;
  $handler->display->display_options['defaults']['header'] = FALSE;
  $handler->display->display_options['defaults']['sorts'] = FALSE;
  /* Sort criterion: Indexed Shared Content Index: Changed */
  $handler->display->display_options['sorts']['changed']['id'] = 'changed';
  $handler->display->display_options['sorts']['changed']['table'] = 'search_api_index_sharedcontent';
  $handler->display->display_options['sorts']['changed']['field'] = 'changed';
  $handler->display->display_options['sorts']['changed']['order'] = 'DESC';
  $handler->display->display_options['sorts']['changed']['expose']['label'] = 'Changed';
  $handler->display->display_options['defaults']['filter_groups'] = FALSE;
  $handler->display->display_options['defaults']['filters'] = FALSE;
  /* Filter criterion: Indexed Shared Content Index: Status */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'search_api_index_sharedcontent';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['operator'] = '<>';
  $handler->display->display_options['filters']['status']['value'] = array(
    0 => '0',
  );
  $handler->display->display_options['filters']['status']['group'] = 1;
  /* Filter criterion: Indexed Shared Content Index: Site */
  $handler->display->display_options['filters']['connection_name']['id'] = 'connection_name';
  $handler->display->display_options['filters']['connection_name']['table'] = 'search_api_index_sharedcontent';
  $handler->display->display_options['filters']['connection_name']['field'] = 'connection_name';
  $handler->display->display_options['filters']['connection_name']['value'] = array();
  $handler->display->display_options['filters']['connection_name']['group'] = 1;
  $handler->display->display_options['filters']['connection_name']['exposed'] = TRUE;
  $handler->display->display_options['filters']['connection_name']['expose']['operator_id'] = 'connection_name_op';
  $handler->display->display_options['filters']['connection_name']['expose']['label'] = 'Origin';
  $handler->display->display_options['filters']['connection_name']['expose']['operator'] = 'connection_name_op';
  $handler->display->display_options['filters']['connection_name']['expose']['identifier'] = 'sc_remote_all_connection_name';
  $handler->display->display_options['filters']['connection_name']['expose']['remember'] = TRUE;
  $handler->display->display_options['filters']['connection_name']['expose']['multiple'] = TRUE;
  /* Filter criterion: Indexed Shared Content Index: Entity_bundle */
  $handler->display->display_options['filters']['entity_bundle']['id'] = 'entity_bundle';
  $handler->display->display_options['filters']['entity_bundle']['table'] = 'search_api_index_sharedcontent';
  $handler->display->display_options['filters']['entity_bundle']['field'] = 'entity_bundle';
  $handler->display->display_options['filters']['entity_bundle']['value'] = array();
  $handler->display->display_options['filters']['entity_bundle']['group'] = 1;
  $handler->display->display_options['filters']['entity_bundle']['exposed'] = TRUE;
  $handler->display->display_options['filters']['entity_bundle']['expose']['operator_id'] = 'entity_bundle_op';
  $handler->display->display_options['filters']['entity_bundle']['expose']['label'] = 'Type';
  $handler->display->display_options['filters']['entity_bundle']['expose']['operator'] = 'entity_bundle_op';
  $handler->display->display_options['filters']['entity_bundle']['expose']['identifier'] = 'entity_bundle';
  $handler->display->display_options['filters']['entity_bundle']['expose']['multiple'] = TRUE;
  $handler->display->display_options['filters']['entity_bundle']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
  );
  $handler->display->display_options['filters']['entity_bundle']['expose']['reduce'] = 0;
  /* Filter criterion: Search: Fulltext search */
  $handler->display->display_options['filters']['search_api_views_fulltext']['id'] = 'search_api_views_fulltext';
  $handler->display->display_options['filters']['search_api_views_fulltext']['table'] = 'search_api_index_sharedcontent';
  $handler->display->display_options['filters']['search_api_views_fulltext']['field'] = 'search_api_views_fulltext';
  $handler->display->display_options['filters']['search_api_views_fulltext']['group'] = 1;
  $handler->display->display_options['filters']['search_api_views_fulltext']['exposed'] = TRUE;
  $handler->display->display_options['filters']['search_api_views_fulltext']['expose']['operator_id'] = 'search_api_views_fulltext_op';
  $handler->display->display_options['filters']['search_api_views_fulltext']['expose']['label'] = 'Keyword';
  $handler->display->display_options['filters']['search_api_views_fulltext']['expose']['operator'] = 'search_api_views_fulltext_op';
  $handler->display->display_options['filters']['search_api_views_fulltext']['expose']['identifier'] = 'search_api_views_fulltext';
  $handler->display->display_options['filters']['search_api_views_fulltext']['expose']['remember'] = TRUE;
  $handler->display->display_options['filters']['search_api_views_fulltext']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    4 => 0,
  );
  $handler->display->display_options['path'] = 'admin/content/sharedcontent/remote';
  $handler->display->display_options['menu']['type'] = 'tab';
  $handler->display->display_options['menu']['title'] = 'Remote Content';
  $handler->display->display_options['menu']['weight'] = '10';
  $handler->display->display_options['menu']['name'] = 'management';
  $handler->display->display_options['menu']['context'] = 0;

  /* Display: Suggestions */
  $handler = $view->new_display('panel_pane', 'Suggestions', 'suggestions');
  $handler->display->display_options['defaults']['title'] = FALSE;
  $handler->display->display_options['title'] = 'Suggested remote content';
  $handler->display->display_options['display_description'] = 'Suggestions for current selected node';
  $handler->display->display_options['defaults']['hide_admin_links'] = FALSE;
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: Search: Relevance */
  $handler->display->display_options['fields']['search_api_relevance']['id'] = 'search_api_relevance';
  $handler->display->display_options['fields']['search_api_relevance']['table'] = 'search_api_index_sharedcontent';
  $handler->display->display_options['fields']['search_api_relevance']['field'] = 'search_api_relevance';
  $handler->display->display_options['fields']['search_api_relevance']['label'] = 'Score';
  $handler->display->display_options['fields']['search_api_relevance']['separator'] = '';
  $handler->display->display_options['fields']['search_api_relevance']['link_to_entity'] = 0;
  /* Field: Shared Content Index: Rendered Shared Content Index */
  $handler->display->display_options['fields']['rendered_entity']['id'] = 'rendered_entity';
  $handler->display->display_options['fields']['rendered_entity']['table'] = 'views_entity_sharedcontent_index';
  $handler->display->display_options['fields']['rendered_entity']['field'] = 'rendered_entity';
  $handler->display->display_options['fields']['rendered_entity']['label'] = 'Title';
  $handler->display->display_options['fields']['rendered_entity']['link_to_entity'] = 0;
  $handler->display->display_options['fields']['rendered_entity']['display'] = 'view';
  $handler->display->display_options['fields']['rendered_entity']['view_mode'] = 'share_board';
  /* Field: Indexed Shared Content Index: Status */
  $handler->display->display_options['fields']['status']['id'] = 'status';
  $handler->display->display_options['fields']['status']['table'] = 'search_api_index_sharedcontent';
  $handler->display->display_options['fields']['status']['field'] = 'status';
  $handler->display->display_options['fields']['status']['label'] = '';
  $handler->display->display_options['fields']['status']['exclude'] = TRUE;
  $handler->display->display_options['fields']['status']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['status']['link_to_entity'] = 0;
  $handler->display->display_options['fields']['status']['format_name'] = 0;
  /* Field: created_short */
  $handler->display->display_options['fields']['entity_created']['id'] = 'entity_created';
  $handler->display->display_options['fields']['entity_created']['table'] = 'search_api_index_sharedcontent';
  $handler->display->display_options['fields']['entity_created']['field'] = 'entity_created';
  $handler->display->display_options['fields']['entity_created']['ui_name'] = 'created_short';
  $handler->display->display_options['fields']['entity_created']['label'] = '';
  $handler->display->display_options['fields']['entity_created']['exclude'] = TRUE;
  $handler->display->display_options['fields']['entity_created']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['entity_created']['date_format'] = 'short';
  $handler->display->display_options['fields']['entity_created']['link_to_entity'] = 0;
  /* Field: changed_short */
  $handler->display->display_options['fields']['entity_changed']['id'] = 'entity_changed';
  $handler->display->display_options['fields']['entity_changed']['table'] = 'search_api_index_sharedcontent';
  $handler->display->display_options['fields']['entity_changed']['field'] = 'entity_changed';
  $handler->display->display_options['fields']['entity_changed']['ui_name'] = 'changed_short';
  $handler->display->display_options['fields']['entity_changed']['label'] = '';
  $handler->display->display_options['fields']['entity_changed']['exclude'] = TRUE;
  $handler->display->display_options['fields']['entity_changed']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['entity_changed']['date_format'] = 'short';
  $handler->display->display_options['fields']['entity_changed']['link_to_entity'] = 0;
  /* Field: created_iso */
  $handler->display->display_options['fields']['entity_created_1']['id'] = 'entity_created_1';
  $handler->display->display_options['fields']['entity_created_1']['table'] = 'search_api_index_sharedcontent';
  $handler->display->display_options['fields']['entity_created_1']['field'] = 'entity_created';
  $handler->display->display_options['fields']['entity_created_1']['ui_name'] = 'created_iso';
  $handler->display->display_options['fields']['entity_created_1']['label'] = '';
  $handler->display->display_options['fields']['entity_created_1']['exclude'] = TRUE;
  $handler->display->display_options['fields']['entity_created_1']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['entity_created_1']['date_format'] = 'custom';
  $handler->display->display_options['fields']['entity_created_1']['custom_date_format'] = 'c';
  $handler->display->display_options['fields']['entity_created_1']['link_to_entity'] = 0;
  /* Field: changed_iso */
  $handler->display->display_options['fields']['entity_changed_1']['id'] = 'entity_changed_1';
  $handler->display->display_options['fields']['entity_changed_1']['table'] = 'search_api_index_sharedcontent';
  $handler->display->display_options['fields']['entity_changed_1']['field'] = 'entity_changed';
  $handler->display->display_options['fields']['entity_changed_1']['ui_name'] = 'changed_iso';
  $handler->display->display_options['fields']['entity_changed_1']['label'] = '';
  $handler->display->display_options['fields']['entity_changed_1']['exclude'] = TRUE;
  $handler->display->display_options['fields']['entity_changed_1']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['entity_changed_1']['date_format'] = 'custom';
  $handler->display->display_options['fields']['entity_changed_1']['custom_date_format'] = 'c';
  $handler->display->display_options['fields']['entity_changed_1']['link_to_entity'] = 0;
  /* Field: Global: Custom text */
  $handler->display->display_options['fields']['nothing']['id'] = 'nothing';
  $handler->display->display_options['fields']['nothing']['table'] = 'views';
  $handler->display->display_options['fields']['nothing']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing']['label'] = 'Created / Changed';
  $handler->display->display_options['fields']['nothing']['alter']['text'] = '<time datetime="[entity_created_1] " title="Created">[entity_created]</time>
<br />
<time datetime="[entity_changed_1] " title="Changed">[entity_changed]</time>';
  $handler->display->display_options['defaults']['sorts'] = FALSE;
  /* Sort criterion: Search: Relevance */
  $handler->display->display_options['sorts']['search_api_relevance']['id'] = 'search_api_relevance';
  $handler->display->display_options['sorts']['search_api_relevance']['table'] = 'search_api_index_sharedcontent';
  $handler->display->display_options['sorts']['search_api_relevance']['field'] = 'search_api_relevance';
  $handler->display->display_options['sorts']['search_api_relevance']['order'] = 'DESC';
  $handler->display->display_options['defaults']['arguments'] = FALSE;
  /* Contextual filter: Search: More like this */
  $handler->display->display_options['arguments']['search_api_views_more_like_this']['id'] = 'search_api_views_more_like_this';
  $handler->display->display_options['arguments']['search_api_views_more_like_this']['table'] = 'search_api_index_sharedcontent';
  $handler->display->display_options['arguments']['search_api_views_more_like_this']['field'] = 'search_api_views_more_like_this';
  $handler->display->display_options['arguments']['search_api_views_more_like_this']['default_action'] = 'default';
  $handler->display->display_options['arguments']['search_api_views_more_like_this']['default_argument_type'] = 'sharedcontent_index_selected';
  $handler->display->display_options['arguments']['search_api_views_more_like_this']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['search_api_views_more_like_this']['fields'] = array(
    'title' => 'title',
    'keywords' => 'keywords',
    'tags' => 'tags',
  );
  $handler->display->display_options['defaults']['filter_groups'] = FALSE;
  $handler->display->display_options['defaults']['filters'] = FALSE;
  /* Filter criterion: Shared Content: Is selected local content */
  $handler->display->display_options['filters']['self_filter']['id'] = 'self_filter';
  $handler->display->display_options['filters']['self_filter']['table'] = 'search_api_index_sharedcontent';
  $handler->display->display_options['filters']['self_filter']['field'] = 'self_filter';
  $handler->display->display_options['filters']['self_filter']['value'] = '0';
  $handler->display->display_options['filters']['self_filter']['group'] = 1;
  /* Filter criterion: Indexed Shared Content Index: Status */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'search_api_index_sharedcontent';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['operator'] = '<>';
  $handler->display->display_options['filters']['status']['value'] = array(
    0 => '0',
  );
  $handler->display->display_options['filters']['status']['group'] = 1;
  /* Filter criterion: Indexed Shared Content Index: Site */
  $handler->display->display_options['filters']['connection_name']['id'] = 'connection_name';
  $handler->display->display_options['filters']['connection_name']['table'] = 'search_api_index_sharedcontent';
  $handler->display->display_options['filters']['connection_name']['field'] = 'connection_name';
  $handler->display->display_options['filters']['connection_name']['value'] = array();
  $handler->display->display_options['filters']['connection_name']['group'] = 1;
  $handler->display->display_options['filters']['connection_name']['exposed'] = TRUE;
  $handler->display->display_options['filters']['connection_name']['expose']['operator_id'] = 'connection_name_op';
  $handler->display->display_options['filters']['connection_name']['expose']['label'] = 'Origin';
  $handler->display->display_options['filters']['connection_name']['expose']['operator'] = 'connection_name_op';
  $handler->display->display_options['filters']['connection_name']['expose']['identifier'] = 'connection_name';
  $handler->display->display_options['filters']['connection_name']['expose']['remember'] = TRUE;
  $handler->display->display_options['filters']['connection_name']['expose']['multiple'] = TRUE;
  $handler->display->display_options['filters']['connection_name']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    4 => 0,
  );
  $handler->display->display_options['filters']['connection_name']['expose']['reduce'] = 0;
  /* Filter criterion: Indexed Shared Content Index: Entity_bundle */
  $handler->display->display_options['filters']['entity_bundle']['id'] = 'entity_bundle';
  $handler->display->display_options['filters']['entity_bundle']['table'] = 'search_api_index_sharedcontent';
  $handler->display->display_options['filters']['entity_bundle']['field'] = 'entity_bundle';
  $handler->display->display_options['filters']['entity_bundle']['value'] = array();
  $handler->display->display_options['filters']['entity_bundle']['group'] = 1;
  $handler->display->display_options['filters']['entity_bundle']['exposed'] = TRUE;
  $handler->display->display_options['filters']['entity_bundle']['expose']['operator_id'] = 'entity_bundle_op';
  $handler->display->display_options['filters']['entity_bundle']['expose']['label'] = 'Type';
  $handler->display->display_options['filters']['entity_bundle']['expose']['operator'] = 'entity_bundle_op';
  $handler->display->display_options['filters']['entity_bundle']['expose']['identifier'] = 'entity_bundle';
  $handler->display->display_options['filters']['entity_bundle']['expose']['remember'] = TRUE;
  $handler->display->display_options['filters']['entity_bundle']['expose']['multiple'] = TRUE;
  $handler->display->display_options['filters']['entity_bundle']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    4 => 0,
  );
  $handler->display->display_options['filters']['entity_bundle']['expose']['reduce'] = 0;
  $handler->display->display_options['inherit_panels_path'] = '1';
  $export['sharedcontent_remote_content'] = $view;

  return $export;
}
