<?php
/**
 * @file
 * storiedmaps_story.views_default.inc
 */

/**
 * Implements hook_views_default_views().
 */
function storiedmaps_story_views_default_views() {
  $export = array();

  $view = new view();
  $view->name = 'story';
  $view->description = 'A list of stories.';
  $view->tag = 'default';
  $view->base_table = 'node';
  $view->human_name = 'Story';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Stories';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '10';
  $handler->display->display_options['style_plugin'] = 'masonry_views_grid';
  $handler->display->display_options['style_options']['masonry_animated'] = 1;
  $handler->display->display_options['style_options']['masonry_resizable'] = 1;
  $handler->display->display_options['style_options']['masonry_center'] = 0;
  $handler->display->display_options['style_options']['masonry_rtl'] = 0;
  $handler->display->display_options['row_plugin'] = 'fields';
  $handler->display->display_options['row_options']['inline'] = array(
    'field_told_by' => 'field_told_by',
    'field_submitted_by' => 'field_submitted_by',
  );
  $handler->display->display_options['row_options']['hide_empty'] = TRUE;
  /* Relationship: Entity Reference: Referenced Entity */
  $handler->display->display_options['relationships']['field_reference_location_target_id']['id'] = 'field_reference_location_target_id';
  $handler->display->display_options['relationships']['field_reference_location_target_id']['table'] = 'field_data_field_reference_location';
  $handler->display->display_options['relationships']['field_reference_location_target_id']['field'] = 'field_reference_location_target_id';
  /* Relationship: Content: Location type (field_location_type) */
  $handler->display->display_options['relationships']['field_location_type_tid']['id'] = 'field_location_type_tid';
  $handler->display->display_options['relationships']['field_location_type_tid']['table'] = 'field_data_field_location_type';
  $handler->display->display_options['relationships']['field_location_type_tid']['field'] = 'field_location_type_tid';
  $handler->display->display_options['relationships']['field_location_type_tid']['relationship'] = 'field_reference_location_target_id';
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = '';
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
  $handler->display->display_options['fields']['title']['element_type'] = 'h2';
  $handler->display->display_options['fields']['title']['element_label_colon'] = FALSE;
  /* Field: Content: Told by */
  $handler->display->display_options['fields']['field_told_by']['id'] = 'field_told_by';
  $handler->display->display_options['fields']['field_told_by']['table'] = 'field_data_field_told_by';
  $handler->display->display_options['fields']['field_told_by']['field'] = 'field_told_by';
  $handler->display->display_options['fields']['field_told_by']['hide_empty'] = TRUE;
  /* Field: Content: Submitted by */
  $handler->display->display_options['fields']['field_submitted_by']['id'] = 'field_submitted_by';
  $handler->display->display_options['fields']['field_submitted_by']['table'] = 'field_data_field_submitted_by';
  $handler->display->display_options['fields']['field_submitted_by']['field'] = 'field_submitted_by';
  $handler->display->display_options['fields']['field_submitted_by']['hide_empty'] = TRUE;
  /* Field: Content: Body */
  $handler->display->display_options['fields']['body']['id'] = 'body';
  $handler->display->display_options['fields']['body']['table'] = 'field_data_body';
  $handler->display->display_options['fields']['body']['field'] = 'body';
  $handler->display->display_options['fields']['body']['label'] = '';
  $handler->display->display_options['fields']['body']['alter']['max_length'] = '500';
  $handler->display->display_options['fields']['body']['alter']['trim'] = TRUE;
  $handler->display->display_options['fields']['body']['alter']['html'] = TRUE;
  $handler->display->display_options['fields']['body']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['body']['type'] = 'text_trimmed';
  $handler->display->display_options['fields']['body']['settings'] = array(
    'trim_length' => '600',
  );
  /* Field: Content: Link */
  $handler->display->display_options['fields']['view_node']['id'] = 'view_node';
  $handler->display->display_options['fields']['view_node']['table'] = 'views_entity_node';
  $handler->display->display_options['fields']['view_node']['field'] = 'view_node';
  $handler->display->display_options['fields']['view_node']['label'] = '';
  $handler->display->display_options['fields']['view_node']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['view_node']['text'] = 'Read more';
  /* Field: Taxonomy term: Location icon */
  $handler->display->display_options['fields']['field_location_icon']['id'] = 'field_location_icon';
  $handler->display->display_options['fields']['field_location_icon']['table'] = 'field_data_field_location_icon';
  $handler->display->display_options['fields']['field_location_icon']['field'] = 'field_location_icon';
  $handler->display->display_options['fields']['field_location_icon']['relationship'] = 'field_location_type_tid';
  $handler->display->display_options['fields']['field_location_icon']['label'] = '';
  $handler->display->display_options['fields']['field_location_icon']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_location_icon']['click_sort_column'] = 'fid';
  $handler->display->display_options['fields']['field_location_icon']['settings'] = array(
    'image_style' => 'thumbnail',
    'image_link' => 'content',
  );
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
  $handler->display->display_options['filters']['status']['group'] = 1;
  $handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
  /* Filter criterion: Content: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'node';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'story' => 'story',
  );

  /* Display: Page: all stories */
  $handler = $view->new_display('page', 'Page: all stories', 'page');
  $handler->display->display_options['defaults']['pager'] = FALSE;
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '20';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['pager']['options']['id'] = '0';
  $handler->display->display_options['pager']['options']['quantity'] = '9';
  $handler->display->display_options['defaults']['style_plugin'] = FALSE;
  $handler->display->display_options['style_plugin'] = 'masonry_views_grid';
  $handler->display->display_options['style_options']['masonry_width'] = '220';
  $handler->display->display_options['style_options']['masonry_animated'] = 1;
  $handler->display->display_options['style_options']['masonry_resizable'] = 1;
  $handler->display->display_options['style_options']['masonry_center'] = 0;
  $handler->display->display_options['style_options']['masonry_gutter'] = '20';
  $handler->display->display_options['style_options']['masonry_rtl'] = 0;
  $handler->display->display_options['defaults']['style_options'] = FALSE;
  $handler->display->display_options['defaults']['row_plugin'] = FALSE;
  $handler->display->display_options['row_plugin'] = 'fields';
  $handler->display->display_options['row_options']['inline'] = array(
    'field_reference_location' => 'field_reference_location',
    'field_location_icon' => 'field_location_icon',
  );
  $handler->display->display_options['row_options']['hide_empty'] = TRUE;
  $handler->display->display_options['defaults']['row_options'] = FALSE;
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: Content: Image */
  $handler->display->display_options['fields']['field_content_image']['id'] = 'field_content_image';
  $handler->display->display_options['fields']['field_content_image']['table'] = 'field_data_field_content_image';
  $handler->display->display_options['fields']['field_content_image']['field'] = 'field_content_image';
  $handler->display->display_options['fields']['field_content_image']['label'] = '';
  $handler->display->display_options['fields']['field_content_image']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_content_image']['click_sort_column'] = 'fid';
  $handler->display->display_options['fields']['field_content_image']['settings'] = array(
    'image_style' => 'medium',
    'image_link' => 'content',
  );
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = '';
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
  $handler->display->display_options['fields']['title']['element_type'] = 'h2';
  $handler->display->display_options['fields']['title']['element_label_colon'] = FALSE;
  /* Field: Content: Body */
  $handler->display->display_options['fields']['body']['id'] = 'body';
  $handler->display->display_options['fields']['body']['table'] = 'field_data_body';
  $handler->display->display_options['fields']['body']['field'] = 'body';
  $handler->display->display_options['fields']['body']['label'] = '';
  $handler->display->display_options['fields']['body']['alter']['max_length'] = '180';
  $handler->display->display_options['fields']['body']['alter']['trim'] = TRUE;
  $handler->display->display_options['fields']['body']['alter']['html'] = TRUE;
  $handler->display->display_options['fields']['body']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['body']['type'] = 'text_trimmed';
  $handler->display->display_options['fields']['body']['settings'] = array(
    'trim_length' => '200',
  );
  /* Field: Content: Link */
  $handler->display->display_options['fields']['view_node']['id'] = 'view_node';
  $handler->display->display_options['fields']['view_node']['table'] = 'views_entity_node';
  $handler->display->display_options['fields']['view_node']['field'] = 'view_node';
  $handler->display->display_options['fields']['view_node']['label'] = '';
  $handler->display->display_options['fields']['view_node']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['view_node']['text'] = 'Read more';
  /* Field: Content: Location */
  $handler->display->display_options['fields']['field_reference_location']['id'] = 'field_reference_location';
  $handler->display->display_options['fields']['field_reference_location']['table'] = 'field_data_field_reference_location';
  $handler->display->display_options['fields']['field_reference_location']['field'] = 'field_reference_location';
  $handler->display->display_options['fields']['field_reference_location']['settings'] = array(
    'link' => 1,
  );
  /* Field: Taxonomy term: Location icon */
  $handler->display->display_options['fields']['field_location_icon']['id'] = 'field_location_icon';
  $handler->display->display_options['fields']['field_location_icon']['table'] = 'field_data_field_location_icon';
  $handler->display->display_options['fields']['field_location_icon']['field'] = 'field_location_icon';
  $handler->display->display_options['fields']['field_location_icon']['relationship'] = 'field_location_type_tid';
  $handler->display->display_options['fields']['field_location_icon']['label'] = '';
  $handler->display->display_options['fields']['field_location_icon']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_location_icon']['click_sort_column'] = 'fid';
  $handler->display->display_options['fields']['field_location_icon']['settings'] = array(
    'image_style' => 'icon_small',
    'image_link' => '',
  );
  $handler->display->display_options['defaults']['arguments'] = FALSE;
  $handler->display->display_options['path'] = 'view-story';
  $handler->display->display_options['menu']['weight'] = '0';
  $handler->display->display_options['menu']['name'] = 'main-menu';
  $handler->display->display_options['menu']['context'] = 0;
  $handler->display->display_options['menu']['context_only_inline'] = 0;

  /* Display: Content pane: featured story */
  $handler = $view->new_display('panel_pane', 'Content pane: featured story', 'panel_pane_1');
  $handler->display->display_options['defaults']['title'] = FALSE;
  $handler->display->display_options['title'] = 'Featured story about this location';
  $handler->display->display_options['defaults']['pager'] = FALSE;
  $handler->display->display_options['pager']['type'] = 'some';
  $handler->display->display_options['pager']['options']['items_per_page'] = '1';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['defaults']['style_plugin'] = FALSE;
  $handler->display->display_options['style_plugin'] = 'default';
  $handler->display->display_options['defaults']['style_options'] = FALSE;
  $handler->display->display_options['defaults']['row_plugin'] = FALSE;
  $handler->display->display_options['row_plugin'] = 'fields';
  $handler->display->display_options['row_options']['inline'] = array(
    'field_told_by' => 'field_told_by',
    'field_submitted_by' => 'field_submitted_by',
  );
  $handler->display->display_options['row_options']['hide_empty'] = TRUE;
  $handler->display->display_options['defaults']['row_options'] = FALSE;
  $handler->display->display_options['defaults']['relationships'] = FALSE;
  /* Relationship: Entity Reference: Referenced Entity */
  $handler->display->display_options['relationships']['field_reference_location_target_id']['id'] = 'field_reference_location_target_id';
  $handler->display->display_options['relationships']['field_reference_location_target_id']['table'] = 'field_data_field_reference_location';
  $handler->display->display_options['relationships']['field_reference_location_target_id']['field'] = 'field_reference_location_target_id';
  $handler->display->display_options['relationships']['field_reference_location_target_id']['required'] = TRUE;
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: Content: Image */
  $handler->display->display_options['fields']['field_content_image']['id'] = 'field_content_image';
  $handler->display->display_options['fields']['field_content_image']['table'] = 'field_data_field_content_image';
  $handler->display->display_options['fields']['field_content_image']['field'] = 'field_content_image';
  $handler->display->display_options['fields']['field_content_image']['label'] = '';
  $handler->display->display_options['fields']['field_content_image']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_content_image']['click_sort_column'] = 'fid';
  $handler->display->display_options['fields']['field_content_image']['settings'] = array(
    'image_style' => 'medium',
    'image_link' => 'content',
  );
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = '';
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
  $handler->display->display_options['fields']['title']['element_type'] = 'h2';
  $handler->display->display_options['fields']['title']['element_label_colon'] = FALSE;
  /* Field: Content: Told by */
  $handler->display->display_options['fields']['field_told_by']['id'] = 'field_told_by';
  $handler->display->display_options['fields']['field_told_by']['table'] = 'field_data_field_told_by';
  $handler->display->display_options['fields']['field_told_by']['field'] = 'field_told_by';
  $handler->display->display_options['fields']['field_told_by']['hide_empty'] = TRUE;
  /* Field: Content: Submitted by */
  $handler->display->display_options['fields']['field_submitted_by']['id'] = 'field_submitted_by';
  $handler->display->display_options['fields']['field_submitted_by']['table'] = 'field_data_field_submitted_by';
  $handler->display->display_options['fields']['field_submitted_by']['field'] = 'field_submitted_by';
  $handler->display->display_options['fields']['field_submitted_by']['hide_empty'] = TRUE;
  /* Field: Content: Body */
  $handler->display->display_options['fields']['body']['id'] = 'body';
  $handler->display->display_options['fields']['body']['table'] = 'field_data_body';
  $handler->display->display_options['fields']['body']['field'] = 'body';
  $handler->display->display_options['fields']['body']['label'] = '';
  $handler->display->display_options['fields']['body']['alter']['max_length'] = '500';
  $handler->display->display_options['fields']['body']['alter']['trim'] = TRUE;
  $handler->display->display_options['fields']['body']['alter']['html'] = TRUE;
  $handler->display->display_options['fields']['body']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['body']['type'] = 'text_trimmed';
  $handler->display->display_options['fields']['body']['settings'] = array(
    'trim_length' => '600',
  );
  /* Field: Content: Link */
  $handler->display->display_options['fields']['view_node']['id'] = 'view_node';
  $handler->display->display_options['fields']['view_node']['table'] = 'views_entity_node';
  $handler->display->display_options['fields']['view_node']['field'] = 'view_node';
  $handler->display->display_options['fields']['view_node']['label'] = '';
  $handler->display->display_options['fields']['view_node']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['view_node']['text'] = 'Read more';
  $handler->display->display_options['defaults']['sorts'] = FALSE;
  /* Sort criterion: Global: Random */
  $handler->display->display_options['sorts']['random']['id'] = 'random';
  $handler->display->display_options['sorts']['random']['table'] = 'views';
  $handler->display->display_options['sorts']['random']['field'] = 'random';
  $handler->display->display_options['defaults']['arguments'] = FALSE;
  /* Contextual filter: Content: Location (field_reference_location) */
  $handler->display->display_options['arguments']['field_reference_location_target_id']['id'] = 'field_reference_location_target_id';
  $handler->display->display_options['arguments']['field_reference_location_target_id']['table'] = 'field_data_field_reference_location';
  $handler->display->display_options['arguments']['field_reference_location_target_id']['field'] = 'field_reference_location_target_id';
  $handler->display->display_options['arguments']['field_reference_location_target_id']['default_action'] = 'default';
  $handler->display->display_options['arguments']['field_reference_location_target_id']['default_argument_type'] = 'node';
  $handler->display->display_options['arguments']['field_reference_location_target_id']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['field_reference_location_target_id']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['field_reference_location_target_id']['summary_options']['items_per_page'] = '25';
  $handler->display->display_options['defaults']['filter_groups'] = FALSE;
  $handler->display->display_options['defaults']['filters'] = FALSE;
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
    'story' => 'story',
  );
  /* Filter criterion: Content: Promoted to front page */
  $handler->display->display_options['filters']['promote']['id'] = 'promote';
  $handler->display->display_options['filters']['promote']['table'] = 'node';
  $handler->display->display_options['filters']['promote']['field'] = 'promote';
  $handler->display->display_options['filters']['promote']['value'] = '1';
  $handler->display->display_options['allow']['use_pager'] = 0;
  $handler->display->display_options['allow']['items_per_page'] = 0;
  $handler->display->display_options['allow']['offset'] = 0;
  $handler->display->display_options['allow']['link_to_view'] = 0;
  $handler->display->display_options['allow']['more_link'] = 0;
  $handler->display->display_options['allow']['path_override'] = 0;
  $handler->display->display_options['allow']['title_override'] = 'title_override';
  $handler->display->display_options['allow']['exposed_form'] = 0;
  $handler->display->display_options['allow']['fields_override'] = 0;

  /* Display: Content pane 2: latest stories about location */
  $handler = $view->new_display('panel_pane', 'Content pane 2: latest stories about location', 'panel_pane_2');
  $handler->display->display_options['defaults']['title'] = FALSE;
  $handler->display->display_options['title'] = 'Latest stories about this location';
  $handler->display->display_options['defaults']['pager'] = FALSE;
  $handler->display->display_options['pager']['type'] = 'some';
  $handler->display->display_options['pager']['options']['items_per_page'] = '8';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['defaults']['style_plugin'] = FALSE;
  $handler->display->display_options['style_plugin'] = 'masonry_views_grid';
  $handler->display->display_options['style_options']['masonry_width'] = '220';
  $handler->display->display_options['style_options']['masonry_animated'] = 1;
  $handler->display->display_options['style_options']['masonry_resizable'] = 1;
  $handler->display->display_options['style_options']['masonry_center'] = 0;
  $handler->display->display_options['style_options']['masonry_gutter'] = '20';
  $handler->display->display_options['style_options']['masonry_rtl'] = 1;
  $handler->display->display_options['defaults']['style_options'] = FALSE;
  $handler->display->display_options['defaults']['row_plugin'] = FALSE;
  $handler->display->display_options['row_plugin'] = 'fields';
  $handler->display->display_options['row_options']['inline'] = array(
    'field_told_by' => 'field_told_by',
    'field_submitted_by' => 'field_submitted_by',
  );
  $handler->display->display_options['row_options']['hide_empty'] = TRUE;
  $handler->display->display_options['defaults']['row_options'] = FALSE;
  $handler->display->display_options['defaults']['relationships'] = FALSE;
  /* Relationship: Entity Reference: Referenced Entity */
  $handler->display->display_options['relationships']['field_reference_location_target_id']['id'] = 'field_reference_location_target_id';
  $handler->display->display_options['relationships']['field_reference_location_target_id']['table'] = 'field_data_field_reference_location';
  $handler->display->display_options['relationships']['field_reference_location_target_id']['field'] = 'field_reference_location_target_id';
  $handler->display->display_options['relationships']['field_reference_location_target_id']['required'] = TRUE;
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: Content: Image */
  $handler->display->display_options['fields']['field_content_image']['id'] = 'field_content_image';
  $handler->display->display_options['fields']['field_content_image']['table'] = 'field_data_field_content_image';
  $handler->display->display_options['fields']['field_content_image']['field'] = 'field_content_image';
  $handler->display->display_options['fields']['field_content_image']['label'] = '';
  $handler->display->display_options['fields']['field_content_image']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_content_image']['click_sort_column'] = 'fid';
  $handler->display->display_options['fields']['field_content_image']['settings'] = array(
    'image_style' => 'medium',
    'image_link' => 'content',
  );
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = '';
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
  $handler->display->display_options['fields']['title']['element_label_colon'] = FALSE;
  $handler->display->display_options['defaults']['arguments'] = FALSE;
  /* Contextual filter: Content: Location (field_reference_location) */
  $handler->display->display_options['arguments']['field_reference_location_target_id']['id'] = 'field_reference_location_target_id';
  $handler->display->display_options['arguments']['field_reference_location_target_id']['table'] = 'field_data_field_reference_location';
  $handler->display->display_options['arguments']['field_reference_location_target_id']['field'] = 'field_reference_location_target_id';
  $handler->display->display_options['arguments']['field_reference_location_target_id']['default_action'] = 'default';
  $handler->display->display_options['arguments']['field_reference_location_target_id']['default_argument_type'] = 'node';
  $handler->display->display_options['arguments']['field_reference_location_target_id']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['field_reference_location_target_id']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['field_reference_location_target_id']['summary_options']['items_per_page'] = '25';

  /* Display: Content pane 3: featured */
  $handler = $view->new_display('panel_pane', 'Content pane 3: featured', 'panel_pane_3');
  $handler->display->display_options['defaults']['title'] = FALSE;
  $handler->display->display_options['title'] = 'Featured stories';
  $handler->display->display_options['defaults']['pager'] = FALSE;
  $handler->display->display_options['pager']['type'] = 'some';
  $handler->display->display_options['pager']['options']['items_per_page'] = '2';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['defaults']['style_plugin'] = FALSE;
  $handler->display->display_options['style_plugin'] = 'default';
  $handler->display->display_options['defaults']['style_options'] = FALSE;
  $handler->display->display_options['defaults']['row_plugin'] = FALSE;
  $handler->display->display_options['row_plugin'] = 'fields';
  $handler->display->display_options['row_options']['inline'] = array(
    'field_told_by' => 'field_told_by',
    'field_submitted_by' => 'field_submitted_by',
    'field_reference_location' => 'field_reference_location',
    'field_location_icon' => 'field_location_icon',
  );
  $handler->display->display_options['row_options']['hide_empty'] = TRUE;
  $handler->display->display_options['defaults']['row_options'] = FALSE;
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: Content: Image */
  $handler->display->display_options['fields']['field_content_image']['id'] = 'field_content_image';
  $handler->display->display_options['fields']['field_content_image']['table'] = 'field_data_field_content_image';
  $handler->display->display_options['fields']['field_content_image']['field'] = 'field_content_image';
  $handler->display->display_options['fields']['field_content_image']['label'] = '';
  $handler->display->display_options['fields']['field_content_image']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_content_image']['click_sort_column'] = 'fid';
  $handler->display->display_options['fields']['field_content_image']['settings'] = array(
    'image_style' => 'medium_large',
    'image_link' => 'content',
  );
  /* Field: Taxonomy term: Location icon */
  $handler->display->display_options['fields']['field_location_icon']['id'] = 'field_location_icon';
  $handler->display->display_options['fields']['field_location_icon']['table'] = 'field_data_field_location_icon';
  $handler->display->display_options['fields']['field_location_icon']['field'] = 'field_location_icon';
  $handler->display->display_options['fields']['field_location_icon']['relationship'] = 'field_location_type_tid';
  $handler->display->display_options['fields']['field_location_icon']['label'] = '';
  $handler->display->display_options['fields']['field_location_icon']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_location_icon']['click_sort_column'] = 'fid';
  $handler->display->display_options['fields']['field_location_icon']['settings'] = array(
    'image_style' => 'icon_medium_small',
    'image_link' => '',
  );
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = '';
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
  $handler->display->display_options['fields']['title']['element_type'] = 'h2';
  $handler->display->display_options['fields']['title']['element_label_colon'] = FALSE;
  /* Field: Content: Body */
  $handler->display->display_options['fields']['body']['id'] = 'body';
  $handler->display->display_options['fields']['body']['table'] = 'field_data_body';
  $handler->display->display_options['fields']['body']['field'] = 'body';
  $handler->display->display_options['fields']['body']['label'] = '';
  $handler->display->display_options['fields']['body']['alter']['max_length'] = '680';
  $handler->display->display_options['fields']['body']['alter']['trim'] = TRUE;
  $handler->display->display_options['fields']['body']['alter']['html'] = TRUE;
  $handler->display->display_options['fields']['body']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['body']['type'] = 'text_trimmed';
  $handler->display->display_options['fields']['body']['settings'] = array(
    'trim_length' => '700',
  );
  /* Field: Content: Link */
  $handler->display->display_options['fields']['view_node']['id'] = 'view_node';
  $handler->display->display_options['fields']['view_node']['table'] = 'views_entity_node';
  $handler->display->display_options['fields']['view_node']['field'] = 'view_node';
  $handler->display->display_options['fields']['view_node']['label'] = '';
  $handler->display->display_options['fields']['view_node']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['view_node']['text'] = 'Read more';
  /* Field: Content: Location */
  $handler->display->display_options['fields']['field_reference_location']['id'] = 'field_reference_location';
  $handler->display->display_options['fields']['field_reference_location']['table'] = 'field_data_field_reference_location';
  $handler->display->display_options['fields']['field_reference_location']['field'] = 'field_reference_location';
  $handler->display->display_options['fields']['field_reference_location']['settings'] = array(
    'link' => 1,
  );

  /* Display: Content pane 5: latest stories */
  $handler = $view->new_display('panel_pane', 'Content pane 5: latest stories', 'panel_pane_5');
  $handler->display->display_options['defaults']['title'] = FALSE;
  $handler->display->display_options['title'] = 'Latest stories';
  $handler->display->display_options['defaults']['use_more'] = FALSE;
  $handler->display->display_options['use_more'] = TRUE;
  $handler->display->display_options['defaults']['use_more_always'] = FALSE;
  $handler->display->display_options['defaults']['use_more_always'] = FALSE;
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['defaults']['use_more_text'] = FALSE;
  $handler->display->display_options['use_more_text'] = 'View more stories';
  $handler->display->display_options['defaults']['pager'] = FALSE;
  $handler->display->display_options['pager']['type'] = 'some';
  $handler->display->display_options['pager']['options']['items_per_page'] = '10';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['defaults']['style_plugin'] = FALSE;
  $handler->display->display_options['style_plugin'] = 'masonry_views_grid';
  $handler->display->display_options['style_options']['masonry_width'] = '220';
  $handler->display->display_options['style_options']['masonry_animated'] = 1;
  $handler->display->display_options['style_options']['masonry_resizable'] = 1;
  $handler->display->display_options['style_options']['masonry_center'] = 0;
  $handler->display->display_options['style_options']['masonry_gutter'] = '20';
  $handler->display->display_options['style_options']['masonry_rtl'] = 0;
  $handler->display->display_options['defaults']['style_options'] = FALSE;
  $handler->display->display_options['defaults']['row_plugin'] = FALSE;
  $handler->display->display_options['row_plugin'] = 'fields';
  $handler->display->display_options['row_options']['inline'] = array(
    'field_reference_location' => 'field_reference_location',
    'field_location_icon' => 'field_location_icon',
  );
  $handler->display->display_options['row_options']['hide_empty'] = TRUE;
  $handler->display->display_options['defaults']['row_options'] = FALSE;
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: Content: Image */
  $handler->display->display_options['fields']['field_content_image']['id'] = 'field_content_image';
  $handler->display->display_options['fields']['field_content_image']['table'] = 'field_data_field_content_image';
  $handler->display->display_options['fields']['field_content_image']['field'] = 'field_content_image';
  $handler->display->display_options['fields']['field_content_image']['label'] = '';
  $handler->display->display_options['fields']['field_content_image']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_content_image']['click_sort_column'] = 'fid';
  $handler->display->display_options['fields']['field_content_image']['settings'] = array(
    'image_style' => 'medium',
    'image_link' => 'content',
  );
  /* Field: Taxonomy term: Location icon */
  $handler->display->display_options['fields']['field_location_icon']['id'] = 'field_location_icon';
  $handler->display->display_options['fields']['field_location_icon']['table'] = 'field_data_field_location_icon';
  $handler->display->display_options['fields']['field_location_icon']['field'] = 'field_location_icon';
  $handler->display->display_options['fields']['field_location_icon']['relationship'] = 'field_location_type_tid';
  $handler->display->display_options['fields']['field_location_icon']['label'] = '';
  $handler->display->display_options['fields']['field_location_icon']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_location_icon']['click_sort_column'] = 'fid';
  $handler->display->display_options['fields']['field_location_icon']['settings'] = array(
    'image_style' => 'icon_medium_small',
    'image_link' => '',
  );
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = '';
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
  $handler->display->display_options['fields']['title']['element_label_colon'] = FALSE;
  /* Field: Content: Location */
  $handler->display->display_options['fields']['field_reference_location']['id'] = 'field_reference_location';
  $handler->display->display_options['fields']['field_reference_location']['table'] = 'field_data_field_reference_location';
  $handler->display->display_options['fields']['field_reference_location']['field'] = 'field_reference_location';
  $handler->display->display_options['fields']['field_reference_location']['settings'] = array(
    'link' => 1,
  );
  $handler->display->display_options['defaults']['filter_groups'] = FALSE;
  $handler->display->display_options['defaults']['filters'] = FALSE;
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
    'story' => 'story',
  );

  /* Display: Content pane: promoted */
  $handler = $view->new_display('panel_pane', 'Content pane: promoted', 'panel_pane_6');
  $handler->display->display_options['defaults']['title'] = FALSE;
  $handler->display->display_options['title'] = 'Recent stories';
  $handler->display->display_options['defaults']['use_more'] = FALSE;
  $handler->display->display_options['use_more'] = TRUE;
  $handler->display->display_options['defaults']['use_more_always'] = FALSE;
  $handler->display->display_options['defaults']['use_more_always'] = FALSE;
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['defaults']['use_more_text'] = FALSE;
  $handler->display->display_options['use_more_text'] = 'View all stories';
  $handler->display->display_options['defaults']['pager'] = FALSE;
  $handler->display->display_options['pager']['type'] = 'some';
  $handler->display->display_options['pager']['options']['items_per_page'] = '4';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['defaults']['style_plugin'] = FALSE;
  $handler->display->display_options['style_plugin'] = 'masonry_views_grid';
  $handler->display->display_options['style_options']['masonry_width'] = '220';
  $handler->display->display_options['style_options']['masonry_animated'] = 1;
  $handler->display->display_options['style_options']['masonry_resizable'] = 1;
  $handler->display->display_options['style_options']['masonry_center'] = 0;
  $handler->display->display_options['style_options']['masonry_gutter'] = '20';
  $handler->display->display_options['style_options']['masonry_rtl'] = 0;
  $handler->display->display_options['defaults']['style_options'] = FALSE;
  $handler->display->display_options['defaults']['row_plugin'] = FALSE;
  $handler->display->display_options['row_plugin'] = 'fields';
  $handler->display->display_options['row_options']['inline'] = array(
    'field_reference_location' => 'field_reference_location',
    'field_location_icon' => 'field_location_icon',
  );
  $handler->display->display_options['row_options']['hide_empty'] = TRUE;
  $handler->display->display_options['defaults']['row_options'] = FALSE;
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: Content: Image */
  $handler->display->display_options['fields']['field_content_image']['id'] = 'field_content_image';
  $handler->display->display_options['fields']['field_content_image']['table'] = 'field_data_field_content_image';
  $handler->display->display_options['fields']['field_content_image']['field'] = 'field_content_image';
  $handler->display->display_options['fields']['field_content_image']['label'] = '';
  $handler->display->display_options['fields']['field_content_image']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_content_image']['click_sort_column'] = 'fid';
  $handler->display->display_options['fields']['field_content_image']['settings'] = array(
    'image_style' => 'medium',
    'image_link' => 'content',
  );
  /* Field: Taxonomy term: Location icon */
  $handler->display->display_options['fields']['field_location_icon']['id'] = 'field_location_icon';
  $handler->display->display_options['fields']['field_location_icon']['table'] = 'field_data_field_location_icon';
  $handler->display->display_options['fields']['field_location_icon']['field'] = 'field_location_icon';
  $handler->display->display_options['fields']['field_location_icon']['relationship'] = 'field_location_type_tid';
  $handler->display->display_options['fields']['field_location_icon']['label'] = '';
  $handler->display->display_options['fields']['field_location_icon']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_location_icon']['click_sort_column'] = 'fid';
  $handler->display->display_options['fields']['field_location_icon']['settings'] = array(
    'image_style' => 'icon_medium_small',
    'image_link' => '',
  );
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = '';
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
  $handler->display->display_options['fields']['title']['element_type'] = 'h4';
  $handler->display->display_options['fields']['title']['element_label_colon'] = FALSE;
  /* Field: Content: Location */
  $handler->display->display_options['fields']['field_reference_location']['id'] = 'field_reference_location';
  $handler->display->display_options['fields']['field_reference_location']['table'] = 'field_data_field_reference_location';
  $handler->display->display_options['fields']['field_reference_location']['field'] = 'field_reference_location';
  $handler->display->display_options['fields']['field_reference_location']['settings'] = array(
    'link' => 1,
  );
  $handler->display->display_options['defaults']['filter_groups'] = FALSE;
  $handler->display->display_options['defaults']['filters'] = FALSE;
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
    'story' => 'story',
  );
  /* Filter criterion: Content: Promoted to front page */
  $handler->display->display_options['filters']['promote']['id'] = 'promote';
  $handler->display->display_options['filters']['promote']['table'] = 'node';
  $handler->display->display_options['filters']['promote']['field'] = 'promote';
  $handler->display->display_options['filters']['promote']['value'] = '1';
  $export['story'] = $view;

  return $export;
}
