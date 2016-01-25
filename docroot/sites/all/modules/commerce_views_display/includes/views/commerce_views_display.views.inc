<?php

/**
 * @file
 * Defines the View Style Plugins
 */

/**
 * Implements hook_views_plugins().
 */
function commerce_views_display_views_plugins() {
  $path = drupal_get_path('module', 'commerce_views_display');
  return array(
    'style' => array(
      'commerce_views_display_plugin_style_add_to_cart_form' => array(
        'title' => t('Commerce Add to Cart'),
        'help' => t('Renders product rows as an add to cart form.'),
        'base' => array('commerce_product'),
        'handler' => 'commerce_views_display_plugin_style_add_to_cart_form',
        'path' => $path . '/includes/views/plugins',
        'theme path' => $path . '/theme',
        'theme file' => 'commerce_views_display.theme.inc',
        'theme' => 'commerce_views_display_view_add_to_cart_form',
        'uses row plugin' => TRUE,
        'uses row class' => TRUE,
        'uses fields' => TRUE,
        'uses options' => TRUE,
        'uses grouping' => TRUE,
        'type' => 'normal',
      ),
    ),
  );
}
