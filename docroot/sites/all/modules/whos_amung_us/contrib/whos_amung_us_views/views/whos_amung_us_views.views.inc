<?php

/**
 * @file
 * Provide views data and handler information for Whos.amung.us module.
 *
 * @author Francisco J. Cruz Romanos <grisendo@gmail.com>
 */

/**
 * Implements hook_views_data_alter().
 */
function whos_amung_us_views_views_data_alter(&$data) {

  $data['views']['whos_amung_us'] = array(
    'title' => t("Who's amung us"),
    'help' => t("Who's amung us"),
    'area' => array(
      'handler' => 'views_handler_area_whos_amung_us',
    ),
  );

}

/**
 * Implements hook_views_handlers().
 */
function whos_amung_us_views_views_handlers() {

  return array(
    'info' => array(
      'path' => drupal_get_path('module', 'whos_amung_us_views'),
    ),
    'handlers' => array(
      'views_handler_area_whos_amung_us' => array(
        'parent' => 'views_handler_area',
      ),
    ),
  );

}
