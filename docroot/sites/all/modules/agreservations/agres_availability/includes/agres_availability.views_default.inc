<?php

/**
 * @file
 * Default views for the Calendar module.
 */

/**
 * Set up so it can be used as an API to create default calendars for
 * specific date fields.
 *
 * Use variable_set() to establish criteria for default calendars.
 * Set the variable in custom modules or in settings.
 *
 * Example: Add a new default calendar to custom
 * calendars that are already configured:
 *
 * $options = variable_get('agres_availability_default_view_options', array());
 * $option = array(
 *   'name' => 'example_event',
 *   'description' => 'An example event calendar for the date field.',
 *   'path' => 'example_event',
 *   'types' => array('example_content_type'),
 *   'date_fields' => array('field_example_date'),
 *   );
 * $options[] = $option;
 * variable_set('agres_availability_default_view_options', $options);
 *
 */
function agres_availability_views_default_views() {
  $views = array();

  // Construct the default view with default options.
  $view = agres_availability_views_construct();
  $views[$view->name] = $view;

  // Then see if there are any custom calendars to be created
  // using variable_get().
  $agres_availability_options = variable_get('agres_availability_default_view_options', array());
  foreach ((array) $agres_availability_options as $agres_availability_option) {
    $view = agres_availability_views_construct($agres_availability_option);
    $views[$view->name] = $view;
  }
  return $views;
}

/**
 *
 * Construct a default calendar to match specified options.
 * Views calls it without options, so the basic
 * default view will use the default values.
 *
 * @param $options: an optional array of options to
 * create default calendars.
 *
 * Possible options include:
 * @param string $name:
 *   The view name, if empty, defaults to 'calendar'.
 * @param string $description:
 *   The view description, if empty, defaults to generic description.
 * @param string $path:
 *   The view url, if empty, defaults to 'calendar'.
 * @param array  $types:
 *   Array of content types to limit the calendar to those types.
 *   If empty, defaults to no type filter.
 * @param array  $date_fields:
 *   Date fields used to filter the calendar.
 *   If empty, defaults to array('changed') for node.changed.
 * @param array  $display_fields:
 *   Fields to display in the calendar.
 *   If empty, defaults to title and date fields.
 *
 * @return the default calendar array.
 */
