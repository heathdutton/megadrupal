<?php

function widget_notification_views_plugins() {

  $includes_path = drupal_get_path('module', 'widget_notification') . '/includes';

  $plugins = array(
    'module' => 'widget_notification',
    'display' => array(
      'widget_notification' => array(
        'title' => t('Widget Notification'),
        'help' => t('Display the view as a widget notification JSON feed.'),
        'handler' => 'widget_notification_plugin_display_json_feed',
        'path' => $includes_path,
        'uses hook menu' => TRUE,
        'use ajax' => FALSE,
        'use pager' => TRUE,
        'accept attachments' => FALSE,
        'admin' => t('Widget Notification Feed'),
      ),
    ),
    'style' => array(
      'widget_notification' => array(
        'type' => 'widget_notification',
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
        'type' => 'widget_notification',
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
