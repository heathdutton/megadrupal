<?php

/**
 * @file
 * Views plugins for mm_schedule.
 */

/**
 * Implements hook_views_plugins
 */
function mm_schedule_views_plugins() {
  $sched_path = drupal_get_path('module', 'mm_schedule');
  $views_path = $sched_path . '/views';
  $theme_path = $sched_path . '/theme';
  $data = array(
    'module' => 'calendar',
    'style' => array(
      // Define a style class for <dl> lists
      'mm_schedule_list' => array(
        'title' => t('Date List (MM)'),
        'help' => t('Generates a list of dates from a view.'),
        'handler' => 'mm_schedule_calendar_plugin_style_list',
        'path' => $views_path,
        'theme' => 'mm_schedule_calendar_style_list',
        'theme file' => 'mm_schedule_theme.inc',
        'theme path' => $theme_path,
        'parent' => 'calendar_plugin_style',
        'uses fields' => TRUE,
        'uses row plugin' => TRUE,
        'uses options' => FALSE,
        'type' => 'normal',
        'even empty' => FALSE,
      ),
    ),
    'row' => array(
      'mm_calendar_entity' => array(
        'title' => t('Calendar Entity List (MM)'),
        'help' => t('Displays each selected entity as a row in an indented list.'),
        'handler' => 'mm_schedule_calendar_plugin_row',
        'parent' => 'calendar_plugin_row',
        'path' => $views_path,
        'theme' => 'mm_schedule_calendar_plugin_row',
        'theme file' => 'mm_schedule_theme.inc',
        'theme path' => $theme_path,
        'base' => array_keys(date_views_base_tables()),
        'uses options' => TRUE,
        'uses fields' => TRUE,
        'type' => 'normal',
      ),
    ),
  );
  return $data;
}

/**
 * Implements hook_views_plugins_alter().
 */
function mm_schedule_views_plugins_alter(&$data) {
  // If the views_rss module is installed, allow its "feed" style to act on
  // attachments, and not just feeds. This allows the merged subquery
  // attachments to use this style plugin.
  if (isset($data['style']['rss_fields'])) {
    $data['style']['mm_rss_fields'] = $data['style']['rss_fields'];
    $data['style']['mm_rss_fields']['type'] = 'normal';
  }
  // This allows the RSS feed display (which is like an attachment) to itself
  // accept attachments for the subqueries.
  if (isset($data['display']['feed'])) {
    $data['display']['feed']['accept attachments'] = TRUE;
  }
  // If the date iCal plugin is installed, duplicate the date_ical row style,
  // and allow it to accept fields so that subqueries can work.
  if (isset($data['row']['date_ical'])) {
    $data['row']['mm_date_ical'] = $data['row']['date_ical'];
    $data['row']['mm_date_ical']['uses fields'] = TRUE;
    $data['row']['mm_date_ical']['title'] .= ' (MM)';
    $data['row']['mm_date_ical']['name'] = 'mm_date_ical';
  }
}

/**
 * Implements hook_views_data_alter().
 */
function mm_schedule_views_data_alter(&$data) {
  // Override date_views_argument_handler with a version that understands how
  // to shift arguments to account for the starting month setting.
  foreach ($data as &$field) {
    if (!empty($field['date_argument']['argument']['handler']) && $field['date_argument']['argument']['handler'] == 'date_views_argument_handler') {
      $field['date_argument']['argument']['handler'] = 'mm_schedule_date_views_argument_handler';
    }
  }
}