function agres_availability_views_construct($options = NULL) {
  $name = NULL;
  $description = NULL;
  $path = NULL;
  $types = NULL;
  $date_fields = NULL;

  if (empty($options)) {
    $disabled = TRUE;
  }
  else {
    $disabled = FALSE;
    extract($options);
  }
  if (empty($name)) {
    $name = 'agres_availability';
  }
  if (empty($description)) {
    $description = 'availability calendar';
  }
  if (empty($path)) {
    $path = 'agres_availability';
  }
  if (empty($types)) {
    $types = array();
  }
  if (empty($date_fields)) {
    $date_fields = array('changed');
  }
  $colors = array();
  $date_link_type = '';
  foreach ($types as $type => $label) {
    $colors[0][$type] = '#ffffff';
    $date_link_type = $type;
  }

  $type = '';
  if (!empty($types) && is_array($types) && count($types) > 0 ) {
    $types = array_keys($types);
    $type = $types[0];
  }

  // Can handle core node timestamp fields or Field date fields.

  $aliases = array();
  foreach ($date_fields as $field_name) {
    if (substr($field_name, 0, 6) == 'field_') {
      $table = 'field_data_' . $field_name;
      $alias = $table . '.' . $field_name . '_value';
      $cck_field = TRUE;
    }
    else {
      $table = 'node';
      $alias = $table . '.' . $field_name;
      $cck_field = FALSE;
    }
    $aliases[] = $alias;
  }

$view = new view;
$view->name = 'agres_availability';
$view->description = 'A bookingcalendar';
$view->tag = 'agres_availability';
$view->base_table = 'node';
$view->human_name = 'Availability Calendar';
$view->core = 7;
$view->api_version = '3.0';
$view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

/* Display: Master */
$handler = $view->new_display('default', 'Master', 'default');
$handler->display->display_options['title'] = 'Availability Calendar';
$handler->display->display_options['link_display'] = 'page_1';
$handler->display->display_options['access']['type'] = 'perm';
$handler->display->display_options['access']['perm'] = 'view agreservations availability calendar';
$handler->display->display_options['cache']['type'] = 'none';
$handler->display->display_options['query']['type'] = 'views_query';
$handler->display->display_options['query']['options']['query_comment'] = FALSE;
$handler->display->display_options['exposed_form']['type'] = 'basic';
$handler->display->display_options['pager']['type'] = 'date_views_pager';
$handler->display->display_options['pager']['options']['date_id'] = 'month';
$handler->display->display_options['style_plugin'] = 'agres_availability_style';
$handler->display->display_options['style_options']['name_size'] = '3';
$handler->display->display_options['style_options']['mini'] = '0';
$handler->display->display_options['style_options']['with_weekno'] = '0';
$handler->display->display_options['style_options']['multiday_theme'] = '0';
$handler->display->display_options['style_options']['theme_style'] = '1';
$handler->display->display_options['style_options']['max_items'] = '0';
$handler->display->display_options['row_plugin'] = 'agres_availability_node';
/* Field: Content: Title */
$handler->display->display_options['fields']['title']['id'] = 'title';
$handler->display->display_options['fields']['title']['table'] = 'node';
$handler->display->display_options['fields']['title']['field'] = 'title';
$handler->display->display_options['fields']['title']['label'] = '';
$handler->display->display_options['fields']['title']['alter']['alter_text'] = 1;
$handler->display->display_options['fields']['title']['alter']['text'] = 'booked';
$handler->display->display_options['fields']['title']['alter']['make_link'] = 0;
$handler->display->display_options['fields']['title']['alter']['absolute'] = 0;
$handler->display->display_options['fields']['title']['alter']['external'] = 0;
$handler->display->display_options['fields']['title']['alter']['replace_spaces'] = 0;
$handler->display->display_options['fields']['title']['alter']['trim_whitespace'] = 0;
$handler->display->display_options['fields']['title']['alter']['nl2br'] = 0;
$handler->display->display_options['fields']['title']['alter']['word_boundary'] = 0;
$handler->display->display_options['fields']['title']['alter']['ellipsis'] = 0;
$handler->display->display_options['fields']['title']['alter']['more_link'] = 0;
$handler->display->display_options['fields']['title']['alter']['strip_tags'] = 0;
$handler->display->display_options['fields']['title']['alter']['trim'] = 0;
$handler->display->display_options['fields']['title']['alter']['html'] = 0;
$handler->display->display_options['fields']['title']['element_label_colon'] = FALSE;
$handler->display->display_options['fields']['title']['element_default_classes'] = 1;
$handler->display->display_options['fields']['title']['hide_empty'] = 0;
$handler->display->display_options['fields']['title']['empty_zero'] = 0;
$handler->display->display_options['fields']['title']['hide_alter_empty'] = 0;
$handler->display->display_options['fields']['title']['link_to_node'] = 1;
/* Field: Content: Booking Date */
$handler->display->display_options['fields']['field_agres_rdate']['id'] = 'field_agres_rdate';
$handler->display->display_options['fields']['field_agres_rdate']['table'] = 'field_data_field_agres_rdate';
$handler->display->display_options['fields']['field_agres_rdate']['field'] = 'field_agres_rdate';
$handler->display->display_options['fields']['field_agres_rdate']['exclude'] = TRUE;
$handler->display->display_options['fields']['field_agres_rdate']['alter']['alter_text'] = 0;
$handler->display->display_options['fields']['field_agres_rdate']['alter']['make_link'] = 0;
$handler->display->display_options['fields']['field_agres_rdate']['alter']['absolute'] = 0;
$handler->display->display_options['fields']['field_agres_rdate']['alter']['external'] = 0;
$handler->display->display_options['fields']['field_agres_rdate']['alter']['replace_spaces'] = 0;
$handler->display->display_options['fields']['field_agres_rdate']['alter']['trim_whitespace'] = 0;
$handler->display->display_options['fields']['field_agres_rdate']['alter']['nl2br'] = 0;
$handler->display->display_options['fields']['field_agres_rdate']['alter']['word_boundary'] = 1;
$handler->display->display_options['fields']['field_agres_rdate']['alter']['ellipsis'] = 1;
$handler->display->display_options['fields']['field_agres_rdate']['alter']['more_link'] = 0;
$handler->display->display_options['fields']['field_agres_rdate']['alter']['strip_tags'] = 0;
$handler->display->display_options['fields']['field_agres_rdate']['alter']['trim'] = 0;
$handler->display->display_options['fields']['field_agres_rdate']['alter']['html'] = 0;
$handler->display->display_options['fields']['field_agres_rdate']['element_label_colon'] = 1;
$handler->display->display_options['fields']['field_agres_rdate']['element_default_classes'] = 1;
$handler->display->display_options['fields']['field_agres_rdate']['hide_empty'] = 0;
$handler->display->display_options['fields']['field_agres_rdate']['empty_zero'] = 0;
$handler->display->display_options['fields']['field_agres_rdate']['hide_alter_empty'] = 0;
$handler->display->display_options['fields']['field_agres_rdate']['settings'] = array(
  'format_type' => 'long',
  'fromto' => 'both',
  'multiple_number' => '',
  'multiple_from' => '',
  'multiple_to' => '',
  'show_repeat_rule' => 'show',
);
$handler->display->display_options['fields']['field_agres_rdate']['field_api_classes'] = 0;
/* Contextual filter: Date: Date (node) */
$handler->display->display_options['arguments']['date_argument']['id'] = 'date_argument';
$handler->display->display_options['arguments']['date_argument']['table'] = 'node';
$handler->display->display_options['arguments']['date_argument']['field'] = 'date_argument';
$handler->display->display_options['arguments']['date_argument']['default_action'] = 'default';
$handler->display->display_options['arguments']['date_argument']['default_argument_skip_url'] = 0;
$handler->display->display_options['arguments']['date_argument']['summary']['format'] = 'default_summary';
$handler->display->display_options['arguments']['date_argument']['granularity'] = 'month';
$handler->display->display_options['arguments']['date_argument']['date_fields'] = array(
  'field_data_field_agres_rdate.field_agres_rdate_value' => 'field_data_field_agres_rdate.field_agres_rdate_value',
  'field_data_field_agres_rdate.field_agres_rdate_value2' => 'field_data_field_agres_rdate.field_agres_rdate_value2',
);
/* Filter criterion: Content: Published */
$handler->display->display_options['filters']['status']['id'] = 'status';
$handler->display->display_options['filters']['status']['table'] = 'node';
$handler->display->display_options['filters']['status']['field'] = 'status';
$handler->display->display_options['filters']['status']['value'] = 1;
$handler->display->display_options['filters']['status']['group'] = 0;
$handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;

/* Display: Year */
$handler = $view->new_display('page', 'Year', 'page');
$handler->display->display_options['defaults']['pager'] = FALSE;
$handler->display->display_options['pager']['type'] = 'date_views_pager';
$handler->display->display_options['pager']['options']['date_id'] = 'year';
$handler->display->display_options['defaults']['style_plugin'] = FALSE;
$handler->display->display_options['style_plugin'] = 'agres_availability_style';
$handler->display->display_options['style_options']['name_size'] = '1';
$handler->display->display_options['style_options']['mini'] = '0';
$handler->display->display_options['style_options']['with_weekno'] = '0';
$handler->display->display_options['style_options']['multiday_theme'] = '1';
$handler->display->display_options['style_options']['theme_style'] = '1';
$handler->display->display_options['style_options']['max_items'] = '0';
$handler->display->display_options['style_options']['agres_availability_type'] = 'year';
$handler->display->display_options['defaults']['style_options'] = FALSE;
$handler->display->display_options['defaults']['row_plugin'] = FALSE;
$handler->display->display_options['row_plugin'] = 'agres_availability_node';
$handler->display->display_options['defaults']['row_options'] = FALSE;
$handler->display->display_options['defaults']['arguments'] = FALSE;
/* Contextual filter: Date: Date (node) */
$handler->display->display_options['arguments']['date_argument']['id'] = 'date_argument';
$handler->display->display_options['arguments']['date_argument']['table'] = 'node';
$handler->display->display_options['arguments']['date_argument']['field'] = 'date_argument';
$handler->display->display_options['arguments']['date_argument']['default_action'] = 'default';
$handler->display->display_options['arguments']['date_argument']['default_argument_skip_url'] = 0;
$handler->display->display_options['arguments']['date_argument']['summary']['format'] = 'default_summary';
$handler->display->display_options['arguments']['date_argument']['granularity'] = 'year';
$handler->display->display_options['arguments']['date_argument']['add_delta'] = 'yes';
$handler->display->display_options['arguments']['date_argument']['date_fields'] = array(
  'field_data_field_agres_rdate.field_agres_rdate_value' => 'field_data_field_agres_rdate.field_agres_rdate_value',
  'field_data_field_agres_rdate.field_agres_rdate_value2' => 'field_data_field_agres_rdate.field_agres_rdate_value2',
);
$handler->display->display_options['path'] = 'agres_availability/year';
$handler->display->display_options['menu']['type'] = 'tab';
$handler->display->display_options['menu']['title'] = 'Year';
$handler->display->display_options['menu']['weight'] = '4';
$handler->display->display_options['menu']['name'] = 'main-menu';
$handler->display->display_options['menu']['context'] = 0;

/* Display: Block */
$handler = $view->new_display('block', 'Block', 'block_1');
$handler->display->display_options['defaults']['pager'] = FALSE;
$handler->display->display_options['pager']['type'] = 'date_views_pager';
$handler->display->display_options['pager']['options']['date_id'] = 'mini';
$handler->display->display_options['pager']['options']['link_format'] = 'clean';
$handler->display->display_options['defaults']['style_plugin'] = FALSE;
$handler->display->display_options['style_plugin'] = 'agres_availability_style';
$handler->display->display_options['style_options']['name_size'] = '3';
$handler->display->display_options['style_options']['mini'] = '1';
$handler->display->display_options['style_options']['with_weekno'] = '0';
$handler->display->display_options['style_options']['multiday_theme'] = '0';
$handler->display->display_options['style_options']['theme_style'] = '1';
$handler->display->display_options['style_options']['max_items'] = '0';
$handler->display->display_options['defaults']['style_options'] = FALSE;
$handler->display->display_options['defaults']['row_plugin'] = FALSE;
$handler->display->display_options['row_plugin'] = 'agres_availability_node';
$handler->display->display_options['defaults']['row_options'] = FALSE;
$handler->display->display_options['defaults']['arguments'] = FALSE;
/* Contextual filter: Date: Date (node) */
$handler->display->display_options['arguments']['date_argument']['id'] = 'date_argument';
$handler->display->display_options['arguments']['date_argument']['table'] = 'node';
$handler->display->display_options['arguments']['date_argument']['field'] = 'date_argument';
$handler->display->display_options['arguments']['date_argument']['default_action'] = 'default';
$handler->display->display_options['arguments']['date_argument']['default_argument_skip_url'] = 0;
$handler->display->display_options['arguments']['date_argument']['summary']['format'] = 'default_summary';
$handler->display->display_options['arguments']['date_argument']['add_delta'] = 'yes';
$handler->display->display_options['arguments']['date_argument']['date_fields'] = array(
  'field_data_field_agres_rdate.field_agres_rdate_value' => 'field_data_field_agres_rdate.field_agres_rdate_value',
  'field_data_field_agres_rdate.field_agres_rdate_value2' => 'field_data_field_agres_rdate.field_agres_rdate_value2',
);

/* Display: Upcoming */
$handler = $view->new_display('block', 'Upcoming', 'block_2');
$handler->display->display_options['display_description'] = 'Upcoming events block';
$handler->display->display_options['defaults']['pager'] = FALSE;
$handler->display->display_options['pager']['type'] = 'some';
$handler->display->display_options['pager']['options']['items_per_page'] = '5';
$handler->display->display_options['pager']['options']['offset'] = '0';
$handler->display->display_options['defaults']['style_plugin'] = FALSE;
$handler->display->display_options['style_plugin'] = 'list';
$handler->display->display_options['defaults']['style_options'] = FALSE;
$handler->display->display_options['defaults']['row_plugin'] = FALSE;
$handler->display->display_options['row_plugin'] = 'fields';
$handler->display->display_options['defaults']['row_options'] = FALSE;
$handler->display->display_options['defaults']['filters'] = FALSE;
/* Filter criterion: Content: Published */
$handler->display->display_options['filters']['status']['id'] = 'status';
$handler->display->display_options['filters']['status']['table'] = 'node';
$handler->display->display_options['filters']['status']['field'] = 'status';
$handler->display->display_options['filters']['status']['value'] = 1;
$handler->display->display_options['filters']['status']['group'] = 0;
$handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
/* Filter criterion: Date: Date (node) */
$handler->display->display_options['filters']['date_filter']['id'] = 'date_filter';
$handler->display->display_options['filters']['date_filter']['table'] = 'node';
$handler->display->display_options['filters']['date_filter']['field'] = 'date_filter';
$handler->display->display_options['filters']['date_filter']['operator'] = '>=';
$handler->display->display_options['filters']['date_filter']['default_date'] = 'now';
$handler->display->display_options['filters']['date_filter']['date_fields'] = array(
  'field_data_field_agres_rdate.field_agres_rdate_value' => 'field_data_field_agres_rdate.field_agres_rdate_value',
);

/* Display: Block 3 */
$handler = $view->new_display('block', 'Block 3', 'block_3');

/* Display: Date browser */
$handler = $view->new_display('date_nav', 'Date browser', 'date_nav_1');

/* Display: Month */
$handler = $view->new_display('page', 'Month', 'page_1');
$handler->display->display_options['path'] = 'agres_availability/month';
$handler->display->display_options['menu']['type'] = 'default tab';
$handler->display->display_options['menu']['title'] = 'Availability';
$handler->display->display_options['menu']['description'] = 'Availability';
$handler->display->display_options['menu']['weight'] = '0';
$handler->display->display_options['menu']['context'] = 0;
$handler->display->display_options['tab_options']['type'] = 'normal';
$handler->display->display_options['tab_options']['title'] = 'Availability';
$handler->display->display_options['tab_options']['weight'] = '0';
$handler->display->display_options['tab_options']['name'] = 'main-menu';
$translatables['agres_availability'] = array(
  t('Master'),
  t('Availability Calendar'),
  t('more'),
  t('Apply'),
  t('Reset'),
  t('Sort by'),
  t('Asc'),
  t('Desc'),
  t('booked'),
  t('Booking Date'),
  t('All'),
  t('Year'),
  t('Block'),
  t('Upcoming'),
  t('Upcoming events block'),
  t('Block 3'),
  t('Date browser'),
  t('Month'),
);

  return $view;
}
