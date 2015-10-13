<?php
/**
 * @file
 * crm_core_profile_commerce_items.theme.inc
 */

/**
 * Themes custom commerce cart.
 */
function theme_crm_core_profile_commerce_cart($variables) {
  $table = $variables['table'];
  $caption = $table['#title'];
  // Preparing table header.
  $header = array();
  foreach (element_children($table['header']) as $key) {
    $header[$key]['data'] = $table['header'][$key];
  }
  unset($table['header']);
  $summary = $table['summary'];
  unset($table['summary']);
  // Preparing table rows.
  $rows = array();
  foreach (element_children($table) as $id) {
    $row = array();
    $row['data'][] = drupal_render($table[$id]['title']);
    $row['data'][] = drupal_render($table[$id]['amount']);
    $row['data'][] = drupal_render($table[$id]['quantity']);
    $row['data'][] = drupal_render($table[$id]['total']);
    $rows[] = $row;
  }
  // Generating table.
  $output = theme('table', array(
    'caption' => $caption,
    'header' => $header,
    'rows' => $rows,
  ));

  $output .= drupal_render($summary);

  return $output;
}
