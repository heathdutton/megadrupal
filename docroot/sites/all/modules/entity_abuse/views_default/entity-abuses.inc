<?php

$view = new view();
$view->name = 'entity_abuse_entity_abuses';
$view->description = 'Provides a list of abuses for an entity from the URL arguments';
$view->tag = 'entity_abuse';
$view->base_table = 'entity_abuse';
$view->human_name = 'Entity abuse: Entity abuses';
$view->core = 7;
$view->api_version = '3.0';
$view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

/* Display: Master */
$handler = $view->new_display('default', 'Master', 'default');
$handler->display->display_options['title'] = 'Entity abuses';
$handler->display->display_options['use_more_always'] = FALSE;
$handler->display->display_options['access']['type'] = 'perm';
$handler->display->display_options['access']['perm'] = 'manage entity abuses';
$handler->display->display_options['cache']['type'] = 'none';
$handler->display->display_options['query']['type'] = 'views_query';
$handler->display->display_options['exposed_form']['type'] = 'basic';
$handler->display->display_options['pager']['type'] = 'full';
$handler->display->display_options['pager']['options']['items_per_page'] = '50';
$handler->display->display_options['style_plugin'] = 'table';
$handler->display->display_options['style_options']['columns'] = array(
  'name' => 'name',
  'timestamp' => 'timestamp',
  'reason' => 'reason',
);
$handler->display->display_options['style_options']['default'] = 'timestamp';
$handler->display->display_options['style_options']['info'] = array(
  'name' => array(
    'sortable' => 1,
    'default_sort_order' => 'asc',
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
  'timestamp' => array(
    'sortable' => 1,
    'default_sort_order' => 'desc',
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
  'reason' => array(
    'sortable' => 1,
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
$handler->display->display_options['empty']['area']['content'] = 'There is no abuses for this %1.';
$handler->display->display_options['empty']['area']['format'] = 'panopoly_wysiwyg_text';
$handler->display->display_options['empty']['area']['tokenize'] = TRUE;
/* Relationship: Entity abuse: User */
$handler->display->display_options['relationships']['uid']['id'] = 'uid';
$handler->display->display_options['relationships']['uid']['table'] = 'entity_abuse';
$handler->display->display_options['relationships']['uid']['field'] = 'uid';
$handler->display->display_options['relationships']['uid']['required'] = TRUE;
/* Field: User: Name */
$handler->display->display_options['fields']['name']['id'] = 'name';
$handler->display->display_options['fields']['name']['table'] = 'users';
$handler->display->display_options['fields']['name']['field'] = 'name';
$handler->display->display_options['fields']['name']['relationship'] = 'uid';
$handler->display->display_options['fields']['name']['label'] = 'User name';
/* Field: Entity abuse: Date of abuse */
$handler->display->display_options['fields']['timestamp']['id'] = 'timestamp';
$handler->display->display_options['fields']['timestamp']['table'] = 'entity_abuse';
$handler->display->display_options['fields']['timestamp']['field'] = 'timestamp';
$handler->display->display_options['fields']['timestamp']['date_format'] = 'short';
/* Field: Entity abuse: Abuse reason */
$handler->display->display_options['fields']['reason']['id'] = 'reason';
$handler->display->display_options['fields']['reason']['table'] = 'entity_abuse';
$handler->display->display_options['fields']['reason']['field'] = 'reason';
/* Contextual filter: Entity abuse: Entity type */
$handler->display->display_options['arguments']['entity_type']['id'] = 'entity_type';
$handler->display->display_options['arguments']['entity_type']['table'] = 'entity_abuse';
$handler->display->display_options['arguments']['entity_type']['field'] = 'entity_type';
$handler->display->display_options['arguments']['entity_type']['default_action'] = 'not found';
$handler->display->display_options['arguments']['entity_type']['default_argument_type'] = 'fixed';
$handler->display->display_options['arguments']['entity_type']['summary']['number_of_records'] = '0';
$handler->display->display_options['arguments']['entity_type']['summary']['format'] = 'default_summary';
$handler->display->display_options['arguments']['entity_type']['summary_options']['items_per_page'] = '25';
$handler->display->display_options['arguments']['entity_type']['limit'] = '0';
/* Contextual filter: Entity abuse: Entity ID */
$handler->display->display_options['arguments']['entity_id']['id'] = 'entity_id';
$handler->display->display_options['arguments']['entity_id']['table'] = 'entity_abuse';
$handler->display->display_options['arguments']['entity_id']['field'] = 'entity_id';
$handler->display->display_options['arguments']['entity_id']['default_action'] = 'not found';
$handler->display->display_options['arguments']['entity_id']['title_enable'] = TRUE;
$handler->display->display_options['arguments']['entity_id']['title'] = 'List of abuses for %1 with ID %2';
$handler->display->display_options['arguments']['entity_id']['default_argument_type'] = 'fixed';
$handler->display->display_options['arguments']['entity_id']['summary']['number_of_records'] = '0';
$handler->display->display_options['arguments']['entity_id']['summary']['format'] = 'default_summary';
$handler->display->display_options['arguments']['entity_id']['summary_options']['items_per_page'] = '25';
$handler->display->display_options['arguments']['entity_id']['specify_validation'] = TRUE;
$handler->display->display_options['arguments']['entity_id']['validate']['type'] = 'numeric';

/* Display: Page */
$handler = $view->new_display('page', 'Page', 'page');
$handler->display->display_options['path'] = 'admin/content/entity_abuse/%/%';
$handler->display->display_options['menu']['title'] = 'List of abuses for an entity %2';
$handler->display->display_options['menu']['weight'] = '0';
$handler->display->display_options['menu']['context'] = 0;
$handler->display->display_options['menu']['context_only_inline'] = 0;
