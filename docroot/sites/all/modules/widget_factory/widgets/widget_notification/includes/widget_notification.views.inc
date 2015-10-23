<?php

function widget_notification_views_plugins() {

  $includes_path = drupal_get_path('module', 'widget_notification') . '/includes';

  $plugins = array(
    'module' => 'widget_notification',
    'style' => array(
      'widget_notification' => array(
        'type' => 'feed',
        'title' => t('Widget Notification'),
        'help' => t('Generates an Widget Notification JSON data from a view.'),
        'handler' => 'widget_notification_plugin_style_json_feed',
        'path' => $includes_path,
        'uses fields' => FALSE,
        'uses grouping' => FALSE,
        'uses row plugin' => TRUE,
        'uses options' => TRUE,
        'even empty' => TRUE,
      ),
    ),
    'row' => array(
      'widget_notification_fields' => array(
        'type' => 'feed',
        'title' => t('Widget Notification Fields'),
        'help' => t('Display fields as widget notification items.'),
        'handler' => 'widget_notification_plugin_row_json_fields',
        'path' => $includes_path,
        'uses options' => TRUE,
        'uses fields' => TRUE,
      ),
    ),
  );

  return $plugins;
}
