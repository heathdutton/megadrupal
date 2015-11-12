<?php

$view = new view;
$view->name = 'project_release_files';
$view->description = 'List of all files attached to a given release';
$view->tag = 'default';
$view->base_table = 'field_collection_item';
$view->human_name = 'Project release files';
$view->core = 7;
$view->api_version = '3.0';
$view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

/* Display: Master */
$handler = $view->new_display('default', 'Master', 'default');
$handler->display->display_options['access']['type'] = 'none';
$handler->display->display_options['cache']['type'] = 'none';
$handler->display->display_options['query']['type'] = 'views_query';
$handler->display->display_options['query']['options']['query_comment'] = FALSE;
$handler->display->display_options['exposed_form']['type'] = 'basic';
$handler->display->display_options['pager']['type'] = 'none';
$handler->display->display_options['pager']['options']['offset'] = '0';
$handler->display->display_options['style_plugin'] = 'table';
$handler->display->display_options['style_options']['columns'] = array(
  'item_id' => 'item_id',
  'field_release_file' => 'field_release_file',
  'field_release_file_hash' => 'field_release_file_hash',
);
$handler->display->display_options['style_options']['default'] = '-1';
$handler->display->display_options['style_options']['info'] = array(
  'item_id' => array(
    'sortable' => 0,
    'default_sort_order' => 'asc',
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
  'field_release_file' => array(
    'sortable' => 0,
    'default_sort_order' => 'asc',
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
  'field_release_file_hash' => array(
    'sortable' => 0,
    'default_sort_order' => 'asc',
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
);
$handler->display->display_options['style_options']['override'] = 0;
$handler->display->display_options['style_options']['sticky'] = 0;
$handler->display->display_options['style_options']['empty_table'] = 0;
/* Relationship: Field collection item: Release file (field_release_file:fid) */
$handler->display->display_options['relationships']['field_release_file_fid']['id'] = 'field_release_file_fid';
$handler->display->display_options['relationships']['field_release_file_fid']['table'] = 'field_data_field_release_file';
$handler->display->display_options['relationships']['field_release_file_fid']['field'] = 'field_release_file_fid';
$handler->display->display_options['relationships']['field_release_file_fid']['required'] = 1;
/* Relationship: Field collection item: Entity with the Release files (field_release_files) */
$handler->display->display_options['relationships']['field_release_files_node']['id'] = 'field_release_files_node';
$handler->display->display_options['relationships']['field_release_files_node']['table'] = 'field_collection_item';
$handler->display->display_options['relationships']['field_release_files_node']['field'] = 'field_release_files_node';
$handler->display->display_options['relationships']['field_release_files_node']['required'] = 1;
/* Field: Field collection item: Release file */
$handler->display->display_options['fields']['field_release_file']['id'] = 'field_release_file';
$handler->display->display_options['fields']['field_release_file']['table'] = 'field_data_field_release_file';
$handler->display->display_options['fields']['field_release_file']['field'] = 'field_release_file';
$handler->display->display_options['fields']['field_release_file']['label'] = 'Download';
$handler->display->display_options['fields']['field_release_file']['alter']['alter_text'] = 0;
$handler->display->display_options['fields']['field_release_file']['alter']['make_link'] = 0;
$handler->display->display_options['fields']['field_release_file']['alter']['absolute'] = 0;
$handler->display->display_options['fields']['field_release_file']['alter']['external'] = 0;
$handler->display->display_options['fields']['field_release_file']['alter']['replace_spaces'] = 0;
$handler->display->display_options['fields']['field_release_file']['alter']['trim_whitespace'] = 0;
$handler->display->display_options['fields']['field_release_file']['alter']['nl2br'] = 0;
$handler->display->display_options['fields']['field_release_file']['alter']['word_boundary'] = 1;
$handler->display->display_options['fields']['field_release_file']['alter']['ellipsis'] = 1;
$handler->display->display_options['fields']['field_release_file']['alter']['more_link'] = 0;
$handler->display->display_options['fields']['field_release_file']['alter']['strip_tags'] = 0;
$handler->display->display_options['fields']['field_release_file']['alter']['trim'] = 0;
$handler->display->display_options['fields']['field_release_file']['alter']['html'] = 0;
$handler->display->display_options['fields']['field_release_file']['element_label_colon'] = 0;
$handler->display->display_options['fields']['field_release_file']['element_default_classes'] = 1;
$handler->display->display_options['fields']['field_release_file']['hide_empty'] = 0;
$handler->display->display_options['fields']['field_release_file']['empty_zero'] = 0;
$handler->display->display_options['fields']['field_release_file']['hide_alter_empty'] = 1;
$handler->display->display_options['fields']['field_release_file']['click_sort_column'] = 'fid';
$handler->display->display_options['fields']['field_release_file']['field_api_classes'] = 0;
/* Field: File: Size */
$handler->display->display_options['fields']['filesize']['id'] = 'filesize';
$handler->display->display_options['fields']['filesize']['table'] = 'file_managed';
$handler->display->display_options['fields']['filesize']['field'] = 'filesize';
$handler->display->display_options['fields']['filesize']['relationship'] = 'field_release_file_fid';
$handler->display->display_options['fields']['filesize']['alter']['alter_text'] = 0;
$handler->display->display_options['fields']['filesize']['alter']['make_link'] = 0;
$handler->display->display_options['fields']['filesize']['alter']['absolute'] = 0;
$handler->display->display_options['fields']['filesize']['alter']['external'] = 0;
$handler->display->display_options['fields']['filesize']['alter']['replace_spaces'] = 0;
$handler->display->display_options['fields']['filesize']['alter']['trim_whitespace'] = 0;
$handler->display->display_options['fields']['filesize']['alter']['nl2br'] = 0;
$handler->display->display_options['fields']['filesize']['alter']['word_boundary'] = 1;
$handler->display->display_options['fields']['filesize']['alter']['ellipsis'] = 1;
$handler->display->display_options['fields']['filesize']['alter']['more_link'] = 0;
$handler->display->display_options['fields']['filesize']['alter']['strip_tags'] = 0;
$handler->display->display_options['fields']['filesize']['alter']['trim'] = 0;
$handler->display->display_options['fields']['filesize']['alter']['html'] = 0;
$handler->display->display_options['fields']['filesize']['element_label_colon'] = 0;
$handler->display->display_options['fields']['filesize']['element_default_classes'] = 1;
$handler->display->display_options['fields']['filesize']['hide_empty'] = 0;
$handler->display->display_options['fields']['filesize']['empty_zero'] = 0;
$handler->display->display_options['fields']['filesize']['hide_alter_empty'] = 1;
/* Field: Field collection item: Release file hash */
$handler->display->display_options['fields']['field_release_file_hash']['id'] = 'field_release_file_hash';
$handler->display->display_options['fields']['field_release_file_hash']['table'] = 'field_data_field_release_file_hash';
$handler->display->display_options['fields']['field_release_file_hash']['field'] = 'field_release_file_hash';
$handler->display->display_options['fields']['field_release_file_hash']['label'] = 'md5 hash';
$handler->display->display_options['fields']['field_release_file_hash']['alter']['alter_text'] = 0;
$handler->display->display_options['fields']['field_release_file_hash']['alter']['make_link'] = 0;
$handler->display->display_options['fields']['field_release_file_hash']['alter']['absolute'] = 0;
$handler->display->display_options['fields']['field_release_file_hash']['alter']['external'] = 0;
$handler->display->display_options['fields']['field_release_file_hash']['alter']['replace_spaces'] = 0;
$handler->display->display_options['fields']['field_release_file_hash']['alter']['trim_whitespace'] = 0;
$handler->display->display_options['fields']['field_release_file_hash']['alter']['nl2br'] = 0;
$handler->display->display_options['fields']['field_release_file_hash']['alter']['word_boundary'] = 1;
$handler->display->display_options['fields']['field_release_file_hash']['alter']['ellipsis'] = 1;
$handler->display->display_options['fields']['field_release_file_hash']['alter']['more_link'] = 0;
$handler->display->display_options['fields']['field_release_file_hash']['alter']['strip_tags'] = 0;
$handler->display->display_options['fields']['field_release_file_hash']['alter']['trim'] = 0;
$handler->display->display_options['fields']['field_release_file_hash']['alter']['html'] = 0;
$handler->display->display_options['fields']['field_release_file_hash']['element_label_colon'] = 1;
$handler->display->display_options['fields']['field_release_file_hash']['element_default_classes'] = 1;
$handler->display->display_options['fields']['field_release_file_hash']['hide_empty'] = 0;
$handler->display->display_options['fields']['field_release_file_hash']['empty_zero'] = 0;
$handler->display->display_options['fields']['field_release_file_hash']['hide_alter_empty'] = 1;
$handler->display->display_options['fields']['field_release_file_hash']['field_api_classes'] = 0;
/* Sort criterion: File: Name */
$handler->display->display_options['sorts']['filename']['id'] = 'filename';
$handler->display->display_options['sorts']['filename']['table'] = 'file_managed';
$handler->display->display_options['sorts']['filename']['field'] = 'filename';
$handler->display->display_options['sorts']['filename']['relationship'] = 'field_release_file_fid';
/* Contextual filter: Content: Nid */
$handler->display->display_options['arguments']['nid']['id'] = 'nid';
$handler->display->display_options['arguments']['nid']['table'] = 'node';
$handler->display->display_options['arguments']['nid']['field'] = 'nid';
$handler->display->display_options['arguments']['nid']['relationship'] = 'field_release_files_node';
$handler->display->display_options['arguments']['nid']['default_action'] = 'not found';
$handler->display->display_options['arguments']['nid']['default_argument_type'] = 'fixed';
$handler->display->display_options['arguments']['nid']['default_argument_skip_url'] = 0;
$handler->display->display_options['arguments']['nid']['summary']['number_of_records'] = '0';
$handler->display->display_options['arguments']['nid']['summary']['format'] = 'default_summary';
$handler->display->display_options['arguments']['nid']['summary_options']['items_per_page'] = '25';
$handler->display->display_options['arguments']['nid']['break_phrase'] = 0;
$handler->display->display_options['arguments']['nid']['not'] = 0;
