<?php
/**
 * @file
 * cm_show_airings.views_default.inc
 */

/**
 * Implements hook_views_default_views().
 */
function cm_show_airings_views_default_views() {
  $export = array();

  $view = new view();
  $view->name = 'cm_cm_show_airings';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'cm_airing';
  $view->human_name = 'Show: Airings';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Airings';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'none';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['query']['options']['query_comment'] = FALSE;
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '25';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['pager']['options']['id'] = '0';
  $handler->display->display_options['style_plugin'] = 'table';
  /* Header: Global: Text area */
  $handler->display->display_options['header']['area']['id'] = 'area';
  $handler->display->display_options['header']['area']['table'] = 'views';
  $handler->display->display_options['header']['area']['field'] = 'area';
  $handler->display->display_options['header']['area']['content'] = '<a href="<?php print url(\'airing/add\'); ?>>Add Airing</a>';
  $handler->display->display_options['header']['area']['format'] = 'full_html';
  /* No results behavior: Global: Text area */
  $handler->display->display_options['empty']['area']['id'] = 'area';
  $handler->display->display_options['empty']['area']['table'] = 'views';
  $handler->display->display_options['empty']['area']['field'] = 'area';
  $handler->display->display_options['empty']['area']['content'] = '<div class="messages warning">This Show has no Airings.</div>';
  $handler->display->display_options['empty']['area']['format'] = 'full_html';
  /* Field: Airing: Airing ID */
  $handler->display->display_options['fields']['airing_id']['id'] = 'airing_id';
  $handler->display->display_options['fields']['airing_id']['table'] = 'cm_airing';
  $handler->display->display_options['fields']['airing_id']['field'] = 'airing_id';
  $handler->display->display_options['fields']['airing_id']['exclude'] = TRUE;
  /* Field: Broken/missing handler */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'cm_airing';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['title']['alter']['path'] = 'airing/[airing_id]';
  /* Field: Airing: Date */
  $handler->display->display_options['fields']['field_airing_date']['id'] = 'field_airing_date';
  $handler->display->display_options['fields']['field_airing_date']['table'] = 'field_data_field_airing_date';
  $handler->display->display_options['fields']['field_airing_date']['field'] = 'field_airing_date';
  $handler->display->display_options['fields']['field_airing_date']['settings'] = array(
    'format_type' => 'short',
    'fromto' => 'both',
    'multiple_number' => '',
    'multiple_from' => '',
    'multiple_to' => '',
    'show_repeat_rule' => 'hide',
  );
  $handler->display->display_options['fields']['field_airing_date']['delta_offset'] = '0';
  /* Field: Global: Custom text */
  $handler->display->display_options['fields']['nothing']['id'] = 'nothing';
  $handler->display->display_options['fields']['nothing']['table'] = 'views';
  $handler->display->display_options['fields']['nothing']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing']['label'] = 'Edit';
  $handler->display->display_options['fields']['nothing']['alter']['text'] = 'Edit';
  $handler->display->display_options['fields']['nothing']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['nothing']['alter']['path'] = 'airing/[airing_id]/edit';
  /* Contextual filter: Broken/missing handler */
  $handler->display->display_options['arguments']['field_airing_show_ref_nid']['id'] = 'field_airing_show_ref_nid';
  $handler->display->display_options['arguments']['field_airing_show_ref_nid']['table'] = 'field_data_field_airing_show_ref';
  $handler->display->display_options['arguments']['field_airing_show_ref_nid']['field'] = 'field_airing_show_ref_nid';
  $handler->display->display_options['arguments']['field_airing_show_ref_nid']['default_action'] = 'empty';
  $handler->display->display_options['arguments']['field_airing_show_ref_nid']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['field_airing_show_ref_nid']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['field_airing_show_ref_nid']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['field_airing_show_ref_nid']['summary_options']['items_per_page'] = '25';
  $export['cm_cm_show_airings'] = $view;

  return $export;
}
