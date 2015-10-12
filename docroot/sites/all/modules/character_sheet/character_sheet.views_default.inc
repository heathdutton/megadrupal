<?php

/**
 * @file
 * Basic views integration for character_sheet and logs.
 */

/**
 * Implements hook_views_default_views(). From Views module.
 */
function character_sheet_views_default_views() {

  // START COPY AND PASTE
$view = new view;
$view->name = 'character_sheet_moderation';
$view->description = 'Character sheets';
$view->tag = 'default';
$view->base_table = 'node';
$view->human_name = 'Character sheets';
$view->core = 7;
$view->api_version = '3.0-alpha1';
$view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

/* Display: Master */
$handler = $view->new_display('default', 'Master', 'default');
$handler->display->display_options['title'] = 'Moderate character sheets';
$handler->display->display_options['access']['type'] = 'perm';
$handler->display->display_options['access']['perm'] = 'view character sheet';
$handler->display->display_options['cache']['type'] = 'none';
$handler->display->display_options['query']['type'] = 'views_query';
$handler->display->display_options['query']['options']['query_comment'] = FALSE;
$handler->display->display_options['exposed_form']['type'] = 'basic';
$handler->display->display_options['pager']['type'] = 'full';
$handler->display->display_options['pager']['options']['items_per_page'] = '35';
$handler->display->display_options['style_plugin'] = 'table';
$handler->display->display_options['style_options']['columns'] = array(
  'timestamp' => 'title_1',
  'title_1' => 'title_1',
  'name' => 'name',
  'created' => 'created',
  'last_updated' => 'last_updated',
  'edit_node' => 'edit_node',
  'status' => 'status',
);
$handler->display->display_options['style_options']['default'] = '-1';
$handler->display->display_options['style_options']['info'] = array(
  'timestamp' => array(
    'align' => '',
    'separator' => '',
  ),
  'title_1' => array(
    'sortable' => 1,
    'default_sort_order' => 'asc',
    'align' => '',
    'separator' => '',
  ),
  'name' => array(
    'sortable' => 1,
    'default_sort_order' => 'asc',
    'align' => '',
    'separator' => '',
  ),
  'created' => array(
    'sortable' => 1,
    'default_sort_order' => 'asc',
    'align' => '',
    'separator' => '',
  ),
  'last_updated' => array(
    'sortable' => 1,
    'default_sort_order' => 'asc',
    'align' => '',
    'separator' => '',
  ),
  'edit_node' => array(
    'align' => '',
    'separator' => '',
  ),
  'status' => array(
    'sortable' => 1,
    'default_sort_order' => 'asc',
    'align' => '',
    'separator' => '',
  ),
);
$handler->display->display_options['style_options']['override'] = 1;
$handler->display->display_options['style_options']['sticky'] = 0;
$handler->display->display_options['style_options']['empty_table'] = 0;
/* Relationship: Content: Author */
$handler->display->display_options['relationships']['uid']['id'] = 'uid';
$handler->display->display_options['relationships']['uid']['table'] = 'node';
$handler->display->display_options['relationships']['uid']['field'] = 'uid';
$handler->display->display_options['relationships']['uid']['required'] = 1;
/* Field: Content: Has new content */
$handler->display->display_options['fields']['timestamp']['id'] = 'timestamp';
$handler->display->display_options['fields']['timestamp']['table'] = 'history';
$handler->display->display_options['fields']['timestamp']['field'] = 'timestamp';
$handler->display->display_options['fields']['timestamp']['label'] = '';
$handler->display->display_options['fields']['timestamp']['alter']['alter_text'] = 0;
$handler->display->display_options['fields']['timestamp']['alter']['make_link'] = 0;
$handler->display->display_options['fields']['timestamp']['alter']['absolute'] = 0;
$handler->display->display_options['fields']['timestamp']['alter']['external'] = 0;
$handler->display->display_options['fields']['timestamp']['alter']['replace_spaces'] = 0;
$handler->display->display_options['fields']['timestamp']['alter']['trim_whitespace'] = 0;
$handler->display->display_options['fields']['timestamp']['alter']['nl2br'] = 0;
$handler->display->display_options['fields']['timestamp']['alter']['word_boundary'] = 1;
$handler->display->display_options['fields']['timestamp']['alter']['ellipsis'] = 1;
$handler->display->display_options['fields']['timestamp']['alter']['strip_tags'] = 0;
$handler->display->display_options['fields']['timestamp']['alter']['trim'] = 0;
$handler->display->display_options['fields']['timestamp']['alter']['html'] = 0;
$handler->display->display_options['fields']['timestamp']['element_label_colon'] = FALSE;
$handler->display->display_options['fields']['timestamp']['element_default_classes'] = 1;
$handler->display->display_options['fields']['timestamp']['hide_empty'] = 0;
$handler->display->display_options['fields']['timestamp']['empty_zero'] = 0;
$handler->display->display_options['fields']['timestamp']['hide_alter_empty'] = 0;
$handler->display->display_options['fields']['timestamp']['link_to_node'] = 0;
$handler->display->display_options['fields']['timestamp']['comments'] = 0;
/* Field: Content: Title */
$handler->display->display_options['fields']['title_1']['id'] = 'title_1';
$handler->display->display_options['fields']['title_1']['table'] = 'node';
$handler->display->display_options['fields']['title_1']['field'] = 'title';
$handler->display->display_options['fields']['title_1']['label'] = 'Name';
$handler->display->display_options['fields']['title_1']['alter']['alter_text'] = 0;
$handler->display->display_options['fields']['title_1']['alter']['make_link'] = 0;
$handler->display->display_options['fields']['title_1']['alter']['absolute'] = 0;
$handler->display->display_options['fields']['title_1']['alter']['external'] = 0;
$handler->display->display_options['fields']['title_1']['alter']['replace_spaces'] = 0;
$handler->display->display_options['fields']['title_1']['alter']['trim_whitespace'] = 0;
$handler->display->display_options['fields']['title_1']['alter']['nl2br'] = 0;
$handler->display->display_options['fields']['title_1']['alter']['word_boundary'] = 1;
$handler->display->display_options['fields']['title_1']['alter']['ellipsis'] = 1;
$handler->display->display_options['fields']['title_1']['alter']['strip_tags'] = 0;
$handler->display->display_options['fields']['title_1']['alter']['trim'] = 0;
$handler->display->display_options['fields']['title_1']['alter']['html'] = 0;
$handler->display->display_options['fields']['title_1']['element_label_colon'] = 1;
$handler->display->display_options['fields']['title_1']['element_default_classes'] = 1;
$handler->display->display_options['fields']['title_1']['hide_empty'] = 0;
$handler->display->display_options['fields']['title_1']['empty_zero'] = 0;
$handler->display->display_options['fields']['title_1']['hide_alter_empty'] = 0;
$handler->display->display_options['fields']['title_1']['link_to_node'] = 1;
/* Field: User: Name */
$handler->display->display_options['fields']['name']['id'] = 'name';
$handler->display->display_options['fields']['name']['table'] = 'users';
$handler->display->display_options['fields']['name']['field'] = 'name';
$handler->display->display_options['fields']['name']['relationship'] = 'uid';
$handler->display->display_options['fields']['name']['label'] = 'Player';
$handler->display->display_options['fields']['name']['alter']['alter_text'] = 0;
$handler->display->display_options['fields']['name']['alter']['make_link'] = 0;
$handler->display->display_options['fields']['name']['alter']['absolute'] = 0;
$handler->display->display_options['fields']['name']['alter']['external'] = 0;
$handler->display->display_options['fields']['name']['alter']['replace_spaces'] = 0;
$handler->display->display_options['fields']['name']['alter']['trim_whitespace'] = 0;
$handler->display->display_options['fields']['name']['alter']['nl2br'] = 0;
$handler->display->display_options['fields']['name']['alter']['word_boundary'] = 1;
$handler->display->display_options['fields']['name']['alter']['ellipsis'] = 1;
$handler->display->display_options['fields']['name']['alter']['strip_tags'] = 0;
$handler->display->display_options['fields']['name']['alter']['trim'] = 0;
$handler->display->display_options['fields']['name']['alter']['html'] = 0;
$handler->display->display_options['fields']['name']['element_label_colon'] = 1;
$handler->display->display_options['fields']['name']['element_default_classes'] = 1;
$handler->display->display_options['fields']['name']['hide_empty'] = 0;
$handler->display->display_options['fields']['name']['empty_zero'] = 0;
$handler->display->display_options['fields']['name']['hide_alter_empty'] = 0;
$handler->display->display_options['fields']['name']['link_to_user'] = 1;
$handler->display->display_options['fields']['name']['overwrite_anonymous'] = 0;
/* Field: Content: Post date */
$handler->display->display_options['fields']['created']['id'] = 'created';
$handler->display->display_options['fields']['created']['table'] = 'node';
$handler->display->display_options['fields']['created']['field'] = 'created';
$handler->display->display_options['fields']['created']['label'] = 'Posted';
$handler->display->display_options['fields']['created']['alter']['alter_text'] = 0;
$handler->display->display_options['fields']['created']['alter']['make_link'] = 0;
$handler->display->display_options['fields']['created']['alter']['absolute'] = 0;
$handler->display->display_options['fields']['created']['alter']['external'] = 0;
$handler->display->display_options['fields']['created']['alter']['replace_spaces'] = 0;
$handler->display->display_options['fields']['created']['alter']['trim_whitespace'] = 0;
$handler->display->display_options['fields']['created']['alter']['nl2br'] = 0;
$handler->display->display_options['fields']['created']['alter']['word_boundary'] = 1;
$handler->display->display_options['fields']['created']['alter']['ellipsis'] = 1;
$handler->display->display_options['fields']['created']['alter']['strip_tags'] = 0;
$handler->display->display_options['fields']['created']['alter']['trim'] = 0;
$handler->display->display_options['fields']['created']['alter']['html'] = 0;
$handler->display->display_options['fields']['created']['element_label_colon'] = 1;
$handler->display->display_options['fields']['created']['element_default_classes'] = 1;
$handler->display->display_options['fields']['created']['hide_empty'] = 0;
$handler->display->display_options['fields']['created']['empty_zero'] = 0;
$handler->display->display_options['fields']['created']['hide_alter_empty'] = 0;
$handler->display->display_options['fields']['created']['date_format'] = 'short';
/* Field: Content: Updated/commented date */
$handler->display->display_options['fields']['last_updated']['id'] = 'last_updated';
$handler->display->display_options['fields']['last_updated']['table'] = 'node_comment_statistics';
$handler->display->display_options['fields']['last_updated']['field'] = 'last_updated';
$handler->display->display_options['fields']['last_updated']['label'] = 'Last updated';
$handler->display->display_options['fields']['last_updated']['alter']['alter_text'] = 0;
$handler->display->display_options['fields']['last_updated']['alter']['make_link'] = 0;
$handler->display->display_options['fields']['last_updated']['alter']['absolute'] = 0;
$handler->display->display_options['fields']['last_updated']['alter']['external'] = 0;
$handler->display->display_options['fields']['last_updated']['alter']['replace_spaces'] = 0;
$handler->display->display_options['fields']['last_updated']['alter']['trim_whitespace'] = 0;
$handler->display->display_options['fields']['last_updated']['alter']['nl2br'] = 0;
$handler->display->display_options['fields']['last_updated']['alter']['word_boundary'] = 1;
$handler->display->display_options['fields']['last_updated']['alter']['ellipsis'] = 1;
$handler->display->display_options['fields']['last_updated']['alter']['strip_tags'] = 0;
$handler->display->display_options['fields']['last_updated']['alter']['trim'] = 0;
$handler->display->display_options['fields']['last_updated']['alter']['html'] = 0;
$handler->display->display_options['fields']['last_updated']['element_label_colon'] = 1;
$handler->display->display_options['fields']['last_updated']['element_default_classes'] = 1;
$handler->display->display_options['fields']['last_updated']['hide_empty'] = 0;
$handler->display->display_options['fields']['last_updated']['empty_zero'] = 0;
$handler->display->display_options['fields']['last_updated']['hide_alter_empty'] = 0;
$handler->display->display_options['fields']['last_updated']['date_format'] = 'short';
/* Field: Character sheet: Status */
$handler->display->display_options['fields']['status']['id'] = 'status';
$handler->display->display_options['fields']['status']['table'] = 'character_sheet_status';
$handler->display->display_options['fields']['status']['field'] = 'status';
$handler->display->display_options['fields']['status']['alter']['alter_text'] = 0;
$handler->display->display_options['fields']['status']['alter']['make_link'] = 0;
$handler->display->display_options['fields']['status']['alter']['absolute'] = 0;
$handler->display->display_options['fields']['status']['alter']['external'] = 0;
$handler->display->display_options['fields']['status']['alter']['replace_spaces'] = 0;
$handler->display->display_options['fields']['status']['alter']['trim_whitespace'] = 0;
$handler->display->display_options['fields']['status']['alter']['nl2br'] = 0;
$handler->display->display_options['fields']['status']['alter']['word_boundary'] = 1;
$handler->display->display_options['fields']['status']['alter']['ellipsis'] = 1;
$handler->display->display_options['fields']['status']['alter']['strip_tags'] = 0;
$handler->display->display_options['fields']['status']['alter']['trim'] = 0;
$handler->display->display_options['fields']['status']['alter']['html'] = 0;
$handler->display->display_options['fields']['status']['element_label_colon'] = 1;
$handler->display->display_options['fields']['status']['element_default_classes'] = 1;
$handler->display->display_options['fields']['status']['hide_empty'] = 0;
$handler->display->display_options['fields']['status']['empty_zero'] = 0;
$handler->display->display_options['fields']['status']['hide_alter_empty'] = 0;
/* Field: Content: Edit link */
$handler->display->display_options['fields']['edit_node']['id'] = 'edit_node';
$handler->display->display_options['fields']['edit_node']['table'] = 'node';
$handler->display->display_options['fields']['edit_node']['field'] = 'edit_node';
$handler->display->display_options['fields']['edit_node']['label'] = 'edit';
$handler->display->display_options['fields']['edit_node']['alter']['alter_text'] = 0;
$handler->display->display_options['fields']['edit_node']['alter']['make_link'] = 0;
$handler->display->display_options['fields']['edit_node']['alter']['absolute'] = 0;
$handler->display->display_options['fields']['edit_node']['alter']['external'] = 0;
$handler->display->display_options['fields']['edit_node']['alter']['replace_spaces'] = 0;
$handler->display->display_options['fields']['edit_node']['alter']['trim_whitespace'] = 0;
$handler->display->display_options['fields']['edit_node']['alter']['nl2br'] = 0;
$handler->display->display_options['fields']['edit_node']['alter']['word_boundary'] = 1;
$handler->display->display_options['fields']['edit_node']['alter']['ellipsis'] = 1;
$handler->display->display_options['fields']['edit_node']['alter']['strip_tags'] = 0;
$handler->display->display_options['fields']['edit_node']['alter']['trim'] = 0;
$handler->display->display_options['fields']['edit_node']['alter']['html'] = 0;
$handler->display->display_options['fields']['edit_node']['element_label_colon'] = 1;
$handler->display->display_options['fields']['edit_node']['element_default_classes'] = 1;
$handler->display->display_options['fields']['edit_node']['hide_empty'] = 0;
$handler->display->display_options['fields']['edit_node']['empty_zero'] = 0;
$handler->display->display_options['fields']['edit_node']['hide_alter_empty'] = 0;
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
$handler->display->display_options['filters']['status']['group'] = 0;
$handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
/* Filter criterion: User: Current */
$handler->display->display_options['filters']['uid_current']['id'] = 'uid_current';
$handler->display->display_options['filters']['uid_current']['table'] = 'users';
$handler->display->display_options['filters']['uid_current']['field'] = 'uid_current';
$handler->display->display_options['filters']['uid_current']['relationship'] = 'uid';
$handler->display->display_options['filters']['uid_current']['value'] = '1';
$handler->display->display_options['filters']['uid_current']['group'] = 0;
/* Filter criterion: Character sheet: Status */
$handler->display->display_options['filters']['status_1']['id'] = 'status_1';
$handler->display->display_options['filters']['status_1']['table'] = 'character_sheet_status';
$handler->display->display_options['filters']['status_1']['field'] = 'status';
$handler->display->display_options['filters']['status_1']['operator'] = 'not empty';

/* Display: Character sheets */
$handler = $view->new_display('page', 'Character sheets', 'character_sheets');
$handler->display->display_options['defaults']['title'] = FALSE;
$handler->display->display_options['title'] = 'Character sheets';
$handler->display->display_options['display_description'] = 'Character sheets';
$handler->display->display_options['defaults']['filters'] = FALSE;
/* Filter criterion: Content: Published */
$handler->display->display_options['filters']['status']['id'] = 'status';
$handler->display->display_options['filters']['status']['table'] = 'node';
$handler->display->display_options['filters']['status']['field'] = 'status';
$handler->display->display_options['filters']['status']['value'] = 1;
$handler->display->display_options['filters']['status']['group'] = 0;
$handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
/* Filter criterion: Character sheet: Status */
$handler->display->display_options['filters']['status_1']['id'] = 'status_1';
$handler->display->display_options['filters']['status_1']['table'] = 'character_sheet_status';
$handler->display->display_options['filters']['status_1']['field'] = 'status';
$handler->display->display_options['filters']['status_1']['operator'] = 'contains';
$handler->display->display_options['filters']['status_1']['value'] = 'approved';
$handler->display->display_options['filters']['status_1']['group'] = 0;
$handler->display->display_options['path'] = 'character-sheets';
$handler->display->display_options['menu']['title'] = 'Character sheet moderation';
$handler->display->display_options['menu']['weight'] = '0';
$handler->display->display_options['menu']['name'] = 'main-menu';

/* Display: My character sheets */
$handler = $view->new_display('page', 'My character sheets', 'my_character_sheets');
$handler->display->display_options['defaults']['title'] = FALSE;
$handler->display->display_options['title'] = 'My character sheets';
$handler->display->display_options['display_description'] = 'My character sheets';
$handler->display->display_options['defaults']['filters'] = FALSE;
/* Filter criterion: Content: Published */
$handler->display->display_options['filters']['status']['id'] = 'status';
$handler->display->display_options['filters']['status']['table'] = 'node';
$handler->display->display_options['filters']['status']['field'] = 'status';
$handler->display->display_options['filters']['status']['value'] = 1;
$handler->display->display_options['filters']['status']['group'] = 0;
$handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
/* Filter criterion: User: Current */
$handler->display->display_options['filters']['uid_current']['id'] = 'uid_current';
$handler->display->display_options['filters']['uid_current']['table'] = 'users';
$handler->display->display_options['filters']['uid_current']['field'] = 'uid_current';
$handler->display->display_options['filters']['uid_current']['relationship'] = 'uid';
$handler->display->display_options['filters']['uid_current']['value'] = '1';
$handler->display->display_options['filters']['uid_current']['group'] = 0;
/* Filter criterion: Character sheet: Status */
$handler->display->display_options['filters']['status_1']['id'] = 'status_1';
$handler->display->display_options['filters']['status_1']['table'] = 'character_sheet_status';
$handler->display->display_options['filters']['status_1']['field'] = 'status';
$handler->display->display_options['filters']['status_1']['operator'] = 'not empty';
$handler->display->display_options['path'] = 'character-sheets/mine';
$handler->display->display_options['menu']['title'] = 'My character sheets';
$handler->display->display_options['menu']['description'] = 'My character sheets';
$handler->display->display_options['menu']['weight'] = '0';

/* Display: Moderate character sheets */
$handler = $view->new_display('page', 'Moderate character sheets', 'moderate_character_sheets');
$handler->display->display_options['defaults']['title'] = FALSE;
$handler->display->display_options['title'] = 'Moderate character sheets';
$handler->display->display_options['display_description'] = 'Moderate character sheets';
$handler->display->display_options['defaults']['filters'] = FALSE;
/* Filter criterion: Content: Published */
$handler->display->display_options['filters']['status']['id'] = 'status';
$handler->display->display_options['filters']['status']['table'] = 'node';
$handler->display->display_options['filters']['status']['field'] = 'status';
$handler->display->display_options['filters']['status']['value'] = 1;
$handler->display->display_options['filters']['status']['group'] = 0;
$handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
/* Filter criterion: Character sheet: Status */
$handler->display->display_options['filters']['status_1']['id'] = 'status_1';
$handler->display->display_options['filters']['status_1']['table'] = 'character_sheet_status';
$handler->display->display_options['filters']['status_1']['field'] = 'status';
$handler->display->display_options['filters']['status_1']['operator'] = 'not empty';
$handler->display->display_options['filters']['status_1']['group'] = 0;
$handler->display->display_options['path'] = 'character-sheets/moderate';
  // END COPY AND PASTE

  $views[$view->name] = $view;

  // START COPY AND PASTE
$view = new view;
$view->name = 'character_sheet_moderation_log';
$view->description = 'Character sheet moderation log';
$view->tag = 'default';
$view->base_table = 'character_sheet_log';
$view->human_name = 'Character sheet moderation log';
$view->core = 7;
$view->api_version = '3.0-alpha1';
$view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

/* Display: Master */
$handler = $view->new_display('default', 'Master', 'default');
$handler->display->display_options['title'] = 'Character sheet moderation log';
$handler->display->display_options['access']['type'] = 'perm';
$handler->display->display_options['access']['perm'] = 'moderate character sheet';
$handler->display->display_options['cache']['type'] = 'none';
$handler->display->display_options['query']['type'] = 'views_query';
$handler->display->display_options['query']['options']['query_comment'] = FALSE;
$handler->display->display_options['exposed_form']['type'] = 'basic';
$handler->display->display_options['pager']['type'] = 'full';
$handler->display->display_options['pager']['options']['items_per_page'] = '35';
$handler->display->display_options['style_plugin'] = 'table';
$handler->display->display_options['style_options']['columns'] = array(
  'lid' => 'lid',
  'type' => 'type',
  'title' => 'title',
  'name_1' => 'name_1',
  'message' => 'message',
  'name' => 'name',
  'timestamp' => 'timestamp',
  'hostname' => 'hostname',
);
$handler->display->display_options['style_options']['default'] = '-1';
$handler->display->display_options['style_options']['info'] = array(
  'lid' => array(
    'sortable' => 1,
    'default_sort_order' => 'asc',
    'align' => '',
    'separator' => '',
  ),
  'type' => array(
    'sortable' => 1,
    'default_sort_order' => 'asc',
    'align' => '',
    'separator' => '',
  ),
  'title' => array(
    'sortable' => 1,
    'default_sort_order' => 'asc',
    'align' => '',
    'separator' => '',
  ),
  'name_1' => array(
    'sortable' => 0,
    'default_sort_order' => 'asc',
    'align' => '',
    'separator' => '',
  ),
  'message' => array(
    'sortable' => 0,
    'default_sort_order' => 'asc',
    'align' => '',
    'separator' => '',
  ),
  'name' => array(
    'sortable' => 0,
    'default_sort_order' => 'asc',
    'align' => '',
    'separator' => '',
  ),
  'timestamp' => array(
    'sortable' => 1,
    'default_sort_order' => 'asc',
    'align' => '',
    'separator' => '',
  ),
  'hostname' => array(
    'sortable' => 1,
    'default_sort_order' => 'asc',
    'align' => '',
    'separator' => '',
  ),
);
$handler->display->display_options['style_options']['override'] = 1;
$handler->display->display_options['style_options']['sticky'] = 0;
$handler->display->display_options['style_options']['empty_table'] = 0;
/* Relationship: Character sheet log: Character sheet log node */
$handler->display->display_options['relationships']['nid']['id'] = 'nid';
$handler->display->display_options['relationships']['nid']['table'] = 'character_sheet_log';
$handler->display->display_options['relationships']['nid']['field'] = 'nid';
$handler->display->display_options['relationships']['nid']['required'] = 0;
/* Relationship: Character sheet log: Character sheet log user */
$handler->display->display_options['relationships']['uid']['id'] = 'uid';
$handler->display->display_options['relationships']['uid']['table'] = 'character_sheet_log';
$handler->display->display_options['relationships']['uid']['field'] = 'uid';
$handler->display->display_options['relationships']['uid']['required'] = 0;
/* Relationship: Content: Author */
$handler->display->display_options['relationships']['uid_1']['id'] = 'uid_1';
$handler->display->display_options['relationships']['uid_1']['table'] = 'node';
$handler->display->display_options['relationships']['uid_1']['field'] = 'uid';
$handler->display->display_options['relationships']['uid_1']['relationship'] = 'nid';
$handler->display->display_options['relationships']['uid_1']['required'] = 0;
/* Field: Character sheet log: Log ID */
$handler->display->display_options['fields']['lid']['id'] = 'lid';
$handler->display->display_options['fields']['lid']['table'] = 'character_sheet_log';
$handler->display->display_options['fields']['lid']['field'] = 'lid';
/* Field: Character sheet log: Log type */
$handler->display->display_options['fields']['type']['id'] = 'type';
$handler->display->display_options['fields']['type']['table'] = 'character_sheet_log';
$handler->display->display_options['fields']['type']['field'] = 'type';
$handler->display->display_options['fields']['type']['alter']['alter_text'] = 0;
$handler->display->display_options['fields']['type']['alter']['make_link'] = 0;
$handler->display->display_options['fields']['type']['alter']['absolute'] = 0;
$handler->display->display_options['fields']['type']['alter']['external'] = 0;
$handler->display->display_options['fields']['type']['alter']['replace_spaces'] = 0;
$handler->display->display_options['fields']['type']['alter']['trim_whitespace'] = 0;
$handler->display->display_options['fields']['type']['alter']['nl2br'] = 0;
$handler->display->display_options['fields']['type']['alter']['word_boundary'] = 1;
$handler->display->display_options['fields']['type']['alter']['ellipsis'] = 1;
$handler->display->display_options['fields']['type']['alter']['strip_tags'] = 0;
$handler->display->display_options['fields']['type']['alter']['trim'] = 0;
$handler->display->display_options['fields']['type']['alter']['html'] = 0;
$handler->display->display_options['fields']['type']['element_label_colon'] = 1;
$handler->display->display_options['fields']['type']['element_default_classes'] = 1;
$handler->display->display_options['fields']['type']['hide_empty'] = 0;
$handler->display->display_options['fields']['type']['empty_zero'] = 0;
$handler->display->display_options['fields']['type']['hide_alter_empty'] = 0;
/* Field: Content: Title */
$handler->display->display_options['fields']['title']['id'] = 'title';
$handler->display->display_options['fields']['title']['table'] = 'node';
$handler->display->display_options['fields']['title']['field'] = 'title';
$handler->display->display_options['fields']['title']['relationship'] = 'nid';
$handler->display->display_options['fields']['title']['alter']['alter_text'] = 0;
$handler->display->display_options['fields']['title']['alter']['make_link'] = 0;
$handler->display->display_options['fields']['title']['alter']['absolute'] = 0;
$handler->display->display_options['fields']['title']['alter']['external'] = 0;
$handler->display->display_options['fields']['title']['alter']['replace_spaces'] = 0;
$handler->display->display_options['fields']['title']['alter']['trim_whitespace'] = 0;
$handler->display->display_options['fields']['title']['alter']['nl2br'] = 0;
$handler->display->display_options['fields']['title']['alter']['word_boundary'] = 1;
$handler->display->display_options['fields']['title']['alter']['ellipsis'] = 1;
$handler->display->display_options['fields']['title']['alter']['strip_tags'] = 0;
$handler->display->display_options['fields']['title']['alter']['trim'] = 0;
$handler->display->display_options['fields']['title']['alter']['html'] = 0;
$handler->display->display_options['fields']['title']['element_label_colon'] = 1;
$handler->display->display_options['fields']['title']['element_default_classes'] = 1;
$handler->display->display_options['fields']['title']['hide_empty'] = 0;
$handler->display->display_options['fields']['title']['empty_zero'] = 0;
$handler->display->display_options['fields']['title']['hide_alter_empty'] = 0;
$handler->display->display_options['fields']['title']['link_to_node'] = 1;
/* Field: User: Name */
$handler->display->display_options['fields']['name_1']['id'] = 'name_1';
$handler->display->display_options['fields']['name_1']['table'] = 'users';
$handler->display->display_options['fields']['name_1']['field'] = 'name';
$handler->display->display_options['fields']['name_1']['relationship'] = 'uid_1';
$handler->display->display_options['fields']['name_1']['label'] = 'Player';
$handler->display->display_options['fields']['name_1']['alter']['alter_text'] = 0;
$handler->display->display_options['fields']['name_1']['alter']['make_link'] = 0;
$handler->display->display_options['fields']['name_1']['alter']['absolute'] = 0;
$handler->display->display_options['fields']['name_1']['alter']['external'] = 0;
$handler->display->display_options['fields']['name_1']['alter']['replace_spaces'] = 0;
$handler->display->display_options['fields']['name_1']['alter']['trim_whitespace'] = 0;
$handler->display->display_options['fields']['name_1']['alter']['nl2br'] = 0;
$handler->display->display_options['fields']['name_1']['alter']['word_boundary'] = 1;
$handler->display->display_options['fields']['name_1']['alter']['ellipsis'] = 1;
$handler->display->display_options['fields']['name_1']['alter']['strip_tags'] = 0;
$handler->display->display_options['fields']['name_1']['alter']['trim'] = 0;
$handler->display->display_options['fields']['name_1']['alter']['html'] = 0;
$handler->display->display_options['fields']['name_1']['element_label_colon'] = 1;
$handler->display->display_options['fields']['name_1']['element_default_classes'] = 1;
$handler->display->display_options['fields']['name_1']['hide_empty'] = 0;
$handler->display->display_options['fields']['name_1']['empty_zero'] = 0;
$handler->display->display_options['fields']['name_1']['hide_alter_empty'] = 0;
$handler->display->display_options['fields']['name_1']['link_to_user'] = 1;
$handler->display->display_options['fields']['name_1']['overwrite_anonymous'] = 0;
/* Field: Character sheet log: Message */
$handler->display->display_options['fields']['message']['id'] = 'message';
$handler->display->display_options['fields']['message']['table'] = 'character_sheet_log';
$handler->display->display_options['fields']['message']['field'] = 'message';
$handler->display->display_options['fields']['message']['alter']['alter_text'] = 0;
$handler->display->display_options['fields']['message']['alter']['make_link'] = 0;
$handler->display->display_options['fields']['message']['alter']['absolute'] = 0;
$handler->display->display_options['fields']['message']['alter']['external'] = 0;
$handler->display->display_options['fields']['message']['alter']['replace_spaces'] = 0;
$handler->display->display_options['fields']['message']['alter']['trim_whitespace'] = 0;
$handler->display->display_options['fields']['message']['alter']['nl2br'] = 0;
$handler->display->display_options['fields']['message']['alter']['word_boundary'] = 1;
$handler->display->display_options['fields']['message']['alter']['ellipsis'] = 1;
$handler->display->display_options['fields']['message']['alter']['strip_tags'] = 0;
$handler->display->display_options['fields']['message']['alter']['trim'] = 0;
$handler->display->display_options['fields']['message']['alter']['html'] = 0;
$handler->display->display_options['fields']['message']['element_label_colon'] = 1;
$handler->display->display_options['fields']['message']['element_default_classes'] = 1;
$handler->display->display_options['fields']['message']['hide_empty'] = 0;
$handler->display->display_options['fields']['message']['empty_zero'] = 0;
$handler->display->display_options['fields']['message']['hide_alter_empty'] = 0;
/* Field: User: Name */
$handler->display->display_options['fields']['name']['id'] = 'name';
$handler->display->display_options['fields']['name']['table'] = 'users';
$handler->display->display_options['fields']['name']['field'] = 'name';
$handler->display->display_options['fields']['name']['relationship'] = 'uid';
$handler->display->display_options['fields']['name']['label'] = 'Logged user';
$handler->display->display_options['fields']['name']['alter']['alter_text'] = 0;
$handler->display->display_options['fields']['name']['alter']['make_link'] = 0;
$handler->display->display_options['fields']['name']['alter']['absolute'] = 0;
$handler->display->display_options['fields']['name']['alter']['external'] = 0;
$handler->display->display_options['fields']['name']['alter']['replace_spaces'] = 0;
$handler->display->display_options['fields']['name']['alter']['trim_whitespace'] = 0;
$handler->display->display_options['fields']['name']['alter']['nl2br'] = 0;
$handler->display->display_options['fields']['name']['alter']['word_boundary'] = 1;
$handler->display->display_options['fields']['name']['alter']['ellipsis'] = 1;
$handler->display->display_options['fields']['name']['alter']['strip_tags'] = 0;
$handler->display->display_options['fields']['name']['alter']['trim'] = 0;
$handler->display->display_options['fields']['name']['alter']['html'] = 0;
$handler->display->display_options['fields']['name']['element_label_colon'] = 1;
$handler->display->display_options['fields']['name']['element_default_classes'] = 1;
$handler->display->display_options['fields']['name']['hide_empty'] = 0;
$handler->display->display_options['fields']['name']['empty_zero'] = 0;
$handler->display->display_options['fields']['name']['hide_alter_empty'] = 0;
$handler->display->display_options['fields']['name']['link_to_user'] = 1;
$handler->display->display_options['fields']['name']['overwrite_anonymous'] = 0;
/* Field: Character sheet log: Timestamp */
$handler->display->display_options['fields']['timestamp']['id'] = 'timestamp';
$handler->display->display_options['fields']['timestamp']['table'] = 'character_sheet_log';
$handler->display->display_options['fields']['timestamp']['field'] = 'timestamp';
$handler->display->display_options['fields']['timestamp']['alter']['alter_text'] = 0;
$handler->display->display_options['fields']['timestamp']['alter']['make_link'] = 0;
$handler->display->display_options['fields']['timestamp']['alter']['absolute'] = 0;
$handler->display->display_options['fields']['timestamp']['alter']['external'] = 0;
$handler->display->display_options['fields']['timestamp']['alter']['replace_spaces'] = 0;
$handler->display->display_options['fields']['timestamp']['alter']['trim_whitespace'] = 0;
$handler->display->display_options['fields']['timestamp']['alter']['nl2br'] = 0;
$handler->display->display_options['fields']['timestamp']['alter']['word_boundary'] = 1;
$handler->display->display_options['fields']['timestamp']['alter']['ellipsis'] = 1;
$handler->display->display_options['fields']['timestamp']['alter']['strip_tags'] = 0;
$handler->display->display_options['fields']['timestamp']['alter']['trim'] = 0;
$handler->display->display_options['fields']['timestamp']['alter']['html'] = 0;
$handler->display->display_options['fields']['timestamp']['element_label_colon'] = 1;
$handler->display->display_options['fields']['timestamp']['element_default_classes'] = 1;
$handler->display->display_options['fields']['timestamp']['hide_empty'] = 0;
$handler->display->display_options['fields']['timestamp']['empty_zero'] = 0;
$handler->display->display_options['fields']['timestamp']['hide_alter_empty'] = 0;
$handler->display->display_options['fields']['timestamp']['date_format'] = 'long';
/* Field: Character sheet log: Hostname */
$handler->display->display_options['fields']['hostname']['id'] = 'hostname';
$handler->display->display_options['fields']['hostname']['table'] = 'character_sheet_log';
$handler->display->display_options['fields']['hostname']['field'] = 'hostname';
$handler->display->display_options['fields']['hostname']['alter']['alter_text'] = 0;
$handler->display->display_options['fields']['hostname']['alter']['make_link'] = 0;
$handler->display->display_options['fields']['hostname']['alter']['absolute'] = 0;
$handler->display->display_options['fields']['hostname']['alter']['external'] = 0;
$handler->display->display_options['fields']['hostname']['alter']['replace_spaces'] = 0;
$handler->display->display_options['fields']['hostname']['alter']['trim_whitespace'] = 0;
$handler->display->display_options['fields']['hostname']['alter']['nl2br'] = 0;
$handler->display->display_options['fields']['hostname']['alter']['word_boundary'] = 1;
$handler->display->display_options['fields']['hostname']['alter']['ellipsis'] = 1;
$handler->display->display_options['fields']['hostname']['alter']['strip_tags'] = 0;
$handler->display->display_options['fields']['hostname']['alter']['trim'] = 0;
$handler->display->display_options['fields']['hostname']['alter']['html'] = 0;
$handler->display->display_options['fields']['hostname']['element_label_colon'] = 1;
$handler->display->display_options['fields']['hostname']['element_default_classes'] = 1;
$handler->display->display_options['fields']['hostname']['hide_empty'] = 0;
$handler->display->display_options['fields']['hostname']['empty_zero'] = 0;
$handler->display->display_options['fields']['hostname']['hide_alter_empty'] = 0;
/* Sort criterion: Character sheet log: Timestamp */
$handler->display->display_options['sorts']['timestamp']['id'] = 'timestamp';
$handler->display->display_options['sorts']['timestamp']['table'] = 'character_sheet_log';
$handler->display->display_options['sorts']['timestamp']['field'] = 'timestamp';
$handler->display->display_options['sorts']['timestamp']['order'] = 'DESC';

/* Display: Page */
$handler = $view->new_display('page', 'Page', 'character_sheets_moderation_log');
$handler->display->display_options['path'] = 'character-sheets/moderate/log';
$handler->display->display_options['menu']['title'] = 'Moderation log';
$handler->display->display_options['menu']['weight'] = '0';
$handler->display->display_options['menu']['name'] = 'main-menu';
  // END COPY AND PASTE

  $views[$view->name] = $view;
  return $views;
}
