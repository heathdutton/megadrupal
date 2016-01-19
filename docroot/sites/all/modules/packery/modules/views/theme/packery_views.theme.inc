<?php

/**
 * @file
 * packery_views.theme.inc
 */

/**
 * Template process Packery variables.
 */
function template_process_packery_views(&$variables) {
  if (!empty($variables['view'])) {
    $settings = $variables['options'];
    $view = $variables['view'];

    $items = array();
    $row_plugin = $view->style_plugin->row_plugin;

    foreach ($variables['rows'] as $id => $row) {
      $view->row_index = $id;
      $item['row'] = $row_plugin->render($row);

      $items[] = $item;
    }

    // Row items & instance settings.
    $variables['settings'] = $settings;
    $variables['items'] = $items;
  }
}

/**
 * Theme declaration for Packery.
 */
function theme_packery_views($variables) {
  $output = array(
    '#theme' => 'packery',
    '#items' => $variables['items'],
    '#settings' => $variables['settings']
  );

  return drupal_render($output);
}
