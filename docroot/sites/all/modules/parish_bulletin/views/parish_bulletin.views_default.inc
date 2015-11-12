<?php
/**
 * @file
 * Parish bulletin module default views.
 */

/**
 * Implements hook_views_default_views().
 */
function parish_bulletin_views_default_views() {
  $views = array();

  /* BEGIN DEFINITION: parish_bulletins ------------------------------------- */

  $view = new view();
  $view->name = 'parish_bulletins';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'node';
  $view->human_name = 'Parish Bulletins';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Bulletins';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['query']['options']['query_comment'] = FALSE;
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '7';
  $handler->display->display_options['style_plugin'] = 'table';
  /* Field: Content: Date */
  $handler->display->display_options['fields']['field_bulletin_date']['id'] = 'field_bulletin_date';
  $handler->display->display_options['fields']['field_bulletin_date']['table'] = 'field_data_field_bulletin_date';
  $handler->display->display_options['fields']['field_bulletin_date']['field'] = 'field_bulletin_date';
  $handler->display->display_options['fields']['field_bulletin_date']['hide_alter_empty'] = FALSE;
  $handler->display->display_options['fields']['field_bulletin_date']['settings'] = array(
    'format_type' => 'month_day_year',
    'fromto' => 'both',
    'multiple_number' => '',
    'multiple_from' => '',
    'multiple_to' => '',
    'show_repeat_rule' => 'show',
  );
  /* Field: Content: Bulletin */
  $handler->display->display_options['fields']['field_bulletin']['id'] = 'field_bulletin';
  $handler->display->display_options['fields']['field_bulletin']['table'] = 'field_data_field_bulletin';
  $handler->display->display_options['fields']['field_bulletin']['field'] = 'field_bulletin';
  $handler->display->display_options['fields']['field_bulletin']['label'] = 'File Download';
  $handler->display->display_options['fields']['field_bulletin']['hide_alter_empty'] = FALSE;
  $handler->display->display_options['fields']['field_bulletin']['click_sort_column'] = 'fid';
  /* Field: Content: Bulletin */
  $handler->display->display_options['fields']['field_bulletin_1']['id'] = 'field_bulletin_1';
  $handler->display->display_options['fields']['field_bulletin_1']['table'] = 'field_data_field_bulletin';
  $handler->display->display_options['fields']['field_bulletin_1']['field'] = 'field_bulletin';
  $handler->display->display_options['fields']['field_bulletin_1']['label'] = '';
  $handler->display->display_options['fields']['field_bulletin_1']['exclude'] = TRUE;
  $handler->display->display_options['fields']['field_bulletin_1']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_bulletin_1']['click_sort_column'] = 'fid';
  $handler->display->display_options['fields']['field_bulletin_1']['type'] = 'file_url_plain';
  /* Field: Content: Edit link */
  $handler->display->display_options['fields']['edit_node']['id'] = 'edit_node';
  $handler->display->display_options['fields']['edit_node']['table'] = 'views_entity_node';
  $handler->display->display_options['fields']['edit_node']['field'] = 'edit_node';
  $handler->display->display_options['fields']['edit_node']['label'] = '';
  $handler->display->display_options['fields']['edit_node']['exclude'] = TRUE;
  $handler->display->display_options['fields']['edit_node']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['edit_node']['text'] = 'edit';
  /* Field: Content: Delete link */
  $handler->display->display_options['fields']['delete_node']['id'] = 'delete_node';
  $handler->display->display_options['fields']['delete_node']['table'] = 'views_entity_node';
  $handler->display->display_options['fields']['delete_node']['field'] = 'delete_node';
  $handler->display->display_options['fields']['delete_node']['label'] = '';
  $handler->display->display_options['fields']['delete_node']['exclude'] = TRUE;
  $handler->display->display_options['fields']['delete_node']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['delete_node']['text'] = 'delete';
  /* Field: Global: Custom text */
  $handler->display->display_options['fields']['nothing']['id'] = 'nothing';
  $handler->display->display_options['fields']['nothing']['table'] = 'views';
  $handler->display->display_options['fields']['nothing']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing']['label'] = 'Operations';
  $handler->display->display_options['fields']['nothing']['alter']['text'] = '<a href="[field_bulletin_1]">download</a> [edit_node] [delete_node]';
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
  $handler->display->display_options['filters']['status']['group'] = 0;
  $handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
  /* Filter criterion: Content: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'node';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'bulletin' => 'bulletin',
  );
  /* Filter criterion: Content: Date (field_bulletin_date) */
  $handler->display->display_options['filters']['field_bulletin_date_value']['id'] = 'field_bulletin_date_value';
  $handler->display->display_options['filters']['field_bulletin_date_value']['table'] = 'field_data_field_bulletin_date';
  $handler->display->display_options['filters']['field_bulletin_date_value']['field'] = 'field_bulletin_date_value';
  $handler->display->display_options['filters']['field_bulletin_date_value']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_bulletin_date_value']['expose']['operator_id'] = 'field_bulletin_date_value_op';
  $handler->display->display_options['filters']['field_bulletin_date_value']['expose']['label'] = 'Find Bulletin from Date';
  $handler->display->display_options['filters']['field_bulletin_date_value']['expose']['operator'] = 'field_bulletin_date_value_op';
  $handler->display->display_options['filters']['field_bulletin_date_value']['expose']['identifier'] = 'field_bulletin_date_value';
  $handler->display->display_options['filters']['field_bulletin_date_value']['form_type'] = 'date_popup';

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['path'] = 'bulletins';
  $handler->display->display_options['menu']['type'] = 'normal';
  $handler->display->display_options['menu']['title'] = 'Bulletins';
  $handler->display->display_options['menu']['name'] = 'main-menu';

  /* Display: Feed */
  $handler = $view->new_display('feed', 'Feed', 'feed');
  $handler->display->display_options['pager']['type'] = 'some';
  $handler->display->display_options['style_plugin'] = 'rss';
  $handler->display->display_options['row_plugin'] = 'node_rss';
  $handler->display->display_options['path'] = 'bulletins.xml';
  $handler->display->display_options['displays'] = array(
    'default' => 'default',
    'page' => 'page',
  );

  /* Display: Block */
  $handler = $view->new_display('block', 'Block', 'block');
  $handler->display->display_options['defaults']['use_more'] = FALSE;
  $handler->display->display_options['use_more'] = TRUE;
  $handler->display->display_options['defaults']['use_more_always'] = FALSE;
  $handler->display->display_options['defaults']['use_more_always'] = FALSE;
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['defaults']['use_more_text'] = FALSE;
  $handler->display->display_options['use_more_text'] = 'more bulletins';
  $handler->display->display_options['defaults']['pager'] = FALSE;
  $handler->display->display_options['pager']['type'] = 'some';
  $handler->display->display_options['pager']['options']['items_per_page'] = '4';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['defaults']['style_plugin'] = FALSE;
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['defaults']['style_options'] = FALSE;
  $handler->display->display_options['defaults']['row_plugin'] = FALSE;
  $handler->display->display_options['defaults']['row_options'] = FALSE;
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: Content: Bulletin */
  $handler->display->display_options['fields']['field_bulletin']['id'] = 'field_bulletin';
  $handler->display->display_options['fields']['field_bulletin']['table'] = 'field_data_field_bulletin';
  $handler->display->display_options['fields']['field_bulletin']['field'] = 'field_bulletin';
  $handler->display->display_options['fields']['field_bulletin']['label'] = '';
  $handler->display->display_options['fields']['field_bulletin']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_bulletin']['hide_alter_empty'] = FALSE;
  $handler->display->display_options['fields']['field_bulletin']['click_sort_column'] = 'fid';
  $handler->display->display_options['defaults']['filter_groups'] = FALSE;
  $handler->display->display_options['defaults']['filters'] = FALSE;
  /* Filter criterion: Content: Published */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'node';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = 1;
  $handler->display->display_options['filters']['status']['group'] = 0;
  $handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
  /* Filter criterion: Content: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'node';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'bulletin' => 'bulletin',
  );
  $handler->display->display_options['block_caching'] = '8';

  /* END DEFINITION: parish_bulletins --------------------------------------- */

  // Add the view to the $views array.
  $views[$view->name] = $view;

  return $views;
}
