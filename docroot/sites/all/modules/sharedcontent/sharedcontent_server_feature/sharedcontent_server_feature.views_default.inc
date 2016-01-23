<?php
/**
 * @file
 * sharedcontent_server_feature.views_default.inc
 */

/**
 * Implements hook_views_default_views().
 */
function sharedcontent_server_feature_views_default_views() {
  $export = array();

  $view = new view();
  $view->name = 'assignments_inspection';
  $view->description = '';
  $view->tag = 'Shared Content';
  $view->base_table = 'sharedcontent_assignment';
  $view->human_name = 'Assingments Inspection';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Assignments inspection';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'administer sharedcontent';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '25';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['pager']['options']['id'] = '0';
  $handler->display->display_options['pager']['options']['quantity'] = '9';
  $handler->display->display_options['pager']['options']['expose']['items_per_page'] = TRUE;
  $handler->display->display_options['pager']['options']['expose']['items_per_page_options'] = '10,25,50,100,250,500';
  $handler->display->display_options['pager']['options']['expose']['offset'] = TRUE;
  $handler->display->display_options['style_plugin'] = 'table';
  /* Field: Shared Content Assignment: Uuid */
  $handler->display->display_options['fields']['uuid']['id'] = 'uuid';
  $handler->display->display_options['fields']['uuid']['table'] = 'sharedcontent_assignment';
  $handler->display->display_options['fields']['uuid']['field'] = 'uuid';
  /* Field: Shared Content Assignment: Bundle */
  $handler->display->display_options['fields']['origin']['id'] = 'origin';
  $handler->display->display_options['fields']['origin']['table'] = 'sharedcontent_assignment';
  $handler->display->display_options['fields']['origin']['field'] = 'origin';
  $handler->display->display_options['fields']['origin']['label'] = 'Origin';
  /* Field: Shared Content Assignment: Url */
  $handler->display->display_options['fields']['url']['id'] = 'url';
  $handler->display->display_options['fields']['url']['table'] = 'sharedcontent_assignment';
  $handler->display->display_options['fields']['url']['field'] = 'url';
  /* Field: Shared Content Assignment: Source */
  $handler->display->display_options['fields']['source']['id'] = 'source';
  $handler->display->display_options['fields']['source']['table'] = 'sharedcontent_assignment';
  $handler->display->display_options['fields']['source']['field'] = 'source';
  /* Field: Shared Content Assignment: Target */
  $handler->display->display_options['fields']['target']['id'] = 'target';
  $handler->display->display_options['fields']['target']['table'] = 'sharedcontent_assignment';
  $handler->display->display_options['fields']['target']['field'] = 'target';
  /* Field: Shared Content Assignment: Status */
  $handler->display->display_options['fields']['status']['id'] = 'status';
  $handler->display->display_options['fields']['status']['table'] = 'sharedcontent_assignment';
  $handler->display->display_options['fields']['status']['field'] = 'status';
  /* Field: Shared Content Assignment: Created */
  $handler->display->display_options['fields']['created']['id'] = 'created';
  $handler->display->display_options['fields']['created']['table'] = 'sharedcontent_assignment';
  $handler->display->display_options['fields']['created']['field'] = 'created';
  $handler->display->display_options['fields']['created']['date_format'] = 'short';
  /* Field: Shared Content Assignment: Changed */
  $handler->display->display_options['fields']['changed']['id'] = 'changed';
  $handler->display->display_options['fields']['changed']['table'] = 'sharedcontent_assignment';
  $handler->display->display_options['fields']['changed']['field'] = 'changed';
  $handler->display->display_options['fields']['changed']['date_format'] = 'short';
  /* Sort criterion: Shared Content Assignment: Changed */
  $handler->display->display_options['sorts']['changed']['id'] = 'changed';
  $handler->display->display_options['sorts']['changed']['table'] = 'sharedcontent_assignment';
  $handler->display->display_options['sorts']['changed']['field'] = 'changed';
  $handler->display->display_options['sorts']['changed']['order'] = 'DESC';
  $handler->display->display_options['sorts']['changed']['exposed'] = TRUE;
  $handler->display->display_options['sorts']['changed']['expose']['label'] = 'Changed';
  /* Sort criterion: Shared Content Assignment: Created */
  $handler->display->display_options['sorts']['created']['id'] = 'created';
  $handler->display->display_options['sorts']['created']['table'] = 'sharedcontent_assignment';
  $handler->display->display_options['sorts']['created']['field'] = 'created';
  $handler->display->display_options['sorts']['created']['order'] = 'DESC';
  $handler->display->display_options['sorts']['created']['exposed'] = TRUE;
  $handler->display->display_options['sorts']['created']['expose']['label'] = 'Created';
  /* Filter criterion: Shared Content Assignment: Uuid */
  $handler->display->display_options['filters']['uuid']['id'] = 'uuid';
  $handler->display->display_options['filters']['uuid']['table'] = 'sharedcontent_assignment';
  $handler->display->display_options['filters']['uuid']['field'] = 'uuid';
  $handler->display->display_options['filters']['uuid']['group'] = 1;
  $handler->display->display_options['filters']['uuid']['exposed'] = TRUE;
  $handler->display->display_options['filters']['uuid']['expose']['operator_id'] = 'uuid_op';
  $handler->display->display_options['filters']['uuid']['expose']['label'] = 'Uuid';
  $handler->display->display_options['filters']['uuid']['expose']['operator'] = 'uuid_op';
  $handler->display->display_options['filters']['uuid']['expose']['identifier'] = 'uuid';
  $handler->display->display_options['filters']['uuid']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    4 => 0,
  );
  /* Filter criterion: Shared Content Assignment: Bundle */
  $handler->display->display_options['filters']['origin']['id'] = 'origin';
  $handler->display->display_options['filters']['origin']['table'] = 'sharedcontent_assignment';
  $handler->display->display_options['filters']['origin']['field'] = 'origin';
  $handler->display->display_options['filters']['origin']['group'] = 1;
  $handler->display->display_options['filters']['origin']['exposed'] = TRUE;
  $handler->display->display_options['filters']['origin']['expose']['operator_id'] = 'origin_op';
  $handler->display->display_options['filters']['origin']['expose']['label'] = 'Origin';
  $handler->display->display_options['filters']['origin']['expose']['operator'] = 'origin_op';
  $handler->display->display_options['filters']['origin']['expose']['identifier'] = 'origin';
  $handler->display->display_options['filters']['origin']['expose']['multiple'] = TRUE;
  $handler->display->display_options['filters']['origin']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    4 => 0,
  );
  /* Filter criterion: Shared Content Assignment: Url */
  $handler->display->display_options['filters']['url']['id'] = 'url';
  $handler->display->display_options['filters']['url']['table'] = 'sharedcontent_assignment';
  $handler->display->display_options['filters']['url']['field'] = 'url';
  $handler->display->display_options['filters']['url']['group'] = 1;
  $handler->display->display_options['filters']['url']['exposed'] = TRUE;
  $handler->display->display_options['filters']['url']['expose']['operator_id'] = 'url_op';
  $handler->display->display_options['filters']['url']['expose']['label'] = 'Url';
  $handler->display->display_options['filters']['url']['expose']['operator'] = 'url_op';
  $handler->display->display_options['filters']['url']['expose']['identifier'] = 'url';
  $handler->display->display_options['filters']['url']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    4 => 0,
  );
  /* Filter criterion: Shared Content Assignment: Status */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'sharedcontent_assignment';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['group'] = 1;
  $handler->display->display_options['filters']['status']['exposed'] = TRUE;
  $handler->display->display_options['filters']['status']['expose']['operator_id'] = 'status_op';
  $handler->display->display_options['filters']['status']['expose']['label'] = 'Status';
  $handler->display->display_options['filters']['status']['expose']['operator'] = 'status_op';
  $handler->display->display_options['filters']['status']['expose']['identifier'] = 'status';
  $handler->display->display_options['filters']['status']['expose']['multiple'] = TRUE;
  $handler->display->display_options['filters']['status']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    4 => 0,
  );
  /* Filter criterion: Shared Content Assignment: Source */
  $handler->display->display_options['filters']['source']['id'] = 'source';
  $handler->display->display_options['filters']['source']['table'] = 'sharedcontent_assignment';
  $handler->display->display_options['filters']['source']['field'] = 'source';
  $handler->display->display_options['filters']['source']['group'] = 1;
  $handler->display->display_options['filters']['source']['exposed'] = TRUE;
  $handler->display->display_options['filters']['source']['expose']['operator_id'] = 'source_op';
  $handler->display->display_options['filters']['source']['expose']['label'] = 'Source';
  $handler->display->display_options['filters']['source']['expose']['operator'] = 'source_op';
  $handler->display->display_options['filters']['source']['expose']['identifier'] = 'source';
  $handler->display->display_options['filters']['source']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    4 => 0,
  );
  /* Filter criterion: Shared Content Assignment: Target */
  $handler->display->display_options['filters']['target']['id'] = 'target';
  $handler->display->display_options['filters']['target']['table'] = 'sharedcontent_assignment';
  $handler->display->display_options['filters']['target']['field'] = 'target';
  $handler->display->display_options['filters']['target']['group'] = 1;
  $handler->display->display_options['filters']['target']['exposed'] = TRUE;
  $handler->display->display_options['filters']['target']['expose']['operator_id'] = 'target_op';
  $handler->display->display_options['filters']['target']['expose']['label'] = 'Target';
  $handler->display->display_options['filters']['target']['expose']['operator'] = 'target_op';
  $handler->display->display_options['filters']['target']['expose']['identifier'] = 'target';
  $handler->display->display_options['filters']['target']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    4 => 0,
  );
  /* Filter criterion: Shared Content Assignment: Created */
  $handler->display->display_options['filters']['created']['id'] = 'created';
  $handler->display->display_options['filters']['created']['table'] = 'sharedcontent_assignment';
  $handler->display->display_options['filters']['created']['field'] = 'created';
  $handler->display->display_options['filters']['created']['group'] = 1;
  $handler->display->display_options['filters']['created']['exposed'] = TRUE;
  $handler->display->display_options['filters']['created']['expose']['operator_id'] = 'created_op';
  $handler->display->display_options['filters']['created']['expose']['label'] = 'Created';
  $handler->display->display_options['filters']['created']['expose']['use_operator'] = TRUE;
  $handler->display->display_options['filters']['created']['expose']['operator'] = 'created_op';
  $handler->display->display_options['filters']['created']['expose']['identifier'] = 'created';
  $handler->display->display_options['filters']['created']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    4 => 0,
  );
  /* Filter criterion: Shared Content Assignment: Changed */
  $handler->display->display_options['filters']['changed']['id'] = 'changed';
  $handler->display->display_options['filters']['changed']['table'] = 'sharedcontent_assignment';
  $handler->display->display_options['filters']['changed']['field'] = 'changed';
  $handler->display->display_options['filters']['changed']['group'] = 1;
  $handler->display->display_options['filters']['changed']['exposed'] = TRUE;
  $handler->display->display_options['filters']['changed']['expose']['operator_id'] = 'changed_op';
  $handler->display->display_options['filters']['changed']['expose']['label'] = 'Changed';
  $handler->display->display_options['filters']['changed']['expose']['use_operator'] = TRUE;
  $handler->display->display_options['filters']['changed']['expose']['operator'] = 'changed_op';
  $handler->display->display_options['filters']['changed']['expose']['identifier'] = 'changed';
  $handler->display->display_options['filters']['changed']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    4 => 0,
  );

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['defaults']['hide_admin_links'] = FALSE;
  $handler->display->display_options['path'] = 'admin/reports/sharedcontent/assingments';
  $handler->display->display_options['menu']['type'] = 'normal';
  $handler->display->display_options['menu']['title'] = 'Shared Content Assignments';
  $handler->display->display_options['menu']['description'] = 'List of the assignments known to this system.';
  $handler->display->display_options['menu']['weight'] = '0';
  $handler->display->display_options['menu']['name'] = 'management';
  $handler->display->display_options['menu']['context'] = 0;
  $export['assignments_inspection'] = $view;

  $view = new view();
  $view->name = 'sharedcontent_index';
  $view->description = '';
  $view->tag = 'Shared Content';
  $view->base_table = 'sharedcontent_index';
  $view->human_name = 'Index Inspection';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Shared Content Index';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'administer sharedcontent';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['query']['options']['query_comment'] = FALSE;
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '25';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['pager']['options']['id'] = '0';
  $handler->display->display_options['pager']['options']['expose']['items_per_page'] = TRUE;
  $handler->display->display_options['pager']['options']['expose']['items_per_page_options'] = '10,25,50,100,250,500';
  $handler->display->display_options['pager']['options']['expose']['items_per_page_options_all'] = TRUE;
  $handler->display->display_options['pager']['options']['expose']['offset'] = TRUE;
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'uuid' => 'uuid',
    'entity_id' => 'entity_id',
    'entity_type' => 'entity_type',
    'entity_bundle' => 'entity_bundle',
    'title' => 'title',
    'keywords' => 'keywords',
    'status' => 'status',
    'url' => 'url',
    'created' => 'created',
    'changed' => 'changed',
  );
  $handler->display->display_options['style_options']['default'] = '-1';
  $handler->display->display_options['style_options']['info'] = array(
    'uuid' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'entity_id' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'entity_type' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'entity_bundle' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'title' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'keywords' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
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
    'url' => array(
      'sortable' => 0,
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
  );
  $handler->display->display_options['style_options']['sticky'] = TRUE;
  /* Field: Uuid */
  $handler->display->display_options['fields']['uuid']['id'] = 'uuid';
  $handler->display->display_options['fields']['uuid']['table'] = 'sharedcontent_index';
  $handler->display->display_options['fields']['uuid']['field'] = 'uuid';
  $handler->display->display_options['fields']['uuid']['ui_name'] = 'Uuid';
  $handler->display->display_options['fields']['uuid']['label'] = 'uuid';
  /* Field: Entity */
  $handler->display->display_options['fields']['entity_id']['id'] = 'entity_id';
  $handler->display->display_options['fields']['entity_id']['table'] = 'sharedcontent_index';
  $handler->display->display_options['fields']['entity_id']['field'] = 'entity_id';
  $handler->display->display_options['fields']['entity_id']['ui_name'] = 'Entity';
  $handler->display->display_options['fields']['entity_id']['label'] = 'entity_id';
  $handler->display->display_options['fields']['entity_id']['separator'] = '';
  /* Field: Type */
  $handler->display->display_options['fields']['entity_type']['id'] = 'entity_type';
  $handler->display->display_options['fields']['entity_type']['table'] = 'sharedcontent_index';
  $handler->display->display_options['fields']['entity_type']['field'] = 'entity_type';
  $handler->display->display_options['fields']['entity_type']['ui_name'] = 'Type';
  $handler->display->display_options['fields']['entity_type']['label'] = 'entity_type';
  /* Field: Bundle */
  $handler->display->display_options['fields']['entity_bundle']['id'] = 'entity_bundle';
  $handler->display->display_options['fields']['entity_bundle']['table'] = 'sharedcontent_index';
  $handler->display->display_options['fields']['entity_bundle']['field'] = 'entity_bundle';
  $handler->display->display_options['fields']['entity_bundle']['ui_name'] = 'Bundle';
  $handler->display->display_options['fields']['entity_bundle']['label'] = 'entity_bundle';
  /* Field: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'sharedcontent_index';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['ui_name'] = 'Title';
  $handler->display->display_options['fields']['title']['label'] = 'title';
  /* Field: Language */
  $handler->display->display_options['fields']['language']['id'] = 'language';
  $handler->display->display_options['fields']['language']['table'] = 'sharedcontent_index';
  $handler->display->display_options['fields']['language']['field'] = 'language';
  $handler->display->display_options['fields']['language']['ui_name'] = 'Language';
  /* Field: Translation set */
  $handler->display->display_options['fields']['translationset_id']['id'] = 'translationset_id';
  $handler->display->display_options['fields']['translationset_id']['table'] = 'sharedcontent_index';
  $handler->display->display_options['fields']['translationset_id']['field'] = 'translationset_id';
  $handler->display->display_options['fields']['translationset_id']['ui_name'] = 'Translation set';
  $handler->display->display_options['fields']['translationset_id']['hide_empty'] = TRUE;
  $handler->display->display_options['fields']['translationset_id']['separator'] = '';
  /* Field: Keywords */
  $handler->display->display_options['fields']['keywords']['id'] = 'keywords';
  $handler->display->display_options['fields']['keywords']['table'] = 'sharedcontent_index';
  $handler->display->display_options['fields']['keywords']['field'] = 'keywords';
  $handler->display->display_options['fields']['keywords']['ui_name'] = 'Keywords';
  $handler->display->display_options['fields']['keywords']['label'] = 'keywords';
  /* Field: Status */
  $handler->display->display_options['fields']['status']['id'] = 'status';
  $handler->display->display_options['fields']['status']['table'] = 'sharedcontent_index';
  $handler->display->display_options['fields']['status']['field'] = 'status';
  $handler->display->display_options['fields']['status']['ui_name'] = 'Status';
  $handler->display->display_options['fields']['status']['label'] = 'status';
  /* Field: URL */
  $handler->display->display_options['fields']['url']['id'] = 'url';
  $handler->display->display_options['fields']['url']['table'] = 'sharedcontent_index';
  $handler->display->display_options['fields']['url']['field'] = 'url';
  $handler->display->display_options['fields']['url']['ui_name'] = 'URL';
  $handler->display->display_options['fields']['url']['label'] = 'url';
  /* Field: Created */
  $handler->display->display_options['fields']['created']['id'] = 'created';
  $handler->display->display_options['fields']['created']['table'] = 'sharedcontent_index';
  $handler->display->display_options['fields']['created']['field'] = 'created';
  $handler->display->display_options['fields']['created']['ui_name'] = 'Created';
  $handler->display->display_options['fields']['created']['label'] = 'created';
  $handler->display->display_options['fields']['created']['date_format'] = 'short';
  /* Field: Changed */
  $handler->display->display_options['fields']['changed']['id'] = 'changed';
  $handler->display->display_options['fields']['changed']['table'] = 'sharedcontent_index';
  $handler->display->display_options['fields']['changed']['field'] = 'changed';
  $handler->display->display_options['fields']['changed']['ui_name'] = 'Changed';
  $handler->display->display_options['fields']['changed']['label'] = 'changed';
  $handler->display->display_options['fields']['changed']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['changed']['date_format'] = 'short';
  /* Sort criterion: Shared Content Index: Changed */
  $handler->display->display_options['sorts']['changed']['id'] = 'changed';
  $handler->display->display_options['sorts']['changed']['table'] = 'sharedcontent_index';
  $handler->display->display_options['sorts']['changed']['field'] = 'changed';
  $handler->display->display_options['sorts']['changed']['order'] = 'DESC';
  $handler->display->display_options['sorts']['changed']['exposed'] = TRUE;
  $handler->display->display_options['sorts']['changed']['expose']['label'] = 'Changed';
  /* Filter criterion: Shared Content Index: Entity_id */
  $handler->display->display_options['filters']['entity_id']['id'] = 'entity_id';
  $handler->display->display_options['filters']['entity_id']['table'] = 'sharedcontent_index';
  $handler->display->display_options['filters']['entity_id']['field'] = 'entity_id';
  $handler->display->display_options['filters']['entity_id']['group'] = 1;
  $handler->display->display_options['filters']['entity_id']['exposed'] = TRUE;
  $handler->display->display_options['filters']['entity_id']['expose']['operator_id'] = 'entity_id_op';
  $handler->display->display_options['filters']['entity_id']['expose']['label'] = 'Id';
  $handler->display->display_options['filters']['entity_id']['expose']['use_operator'] = TRUE;
  $handler->display->display_options['filters']['entity_id']['expose']['operator'] = 'entity_id_op';
  $handler->display->display_options['filters']['entity_id']['expose']['identifier'] = 'entity_id';
  /* Filter criterion: Shared Content Index: Entity_type */
  $handler->display->display_options['filters']['entity_type']['id'] = 'entity_type';
  $handler->display->display_options['filters']['entity_type']['table'] = 'sharedcontent_index';
  $handler->display->display_options['filters']['entity_type']['field'] = 'entity_type';
  $handler->display->display_options['filters']['entity_type']['group'] = 1;
  $handler->display->display_options['filters']['entity_type']['exposed'] = TRUE;
  $handler->display->display_options['filters']['entity_type']['expose']['operator_id'] = 'entity_type_op';
  $handler->display->display_options['filters']['entity_type']['expose']['label'] = 'Type';
  $handler->display->display_options['filters']['entity_type']['expose']['use_operator'] = TRUE;
  $handler->display->display_options['filters']['entity_type']['expose']['operator'] = 'entity_type_op';
  $handler->display->display_options['filters']['entity_type']['expose']['identifier'] = 'entity_type';
  /* Filter criterion: Shared Content Index: Entity_bundle */
  $handler->display->display_options['filters']['entity_bundle']['id'] = 'entity_bundle';
  $handler->display->display_options['filters']['entity_bundle']['table'] = 'sharedcontent_index';
  $handler->display->display_options['filters']['entity_bundle']['field'] = 'entity_bundle';
  $handler->display->display_options['filters']['entity_bundle']['group'] = 1;
  $handler->display->display_options['filters']['entity_bundle']['exposed'] = TRUE;
  $handler->display->display_options['filters']['entity_bundle']['expose']['operator_id'] = 'entity_bundle_op';
  $handler->display->display_options['filters']['entity_bundle']['expose']['label'] = 'Bundle';
  $handler->display->display_options['filters']['entity_bundle']['expose']['operator'] = 'entity_bundle_op';
  $handler->display->display_options['filters']['entity_bundle']['expose']['identifier'] = 'entity_bundle';
  $handler->display->display_options['filters']['entity_bundle']['expose']['multiple'] = TRUE;
  $handler->display->display_options['filters']['entity_bundle']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    4 => 0,
  );
  /* Filter criterion: Shared Content Index: Status */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'sharedcontent_index';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['group'] = 1;
  $handler->display->display_options['filters']['status']['exposed'] = TRUE;
  $handler->display->display_options['filters']['status']['expose']['operator_id'] = 'status_op';
  $handler->display->display_options['filters']['status']['expose']['label'] = 'Status';
  $handler->display->display_options['filters']['status']['expose']['operator'] = 'status_op';
  $handler->display->display_options['filters']['status']['expose']['identifier'] = 'status';
  $handler->display->display_options['filters']['status']['expose']['multiple'] = TRUE;
  $handler->display->display_options['filters']['status']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    4 => 0,
  );
  /* Filter criterion: Shared Content Index: Changed */
  $handler->display->display_options['filters']['changed']['id'] = 'changed';
  $handler->display->display_options['filters']['changed']['table'] = 'sharedcontent_index';
  $handler->display->display_options['filters']['changed']['field'] = 'changed';
  $handler->display->display_options['filters']['changed']['operator'] = '>';
  $handler->display->display_options['filters']['changed']['group'] = 1;
  $handler->display->display_options['filters']['changed']['exposed'] = TRUE;
  $handler->display->display_options['filters']['changed']['expose']['operator_id'] = 'changed_op';
  $handler->display->display_options['filters']['changed']['expose']['label'] = 'Changed';
  $handler->display->display_options['filters']['changed']['expose']['operator'] = 'changed_op';
  $handler->display->display_options['filters']['changed']['expose']['identifier'] = 'changed';
  /* Filter criterion: Shared Content Index: Site */
  $handler->display->display_options['filters']['connection_name']['id'] = 'connection_name';
  $handler->display->display_options['filters']['connection_name']['table'] = 'sharedcontent_index';
  $handler->display->display_options['filters']['connection_name']['field'] = 'connection_name';
  $handler->display->display_options['filters']['connection_name']['exposed'] = TRUE;
  $handler->display->display_options['filters']['connection_name']['expose']['operator_id'] = 'connection_name_op';
  $handler->display->display_options['filters']['connection_name']['expose']['label'] = 'Site';
  $handler->display->display_options['filters']['connection_name']['expose']['operator'] = 'connection_name_op';
  $handler->display->display_options['filters']['connection_name']['expose']['identifier'] = 'connection_name';
  $handler->display->display_options['filters']['connection_name']['expose']['multiple'] = TRUE;
  $handler->display->display_options['filters']['connection_name']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    4 => 0,
  );

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'inspection');
  $handler->display->display_options['defaults']['hide_admin_links'] = FALSE;
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: Uuid */
  $handler->display->display_options['fields']['uuid']['id'] = 'uuid';
  $handler->display->display_options['fields']['uuid']['table'] = 'sharedcontent_index';
  $handler->display->display_options['fields']['uuid']['field'] = 'uuid';
  $handler->display->display_options['fields']['uuid']['ui_name'] = 'Uuid';
  $handler->display->display_options['fields']['uuid']['label'] = 'uuid';
  /* Field: Entity */
  $handler->display->display_options['fields']['entity_id']['id'] = 'entity_id';
  $handler->display->display_options['fields']['entity_id']['table'] = 'sharedcontent_index';
  $handler->display->display_options['fields']['entity_id']['field'] = 'entity_id';
  $handler->display->display_options['fields']['entity_id']['ui_name'] = 'Entity';
  $handler->display->display_options['fields']['entity_id']['label'] = 'entity_id';
  $handler->display->display_options['fields']['entity_id']['separator'] = '';
  /* Field: Type */
  $handler->display->display_options['fields']['entity_type']['id'] = 'entity_type';
  $handler->display->display_options['fields']['entity_type']['table'] = 'sharedcontent_index';
  $handler->display->display_options['fields']['entity_type']['field'] = 'entity_type';
  $handler->display->display_options['fields']['entity_type']['ui_name'] = 'Type';
  $handler->display->display_options['fields']['entity_type']['label'] = 'entity_type';
  /* Field: Bundle */
  $handler->display->display_options['fields']['entity_bundle']['id'] = 'entity_bundle';
  $handler->display->display_options['fields']['entity_bundle']['table'] = 'sharedcontent_index';
  $handler->display->display_options['fields']['entity_bundle']['field'] = 'entity_bundle';
  $handler->display->display_options['fields']['entity_bundle']['ui_name'] = 'Bundle';
  $handler->display->display_options['fields']['entity_bundle']['label'] = 'entity_bundle';
  /* Field: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'sharedcontent_index';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['ui_name'] = 'Title';
  $handler->display->display_options['fields']['title']['label'] = 'title';
  /* Field: Language */
  $handler->display->display_options['fields']['language']['id'] = 'language';
  $handler->display->display_options['fields']['language']['table'] = 'sharedcontent_index';
  $handler->display->display_options['fields']['language']['field'] = 'language';
  $handler->display->display_options['fields']['language']['ui_name'] = 'Language';
  /* Field: Translation set */
  $handler->display->display_options['fields']['translationset_id']['id'] = 'translationset_id';
  $handler->display->display_options['fields']['translationset_id']['table'] = 'sharedcontent_index';
  $handler->display->display_options['fields']['translationset_id']['field'] = 'translationset_id';
  $handler->display->display_options['fields']['translationset_id']['ui_name'] = 'Translation set';
  $handler->display->display_options['fields']['translationset_id']['hide_empty'] = TRUE;
  $handler->display->display_options['fields']['translationset_id']['separator'] = '';
  /* Field: Keywords */
  $handler->display->display_options['fields']['keywords']['id'] = 'keywords';
  $handler->display->display_options['fields']['keywords']['table'] = 'sharedcontent_index';
  $handler->display->display_options['fields']['keywords']['field'] = 'keywords';
  $handler->display->display_options['fields']['keywords']['ui_name'] = 'Keywords';
  $handler->display->display_options['fields']['keywords']['label'] = 'keywords';
  /* Field: Tags */
  $handler->display->display_options['fields']['tags']['id'] = 'tags';
  $handler->display->display_options['fields']['tags']['table'] = 'sharedcontent_index';
  $handler->display->display_options['fields']['tags']['field'] = 'tags';
  $handler->display->display_options['fields']['tags']['ui_name'] = 'Tags';
  $handler->display->display_options['fields']['tags']['label'] = 'tags';
  /* Field: Status */
  $handler->display->display_options['fields']['status']['id'] = 'status';
  $handler->display->display_options['fields']['status']['table'] = 'sharedcontent_index';
  $handler->display->display_options['fields']['status']['field'] = 'status';
  $handler->display->display_options['fields']['status']['ui_name'] = 'Status';
  $handler->display->display_options['fields']['status']['label'] = 'status';
  /* Field: URL */
  $handler->display->display_options['fields']['url']['id'] = 'url';
  $handler->display->display_options['fields']['url']['table'] = 'sharedcontent_index';
  $handler->display->display_options['fields']['url']['field'] = 'url';
  $handler->display->display_options['fields']['url']['ui_name'] = 'URL';
  $handler->display->display_options['fields']['url']['label'] = 'url';
  /* Field: Created */
  $handler->display->display_options['fields']['created']['id'] = 'created';
  $handler->display->display_options['fields']['created']['table'] = 'sharedcontent_index';
  $handler->display->display_options['fields']['created']['field'] = 'created';
  $handler->display->display_options['fields']['created']['ui_name'] = 'Created';
  $handler->display->display_options['fields']['created']['label'] = 'created';
  $handler->display->display_options['fields']['created']['date_format'] = 'short';
  /* Field: Changed */
  $handler->display->display_options['fields']['changed']['id'] = 'changed';
  $handler->display->display_options['fields']['changed']['table'] = 'sharedcontent_index';
  $handler->display->display_options['fields']['changed']['field'] = 'changed';
  $handler->display->display_options['fields']['changed']['ui_name'] = 'Changed';
  $handler->display->display_options['fields']['changed']['label'] = 'changed';
  $handler->display->display_options['fields']['changed']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['changed']['date_format'] = 'short';
  /* Field: Shared Content Index: Parent_uuid */
  $handler->display->display_options['fields']['parent_uuid']['id'] = 'parent_uuid';
  $handler->display->display_options['fields']['parent_uuid']['table'] = 'sharedcontent_index';
  $handler->display->display_options['fields']['parent_uuid']['field'] = 'parent_uuid';
  $handler->display->display_options['path'] = 'admin/reports/sharedcontent/index';
  $handler->display->display_options['menu']['type'] = 'normal';
  $handler->display->display_options['menu']['title'] = 'Shared Content Index';
  $handler->display->display_options['menu']['description'] = 'Shows the records stored inside of the Shared Content index';
  $handler->display->display_options['menu']['weight'] = '0';
  $handler->display->display_options['menu']['name'] = 'management';
  $handler->display->display_options['menu']['context'] = 0;
  $export['sharedcontent_index'] = $view;

  $view = new view();
  $view->name = 'sharedcontent_rebuild';
  $view->description = 'This view is used to display the content that can be filtered by various criteria and also can be used to update index.';
  $view->tag = 'Shared Content';
  $view->base_table = 'node';
  $view->human_name = 'Index Rebuilding';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Shared Content Manual Index Rebuilding';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['query']['options']['query_comment'] = FALSE;
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '25';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['pager']['options']['id'] = '0';
  $handler->display->display_options['pager']['options']['expose']['items_per_page'] = TRUE;
  $handler->display->display_options['pager']['options']['expose']['items_per_page_options'] = '10,25,50,100,250,500';
  $handler->display->display_options['pager']['options']['expose']['offset'] = TRUE;
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'title' => 'title',
    'type' => 'type',
    'created' => 'created',
    'changed' => 'changed',
  );
  $handler->display->display_options['style_options']['default'] = '-1';
  $handler->display->display_options['style_options']['info'] = array(
    'title' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'type' => array(
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
  );
  /* Header: Global: Text area */
  $handler->display->display_options['header']['area']['id'] = 'area';
  $handler->display->display_options['header']['area']['table'] = 'views';
  $handler->display->display_options['header']['area']['field'] = 'area';
  $handler->display->display_options['header']['area']['empty'] = TRUE;
  $handler->display->display_options['header']['area']['content'] = 'Add content (nodes) to the Shared Content index. This view shows all local content and is intended for initial mass provisioning of the index. The actual indexing will be done in the background, so make sure that cron gets executed. Regular index maintenance happens with rules on node_save / entity_save. Other entities are supported by the Shared Content index, but not by the UI yet. Marking content as deleted possibly leads to unassignment on client systems - potential data loss. This action is unneded in regular cases.';
  $handler->display->display_options['header']['area']['format'] = 'filtered_html';
  /* Field: Bulk operations: Content */
  $handler->display->display_options['fields']['views_bulk_operations']['id'] = 'views_bulk_operations';
  $handler->display->display_options['fields']['views_bulk_operations']['table'] = 'node';
  $handler->display->display_options['fields']['views_bulk_operations']['field'] = 'views_bulk_operations';
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['display_type'] = '1';
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['enable_select_all_pages'] = 1;
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['force_single'] = 0;
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['display_result'] = 1;
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['entity_load_capacity'] = '10';
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_operations'] = array(
    'action::node_assign_owner_action' => array(
      'selected' => 0,
      'use_queue' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
    ),
    'action::views_bulk_operations_delete_item' => array(
      'selected' => 0,
      'use_queue' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
    ),
    'action::system_message_action' => array(
      'selected' => 0,
      'use_queue' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
    ),
    'action::views_bulk_operations_script_action' => array(
      'selected' => 0,
      'use_queue' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
    ),
    'action::flag_node_action' => array(
      'selected' => 0,
      'use_queue' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
    ),
    'action::node_make_sticky_action' => array(
      'selected' => 0,
      'use_queue' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
    ),
    'action::node_make_unsticky_action' => array(
      'selected' => 0,
      'use_queue' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
    ),
    'action::views_bulk_operations_modify_action' => array(
      'selected' => 0,
      'use_queue' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
      'settings' => array(
        'show_all_tokens' => 1,
        'display_values' => array(
          '_all_' => '_all_',
        ),
      ),
    ),
    'action::views_bulk_operations_argument_selector_action' => array(
      'selected' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
      'settings' => array(
        'url' => '',
      ),
    ),
    'action::node_promote_action' => array(
      'selected' => 0,
      'use_queue' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
    ),
    'action::node_publish_action' => array(
      'selected' => 0,
      'use_queue' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
    ),
    'action::system_goto_action' => array(
      'selected' => 0,
      'use_queue' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
    ),
    'action::node_unpromote_action' => array(
      'selected' => 0,
      'use_queue' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
    ),
    'action::node_save_action' => array(
      'selected' => 0,
      'use_queue' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
    ),
    'action::system_send_email_action' => array(
      'selected' => 0,
      'use_queue' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
    ),
    'rules_component::rules_sharedcontent_create_update_node_index' => array(
      'selected' => 1,
      'use_queue' => 1,
      'skip_confirmation' => 0,
      'override_label' => 1,
      'label' => 'Create or update index',
    ),
    'rules_component::rules_sharedcontent_index_delete_node' => array(
      'selected' => 1,
      'use_queue' => 1,
      'skip_confirmation' => 0,
      'override_label' => 1,
      'label' => 'Mark index as deleted',
    ),
    'action::node_unpublish_action' => array(
      'selected' => 0,
      'use_queue' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
    ),
    'action::node_unpublish_by_keyword_action' => array(
      'selected' => 0,
      'use_queue' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
    ),
  );
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = '';
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
  /* Field: Content: Type */
  $handler->display->display_options['fields']['type']['id'] = 'type';
  $handler->display->display_options['fields']['type']['table'] = 'node';
  $handler->display->display_options['fields']['type']['field'] = 'type';
  $handler->display->display_options['fields']['type']['label'] = 'Content Type';
  /* Field: Content: Post date */
  $handler->display->display_options['fields']['created']['id'] = 'created';
  $handler->display->display_options['fields']['created']['table'] = 'node';
  $handler->display->display_options['fields']['created']['field'] = 'created';
  $handler->display->display_options['fields']['created']['date_format'] = 'short';
  /* Field: Content: Updated date */
  $handler->display->display_options['fields']['changed']['id'] = 'changed';
  $handler->display->display_options['fields']['changed']['table'] = 'node';
  $handler->display->display_options['fields']['changed']['field'] = 'changed';
  $handler->display->display_options['fields']['changed']['date_format'] = 'short';
  /* Sort criterion: Content: Post date */
  $handler->display->display_options['sorts']['created']['id'] = 'created';
  $handler->display->display_options['sorts']['created']['table'] = 'node';
  $handler->display->display_options['sorts']['created']['field'] = 'created';
  $handler->display->display_options['sorts']['created']['order'] = 'DESC';
  $handler->display->display_options['sorts']['created']['exposed'] = TRUE;
  $handler->display->display_options['sorts']['created']['expose']['label'] = 'Post date';
  $handler->display->display_options['sorts']['created']['granularity'] = 'minute';
  /* Filter criterion: Content: Published */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'node';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = '1';
  $handler->display->display_options['filters']['status']['group'] = 1;
  $handler->display->display_options['filters']['status']['exposed'] = TRUE;
  $handler->display->display_options['filters']['status']['expose']['operator_id'] = '';
  $handler->display->display_options['filters']['status']['expose']['label'] = 'Published';
  $handler->display->display_options['filters']['status']['expose']['operator'] = 'status_op';
  $handler->display->display_options['filters']['status']['expose']['identifier'] = 'status';
  $handler->display->display_options['filters']['status']['expose']['required'] = TRUE;
  /* Filter criterion: Content: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'node';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['exposed'] = TRUE;
  $handler->display->display_options['filters']['type']['expose']['operator_id'] = 'type_op';
  $handler->display->display_options['filters']['type']['expose']['label'] = 'Type';
  $handler->display->display_options['filters']['type']['expose']['use_operator'] = TRUE;
  $handler->display->display_options['filters']['type']['expose']['operator'] = 'type_op';
  $handler->display->display_options['filters']['type']['expose']['identifier'] = 'type';
  $handler->display->display_options['filters']['type']['expose']['remember'] = TRUE;
  $handler->display->display_options['filters']['type']['expose']['multiple'] = TRUE;
  /* Filter criterion: Content: Post date */
  $handler->display->display_options['filters']['created']['id'] = 'created';
  $handler->display->display_options['filters']['created']['table'] = 'node';
  $handler->display->display_options['filters']['created']['field'] = 'created';
  $handler->display->display_options['filters']['created']['exposed'] = TRUE;
  $handler->display->display_options['filters']['created']['expose']['operator_id'] = 'created_op';
  $handler->display->display_options['filters']['created']['expose']['label'] = 'Post date';
  $handler->display->display_options['filters']['created']['expose']['use_operator'] = TRUE;
  $handler->display->display_options['filters']['created']['expose']['operator'] = 'created_op';
  $handler->display->display_options['filters']['created']['expose']['identifier'] = 'created';
  /* Filter criterion: Content: Updated date */
  $handler->display->display_options['filters']['changed']['id'] = 'changed';
  $handler->display->display_options['filters']['changed']['table'] = 'node';
  $handler->display->display_options['filters']['changed']['field'] = 'changed';
  $handler->display->display_options['filters']['changed']['exposed'] = TRUE;
  $handler->display->display_options['filters']['changed']['expose']['operator_id'] = 'changed_op';
  $handler->display->display_options['filters']['changed']['expose']['label'] = 'Updated date';
  $handler->display->display_options['filters']['changed']['expose']['use_operator'] = TRUE;
  $handler->display->display_options['filters']['changed']['expose']['operator'] = 'changed_op';
  $handler->display->display_options['filters']['changed']['expose']['identifier'] = 'changed';

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['defaults']['hide_admin_links'] = FALSE;
  $handler->display->display_options['path'] = 'admin/config/services/sharedcontent/server/rebuild';
  $handler->display->display_options['menu']['type'] = 'tab';
  $handler->display->display_options['menu']['title'] = 'Rebuild Index';
  $handler->display->display_options['menu']['weight'] = '0';
  $handler->display->display_options['menu']['name'] = 'management';
  $handler->display->display_options['menu']['context'] = 0;
  $export['sharedcontent_rebuild'] = $view;

  return $export;
}
