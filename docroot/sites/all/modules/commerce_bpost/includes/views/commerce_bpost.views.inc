<?php

/**
 * @file
 * Views definitions for commerce_bpost module.
 */

/**
 * Implements hook_views_data().
 */
function commerce_bpost_views_data() {
  $data = array();
  $data['commerce_order']['bpost_pdf_icon'] = array(
    'title' => t('Display a pdf icon if label available'),
    'help' => t('Display a clickable icon to edit the order and see the label.'),
    'field' => array(
      'handler' => 'commerce_bpost_handler_bpost_pdf_icon',
    ),
  );
  return $data;
}

/**
 * Implements hook_views_plugins().
 */
function commerce_bpost_views_plugins() {
  $plugins =
    array(
        'access' => array(
          'commerce_bpost' => array(
            'title' => t('Commerce bpost access check'),
            'help' => t('This is a custom access plugin'),
            'handler' => 'commerce_bpost_access_plugin',
            'path' => drupal_get_path('module', 'commerce_bpost') . '/includes/views',
          ),
        ),
  );
  return $plugins;
}
