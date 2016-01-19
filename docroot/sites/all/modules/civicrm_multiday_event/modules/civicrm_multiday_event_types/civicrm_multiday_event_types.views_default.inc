<?php
/**
 * @file
 * civicrm_multiday_event_types.views_default.inc
 */

/**
 * Implementation of hook_views_default_views().
 */
function civicrm_multiday_event_types_views_default_views() {
  $export = array();

  $view = new view;
  $view->name = 'civicrm_multiday_event_event_type_legend';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'taxonomy_term_data';
  $view->human_name = 'CiviCRM Multiday Event: Event Type Legend';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'CiviCRM Multiday Event: Event Type Legend';
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['query']['options']['query_comment'] = FALSE;
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'none';
  $handler->display->display_options['style_plugin'] = 'table';
  /* Field: Taxonomy term: Name */
  $handler->display->display_options['fields']['name']['id'] = 'name';
  $handler->display->display_options['fields']['name']['table'] = 'taxonomy_term_data';
  $handler->display->display_options['fields']['name']['field'] = 'name';
  $handler->display->display_options['fields']['name']['label'] = '';
  $handler->display->display_options['fields']['name']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['name']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['name']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['name']['alter']['external'] = 0;
  $handler->display->display_options['fields']['name']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['name']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['name']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['name']['alter']['word_boundary'] = 0;
  $handler->display->display_options['fields']['name']['alter']['ellipsis'] = 0;
  $handler->display->display_options['fields']['name']['alter']['more_link'] = 0;
  $handler->display->display_options['fields']['name']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['name']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['name']['alter']['html'] = 0;
  $handler->display->display_options['fields']['name']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['name']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['name']['hide_empty'] = 0;
  $handler->display->display_options['fields']['name']['empty_zero'] = 0;
  $handler->display->display_options['fields']['name']['hide_alter_empty'] = 1;
  $handler->display->display_options['fields']['name']['link_to_taxonomy'] = 0;
  $handler->display->display_options['fields']['name']['convert_spaces'] = 0;
  /* Field: Taxonomy term: Term ID */
  $handler->display->display_options['fields']['tid']['id'] = 'tid';
  $handler->display->display_options['fields']['tid']['table'] = 'taxonomy_term_data';
  $handler->display->display_options['fields']['tid']['field'] = 'tid';
  $handler->display->display_options['fields']['tid']['label'] = '';
  $handler->display->display_options['fields']['tid']['alter']['alter_text'] = 1;
  $handler->display->display_options['fields']['tid']['alter']['text'] = '<span class="colors-taxonomy-term-[tid]">&nbsp;&nbsp;&nbsp;</span>';
  $handler->display->display_options['fields']['tid']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['tid']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['tid']['alter']['external'] = 0;
  $handler->display->display_options['fields']['tid']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['tid']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['tid']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['tid']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['tid']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['tid']['alter']['more_link'] = 0;
  $handler->display->display_options['fields']['tid']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['tid']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['tid']['alter']['html'] = 0;
  $handler->display->display_options['fields']['tid']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['tid']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['tid']['hide_empty'] = 0;
  $handler->display->display_options['fields']['tid']['empty_zero'] = 0;
  $handler->display->display_options['fields']['tid']['hide_alter_empty'] = 1;
  $handler->display->display_options['fields']['tid']['separator'] = '';
  $handler->display->display_options['fields']['tid']['format_plural'] = 0;
  /* Sort criterion: Taxonomy term: Weight */
  $handler->display->display_options['sorts']['weight']['id'] = 'weight';
  $handler->display->display_options['sorts']['weight']['table'] = 'taxonomy_term_data';
  $handler->display->display_options['sorts']['weight']['field'] = 'weight';
  /* Filter criterion: Taxonomy vocabulary: Machine name */
  $handler->display->display_options['filters']['machine_name']['id'] = 'machine_name';
  $handler->display->display_options['filters']['machine_name']['table'] = 'taxonomy_vocabulary';
  $handler->display->display_options['filters']['machine_name']['field'] = 'machine_name';
  $handler->display->display_options['filters']['machine_name']['value'] = array(
    'civicrm_multiday_event_type' => 'civicrm_multiday_event_type',
  );

  /* Display: Block */
  $handler = $view->new_display('block', 'Block', 'block');
  $export['civicrm_multiday_event_event_type_legend'] = $view;

  $view = new view;
  $view->name = 'civicrm_multiday_event_list_by_type';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'taxonomy_term_data';
  $view->human_name = 'CiviCRM Multiday Event: List by Type';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Upcoming Events';
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['query']['options']['query_comment'] = FALSE;
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'some';
  $handler->display->display_options['pager']['options']['items_per_page'] = '25';
  $handler->display->display_options['style_plugin'] = 'views_accordion';
  $handler->display->display_options['style_options']['grouping'] = array(
    0 => array(
      'field' => 'name',
      'rendered' => 1,
      'rendered_strip' => 0,
    ),
  );
  $handler->display->display_options['style_options']['use-grouping-header'] = 1;
  $handler->display->display_options['style_options']['row-start-open'] = '0';
  $handler->display->display_options['row_plugin'] = 'fields';
  /* Relationship: Taxonomy term: Content with term */
  $handler->display->display_options['relationships']['nid']['id'] = 'nid';
  $handler->display->display_options['relationships']['nid']['table'] = 'taxonomy_index';
  $handler->display->display_options['relationships']['nid']['field'] = 'nid';
  $handler->display->display_options['relationships']['nid']['required'] = 0;
  /* Relationship: Content: Sessions (field_civicrm_multiday_session) */
  $handler->display->display_options['relationships']['field_civicrm_multiday_session_value']['id'] = 'field_civicrm_multiday_session_value';
  $handler->display->display_options['relationships']['field_civicrm_multiday_session_value']['table'] = 'field_data_field_civicrm_multiday_session';
  $handler->display->display_options['relationships']['field_civicrm_multiday_session_value']['field'] = 'field_civicrm_multiday_session_value';
  $handler->display->display_options['relationships']['field_civicrm_multiday_session_value']['relationship'] = 'nid';
  $handler->display->display_options['relationships']['field_civicrm_multiday_session_value']['required'] = 0;
  $handler->display->display_options['relationships']['field_civicrm_multiday_session_value']['delta'] = '-1';
  /* Field: Taxonomy term: Name */
  $handler->display->display_options['fields']['name']['id'] = 'name';
  $handler->display->display_options['fields']['name']['table'] = 'taxonomy_term_data';
  $handler->display->display_options['fields']['name']['field'] = 'name';
  $handler->display->display_options['fields']['name']['label'] = '';
  $handler->display->display_options['fields']['name']['exclude'] = TRUE;
  $handler->display->display_options['fields']['name']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['name']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['name']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['name']['alter']['external'] = 0;
  $handler->display->display_options['fields']['name']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['name']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['name']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['name']['alter']['word_boundary'] = 0;
  $handler->display->display_options['fields']['name']['alter']['ellipsis'] = 0;
  $handler->display->display_options['fields']['name']['alter']['more_link'] = 0;
  $handler->display->display_options['fields']['name']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['name']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['name']['alter']['html'] = 0;
  $handler->display->display_options['fields']['name']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['name']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['name']['hide_empty'] = 0;
  $handler->display->display_options['fields']['name']['empty_zero'] = 0;
  $handler->display->display_options['fields']['name']['hide_alter_empty'] = 1;
  $handler->display->display_options['fields']['name']['link_to_taxonomy'] = 1;
  $handler->display->display_options['fields']['name']['convert_spaces'] = 0;
  /* Field: Taxonomy term: Term description */
  $handler->display->display_options['fields']['description']['id'] = 'description';
  $handler->display->display_options['fields']['description']['table'] = 'taxonomy_term_data';
  $handler->display->display_options['fields']['description']['field'] = 'description';
  $handler->display->display_options['fields']['description']['label'] = '';
  $handler->display->display_options['fields']['description']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['description']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['description']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['description']['alter']['external'] = 0;
  $handler->display->display_options['fields']['description']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['description']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['description']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['description']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['description']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['description']['alter']['more_link'] = 0;
  $handler->display->display_options['fields']['description']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['description']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['description']['alter']['html'] = 0;
  $handler->display->display_options['fields']['description']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['description']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['description']['hide_empty'] = 0;
  $handler->display->display_options['fields']['description']['empty_zero'] = 0;
  $handler->display->display_options['fields']['description']['hide_alter_empty'] = 1;
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['relationship'] = 'nid';
  $handler->display->display_options['fields']['title']['label'] = '';
  $handler->display->display_options['fields']['title']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['title']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['title']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['title']['alter']['external'] = 0;
  $handler->display->display_options['fields']['title']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['title']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['title']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['title']['alter']['more_link'] = 0;
  $handler->display->display_options['fields']['title']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['title']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['title']['alter']['html'] = 0;
  $handler->display->display_options['fields']['title']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['title']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['title']['hide_empty'] = 0;
  $handler->display->display_options['fields']['title']['empty_zero'] = 0;
  $handler->display->display_options['fields']['title']['hide_alter_empty'] = 1;
  $handler->display->display_options['fields']['title']['link_to_node'] = 1;
  /* Field: Content: Sessions */
  $handler->display->display_options['fields']['field_civicrm_multiday_session']['id'] = 'field_civicrm_multiday_session';
  $handler->display->display_options['fields']['field_civicrm_multiday_session']['table'] = 'field_data_field_civicrm_multiday_session';
  $handler->display->display_options['fields']['field_civicrm_multiday_session']['field'] = 'field_civicrm_multiday_session';
  $handler->display->display_options['fields']['field_civicrm_multiday_session']['relationship'] = 'nid';
  $handler->display->display_options['fields']['field_civicrm_multiday_session']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['field_civicrm_multiday_session']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['field_civicrm_multiday_session']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['field_civicrm_multiday_session']['alter']['external'] = 0;
  $handler->display->display_options['fields']['field_civicrm_multiday_session']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['field_civicrm_multiday_session']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['field_civicrm_multiday_session']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['field_civicrm_multiday_session']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['field_civicrm_multiday_session']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['field_civicrm_multiday_session']['alter']['more_link'] = 0;
  $handler->display->display_options['fields']['field_civicrm_multiday_session']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['field_civicrm_multiday_session']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['field_civicrm_multiday_session']['alter']['html'] = 0;
  $handler->display->display_options['fields']['field_civicrm_multiday_session']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['field_civicrm_multiday_session']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['field_civicrm_multiday_session']['hide_empty'] = 0;
  $handler->display->display_options['fields']['field_civicrm_multiday_session']['empty_zero'] = 0;
  $handler->display->display_options['fields']['field_civicrm_multiday_session']['hide_alter_empty'] = 1;
  $handler->display->display_options['fields']['field_civicrm_multiday_session']['settings'] = array(
    'edit' => '',
    'delete' => '',
    'add' => '',
    'description' => 0,
    'view_mode' => 'full',
  );
  $handler->display->display_options['fields']['field_civicrm_multiday_session']['group_rows'] = 1;
  $handler->display->display_options['fields']['field_civicrm_multiday_session']['delta_offset'] = '0';
  $handler->display->display_options['fields']['field_civicrm_multiday_session']['delta_reversed'] = 0;
  $handler->display->display_options['fields']['field_civicrm_multiday_session']['delta_first_last'] = 0;
  $handler->display->display_options['fields']['field_civicrm_multiday_session']['field_api_classes'] = 0;
  /* Sort criterion: Content: Date of First Session (field_date_of_first_session) */
  $handler->display->display_options['sorts']['field_date_of_first_session_value']['id'] = 'field_date_of_first_session_value';
  $handler->display->display_options['sorts']['field_date_of_first_session_value']['table'] = 'field_data_field_date_of_first_session';
  $handler->display->display_options['sorts']['field_date_of_first_session_value']['field'] = 'field_date_of_first_session_value';
  $handler->display->display_options['sorts']['field_date_of_first_session_value']['relationship'] = 'nid';
  $handler->display->display_options['sorts']['field_date_of_first_session_value']['order'] = 'DESC';
  /* Filter criterion: Taxonomy vocabulary: Machine name */
  $handler->display->display_options['filters']['machine_name']['id'] = 'machine_name';
  $handler->display->display_options['filters']['machine_name']['table'] = 'taxonomy_vocabulary';
  $handler->display->display_options['filters']['machine_name']['field'] = 'machine_name';
  $handler->display->display_options['filters']['machine_name']['value'] = array(
    'civicrm_multiday_event_type' => 'civicrm_multiday_event_type',
  );
  /* Filter criterion: Field collection item: Session Date -  start date (field_session_date) */
  $handler->display->display_options['filters']['field_session_date_value']['id'] = 'field_session_date_value';
  $handler->display->display_options['filters']['field_session_date_value']['table'] = 'field_data_field_session_date';
  $handler->display->display_options['filters']['field_session_date_value']['field'] = 'field_session_date_value';
  $handler->display->display_options['filters']['field_session_date_value']['relationship'] = 'field_civicrm_multiday_session_value';
  $handler->display->display_options['filters']['field_session_date_value']['operator'] = '>=';
  $handler->display->display_options['filters']['field_session_date_value']['default_date'] = 'now -1 day';

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['path'] = 'civicrm-multiday-event/list';
  $handler->display->display_options['menu']['type'] = 'default tab';
  $handler->display->display_options['menu']['title'] = 'Upcoming Events';
  $handler->display->display_options['menu']['weight'] = '-99';
  $handler->display->display_options['menu']['context'] = 0;
  $handler->display->display_options['tab_options']['type'] = 'normal';
  $handler->display->display_options['tab_options']['title'] = 'Event List (CiviCRM Multiday)';
  $handler->display->display_options['tab_options']['weight'] = '0';
  $export['civicrm_multiday_event_list_by_type'] = $view;

  $view = new view;
  $view->name = 'civicrm_multiday_event_public_calendar';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'node';
  $view->human_name = 'CiviCRM Multiday Event: Public Calendar';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'CiviCRM Multiday Event Calendar';
  $handler->display->display_options['access']['type'] = 'none';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['query']['options']['query_comment'] = FALSE;
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'none';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['style_plugin'] = 'fullcalendar';
  $handler->display->display_options['style_options']['editable'] = TRUE;
  $handler->display->display_options['style_options']['fields']['url_field'] = 'nid';
  $handler->display->display_options['style_options']['fields']['title'] = TRUE;
  $handler->display->display_options['style_options']['fields']['url'] = TRUE;
  /* Header: Global: View area */
  $handler->display->display_options['header']['view']['id'] = 'view';
  $handler->display->display_options['header']['view']['table'] = 'views';
  $handler->display->display_options['header']['view']['field'] = 'view';
  $handler->display->display_options['header']['view']['empty'] = TRUE;
  $handler->display->display_options['header']['view']['view_to_insert'] = 'civicrm_multiday_event_event_type_legend:block';
  $handler->display->display_options['header']['view']['inherit_arguments'] = 0;
  /* Relationship: Content: Sessions (field_civicrm_multiday_session) */
  $handler->display->display_options['relationships']['field_civicrm_multiday_session_value']['id'] = 'field_civicrm_multiday_session_value';
  $handler->display->display_options['relationships']['field_civicrm_multiday_session_value']['table'] = 'field_data_field_civicrm_multiday_session';
  $handler->display->display_options['relationships']['field_civicrm_multiday_session_value']['field'] = 'field_civicrm_multiday_session_value';
  $handler->display->display_options['relationships']['field_civicrm_multiday_session_value']['required'] = 0;
  $handler->display->display_options['relationships']['field_civicrm_multiday_session_value']['delta'] = '-1';
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = '';
  $handler->display->display_options['fields']['title']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['title']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['title']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = 0;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = 0;
  $handler->display->display_options['fields']['title']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['title']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['title']['alter']['html'] = 0;
  $handler->display->display_options['fields']['title']['hide_empty'] = 0;
  $handler->display->display_options['fields']['title']['empty_zero'] = 0;
  $handler->display->display_options['fields']['title']['link_to_node'] = 1;
  /* Field: Field collection item: Session Date */
  $handler->display->display_options['fields']['field_session_date']['id'] = 'field_session_date';
  $handler->display->display_options['fields']['field_session_date']['table'] = 'field_data_field_session_date';
  $handler->display->display_options['fields']['field_session_date']['field'] = 'field_session_date';
  $handler->display->display_options['fields']['field_session_date']['relationship'] = 'field_civicrm_multiday_session_value';
  $handler->display->display_options['fields']['field_session_date']['exclude'] = TRUE;
  $handler->display->display_options['fields']['field_session_date']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['field_session_date']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['field_session_date']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['field_session_date']['alter']['external'] = 0;
  $handler->display->display_options['fields']['field_session_date']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['field_session_date']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['field_session_date']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['field_session_date']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['field_session_date']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['field_session_date']['alter']['more_link'] = 0;
  $handler->display->display_options['fields']['field_session_date']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['field_session_date']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['field_session_date']['alter']['html'] = 0;
  $handler->display->display_options['fields']['field_session_date']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['field_session_date']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['field_session_date']['hide_empty'] = 0;
  $handler->display->display_options['fields']['field_session_date']['empty_zero'] = 0;
  $handler->display->display_options['fields']['field_session_date']['hide_alter_empty'] = 1;
  $handler->display->display_options['fields']['field_session_date']['settings'] = array(
    'format_type' => 'long',
    'fromto' => 'both',
    'multiple_number' => '',
    'multiple_from' => '',
    'multiple_to' => '',
    'show_repeat_rule' => 'show',
  );
  $handler->display->display_options['fields']['field_session_date']['field_api_classes'] = 0;
  /* Field: Content: Nid */
  $handler->display->display_options['fields']['nid']['id'] = 'nid';
  $handler->display->display_options['fields']['nid']['table'] = 'node';
  $handler->display->display_options['fields']['nid']['field'] = 'nid';
  $handler->display->display_options['fields']['nid']['alter']['alter_text'] = 1;
  $handler->display->display_options['fields']['nid']['alter']['text'] = 'node/[nid] ';
  $handler->display->display_options['fields']['nid']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['nid']['alter']['path'] = 'node/[nid]';
  $handler->display->display_options['fields']['nid']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['nid']['alter']['external'] = 0;
  $handler->display->display_options['fields']['nid']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['nid']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['nid']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['nid']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['nid']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['nid']['alter']['more_link'] = 0;
  $handler->display->display_options['fields']['nid']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['nid']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['nid']['alter']['html'] = 0;
  $handler->display->display_options['fields']['nid']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['nid']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['nid']['hide_empty'] = 0;
  $handler->display->display_options['fields']['nid']['empty_zero'] = 0;
  $handler->display->display_options['fields']['nid']['hide_alter_empty'] = 1;
  $handler->display->display_options['fields']['nid']['link_to_node'] = 0;
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
    'civicrm_multiday_event' => 'civicrm_multiday_event',
  );

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['path'] = 'civicrm-multiday-event/calendar';
  $handler->display->display_options['menu']['type'] = 'tab';
  $handler->display->display_options['menu']['title'] = 'Calendar';
  $handler->display->display_options['menu']['weight'] = '-45';
  $handler->display->display_options['menu']['context'] = 0;
  $handler->display->display_options['tab_options']['type'] = 'normal';
  $handler->display->display_options['tab_options']['title'] = 'Events';
  $handler->display->display_options['tab_options']['weight'] = '0';
  $handler->display->display_options['tab_options']['name'] = 'management';
  $export['civicrm_multiday_event_public_calendar'] = $view;

  return $export;
}
