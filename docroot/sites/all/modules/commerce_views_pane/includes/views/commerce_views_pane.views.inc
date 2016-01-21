<?php

/**
 * Provides Views integration.
 */


/**
 * Implements hook_views_plugins().
 *
 * - 'uses hook commerce_checkout_pane_info': Set to TRUE to have the
 *      display included by commerce_checkout_panes().
 */
function commerce_views_pane_views_plugins() {
  $path = drupal_get_path('module', 'commerce_views_pane');

  return array(
    'display' => array(
      'commerce_checkout_pane' => array(
        'uses hook commerce_checkout_pane_info' => TRUE,
        'title' => t('Commerce checkout pane'),
        'admin' => t('Commerce checkout pane'),
        'help' => t('Is available as a checkout pane for Commerce checkout.'),
        'handler' => 'commerce_views_pane_plugin_display_checkout_pane',
        'path' =>  $path . '/includes/views/plugins',
        'theme path' => drupal_get_path('module', 'views') . '/theme',
        'theme' => 'views_view',
        'register theme' => FALSE,
        'use ajax' => FALSE,
        'use pager' => FALSE,
        'use more' => FALSE,
        'accept attachments' => TRUE,
        'help topic' => 'display-commerce-checkout-pane',
      ),
    ),
  );
}
