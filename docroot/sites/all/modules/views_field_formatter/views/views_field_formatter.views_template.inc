<?php

/**
 * @file
 * View templates for the Views Field Formatter module.
 */

/**
 * Implements hook_views_templates().
 */
function views_field_formatter_views_templates() {
  $entity_info = entity_get_info();
  $views = array();

  foreach($entity_info as $entity => $data) {
    $name = 'views_field_formatter_' . $data['base table'];
    $human_name = 'Views field formatter for entity ' . $data['label'];
    $description = 'Default views to help you getting started with Views Field Formatter for ' . $data['label'];
    $base_table = $data['base table'];
    $key = $data['entity keys']['id'];

    $view = new view();
    $view->name = $name;
    $view->description = $description;
    $view->tag = 'views_field_formatter';
    $view->base_table = $base_table;
    $view->human_name = $human_name;
    $view->core = 7;
    $view->api_version = '3.0';
    $view->disabled = TRUE; /* Edit this to true to make a default view disabled initially */

    /* Display: Master */
    $handler = $view->new_display('default', 'Master', 'default');
    $handler->display->display_options['hide_admin_links'] = TRUE;
    $handler->display->display_options['use_more_always'] = FALSE;
    $handler->display->display_options['access']['type'] = 'perm';
    $handler->display->display_options['cache']['type'] = 'none';
    $handler->display->display_options['query']['type'] = 'views_query';
    $handler->display->display_options['exposed_form']['type'] = 'basic';
    $handler->display->display_options['pager']['type'] = 'some';
    $handler->display->display_options['pager']['options']['items_per_page'] = '1';
    $handler->display->display_options['pager']['options']['offset'] = '0';
    $handler->display->display_options['style_plugin'] = 'default';
    $handler->display->display_options['row_plugin'] = 'fields';
    /* Field: Global: Custom text */
    $handler->display->display_options['fields']['nothing']['id'] = 'nothing';
    $handler->display->display_options['fields']['nothing']['table'] = 'views';
    $handler->display->display_options['fields']['nothing']['field'] = 'nothing';
    $handler->display->display_options['fields']['nothing']['label'] = '';
    $handler->display->display_options['fields']['nothing']['alter']['text'] = 'This is a default view field \'Global: Custom text\'.
You should remove it and display the field that you want.';
    $handler->display->display_options['fields']['nothing']['element_label_colon'] = FALSE;
    /* Field: Entity: ID */
    $handler->display->display_options['fields'][$key]['id'] = $key;
    $handler->display->display_options['fields'][$key]['table'] = $base_table;
    $handler->display->display_options['fields'][$key]['field'] = $key;
    $handler->display->display_options['fields'][$key]['exclude'] = TRUE;
    /* Contextual filter: This is the field item value placeholder */
    $handler->display->display_options['arguments']['null']['id'] = 'null';
    $handler->display->display_options['arguments']['null']['table'] = 'views';
    $handler->display->display_options['arguments']['null']['field'] = 'null';
    $handler->display->display_options['arguments']['null']['ui_name'] = 'This is the field item value placeholder.';
    $handler->display->display_options['arguments']['null']['default_argument_type'] = 'fixed';
    $handler->display->display_options['arguments']['null']['summary']['number_of_records'] = '0';
    $handler->display->display_options['arguments']['null']['summary']['format'] = 'default_summary';
    $handler->display->display_options['arguments']['null']['summary_options']['items_per_page'] = '25';
    /* Contextual filter: This is the entity ID placeholder */
    $handler->display->display_options['arguments'][$key]['id'] = $key;
    $handler->display->display_options['arguments'][$key]['table'] = $base_table;
    $handler->display->display_options['arguments'][$key]['field'] = $key;
    $handler->display->display_options['arguments'][$key]['ui_name'] = 'This is the entity ID placeholder.';
    $handler->display->display_options['arguments'][$key]['default_action'] = 'not found';
    $handler->display->display_options['arguments'][$key]['default_argument_type'] = 'fixed';
    $handler->display->display_options['arguments'][$key]['summary']['number_of_records'] = '0';
    $handler->display->display_options['arguments'][$key]['summary']['format'] = 'default_summary';
    $handler->display->display_options['arguments'][$key]['summary_options']['items_per_page'] = '25';
    /* Contextual filter: This is the item delta placeholder. */
    $handler->display->display_options['arguments']['null_1']['id'] = 'null_1';
    $handler->display->display_options['arguments']['null_1']['table'] = 'views';
    $handler->display->display_options['arguments']['null_1']['field'] = 'null';
    $handler->display->display_options['arguments']['null_1']['ui_name'] = 'This is the item delta placeholder.';
    $handler->display->display_options['arguments']['null_1']['default_argument_type'] = 'fixed';
    $handler->display->display_options['arguments']['null_1']['summary']['number_of_records'] = '0';
    $handler->display->display_options['arguments']['null_1']['summary']['format'] = 'default_summary';
    $handler->display->display_options['arguments']['null_1']['summary_options']['items_per_page'] = '25';

    $views[$view->name] = $view;
  }

  return $views;
}
