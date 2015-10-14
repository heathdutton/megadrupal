<?php

/**
 * @file
 * Views Default Template
 */

/**
 * Returns View Exporter object.
 *
 * @param string $name
 *   Name of exporter.
 *
 * @param string $entity_type
 *   Entity type (node, user, etc.). Used for description and path.
 *
 * @param string $base_table
 *   Base_table (node, users, etc.).
 *
 * @param array $display_options
 *   Contains filters, relationsships, sorts and fields.
 */
function mirrors_views_default_template($name, $entity_type, $base_table, $display_options) {
  /* Build up the view */
  $view = new view();
  $view->name = $name;
  $view->description = 'Export ' . $entity_type . 's to CSV file.';
  $view->tag = 'Mirrors';
  $view->base_table = $base_table;
  $view->human_name = $name;
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'Administer mirrors';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['style_plugin'] = 'table';

  /* Display Options */
  /* Filters */
  foreach ($display_options['filters'] as $key => $filter) {
    $handler->display->display_options['filters'][$key] = $filter;
  }

  /* Relations */
  foreach ($display_options['relationships'] as $key => $relation) {
    $handler->display->display_options['relationships'][$key] = $relation;
  }

  /* Sorts */
  foreach ($display_options['sorts'] as $key => $sort) {
    $handler->display->display_options['sorts'][$key] = $sort;
  }

  /* Fields */
  foreach ($display_options['fields'] as $key => $field) {
    $handler->display->display_options['fields'][$key] = $field;
  }

  /* Display: Data export */
  $handler = $view->new_display('views_data_export', 'Data export', 'views_data_export_1');
  $handler->display->display_options['defaults']['hide_admin_links'] = FALSE;
  $handler->display->display_options['pager']['type'] = 'none';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['style_plugin'] = 'views_data_export_csv';
  $handler->display->display_options['style_options']['provide_file'] = 0;
  $handler->display->display_options['style_options']['parent_sort'] = 0;
  $handler->display->display_options['style_options']['separator'] = ';';
  $handler->display->display_options['style_options']['quote'] = 1;
  $handler->display->display_options['style_options']['trim'] = 1;
  $handler->display->display_options['style_options']['replace_newlines'] = 1;
  $handler->display->display_options['style_options']['newline_replacement'] = '<br/>';
  $handler->display->display_options['style_options']['header'] = 1;
  $handler->display->display_options['style_options']['keep_html'] = 1;
  $handler->display->display_options['style_options']['encoding'] = 'UTF-8';
  $handler->display->display_options['path'] = 'views_data_export/' . $entity_type . '/' . $name . '.csv';
  $handler->display->display_options['use_batch'] = 'batch';

  return $view;
}
