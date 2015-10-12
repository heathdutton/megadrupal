<?php

/**
 * @file entity_operations_test_views.views_default.inc
 */

/**
 * Implements hook_views_default_views().
 */
function entity_operations_test_views_views_default_views() {
  // Begin copy and paste of output from the Export tab of a view.
  $view = new view();
  $view->name = 'entity_operations_test_tab_view';
  $view->description = 'Tests the Views plugin for showing a view as an entity operation tab.';
  $view->tag = 'testing';
  $view->base_table = 'node';
  $view->human_name = 'Entity Operations Views Tab';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Nodes list';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'some';
  $handler->display->display_options['pager']['options']['items_per_page'] = '10';
  $handler->display->display_options['style_plugin'] = 'table';
  /* No results behavior: Global: Text area */
  $handler->display->display_options['empty']['area']['id'] = 'area';
  $handler->display->display_options['empty']['area']['table'] = 'views';
  $handler->display->display_options['empty']['area']['field'] = 'area';
  $handler->display->display_options['empty']['area']['empty'] = TRUE;
  $handler->display->display_options['empty']['area']['content'] = 'No results were found.';
  $handler->display->display_options['empty']['area']['format'] = 'filtered_html';
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
  /* Contextual filter: Content: Test entities (field_test_test_entities) */
  $handler->display->display_options['arguments']['field_test_test_entities_target_id']['id'] = 'field_test_test_entities_target_id';
  $handler->display->display_options['arguments']['field_test_test_entities_target_id']['table'] = 'field_data_field_test_test_entities';
  $handler->display->display_options['arguments']['field_test_test_entities_target_id']['field'] = 'field_test_test_entities_target_id';
  $handler->display->display_options['arguments']['field_test_test_entities_target_id']['default_action'] = 'empty';
  $handler->display->display_options['arguments']['field_test_test_entities_target_id']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['field_test_test_entities_target_id']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['field_test_test_entities_target_id']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['field_test_test_entities_target_id']['summary_options']['items_per_page'] = '25';
  /* Filter criterion: Content: Promoted to front page */
  $handler->display->display_options['filters']['promote']['id'] = 'promote';
  $handler->display->display_options['filters']['promote']['table'] = 'node';
  $handler->display->display_options['filters']['promote']['field'] = 'promote';
  $handler->display->display_options['filters']['promote']['value'] = 'All';
  $handler->display->display_options['filters']['promote']['exposed'] = TRUE;
  $handler->display->display_options['filters']['promote']['expose']['operator_id'] = '';
  $handler->display->display_options['filters']['promote']['expose']['label'] = 'Promoted to front page';
  $handler->display->display_options['filters']['promote']['expose']['operator'] = 'promote_op';
  $handler->display->display_options['filters']['promote']['expose']['identifier'] = 'promote';
  $handler->display->display_options['filters']['promote']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
  );

  /* Display: Entity tab */
  $handler = $view->new_display('entity_operation', 'Entity tab', 'entity_operation_1');
  $handler->display->display_options['entity_type'] = 'entity_operations_test';
  $handler->display->display_options['path'] = 'test-view-nodes';
  $handler->display->display_options['menu_weight'] = '10';
  $handler->display->display_options['menu_default'] = 0;
  $handler->display->display_options['menu_default_secondary'] = 0;
  $handler->display->display_options['menu_parent_weight'] = '10';
  // (Export ends here.)

  // Add view to list of views to provide.
  $views[$view->name] = $view;

  // ...Repeat all of the above for each view the module should provide.

  // At the end, return array of default views.
  return $views;
}
