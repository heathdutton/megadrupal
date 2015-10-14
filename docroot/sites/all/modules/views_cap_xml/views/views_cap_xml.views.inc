<?php

function views_cap_xml_views_plugins() {
  return array(
    'style' => array(
      'cap_xml' => array(
        'title' => t('CAP XML'),
        'path' => drupal_get_path('module', 'views_cap_xml') . '/views',
        'handler' => 'views_plugin_style_cap_xml',
        'theme' => 'views_style_cap_xml',
        'theme path' => drupal_get_path('module', 'views_cap_xml') . '/views',
        'uses row plugin' => TRUE,
        'uses fields' => TRUE,
        'uses options' => TRUE,
        'type' => 'feed',
      ),
    ),

    'row' => array(
      'cap_xml' => array(
        'title' => t('CAP XML Fields'),
        'help' => t('Display fields as CAP XML elements.'),
        'path' => drupal_get_path('module', 'views_cap_xml') . '/views',
        'handler' => 'views_plugin_row_cap_xml',
        'theme' => 'views_row_cap_xml',
        'theme path' => drupal_get_path('module', 'views_cap_xml') . '/views',
        'uses fields' => TRUE,
        'uses options' => TRUE,
        'type' => 'feed',
      ),
    )

  );

}