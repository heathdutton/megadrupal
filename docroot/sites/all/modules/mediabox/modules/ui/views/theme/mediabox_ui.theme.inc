<?php

/**
 * @file
 * Mediabox display theme inc.
 */

/**
 * @param $variables
 */
function template_preprocess_mediaboxui_view(&$variables) {
  $view = $variables['view'];
  $ids = array();

  // Identifier of mediabox is required -> sending it to template.
  foreach ($view->result as $item) {
    $ids[] = $item->id;
  }

  $variables['ids'] = $ids;
}

/**
 * @param $variables
 */
function template_preprocess_mediaboxui_grid_view(&$variables) {
  $view = $variables['view'];
  $result = $variables['rows'];
  $options = $variables['options'];

  $handler = $view->style_plugin;


  $rows = array();
  $row = 0;
  $fields = $handler->render_fields($result);

  foreach ($result as $delta => $record) {
    $rows[$row][$delta]['record'] = $record;
    $rows[$row][$delta]['id'] = $fields[$delta]['id'];
    // end of the row
    if (($delta + 1) % $options['columns'] == 0) {
      $row++;
    }
  }

  $variables['rows'] = $rows;
}
