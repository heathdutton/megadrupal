<?php
/**
 * @file
 * Provide default_views for breadcrumb2.
 */

/**
 * Implements hook_views_default_views().
 */
function breadcrumb2_views_default_views() {
  $view = new view();
  $view->name = 'breadcrumbs';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'breadcrumb';
  $view->human_name = 'Breadcrumbs';
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
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '30';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['pager']['options']['id'] = '0';
  $handler->display->display_options['pager']['options']['quantity'] = '9';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'bid' => 'bid',
  );
  $handler->display->display_options['style_options']['default'] = '-1';
  $handler->display->display_options['style_options']['info'] = array(
    'bid' => array(
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
  $handler->display->display_options['empty']['area']['empty'] = TRUE;
  $handler->display->display_options['empty']['area']['content'] = 'No breadcrumbs available.';
  $handler->display->display_options['empty']['area']['format'] = 'filtered_html';
  /* Field: Breadcrumb: path */
  $handler->display->display_options['fields']['path']['id'] = 'path';
  $handler->display->display_options['fields']['path']['table'] = 'breadcrumb';
  $handler->display->display_options['fields']['path']['field'] = 'path';
  /* Field: Breadcrumb: Breadcrumb Link */
  $handler->display->display_options['fields']['link']['id'] = 'link';
  $handler->display->display_options['fields']['link']['table'] = 'field_data_link';
  $handler->display->display_options['fields']['link']['field'] = 'link';
  $handler->display->display_options['fields']['link']['click_sort_column'] = 'url';
  $handler->display->display_options['fields']['link']['delta_offset'] = '0';
  $handler->display->display_options['fields']['link']['separator'] = ' » ';
  /* Field: Breadcrumb: Delete Link */
  $handler->display->display_options['fields']['delete_breadcrumb']['id'] = 'delete_breadcrumb';
  $handler->display->display_options['fields']['delete_breadcrumb']['table'] = 'breadcrumb';
  $handler->display->display_options['fields']['delete_breadcrumb']['field'] = 'delete_breadcrumb';
  $handler->display->display_options['fields']['delete_breadcrumb']['label'] = '';
  $handler->display->display_options['fields']['delete_breadcrumb']['element_label_colon'] = FALSE;
  /* Field: Breadcrumb: Edit Link */
  $handler->display->display_options['fields']['edit_breadcrumb']['id'] = 'edit_breadcrumb';
  $handler->display->display_options['fields']['edit_breadcrumb']['table'] = 'breadcrumb';
  $handler->display->display_options['fields']['edit_breadcrumb']['field'] = 'edit_breadcrumb';
  $handler->display->display_options['fields']['edit_breadcrumb']['label'] = '';
  $handler->display->display_options['fields']['edit_breadcrumb']['element_label_colon'] = FALSE;
  /* Sort criterion: Breadcrumb: Breadcrumb ID */
  $handler->display->display_options['sorts']['bid']['id'] = 'bid';
  $handler->display->display_options['sorts']['bid']['table'] = 'breadcrumb';
  $handler->display->display_options['sorts']['bid']['field'] = 'bid';
  $handler->display->display_options['sorts']['bid']['order'] = 'DESC';
  /* Filter criterion: Breadcrumb: path */
  $handler->display->display_options['filters']['path']['id'] = 'path';
  $handler->display->display_options['filters']['path']['table'] = 'breadcrumb';
  $handler->display->display_options['filters']['path']['field'] = 'path';
  $handler->display->display_options['filters']['path']['operator'] = 'contains';
  $handler->display->display_options['filters']['path']['exposed'] = TRUE;
  $handler->display->display_options['filters']['path']['expose']['operator_id'] = 'path_op';
  $handler->display->display_options['filters']['path']['expose']['label'] = 'path';
  $handler->display->display_options['filters']['path']['expose']['operator'] = 'path_op';
  $handler->display->display_options['filters']['path']['expose']['identifier'] = 'path';
  $handler->display->display_options['filters']['path']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
  );

  $views[$view->name] = $view;  
  return $views;
}