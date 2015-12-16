<?php

/**
 * @file
 * Theme functions.
 */

/**
 * Theme callback for the form table.
 */
function theme_colorized_gmap_table_form(&$variables) {
  $output = '';
  $form = $variables['form'];
  $rows = $form['settings'];
  $header = $form['#header'];
  $content = array(
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => array(),
  );

  foreach (element_children($rows) as $row_index) {
    $row = array();
    foreach (element_children($rows[$row_index]) as $col_index) {
      $row[] = drupal_render($rows[$row_index][$col_index]);
    }
    $content['#rows'][] = $row;
  }
  $output .= drupal_render($content);

  return $output;
}

/**
 * Theme callback for the image colorized gmap marker.
 */
function theme_colorized_gmap_image_marker(&$variables) {
  $element = $variables['element'];
  $output = '';
  if($element['fid']['#value'] != 0) {
    $output .= '<div class="multifield-thumbnail">';
    $output .= theme('image_style', array('style_name' => 'thumbnail', 'path' => file_load($element['fid']['#value'])->uri, 'getsize' => FALSE));
    $output .= drupal_render_children($element);
    $output .= '</div>';
  }
  return $output;
}