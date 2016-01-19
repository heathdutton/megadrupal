<?php

/**
 * Implements hook_views_default_views().
 */
function bookit_item_views_default_views() {
  $views = array();

  $view = new view();
  $view->name = 'bookit_items';
  $view->description = 'Display a list of bookable items for admin.';
  $view->tag = 'bookit';
  $view->base_table = 'bookit_item';
  $view->human_name = 'Bookable Items';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Bookable Items';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'administer bookit_item entities';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '50';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'item_id' => 'item_id',
    'title' => 'title',
    'type' => 'type',
    'status' => 'status',
    'operations' => 'operations',
  );
  $handler->display->display_options['style_options']['default'] = '-1';
  $handler->display->display_options['style_options']['info'] = array(
    'item_id' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
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
    'status' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'operations' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
  );
  $handler->display->display_options['style_options']['empty_table'] = TRUE;
  /* No results behavior: Bookable Item: Empty text */
  $handler->display->display_options['empty']['empty_text']['id'] = 'empty_text';
  $handler->display->display_options['empty']['empty_text']['table'] = 'bookit_item';
  $handler->display->display_options['empty']['empty_text']['field'] = 'empty_text';
  $handler->display->display_options['empty']['empty_text']['empty'] = TRUE;
  /* Field: Bookable Item: Bookable Item ID */
  $handler->display->display_options['fields']['item_id']['id'] = 'item_id';
  $handler->display->display_options['fields']['item_id']['table'] = 'bookit_item';
  $handler->display->display_options['fields']['item_id']['field'] = 'item_id';
  $handler->display->display_options['fields']['item_id']['label'] = 'Item ID';
  $handler->display->display_options['fields']['item_id']['link_to_item'] = 0;
  /* Field: Bookable Item: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'bookit_item';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['link_to_item'] = 1;
  /* Field: Bookable Item: Booking Type */
  $handler->display->display_options['fields']['type']['id'] = 'type';
  $handler->display->display_options['fields']['type']['table'] = 'bookit_item';
  $handler->display->display_options['fields']['type']['field'] = 'type';
  $handler->display->display_options['fields']['type']['link_to_item'] = 0;
  $handler->display->display_options['fields']['type']['use_raw_value'] = 0;
  /* Field: Bookable Item: Status */
  $handler->display->display_options['fields']['status']['id'] = 'status';
  $handler->display->display_options['fields']['status']['table'] = 'bookit_item';
  $handler->display->display_options['fields']['status']['field'] = 'status';
  $handler->display->display_options['fields']['status']['type'] = 'active-disabled';
  $handler->display->display_options['fields']['status']['not'] = 0;
  /* Field: Bookable Item: Operations links */
  $handler->display->display_options['fields']['operations']['id'] = 'operations';
  $handler->display->display_options['fields']['operations']['table'] = 'bookit_item';
  $handler->display->display_options['fields']['operations']['field'] = 'operations';
  $handler->display->display_options['fields']['operations']['label'] = 'Operations';
  $handler->display->display_options['fields']['operations']['add_destination'] = 1;
  /* Sort criterion: Bookable Item: Created date */
  $handler->display->display_options['sorts']['created']['id'] = 'created';
  $handler->display->display_options['sorts']['created']['table'] = 'bookit_item';
  $handler->display->display_options['sorts']['created']['field'] = 'created';
  $handler->display->display_options['sorts']['created']['order'] = 'DESC';

  /* Display: Admin page */
  $handler = $view->new_display('page', 'Admin page', 'admin_page');
  $handler->display->display_options['path'] = 'admin/bookit/items/list';
  $handler->display->display_options['menu']['type'] = 'default tab';
  $handler->display->display_options['menu']['title'] = 'List';
  $handler->display->display_options['menu']['weight'] = '-10';
  $handler->display->display_options['tab_options']['type'] = 'normal';
  $handler->display->display_options['tab_options']['title'] = 'Bookable Items';
  $handler->display->display_options['tab_options']['description'] = 'Manage items and item types in the bookit platform.';
  $handler->display->display_options['tab_options']['weight'] = '';
  $handler->display->display_options['tab_options']['name'] = 'management';

  $views[$view->name] = $view;

  // Foreach entity reference field that references a bookit_item for a node.
  foreach (commerce_info_fields('entityreference') as $field_name => $field) {
    if ($field['settings']['target_type'] == 'bookit_item' and isset($field['bundles']['node'])) {
      foreach ($field['bundles']['node'] as $type) {
        // Load the content type.
        $content_type = node_type_load($type);

        // Create a default view for this content type.
        $view = new view();
        $view->name = "{$type}_bookit";
        $view->description = "Booking search results for the content type {$content_type->name}";
        $view->tag = 'bookit';
        $view->base_table = 'node';
        $view->human_name = drupal_ucfirst($content_type->name) . ' bookit results';
        $view->core = 7;
        $view->api_version = '3.0';
        $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

        /* Display: Master */
        $handler = $view->new_display('default', 'Master', 'default');
        $handler->display->display_options['title'] = $view->human_name;
        $handler->display->display_options['use_more_always'] = FALSE;
        $handler->display->display_options['access']['type'] = 'perm';
        $handler->display->display_options['cache']['type'] = 'none';
        $handler->display->display_options['query']['type'] = 'views_query';
        $handler->display->display_options['query']['options']['distinct'] = TRUE;
        $handler->display->display_options['exposed_form']['type'] = 'input_required';
        $handler->display->display_options['exposed_form']['options']['text_input_required_format'] = 'filtered_html';
        $handler->display->display_options['pager']['type'] = 'full';
        $handler->display->display_options['pager']['options']['items_per_page'] = '20';
        $handler->display->display_options['pager']['options']['offset'] = '0';
        $handler->display->display_options['pager']['options']['id'] = '0';
        $handler->display->display_options['pager']['options']['quantity'] = '9';
        $handler->display->display_options['style_plugin'] = 'default';
        $handler->display->display_options['row_plugin'] = 'node';

        /* Relationship: Entity Reference: Referenced Entity */
        $field_name = $field['field_name'];
        $handler->display->display_options['relationships']["{$field_name}_target_id"]['id'] = "{$field_name}_target_id";
        $handler->display->display_options['relationships']["{$field_name}_target_id"]['table'] = "field_data_{$field_name}";
        $handler->display->display_options['relationships']["{$field_name}_target_id"]['field'] = "{$field_name}_target_id";
        $handler->display->display_options['relationships']["{$field_name}_target_id"]['label'] = 'Bookable Item';

        /* Sort criterion: Content: Post date */
        $handler->display->display_options['sorts']['created']['id'] = 'created';
        $handler->display->display_options['sorts']['created']['table'] = 'node';
        $handler->display->display_options['sorts']['created']['field'] = 'created';
        $handler->display->display_options['sorts']['created']['order'] = 'DESC';

        /* Filter criterion: Content: Type */
        $handler->display->display_options['filters']['type']['id'] = 'type';
        $handler->display->display_options['filters']['type']['table'] = 'node';
        $handler->display->display_options['filters']['type']['field'] = 'type';
        $handler->display->display_options['filters']['type']['group'] = 1;
        $handler->display->display_options['filters']['type']['value'] = array(
          $type => $type,
        );

        /* Filter criterion: Content: Published */
        $handler->display->display_options['filters']['status']['id'] = 'status';
        $handler->display->display_options['filters']['status']['table'] = 'node';
        $handler->display->display_options['filters']['status']['field'] = 'status';
        $handler->display->display_options['filters']['status']['value'] = 1;
        $handler->display->display_options['filters']['status']['group'] = 1;
        $handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;

        /* Filter criterion: Bookable Item: Status */
        $handler->display->display_options['filters']['status_1']['id'] = 'status_1';
        $handler->display->display_options['filters']['status_1']['table'] = 'bookit_item';
        $handler->display->display_options['filters']['status_1']['field'] = 'status';
        $handler->display->display_options['filters']['status_1']['relationship'] = "{$field_name}_target_id";
        $handler->display->display_options['filters']['status_1']['value'] = '1';
        $handler->display->display_options['filters']['status_1']['group'] = 1;

        /* Display: Page */
        $handler = $view->new_display('page', 'Page', 'page');
        $handler->display->display_options['path'] = "booking/{$type}";
        $translatables[$view->name] = array(
          t('Master'),
          t($view->human_name),
          t('more'),
          t('Apply'),
          t('Reset'),
          t('Sort by'),
          t('Asc'),
          t('Desc'),
          t('Select any filter and click on Apply to see results'),
          t('Items per page'),
          t('- All -'),
          t('Offset'),
          t('« first'),
          t('‹ previous'),
          t('next ›'),
          t('last »'),
          t('Bookable Item'),
          t('Date'),
          t('Page'),
        );

        // Add the view to views list.
        $views[$view->name] = $view;

        $view = new view();
        $view->name = "{$type}_bookit_item_table";
        $view->description = "Display a set of {$type} bookit items in a table.";
        $view->tag = 'bookit';
        $view->base_table = 'bookit_item';
        $view->human_name = drupal_ucfirst($content_type->name) . ' bookit items';
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
        $handler->display->display_options['style_plugin'] = 'table';
        $handler->display->display_options['style_options']['columns'] = array(
          'title' => 'title',
        );
        $handler->display->display_options['style_options']['default'] = '-1';
        $handler->display->display_options['style_options']['info'] = array(
          'title' => array(
            'sortable' => 0,
            'default_sort_order' => 'asc',
            'align' => '',
            'separator' => '',
            'empty_column' => 0,
          ),
        );

        /* Field: Bookable Item: Title */
        $handler->display->display_options['fields']['title']['id'] = 'title';
        $handler->display->display_options['fields']['title']['table'] = 'bookit_item';
        $handler->display->display_options['fields']['title']['field'] = 'title';
        $handler->display->display_options['fields']['title']['link_to_item'] = 0;

        /* Contextual filter: Bookable Item: Bookable Item ID */
        $handler->display->display_options['arguments']['item_id']['id'] = 'item_id';
        $handler->display->display_options['arguments']['item_id']['table'] = 'bookit_item';
        $handler->display->display_options['arguments']['item_id']['field'] = 'item_id';
        $handler->display->display_options['arguments']['item_id']['default_action'] = 'not found';
        $handler->display->display_options['arguments']['item_id']['default_argument_type'] = 'fixed';
        $handler->display->display_options['arguments']['item_id']['summary']['number_of_records'] = '0';
        $handler->display->display_options['arguments']['item_id']['summary']['format'] = 'default_summary';
        $handler->display->display_options['arguments']['item_id']['summary_options']['items_per_page'] = '25';
        $handler->display->display_options['arguments']['item_id']['break_phrase'] = TRUE;

        // Add the view to views list.
        $views[$view->name] = $view;
      }
    }
  }

  return $views;
}
