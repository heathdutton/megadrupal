<?php

/**
 * @file
 * Contains hooks for view plugins.
 */


/**
 * Implements hook_views_default_views().
 */
function total_subscription_views_default_views() {
  $view = new view();
  $view->name = 'total_subscription';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'total_subscription';
  $view->human_name = 'Total Subscription';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Total Subscription';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'none';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '5';
  $handler->display->display_options['style_plugin'] = 'default';
  $handler->display->display_options['row_plugin'] = 'fields';
  /* Field: Total Subscription: Total Subscription Entity Type */
  $handler->display->display_options['fields']['entity_type']['id'] = 'entity_type';
  $handler->display->display_options['fields']['entity_type']['table'] = 'total_subscription';
  $handler->display->display_options['fields']['entity_type']['field'] = 'entity_type';

  /* Display: Total Subscription Taxonomy */
  $handler = $view->new_display('block', 'Total Subscription Taxonomy', 'block');
  $handler->display->display_options['display_description'] = 'This is for taxonomy';
  $handler->display->display_options['defaults']['use_ajax'] = FALSE;
  $handler->display->display_options['use_ajax'] = TRUE;
  $handler->display->display_options['defaults']['style_plugin'] = FALSE;
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'mail' => 'mail',
    'bundle' => 'bundle',
  );
  $handler->display->display_options['style_options']['default'] = '-1';
  $handler->display->display_options['style_options']['info'] = array(
    'mail' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'bundle' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
  );
  $handler->display->display_options['defaults']['style_options'] = FALSE;
  $handler->display->display_options['defaults']['row_plugin'] = FALSE;
  $handler->display->display_options['defaults']['row_options'] = FALSE;
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: Total Subscription: Total Subscription entity_id */
  $handler->display->display_options['fields']['entity_id']['id'] = 'entity_id';
  $handler->display->display_options['fields']['entity_id']['table'] = 'total_subscription';
  $handler->display->display_options['fields']['entity_id']['field'] = 'entity_id';
  $handler->display->display_options['fields']['entity_id']['exclude'] = TRUE;
  /* Field: Total Subscription: Total Subscription Mail */
  $handler->display->display_options['fields']['mail']['id'] = 'mail';
  $handler->display->display_options['fields']['mail']['table'] = 'total_subscription';
  $handler->display->display_options['fields']['mail']['field'] = 'mail';
  /* Field: Total Subscription: Total Subscription bundle */
  $handler->display->display_options['fields']['bundle']['id'] = 'bundle';
  $handler->display->display_options['fields']['bundle']['table'] = 'total_subscription';
  $handler->display->display_options['fields']['bundle']['field'] = 'bundle';
  $handler->display->display_options['fields']['bundle']['label'] = 'Total Subscription Category';
  $handler->display->display_options['fields']['bundle']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['bundle']['alter']['text'] = '<a href="/taxonomy/term/[entity_id]">[bundle]</a>';
  $handler->display->display_options['defaults']['filter_groups'] = FALSE;
  $handler->display->display_options['defaults']['filters'] = FALSE;
  /* Filter criterion: Total Subscription: Total Subscription Entity Type */
  $handler->display->display_options['filters']['entity_type']['id'] = 'entity_type';
  $handler->display->display_options['filters']['entity_type']['table'] = 'total_subscription';
  $handler->display->display_options['filters']['entity_type']['field'] = 'entity_type';
  $handler->display->display_options['filters']['entity_type']['value'] = 'taxonomy_term';
  /* Filter criterion: Total Subscription: Total Subscription Mail */
  $handler->display->display_options['filters']['mail']['id'] = 'mail';
  $handler->display->display_options['filters']['mail']['table'] = 'total_subscription';
  $handler->display->display_options['filters']['mail']['field'] = 'mail';
  $handler->display->display_options['filters']['mail']['exposed'] = TRUE;
  $handler->display->display_options['filters']['mail']['expose']['operator_id'] = 'mail_op';
  $handler->display->display_options['filters']['mail']['expose']['label'] = 'Total Subscription Mail';
  $handler->display->display_options['filters']['mail']['expose']['operator'] = 'mail_op';
  $handler->display->display_options['filters']['mail']['expose']['identifier'] = 'mail';
  $handler->display->display_options['filters']['mail']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
  );
  /* Filter criterion: Total Subscription: Total Subscription bundle */
  $handler->display->display_options['filters']['bundle']['id'] = 'bundle';
  $handler->display->display_options['filters']['bundle']['table'] = 'total_subscription';
  $handler->display->display_options['filters']['bundle']['field'] = 'bundle';
  $handler->display->display_options['filters']['bundle']['operator'] = 'word';
  $handler->display->display_options['filters']['bundle']['exposed'] = TRUE;
  $handler->display->display_options['filters']['bundle']['expose']['operator_id'] = 'bundle_op';
  $handler->display->display_options['filters']['bundle']['expose']['label'] = 'Total Subscription Category';
  $handler->display->display_options['filters']['bundle']['expose']['operator'] = 'bundle_op';
  $handler->display->display_options['filters']['bundle']['expose']['identifier'] = 'bundle';
  $handler->display->display_options['filters']['bundle']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
  );
  /* Filter criterion: Total Subscription: Total Subscription active */
  $handler->display->display_options['filters']['active']['id'] = 'active';
  $handler->display->display_options['filters']['active']['table'] = 'total_subscription';
  $handler->display->display_options['filters']['active']['field'] = 'active';
  $handler->display->display_options['filters']['active']['value'] = '1';
  $handler->display->display_options['filters']['active']['exposed'] = TRUE;
  $handler->display->display_options['filters']['active']['expose']['operator_id'] = 'active_op';
  $handler->display->display_options['filters']['active']['expose']['label'] = 'Total Subscription active';
  $handler->display->display_options['filters']['active']['expose']['operator'] = 'active_op';
  $handler->display->display_options['filters']['active']['expose']['identifier'] = 'active';
  $handler->display->display_options['filters']['active']['is_grouped'] = TRUE;
  $handler->display->display_options['filters']['active']['group_info']['label'] = 'Total Subscription active';
  $handler->display->display_options['filters']['active']['group_info']['identifier'] = 'active';
  $handler->display->display_options['filters']['active']['group_info']['group_items'] = array(
    1 => array(
      'title' => 'Active',
      'operator' => '=',
      'value' => '1',
    ),
    2 => array(
      'title' => 'Inactive',
      'operator' => '=',
      'value' => '0',
    ),
  );

  /* Display: Total Subscription Nodes */
  $handler = $view->new_display('block', 'Total Subscription Nodes', 'block_1');
  $handler->display->display_options['display_description'] = 'This is for the nodes displaying';
  $handler->display->display_options['defaults']['use_ajax'] = FALSE;
  $handler->display->display_options['use_ajax'] = TRUE;
  $handler->display->display_options['defaults']['style_plugin'] = FALSE;
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'mail' => 'mail',
    'bundle' => 'bundle',
  );
  $handler->display->display_options['style_options']['default'] = '-1';
  $handler->display->display_options['style_options']['info'] = array(
    'mail' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'bundle' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
  );
  $handler->display->display_options['defaults']['style_options'] = FALSE;
  $handler->display->display_options['defaults']['row_plugin'] = FALSE;
  $handler->display->display_options['defaults']['row_options'] = FALSE;
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: Total Subscription: Total Subscription entity_id */
  $handler->display->display_options['fields']['entity_id']['id'] = 'entity_id';
  $handler->display->display_options['fields']['entity_id']['table'] = 'total_subscription';
  $handler->display->display_options['fields']['entity_id']['field'] = 'entity_id';
  $handler->display->display_options['fields']['entity_id']['exclude'] = TRUE;
  /* Field: Total Subscription: Total Subscription Mail */
  $handler->display->display_options['fields']['mail']['id'] = 'mail';
  $handler->display->display_options['fields']['mail']['table'] = 'total_subscription';
  $handler->display->display_options['fields']['mail']['field'] = 'mail';
  /* Field: Total Subscription: Total Subscription bundle */
  $handler->display->display_options['fields']['bundle']['id'] = 'bundle';
  $handler->display->display_options['fields']['bundle']['table'] = 'total_subscription';
  $handler->display->display_options['fields']['bundle']['field'] = 'bundle';
  $handler->display->display_options['fields']['bundle']['label'] = 'Total Subscription Category';
  $handler->display->display_options['fields']['bundle']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['bundle']['alter']['text'] = '<a href="/node/[entity_id]">[bundle]</a>';
  $handler->display->display_options['defaults']['filter_groups'] = FALSE;
  $handler->display->display_options['defaults']['filters'] = FALSE;
  /* Filter criterion: Total Subscription: Total Subscription Entity Type */
  $handler->display->display_options['filters']['entity_type']['id'] = 'entity_type';
  $handler->display->display_options['filters']['entity_type']['table'] = 'total_subscription';
  $handler->display->display_options['filters']['entity_type']['field'] = 'entity_type';
  $handler->display->display_options['filters']['entity_type']['value'] = 'node';
  /* Filter criterion: Total Subscription: Total Subscription Mail */
  $handler->display->display_options['filters']['mail']['id'] = 'mail';
  $handler->display->display_options['filters']['mail']['table'] = 'total_subscription';
  $handler->display->display_options['filters']['mail']['field'] = 'mail';
  $handler->display->display_options['filters']['mail']['exposed'] = TRUE;
  $handler->display->display_options['filters']['mail']['expose']['operator_id'] = 'mail_op';
  $handler->display->display_options['filters']['mail']['expose']['label'] = 'Total Subscription Mail';
  $handler->display->display_options['filters']['mail']['expose']['operator'] = 'mail_op';
  $handler->display->display_options['filters']['mail']['expose']['identifier'] = 'mail';
  $handler->display->display_options['filters']['mail']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
  );
  /* Filter criterion: Total Subscription: Total Subscription bundle */
  $handler->display->display_options['filters']['bundle']['id'] = 'bundle';
  $handler->display->display_options['filters']['bundle']['table'] = 'total_subscription';
  $handler->display->display_options['filters']['bundle']['field'] = 'bundle';
  $handler->display->display_options['filters']['bundle']['operator'] = 'word';
  $handler->display->display_options['filters']['bundle']['exposed'] = TRUE;
  $handler->display->display_options['filters']['bundle']['expose']['operator_id'] = 'bundle_op';
  $handler->display->display_options['filters']['bundle']['expose']['label'] = 'Total Subscription Category';
  $handler->display->display_options['filters']['bundle']['expose']['operator'] = 'bundle_op';
  $handler->display->display_options['filters']['bundle']['expose']['identifier'] = 'bundle';
  $handler->display->display_options['filters']['bundle']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
  );
  /* Filter criterion: Total Subscription: Total Subscription active */
  $handler->display->display_options['filters']['active']['id'] = 'active';
  $handler->display->display_options['filters']['active']['table'] = 'total_subscription';
  $handler->display->display_options['filters']['active']['field'] = 'active';
  $handler->display->display_options['filters']['active']['exposed'] = TRUE;
  $handler->display->display_options['filters']['active']['expose']['operator_id'] = 'active_op';
  $handler->display->display_options['filters']['active']['expose']['label'] = 'Total Subscription active';
  $handler->display->display_options['filters']['active']['expose']['operator'] = 'active_op';
  $handler->display->display_options['filters']['active']['expose']['identifier'] = 'active';
  $handler->display->display_options['filters']['active']['is_grouped'] = TRUE;
  $handler->display->display_options['filters']['active']['group_info']['label'] = 'Active Users';
  $handler->display->display_options['filters']['active']['group_info']['identifier'] = 'active';
  $handler->display->display_options['filters']['active']['group_info']['group_items'] = array(
    1 => array(
      'title' => 'Active',
      'operator' => '=',
      'value' => '1',
    ),
    2 => array(
      'title' => 'Inactive',
      'operator' => '=',
      'value' => '0',
    ),
    3 => array(
      'title' => '',
      'operator' => '=',
      'value' => '',
    ),
  );
  // Add view to list of views to provide.
  $views[$view->name] = $view;
  // Repeat all of the above for each view the module should provide.
  // At the end, return array of default views.
  return $views;
}
