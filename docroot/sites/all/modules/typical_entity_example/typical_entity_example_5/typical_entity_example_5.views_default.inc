<?php

/**
 * @file
 * Views exported from Drupal with "Export" button.
 */

/**
 * Implements hook_views_default_views().
 */
function typical_entity_example_5_views_default_views() {
  // Paste exported view code.
  $view = new view();
  $view->name = 'typical_entity_example_5';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'typical_entity_example_5';
  $view->human_name = 'Typical Entity 5';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */
  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Typical Entity 5';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'view typical_entity_example content';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '10';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'teid' => 'teid',
    'title' => 'title',
    'description' => 'description',
    'typical_entity_example_5_field_1' => 'typical_entity_example_5_field_1',
  );
  $handler->display->display_options['style_options']['default'] = '-1';
  $handler->display->display_options['style_options']['info'] = array(
    'teid' => array(
      'sortable' => 0,
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
    'description' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'typical_entity_example_5_field_1' => array(
      'sortable' => 0,
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
  $handler->display->display_options['header']['area']['content'] = '<a href="<?php print url(\'typical_entity_example_5/add\'); ?>>Add Typical Entity 5</a>';
  $handler->display->display_options['header']['area']['format'] = 'full_html';
  /* Field: Typical Entity 5: Typical Entity 5 ID */
  $handler->display->display_options['fields']['teid']['id'] = 'teid';
  $handler->display->display_options['fields']['teid']['table'] = 'typical_entity_example_5';
  $handler->display->display_options['fields']['teid']['field'] = 'teid';
  $handler->display->display_options['fields']['teid']['label'] = 'ID';
  /* Field: Typical Entity 5: Typical Entity 5 title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'typical_entity_example_5';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = 'Title';
  $handler->display->display_options['fields']['title']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['title']['alter']['path'] = 'typical_entity_example_5/[teid]';
  $handler->display->display_options['fields']['title']['element_label_colon'] = FALSE;
  /* Field: Typical Entity 5: Typical Entity 5 description */
  $handler->display->display_options['fields']['description']['id'] = 'description';
  $handler->display->display_options['fields']['description']['table'] = 'typical_entity_example_5';
  $handler->display->display_options['fields']['description']['field'] = 'description';
  $handler->display->display_options['fields']['description']['label'] = 'Description';
  /* Field: Global: Custom text */
  $handler->display->display_options['fields']['nothing']['id'] = 'nothing';
  $handler->display->display_options['fields']['nothing']['table'] = 'views';
  $handler->display->display_options['fields']['nothing']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing']['label'] = 'Edit';
  $handler->display->display_options['fields']['nothing']['alter']['text'] = 'Edit';
  $handler->display->display_options['fields']['nothing']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['nothing']['alter']['path'] = 'typical_entity_example_5/[teid]/edit';
  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['defaults']['hide_admin_links'] = FALSE;
  $handler->display->display_options['path'] = 'typical_entity_example_5';
  $handler->display->display_options['menu']['type'] = 'normal';
  $handler->display->display_options['menu']['title'] = 'Typical Entity 5';

  // Return the view.
  $views['typical_entity_example_5'] = $view;
  return $views;
}
