<?php

/**
 * @file
 * Providing extra functionality for the Party Activity UI via views.
 */

/**
 * Implements hook_views_default_views().
 */
function party_activity_views_default_views() {
  $views = array();

  $view = new view;
  $view->name = 'activities';
  $view->description = 'A list of all activities';
  $view->tag = 'activities';
  $view->base_table = 'party_activity';
  $view->human_name = 'Activities';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Activities';
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'create any party_activity type';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '10';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'name' => 'name',
    'id' => 'id',
  );
  $handler->display->display_options['style_options']['default'] = '-1';
  $handler->display->display_options['style_options']['info'] = array(
    'name' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
    ),
    'id' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
    ),
  );
  $handler->display->display_options['style_options']['override'] = 1;
  $handler->display->display_options['style_options']['sticky'] = 0;
  $handler->display->display_options['style_options']['empty_table'] = 0;
  /* No results behavior: Global: Text area */
  $handler->display->display_options['empty']['area']['id'] = 'area';
  $handler->display->display_options['empty']['area']['table'] = 'views';
  $handler->display->display_options['empty']['area']['field'] = 'area';
  $handler->display->display_options['empty']['area']['label'] = 'Empty ';
  $handler->display->display_options['empty']['area']['empty'] = FALSE;
  $handler->display->display_options['empty']['area']['content'] = 'No activities have been created yet';
  /* Field: Activity: Activity ID */
  $handler->display->display_options['fields']['id']['id'] = 'id';
  $handler->display->display_options['fields']['id']['table'] = 'party_activity';
  $handler->display->display_options['fields']['id']['field'] = 'id';
  $handler->display->display_options['fields']['id']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['id']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['id']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['id']['alter']['external'] = 0;
  $handler->display->display_options['fields']['id']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['id']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['id']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['id']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['id']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['id']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['id']['alter']['html'] = 0;
  $handler->display->display_options['fields']['id']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['id']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['id']['hide_empty'] = 0;
  $handler->display->display_options['fields']['id']['empty_zero'] = 0;
  /* Field: Activity: Name */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'party_activity';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['title']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['title']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['title']['alter']['external'] = 0;
  $handler->display->display_options['fields']['title']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['title']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['title']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['title']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['title']['alter']['html'] = 0;
  $handler->display->display_options['fields']['title']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['title']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['title']['hide_empty'] = 0;
  $handler->display->display_options['fields']['title']['empty_zero'] = 0;
  /* Field: Activity: Link */
  $handler->display->display_options['fields']['link_party_activity']['id'] = 'link_party_activity';
  $handler->display->display_options['fields']['link_party_activity']['table'] = 'party_activity';
  $handler->display->display_options['fields']['link_party_activity']['field'] = 'link_party_activity';
  $handler->display->display_options['fields']['link_party_activity']['label'] = 'View';
  $handler->display->display_options['fields']['link_party_activity']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['link_party_activity']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['link_party_activity']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['link_party_activity']['alter']['external'] = 0;
  $handler->display->display_options['fields']['link_party_activity']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['link_party_activity']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['link_party_activity']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['link_party_activity']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['link_party_activity']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['link_party_activity']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['link_party_activity']['alter']['html'] = 0;
  $handler->display->display_options['fields']['link_party_activity']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['link_party_activity']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['link_party_activity']['hide_empty'] = 0;
  $handler->display->display_options['fields']['link_party_activity']['empty_zero'] = 0;
  /* Field: Activity: Operations links */
  $handler->display->display_options['fields']['operations']['id'] = 'operations';
  $handler->display->display_options['fields']['operations']['table'] = 'party_activity';
  $handler->display->display_options['fields']['operations']['field'] = 'operations';
  $handler->display->display_options['fields']['operations']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['operations']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['operations']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['operations']['alter']['external'] = 0;
  $handler->display->display_options['fields']['operations']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['operations']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['operations']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['operations']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['operations']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['operations']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['operations']['alter']['html'] = 0;
  $handler->display->display_options['fields']['operations']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['operations']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['operations']['hide_empty'] = 0;
  $handler->display->display_options['fields']['operations']['empty_zero'] = 0;
  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'activities_admin_page');
  $handler->display->display_options['path'] = 'admin/community/activities/list';
  $handler->display->display_options['menu']['type'] = 'default tab';
  $handler->display->display_options['menu']['title'] = 'List';
  $handler->display->display_options['menu']['weight'] = '-10';
  $handler->display->display_options['tab_options']['type'] = 'tab';
  $handler->display->display_options['tab_options']['title'] = 'Activities';
  $handler->display->display_options['tab_options']['description'] = 'Manage activities';
  $handler->display->display_options['tab_options']['weight'] = '0';
  $handler->display->display_options['tab_options']['name'] = 'management';
  $translatables['activities'] = array(
    t('Master'),
    t('Activities'),
    t('more'),
    t('Apply'),
    t('Reset'),
    t('Sort by'),
    t('Asc'),
    t('Desc'),
    t('Items per page'),
    t('- All -'),
    t('Offset'),
    t('Empty'),
    t('No activities have been created yet'),
    t('Activity ID'),
    t('.'),
    t(','),
    t('Name'),
    t('View'),
    t('Operations links'),
    t('Page'),
  );
  $views[] = $view;

  // Party Activity content panes
  $view = new view;
  $view->name = 'party_activities';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'party_activity';
  $view->human_name = 'Party Activities';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Activities';
  $handler->display->display_options['access']['type'] = 'none';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['query']['options']['query_comment'] = FALSE;
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'id' => 'id',
    'title' => 'title',
    'activity_participants' => 'activity_participants',
  );
  $handler->display->display_options['style_options']['default'] = '-1';
  $handler->display->display_options['style_options']['info'] = array(
    'id' => array(
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
    'activity_participants' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 1,
    ),
  );
  $handler->display->display_options['style_options']['override'] = 1;
  $handler->display->display_options['style_options']['sticky'] = 0;
  $handler->display->display_options['style_options']['empty_table'] = 0;
  /* Relationship: Entity Reference: Referenced Entity */
  $handler->display->display_options['relationships']['activity_assigned_to_target_id']['id'] = 'activity_assigned_to_target_id';
  $handler->display->display_options['relationships']['activity_assigned_to_target_id']['table'] = 'field_data_activity_assigned_to';
  $handler->display->display_options['relationships']['activity_assigned_to_target_id']['field'] = 'activity_assigned_to_target_id';
  $handler->display->display_options['relationships']['activity_assigned_to_target_id']['required'] = 0;
  /* Field: Activity: Activity ID */
  $handler->display->display_options['fields']['id']['id'] = 'id';
  $handler->display->display_options['fields']['id']['table'] = 'party_activity';
  $handler->display->display_options['fields']['id']['field'] = 'id';
  $handler->display->display_options['fields']['id']['label'] = '';
  $handler->display->display_options['fields']['id']['exclude'] = TRUE;
  $handler->display->display_options['fields']['id']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['id']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['id']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['id']['alter']['external'] = 0;
  $handler->display->display_options['fields']['id']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['id']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['id']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['id']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['id']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['id']['alter']['more_link'] = 0;
  $handler->display->display_options['fields']['id']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['id']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['id']['alter']['html'] = 0;
  $handler->display->display_options['fields']['id']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['id']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['id']['hide_empty'] = 0;
  $handler->display->display_options['fields']['id']['empty_zero'] = 0;
  $handler->display->display_options['fields']['id']['hide_alter_empty'] = 1;
  $handler->display->display_options['fields']['id']['separator'] = '';
  $handler->display->display_options['fields']['id']['format_plural'] = 0;
  /* Field: Activity: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'party_activity';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['title']['alter']['make_link'] = 1;
  $handler->display->display_options['fields']['title']['alter']['path'] = 'activity/[id]';
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
  $handler->display->display_options['fields']['title']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['title']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['title']['hide_empty'] = 0;
  $handler->display->display_options['fields']['title']['empty_zero'] = 0;
  $handler->display->display_options['fields']['title']['hide_alter_empty'] = 1;
  /* Field: Activity: Date */
  $handler->display->display_options['fields']['activity_date']['id'] = 'activity_date';
  $handler->display->display_options['fields']['activity_date']['table'] = 'field_data_activity_date';
  $handler->display->display_options['fields']['activity_date']['field'] = 'activity_date';
  $handler->display->display_options['fields']['activity_date']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['activity_date']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['activity_date']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['activity_date']['alter']['external'] = 0;
  $handler->display->display_options['fields']['activity_date']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['activity_date']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['activity_date']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['activity_date']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['activity_date']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['activity_date']['alter']['more_link'] = 0;
  $handler->display->display_options['fields']['activity_date']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['activity_date']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['activity_date']['alter']['html'] = 0;
  $handler->display->display_options['fields']['activity_date']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['activity_date']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['activity_date']['hide_empty'] = 0;
  $handler->display->display_options['fields']['activity_date']['empty_zero'] = 0;
  $handler->display->display_options['fields']['activity_date']['hide_alter_empty'] = 1;
  $handler->display->display_options['fields']['activity_date']['settings'] = array(
    'format_type' => 'short',
    'fromto' => 'both',
    'multiple_number' => '',
    'multiple_from' => '',
    'multiple_to' => '',
  );
  $handler->display->display_options['fields']['activity_date']['field_api_classes'] = 0;
  /* Field: Activity: Participants */
  $handler->display->display_options['fields']['activity_participants']['id'] = 'activity_participants';
  $handler->display->display_options['fields']['activity_participants']['table'] = 'field_data_activity_participants';
  $handler->display->display_options['fields']['activity_participants']['field'] = 'activity_participants';
  $handler->display->display_options['fields']['activity_participants']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['activity_participants']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['activity_participants']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['activity_participants']['alter']['external'] = 0;
  $handler->display->display_options['fields']['activity_participants']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['activity_participants']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['activity_participants']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['activity_participants']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['activity_participants']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['activity_participants']['alter']['more_link'] = 0;
  $handler->display->display_options['fields']['activity_participants']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['activity_participants']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['activity_participants']['alter']['html'] = 0;
  $handler->display->display_options['fields']['activity_participants']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['activity_participants']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['activity_participants']['hide_empty'] = 0;
  $handler->display->display_options['fields']['activity_participants']['empty_zero'] = 0;
  $handler->display->display_options['fields']['activity_participants']['hide_alter_empty'] = 1;
  $handler->display->display_options['fields']['activity_participants']['settings'] = array(
    'link' => 1,
  );
  $handler->display->display_options['fields']['activity_participants']['group_rows'] = 1;
  $handler->display->display_options['fields']['activity_participants']['delta_offset'] = '0';
  $handler->display->display_options['fields']['activity_participants']['delta_reversed'] = 0;
  $handler->display->display_options['fields']['activity_participants']['delta_first_last'] = 0;
  $handler->display->display_options['fields']['activity_participants']['field_api_classes'] = 0;
  /* Contextual filter: Party: Party ID */
  $handler->display->display_options['arguments']['pid']['id'] = 'pid';
  $handler->display->display_options['arguments']['pid']['table'] = 'party';
  $handler->display->display_options['arguments']['pid']['field'] = 'pid';
  $handler->display->display_options['arguments']['pid']['relationship'] = 'activity_participants_target_id';
  $handler->display->display_options['arguments']['pid']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['pid']['default_argument_skip_url'] = 0;
  $handler->display->display_options['arguments']['pid']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['pid']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['pid']['summary_options']['items_per_page'] = '25';
  $handler->display->display_options['arguments']['pid']['break_phrase'] = 0;
  $handler->display->display_options['arguments']['pid']['not'] = 0;

  /* Display: Assigned Activities */
  $handler = $view->new_display('panel_pane', 'Assigned Activities', 'panel_pane_1');
  $handler->display->display_options['defaults']['arguments'] = FALSE;
  /* Contextual filter: Party: Party ID */
  $handler->display->display_options['arguments']['pid']['id'] = 'pid';
  $handler->display->display_options['arguments']['pid']['table'] = 'party';
  $handler->display->display_options['arguments']['pid']['field'] = 'pid';
  $handler->display->display_options['arguments']['pid']['relationship'] = 'activity_assigned_to_target_id';
  $handler->display->display_options['arguments']['pid']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['pid']['default_argument_skip_url'] = 0;
  $handler->display->display_options['arguments']['pid']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['pid']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['pid']['summary_options']['items_per_page'] = '25';
  $handler->display->display_options['arguments']['pid']['break_phrase'] = 0;
  $handler->display->display_options['arguments']['pid']['not'] = 0;
  $handler->display->display_options['argument_input'] = array(
    'pid' => array(
      'type' => 'context',
      'context' => 'entity:party.pid',
      'context_optional' => 0,
      'panel' => '0',
      'fixed' => '',
      'label' => 'Party: Party ID',
    ),
  );

  /* Display: Participating Activities */
  $handler = $view->new_display('panel_pane', 'Participating Activities', 'panel_pane_2');
  $handler->display->display_options['defaults']['relationships'] = FALSE;
  /* Relationship: Entity Reference: Referenced Entity */
  $handler->display->display_options['relationships']['activity_participants_target_id']['id'] = 'activity_participants_target_id';
  $handler->display->display_options['relationships']['activity_participants_target_id']['table'] = 'field_data_activity_participants';
  $handler->display->display_options['relationships']['activity_participants_target_id']['field'] = 'activity_participants_target_id';
  $handler->display->display_options['relationships']['activity_participants_target_id']['required'] = 0;
  $handler->display->display_options['defaults']['arguments'] = FALSE;
  /* Contextual filter: Party: Party ID */
  $handler->display->display_options['arguments']['pid']['id'] = 'pid';
  $handler->display->display_options['arguments']['pid']['table'] = 'party';
  $handler->display->display_options['arguments']['pid']['field'] = 'pid';
  $handler->display->display_options['arguments']['pid']['relationship'] = 'activity_participants_target_id';
  $handler->display->display_options['arguments']['pid']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['pid']['default_argument_skip_url'] = 0;
  $handler->display->display_options['arguments']['pid']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['pid']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['pid']['summary_options']['items_per_page'] = '25';
  $handler->display->display_options['arguments']['pid']['break_phrase'] = 0;
  $handler->display->display_options['arguments']['pid']['not'] = 0;
  $views[] = $view;

  return $views;
}
