<?php

/**
 * @file views_filter_option_limit_test.views_default.inc
 * Contains default views.
 */

/**
 * Implements hook_views_default_views().
 */
function views_filter_option_limit_test_views_default_views() {
  // Begin copy and paste of output from the Export tab of a view.
  $view = new view();
  $view->name = 'test_views_filter_options_limit_nodes';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'node';
  $view->human_name = 'test views filter options limit nodes';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Views Filter Options Limit test';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['exposed_form']['options']['reset_button'] = TRUE;
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '10';
  $handler->display->display_options['style_plugin'] = 'table';
  /* Relationship: Entity Reference: Referenced Entity */
  $handler->display->display_options['relationships']['test_vfol_city_ref_target_id']['id'] = 'test_vfol_city_ref_target_id';
  $handler->display->display_options['relationships']['test_vfol_city_ref_target_id']['table'] = 'field_data_test_vfol_city_ref';
  $handler->display->display_options['relationships']['test_vfol_city_ref_target_id']['field'] = 'test_vfol_city_ref_target_id';
  /* Field: Content: Nid */
  $handler->display->display_options['fields']['nid']['id'] = 'nid';
  $handler->display->display_options['fields']['nid']['table'] = 'node';
  $handler->display->display_options['fields']['nid']['field'] = 'nid';
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
  /* Contextual filter: Content: Country (test_vfol_country_ref) */
  $handler->display->display_options['arguments']['test_vfol_country_ref_target_id']['id'] = 'test_vfol_country_ref_target_id';
  $handler->display->display_options['arguments']['test_vfol_country_ref_target_id']['table'] = 'field_data_test_vfol_country_ref';
  $handler->display->display_options['arguments']['test_vfol_country_ref_target_id']['field'] = 'test_vfol_country_ref_target_id';
  $handler->display->display_options['arguments']['test_vfol_country_ref_target_id']['relationship'] = 'test_vfol_city_ref_target_id';
  $handler->display->display_options['arguments']['test_vfol_country_ref_target_id']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['test_vfol_country_ref_target_id']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['test_vfol_country_ref_target_id']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['test_vfol_country_ref_target_id']['summary_options']['items_per_page'] = '25';
  $handler->display->display_options['arguments']['test_vfol_country_ref_target_id']['break_phrase'] = TRUE;
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
    'test_vfol_article' => 'test_vfol_article',
  );
  $handler->display->display_options['filters']['type']['group'] = 1;
  /* Filter criterion: Content: Promoted to front page */
  $handler->display->display_options['filters']['promote']['id'] = 'promote';
  $handler->display->display_options['filters']['promote']['table'] = 'node';
  $handler->display->display_options['filters']['promote']['field'] = 'promote';
  $handler->display->display_options['filters']['promote']['relationship'] = 'test_vfol_city_ref_target_id';
  $handler->display->display_options['filters']['promote']['value'] = 'All';
  $handler->display->display_options['filters']['promote']['group'] = 1;
  $handler->display->display_options['filters']['promote']['exposed'] = TRUE;
  $handler->display->display_options['filters']['promote']['expose']['operator_id'] = '';
  $handler->display->display_options['filters']['promote']['expose']['label'] = 'Promoted';
  $handler->display->display_options['filters']['promote']['expose']['operator'] = 'promote_op';
  $handler->display->display_options['filters']['promote']['expose']['identifier'] = 'promote';
  $handler->display->display_options['filters']['promote']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    4 => 0,
  );
  /* Filter criterion: Content: Country (test_vfol_country_ref) */
  $handler->display->display_options['filters']['test_vfol_country_ref_target_id']['id'] = 'test_vfol_country_ref_target_id';
  $handler->display->display_options['filters']['test_vfol_country_ref_target_id']['table'] = 'field_data_test_vfol_country_ref';
  $handler->display->display_options['filters']['test_vfol_country_ref_target_id']['field'] = 'test_vfol_country_ref_target_id';
  $handler->display->display_options['filters']['test_vfol_country_ref_target_id']['relationship'] = 'test_vfol_city_ref_target_id';
  $handler->display->display_options['filters']['test_vfol_country_ref_target_id']['group'] = 1;
  $handler->display->display_options['filters']['test_vfol_country_ref_target_id']['exposed'] = TRUE;
  $handler->display->display_options['filters']['test_vfol_country_ref_target_id']['expose']['operator_id'] = 'test_vfol_country_ref_target_id_op';
  $handler->display->display_options['filters']['test_vfol_country_ref_target_id']['expose']['label'] = 'Country';
  $handler->display->display_options['filters']['test_vfol_country_ref_target_id']['expose']['operator'] = 'test_vfol_country_ref_target_id_op';
  $handler->display->display_options['filters']['test_vfol_country_ref_target_id']['expose']['identifier'] = 'country';
  $handler->display->display_options['filters']['test_vfol_country_ref_target_id']['expose']['multiple'] = TRUE;
  $handler->display->display_options['filters']['test_vfol_country_ref_target_id']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    4 => 0,
  );
  /* Filter criterion: Content: City (test_vfol_city_ref) (with restricted options) */
  $handler->display->display_options['filters']['test_vfol_city_ref_target_id_option_limit']['id'] = 'test_vfol_city_ref_target_id_option_limit';
  $handler->display->display_options['filters']['test_vfol_city_ref_target_id_option_limit']['table'] = 'field_data_test_vfol_city_ref';
  $handler->display->display_options['filters']['test_vfol_city_ref_target_id_option_limit']['field'] = 'test_vfol_city_ref_target_id_option_limit';
  $handler->display->display_options['filters']['test_vfol_city_ref_target_id_option_limit']['group'] = 1;
  $handler->display->display_options['filters']['test_vfol_city_ref_target_id_option_limit']['exposed'] = TRUE;
  $handler->display->display_options['filters']['test_vfol_city_ref_target_id_option_limit']['expose']['operator_id'] = 'test_vfol_city_ref_target_id_option_limit_op';
  $handler->display->display_options['filters']['test_vfol_city_ref_target_id_option_limit']['expose']['label'] = 'City';
  $handler->display->display_options['filters']['test_vfol_city_ref_target_id_option_limit']['expose']['operator'] = 'test_vfol_city_ref_target_id_option_limit_op';
  $handler->display->display_options['filters']['test_vfol_city_ref_target_id_option_limit']['expose']['identifier'] = 'city';
  $handler->display->display_options['filters']['test_vfol_city_ref_target_id_option_limit']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    4 => 0,
  );
  $handler->display->display_options['filters']['test_vfol_city_ref_target_id_option_limit']['options_limit_filters'] = array(
    'promote' => 'promote',
    'test_vfol_country_ref_target_id' => 'test_vfol_country_ref_target_id',
  );
  $handler->display->display_options['filters']['test_vfol_city_ref_target_id_option_limit']['options_limit_arguments'] = array(
    'test_vfol_country_ref_target_id' => 'test_vfol_country_ref_target_id',
  );
  $handler->display->display_options['filters']['test_vfol_city_ref_target_id_option_limit']['no_limiting_values'] = 'none';

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['path'] = 'test-vfol';
  // (Export ends here.)

  // Add view to list of views to provide.
  $views[$view->name] = $view;

  // ...Repeat all of the above for each view the module should provide.

  // At the end, return array of default views.
  return $views;
}
