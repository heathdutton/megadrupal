<?php
/**
 * @file
 * View the photo albums migrated from Gallery2.
 */

/**
 * Implements hook_views_default_views().
 */
function g2migrate_views_default_views() {
  $export = array();

  $view = new view();
  $view->name = 'g2migrate_albums';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'node';
  $view->human_name = 'G2migrate Albums';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Photo Gallery';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'mini';
  $handler->display->display_options['pager']['options']['items_per_page'] = '16';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['pager']['options']['id'] = '0';
  $handler->display->display_options['style_plugin'] = 'default';
  $handler->display->display_options['style_options']['row_class'] = 'g2migrate-item';
  $handler->display->display_options['style_options']['default_row_class'] = FALSE;
  $handler->display->display_options['style_options']['row_class_special'] = FALSE;
  $handler->display->display_options['row_plugin'] = 'fields';
  $handler->display->display_options['row_options']['inline'] = array(
    'title' => 'title',
    'body' => 'body',
    'field_week' => 'field_week',
    'field_yyyy' => 'field_yyyy',
    'field_g2_keywords' => 'field_g2_keywords',
    'field_type' => 'field_type',
  );
  $handler->display->display_options['row_options']['hide_empty'] = TRUE;
  /* No results behavior: Global: Text area */
  $handler->display->display_options['empty']['area']['id'] = 'area';
  $handler->display->display_options['empty']['area']['table'] = 'views';
  $handler->display->display_options['empty']['area']['field'] = 'area';
  $handler->display->display_options['empty']['area']['empty'] = TRUE;
  $handler->display->display_options['empty']['area']['content'] = '<p>Sorry, there are no photos for this combination of choices.  You can try again using the <a href="#threemile-photo-gallery-breadcrumb-year-form">select lists</a> above.</p>';
  $handler->display->display_options['empty']['area']['format'] = 'rich_text';
  /* Field: Global: View result counter */
  $handler->display->display_options['fields']['counter']['id'] = 'counter';
  $handler->display->display_options['fields']['counter']['table'] = 'views';
  $handler->display->display_options['fields']['counter']['field'] = 'counter';
  $handler->display->display_options['fields']['counter']['exclude'] = TRUE;
  $handler->display->display_options['fields']['counter']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['counter']['alter']['text'] = '[counter]';
  $handler->display->display_options['fields']['counter']['counter_start'] = '0';
  /* Field: Global: Math expression */
  $handler->display->display_options['fields']['expression']['id'] = 'expression';
  $handler->display->display_options['fields']['expression']['table'] = 'views';
  $handler->display->display_options['fields']['expression']['field'] = 'expression';
  $handler->display->display_options['fields']['expression']['label'] = 'Current page';
  $handler->display->display_options['fields']['expression']['exclude'] = TRUE;
  $handler->display->display_options['fields']['expression']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['expression']['precision'] = '0';
  $handler->display->display_options['fields']['expression']['expression'] = '[counter]';
  /* Field: Content: Image */
  $handler->display->display_options['fields']['field_g2migrate_image']['id'] = 'field_g2migrate_image';
  $handler->display->display_options['fields']['field_g2migrate_image']['table'] = 'field_data_field_g2migrate_image';
  $handler->display->display_options['fields']['field_g2migrate_image']['field'] = 'field_g2migrate_image';
  $handler->display->display_options['fields']['field_g2migrate_image']['label'] = '';
  $handler->display->display_options['fields']['field_g2migrate_image']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['field_g2migrate_image']['alter']['text'] = '<a id="photo-fid-[field_g2migrate_image-fid]" href="item?page=[expression]#content">
    [field_g2migrate_image]
    </a>';
  $handler->display->display_options['fields']['field_g2migrate_image']['element_type'] = 'div';
  $handler->display->display_options['fields']['field_g2migrate_image']['element_class'] = 'photo';
  $handler->display->display_options['fields']['field_g2migrate_image']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_g2migrate_image']['element_wrapper_type'] = '0';
  $handler->display->display_options['fields']['field_g2migrate_image']['element_default_classes'] = FALSE;
  $handler->display->display_options['fields']['field_g2migrate_image']['click_sort_column'] = 'fid';
  $handler->display->display_options['fields']['field_g2migrate_image']['settings'] = array(
    'image_style' => 'medium',
    'image_link' => '',
  );
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = '';
  $handler->display->display_options['fields']['title']['exclude'] = TRUE;
  $handler->display->display_options['fields']['title']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['title']['alter']['path'] = 'gallery/album/!1/item?page=[expression]#content';
  $handler->display->display_options['fields']['title']['alter']['alt'] = '[body]';
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
  $handler->display->display_options['fields']['title']['element_type'] = 'h6';
  $handler->display->display_options['fields']['title']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['title']['element_wrapper_type'] = '0';
  $handler->display->display_options['fields']['title']['element_wrapper_class'] = 'title';
  $handler->display->display_options['fields']['title']['element_default_classes'] = FALSE;
  $handler->display->display_options['fields']['title']['link_to_node'] = FALSE;
  /* Field: Content: Body */
  $handler->display->display_options['fields']['body']['id'] = 'body';
  $handler->display->display_options['fields']['body']['table'] = 'field_data_body';
  $handler->display->display_options['fields']['body']['field'] = 'body';
  $handler->display->display_options['fields']['body']['label'] = '';
  $handler->display->display_options['fields']['body']['exclude'] = TRUE;
  $handler->display->display_options['fields']['body']['alter']['trim_whitespace'] = TRUE;
  $handler->display->display_options['fields']['body']['alter']['max_length'] = '100';
  $handler->display->display_options['fields']['body']['alter']['strip_tags'] = TRUE;
  $handler->display->display_options['fields']['body']['alter']['trim'] = TRUE;
  $handler->display->display_options['fields']['body']['element_type'] = '0';
  $handler->display->display_options['fields']['body']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['body']['element_wrapper_type'] = '0';
  $handler->display->display_options['fields']['body']['element_default_classes'] = FALSE;
  $handler->display->display_options['fields']['body']['type'] = 'text_summary_or_trimmed';
  $handler->display->display_options['fields']['body']['settings'] = array(
    'trim_length' => '600',
  );
  /* Field: Global: Custom text */
  $handler->display->display_options['fields']['nothing']['id'] = 'nothing';
  $handler->display->display_options['fields']['nothing']['table'] = 'views';
  $handler->display->display_options['fields']['nothing']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing']['label'] = '';
  $handler->display->display_options['fields']['nothing']['alter']['text'] = '[title]';
  $handler->display->display_options['fields']['nothing']['element_type'] = 'div';
  $handler->display->display_options['fields']['nothing']['element_class'] = 'details';
  $handler->display->display_options['fields']['nothing']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['nothing']['element_wrapper_type'] = '0';
  $handler->display->display_options['fields']['nothing']['element_default_classes'] = FALSE;
  /* Sort criterion: Content: Post date */
  $handler->display->display_options['sorts']['created']['id'] = 'created';
  $handler->display->display_options['sorts']['created']['table'] = 'node';
  $handler->display->display_options['sorts']['created']['field'] = 'created';
  $handler->display->display_options['sorts']['created']['order'] = 'DESC';
  $handler->display->display_options['sorts']['created']['expose']['label'] = 'Post date';
  /* Sort criterion: Content: Nid */
  $handler->display->display_options['sorts']['nid']['id'] = 'nid';
  $handler->display->display_options['sorts']['nid']['table'] = 'node';
  $handler->display->display_options['sorts']['nid']['field'] = 'nid';
  $handler->display->display_options['sorts']['nid']['order'] = 'DESC';
  /* Contextual filter: Content: Album (g2migrate_album) */
  $handler->display->display_options['arguments']['g2migrate_album_tid']['id'] = 'g2migrate_album_tid';
  $handler->display->display_options['arguments']['g2migrate_album_tid']['table'] = 'field_data_g2migrate_album';
  $handler->display->display_options['arguments']['g2migrate_album_tid']['field'] = 'g2migrate_album_tid';
  $handler->display->display_options['arguments']['g2migrate_album_tid']['default_action'] = 'not found';
  $handler->display->display_options['arguments']['g2migrate_album_tid']['exception']['title_enable'] = TRUE;
  $handler->display->display_options['arguments']['g2migrate_album_tid']['exception']['title'] = 'All albums';
  $handler->display->display_options['arguments']['g2migrate_album_tid']['title_enable'] = TRUE;
  $handler->display->display_options['arguments']['g2migrate_album_tid']['title'] = 'Photo Gallery (%1)';
  $handler->display->display_options['arguments']['g2migrate_album_tid']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['g2migrate_album_tid']['default_argument_skip_url'] = TRUE;
  $handler->display->display_options['arguments']['g2migrate_album_tid']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['g2migrate_album_tid']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['g2migrate_album_tid']['summary_options']['items_per_page'] = '25';
  $handler->display->display_options['arguments']['g2migrate_album_tid']['specify_validation'] = TRUE;
  $handler->display->display_options['arguments']['g2migrate_album_tid']['validate']['type'] = 'taxonomy_term';
  $handler->display->display_options['arguments']['g2migrate_album_tid']['validate_options']['vocabularies'] = array(
    'g2migrate_album' => 'g2migrate_album',
  );
  $handler->display->display_options['arguments']['g2migrate_album_tid']['validate_options']['type'] = 'tids';
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
    'g2migrate_photo' => 'g2migrate_photo',
  );
  $handler->display->display_options['filters']['type']['group'] = 1;

  /* Display: Album */
  $handler = $view->new_display('page', 'Album', 'page_album');
  $handler->display->display_options['defaults']['empty'] = FALSE;
  /* No results behavior: Global: Text area */
  $handler->display->display_options['empty']['area']['id'] = 'area';
  $handler->display->display_options['empty']['area']['table'] = 'views';
  $handler->display->display_options['empty']['area']['field'] = 'area';
  $handler->display->display_options['empty']['area']['empty'] = TRUE;
  $handler->display->display_options['empty']['area']['content'] = '<p>There are no photos in this album.  You can choose a sub-album using the <a href="#g2migrate-breadcrumb-term-form">select list</a> above.</p>';
  $handler->display->display_options['empty']['area']['format'] = 'filtered_html';
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: Global: View result counter */
  $handler->display->display_options['fields']['counter']['id'] = 'counter';
  $handler->display->display_options['fields']['counter']['table'] = 'views';
  $handler->display->display_options['fields']['counter']['field'] = 'counter';
  $handler->display->display_options['fields']['counter']['exclude'] = TRUE;
  $handler->display->display_options['fields']['counter']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['counter']['alter']['text'] = '[counter]';
  $handler->display->display_options['fields']['counter']['counter_start'] = '0';
  /* Field: Global: Math expression */
  $handler->display->display_options['fields']['expression']['id'] = 'expression';
  $handler->display->display_options['fields']['expression']['table'] = 'views';
  $handler->display->display_options['fields']['expression']['field'] = 'expression';
  $handler->display->display_options['fields']['expression']['label'] = 'Current page';
  $handler->display->display_options['fields']['expression']['exclude'] = TRUE;
  $handler->display->display_options['fields']['expression']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['expression']['precision'] = '0';
  $handler->display->display_options['fields']['expression']['expression'] = '[counter]';
  /* Field: Content: Image */
  $handler->display->display_options['fields']['field_g2migrate_image']['id'] = 'field_g2migrate_image';
  $handler->display->display_options['fields']['field_g2migrate_image']['table'] = 'field_data_field_g2migrate_image';
  $handler->display->display_options['fields']['field_g2migrate_image']['field'] = 'field_g2migrate_image';
  $handler->display->display_options['fields']['field_g2migrate_image']['label'] = '';
  $handler->display->display_options['fields']['field_g2migrate_image']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['field_g2migrate_image']['alter']['text'] = '<a id="photo-fid-[field_g2migrate_image-fid]" href="item?page=[expression]#content">
    [field_g2migrate_image]
    </a>';
  $handler->display->display_options['fields']['field_g2migrate_image']['element_type'] = 'div';
  $handler->display->display_options['fields']['field_g2migrate_image']['element_class'] = 'photo';
  $handler->display->display_options['fields']['field_g2migrate_image']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_g2migrate_image']['element_wrapper_type'] = '0';
  $handler->display->display_options['fields']['field_g2migrate_image']['element_default_classes'] = FALSE;
  $handler->display->display_options['fields']['field_g2migrate_image']['click_sort_column'] = 'fid';
  $handler->display->display_options['fields']['field_g2migrate_image']['settings'] = array(
    'image_style' => 'medium',
    'image_link' => '',
  );
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = '';
  $handler->display->display_options['fields']['title']['exclude'] = TRUE;
  $handler->display->display_options['fields']['title']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['title']['alter']['path'] = 'gallery/album/!1/item?page=[expression]#content';
  $handler->display->display_options['fields']['title']['alter']['alt'] = '[body]';
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
  $handler->display->display_options['fields']['title']['element_type'] = 'h6';
  $handler->display->display_options['fields']['title']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['title']['element_wrapper_type'] = '0';
  $handler->display->display_options['fields']['title']['element_wrapper_class'] = 'title';
  $handler->display->display_options['fields']['title']['element_default_classes'] = FALSE;
  $handler->display->display_options['fields']['title']['link_to_node'] = FALSE;
  /* Field: Content: Body */
  $handler->display->display_options['fields']['body']['id'] = 'body';
  $handler->display->display_options['fields']['body']['table'] = 'field_data_body';
  $handler->display->display_options['fields']['body']['field'] = 'body';
  $handler->display->display_options['fields']['body']['label'] = '';
  $handler->display->display_options['fields']['body']['exclude'] = TRUE;
  $handler->display->display_options['fields']['body']['alter']['trim_whitespace'] = TRUE;
  $handler->display->display_options['fields']['body']['alter']['max_length'] = '100';
  $handler->display->display_options['fields']['body']['alter']['strip_tags'] = TRUE;
  $handler->display->display_options['fields']['body']['alter']['trim'] = TRUE;
  $handler->display->display_options['fields']['body']['element_type'] = '0';
  $handler->display->display_options['fields']['body']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['body']['element_wrapper_type'] = '0';
  $handler->display->display_options['fields']['body']['element_default_classes'] = FALSE;
  $handler->display->display_options['fields']['body']['type'] = 'text_summary_or_trimmed';
  $handler->display->display_options['fields']['body']['settings'] = array(
    'trim_length' => '600',
  );
  /* Field: Global: Custom text */
  $handler->display->display_options['fields']['nothing']['id'] = 'nothing';
  $handler->display->display_options['fields']['nothing']['table'] = 'views';
  $handler->display->display_options['fields']['nothing']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing']['label'] = '';
  $handler->display->display_options['fields']['nothing']['alter']['text'] = '[title]';
  $handler->display->display_options['fields']['nothing']['element_type'] = 'div';
  $handler->display->display_options['fields']['nothing']['element_class'] = 'details';
  $handler->display->display_options['fields']['nothing']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['nothing']['element_wrapper_type'] = '0';
  $handler->display->display_options['fields']['nothing']['element_default_classes'] = FALSE;
  $handler->display->display_options['path'] = 'gallery/album/%/list';

  /* Display: Album item */
  $handler = $view->new_display('page', 'Album item', 'page_item');
  $handler->display->display_options['defaults']['pager'] = FALSE;
  $handler->display->display_options['pager']['type'] = 'g2migrate_mini';
  $handler->display->display_options['pager']['options']['items_per_page'] = '1';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['pager']['options']['id'] = '0';
  $handler->display->display_options['pager']['options']['fragments']['previous'] = 'content';
  $handler->display->display_options['pager']['options']['fragments']['current'] = 'content';
  $handler->display->display_options['pager']['options']['fragments']['next'] = 'content';
  $handler->display->display_options['defaults']['style_plugin'] = FALSE;
  $handler->display->display_options['style_plugin'] = 'default';
  $handler->display->display_options['style_options']['row_class'] = 'g2migrate-item g2migrate-single';
  $handler->display->display_options['style_options']['default_row_class'] = FALSE;
  $handler->display->display_options['style_options']['row_class_special'] = FALSE;
  $handler->display->display_options['defaults']['style_options'] = FALSE;
  $handler->display->display_options['defaults']['row_plugin'] = FALSE;
  $handler->display->display_options['row_plugin'] = 'fields';
  $handler->display->display_options['row_options']['inline'] = array(
    'title' => 'title',
    'body' => 'body',
    'field_week' => 'field_week',
    'field_yyyy' => 'field_yyyy',
    'field_g2_keywords' => 'field_g2_keywords',
    'field_type' => 'field_type',
  );
  $handler->display->display_options['row_options']['hide_empty'] = TRUE;
  $handler->display->display_options['defaults']['row_options'] = FALSE;
  $handler->display->display_options['defaults']['empty'] = FALSE;
  /* No results behavior: Global: Text area */
  $handler->display->display_options['empty']['area']['id'] = 'area';
  $handler->display->display_options['empty']['area']['table'] = 'views';
  $handler->display->display_options['empty']['area']['field'] = 'area';
  $handler->display->display_options['empty']['area']['empty'] = TRUE;
  $handler->display->display_options['empty']['area']['content'] = '<p>Sorry, there is no photos to show.  You can try again using the <a href="#g2migrate-breadcrumb-year-form">select list</a> above or go back to the <a href="list">list view</a>.</p>';
  $handler->display->display_options['empty']['area']['format'] = 'filtered_html';
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: Global: View result counter */
  $handler->display->display_options['fields']['counter']['id'] = 'counter';
  $handler->display->display_options['fields']['counter']['table'] = 'views';
  $handler->display->display_options['fields']['counter']['field'] = 'counter';
  $handler->display->display_options['fields']['counter']['exclude'] = TRUE;
  $handler->display->display_options['fields']['counter']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['counter']['alter']['text'] = '[counter]';
  $handler->display->display_options['fields']['counter']['counter_start'] = '0';
  /* Field: Global: Math expression */
  $handler->display->display_options['fields']['expression']['id'] = 'expression';
  $handler->display->display_options['fields']['expression']['table'] = 'views';
  $handler->display->display_options['fields']['expression']['field'] = 'expression';
  $handler->display->display_options['fields']['expression']['label'] = 'Current page';
  $handler->display->display_options['fields']['expression']['exclude'] = TRUE;
  $handler->display->display_options['fields']['expression']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['expression']['precision'] = '0';
  $handler->display->display_options['fields']['expression']['expression'] = '[counter]';
  /* Field: Content: Image */
  $handler->display->display_options['fields']['field_g2migrate_image']['id'] = 'field_g2migrate_image';
  $handler->display->display_options['fields']['field_g2migrate_image']['table'] = 'field_data_field_g2migrate_image';
  $handler->display->display_options['fields']['field_g2migrate_image']['field'] = 'field_g2migrate_image';
  $handler->display->display_options['fields']['field_g2migrate_image']['label'] = '';
  $handler->display->display_options['fields']['field_g2migrate_image']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['field_g2migrate_image']['alter']['text'] = '<a id="photo-fid-[field_g2migrate_image-fid]" href="item?page=[expression]#content">
    [field_g2migrate_image]
    </a>';
  $handler->display->display_options['fields']['field_g2migrate_image']['element_type'] = 'div';
  $handler->display->display_options['fields']['field_g2migrate_image']['element_class'] = 'photo';
  $handler->display->display_options['fields']['field_g2migrate_image']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_g2migrate_image']['element_wrapper_type'] = '0';
  $handler->display->display_options['fields']['field_g2migrate_image']['element_default_classes'] = FALSE;
  $handler->display->display_options['fields']['field_g2migrate_image']['click_sort_column'] = 'fid';
  $handler->display->display_options['fields']['field_g2migrate_image']['settings'] = array(
    'image_style' => '',
    'image_link' => 'file',
  );
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = '';
  $handler->display->display_options['fields']['title']['exclude'] = TRUE;
  $handler->display->display_options['fields']['title']['alter']['path'] = 'gallery/album/!1/list?page=[expression]#content';
  $handler->display->display_options['fields']['title']['alter']['alt'] = '[body]';
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
  $handler->display->display_options['fields']['title']['element_type'] = 'h6';
  $handler->display->display_options['fields']['title']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['title']['element_wrapper_type'] = '0';
  $handler->display->display_options['fields']['title']['element_wrapper_class'] = 'title';
  $handler->display->display_options['fields']['title']['element_default_classes'] = FALSE;
  $handler->display->display_options['fields']['title']['link_to_node'] = FALSE;
  /* Field: Content: Body */
  $handler->display->display_options['fields']['body']['id'] = 'body';
  $handler->display->display_options['fields']['body']['table'] = 'field_data_body';
  $handler->display->display_options['fields']['body']['field'] = 'body';
  $handler->display->display_options['fields']['body']['label'] = '';
  $handler->display->display_options['fields']['body']['exclude'] = TRUE;
  $handler->display->display_options['fields']['body']['alter']['trim_whitespace'] = TRUE;
  $handler->display->display_options['fields']['body']['alter']['max_length'] = '100';
  $handler->display->display_options['fields']['body']['alter']['strip_tags'] = TRUE;
  $handler->display->display_options['fields']['body']['alter']['trim'] = TRUE;
  $handler->display->display_options['fields']['body']['element_type'] = '0';
  $handler->display->display_options['fields']['body']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['body']['element_wrapper_type'] = '0';
  $handler->display->display_options['fields']['body']['element_default_classes'] = FALSE;
  $handler->display->display_options['fields']['body']['type'] = 'text_summary_or_trimmed';
  $handler->display->display_options['fields']['body']['settings'] = array(
    'trim_length' => '600',
  );
  /* Field: Global: Custom text */
  $handler->display->display_options['fields']['nothing']['id'] = 'nothing';
  $handler->display->display_options['fields']['nothing']['table'] = 'views';
  $handler->display->display_options['fields']['nothing']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing']['label'] = '';
  $handler->display->display_options['fields']['nothing']['alter']['text'] = '[title]';
  $handler->display->display_options['fields']['nothing']['element_type'] = 'div';
  $handler->display->display_options['fields']['nothing']['element_class'] = 'details';
  $handler->display->display_options['fields']['nothing']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['nothing']['element_wrapper_type'] = '0';
  $handler->display->display_options['fields']['nothing']['element_default_classes'] = FALSE;
  $handler->display->display_options['path'] = 'gallery/album/%/item';

  $export['g2migrate_albums'] = $view;

  return $export;
}
