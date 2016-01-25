<?php
/**
 * @file
 * leads.views_default.inc
 */

/**
 * Implements hook_views_default_views().
 */
function leads_views_default_views() {
  $export = array();

  $view = new view();
  $view->name = 'leads';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'node';
  $view->human_name = 'Leads';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Leads';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'create lead content';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '30';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'field_lead_name_first' => 'field_lead_name_first',
    'field_lead_last_name' => 'field_lead_last_name',
    'field_lead_email' => 'field_lead_email',
    'field_lead_phone' => 'field_lead_phone',
    'field_lead_age' => 'field_lead_age',
    'field_lead_address' => 'field_lead_address',
    'changed' => 'changed',
  );
  $handler->display->display_options['style_options']['default'] = 'changed';
  $handler->display->display_options['style_options']['info'] = array(
    'field_lead_name_first' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_lead_last_name' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_lead_email' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_lead_phone' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_lead_age' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_lead_address' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'changed' => array(
      'sortable' => 1,
      'default_sort_order' => 'desc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
  );
  /* Field: Content: First name */
  $handler->display->display_options['fields']['field_lead_name_first']['id'] = 'field_lead_name_first';
  $handler->display->display_options['fields']['field_lead_name_first']['table'] = 'field_data_field_lead_name_first';
  $handler->display->display_options['fields']['field_lead_name_first']['field'] = 'field_lead_name_first';
  /* Field: Content: Last name */
  $handler->display->display_options['fields']['field_lead_last_name']['id'] = 'field_lead_last_name';
  $handler->display->display_options['fields']['field_lead_last_name']['table'] = 'field_data_field_lead_last_name';
  $handler->display->display_options['fields']['field_lead_last_name']['field'] = 'field_lead_last_name';
  /* Field: Content: Email */
  $handler->display->display_options['fields']['field_lead_email']['id'] = 'field_lead_email';
  $handler->display->display_options['fields']['field_lead_email']['table'] = 'field_data_field_lead_email';
  $handler->display->display_options['fields']['field_lead_email']['field'] = 'field_lead_email';
  /* Field: Content: Phone */
  $handler->display->display_options['fields']['field_lead_phone']['id'] = 'field_lead_phone';
  $handler->display->display_options['fields']['field_lead_phone']['table'] = 'field_data_field_lead_phone';
  $handler->display->display_options['fields']['field_lead_phone']['field'] = 'field_lead_phone';
  /* Field: Content: Age */
  $handler->display->display_options['fields']['field_lead_age']['id'] = 'field_lead_age';
  $handler->display->display_options['fields']['field_lead_age']['table'] = 'field_data_field_lead_age';
  $handler->display->display_options['fields']['field_lead_age']['field'] = 'field_lead_age';
  $handler->display->display_options['fields']['field_lead_age']['settings'] = array(
    'thousand_separator' => ' ',
    'prefix_suffix' => 0,
  );
  /* Field: Content: Address */
  $handler->display->display_options['fields']['field_lead_address']['id'] = 'field_lead_address';
  $handler->display->display_options['fields']['field_lead_address']['table'] = 'field_data_field_lead_address';
  $handler->display->display_options['fields']['field_lead_address']['field'] = 'field_lead_address';
  /* Field: Content: Updated date */
  $handler->display->display_options['fields']['changed']['id'] = 'changed';
  $handler->display->display_options['fields']['changed']['table'] = 'node';
  $handler->display->display_options['fields']['changed']['field'] = 'changed';
  $handler->display->display_options['fields']['changed']['label'] = 'Updated';
  $handler->display->display_options['fields']['changed']['date_format'] = 'time ago';
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
    'lead' => 'lead',
  );

  /* Display: Leads */
  $handler = $view->new_display('page', 'Leads', 'leads_listing');
  $handler->display->display_options['path'] = 'leads';
  $export['leads'] = $view;

  return $export;
}
